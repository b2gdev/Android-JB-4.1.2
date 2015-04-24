/*
 * Filename: usuproto.h
 *
 * Purpose:  Global prototypes for the US package
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 */

#include <stdlib.h>
#include "qm/qmudefs.h"

/* usscan_sdk.c */
extern swi_bool
usvidpid(
    swi_uint32 *vendidp,
    swi_uint32 *prodidp );

extern swi_bool
usalive(void);

extern swi_bool usrcvtty(
    enum useptype epdev,
    enum qmisupportedclients client,
    swi_uint16  endpoint,
    swi_uint8   *datap,
    swi_size_t  *dlengthp,
    swi_uint32  timeout );

extern swi_bool
ussendtty(
    enum useptype epdev,
    enum qmisupportedclients client,
    swi_uint16  endpoint,
    swi_uint8   *datap,
    swi_size_t  dlength );

extern void
ustaskinit(void);

extern swi_bool
usgetep(
    enum useptype eptype,
    struct usbep *ep );

extern swi_bool
usgetepname(
    swi_uint32 endpoint,
    swi_uint8* namep,
    swi_uint32 namelen,
    swi_bool   inout );

extern void
usinit(void);

extern void
usclosetty(
    enum useptype epdev,
    enum qmisupportedclients client,
    struct usbep *ep );

extern swi_bool
usgetnuminterfaces(
    swi_uint32 *pnumif );

extern swi_bool
usioctltty(
    enum useptype epdev,
    enum qmisupportedclients client,
    struct usbep endpoint,
    swi_uint16   ioctlcmd,
    void         *pparm,
    size_t       parmlen );

extern swi_bool
us_register_device_notification_callback(
    void(*pcb)(enum usb_device_event ev) );

extern void
us_trigger_device_notification_callbacks(
    enum usb_device_event ev );

extern swi_int32
usopentty(
    enum useptype epdev,
    enum qmisupportedclients client,
    struct usbep *ep,
    int mode );
