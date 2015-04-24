/*************
 *
 * Filename: swi_ossdkcheck.c
 *
 * Purpose: Device Detection/Scanning algorithms for the SWI OS Wrapper layer
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "aa/aaglobal.h"        /* system wide typedefs */
#include "swi_ossdk.h"          /* package user include file */
#include "sl/sludefs.h"
#include "dl/dludefs.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/prctl.h>
#define SWI_USB_MAX_PATH 256
#define SWI_USB_MAX_FILENAME 32

/* Encryption key used for hiding vendor IDs from scrutiny */
#define SWI_VENDORID_ENCRYPT_KEY 0x7679

/* directory parse structure */
struct swi_osdirparse {
    swi_uint32 *osdirvendorlistp; /* pointer to list of vendors */
    swi_uint32 osdirvendorid;     /* vendor id, if found, or zero */
    swi_uint32 osdirproductid;    /* product id */
    swi_uint32 osdirbNumInterfaces; /* Number of USB interfaces */
    const char *osdirtopdevicep;  /* name of top-level device (not EPs) */
    swi_uint32 osnumreadeps;      /* number of read endpoints in subdevice */
    swi_uint32 osdirreadeps[SWI_OS_MAXENDPOINTS];   /* read EPs in subdevice */
    swi_uint32 osnumwriteeps;                       /* # of write EPs in subdevice */
    swi_uint32 osdirwriteeps[SWI_OS_MAXENDPOINTS];  /* write EPs in subdev */
    swi_uint32 osdirnumttys;                        /* # of ttyUSBs in subdevice */
    swi_uint32 osdirttys[SWI_OS_MAXENDPOINTS];      /* ttyUSBs in subdevice */
    swi_uint32 osdirnumqcqmis;    /* # of qcqmis in subdevice */
    swi_uint32 osdirqcqmi[SWI_OS_MAXENDPOINTS];   /* qcqmis in subdevice */
    swi_uint32 osdirifnum;                  /* interface number in subdevice */

    struct swi_endpointtty osdirreadendpointtty[SWI_OS_MAXENDPOINTS];   /* ttyUSBn for read EP */
    struct swi_endpointtty osdirwriteendpointtty[SWI_OS_MAXENDPOINTS];  /* ttyUSBn for write EP */
    struct swi_endpointqcqmi osdirreadendpointqcqmi[SWI_OS_MAXENDPOINTS];   /* qcqmi<n> for read EP */
    struct swi_endpointqcqmi osdirwriteendpointqcqmi[SWI_OS_MAXENDPOINTS];  /* qcqmi<n> for write EP */
};

extern void swi_sdkusbioerrorlocked(struct swi_osusb *up);
extern void swi_sdkusbioerrorunlocked(struct swi_osusb *up);

/* Default list of Vendor IDs to use, applies when callers opt
 * not to specify their own list on system startup. BZ 12553
 */
static swi_uint32 osdfltvendorids[SWI_OS_MAXIDS] =
{
    (0x1199 ^ SWI_VENDORID_ENCRYPT_KEY),
    (0x0F3D ^ SWI_VENDORID_ENCRYPT_KEY),
    (0x03F0 ^ SWI_VENDORID_ENCRYPT_KEY),
     0x0
};

swi_uint32 osvendorid[] =
{
    0x1199,
    0
};

/* Space to store list of vendor IDs specified by a caller
 * on system startup (optional).
 */
static swi_uint32 oscalrvendorids[SWI_OS_MAXIDS];

/* Pointer to the default list of vendor IDs defined above
 * unless overridden by a caller during system startup
 */
static swi_uint32 *vendoridp = osdfltvendorids;

/*************
 *
 * Name: swi_ossdksetlist
 *
 * Purpose: Allows a caller-supplied list of Vendor IDs to be configured
 *          for this session. Calling this function overrides the built-in
 *          list of vendor IDs which is installed by default
 *
 * Params:  idlistp - Pointer to a NULL-terminated list of vendor IDs that
 *                    this module will use to search for USB modems in the
 *                    sysfs file system.
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   This function copies the caller's list items into the
 *          statically defined one and adds an encryption to them, then
 *          it overrides the default list by writing a pointer to the
 *          caller's list into "vendoridp"
 *
 **************/
package void swi_ossdksetlist( swi_uint32 *idlistp )
{
    int i;

    /* Copy each user's entry into our local list,
     * disguising it with our encryption xor value
     * along the way
     */
    for (i = 0; i < SWI_OS_MAXIDS && *idlistp; i++)
    {
        oscalrvendorids[i] = (*idlistp ^ SWI_VENDORID_ENCRYPT_KEY);
        idlistp += 1;
    }

    /* Make sure we haven't filled the whole array. We need at least
     * one entry reserved for a NULL termination
     */
    if( i == SWI_OS_MAXIDS )
        i -= 1;

    /* Put in the NULL list entry, signifying the end of the list */
    oscalrvendorids[i] = 0;

    /* Finally, install the pointer to the list */
    vendoridp = oscalrvendorids;
}

/*************
 *
 * Name: swi_sdkfdclose
 *
 * Purpose: Closes a file descriptor.
 *
 * Params:  None
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
package void swi_sdkfdclose(int fd)
{
    int rv;

    do {
        rv = close(fd);        /* close file descriptor */
    } while (rv < 0 && errno == EINTR);
}

