/*
 * \ingroup cbk
 *
 * \file    qaCbkWmsMemoryFullInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkWmsMemoryFullInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WMS_MEMORY_FULL_IND_H__
#define __WMS_MEMORY_FULL_IND_H__

/* An enumeration of eQMI_WMS_MEMORY_FULL_IND response TLV IDs */
enum eQMI_WMS_MEMORY_FULL_IND_TLV
{
    eTLV_MEMORY_FULL_INFO = 0x01
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkWmsMemoryFullInfoInd(
    BYTE          *pMdmResp,
    SMSMemoryInfo *pApiResp );

#endif /* __WMS_MEMORY_FULL_IND_H__ */
