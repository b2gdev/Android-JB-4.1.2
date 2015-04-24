/*************
 *
 * Filename:    mc77xximgmgm.c
 *
 * Purpose:     MC77xx Image Management application
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "SWIWWANCMAPI.h"
#include <stdlib.h>
#include <getopt.h>
#define  LOG_TAG  "swi_imgmgr"
#include "swiril_log.h"
#include "im/imudefs.h"
#include "qm/qmerrno.h"
#include <swiqmitool_common.h>

/****************************************************************
*                       DEFINES
****************************************************************/
#define OPTION_LEN         4
#define SUCCESS            0

/****************************************************************
*                       DATA STRUCTURES
****************************************************************/

/* firmware download */
static swi_bool fwDwlComplete = FALSE;
static swi_bool verbose = FALSE;

/* Command Line Options */
const char * const short_options = "h?vgd:i:";

const struct option long_options[] = {
    {"help",    0, NULL, 'h'},  /* Provides terse help to users */
    {"help",    0, NULL, '?'},  /* Provides terse help to users */
    {"verbose", 0, NULL, 'v'},  /* Run in Verbose mode */
    {"info",    1, NULL, 'i'},  /* Display the information for the executing device image */
    {"download",1, NULL, 'd'},  /* Download an image to the device */
    {NULL,      0, NULL, 0  }   /* End of list */
};

/****************************************************************
*                       FUNCTIONS
****************************************************************/
/**************
 *
 * Name:     printUsage
 *
 * Purpose:  Print the usage information for this executable
 *           
 * Parms:    programname - name of the program
 *
 * Return:   None
 *
 * Notes:    This program can be invoked with the following options:
 *
 *           -h               Print usage information and exit cleanly
 *           -?               Print usage information and exit cleanly
 *           -v               Verbose mode. 
 *           -g               Display the information for the executing
 *                            device image.
 *           -d <imagepath>   Use <imagepath> to specific the CWE image path
 *                            to be download to the device.
 *                            Note that the path is just the directory
 *                            where the firmware resides.
 *           -i <imagepath>   Display the information for a particular CWE
 *                            image file located at <CWEimagepath>.
 *                            Note that the path is just the directory
 *                            where the firmware resides.
 * 
 *
 **************/
local void printUsage( char *programname )
{
    printf("\n\nUsage: %s:\n", programname );
    printf("  -h  --help                        Display this information and exit\n" );
    printf("  -?  --help                        Display this information and exit\n" );
    printf("  -v  --verbose                     Display extra info while running\n");
    printf("                                    NOTE: specify 'verbose' first\n");
    printf("  -g  --get                         Display the information for \n");
    printf("                                    the executing device image\n");
    printf("  -d  --download <CWEimagepath>     CWE image to be downloaded is \n");
    printf("                                    at <CWEimagepath>\n");    
    printf("                                    NOTE: this must be an absolute path\n");
    printf("  -i  --info <CWEimagepath>         Display the information for \n");
    printf("                                    a particular CWE image file located\n");
    printf("                                    at <CWEimagepath>\n");
    printf("                                    NOTE: this must be an absolute path\n");
    printf("\n");
}

/*************
 * Name:     GetImagePath
 *
 * Purpose:  Receives the image path located at host from the user,validates it
 *           and retrieve the information from the image.
 *           
 * Parms:    pImagePath - memory location to place pointer to the image 
 *           pFwInfo    - Information of the firware
 *
 * Return:   None
 *
 * Notes:    None
 **************/
void GetImagePath(
    char                *pImagePath,
    struct qmifwinfo_s *pFwInfo)
{
    WORD  len = 0;
    ULONG resultCode = 0;
    char  *pLocalPath;

    LOGD("%s Entered. with pImagePath: %s\n", __func__, pImagePath);

    if(verbose == TRUE)
        fprintf( stderr,  "Image Path: %s\n", pImagePath );

    len = strlen( pImagePath );
    if( pImagePath[len - 1] != '/' )
        asprintf(&pLocalPath, "%s%s", pImagePath, "/");
    else
        asprintf(&pLocalPath, "%s", pImagePath);

