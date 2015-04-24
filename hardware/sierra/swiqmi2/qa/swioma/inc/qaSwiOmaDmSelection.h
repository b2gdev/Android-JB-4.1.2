/*
 * \ingroup swioma
 *
 * \file    qaSwiOmaDmSelection.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSwiOmaDmSelection.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SWIOMA_DM_SELECTION_H__
#define __SWIOMA_DM_SELECTION_H__

/* enum declarations */
/*
 * An enumeration of eQMI_SWIOMA_DM_SELECTION request TLV IDs
 */
enum eSWIOMA_DM_SELECTION_REQ
{
    eTLV_USER_SELECTION = 0x10
};

/*
 * This structure contains the SLQSOMADMSendSelection request parameters.
 */
struct QmiSwiOmaDmSelectionReq
{
    ULONG selection;
};

/*
 * This structure contains the SLQSOMADMSendSelection response parameters.
 */
struct QmiSwiOmaDmSelectionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSwiOmaDmSelection(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG selection );

extern enum eQCWWANError UpkQmiSwiOmaDmSelection(
    BYTE                            *pMdmResp,
    struct QmiSwiOmaDmSelectionResp *pApiResp );

#endif /* __SWIOMA_DM_SELECTION_H__ */
