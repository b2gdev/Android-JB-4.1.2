/*
 * \ingroup cbk
 *
 * \file qaCbkNasNetworkTimeInd.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaCbkNasNetworkTimeInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_NETWORK_TIME_IND_H__
#define __NAS_NETWORK_TIME_IND_H__

#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_NAS_NETWORK_TIME_IND response TLV IDs
 *
 */
enum eQMI_NAS_NETWORK_TIME_IND
{
    eTLV_IND_UNIVERSAL_TIME = 0x01,
    eTLV_IND_TIME_ZONE      = 0x10,
    eTLV_IND_DAYLT_SAV_ADJ  = 0x11
};

/*
 * Prototypes
 *
 */
enum eQCWWANError UpkQmiCbkNasNetworkTimeInd(
    BYTE           *pMdmResp,
    nasNetworkTime *pApiResp );

#endif /* __NAS_NETWORK_TIME_IND_H__ */
