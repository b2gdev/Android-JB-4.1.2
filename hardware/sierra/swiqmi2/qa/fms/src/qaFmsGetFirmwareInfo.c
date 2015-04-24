/*
 * \ingroup fms
 *
 * \file    qaFmsGetFirmwareInfo.c
 *
 * \brief   GetFirmwareInfo API support functions
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaFmsGetFirmwareInfo.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 *  Extracts the technology and GPS capability from the firmware version ID
 *
 *  \param  FirmwareID
 *          - Firmware ID obtained from the firmware image
 *
 *  \param  Carrier
 *          - Carrier

 *  \param  pTechnology[OUT]
 *          - Technology
 *
 *  \param  pGPSCapability[OUT]
 *          - GPS capability
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError qaParseFirmwareID(
    ULONG   FirmwareID,
    ULONG   Carrier,
    ULONG   *pTechnology,
    ULONG   *pGPSCapability )
{
    ULONG XTRADisabled;
    ULONG GPSDisabled;
    ULONG System;

    /* extract the parameters from the firmware ID */
    XTRADisabled =
    (FirmwareID & GOBI_FW_XTRA_DISABLED_MASK) >> GOBI_FW_XTRA_DISABLED_OFFSET;

    GPSDisabled =
    (FirmwareID & GOBI_FW_GPS_DISABLED_MASK) >> GOBI_FW_GPS_DISABLED_OFFSET;

    System =
    (FirmwareID & GOBI_FW_SYSTEM_MASK) >> GOBI_FW_SYSTEM_OFFSET;

    /* check for valid technologies */
    if( System == eQMI_SYSTEM_VALUE_UMTS )
    {
        *pTechnology = (ULONG)eGOBI_IMG_TECH_UMTS;
    }
    else if( System == eQMI_SYSTEM_VALUE_CDMA )
    {
        *pTechnology = (ULONG)eGOBI_IMG_TECH_CDMA;
    }
    else
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Set GPS capability */
    if( GPSDisabled == 1 )
    {
        *pGPSCapability = (ULONG)eGOBI_IMG_GPS_NONE;
    }
    else if( Carrier == (ULONG)eGOBI_IMG_CAR_NORF )
    {
        /* No RF with GPS results in stand-alone GPS support only */
        *pGPSCapability = (ULONG)eGOBI_IMG_GPS_STAND_ALONE;
    }
    else
    {
        if( XTRADisabled == 1 )
            *pGPSCapability = (ULONG)eGOBI_IMG_GPS_NO_XTRA;
        else
            *pGPSCapability = (ULONG)eGOBI_IMG_GPS_ASSISTED;
    }

    return eQCWWAN_ERR_NONE;
}
