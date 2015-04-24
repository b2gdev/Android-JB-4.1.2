/*
 * \ingroup basic
 *
 * \file    qm_api.c.c
 *
 * \brief   Helper functions for encoding/decoding QMI messages
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "qa/inc/SwiDataTypes.h"
#include "qm/qmerrno.h"
#include "am/amudefs.h"
#include "ic/icudefs.h"
#include "qm/qmidefs.h"
#include "pi/piudefs.h"

/* Defines */

/* global storage */

/**
 * Return a pointer to sufficient storage for holding a
 * QMI request for transmission to the SDK side.
 *
 * \return  pointer to request block
 *
 */
global swi_uint8 *qmgetreqbkp( void )
{
    return(amgetreqbufp());
}

/**
 * Releases the pointer which was holding a
 * QMI request for transmission to the SDK side.
 *
 * \return  None
 *
 */
global void qmrelreqbkp( void )
{
    amrelreqbufp( );
}
/*************
 *
 * Unpack TLV result code into given buffer
 *
 * \param     pTlvData    Pointer to TLV result code to be unpacked.
 * \param     resp        Pointer to storage provided by the application where
 *                        the result codes are returned.
 *
 * \return    eQCWWAN_ERR_NONE
 *
 **************/
global enum eQCWWANError qmUnpackTlvResultCode(
    swi_uint8 *pTlvData,
    swi_uint8 *resp)
{
    struct qmTBuffer *tbufp = (struct qmTBuffer *)pTlvData;
    struct qmTlvResult *resultp = (struct qmTlvResult *)resp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    resultp->TlvResultCode = FALSE;
    resultp->DeviceResult  = eQCWWAN_ERR_NONE;
    resultp->DeviceError   = eQCWWAN_ERR_NONE;

    /* Get Results and error values */
    eRCode = GetWord((swi_uint8*)tbufp, &resultp->DeviceResult );
    if (eRCode)
    {
        return (eRCode);
    }

    eRCode = GetWord((swi_uint8*)tbufp, &resultp->DeviceError);
    if (eRCode)
    {
        return (eRCode);
    }

    switch (resultp->DeviceResult)
    {
        case eQCWWAN_ERR_NONE:
            break;
        case eQCWWAN_ERR_GENERAL:
            break;
        default:
            return eQCWWAN_ERR_INVALID_QMI_RSP;
    }

    /* all checked out OK */
    resultp->TlvResultCode = TRUE;
    return eQCWWAN_ERR_NONE;
}

/*
 *
 * Unpack response into given buffer, calls qmunpack and handle return codes.
 *
 * \param     bufp        Pointer to TLV data to be unpacked.
 * \param     resp        Pointer to storage provided by the application where
 *                        extracted TLV data will be extracted to.
 * \param     pItem       Pointer to map of TLV items and callbacks for the items
 *                        that does the work of extracting the data to user
 *                        provided return storage.
 * \param     messageId   QMI message ID - unused for now. May  be used to check
 *                        that we are getting the correct results from the modem.
 "
 * \return    eQCWWAN_ERR_NONE         - successful
 *
 */
global enum eQCWWANError qmunpackresp(
    swi_uint8 *bufp,
    swi_uint8 *resp,
    struct qmTlvUnpackerItem *pItem,
    const swi_uint16 messageId)
{
    struct qmTlvResult *resultp = (struct qmTlvResult *)resp;

    /* Unpack all TLV items. */
    enum eQCWWANError eRCode = qmunpack(bufp, resp, pItem, messageId);

    /* TLV specific error codes have higher priority */
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

     /* Must have a result code. */
    if (!resultp->TlvResultCode)
        return eQCWWAN_ERR_INVALID_QMI_RSP;

    if (resultp->DeviceResult != eQCWWAN_ERR_NONE)
    {
        /* Convert device error code to Gobi SDK error codes.*/
        return resultp->DeviceError + eQCWWAN_ERR_QMI_OFFSET;
    }

    return eQCWWAN_ERR_NONE;
}
