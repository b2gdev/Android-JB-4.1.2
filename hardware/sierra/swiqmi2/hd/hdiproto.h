/*
 *	Filename:  hdiproto.h
 *
 *	Purpose:   Local prototypes for the Data Link Layer package
 *
 *	Copyright: C 2008 Sierra Wireless Inc., all rights reserved
 *
 */

#ifndef hdiproto_h
#define hdiproto_h

extern swi_bool hdsend (struct hdreq *hdreqp);
extern void hdrxreset(struct hdRxCtlBk * hdRxpcbp, swi_bool doallocate );
extern swi_uint8 hdrxdllstate (swi_uint8 * rxchrp);
extern swi_bool hdRxGetStatus ();
extern struct hdTxCtlBk *hdGetTxCtlBk ();
extern struct hdRxCtlBk *hdGetRxCtlBk ();
extern void hdTxInit(void);
extern void hdRxInit(swi_uint32 outoffset);

#endif