/*************
 *
 * Name: swi_osdirfile
 *
 * Purpose: Read the entire contents of a file as a string.
 *
 * Params:  pathp - path name of file
 *          bufp - storage for string
 *          bufsize - size of bufp
 *
 * Return:  True if file exists and can be read.
 *
 * Abort:   none
 *
 * Notes:   bufp will always contain a proper string if bufsize > 0.
 *          File will be truncated if necessary.
 *
 **************/
local swi_bool swi_osdirfile(const char *pathp, char *bufp, swi_size_t bufsize)
{
    int fd;
    int rrv = 0;
    char *bend;

    if (pathp == NULL || bufsize == 0)
        return FALSE;

    fd = open(pathp, O_RDONLY);
    if (fd < 0)
        return FALSE;

    bend = bufp + bufsize - 1;

    while (bufp < bend)
    {
        rrv = read(fd, bufp, bend - bufp);

        if (rrv < 0 && errno != EINTR)    /* that's bad */
            break;

        else if (rrv == 0)    /* if end of file */
            break;

        else        /* can be partial read due to signal */
            bufp += rrv;
    }

    *bufp = '\0';

    swi_sdkfdclose(fd);

    return rrv == 0;    /* success if end of file reached */
}

/*************
 *
 * Name: swi_osdirmap
 *
 * Purpose: Work through a directory, calling a function for each file
 *          that doesn't start with a '.'
 *
 * Params:  dpp   - swi_oswi_osdirparse structure pointer, just passed through
 *          pathp - path name of directory
 *          fnp   - function, defined as:
 *                  swi_bool (*fnp)(struct swi_osdirparse *dpp,
 *                              const char *pathp,
 *                              const char *filep)
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   If (*fnp)() returns TRUE, directory scan is stopped
 *
 **************/
local void swi_osdirmap(struct swi_osdirparse *dpp,
                const char *pathp,
                swi_bool (*fnp)(struct swi_osdirparse *dpp,const char *pathp,
                                            const char *filep))
{
    DIR *dirp;
    struct dirent *dep;
    int rv;

    if (fnp == NULL)
        return;

    dirp = opendir(pathp);
    if (dirp == NULL)
        return;

    for ( ; ; )
    {
        dep = readdir(dirp);

        if (dep == NULL)
            break;

        if (dep->d_name[0] != '\0' && dep->d_name[0] != '.') /* not ".." etc. */
        {
            if ((*fnp)(dpp, pathp, dep->d_name))   /* TRUE means early end */
                break;
        }
    }

    do
    {
        rv = closedir(dirp);
    } while (rv < 0 && errno == EINTR);

}

/*************
 *
 * Name: swi_osdiratoi
 *
 * Purpose: Convert an ascii string to a swi_uint32
 *
 * Params:  str  - pointer to the string
 *          base - numeric base
 *
 * Return:  Integer value
 *
 * Abort:   none
 *
 * Notes:   Invalid characters truncate the integer
 *
 **************/
local swi_uint32 swi_osdiratoi(const char *str, swi_uint32 base)
{
    swi_uint32 rv;
    swi_uint32 cv;

    while (*str && *str <= ' ')    /* while leading white */
        str += 1;

    rv = 0U;

    while (*str)
    {

        if (*str >= '0' && *str <= '9')
            cv = *str - '0';

        else if (*str >= 'a' && *str <= 'z')
            cv =  *str - 'a' + 10;

        else if (*str >= 'A' && *str <= 'Z')
            cv = *str - 'A' + 10;

        else            /* weird character */
            break;

        if (cv < base)
        {
            rv *= base;
            rv += cv;
        }
        else            /* char not in numeric range */
            break;

        str += 1;
    }

    return rv;
}

/*************
 *
 * Name: swi_osdirisprefix
 *
 * Purpose: Determine if one string is a prefix to another, ignoring case
 *
 * Params:  prefixp - pointer to the prefix string
 *          teststrp - pointer to string to be tested
 *
 * Return:  True if the string pointed to by prefixp is a prefix of the
 *          string pointed to by teststrp, ignoring case
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local swi_bool swi_osdirisprefix(const char *prefixp, const char *teststrp)
{
    while (*prefixp != EOS && tolower(*prefixp) == tolower(*teststrp))
    {
        prefixp += 1;
        teststrp += 1;
    }

    return *prefixp == EOS;
}

/*************
 *
 * Name: swi_osdirsortuint
 *
 * Purpose: Sort an array of numerical values in ascending order
 *
 * Params:  size - number of entries in the array
 *          array - pointer to the array of values
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   Intended for very small arrays
 *
 **************/
local void swi_osdirsortuint(swi_uint32 size, swi_uint32 *arrayp)
{
    swi_uint32 i, j, temp;

    for (i = 1; i < size; i++)
    {
        temp = arrayp[i];
        j = i;

        while ((j > 0) && (arrayp[j - 1] > temp))
        {
            arrayp[j] = arrayp[j - 1];
            j = j - 1;
        }

        arrayp[j] = temp;
    }
}


 /*************
 *
 * Name: swi_osdirepscan
 *
 * Purpose: Called for each file/directory in modem subdevice directory.
 *          Looks for directories of the form "ep_xx" (for endpoints) and
 *          "ttyUSBxx" (for ttys). Ignores other file names and endpoints
 *          that are not of type "Bulk."  Stores the endpoint or tty number
 *          in arrays in the swi_osdirparse structure.
 *
 * Params:  dpp   - pointer to swi_osdirparse structure
 *          pathp - pointer to string that has directory name
 *                    ("xxxx:yyyy")
 *          filep - name of file/directory ("ep_xx" or "ttyUSBxx" or
 *                  something else
 *
 * Return:  FALSE, always
 *
 * Abort:   None (almost anything can happen when modem crashes)
 *
 * Notes:   None
 *
 **************/
