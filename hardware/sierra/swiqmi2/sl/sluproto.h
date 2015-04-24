/*************
 *
 * Filename:    sluproto.h
 *
 * Purpose:     This has the public API for the string library
 *
 * Copyright:   © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef SLUPROTO_H
#define SLUPROTO_H

/* include files */
#include "aa/aaglobal.h"

/* String Library Function Prototypes */

extern swi_uint32 slahtol(const char *sp);
extern swi_uint16 slatoi(const char *sp);
extern swi_uint32 slatol(const char *sp);
extern swi_uint64 slatol64(const char *sp);
extern int slmemcmp(const void *s1p, const void *s2p, swi_uint32 length);
extern void *slmemcpy(void *destp, void *srcp, swi_uint32 length);
extern void slmemset(char *srcp, swi_uint8 value, swi_uint32 length);
extern int slstrcmp(const char *s1p, const char *s2p);
extern swi_uint32 slstrlen(const char *sp);
extern int slstrncmp(const char *s1p, const char *s2p, swi_uint32 length);
extern char *slstrncpy(char *destp, const char *srcp, swi_uint32 length);
extern char sltoupper(char c);
extern swi_bool slisprint(char c);
extern swi_uint8 slasciihextouint8(char *sp);
extern char *sluint8toasciidec(char *destp, swi_uint8 value);

extern char *sluint64toasciidec(char *destp, swi_uint32 valuehigh,
        swi_uint32 valuelow);

extern char *sluint16toasciihex(char *destp, swi_uint16 value);
extern char *sluint32toasciihex(char *destp, swi_uint32 value);
extern char *slstrtok(char *strtokp, const char *strdelimitp, struct slstrtokblk *strtokblkp);
extern swi_uint32 slvsprintflenc (
    char *bufp,
    const char *fmtp,
    char *argp,
    swi_uint32 maxlen,
    swi_uint32 maxargs);

extern swi_uint32 slsscanf(const char *strp, const char *fmtp, ...);
extern void slva_start(char **argpp, const char **fixpp);
extern void slva_end(char **argpp);
extern char *sluint8toasciihex(char *destp, swi_uint8 value);
extern int slstrnlen(const char * sp, int count);

extern swi_bool slUtf8ToUcs2( unsigned char *utf8stringp,
                              swi_UCS2      *usc2stringp,
                              swi_uint16    maxnumchars,
                              swi_uint16    *numcharsp );

extern swi_bool slUcs2ToUtf8( swi_UCS2      *ucs2stringp,
                              swi_uint16    numucs2chars,
                              unsigned char *utf8stringp,
                              swi_uint16    maxbytes );

extern swi_uint32 slcountfmtargs(const char *sp);
extern swi_ssize_t slstrlenutf8(const char * _s);

extern char *slstrstr( const swi_char *haystack,
                       const swi_char *needle);

swi_int32 slglob( const char    *path,
                  const slstr_t *filter,
                  const slstr_t *ignore,
                  slstr_t       *list,
                  const int     listsz );

#endif
