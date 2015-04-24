/**************
 *  Filename: qatest.c
 *
 *  Purpose:  This file contains routine(s) that exercise the
 *            API package.
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
  **************/
/* Linux definitions */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qatestproto.h"
#include "qmerrno.h"
#include "../../SWIWWANCMAPI.h"
#include "qaDcsQCWWAN2kEnumerateDevices.h"

/* global data */
#define QAMAXTESTS 100
#define DEFAULTTIMEOUT 3000
//#define QADEBUG

/* Running in verbose mode or not */
BOOL verbose = FALSE;

/* Log test outputs to file in directory TestReport or not */
BOOL blogtofile = FALSE;

/* Name of this program */
const char *programname;

/* Command line options, short form. Note, a ":"
 * character following an option indicates to
 * the operating system that that option takes
 * an argument(s).
 */
const char * const short_options = "hp:vt:mn:rf:s:l";

const struct option long_options[] = {
    {"help",     0, NULL, 'h'},      /* Provides terse help to users */
    {"pathname", 1, NULL, 'p'},     /* Specify the pathname */
    {"verbose",  0, NULL, 'v'},      /* Run in Verbose mode */
    {"timeout",  1, NULL, 't'},      /* Timeout to use for StopnWait calls */
    {"monitor",  0, NULL, 'm'},      /* Enable not'ns and monitor them */
    {"numtest",  1, NULL, 'n'},      /* Test number or list of comma separated
                                     * test numbers to run
                                     */
    {"logtofile",  0, NULL, 'l'},    /* Log test outputs to respective file */
    {"repeat",   0, NULL, 'r'},      /* Repeat test until ^C */
    {"file",     1, NULL, 'f'},      /* file name with list of tests to run */
    {"sequence", 2, NULL, 's'},      /* pick the sequence from the file */
    {NULL,       0, NULL,  0 }       /* End of list */
};

/* device connectivity */
static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];


/**************
 *
 * Name:     qaPrintusage
 *
 * Purpose:  Print the usage information for this executable
 *
 * Parms:    none
 *
 * Return:   none
 *
 * Notes:    This program can be invoked with the following options:
 *
 *           -h               Print usage information and exit cleanly
 *           -v               Verbose mode. This option should come before
 *                            any of the following for best results
 *           -p <sdkpath>     Use <sdkpath> to launch the SDK executable.
 *                            Note that the path must include the name of
 *                            the executable itself, not just the directory
 *                            where it resides. This option must always be
 *                            present; otherwise the application does not start.
 *           -t <timeout>     Timeout value to use when calling Stop and Wait
 *                            APIs (i.e. Get or Set routines). Specified in
 *                            milliseconds. The SDK won't run properly for
 *                            values that are too small. Recommend using
 *                            minimum 3000 for this value
 *           -m               Enable notifications and remain active,
 *                            monitoring incoming notifications from the modem
 *                            and displaying them to the console. Omitting this
 *                            option causes this program to terminate as soon as
 *                            the specified test case(s) has completed
 *           -n <x or test #> x or X displays cross-reference of test tags.
 *                            test # can be a single test tag, e.g., t10, or
 *                            a list of comma-separated test tags, e.g.,
 *                            t1,t10,t6
 *           -r --repeat      Repeat the specified test(s) until the user
 *                            terminates with ^C
 *           -l --logtofile   Log the test outputs to their respective files.
 *           -f <filename>    file name with list of test tags as in -n above.
 *           -s  <sequence>   Execute test sequence from the file
 *
 *           If the caller does not specify a path, this program will return
 *           an error and will not start any execution.
 *
 *
 **************/