local swi_bool swi_osdirepscan( struct swi_osdirparse *dpp,
                                const char *pathp,
                                const char *filep )
{
    char newpathp[SWI_USB_MAX_PATH];
    char filestring[SWI_USB_MAX_FILENAME];
    swi_uint32 devno;
    swi_uint32 ifnum;

    if (swi_osdirisprefix("ep_", filep))
    {
        /* get type of endpoint */
        snprintf(newpathp, sizeof(newpathp), "%s/%s/type", pathp, filep);

        if( !swi_osdirfile(newpathp, filestring, sizeof(filestring)) ||
            !swi_osdirisprefix("Bulk", filestring) )
        {
                return FALSE;
        }

        devno = swi_osdiratoi(&filep[3], 16);    /* endpoint number */
        if ((devno & 0x80) != 0) /* if input device */
        {
            devno &= ~0x80; /* clear flag bit */
            if( devno < SWI_OS_MAXENDPOINTS && dpp->osnumreadeps < SWI_OS_MAXENDPOINTS )
            {
                dpp->osdirreadeps[dpp->osnumreadeps] = devno;
                dpp->osnumreadeps += 1;
            }
        }
        else if (devno < SWI_OS_MAXENDPOINTS && dpp->osnumwriteeps < SWI_OS_MAXENDPOINTS)
        {
            dpp->osdirwriteeps[dpp->osnumwriteeps] = devno;
            dpp->osnumwriteeps += 1;
        }
    }
    else if (swi_osdirisprefix("ttyUSB", filep))
    {
        devno = swi_osdiratoi(&filep[6], 10);    /* ttyUSB numbers are in decimal */
        if( devno < SWI_OS_MAXUSBTTYS && dpp->osdirnumttys < SWI_OS_MAXENDPOINTS )
        {
            dpp->osdirttys[dpp->osdirnumttys] = devno;
            dpp->osdirnumttys += 1;
        }
    }
    else if (swi_osdirisprefix("net:usb", filep) ||
             swi_osdirisprefix("net:eth", filep) ) /* kernel 3.0+ */
    {
        devno = swi_osdiratoi(&filep[7], 10);    /* qcqmi number */
        if( devno < SWI_OS_MAXQCQMIS && dpp->osdirnumqcqmis < SWI_OS_MAXENDPOINTS )
        {
            char qcqmi[12];
            snprintf( qcqmi, sizeof(qcqmi), "/dev/qcqmix");
            qcqmi[strlen(qcqmi) - 1] = filep[7];

            struct stat buf;
            if( !stat(qcqmi, &buf ) )
            {
                dpp->osdirqcqmi[dpp->osdirnumqcqmis] = devno;
                dpp->osdirnumqcqmis++;
            }
#ifdef DBG_DEVSCAN
            {
                char msg[256];
                snprintf( msg, sizeof(msg), "%d: %s: net:usb - %lu  numqcqmis: %lu",
                          __LINE__, __func__, devno, dpp->osdirnumqcqmis );
                swi_ossdklog(msg);
            }
#endif /* DBG_DEVSCAN */
        }
    }
    else if (swi_osdirisprefix("net", filep))
    {
#ifdef DBG_DEVSCAN
            {
                char msg[256];
                snprintf( msg, sizeof(msg), "%d: %s: net - %s",
                          __LINE__, __func__, filep );
                swi_ossdklog(msg);
            }
#endif /* DBG_DEVSCAN */
        snprintf(newpathp, sizeof(newpathp), "%s/%s", pathp, filep);
#ifdef DBG_DEVSCAN
            {
                char msg[256];
                snprintf( msg, sizeof(msg), "%d: %s: net - %s",
                          __LINE__, __func__, newpathp );
                swi_ossdklog(msg);
            }
#endif /* DBG_DEVSCAN */
        swi_osdirmap(dpp, newpathp, swi_osdirepscan);
    }
    else if (swi_osdirisprefix("usb", filep) ||
             swi_osdirisprefix("eth", filep) ) /* kernel 3.0+ */
    {
        devno = swi_osdiratoi(&filep[3], 10);    /* qcqmi number */
        if( devno < SWI_OS_MAXQCQMIS && dpp->osdirnumqcqmis < SWI_OS_MAXENDPOINTS )
        {
            char qcqmi[12];
            snprintf( qcqmi, sizeof(qcqmi), "/dev/qcqmix");
            qcqmi[strlen(qcqmi) - 1] = filep[3];

            struct stat buf;
            if( !stat(qcqmi, &buf ) )
            {
                dpp->osdirqcqmi[dpp->osdirnumqcqmis] = devno;
                dpp->osdirnumqcqmis++;
            }
#ifdef DBG_DEVSCAN
            {
                char msg[256];
                snprintf( msg, sizeof(msg), "%d: %s: net/usb - %lu  numqcqmis: %lu",
                          __LINE__, __func__, devno, dpp->osdirnumqcqmis );
                 swi_ossdklog(msg);
            }
#endif /* DBG_DEVSCAN */
        }
    }
    else if (swi_osdirisprefix("bInterfaceNumber", filep))
    {
        snprintf(newpathp, sizeof(newpathp), "%s/%s", pathp, filep);

        if (!swi_osdirfile(newpathp, filestring, sizeof(filestring)))
            return FALSE;    /* if can't read interface number file */

        ifnum = swi_osdiratoi(filestring, 16);
        dpp->osdirifnum = ifnum;
    }

    return FALSE;
}

