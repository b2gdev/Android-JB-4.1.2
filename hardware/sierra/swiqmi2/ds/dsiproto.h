/*************
 *
 * Filename:  dsiproto.h
 *
 * Purpose:   This file contains internal prototypes for the ds package
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* dsshell.c */
swi_bool dsep_send( enum useptype epid,
                           enum qmisupportedclients client,
                           swi_uint8 *packetp,
                           swi_uint32 paylength );

void dsshell( swi_uint8 *owneridp,
                     enum qmisupportedclients client,
                     void *dsrcvcbckp( swi_uint8 *inipcmsgp,
                                       swi_uint32 inmsglen,
                                       swi_uint8 *memfreep,
                                       swi_uint16 ipcchannel ),
                     void (*dsinitcbfp)(swi_bool) );

swi_bool dsep_ioctl( enum useptype epid,
                            enum qmisupportedclients client,
                            swi_uint16    ioctlcmd,
                            void          *pparm,
                            size_t        parmlen );

void dsep_close( enum useptype epid,
                        enum qmisupportedclients client );
