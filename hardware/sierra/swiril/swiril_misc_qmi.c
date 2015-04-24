/**
 *
 * @ingroup swiril
 *
 * @file 
 * Provides miscellaneous QMI Sierra specific functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#define LOG_TAG "RIL"
#include "swiril_log.h"
#include <stdlib.h>

#include "swiril_misc.h"
#include "swiril_misc_qmi.h"

/* Maximum possible String size for an IP address 
 * in dotted decimal notation, plus 2 extra bytes, 
 * one for NULL and one extra '.' character which 
 * gets overwritten with the final NULL
 */
#define MAXIPSTRSIZE 17
/* Maximum possible String size for an IPV6 address 
 * in colon-hexadecima notation, plus 2 extra bytes, 
 * one for NULL and one extra ':' character which 
 * gets overwritten with the final NULL
 */
#define MAXIPV6STRSIZE 41

#define UNSPECIFIED_ADDRESS_V6 "0:0:0:0:0:0:0:0"

/**
 *
 * Get the IP address for the current connection and convert
 * it into a string in proper dotted-decimal notation. 
 *
 * @param IPAddress 
 *     Address in unsigned long format to convert to ASCII
 * @param [out]
 *     ipaddrStrp pointer to pointer to storage for the allocated string 
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     The caller function is responsible to free the memory
 */
void getIPAddressStr( ULONG IPAddress, char **ipaddrStrp )
{
    unsigned char octet;
    int i, written = 0;
    char *strp;

    /* Allocate memory for the string. Caller must remember to free it */
    strp = swimalloc(MAXIPSTRSIZE, "getIPAddressStr memory allocation failure");

    /* Convert it into a string */
    for(i=4; i!=0; i--) {
        /* Start with most significant byte and work downward */
        octet = (IPAddress >> 8*(i-1)) & 0xFF;
        written += snprintf( strp+written, MAXIPSTRSIZE-written, "%d.", octet );
    }
    /* Clear out the final trailing '.' character */
    strp[written-1] = '\0';

    /* Assign the return value */
    *ipaddrStrp = strp;
}

/**
 *
 * Get the 2 IP addresses for the current connection and convert
 * it into a string in proper dotted-decimal notation. 
 *
 * @param IPAddress1 
 *     Address in unsigned long format to convert to ASCII
 * @param IPAddress2 
 *     Address in unsigned long format to convert to ASCII
 * @param [out]
 *     ipaddrStrp pointer to pointer to storage for the allocated string 
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     The caller function is responsible to free the memory
 */
void getIPAddresses2Str( ULONG IPAddress1, ULONG IPAddress2, char **ipaddrStrp )
{
    char *strp = NULL;
    char *strtmp = NULL;

    /* Allocate memory for the string. Caller must remember to free it */
    strp = swimalloc((MAXIPSTRSIZE * 2) + 1, "getIPAddresses2Str memory allocation failure");

    /* Convert first address into a string */
    getIPAddressStr(IPAddress1, &strtmp);
    strncpy(strp, strtmp, MAXIPSTRSIZE);
    free(strtmp);
    /* append a space */
    strcat(strp, " ");
    /* Convert second address into a string and append to return string */
    getIPAddressStr(IPAddress2, &strtmp);
    strcat(strp, strtmp);
    free(strtmp);
    
    /* Assign the return value */
    *ipaddrStrp = strp;
}

/**
 *
 * Get the IPV6 address for the current connection and convert
 * it into a string in proper colon-hexadecima notation. 
 *
 * @param [in] 
 *     Address in 16 bit format to convert to ASCII
 * @param [out]
 *     ipaddrStrp pointer to pointer to storage for the allocated string 
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     The caller function is responsible to free the memory
 */
void getIPV6AddressStr( USHORT *pIPV6Address, char **ipaddrStrp )
{
    int i, written = 0;
    char *strp;

    /* Allocate memory for the string. Caller must remember to free it */
    strp = swimalloc(MAXIPV6STRSIZE, "getIPV6AddressStr memory allocation failure");

    /* Convert it into a string */
    if (pIPV6Address == NULL) {
        strncpy(strp, UNSPECIFIED_ADDRESS_V6, MAXIPV6STRSIZE);
    }
    else {
        for(i=0; i <8; i++) {
            /* process as 8 element of 16 bit */
            written += snprintf( strp+written, MAXIPV6STRSIZE-written, "%x:", *pIPV6Address++);
        }
        /* Clear out the final trailing ':' character */
        strp[written-1] = '\0';
    }

    /* Assign the return value */
    *ipaddrStrp = strp;
}