local void qaprintusage( void )
{
    printf("\n\nUsage: %s:\n", programname );
    printf("   -h  --help                Display this information and exit\n" );
    printf("   -v  --verbose             Display extra info while running\n");
    printf("                             NOTE: specify 'verbose' first\n");
    printf("   -t  --timeout  <timeout>  timeout (ms) for Stop and Wait APIs\n");
    printf("   -p  --pathname <sdkpath>  SDK executable is at <sdkpath>\n");
    printf("                             NOTE: always specify pathname\n");
    printf("   -m  --monitor             Enable notifications and continue\n");
    printf("                             executing, displaying notification\n");
    printf("                             messages until stopped with ^C\n");
    printf("   -n  --numtest  <test #>   Test number or list of\n");
    printf("                             comma-separated test numbers to run\n");
    printf("                  <x>        List cross reference of test tag \n");
    printf("                             to API functions name\n");
    printf("   -r  --repeat              Repeat the test(s) until stopped\n" );
    printf("                             with ^C\n");
    printf("   -f  --file <filename>     file name with list of tests to run\n");
    printf("                             The file format is as below::\n");
    printf("                             Line which start with '#' has the\n");
    printf("                             sequence name like #startdatasession\n");
    printf("                             The sequence can be of the following\n");
    printf("                             formats\n");
    printf("                             \ttn-tm;\n");
    printf("                             \tta:tb:tc:td;\n");
    printf("                             \t[tn-tm]<repeat x>;\n");
    printf("                             \t[tn-tm]<delay x>;\n");
    printf("                             \t[tn<delay x>-tm];\n");
    printf("                             \t[tn<delay x>-tm]<repeat y>;\n");
    printf("                             \t[ta:tb:tc:td:te]<delay x>;\n");
    printf("                             \t[ta:tb:tc:td:te]<repeat x>;\n");
    printf("                             \t[ta:tb<delay y>:te]<repeat x>;\n");
    printf("                             \t[tn<delay x>-tm]:ta<repeat y>:tb;\n");
    printf("                             Any Combination of the above\n");
    printf("   -s  --testsequence        Execute test sequence from the file\n");
    printf("                             If --s is provided all sequences\n");
    printf("                             are executed\n");
    printf("                             If -f is not provided the sequence\n");
    printf("                             is picked from sequence.txt\n");
    printf("   -l --logtofile            Log the test outputs to their \n");
    printf("                             respective files.\n");
}

/**************
 *
 * Name:     qagettimeout
 *
 * Purpose:  Validate the timeout value passed on the command line
 *
 * Parms:    timeoutp  - Pointer to the string containing the timeout
 *           timevalp  - Pointer to storage to write the converted timeout
 *                       if the ASCII value contains valid digits
 *
 * Return:   TRUE  - A valid timeout value is returned in timevalp
 *           FALSE - The string contains invalid digits
 *
 * Notes:    none
 *
 **************/
local BOOL qagettimeout( char *timeoutp, unsigned int *timevalp )
{
    BOOL retval;
    int i, strlength;

    retval = FALSE;    /* Assume bad conversion */

    /* See how many chars to check */
    strlength = strlen( timeoutp );

    if( strlength )
    {
        for( i = 0; i<strlength; i++ )
        {
            /* One false digit and we're done */
            if( !( timeoutp[i] >= '0' && timeoutp[i] <= '9' ) )
                return FALSE;
        }
        /* If we get here, the timeoutp points to a valid ASCII number */
        *timevalp = atoi( timeoutp );
        retval = TRUE;
    }
    return retval;
}

/**************
 *
 * Name:     main
 *
 * Purpose:  Entry point where testing of the API package starts.
 *
 * Parms:    argc   - Not used
 *           argv[] - Not used
 * Return:   0 - Normal termination
 *           1 - Problem with the execution
 *
 * Notes:    see function qaprintusage()
 *
 **************/
