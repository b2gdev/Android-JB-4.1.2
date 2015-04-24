/*
 * \ingroup basic
 *
 * \file    qaQmiBasic.c
 *
 * \brief   Contains support and helper functions to interface to APIs.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "qaQmiBasic.h"
#include "qaQmiNotify.h"

/* external functions */
extern void qaWdsInit();

/*
 * Package-wide function designed to accept basic QMI request
 * parameters, package them up and send them to the modem,
 * blocking the caller until a response is received
 *
 * \param   pReqBuf      - [IN]  Pointer to the buffer into which the outgoing
 *                               request will be built
 * \param   service      - [IN]  QMI service type  for this request
 *
 * \param   length       - [IN]  request parameter field length of the outgoing
 *                               packet. 0 if there is no parameter field in the
 *                               request
 * \param   timeout      - [IN]  Caller-specified timeout for this
 *                               request/response transaction
 * \param   ppInParm     - [OUT] Pointer to pointer of incoming Parameter field
 *                               from the modem, if any
 * \param   pParamLength - [OUT] Pointer to storage into which the length, if any,
 *                               of the  parameter field from the modem will be
 *                               written
 *
 * \return  enum eQCWWANError value
 * \sa      qmerrno.h
 *
 */
package enum eQCWWANError SwiQmiSendnWait (
    BYTE   *pReqBuf,
    USHORT service,
    USHORT length,
    ULONG  timeout,
    BYTE   **ppInParm,
    USHORT *pParamLength )
{
    struct amrrparms    reqparms;   /* For collecting up the caller's args */
    enum eQCWWANError   resultcode; /* Result of calling AM entry point */

    UNUSEDPARAM( pParamLength );

    /* Fill in the AM structure with the required values for this request */
    reqparms.amparmtype = SWI_PARM_QMI;
    reqparms.amtimeout = timeout;
    reqparms.amparm.amqmi.amqmireqtimeout = timeout;
    reqparms.amparm.amqmi.amqmixactionlen = length + QMISVC_MSGHDR_SZ;
    reqparms.amparm.amqmi.amqmisvctype = service;

    /* Dispatch this packet to the SDK side, waiting for a response */
    resultcode = amsendnwait( pReqBuf, &reqparms, ppInParm );

    return(resultcode);
}

package enum eQCWWANError SwiSdkSendnWait (
    BYTE   *pReqBuf,
    USHORT reqtype,
    USHORT length,
    ULONG  timeout,
    BYTE   **ppInParm,
    USHORT *pParamLength )
{
    struct amrrparms reqparms;   /* For collecting up the caller's args */
    enum eQCWWANError   resultcode; /* Result of calling AM entry point */

    UNUSEDPARAM( length );
    UNUSEDPARAM( pParamLength );

    /* Fill in the AM structure with the required values for this request */
    reqparms.amparmtype = SWI_PARM_SDK;
    reqparms.amtimeout = timeout;
    reqparms.amparm.amsdk.amsdkrrtype = reqtype;

    /* Dispatch this packet to the SDK side, waiting for a response */
    resultcode = amsendnwait( pReqBuf, &reqparms, ppInParm );

    return(resultcode);
}

/*
 * Name:    qaInit
 *
 * Purpose: QA Package initialization routine
 *
 * Parms:   none$
 *
 * Return:  none$
 *
 */
global void qaInit(void)
{
    static BYTE qaPkgInitFlag = FALSE;

    if ( qaPkgInitFlag == FALSE )
    {
        qaWdsInit();

        /* create the notification thread */
        qaNotifyInit();
        qaPkgInitFlag = TRUE;
    }
}

