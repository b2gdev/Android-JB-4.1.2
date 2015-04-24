/*
 * \ingroup cbk
 *
 * \file    qaCbkUIMSLQSSetStatusChangeInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkUIMSLQSSetStatusChangeInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_STATUS_CHANGE_IND_H_
#define __UIM_STATUS_CHANGE_IND_H_

#include "qaGobiApiVoice.h"
#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_UIM_STATUS_CHANGE_IND response TLV IDs
 */
enum eQMI_UIM_STATUS_CHANGE_IND
{
    eTLV_CARD_STATUS = 0x10
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkUIMSetStatusChangeInd(
    BYTE                *pMdmResp,
    UIMStatusChangeInfo *pApiResp );

#endif /* __UIM_STATUS_CHANGE_IND_H_ */