int main( int argc, char *argv[] )
{
    int   next_option;
    int   optioncount = 0;
    unsigned int timeout;
    char  *timeoutp;
    char  *pathnamep=NULL;
    char  *testsp=NULL;         /* pointer to test number or list of... */
    char  *testsfilenamep=NULL; /* pointer to file containing list of tests */
    char  *testsequencename=NULL;
    char  *sdkversionp=NULL;
    BOOL  repeatest;             /* Repeat test flag */
    ULONG testslength;         /* Length (bytes) of "-n" command line */
    char  intestlist[QAMAXTESTS];    /* Copy of the requested tests */
    char  testlist[QAMAXTESTS];      /* List of tests passed to the handler */

    /* Pre-fill the copy array with NULLs */
    slmemset( intestlist, 0x0, QAMAXTESTS );
    slmemset( testlist, 0x0, QAMAXTESTS );

    /* Remember our own name */
    programname = argv[0];

    /* The length of the test string specified using the "-n" option
     * must be zero unless someone uses this option. The zero length
     * ensures there will be no stack dump in the call to slstrncpy()
     * below where the repetition option is being handled
     */
    testslength = 0;

    /*** Set up default values ***/
    /* Default timeout */
    timeout = DEFAULTTIMEOUT;
    timeoutp = NULL;
    pathnamep = NULL; /* one must always specify path name for SDK executable */
    repeatest = FALSE;

    /* Make a Test Report Directoy */
    mkdir ( "TestReport", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );

    /******************************************************
     * PARSING BLOCK - READ ALL THE COMMAND LINE SWITCHES *
     ******************************************************/

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
                qaprintusage();
                exit( 0 );
                break;

            case 'p':
                /* caller specifies a pathname to the SDK executable */
                pathnamep = optarg;
                if( verbose )
                    printf("\npathname: %s\n", pathnamep );
                break;

            case 'v':
                /* Verbose mode on */
                verbose = TRUE;
                break;

            case 't':
                /* Timeout */
                timeoutp = optarg;
                break;

            case 'm':
                /* Enable notifications and remain active until
                 * the tester hits Ctrl C to end the test
                 */
                printf("\n\nNOTE: Terminate execution with Ctrl-C\n");
                break;

            case 'n':
                /* caller specifies test(s) on the command line */
                testsp = optarg;

                /* Make a copy of the requested command line
                 * tests in case the "-r" option is also
                 * specified
                 */
                testslength = slstrlen( testsp );

                /* Only copy if there is enough space in the destination */
                if( testslength < QAMAXTESTS )
                {
                    slstrncpy( intestlist, testsp, testslength );
                }
                else
                {
                    printf("Test list too long, %d max.\n", QAMAXTESTS );
                    exit(1);
                }

                if( verbose )
                    printf("\nTest cases: %s\n", testsp );
                break;

           case 'r':
                /* Repeat the test until stopped with ^C */
                repeatest = TRUE;
                if( verbose )
                    printf("\nRepetition mode enabled\n");
                break;

           case 's':
                /* get the equence name if supplied */
                testsequencename = optarg;

                /* if caller hasn't specified a filename assume sequence.txt */
                if( !testsfilenamep )
                    testsfilenamep = "sequence.txt";

                if( verbose )
                    printf("\ntests filename: %s\n", testsfilenamep );
                break;

           case 'f':
                /* caller specifies a tests filename to the SDK executable */
                testsfilenamep = optarg;
                if( verbose )
                    printf("\ntests filename: %s\n", testsfilenamep );
                break;

            case '?':
                /* Caller has used an invalid option */
                if( verbose )
                    printf("\nInvalid option\n" );

                /* Print the usage info and exit */
                qaprintusage();
                exit(1);

                break;

            case 'l':
                 /* Log test outputs to file in directory TestReport */
                blogtofile = TRUE;
                break;

            case -1:
                /* Done with options list */
                if( verbose )
                    printf("\n%d options parsed\n", optioncount );
                break;

            default:
                exit(1);
                break;
        }
        optioncount++;
    }
    while( next_option != -1 );


    /*************************************************************
     * SANITY CHECK. DON'T PROCEED WITHOUT CERTAIN CRITICAL INFO *
     *************************************************************/

    /* Perform some basic checks before proceeding.
     * Can't proceed if these checks fail
     */
    if (pathnamep == NULL)
    {
        printf("\n\nWarning: Path for SDK executable not provided, ");
        printf("using default path\n\n");
    }
    else if (pathnamep != NULL)
    {
        printf("\n\nUsing %s\n\n ", pathnamep);
    }
    else if (testsp && testsfilenamep)
    {
        printf("\n\nCannot combine options -n and -f\n");
        exit(1);
    }
    else if ((testsp == NULL) && (testsfilenamep == NULL))
    {
        printf("\n\nError: please enter at least 1 test case\n\n");
        qaprintusage();
        exit(1);
    }

    /* Validate the timeout value, if specified */
    if( timeoutp )
    {
        /* If the specified timeout is valid, convert from ASCII */
        qagettimeout( timeoutp, &timeout );
    }

    /*****************************************************
     * STARTUP THE SDK AND INTERPRET THE RESULT CODE     *
     *****************************************************/
    if( verbose )
    {
        printf("\nA timeout of %d ms will be used\n", timeout );
    }

    BYTE DevicesSize = 1;
    BOOL AppConn2SDK = FALSE;

    if( SetSDKImagePath (pathnamep) != 0 )
    {
        printf ("\nUnable to start SDK..\n");
#ifndef QADEBUG
        exit(1);
#endif
    }

    if( eQCWWAN_ERR_NONE != SLQSStart() )
    {
        printf("%s: unable to start SDK", __func__ );
        exit(EXIT_FAILURE);
    }

    while( QCWWAN2kEnumerateDevices(&DevicesSize, (BYTE *)pdev) != 0 )
    {
        printf ("\nUnable to find device..\n");
        sleep(1);
#ifndef QADEBUG
        //exit(1);
#endif
    }

    fprintf(stderr, "#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
      DevicesSize,
      pdev->deviceNode,
      pdev->deviceKey  );

    if (QCWWAN2kConnect(pdev->deviceNode, pdev->deviceKey) != 0)
    {
        printf ("\nUnable to find device..\n");
#ifndef QADEBUG
        exit(1);
#endif
    }
    else
    {
        AppConn2SDK = TRUE;
    }

    /* Print the SDK version used, SDK must be tagged for correct information */
    ULONG retcode = SLQSGetSdkVersion (&sdkversionp);
    if (retcode == eQCWWAN_ERR_NONE)
    {
        printf ("\nUsing SLQS SDK version: %s\n\n", sdkversionp);
    }

    char *sequencefilename = testsfilenamep;
    /* If the "-r" switch was used, then this do loop will be
     * repeated until ^C is used to stop execution. Note, this
     * loop is identical to the one above for handling the "-m"
     * switch.
     */
    do
    {
        /* Put a fresh copy of the intests into the working list */
        if( testslength )
            memcpy( testlist, intestlist, testslength );

        testlist[testslength] = 0;
        qaProcessTests (testlist, sequencefilename, testsequencename, timeout);

        if (testsfilenamep)
            printf("\n\n Completed sequence in \"%s\"...\n",testsfilenamep);
        else
            printf("\n\n Completed %s...\n",intestlist);

        if(repeatest)
        {
            printf("\n\nEnter the testid/testsequence to be executed ");
            testslength = 0;
            testsequencename = NULL;
            sequencefilename = NULL;

            scanf( "%c", &intestlist[testslength] );
            while( intestlist[testslength] != '\n' )
                scanf( "%c", &intestlist[++testslength] );
            intestlist[testslength] = '\0';

            if( (intestlist[0] != 't') &&
                (intestlist[0] != '[') &&
                (intestlist[0] != 'x') &&
                (intestlist[0] != 'h') )
            {
                testslength = 0;
                testsequencename = intestlist;
                sequencefilename = testsfilenamep;
            }
            if ('h' == intestlist[0])
            {
                /* Print usage information */
                qaprintusage();
            }
        }
    }
    while( repeatest && slstrncmp(intestlist, "t0", 2) );

    printf("\nTest Completed ----------------------------------\n");

    /* Normal exit */
    if( AppConn2SDK )
    {
        if( QCWWANDisconnect() != eQCWWAN_ERR_NONE )
        {
            printf ("\nUnable to Disconnect from SDK..\n");
            exit(1);
        }

        AppConn2SDK = FALSE;
    }
    exit( 0 );
}
