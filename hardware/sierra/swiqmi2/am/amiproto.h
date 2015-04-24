/*************
 *
 * Filename:  amiproto.h
 *
 * Purpose:   This file contains internal prototypes for the am package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* am.c */
extern void ambuildamheader(
    swi_uint16 amtype,
    swi_uint16 amheaderversion,
    enum eQCWWANError amresultcode,
    swi_uint16 ampacketlength,
    swi_uint8 **packetpp);
/* amsdk.c */

/* RILSTART */
package void amsdkcheckandhandlekill( swi_uint8 *inipcmsgp );
/* RILSTOP */

extern void amsdkhandler(
    swi_uint8 *inipcmsgp,
    swi_uint8 *memfreep,
    swi_uint8 ipcchannel );
