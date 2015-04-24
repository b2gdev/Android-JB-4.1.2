/*
 *	Fileiname:	mmuproto.h
 *
 *	Purpose:	Prototypes for user-callable functions for the MM package
 *
 *	Copyright Sierra Wireless., 2008 All rights reserved
 *
 */
extern void mmplogging( struct mmpool *poolp, swi_bool logging);
extern void mmpcreate( struct mmpool *poolp, char *pnamep, swi_uint32 numblocks, 
                       swi_uint32 reqblocksz );
extern swi_uint8 *mmbufget( struct mmpool *poolp );
extern void mmbufrel( swi_uint8 *bufferp );
extern void mminit( void );
