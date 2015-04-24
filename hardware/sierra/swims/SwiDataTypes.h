/**
 *
 * @ingroup swims
 *
 * @file
 * SWI data types
 *
 * @author
 * Copyright: © 2010 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */


#ifndef __SWI_DATA_TYPES_H
#define __SWI_DATA_TYPES_H

#ifndef SWI_API
#define SWI_API
#endif


typedef unsigned char      swi_uint8;   /*!< 8 bit integer unsigned   */
typedef unsigned short     swi_uint16;  /*!< 16 bit integer unsigned  */
typedef unsigned long      swi_uint32;  /*!< 32 bit integer unsigned  */
typedef unsigned long long swi_uint64;  /*!< 64 bit integer unsigned  */

typedef signed char        swi_int8;    /*!< 8 bit integer signed     */
typedef signed short       swi_int16;   /*!< 16 bit integer signed    */
typedef signed long        swi_int32;   /*!< 32 bit integer signed    */
typedef signed long long   swi_int64;   /*!< 64 bit integer signed    */

typedef int swi_bool;    /*!< boolean type, used for true/false. should 
                          *  only be tested for 0 or non-zero  
                          */

typedef char swi_char, *swi_charp;      /*!< char type */


/** Standard C defines a size_t type to be used for
 * passing around length variables between functions, etc.
 * The SDK needs a unversally-defined size_t that can be 
 * modified according to the architecture it will be
 * running on. This definition is suitable for 32-bit 
 * architectures
 */
typedef unsigned long      swi_size_t;

/**
 * The swi_ssize_t type is a return value from Linux calls
 * in some of the OS wrapper functions. Defined for the same 
 * reason swi_size_t is. According to online documentation
 * this is similar to size_t except that it must represent
 * a signed quantity
 */
typedef long               swi_ssize_t;

/**
 * The modem returns some strings in UCS2 format.
 * UCS2 is 16-bits max. The definition here is to aid in
 * copying UCS2 data from the modem
 */
typedef unsigned short     swi_UCS2;
typedef swi_UCS2           *swi_UCS2p;

/* Alignment operators */
#define ALIGN4 (sizeof(swi_uint32) -1)
#define ALIGN2 (sizeof(swi_uint16) -1)


/* function and variable scope modifiers */

#define local static  /*!< function/variable accessed within local file only */
#define package       /*!< accessable within package only, used for fns only */
#define global        /*!< function accessed globally, used for functions only */

/* note: currently package and global are only used manually,  
 * someday tools should make use of these indentifiers
 * ie. prototype generation tools
 */ 
 
 #define UTF8_MAX_CHAR_SIZE 4

#define YES 1
#define NO 0

#define TRUE 1
#define FALSE 0
#define EOS '\0'

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif /* __SWI_DATA_TYPES_H */

