/**
 * \ingroup swi
 *
 * \file    qaGobiApiSwi.h
 *
 * \brief   SWI API function prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_SWI_H__
#define __GOBI_API_SWI_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Returns the SDK version string
 *
 *  \param  ppString[OUT]
 *          - pointer to pointer of NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE         success
 *          eQCWWAN_ERR_INVALID_ARG  provided pointer is NULL
 *
 *  \note   Technology Supported: N/A
 *          Timeout: 2 seconds
 *
 */

ULONG SLQSGetSdkVersion(
    CHAR **sdkversionpp );

/*
 * Function designed to accept Raw QMI request parameters,
 * package them up and send them to the modem blocking the
 * caller until a response is received
 *
 * \param   pReqBuf[IN]  
 *          - Pointer to the buffer into which the outgoing
 *            request will be built
 *
 * \param   service[IN]  
 *          -QMI service type  for this request
 *
 * \param   length[IN]  
 *          - request parameter field length of the outgoing
 *            packet. 0 if there is no parameter field in the
 *                               request
 * \param   timeout[IN]
 *          - Caller-specified timeout for this
 *            request/response transaction
 *
 * \param   ppInParm[OUT] -
 *          - Pointer to pointer of incoming Parameter field
 *            from the modem, if any
 *
 * \param   pParamLength[OUT]
 *          - Pointer to storage into which the length, if any,
 *            of the  parameter field from the modem will be
 *            written
 *
 * \return  eQCWWAN_ERR_NONE        success
 *          eQCWWAN_ERR_INVALID_ARG  provided pointer is NULL
 * 
 * \note    Technology Supported: N/A
 *          time-out : user input
 *
 */
ULONG SLQSSendRawQMI(
    BYTE   *pReqBuf,
    USHORT service,
    USHORT length,
    ULONG  timeout,
    BYTE   **ppInParm,
    USHORT *pParamLength );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_SWI_H__ */
