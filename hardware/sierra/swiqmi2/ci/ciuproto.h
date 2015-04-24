/*
 * Filename:  ciuproto.h
 *
 * Purpose:   User prototypes for the CI package
 *
 * Copyright: © 2009 Sierra Wireless Inc., all rights reserved
 */

#ifndef ciuproto_h
#define ciuproto_h

/* cipackage.c */
extern swi_uint16 cigetnumchannels(void);
extern void ciinit(void);
extern swi_bool ciinitapiipcchan(struct icipccb *ipccbp, swi_uint8 * ciipcchannamep);
extern void ciinitsdkipcchan(swi_uint8 * ciipcchannamep);

extern struct icipccb *cigetipcp(swi_uint16 ciipcchannel);
extern struct icipccb *cigetnextnotifipcp(swi_uint16 notifipcnum);
extern struct icipccb *cigetnextrripcp(swi_uint16 ipcnum);

extern int cigetifusetype( swi_uint16 ciipcchannel );
extern enum ciifacetype cigetifacetype( swi_uint8 *chnamep );
extern swi_uint8  *cigetchannelname(swi_uint16 channel);
extern swi_uint16 cigetchannelnum(char *chnamep);
extern swi_uint16 cigetnumRRchan( void );
extern swi_uint16 cigetnumnotifchan( void );
extern swi_uint16 cigetnumdrservices( void );
extern swi_uint16 cigetnextrrchannum(swi_uint16 cnrripcnum);
extern swi_uint8  *cigetnextrrnamep(swi_uint16 ipcnum);
extern swi_uint8  *cigetnextdrnamep(swi_uint16 ipcnum);
extern swi_uint16 cigetpeerchan( swi_uint16 channel );
extern swi_uint8  *cigetpeernamep( swi_uint16 channel );
extern swi_uint16 cigetindex( swi_uint8 *channelnamep );
#endif