/*************
 *
 * Name: swi_osdisubscan
 *
 * Purpose: Called for each file/directory in SWI modem top-level directory.
 *          Looks for directories of the form xxxx:yyyy, where xxxx is
 *          the name of the SWI modem top-level directory; ignores other
 *          file names. Then recursively parses correct subdirectories.
 *          Then sorts the list of endpoints and ttyUSBs found by each
 *          recursive parse and adds them to the enpoint->tty mapping tables.
 *
 * Params:  dpp   - pointer to swi_osdirparse structure
 *          pathp - pointer to string that has directory name
 *                  (xxx - the SWI modem top-level directory, of form
 *                  1-2 or 1-2.3 etc., depending on whether modem
 *                  connects through a hub or hubs)
 *          filep - name of file/directory
 *
 * Return:  FALSE, always.
 *
 * Abort:   None (almost anything can happen when modem crashes)
 *
 * Notes:   None
 *
 **************/
local swi_bool swi_osdirsubscan( struct swi_osdirparse *dpp,
                                 const char *pathp,
                                 const char *filep )
{
    char newpathp[SWI_USB_MAX_PATH];
    swi_uint32 epidx;
    swi_uint32 ttyidx;
    swi_uint32 qcqmiidx;

    if (!swi_osdirisprefix(dpp->osdirtopdevicep, filep))
        return FALSE;           /* file name not of form xxxx:yyyy */

    dpp->osnumreadeps = 0;      /* no read endpoints found for this subdevice */
    dpp->osnumwriteeps = 0;     /* no write endpoints found for this subdevice */
    dpp->osdirnumttys = 0;      /* no ttyUSBs found for this subdevice */
    dpp->osdirifnum = *(filep + slstrlen(filep) - 1) - 0x30;
    dpp->osdirnumqcqmis = 0;    /* no qcqmis found for this subdevice */

    /* scan the sub directory */
    snprintf(newpathp, sizeof(newpathp), "%s/%s", pathp, filep);

    swi_osdirmap(dpp, newpathp, swi_osdirepscan);

    /* sort the endpoints and ttyUSBs in ascending order */
    swi_osdirsortuint(dpp->osnumreadeps, dpp->osdirreadeps);
    swi_osdirsortuint(dpp->osnumwriteeps, dpp->osdirwriteeps);
    swi_osdirsortuint(dpp->osdirnumttys, dpp->osdirttys);
    /* sort the qcqmis in ascending order */
    swi_osdirsortuint(dpp->osdirnumqcqmis, dpp->osdirqcqmi);

    /* assign ttys to read/write endpoints (only one will be found for */
    /* composite devices but several will be found for non-composite -- */
    /* the only way to match ebs/ttys up is in numerical order) */
    ttyidx = 0;
    qcqmiidx = 0;
#ifdef DBG_DEVSCAN
    char msg[256];
    snprintf(msg, sizeof(msg), "numrdeps: %lu numwreps: %lu numttys: %lu nuqcqmis: %lu",
    dpp->osnumreadeps, dpp->osnumwriteeps, dpp->osdirnumttys, dpp->osdirnumqcqmis);
    swi_ossdklog(msg);
#endif /* DBG_DEVSCAN */
    for (epidx = 0; epidx < dpp->osnumreadeps; epidx++)
    {
        if (ttyidx < dpp->osdirnumttys)
        {
            struct swi_endpointtty * ep =
                &dpp->osdirreadendpointtty[dpp->osdirreadeps[epidx]];
            ep->osusbttynum = dpp->osdirttys[ttyidx];
            ep->osusbifnum  = dpp->osdirifnum;
            ttyidx += 1;
#ifdef DBG_DEVSCAN
            snprintf(msg, sizeof(msg), "%d: %s: osdirreadenpointtty: %lu osusbttynum: %lu osusbifnum: %lu",
            __LINE__, __func__, dpp->osdirreadeps[epidx], ep->osusbttynum, ep->osusbifnum);
            swi_ossdklog(msg);
#endif /* DBG_DEVSCAN */
        }
        /* assign qcqmi device instance to read endpoints */
        if (qcqmiidx < dpp->osdirnumqcqmis)
        {
            struct swi_endpointqcqmi *ep = &dpp->osdirreadendpointqcqmi
                                                [dpp->osdirreadeps[epidx]];

            ep->osusbqcqminum = dpp->osdirqcqmi[qcqmiidx];
            ep->osusbifnum    = dpp->osdirifnum;
            qcqmiidx++;
#ifdef DBG_DEVSCAN
            snprintf(msg, sizeof(msg), "%d: %s: osdirreadenpointqcqmi: %lu osusbqcqminum: %lu osusbifnum: %lu",
            __LINE__, __func__, dpp->osdirreadeps[epidx], ep->osusbqcqminum, ep->osusbifnum);
            swi_ossdklog(msg);
#endif /* DBG_DEVSCAN */
        }
    }

    ttyidx = 0;
    qcqmiidx = 0;
    for (epidx = 0; epidx < dpp->osnumwriteeps; epidx++)
    {
        if (ttyidx < dpp->osdirnumttys)
        {
            struct swi_endpointtty * ep =
                &dpp->osdirwriteendpointtty[dpp->osdirwriteeps[epidx]];

            ep->osusbttynum = dpp->osdirttys[ttyidx];
            ep->osusbifnum  = dpp->osdirifnum;
            ttyidx += 1;
#ifdef DBG_DEVSCAN
            snprintf(msg, sizeof(msg), "%d: %s: osdirwriteenpointtty: %lu osusbttynum: %lu osusbifnum: %lu",
            __LINE__, __func__, dpp->osdirwriteeps[epidx], ep->osusbttynum, ep->osusbifnum); swi_ossdklog(msg);
#endif /* DBG_DEVSCAN */
        }
        /* assign qcqmi device instance to write endpoints */
        if (qcqmiidx < dpp->osdirnumqcqmis)
        {
            struct swi_endpointqcqmi *ep = &dpp->osdirwriteendpointqcqmi
                                                [dpp->osdirwriteeps[epidx]];

            ep->osusbqcqminum = dpp->osdirqcqmi[qcqmiidx];
            ep->osusbifnum    = dpp->osdirifnum;
            qcqmiidx++;
#ifdef DBG_DEVSCAN
            snprintf(msg, sizeof(msg), "%d: %s: osdirwriteenpointqcqmi: %lu osusbqcqminum: %lu osusbifnum: %lu",
            __LINE__, __func__, dpp->osdirwriteeps[epidx], ep->osusbqcqminum, ep->osusbifnum); swi_ossdklog(msg);
#endif /* DBG_DEVSCAN */
        }
    }

    return FALSE;
}

