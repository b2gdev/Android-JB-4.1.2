/*
 * \ingroup cbk
 *
 * \file    qaCbkWmsTransNWRegInfoInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkWmsTransNWRegInfoInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WMS_TRANS_NW_REG_INFO_IND_H_
#define __WMS_TRANS_NW_REG_INFO_IND_H_

#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_WMS_NW_REG_INFO_IND response TLV IDs
 */
enum eQMI_WMS_NW_REG_INFO_IND
{
    eTLV_TRANS_NW_REG_INFO = 0x01,
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkWmsTransNWRegInfoInd(
    BYTE                       *pMdmResp,
    transNWRegInfoNotification *pApiResp );

#endif /* __WMS_TRANS_NW_REG_INFO_IND_H_ */
