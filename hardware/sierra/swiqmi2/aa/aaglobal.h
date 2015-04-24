/*************
 *
 *  Name:      aaglobal.h
 *
 *  Purpose:   Include file for typedefs required in the Sierra
 *             Wireless portion of the system. Third party code
 *             may not follow this standard.
 *
 *  Note:      These definitions are for use in the internal
 *             portion of the API. The definitions used by the public
 *             part are defined separately.
 *
 *             Do not add any definitions to this file. All new definitions
 *             should go into SwiDataTypes.h
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef aaglobal_h
#define aaglobal_h

/* Note: This file contains all global definitions that are also available
 * to the Application
 */

typedef unsigned char      swi_uint8;   /* 8  bit integer unsigned  */
typedef unsigned short     swi_uint16;  /* 16 bit integer unsigned  */
typedef unsigned long      swi_uint32;  /* 32 bit integer unsigned  */
typedef unsigned long long swi_uint64;  /* 64 bit integer unsigned  */

typedef signed char        swi_int8;    /* 8  bit integer signed    */
typedef signed short       swi_int16;   /* 16 bit integer signed    */
typedef signed long        swi_int32;   /* 32 bit integer signed    */
typedef signed long long   swi_int64;   /* 64 bit integer signed    */

typedef int swi_bool;    /* boolean type used for true/false. should only be
                          * tested for zero or non-zero
                          */

typedef char swi_char, *swi_charp;  /* char type */

/** Standard C defines a size_t type to be used for
 * passing around length variables between functions, etc.
 * The SDK needs a unversally-defined size_t that can be
 * modified according to the architecture it will be
 * running on. This definition is suitable for 32-bit
 * architectures
 */
typedef unsigned long   swi_size_t;

/**
 * The swi_ssize_t type is a return value from Linux calls
 * in some of the OS wrapper functions. Defined for the same
 * reason swi_size_t is. According to online documentation
 * this is similar to size_t except that it must represent
 * a signed quantity
 */
typedef long    swi_ssize_t;

/**
 * The modem returns some strings in UCS2 format.
 * UCS2 is 16-bits max. The definition here is to aid in
 * copying UCS2 data from the modem
 */
typedef unsigned short  swi_UCS2;
typedef swi_UCS2        *swi_UCS2p;

/* Alignment operators */
#define ALIGN4 (sizeof(swi_uint32) -1)
#define ALIGN2 (sizeof(swi_uint16) -1)

/**
 * Macro to use unused parameter in function. This is use to avoid the
 * warning caused by including "-Wextra" flag for unused variables during
 * compilation.
 */
#define UNUSEDPARAM( x ) (void)x

/* function and variable scope modifiers */
#define local static  /* function/variable accessed within local file only */
#define package       /* accessable within package only, used for fns only */
#define global        /* function accessed globally, used for functions only */

/* note: currently package and global are only used manually,
 * someday tools should make use of these indentifiers
 * ie. prototype generation tools
 */

#define UTF8_MAX_CHAR_SIZE 4

#define YES 1
#define NO 0

#define FALSE 0
#define TRUE !FALSE
#define EOS '\0'

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif /* aaglobal.h */
