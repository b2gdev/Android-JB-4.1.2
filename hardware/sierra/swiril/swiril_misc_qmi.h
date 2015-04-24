/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes of miscellaneous QMI related Sierra functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#include "aa/aaglobal.h"
#include "SwiDataTypes.h"

#define NUM_MAX_STR_LEN 0xFF

void getIPAddressStr( ULONG IPAddress, char **ipaddrStrp );
void getIPAddresses2Str( ULONG IPAddress1, ULONG IPAddress2, char **ipaddrStrp );
void getIPV6AddressStr( USHORT * pIPV6Address, char **ipaddrStrp );
void getIPV6Addresses2Str( USHORT *pIPV6Address1, USHORT *pIPV6Address2, char **ipaddrStrp );
void getIPV4V6AddressesStr( ULONG IPAddress, USHORT *pIPV6Address, char **ipaddrStrp );
void getIPV4V6AddressesStr2( ULONG IPAddress1, 
                             ULONG IPAddress2, 
                             USHORT *pIPV6Address1, 
                             USHORT *pIPV6Address2, 
                             char **ipaddrStrp );