/*************
 *
 * Name: swi_osdirdevscan
 *
 * Purpose: Called for each file/directory in /sys/bus/usb/devices
 *          Looks for first directory (i.e., device) that has a
 *          vendor ID file that is on the list in swi_osdirparse structure.
 *          Then that device is recursively parsed.
 *
 * Params:  dpp   - pointer to swi_osdirparse structure
 *          pathp - pointer to string that has directory name
 *                  ("/sys/bus/usb/devices")
 *
 *          filep - name of file/directory
 *
 * Return:  TRUE if SWI modem found, FALSE if not
 *
 * Abort:   None (almost anything can happen when modem crashes)
 *
 * Notes:   None
 *
 **************/
local swi_bool swi_osdirdevscan( struct swi_osdirparse *dpp,
                                 const char *pathp,
                                 const char *filep )
{
    char newpathp[SWI_USB_MAX_PATH];
    char filestring[SWI_USB_MAX_FILENAME];
    swi_uint32 idvendval;
    swi_uint32 idprodval;
    swi_uint32 *vp;

    if (filep[0] < '0' || filep[0] > '9')    /* if not a USB device name */
        return FALSE;

    snprintf(newpathp, sizeof(newpathp), "%s/%s/idVendor", pathp, filep);
            /* full path to /sys/bus/usb/devices/.../idVendor   */
            /* filep will have a name like "4-2" or "4-2.4" */

    if (!swi_osdirfile(newpathp, filestring, sizeof(filestring)))
        return FALSE;                    /* if can't read idVendor file */

    /* Vendor ID value from the just-read file, encrypted */
    idvendval = swi_osdiratoi(filestring, 16);

    /* Make a local copy of the vendor ID list pointer */
    vp = vendoridp;

    /* Compare the received vendor ID with our list items */
    while (*vp != 0)
    {
        if ((*vp ^ SWI_VENDORID_ENCRYPT_KEY) == idvendval)
            break;

        vp += 1;
    }

    if (*vp == 0)        /* if device not on vendor list */
        return FALSE;

    /* we found a device with a vendor ID on the list */

    snprintf(newpathp, sizeof(newpathp), "%s/%s/idProduct", pathp, filep);
            /* full path to /sys/bus/usb/devices/.../idProduct   */

    if (!swi_osdirfile(newpathp, filestring, sizeof(filestring)))
        return FALSE;                    /* if can't read idProduct file */

    idprodval = swi_osdiratoi(filestring, 16);    /* product value */

    if (idprodval == 0)    /* if zero product value, that's impossible */
        return FALSE;

    dpp->osdirvendorid = idvendval;        /* found our modem! */
    dpp->osdirproductid = idprodval;

    snprintf(newpathp, sizeof(newpathp), "%s/%s/bNumInterfaces", pathp, filep);
        /* full path to /sys/bus/usb/devices/.../bNumInterfaces   */

    if (!swi_osdirfile(newpathp, filestring, sizeof(filestring)))
        return FALSE;   /* if can't read bNumInterfaces file - seriously wrong */

    dpp->osdirbNumInterfaces = swi_osdiratoi(filestring, 10);   /* Number of USB Interfaces */

    if (dpp->osdirbNumInterfaces  == 0)    /* impossible, should be 1 or greater */
        return FALSE;

    /* we've now found the SWI modem  and can proceed to parse
     * its directory for either one or many directories of
     * the form xxxx:yyyy, where xxxx matches filep and yyyy
     * starts with a number and is of similar form. These
     * subdirectories will contain ep_xx and ttyUSBxx directories.
     */
    dpp->osdirtopdevicep = filep;

    /* scan the sub directory */
    snprintf(newpathp, sizeof(newpathp), "%s/%s", pathp, filep);

    swi_osdirmap(dpp, newpathp, swi_osdirsubscan);

    return TRUE;    /* found the SWI modem, don't look further */
}


