/*
 *  Filename: ciudefs.h
 *
 *  Purpose:  User definitions for the CI package
 *
 *  Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 */

#ifndef ciudefs_h
#define ciudefs_h

#include "ic/icudefs.h"
#include "us/usudefs.h"

/* Maximum number of channels supported  */
#define CIMAXIPCCHAN       14
#define CIINVIPCCHAN       CIMAXIPCCHAN /* used for error checking
                                         * This value cannot be used
                                         * for indexing our channel
                                         * array as it is out
                                         * of bounds.
                                         */

/* Maximum number of RR & Notification channel-pairs supported  */
#define CIMAXRRPAIR      5

/* Maximum number of DR services supported  */
#define CIMAXDRSVC         3

/* IPC channels names */
#define CIIPCUI         "UIIPC"
#define CIIPCDIAG       "DIAG"
#define CIIPCUDIAG      "UDIAG"
#define CIIPCNMEA       "NMEA"
#define CIIPCUNMEA      "UNMEA"
#define CIIPCAT         "AT"
#define CIIPCUAT        "UAT"
#ifdef SLQS
#define CIIPCQMRR1      "QMRR1"
#define CIIPCQMNOTIF1   "QMNOTIF1"
#define CIIPCUQMRR1     "QMURR1"
#define CIIPCUIMRR1     "IMURR1"
#define CIIPCUIMRR2     "IMURR2"
#endif /* SLQS */
/*
 * Name:    ciipcifusetyp
 *
 * Purpose: High-level usage indicator
 *
 * Members: CIRR       - Request/Response channel
 *          CINOTIF    - Notification channel
 *          CIDRSVC    - Data Relay service channel
 *          CIUI       - User Interface channel
 *
 * Note:
 */
enum ciipcifusetyp
{
    CIRR,
    CINOTIF,
    CIDRSVC,
    CIUI,
    CIUSETYPEMAX
};

/**
 * Name:   ciifacetype
 *
 * Purpose: Enumerated list of CI device interface types
 *
 * Members: CIIPC      - IPC channel
 *          CIUIF      - USB Interface
 *          CIIFINVALID - Invalid interface type
 *
 * Note:
 */
enum ciifacetype
{
    CIIPC,
    CIUIF,
    CIIFINVALID
};

/*************
 *
 * Name:    ciipcchanfixed
 *
 * Purpose: Structure that implements a table of fixed channel information.
 *
 * Members: cichannamep     - Pointer to name of channel
 *          cipeerchannamep - Pointer to name of peer channel, eg, notification
 *          ciiftype        - Interface type - IPC or USB (currently)
 *          ciipcifuse      - Usage of IPC channel, ie, type of service
 *          ciuifuse        - Usage of USB channel, ie, type of service
 *
 * Notes:
 *
 **************/
struct ciipcchanfixed
{
    char           *cichannamep;
    char           *cipeerchannamep;
    enum ciifacetype    ciiftype;
    enum ciipcifusetyp  ciipcifuse;
    enum useptype       ciuifuse;
};

/*************
 *
 * Name:    ciipcchan
 *
 * Purpose: Structure that implements a table of all channel information.
 *
 * Members: cichanfixedinfop  - Pointer to fixed info portion
 *          cichannel         - Channel number
 *          ciipccb           - Storage for IPC control block
 *
 * Notes:
 *
 **************/
struct ciipcchan
{
    struct ciipcchanfixed *cichanfixedinfop;
    swi_uint16 cichannel;
    struct icipccb ciipccb;
};

/* User prototypes */
#include "ci/ciuproto.h"

#endif /* ciudefs_h */
