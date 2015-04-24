/*
 * \ingroup cbk
 *
 * \file    qaCbkUIMSLQSUIMSetRefreshInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkUIMSLQSUIMSetRefreshInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_REFRESH_IND_H_
#define __UIM_REFRESH_IND_H_

#include "qaGobiApiVoice.h"
#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_UIM_REFRESH_IND response TLV IDs
 */
enum eQMI_UIM_REFRESH_IND
{
    eTLV_REFRESH_EVENT = 0x10
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkUIMSetRefreshInd(
    BYTE            *pMdmResp,
    UIMRefreshEvent *pApiResp );

#endif /* __UIM_REFRESH_IND_H_ */