    LOGD("%s pImagePath: %s\n", __func__, pLocalPath);

    /* Get the information about the image located at specified path */
    resultCode = SLQSGetImageInfo( pLocalPath,
                                   pFwInfo );
    LOGD("%s SLQSGetImageInfo return: %lu\n", __func__, resultCode);
    /* If we fail to retrieve the Image information */
    if( SUCCESS != resultCode )
    {
        
        LOGE("%s Failed to retrieve Image Info,Failure Code: %lu ", __func__, resultCode);
        fprintf( stderr, "Failed to retrieve Image Info\n"\
                         "Failure Code: %lu\n", resultCode );
    }
    free(pLocalPath);

}

/*************
 * Name:     DevStateChgCbk
 *
 * Purpose:  Device State change callback
 *
 * Parms:    devstatus    - Call back status
 *
 * Return:   None
 *
 * Notes:    None
 **************/
void DevStateChgCbk(eDevState devstatus)
{
    LOGD("%s Device State %d", __func__, devstatus );
}

/*************
 * Name:     FirmwareDwlCbk
 *
 * Purpose:  Firmware download completion callback
 *
 *
 * Parms:    status    - Call back status
 *
 * Return:   None
 *
 * Notes:    None
 **************/
void FirmwareDwlCbk(ULONG status)
{
    if (eQCWWAN_ERR_NONE == status) {
        LOGI("Firmware Download Completed");
    } else {
        LOGE( "Firmware Not Downloaded");
    }
    /* set firmware complete to true */
    fwDwlComplete = TRUE;

    /* Unsubscribe from the callback */
    SetFwDldCompletionCbk(NULL);
}

/*************
 * Name:     DisplayImageInfo
 *
 * Purpose:  Print the information(passed as input) of the firmware image.
 *
 * Parms:    pin    - Information of the firware
 *   
 * Return:   None
 *
 * Notes:    None
 **************/
void DisplayImageInfo( struct slqsfwinfo_s *pin )
{
    fprintf( stderr,
             "Model ID: %s\nBoot image Version: %s\nApplication image Version: %s\n"\
             "SKU ID: %s\nPackage ID: %s\nCarrier: %s\nPRI version: %s\n",
             pin->modelid_str,  pin->bootversion_str,   pin->appversion_str, 
             pin->sku_str,      pin->packageid_str,     pin->carrier_str,
             pin->priversion_str  );
 }

/*************
 * Name:     GetDeviceImageInfo
 *
 * Purpose:  Get the information about the image running on the device.
 *           
 * Parms:    None    
 *                       
 * Return:   None
 *
 * Notes:    None
 **************/
void GetDeviceImageInfo()
{
    struct qmifwinfo_s fwInfo;
    ULONG               resultCode = SUCCESS;

    /* Get the information about the image loaded on the device */
    resultCode = SLQSGetFirmwareInfo( &fwInfo );
    LOGD("%s SLQSGetFirmwareInfo reutrn %lu", __func__, resultCode);
    /* If we fail to retrieve the Image information, return */
    if( SUCCESS != resultCode )
    {
        LOGE("%s Failed to retrieve Device Image Info, Failure Code: %lu", __func__, resultCode);
        return;
    }

    /* Display the retrieved image information */
    DisplayImageInfo( &(fwInfo.dev.s) );
}

/*************
 * Name:     FirmwareDownloader
 *
 * Purpose:  Download a CWE image to the device
 *
 * Parms:    pathnamepp    - memory location to place pointer to the image 
 *
 * Return:   None
 *
 * Notes:    None
 **************/
