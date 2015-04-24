/*
 * Filename:  icuproto.h
 *
 * Purpose:   User prototypes for the IC package
 *
 * Copyright: © 2008-2012 Sierra Wireless Inc., all rights reserved
 *
 */

#ifndef icuproto_h
#define icuproto_h

#include "ql/qludefs.h"

/* icsmt_sdk.c */
extern void icsmt_create(
                          struct icsmtcb *icsmtp,
                          char *smtqueuenamep,
                          swi_uint16 smtqueuedepth );
extern void icsmt_send( struct icsmtcb *icsmtp, swi_uint32 msg );
extern swi_bool icsmt_rcv( struct icsmtcb *icsmtp,
                           swi_uint32 timeout,
                           swi_uint32 *msgp );
extern swi_uint32 icsmt_getmaxqueued( struct icsmtcb *icsmtp );
extern swi_charp icsmt_getqnamep( struct icsmtcb *icsmtp );

/* icipc_sdk.c */
extern swi_bool icsdk_rcv( struct icipccb *ipccbp,
                           swi_uint8 *ipcbufp,
                           swi_uint32 *ipcbuflenp,
                           swi_uint32 timeout );
extern swi_bool icsdk_send( struct icipccb *ipccbp,
                            swi_uint8 *ipcbufp,
                            swi_uint32 ipcbuflen  );
extern void icinitsdkhndl( struct icipccb *ipccbp,
                           swi_size_t ipcindex,
                           swi_uint8 * chnamep);
extern void icinitipc( void );

/* icticktask_sdk.c   */
extern void ictttickreg(
    struct icsmtcb      *icsmtp,
    swi_uint32          tickmsg,
    struct icttregqblk  *callerregblk,
    swi_uint32          mstimeout );

extern void ictttaskinit( void );

/* icipc_api.c */
extern swi_bool icapi_rcv( struct icipccb *ipccbp,
                           swi_uint8 *ipcbufp,
                           swi_uint32 *ipcbuflenp,
                           swi_uint32 timeout );
extern swi_bool icapi_send( struct icipccb *ipccbp,
                            swi_uint8 *ipcbufp,
                            swi_uint32 ipcbuflen  );
extern swi_bool icinitapihndl( struct icipccb *ipccbp,
                               swi_size_t ipcindex,
                               swi_uint8 *chnamep );

#endif
