/************
 *
 * Filename:  dluproto.h
 *
 * Purpose:   external prototypes for dl
 *
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 ************/
extern global void dlregister( char *pkgnamep,
                               struct dlusercb * usercb,
                               swi_bool masterenable );

extern void dlLog3( struct dlusercb *usercbp,
                    swi_uint16      class,
                    swi_char        *stringp,
                    swi_uint32      param1,
                    swi_uint32      param2,
                    swi_uint32      param3 );

extern void dlLog2( struct dlusercb *usercbp,
                    swi_uint16      class,
                    swi_char        *stringp,
                    swi_uint32      param1,
                    swi_uint32      param2 );

extern void dlLog1( struct dlusercb *usercbp,
                    swi_uint16      class,
                    swi_char        *stringp,
                    swi_uint32      param1 );

extern void dlLog0( struct dlusercb *usercbp,
                    swi_uint16      class,
                    swi_char        *stringp );

extern void dlLog(  struct dlusercb * usercb,
                    swi_uint16 class,
                    char * stringp,
                    swi_uint32 dlparam1,
                    swi_uint32 dlparam2,
                    swi_uint32 dlparam3,
                    swi_uint32 dlparam4 );

extern void dlmasterenable( struct dlusercb *dlcbkp,
                            swi_bool        state );

extern swi_uint16 dlpackidget(char    *userpackidp );

extern swi_bool dlusercbconfig( struct dlusercb *dlusercbp,
                                swi_bool        dlkeepmask );

extern char * dlpackidstrget(swi_uint16 pkgid);
extern void dllogenable(swi_bool state);
extern void dllogentoggle(void);
extern void dlinit(void);