void FirmwareDownloader(char *pathnamep)
{
    struct qmifwinfo_s fwInfo;
    ULONG               resultCode = SUCCESS;
    char                *pStrPtr = NULL;
    char                appVersion[SLQSFWINFO_APPVERSION_SZ] = "";

    LOGD("%s Entered. with pathnamep: %s\n", __func__, pathnamep);
    
    memset(&fwInfo, 0, sizeof(fwInfo));
    /* Reset the firmware download completion flag at the begining */
    fwDwlComplete = FALSE;

    /* Receive the path of the image from the user */
    GetImagePath( pathnamep, &fwInfo );
    if(strlen(fwInfo.dev.s.modelid_str) == 0){
        LOGE("%s Failed to retrieve path: %s Image Inf \n", __func__, pathnamep);
        return; 
    }

    /* Save the application version locally */
    strcpy( appVersion, fwInfo.dev.s.appversion_str );

    /* Get the information about the image loaded on the device */
    resultCode = SLQSGetFirmwareInfo( &fwInfo );
    LOGD("%s SLQSGetFirmwareInfo return: %lu\n", __func__, resultCode);
    /* If we fail to retrieve the Image information, return */
    if( SUCCESS != resultCode )
    {
        LOGE("%s Failed to retrieve Device Image Info, Failure Code: %lu", __func__, resultCode);
        return;
    }

    /* Check if device has the same firmware image */
    pStrPtr = strstr( fwInfo.dev.s.appversion_str, appVersion );
    if( NULL != pStrPtr )
    {
        LOGE("%s Upgrade Not Required; Same Firmware image exist on Device", __func__);
        fprintf( stderr,  "Upgrade Not Required; Same Firmware image exist on Device\n" );
        return;
    }

    LOGD("%s Vaild firmware image found",__func__);

    /* Reaching here means that a CWE image was found at specified path */
    /* Subscribe to Device State Change callback */
    resultCode = SetDeviceStateChangeCbk(DevStateChgCbk);
    LOGD("%s SetDeviceStateChangeCbk return: %lu\n", __func__, resultCode);
    if( SUCCESS != resultCode )
    {
        LOGE("%s REGISTRATION FAILED - Device State Change Callback, Failure Code: %lu", __func__, resultCode);
        return;
    }

    /* Subscribe to Firmware Download completion callback */
    resultCode = SetFwDldCompletionCbk(FirmwareDwlCbk);
    LOGD("%s SetFwDldCompletionCbk return: %lu\n", __func__, resultCode);
    if( SUCCESS != resultCode )
    {
        LOGE("%s REGISTRATION FAILED - Firmware Download Completion Callback, Failure Code: %lu", __func__, resultCode);
        return;
    }

    /* Start downloading the firmware */
    resultCode = UpgradeFirmware2k( pathnamep );
    LOGD("%s UpgradeFirmware2k return: %lu\n", __func__, resultCode);
    if( SUCCESS != resultCode )
    {
        LOGE("%s Firmware Download Failed, Failure Code: %lu", __func__, resultCode);
        return;
    }

    /* Keep displaying "." until fimrware downloads complete */
    fprintf( stderr, "Downloading Firmware ");
    while( fwDwlComplete != TRUE )
    {
        fprintf( stderr, " .");
        sleep(1);
    }
    
    /* Get the information about the image loaded on the device */
    resultCode = SLQSGetFirmwareInfo( &fwInfo );
    LOGD("%s SLQSGetFirmwareInfo return: %lu\n", __func__, resultCode);

    /* If we fail to retrieve the Image information, return */
    if( SUCCESS != resultCode )
    {
        LOGE("%s Failed to retrieve Device Image Info, Failure Code: %lu", __func__, resultCode);
        return;
    }

    /* Check if device has the same firmware image */
    pStrPtr = strstr( fwInfo.dev.s.appversion_str, appVersion );
    if( NULL != pStrPtr )
    {
        fprintf( stderr,  "\nFirmware Update Success\n" );
    }
    else
    {
        fprintf( stderr,  "\nFirmware Update Failed\n" );
    }
    
}

/*************
 * Name:     GetHostImageInfo
 *
 * Purpose:  Get the information about the image located on host at a specified
 *           path.
 *
 * Parms:    pathnamepp    - memory location to place pointer to the image 
 *
 * Return:   None
 *
 * Notes:    none
 **************/