/*************
 *
 * Name: swi_endpointupdate
 *
 * Purpose: Update old endpoint if necessary
 *
 * Params:  old - old endpoint
 *          new - new (potentialy) endpoint
 *
 * Return:  TRUE when updated, FALSE otherwise
 *
 * Abort:   None
 *
 * Notes:   None
 *
 **************/
local swi_bool
swi_endpointupdate( struct swi_endpointtty * oldp,
                    const struct swi_endpointtty * newp )
{
    swi_bool equal = swi_endpointcompare( oldp, newp );
    if( !equal )
    {
        if( SWI_USB_INVALID_DEVICE_NUM != newp->osusbttynum )
        {
            syslog( LOG_DEBUG,
                    "ttyUSB%lu device found on USB Interface %lu",
                    newp->osusbttynum,
                    newp->osusbifnum );
        }
        else
        {
           syslog(  LOG_DEBUG,
                    "ttyUSB%lu device on USB Interface %lu not found",
                    oldp->osusbttynum,
                    oldp->osusbifnum );
        }

        oldp->osusbttynum = newp->osusbttynum;
        oldp->osusbifnum  = newp->osusbifnum;

        return TRUE;
    }

    return FALSE;
}

/*************
 *
 * Name:    swi_qcqmiendpointupdate
 *
 * Purpose: Update old endpoint if necessary
 *
 * Params:  old - old endpoint
 *          new - new (potentialy) endpoint
 *
 * Return:  TRUE when updated, FALSE otherwise
 *
 * Abort:   None
 *
 * Notes:   None
 *
 **************/
local swi_bool
swi_qcqmiendpointupdate( struct swi_endpointqcqmi *oldp,
                         const struct swi_endpointqcqmi *newp )
{
    swi_bool equal = swi_qcqmiendpointcompare( oldp, newp );
    if( !equal )
    {
        if( SWI_USB_INVALID_DEVICE_NUM != newp->osusbqcqminum )
        {
           syslog(  LOG_DEBUG,
                    "qcqmi%lu device found on USB Interface %lu",
                    newp->osusbqcqminum,
                    newp->osusbifnum );
        }
        else
        {
           syslog(  LOG_DEBUG,
                    "qcqmi%lu device on USB Interface %lu not found",
                    oldp->osusbqcqminum,
                    oldp->osusbifnum );
        }

        oldp->osusbqcqminum = newp->osusbqcqminum;
        oldp->osusbifnum  = newp->osusbifnum;

        return TRUE;
    }

    return FALSE;
}

local void ossdkusbscan_init(struct swi_osdirparse *dp)
{
    /* variables to clear prior to scanning */
    dp->osdirvendorid    = 0; /* vendor id */
    dp->osdirproductid   = 0; /* product id */
    dp->osdirnumttys     = 0; /* number of tty interfaces */
    dp->osdirnumqcqmis   = 0; /* number of qcqmi interfaces */
    dp->osdirbNumInterfaces = 0; /* number of USB interfaces */

    int i;
    for( i = 0; i < SWI_OS_MAXENDPOINTS; i++ )
    {
        /* invalidate tty to usb endpoint mapping */
        dp->osdirreadendpointtty[i].osusbttynum  = SWI_USB_INVALID_DEVICE_NUM;
        dp->osdirreadendpointtty[i].osusbifnum   = SWI_USB_INVALID_DEVICE_NUM;
        dp->osdirwriteendpointtty[i].osusbttynum = SWI_USB_INVALID_DEVICE_NUM;
        dp->osdirwriteendpointtty[i].osusbifnum  = SWI_USB_INVALID_DEVICE_NUM;

        /* invalidate qcqmi to usb endpoint mapping */
        dp->osdirreadendpointqcqmi[i].osusbqcqminum  = SWI_USB_INVALID_DEVICE_NUM;
        dp->osdirreadendpointqcqmi[i].osusbifnum     = SWI_USB_INVALID_DEVICE_NUM;
        dp->osdirwriteendpointqcqmi[i].osusbqcqminum = SWI_USB_INVALID_DEVICE_NUM;
        dp->osdirwriteendpointqcqmi[i].osusbifnum    = SWI_USB_INVALID_DEVICE_NUM;
    }
}

local swi_bool
ossdkusbscan_device_descriptor( struct swi_osdirparse *dp,
                                struct swi_osusb *up )
{
#ifdef OS_DBG_DEVSCAN
    local swi_uint32 pid = 0;
    if( pid != dp->osdirproductid )
    {
        syslog( LOG_DEBUG, "%s: vid: %lx pid: %lx", __func__,
                dp->osdirvendorid,
                dp->osdirproductid);

        pid = dp->osdirproductid;
    }
#endif
    if( 0 == dp->osdirvendorid || 0 == dp->osdirproductid )
    {
        return FALSE;
    }

