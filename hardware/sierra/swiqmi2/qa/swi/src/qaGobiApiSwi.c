/**
 * \ingroup swi
 *
 * \file    qaGobiApiSwi.c
 *
 * \brief   Entry points for Swi APIs that do not belong to any service.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#include "SwiDataTypes.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "qaQmiBasic.h"

ULONG SLQSGetSdkVersion(
    CHAR **sdkversionpp)
{
    if (sdkversionpp == NULL)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }
    *sdkversionpp = (CHAR *)amgettagname();
    return eQCWWAN_ERR_NONE;
}

ULONG SLQSSendRawQMI(  
    BYTE   *pReqBuf,
    USHORT service,
    USHORT length,
    ULONG  timeout,
    BYTE   **ppInParm,
    USHORT *pParamLength )
{
    ULONG resultCode;

    /* IN/OUT parameters should not be NULL */ 
    if ( !pReqBuf  ||
         !ppInParm ||
         !pParamLength )
    {
        return eQCWWAN_ERR_INVALID_ARG;     
    }  
    resultCode = SwiQmiSendnWait ( pReqBuf,
                                   service,
                                   length,
                                   timeout,
                                   ppInParm,
                                   pParamLength );
    return resultCode;    
}