void GetHostImageInfo(char *pathnamep)
{
    struct qmifwinfo_s fwInfo;
    ULONG               resultCode = SUCCESS;
    
    LOGD("%s Entered. with pathnamep: %s\n", __func__, pathnamep);

    /* Clear the structure */
    memset(&fwInfo, 0, sizeof(fwInfo));
    /* Receive the path of the image from the user */
    GetImagePath( pathnamep, &fwInfo );

    if(strlen(fwInfo.dev.s.modelid_str) != 0)
        DisplayImageInfo( &(fwInfo.dev.s) );
}

/**************
 *
 * Name:     parseCommandLine
 *
 * Purpose:  This function parses the caller's inputed parameters.
 *           
 * Parms:    argc          - argument count
 *           argv          - argument vector
 *           pathnamepp    - memory location to place pointer to the image 
 *           fDownload     - memory location to place download flag
 *           fGetDeviceImgInfor     - memory location to place get device image info flag
 *
 * Return:   FALSE         - failed
 *           TRUE          - Succeed
 *
 * Notes:    None
 * 
 **************/
local swi_bool parseCommandLine(
    int argc, 
    char *argv[], 
    char **pathnamepp,
    swi_bool *fDownload,
    swi_bool *fGetDeviceImgInfor)
{
    int next_option;
    int optioncount = 0;
    char *programname;
    
    *pathnamepp    = NULL;

    /* Remember our own name */
    programname = argv[0]; 

    if(argc == 1)
    {
        printUsage(programname);
        exit( 0 );
    }        
    
    /* Parse the command line before doing anything else */
    do 
    {
        /* Read the next option until there are no more */
        next_option = getopt_long( argc, argv, 
                                   short_options, 
                                   long_options, NULL );

        switch( next_option )
        {
            case 'h':
                /* Print usage information */
                printUsage(programname);
                exit( 0 );
                break;

            case '?':
                /* Print usage information */
                printUsage(programname);
                exit( 0 );
                break;

            case 'g':
                *fGetDeviceImgInfor = TRUE;
                break;

            case 'i':
                /* caller specifies a pathname to the CWE image to download */
                *pathnamepp = optarg;
                *fDownload = FALSE;
                break;

            case 'd':
                /* caller specifies a pathname to the CWE image to download */
                *pathnamepp = optarg;
                *fDownload = TRUE;
                break;
 
            case 'v':
                /* Verbose mode on */
                verbose = TRUE;
                break;

            case -1:
                /* Done with options list */
                if( verbose )
                {
                    printf("%d command line options found\n", optioncount );
                    printf("verbose:     ON\n");
                    printf("imgpathname: %s\n", *pathnamepp );
                    printf("*fDownload: %d\n", *fDownload );
                    printf("*fGetDeviceImgInfor: %d\n", *fGetDeviceImgInfor );
                 TRUE;
                }

                break;

            default:
                return(FALSE);
                break;
        }
        optioncount++;
    }
    while( next_option != -1 );
    
    return(TRUE);
}

/*************
 * Name:     main
 *
 * Purpose:  Entry point of the application
 *
 * Return:   None
 *
 * Notes:    None
 **************/
int main( int argc, char *argv[] )
{
    ULONG    resultCode = 0;
    char     selOption[OPTION_LEN];
    char     *pathnamep = NULL;
    swi_bool fDownload = FALSE;
    swi_bool fGetDeviceImgInfor = FALSE;

    /* Parse the command line  */
    if(!parseCommandLine(argc, argv, &pathnamep, &fDownload, &fGetDeviceImgInfor))
    {
        exit(1);
    }

    qmiDeviceConnect();
    if(fGetDeviceImgInfor)
    {
        GetDeviceImageInfo();
    }
    else if(pathnamep!= NULL)
    {
        if(fDownload == TRUE)
            FirmwareDownloader(pathnamep);
        else
            GetHostImageInfo(pathnamep);
    }
    qmiDeviceDisconnect();
    exit(EXIT_SUCCESS);
}
