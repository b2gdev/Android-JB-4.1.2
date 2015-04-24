/*************
 *
 * Filename: dsuproto.h
 *
 * Purpose:  This file external prototypes for the ds package
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "qa/inc/SwiDataTypes.h"
#include "qm/qmudefs.h"

/* dsshell.c */
extern void dsinit(void);

extern void dslaunchshell(
    void *shellparmp );

/* dsipctask_sdk.c */
extern swi_bool dssend(
    swi_uint8 *packetptr,
    swi_uint32 packetlen,
    swi_uint16  chanid,
    enum qmisupportedclients client
);

extern void dsclose(
    swi_uint16 chanid,
    enum qmisupportedclients client
);

extern swi_bool dsioctl(
    swi_uint16 chanid,
    enum qmisupportedclients client,
    swi_uint16 ioctlcmd,
    void       *paramp,
    size_t     parmlen );

extern swi_bool
ds_register_device_notification_callback(
    void(*pcb)(enum ds_sio_event ev) );
