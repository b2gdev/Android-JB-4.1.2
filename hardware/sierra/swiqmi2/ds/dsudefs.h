/*
 * Filename:  dsudefs.h
 *
 * Purpose:   User definitions for DS package
 *
 * Copyright: © 2010-2012 Sierra Wireless Inc., all rights reserved
 *
 */

#ifndef dsudefs_h
#define dsudefs_h

/* Includes */
#include "qm/qmudefs.h"

/* DS Package Definitions */
enum ds_sio_event{
    DS_SIO_DISCONNECTED,
    DS_SIO_READY,
    DS_SIO_BOOT_READY
};

/* DS structures */
/*************
 *
 * Name:    dsipcshell
 *
 * Purpose: DS IPC shell structure
 *
 * Members: dsipcshellnamep  - Pointer to DS shell name
 *          client           - client of the DS shell
 *          dsipccbfcnp      - Callback function of the DS user.
 *          dssinitcbfp      - function to call once the DS shell has
 *                             been created
 *
 * Note:    The client member provides a higher resolution then the DS shell name.
 *          This allows one service to use multiple clients as in the case of QMI.
 *
 **************/
struct dsipcshell
{
    swi_uint8 *dsipcshellnamep;
    enum qmisupportedclients client;
    void    *dsipccbfcnp;
    void    (*dssinitcbfp)(swi_bool);
};

#include "ds/dsuproto.h"

#endif
