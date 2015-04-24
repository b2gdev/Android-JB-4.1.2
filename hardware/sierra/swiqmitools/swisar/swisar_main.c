/**
 *
 * @ingroup swisar
 *
 * @file
 * main fuction of RF SAR state query and setting 
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

/* include files */
#include <stdlib.h>
#define  LOG_TAG  "swi_sar"
#include "swiril_log.h"
#include "qmerrno.h"
#include "SWIWWANCMAPI.h"
#include <swiqmitool_common.h>

static void usage(char *s)
{
    fprintf(stderr, "usage: \n%s \tquery the current SAR value \n \
             <new SAR State>  \tset the current value to new SAR state \n \
             Note: SAR state is a numeric value from 0 to 8 \n", s);
   
}

/**
 *
 * main function for getting and setting current RF SAR state
 *
 * @param argc
 *     argument count
 * @param[in] argv 
 *     Pointer to a array of arguments
 *
 * @return
 *     exit code
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
int main(int argc, char **argv)
{
    ULONG nRet = 0 ;
    ULONG nRfSarState = 0 ;

    if (argc > 2) 
    {
        LOGE("unsupported parameter number: %d ", argc-1);
        usage(argv[0]);
        return 1;
    }
    
    qmiDeviceConnect();
    if (argc == 1) 
    {
        /* Request a client-initiated SAR query function */
        nRet = SLQSGetRfSarState(&nRfSarState);
        if (nRet == eQCWWAN_ERR_NONE) 
        {
            LOGD("GetRfSarState succeed, nRfSarState: %lu. ", nRfSarState );
            printf("Current SAR state :%lu \n", nRfSarState);    
        }
        else 
        {
            LOGE("GetRfSarState failed error: %lu", nRet);
            printf("%s failed \n", argv[0]);    
        } 
    }
    else
    {
        nRfSarState = (ULONG)atoi(argv[1]);
        LOGD("set SAR with status %lu ", nRfSarState);
        
        if(nRfSarState > 8)
        {
            LOGE("unsupported SAR status %lu ", nRfSarState);
            usage(argv[0]);
            qmiDeviceDisconnect();
            return 1;
        }
        
        /* Launch the request to set SAR status */
        nRet = SLQSSetRfSarState(nRfSarState);
        /* Report the result depending on the outcome */
        if (nRet == eQCWWAN_ERR_NONE) 
        {
            LOGD("SetRfSarState succeed. ");
            printf("Success \n");    
        }
        else 
        {
            LOGE("GetRfSarState failed error: %lu", nRet);
            printf("Failed \n");    
        } 
    }

    qmiDeviceDisconnect();
    exit(0);
}
