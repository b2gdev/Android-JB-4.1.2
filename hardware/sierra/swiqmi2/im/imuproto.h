/*************
 *
 * Filename: imuproto.h
 *
 * Purpose:  External prototypes for im package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef IMUPROTO_H
#define IMUPROTO_H

#include "imudefs.h"

/*-------------
  Prototypes
 --------------*/
/* imtask_sdk.c */
extern swi_uint8* imgetreqbkp(void);
extern void iminit(void);
extern void imtaskinit( const char *fwimagepath );
extern void imdssend(   swi_uint8 *txbufp,
                        swi_uint32 pktsize );

extern enum imerrcodes_e imuser_image_info_get(
    struct im_image_info_s *pin);
#endif /* IMUPROTO_H */
