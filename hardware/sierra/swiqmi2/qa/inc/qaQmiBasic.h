/**************
 *
 * Filename: QmiBasic.h
 *
 * Purpose:  Header file for QMI support interface
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 **************/

#ifndef __QMI_BASIC_H__
#define __QMI_BASIC_H__

#ifdef __cplusplus
extern "C" {
#endif

extern  enum eQCWWANError SwiQmiSendnWait(
    BYTE    *pReqBuf,
    WORD    service,
    WORD    length,
    ULONG   timeout,
    BYTE    **ppInParam,
    WORD    *pParamLength);

void qaInit(void);

extern enum eQCWWANError SwiSdkSendnWait (
    BYTE   *pReqBuf,
    USHORT reqtype,
    USHORT length,
    ULONG  timeout,
    BYTE   **ppInParm,
    USHORT *pParamLength );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif // __QMI_BASIC_H__
