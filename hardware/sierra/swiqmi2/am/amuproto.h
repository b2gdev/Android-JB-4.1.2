/*************
 *
 * Filename: amuproto.h
 *
 * Purpose:  This file external prototypes for the am package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* am.c */
extern swi_uint8 *amgethdrptr(
    swi_uint8 *parmp,
    swi_uint16 amtype,
    swi_uint8 *bufstartp );

extern void amparseamheader(
    swi_uint16 *amtypep,
    swi_uint16 *amheaderversionp,
    enum eQCWWANError *amresultcodep,
    swi_uint16 *ampacketlengthp,
    swi_uint8 **packetpp);

extern swi_uint8 *amgettagname( void );

/* amapi.c */
extern swi_uint8 *amgetparamp(
    swi_uint16 amqmitype,
    swi_uint8 *bufstartp );
global swi_uint8 *amgetNotifbufp( void );
extern swi_uint8 *amgetreqbufp( void );
extern enum eQCWWANError  amsendnwait(
    swi_uint8 *amhdrp,
    struct amrrparms *amparamsp,
    swi_uint8 **responsepp );
extern swi_bool amapiwaitnotif(
    swi_uint8 *apipcmsgp,
    swi_uint32 *apipcbuflngp,
    struct amrrparms *parmsp,
    swi_uint8  **payloadp,
    swi_uint32 timeout );
extern swi_bool aminitapi( void );
extern void     amrelreqbufp( void );

/* amipctask_sdk.c */
extern void aminit( void );
extern void amipctaskinit( void );
extern void amsdk_send( swi_uint8 *ipcbufp,
                            swi_uint32 ipcbuflen,
                            swi_uint8 ipcchannel );

/* amqmi.c */
extern swi_uint16 ambuildqmixactionheader(
    swi_uint8 **amsgpp,
    struct amqmirrparms *amqmirrparmp,
    enum ammsgtypes amtype );

extern void amparseqmixactionheader(
    swi_uint8 **amqmipacketpp,
    struct amqmirrparms *amqmirrparmp );

/* amsdk.c */
extern void am_sdk_terminated_notify(
    swi_uint8 *psReason );
extern swi_uint16 ambuildsdkrequest(
    swi_uint8 **amsgpp,
    struct    amsdkrrparms *amsdkp,
    enum      ammsgtypes   amtype );

extern void amparsesdkheader(
    swi_uint8 **amsdkpacketpp,
    struct amsdkrrparms *amsdkrrparmp );

