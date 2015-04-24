/*************
 *
 * Filename:    amapi.c
 *
 * Purpose:     Image management API prototypes
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMAPI_H
#define IMAPI_H
extern ULONG GobiEnumerateQDLPorts(
    BYTE    *pPortSize,
    BYTE    *pPorts );

extern ULONG
SetQDLTimeout(
    ULONG   to );

extern ULONG 
OpenQDLPort(
    BYTE    portID,
    ULONG   bBARMode,
    ULONG   *pMajorVersion,
    ULONG   *pMinorVersion );

extern ULONG
CloseQDLPort(
    ULONG   bInformDevice );

extern ULONG
GetQDLImagePreference(
   ULONG    *pImageListSize,
   BYTE     *pImageList );

extern ULONG
PrepareQDLImageWrite(
    BYTE    imageType,
    ULONG   imageSize,
    ULONG   *pBlockSize );

extern ULONG
WriteQDLImageBlock(
    USHORT  sequenceNumber,
    ULONG   blockSize,
    BYTE    *pImageBlock );

extern ULONG
ValidateQDLImages(
    BYTE    *pImageType );
#endif /* IMAPI_H */

