/**
 * \ingroup sar
 *
 * \file    qaGobiApiSar.h
 *
 * \brief   Specific Absorption Rate API function prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_SAR_H__
#define __GOBI_API_SAR_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This enum contains the SAR RF States
 *
 *  \param  SAR RF State
 *          - QMI_SAR_RF_STATE_DEFAULT = 0
 *          - QMI_SAR_RF_STATE_1
 *          - QMI_SAR_RF_STATE_2
 *          - QMI_SAR_RF_STATE_3
 *          - QMI_SAR_RF_STATE_4
 *          - QMI_SAR_RF_STATE_5
 *          - QMI_SAR_RF_STATE_6
 *          - QMI_SAR_RF_STATE_7
 *          - QMI_SAR_RF_STATE_8
 */
enum eQMISARRFState
{
   QMI_SAR_RF_STATE_DEFAULT = 0,
   QMI_SAR_RF_STATE_1,
   QMI_SAR_RF_STATE_2,
   QMI_SAR_RF_STATE_3,
   QMI_SAR_RF_STATE_4,
   QMI_SAR_RF_STATE_5,
   QMI_SAR_RF_STATE_6,
   QMI_SAR_RF_STATE_7,
   QMI_SAR_RF_STATE_8
};

/**
 *  Gets the specified RF SAR state.
 *
 *  \param  pSarRFState
 *          - SAR RF State
 *            - QMI_SAR_RF_STATE_DEFAULT
 *            - QMI_SAR_RF_STATE_1
 *            - QMI_SAR_RF_STATE_2
 *            - QMI_SAR_RF_STATE_3
 *            - QMI_SAR_RF_STATE_4
 *            - QMI_SAR_RF_STATE_5
 *            - QMI_SAR_RF_STATE_6
 *            - QMI_SAR_RF_STATE_7
 *            - QMI_SAR_RF_STATE_8
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 */
ULONG SLQSGetRfSarState(
    ULONG *pSarRFState) ;

/**
 *  Sets the specified RF SAR state.
 *
 *  \param  sar_rf_state
 *          - SAR RF State
 *            - QMI_SAR_RF_STATE_DEFAULT
 *            - QMI_SAR_RF_STATE_1
 *            - QMI_SAR_RF_STATE_2
 *            - QMI_SAR_RF_STATE_3
 *            - QMI_SAR_RF_STATE_4
 *            - QMI_SAR_RF_STATE_5
 *            - QMI_SAR_RF_STATE_6
 *            - QMI_SAR_RF_STATE_7
 *            - QMI_SAR_RF_STATE_8
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 */
ULONG SLQSSetRfSarState(
    ULONG RfSarState ) ;

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_SAR_H__ */
