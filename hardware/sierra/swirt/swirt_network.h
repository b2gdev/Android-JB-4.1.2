/**
 *
 * @ingroup swirt
 *
 * @file 
 * Public definitions and prototypes for swirt-network 
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#ifndef __SWIRT_NETWORK_H
#define __SWIRT_NETWORK_H


#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define PORT "9034"   // port we're listening on
#define ADRESS_IP "127.0.0.1"
#define MAXPENDING 5

#define SERVER_SOCKET 1
#define CLIENT_SOCKET 0

#define TRUE 1
#define FALSE 0
#define START 11
#define DIVIDER ":"

#define MAXDATASIZE 2048         // max number of bytes we can get at once, it may not be enough for AT!ERR or AT!GCDUMP 
#define MAX_READ_ITERATIONS 10   // max number of read for one packet

extern void *get_in_addr(struct sockaddr *sa);
extern int start_client(const char * server_IP);

#endif /* __SWIRT_NETWORK_H */

