/*************
 *
 * Filename: qmparser.c
 *
 * Purpose:  qmi message parser
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "qmidefs.h"
#include "pi/piudefs.h"

#define QMPSR_LIST_LEN 10
#define QMPSR_NUM_PSRS 1
#define DBG_QMPARSER
#ifdef DBG_QMPARSER
#include <syslog.h>
#endif

struct qmimsgnode{
    swi_uint16 msgid;
    void *(*msgparser)(swi_uint8* pmsg);
};

struct qmpsrentry{
    enum eQMIService svc;
    void (*parser)(swi_uint8 *pmsg);
    struct qmimsgnode *list[QMPSR_LIST_LEN];
};

/* WDS */
local void* psrwds_stopnet(swi_uint8*);
local void* psrwds_starnet(swi_uint8*);
local void* psrwds_setipfamily(swi_uint8*);
local void qmparserwds(swi_uint8 *pmsg);

local struct qmimsgnode wdsn3 =
{
    eQMI_WDS_STOP_NET,
    psrwds_stopnet
};

local struct qmimsgnode wdsn2 =
{
    eQMI_WDS_START_NET,
    psrwds_starnet
};

local struct qmimsgnode wdsn1 =
{
    eQMI_WDS_SET_IP_FAMILY,
    psrwds_setipfamily
};

local struct qmpsrentry wdspsr = {
    eQMI_SVC_WDS,
    qmparserwds,
    { &wdsn1, &wdsn2, &wdsn3, NULL, NULL,
      NULL, NULL, NULL, NULL, NULL }
};

local struct qmpsrentry *qmparsers[QMPSR_NUM_PSRS] = {
    &wdspsr
};

local void* psrwds_stopnet(swi_uint8* pmsg)
{
#ifdef DBG_QMPARSER
    syslog( LOG_DEBUG, "%s:", __func__ );
#endif
   return pmsg; }

local void* psrwds_starnet(swi_uint8* pmsg)
{
#ifdef DBG_QMPARSER
    syslog( LOG_DEBUG, "%s:", __func__ );
#endif
   return pmsg;
}

local void* psrwds_setipfamily(swi_uint8* pmsg)
{
    /* extract ip family preference */
    swi_uint8 ipfp = *( pmsg + QMISVC_MSGLENSZ + QMITLV_TYPE_SZ +
                        QMITLV_LENGTH_SZ);

    if( IPv4_FAMILY_PREFERENCE == ipfp )
    {
        qmwdsclientset( QMIWDSCLNT );
    }
    else if( IPv6_FAMILY_PREFERENCE == ipfp )
    {
        qmwdsclientset( QMIWDSV6CLNT );
    }
#ifdef DBG_QMPARSER
    syslog( LOG_DEBUG,
            "%s: IP family: V%d Active WDS Client: %s",
            __func__,
            ipfp,
            QMIWDSCLNT == qmwdsclientget() ? "V4" : "V6" );
#endif
    return NULL;
}

local void qmparserwds(swi_uint8* pmsg)
{
    swi_uint16 msgid = piget16(&pmsg);
    int nnodes = sizeof(wdspsr.list)/sizeof(wdspsr.list[0]) ;
    int i;

    for( i = 0 ; i < nnodes && wdspsr.list[i] != NULL ; i++ )
    {
        if( msgid == wdspsr.list[i]->msgid )
        {
            /* QMI message parser found */
            wdspsr.list[i]->msgparser(pmsg);
            break;
        }
    }

}

package void qmparser(  enum eQMIService svc,
                        swi_uint8* pmsg )
{
    int numpsrs = sizeof(qmparsers)/sizeof(qmparsers[0]) ;
    int i;
    for( i = 0 ; i < numpsrs ; i++ )
    {
        if( svc == qmparsers[i]->svc )
        {
            /* QMI service parser found */
            qmparsers[i]->parser(pmsg);
            break;
        }
    }
}