    up->osusbvendorid = dp->osdirvendorid;
    up->osusbproductid = dp->osdirproductid;

    return TRUE;
}

local swi_bool
ossdkusbscan_configuration_descriptor( struct swi_osdirparse *dp,
                                       struct swi_osusb *up )
{
#ifdef OS_DBG_DEVSCAN
    local swi_uint32 ifnum = 0;
    if( ifnum != dp->osdirbNumInterfaces )
    {
        syslog( LOG_DEBUG, "%s: %lu interface(s) found", __func__,
                dp->osdirbNumInterfaces );

        ifnum = dp->osdirbNumInterfaces;
    }
#endif
    if( 0 == dp->osdirbNumInterfaces )
    {
        return FALSE;
    }

    up->osusbNumInterfaces = dp->osdirbNumInterfaces;

    return TRUE;
}

local swi_bool
ossdkusbscan_endpoint_qcqmi( struct swi_osdirparse *dp,
                             struct swi_osusb *up )
{
    int i, rdepcnt = 0, wrepcnt = 0;
    for( i = 0; i < SWI_OS_MAXENDPOINTS ; i++ )
    {
#ifdef OS_DBG_DEVSCAN_QCQMI
        syslog( LOG_DEBUG, "%s: qcqmi read ep updated?", __func__);
#endif
        (void)swi_qcqmiendpointupdate( &up->osusbreadendpointqcqmi[i],
                                       &dp->osdirreadendpointqcqmi[i]  );
#ifdef OS_DBG_DEVSCAN_QCQMI
        syslog( LOG_DEBUG, "%s: qcqmi write ep updated?", __func__);
#endif
        (void)swi_qcqmiendpointupdate( &up->osusbwriteendpointqcqmi[i],
                                       &dp->osdirwriteendpointqcqmi[i]  );

        if( SWI_USB_INVALID_DEVICE_NUM != dp->osdirreadendpointqcqmi[i].osusbqcqminum )
        {
            rdepcnt++;
        }

        if( SWI_USB_INVALID_DEVICE_NUM != dp->osdirreadendpointqcqmi[i].osusbqcqminum )
        {
            wrepcnt++;
        }
    }

    if( 1 != wrepcnt || 1 != rdepcnt )
    {
        return FALSE;
    }

    return TRUE;
}

local swi_bool
ossdkusbscan_endpoint_tty( struct swi_osdirparse *dp,
                           struct swi_osusb *up)
{
    int i, rdepcnt =0, wrepcnt = 0;
    for( i = 0; i < SWI_OS_MAXENDPOINTS ; i++ )
    {
#ifdef OS_DBG_DEVSCAN_TTY
        syslog( LOG_DEBUG, "%s: tty read ep updated?", __func__);
#endif
        (void)swi_endpointupdate( &up->osusbreadendpointtty[i],
                                  &dp->osdirreadendpointtty[i]  );
#ifdef OS_DBG_DEVSCAN_TTY
        syslog( LOG_DEBUG, "%s: tty write ep updated?",__func__);
#endif
        (void)swi_endpointupdate( &up->osusbwriteendpointtty[i],
                                  &dp->osdirwriteendpointtty[i]  );

        if( SWI_USB_INVALID_DEVICE_NUM != dp->osdirreadendpointtty[i].osusbttynum )
        {
            rdepcnt++;
        }

        if( SWI_USB_INVALID_DEVICE_NUM != dp->osdirreadendpointtty[i].osusbttynum )
        {
            wrepcnt++;
        }
    }

    int targetepcnt = dp->osdirbNumInterfaces == 1
                      ? 1 /* boot loader => a single tty usb interface */
                      : dp->osdirbNumInterfaces - 1; /* app. mode - one qcqmi interface */

    if( 1 < dp->osdirbNumInterfaces && /* app. mode */
        targetepcnt == wrepcnt &&
        targetepcnt == rdepcnt )
    {
        return TRUE;
    }
    else if( 1 == dp->osdirbNumInterfaces && /* boot mode */
             targetepcnt == wrepcnt &&
             targetepcnt == rdepcnt )
    {
        return TRUE;
    }
#ifdef OS_DBG_DEVSCAN
    else if( 0 != dp->osdirbNumInterfaces ) /* scanning in progress */
    {
        syslog( LOG_DEBUG, "%s: wrepcnt: %d, rdepcnt: %d",__func__, wrepcnt, rdepcnt);
    }
#endif

    return FALSE;
}

local swi_bool ossdk_driver_ready(struct swi_osdirparse *dp)
{
    /* check ttyUSB device driver */
    int epcount = sizeof(dp->osdirreadendpointtty)/sizeof(dp->osdirreadendpointtty[0]);

    struct swi_endpointtty *pt;
    int i;
    for( i = 0 ; i < epcount ; i++ )
    {
        pt = &dp->osdirreadendpointtty[i];

        if( SWI_USB_INVALID_DEVICE_NUM != pt->osusbttynum )
        {
            char devnode[20];
            snprintf(devnode, strlen("/dev/ttyUSBx") + 1,"/dev/ttyUSB%lu", pt->osusbttynum);
            int fd = open(devnode, O_RDONLY, O_NOCTTY | O_NONBLOCK);
            if( -1 == fd )
            {
                return FALSE; /* driver is not ready */
            }
            else
            {
                close(fd); /* driver is ready */
            }
        }
    }

    /* check qcmqmi device driver */
    epcount = sizeof(dp->osdirreadendpointqcqmi)/sizeof(dp->osdirreadendpointtty[0]);

    struct swi_endpointqcqmi *pq;
    for( i = 0 ; i < epcount ; i++ )
    {
        pq = &dp->osdirreadendpointqcqmi[i];

        if( SWI_USB_INVALID_DEVICE_NUM != pq->osusbqcqminum )
        {
            char devnode[20];
            snprintf(devnode, strlen("/dev/qcqmix") + 1,"/dev/qcqmi%lu", pq->osusbqcqminum);
            int fd = open(devnode, O_RDONLY, O_NOCTTY | O_NONBLOCK);
            if( -1 == fd )
            {
                return FALSE; /* driver is not ready */
            }
            else
            {
                close(fd); /* driver is ready */
            }
        }
    }

    return TRUE;
}