/**
 *
 * Get the 2 IPV6 addresses for the current connection and convert
 * it into a string in proper colon-hexadecima notation. 
 *
 * @param [in] 
 *     Address1 in 16 bit format to convert to ASCII
 * @param [in] 
 *     Address2 in 16 bit format to convert to ASCII
 * @param [out]
 *     ipaddrStrp pointer to pointer to storage for the allocated string 
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     The caller function is responsible to free the memory
 */
void getIPV6Addresses2Str( USHORT *pIPV6Address1, USHORT *pIPV6Address2, char **ipaddrStrp )
{
    char *strp = NULL;
    char *strtmp = NULL;

    /* Allocate memory for the string. Caller must remember to free it */
    strp = swimalloc((MAXIPV6STRSIZE * 2) + 1, "getIPAddresses2Str memory allocation failure");

    /* Convert first address into a string */
    getIPV6AddressStr(pIPV6Address1, &strtmp);
    strncpy(strp, strtmp, MAXIPV6STRSIZE);
    free(strtmp);
    /* append a space */
    strcat(strp, " ");
    /* Convert second address into a string and append to return string */
    getIPV6AddressStr(pIPV6Address2, &strtmp);
    strcat(strp, strtmp);
    free(strtmp);
    
    /* Assign the return value */
    *ipaddrStrp = strp;
}

/**
 *
 * Get the IPV4 and IPV6 addresses for the current connection and convert
 * it into a string in proper dotted-decimal and colon-hexadecima notation. 
 *
 * @param [in] 
 *     IPV4 address in unsigned long format to convert to ASCII
 * @param [in] 
 *     IPV6 address in 16 bit format to convert to ASCII
 * @param [out]
 *     ipaddrStrp pointer to pointer to storage for the allocated string 
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     The caller function is responsible to free the memory
 */
void getIPV4V6AddressesStr( ULONG IPAddress, USHORT *pIPV6Address, char **ipaddrStrp )
{
    char *strp = NULL;
    char *strtmp = NULL;

    /* Allocate memory for the string. Caller must remember to free it */
    strp = swimalloc((MAXIPSTRSIZE + MAXIPV6STRSIZE) + 1, "getIPV4V6AddressesStr memory allocation failure");

    /* Convert IPV4 address into a string */
    getIPAddressStr(IPAddress, &strtmp);
    strncpy(strp, strtmp, MAXIPSTRSIZE);
    free(strtmp);
    /* append a space */
    strcat(strp, " ");
    /* Convert IPV6 address into a string and append to return string */
    getIPV6AddressStr(pIPV6Address, &strtmp);
    strcat(strp, strtmp);
    free(strtmp);
    
    /* Assign the return value */
    *ipaddrStrp = strp;
}

/**
 *
 * Get the 2 IPV4 and IPV6 addresses for the current connection and convert
 * it into a string in proper dotted-decimal and colon-hexadecima notation. 
 *
 * @param [in] 
 *     IPV4 address1 in unsigned long format to convert to ASCII
 * @param [in] 
 *     IPV4 address2 in unsigned long format to convert to ASCII
 * @param [in] 
 *     IPV6 address1 in 16 bit format to convert to ASCII
 * @param [in] 
 *     IPV6 address2 in 16 bit format to convert to ASCII
 * @param [out]
 *     ipaddrStrp pointer to pointer to storage for the allocated string 
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     The caller function is responsible to free the memory
 */
void getIPV4V6AddressesStr2( ULONG IPAddress1, ULONG IPAddress2, 
                             USHORT *pIPV6Address1, USHORT *pIPV6Address2, 
                             char **ipaddrStrp )
{
    char *strp = NULL;
    char *strtmp = NULL;

    /* Allocate memory for the string. Caller must remember to free it */
    strp = swimalloc((MAXIPSTRSIZE + MAXIPV6STRSIZE) * 2 + 3, "getIPV4V6AddressesStr2 memory allocation failure");

    /* Convert IPV4 address1 into a string */
    getIPAddressStr(IPAddress1, &strtmp);
    strncpy(strp, strtmp, MAXIPSTRSIZE);
    free(strtmp);
    /* append a space */
    strcat(strp, " ");
    /* Convert IPV4 address2 into a string and append to return string */
    getIPAddressStr(IPAddress2, &strtmp);
    strcat(strp, strtmp);
    free(strtmp);
    /* append a space */
    strcat(strp, " ");
    /* Convert IPV6 address1 into a string and append to return string */
    getIPV6AddressStr(pIPV6Address1, &strtmp);
    strcat(strp, strtmp);
    free(strtmp);
    /* append a space */
    strcat(strp, " ");
    /* Convert IPV6 address2 into a string and append to return string */
    getIPV6AddressStr(pIPV6Address2, &strtmp);
    strcat(strp, strtmp);
    free(strtmp);
    
    /* Assign the return value */
    *ipaddrStrp = strp;
}
