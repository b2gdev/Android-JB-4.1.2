/*
 * \ingroup cbk
 *
 * \file    qaCbkWmsTransLayerInfoInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkWmsTransLayerInfoInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WMS_TRANS_LAYER_INFO_IND_H_
#define __WMS_TRANS_LAYER_INFO_IND_H_

#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_WMS_TRANS_LAYER_INFO_IND response TLV IDs
 */
enum eQMI_WMS_TRANS_LAYER_INFO_IND
{
    eTLV_TRANS_LAYER_REG_INFO = 0x01,
    eTLV_TRANS_LAYER_INFO     = 0x10,
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkWmsTransLayerInfoInd(
    BYTE                   *pMdmResp,
    transLayerNotification *pApiResp );

#endif /* __WMS_TRANS_LAYER_INFO_IND_H_ */