/*************
 *
 * Name:    swi_ossdkusbscan
 *
 * Purpose: scan sysfs looking for a supported device, update our USB structure
 *          with the device's ttyUSB and qcqmi device node numbers and the
 *          corresponding USB inteface numbers, and check that the drivers
 *          associated with device nodes are ready.
 *
 * Params:  up  - pointer to usb structure
 *
 * Return:  None
 *
 * Abort:   failure to lock or unlock a mutex
 *
 * Notes:   1. This routine should be called periodically to ensure that the USB
 *             structure is in sync with sysfs
 *          2. If the device state is declared to be OK, the SDK and drivers
 *             must be ready to communicate with the device.
 *          3. swi_ossdkusbinit must been called prior to executing this routine
 *
 **************/
global void swi_ossdkusbscan(struct swi_osusb *up)
{
    /* initialize sysfs device scanning data structure */
    struct swi_osdirparse dp;
    ossdkusbscan_init(&dp);

    /* recursively scan sysfs for supported device, configuration, interface,
     * and endpoint USB descriptors. Unless all tests pass, we cannot claim
     * to have found a supported device.
     */
    swi_osdirmap(&dp, "/sys/bus/usb/devices", swi_osdirdevscan);

    /* get exclusive access to USB control block */
    if (pthread_mutex_lock(&up->osusbmutex) < 0)
    {
        swi_ossdkaborterror("USB semaphore lock failed", errno);
    }

    /* cache previous device state */
    swi_uint32 prevstate = up->osusbstate;

    /* start in an undetermined state */
    swi_uint32 devstate = SWI_OS_USB_STATE_CHECK;

    /* USB Device Descriptor scanned? */
    if( ossdkusbscan_device_descriptor(&dp, up) )
    {
        /* USB Configuration Descriptor scanned? */
        if( ossdkusbscan_configuration_descriptor(&dp, up) )
        {
            if( 0 < dp.osdirbNumInterfaces )
            {
                /* ttyUSB Interface/Endpoint Descriptors scanned? */
                if( ossdkusbscan_endpoint_tty(&dp, up) )
                {
                    swi_bool scanpass = TRUE; /* boot mode - scan passed
                                               * app mode  - continue scanning
                                               */

                    /* check qcqmix in application mode */
                    if( 1 < dp.osdirbNumInterfaces)
                    {
                        if( !ossdkusbscan_endpoint_qcqmi(&dp, up) )
                        {
                            scanpass = FALSE;
                        }
                    }
                    if( scanpass )
                    {
                        /* assume driver check will pass */
                        devstate = SWI_OS_USB_STATE_OK;

                        /* scan passed - proceed to check driver state */
                        if( (SWI_OS_USB_STATE_OK != prevstate) )
                        {
                            if( !ossdk_driver_ready(&dp) )
                            {
                                devstate = SWI_OS_USB_STATE_CHECK; /* driver not ready */
                                syslog( LOG_DEBUG, "%s: drivers initializing...", __func__);
                            }
                            else /* device ready */
                            {
                                /* driver check passed:
                                 * SDK<=>Device communication can now take place
                                 */
                                syslog( LOG_DEBUG, "%s: drivers ready!", __func__);
                            }
                        }
                    }
                }
            } /* scanned interface count > 0 */
            else /* zero interfaces found */
            {
                devstate = SWI_OS_USB_STATE_BAD; /* supported device not found */
#ifdef OS_DBG_DEVSCAN
                if( devstate != prevstate )
                {
                    syslog( LOG_DEBUG,
                            "%s: vendor: %lx device: %lx (0 Interfaces)", __func__,
                            dp.osdirvendorid,
                            dp.osdirproductid );
                }
#endif
            }
        }
    }
    else /* no vid/pid => no device present in sysfs */
    {
        devstate = SWI_OS_USB_STATE_BAD;
    }

#ifdef OS_DBG_DEVSCAN
    /* update the log if the device state changed */
    if( devstate != prevstate )
    {
        syslog( LOG_DEBUG, "%s: device state: %lu", __func__, devstate );
    }
#endif

    if( SWI_OS_USB_STATE_BAD == devstate )
    {
        /* error recovery */
        swi_sdkusbioerrorlocked(up);
    }

    /* update USB structure device state */
    up->osusbstate = devstate;

    /* relinquish exclusive access to USB control block */
    if( pthread_mutex_unlock(&up->osusbmutex) < 0 )
    {
        swi_ossdkaborterror("USB semaphore unlock failed", errno);
    }
}
