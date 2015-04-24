/************
 *
 * Filename:  smuproto.h
 *
 * Purpose:   external prototypes for sm
 *
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 ************/
extern void smvalidate(
    const sm_t *sm );

extern void smrun(
    sm_t *sm,
    const swi_uint32 ev );

extern void sminit(
    void );
