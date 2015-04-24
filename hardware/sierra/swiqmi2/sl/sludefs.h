/*************
 *
 * Filename:    sludefs.h
 *
 * Purpose:     Public include file for the String Library (sl) package
 *
 * Copyright:   © 2011 Sierra Wireless Inc.,  all rights reserved
 *
 **************/
#ifndef SLUDEFS_H
#define SLUDEFS_H

/* Include files */
#include "aa/aaglobal.h"

/* Local constants and enumerated types */
#define    SLCR    0x0D /* carriage return */
#define    SLLF    0x0A /* line feed */

/* structures */
/*************
 *
 * Name:    slstrtokblk
 *
 * Purpose: This structure contains the pointer to where the untokenized
 *          character sequence is located.
 *
 * Members: (IN)slstrtokevalp - points to first character after the delimiter.
 *                              Used to evaluate whether the sequence of
 *                              characters after the delimiter is a token.
 *
 * Notes:
 *
 **************/
struct slstrtokblk
{
    swi_char *slstrtokevalp;
};

/*************
 *
 * Name:     slstr_s
 *
 * Purpose:  User type to wrap a c string type
 *
 * Members:  str    - c string
 *
 * Notes:
 *
 **************/
typedef struct slstr_s
{
    swi_char str[512];
}slstr_t;

#include "sluproto.h"
#endif /* SLUDEFS_H */
