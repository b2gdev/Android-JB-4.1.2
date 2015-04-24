/*
 * Filename:  hduproto.h
 *
 * Purpose:   User prototypes for the Data Link Layer package
 *
 * Copyright: C 2008 Sierra Wireless Inc., all rights reserved
 *
 */

#ifndef hduproto_h
#define hduproto_h

global swi_bool hdTxPacket( swi_uint8 *packetp, 
                          swi_uint16 packetsz,
                          void (*sendcfp)(void *userp, 
                                swi_uint8 *bufp, 
                                swi_bool errorflg),
                          void *userp,
                          void (*usersendfp)( swi_uint8 *bufp,
                                              swi_uint32 bufsize),
                          swi_uint16 fcs );

extern void hdRxPacket(swi_uint8* datap, 
                       swi_uint16 length,
                       void (*userrxfp)( swi_uint8  *memrelp,
                                         swi_uint8  *ufbufp,
                                         swi_uint16 pktsize,
                                         swi_bool   errorflag) );
extern void hdRxRegister(
    swi_uint8 *(*notifyfp)(swi_uint16 *sizep),
    void (*deliverbuffp)(swi_uint8 *bufp, swi_bool errorflg)
);

extern void hdinstrxfp(void);
extern void hdinit( swi_uint32 outoffset );
extern swi_uint32 hdTxGetDroppedPktCnt();
#endif
