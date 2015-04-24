/**
 *
 *  \file   SwiDataTypes.h
 *
 *  \brief  SWI data types
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 */

#ifndef __SWI_DATA_TYPES_H
#define __SWI_DATA_TYPES_H

#ifndef SWI_API
#define SWI_API
#endif /* SWI_API */

/* Define for LTE specific implementation */
/* Need to remove both the defines when the FW has support for LTE */
#define QMI_NO_LTE_FW_SUPPORT 0
#define QMI_TLV_PLACEHOLDER   0x8F

/* Type Definitions */
typedef unsigned long      ULONG;
typedef unsigned long long ULONGLONG;
typedef signed char        INT8;
typedef unsigned char      BYTE;
typedef char               CHAR;
typedef unsigned short     WORD;
typedef unsigned short     USHORT;
typedef const char *       LPCSTR;
typedef int                BOOL;
typedef signed short       SHORT;

/**
* Macro used to avoid “unused variable” compiler warnings generated due
* to the inclusion of the "-Wextra" flag in our make files.
*/
#define UNUSEDPARAM( x ) (void)x

#endif /* __SWI_DATA_TYPES_H */
