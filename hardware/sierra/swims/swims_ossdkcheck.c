/**
 *
 * @ingroup swims
 *
 * @file
 * Device Detection/Scanning algorithms for the SWI OS Wrapper layer
 *
 * @author
 * Copyright: © 2010 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"      /* data types typedefs */
#include "swims_ossdk.h"       /* package user include file  */

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

#if ANDROID_NOT_PORTED
#include <ucontext.h>
#endif

#define SWI_USB_MAX_PATH 256
#define SWI_USB_MAX_FILENAME 32

/* Encryption key used for hiding vendor IDs from scrutiny */
#define SWI_VENDORID_ENCRYPT_KEY 0x7679

/* directory parse structure */
struct swi_osdirparse {
    swi_uint32 *osdirvendorlistp; /* pointer to list of vendors */
    swi_uint32 osdirvendorid;     /* vendor id, if found, or zero */
    swi_uint32 osdirproductid;    /* product id */
    const char *osdirtopdevicep;  /* name of top-level device (not EPs) */
    swi_uint32 osnumreadeps;      /* number of read endpoints in subdevice */
    swi_uint32 osdirreadeps[SWI_OS_MAXENDPOINTS]; /* read EPs in subdevice */
    swi_uint32 osnumwriteeps;                /* # of read EPs in subdevice */
    swi_uint32 osdirwriteeps[SWI_OS_MAXENDPOINTS];   /* read EPs in subdev */
    swi_uint32 osdirnumttys;                  /* # of ttyUSBs in subdevice */
    swi_uint32 osdirttys[SWI_OS_MAXENDPOINTS];     /* ttyUSBs in subdevice */
    swi_uint32 osdirifnum;                  /* interface number in subdevice */
    
    struct swi_endpointtty osdirreadendpointtty[SWI_OS_MAXENDPOINTS]; /* ttyUSBn for read EP */
    struct swi_endpointtty osdirwriteendpointtty[SWI_OS_MAXENDPOINTS]; /* ttyUSBn for write EP*/
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

/* BZ 12553 */
swi_uint32 osvendorid[] = 
{
    0x1199,
         0
};

/* Space to store list of vendor IDs specified by a caller
 * on system startup if they decide to. BZ 12553
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
    int i;
    int j;
    swi_uint32 temp;

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
local swi_bool swi_osdirepscan(struct swi_osdirparse *dpp, 
                               const char *pathp, 
                               const char *filep)
{
    char newpathp[SWI_USB_MAX_PATH];
    char filestring[SWI_USB_MAX_FILENAME];
    swi_uint32 devno;
    swi_uint32 ifnum;
    
    if (swi_osdirisprefix("ep_", filep))
    {
        /* get type of endpoint */
        snprintf(newpathp, sizeof(newpathp), "%s/%s/type", pathp, filep);

        if (!swi_osdirfile(newpathp, filestring, sizeof(filestring)))
            return FALSE;    /* if can't read type file */
        
        if (!swi_osdirisprefix("Bulk", filestring))
            return FALSE;    /* if not a Bulk-type endpoint, it's Interrupt */
        
        devno = swi_osdiratoi(&filep[3], 16);    /* endpoint number */
        
        if ((devno & 0x80) != 0)        /* if input device */
        {
            devno &= ~0x80;        /* clear flag bit */
            if (devno >= SWI_OS_MAXENDPOINTS)    /* if endpoint # too big */
                return FALSE;
            
            if (dpp->osnumreadeps >= SWI_OS_MAXENDPOINTS)
                return FALSE;        /* if too many endpoints */
            
            dpp->osdirreadeps[dpp->osnumreadeps] = devno;
            dpp->osnumreadeps += 1;
        }
        else
        {
            if (devno >= SWI_OS_MAXENDPOINTS)    /* if endpoint # too big */
                return FALSE;
            
            if (dpp->osnumwriteeps >= SWI_OS_MAXENDPOINTS)
                return FALSE;        /* if too many endpoints */
            
            dpp->osdirwriteeps[dpp->osnumwriteeps] = devno;
            dpp->osnumwriteeps += 1;
        }
    }
    else if (swi_osdirisprefix("ttyUSB", filep))
    {
        devno = swi_osdiratoi(&filep[6], 10);    /* ttyUSB number */
                /* note that ttyUSB numbers are in decimal, unlike all
                   the other ttys */
        
        if (devno >= SWI_OS_MAXUSBTTYS)    /* if tty number too big */
            return FALSE;
        
        if (dpp->osdirnumttys >= SWI_OS_MAXENDPOINTS)
            return FALSE;        /* if too many endpoints */
            
        dpp->osdirttys[dpp->osdirnumttys] = devno;
        dpp->osdirnumttys += 1;
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
local swi_bool swi_osdirsubscan(struct swi_osdirparse *dpp, 
                                const char *pathp, 
                                const char *filep)
{
    char newpathp[SWI_USB_MAX_PATH];
    swi_uint32 epidx;
    swi_uint32 ttyidx;

    if (!swi_osdirisprefix(dpp->osdirtopdevicep, filep))
        return FALSE;        /* file name not of form xxxx:yyyy */
    
    dpp->osnumreadeps = 0;    /* no read endpoints found for this subdevice */
    dpp->osnumwriteeps = 0;    /* no write endpoints found for this subdevice */
    dpp->osdirnumttys = 0;        /* no ttyUSBs found for this subdevice */
    dpp->osdirifnum = 0xFFFFFFFF;

    /* scan the sub directory */
    snprintf(newpathp, sizeof(newpathp), "%s/%s", pathp, filep);
    
    swi_osdirmap(dpp, newpathp, swi_osdirepscan);
    
    /* sort the endpoints and ttyUSBs in ascending order */
    swi_osdirsortuint(dpp->osnumreadeps, dpp->osdirreadeps);
    swi_osdirsortuint(dpp->osnumwriteeps, dpp->osdirwriteeps);
    swi_osdirsortuint(dpp->osdirnumttys, dpp->osdirttys);

    /* assign ttys to read/write endpoints (only one will be found for */
    /* composite devices but several will be found for non-composite -- */
    /* the only way to match ebs/ttys up is in numerical order) */

    ttyidx = 0;
    for (epidx = 0; epidx < dpp->osnumreadeps; epidx++)
    {
        if (ttyidx < dpp->osdirnumttys)
        {
            struct swi_endpointtty * ep = 
                &dpp->osdirreadendpointtty[dpp->osdirreadeps[epidx]];
            
            ep->osusbttynum = dpp->osdirttys[ttyidx];
            ep->osusbifnum  = dpp->osdirifnum;
            ttyidx += 1;
        }
    }
    
    ttyidx = 0;
    for (epidx = 0; epidx < dpp->osnumwriteeps; epidx++)
    {
        if (ttyidx < dpp->osdirnumttys)
        {
            struct swi_endpointtty * ep = 
                &dpp->osdirwriteendpointtty[dpp->osdirwriteeps[epidx]];
                                                    
            ep->osusbttynum = dpp->osdirttys[ttyidx];
            ep->osusbifnum  = dpp->osdirifnum;
            ttyidx += 1;
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
local swi_bool swi_osdirdevscan(struct swi_osdirparse *dpp, 
                                const char *pathp, 
                                const char *filep)
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
        oldp->osusbttynum = newp->osusbttynum;
        oldp->osusbifnum  = newp->osusbifnum;
        return TRUE;
    }
    return FALSE;
}


/*************
 *
 * Name: swi_ossdkusbscan
 *
 * Purpose: Scan the system, updating the list of endpoints to ttyUSB
 *          mappings in the swi_osusb structure
 *           
 * Params:  up  - pointer to usb structure
 *
 * Return:  Vendor ID - decoy value - BZ12553
 *
 * Abort:   Linux problem - corrupt or no memory, etc.
 *
 * Notes:   Should be called periodically to update structure (from a 
 *          thread)
 *
 *          swi_ossdkusbinit must have been called first
 *
 **************/
global swi_uint32 swi_ossdkusbscan(struct swi_osusb *up)
{
    struct swi_osdirparse dp;
    int i;
    swi_bool updated;
    swi_uint32 *vidp;         /* BZ12553 */
    
    dp.osdirvendorid = 0;     /* vendor id */
    dp.osdirproductid = 0;    /* product id */
    
    for (i = 0; i < SWI_OS_MAXENDPOINTS; i++)
    {
        dp.osdirreadendpointtty[i].osusbttynum = ~0U; 
        dp.osdirreadendpointtty[i].osusbifnum  = ~0U;
        dp.osdirwriteendpointtty[i].osusbttynum = ~0U;
        dp.osdirwriteendpointtty[i].osusbifnum = ~0U;
    }
    
    swi_osdirmap(&dp, "/sys/bus/usb/devices", swi_osdirdevscan);
                                        /* recursively find everything */

    /* Obtain pointer to decoy vendor ID list */
    vidp = osvendorid;

    if (pthread_mutex_lock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore lock failed", errno);

    if (dp.osdirvendorid == 0 || dp.osdirproductid == 0 ||
                up->osusbstate > SWI_OS_USB_STATE_OK)
                                        /* if no modem or weird state */
    {
        swi_sdkusbioerrorlocked(up);    /* error recovery */
    }
    else
    {
        updated = FALSE;
        
        if (up->osusbvendorid != dp.osdirvendorid)
        {
            updated = TRUE;
            up->osusbvendorid = dp.osdirvendorid;
        }
        
        if (up->osusbproductid != dp.osdirproductid)
        {
            updated = TRUE;
            up->osusbproductid = dp.osdirproductid;
        }
        
        for (i = 0; i < SWI_OS_MAXENDPOINTS ; i++)
        {
            updated |= swi_endpointupdate( &up->osusbreadendpointtty[i],   
                                           &dp.osdirreadendpointtty[i]  );
            
            updated |= swi_endpointupdate( &up->osusbwriteendpointtty[i],   
                                           &dp.osdirwriteendpointtty[i]  );
        }
        
        if (up->osusbstate == SWI_OS_USB_STATE_BAD)
            up->osusbstate = SWI_OS_USB_STATE_CHECK;
        else if (up->osusbstate == SWI_OS_USB_STATE_CHECK && !updated)
                up->osusbstate = SWI_OS_USB_STATE_OK;
    }
    
    if (pthread_mutex_unlock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore unlock failed", errno);

    /* Return decoy value */
    return *vidp;
}
