/**
 *
 * @ingroup swiim
 *
 * @file
 * os dependent files for image management 
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "swiim_img.h"
#define  LOG_TAG  "swi_imgmgr"
#include "swiril_log.h"


/*************
 *
 * Name:     swi_ossdkfileopenread
 *
 * Purpose:  Open a disk file for read only of binary bytes
 *
 * Params:   fp      - pointer to file control structure
 *           pathp   - pointer to string containing file path
 *
 * Return:   TRUE if OK, false if file doesn't exist
 *
 * Abort:    none
 *
 * Notes:    none
 *
 **************/
swi_bool swi_ossdkfileopenread(struct swi_osfile *fp, const char *pathp)
{
    int rv;

    rv = open(pathp, O_RDONLY);

    fp->osfiledescrip = rv;

    return rv >= 0;
}

/*************
 *
 * Name: swi_ossdkfilesize
 *
 * Purpose: Returns the total size of the file in bytes
 *
 * Params:  fp    - pointer to file control structure
 *          sizep - memory to return the file size (in bytes)
 *
 * Return:  TRUE if OK, FALSE if file system error
 *
 * Abort:   none
 *
 * Notes:   swi_ossdkfileopenread must have been called first
 *
 **************/
swi_bool swi_ossdkfilesize(struct swi_osfile *fp, swi_size_t *sizep)
{
    struct stat finfo;

    *sizep = 0;

    if (fp->osfiledescrip < 0)
    {
        return FALSE;
    }

    if(fstat(fp->osfiledescrip, &finfo) < 0)
    {
        return FALSE;
    }

    *sizep = (swi_size_t)finfo.st_size;

    return TRUE;
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
void swi_sdkfdclose(int fd)
{
    int rv;

    do {
        rv = close(fd);        /* close file descriptor */
    } while (rv < 0 && errno == EINTR);
}

/*************
 *
 * Name: swi_ossdkfileclose
 *
 * Purpose: Close a disk file opened with swi_ossdkfileopenread or
 *          swi_ossdkfileopenwrite
 *
 * Params:  fp      - pointer to file control structure
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
void swi_ossdkfileclose(struct swi_osfile *fp)
{
    if (fp->osfiledescrip < 0)
        return;

    swi_sdkfdclose(fp->osfiledescrip);

    fp->osfiledescrip = -1;
}

/*************
 *
 * Name:    swi_ossdk_mmapro
 *
 * Purpose: memory map a file in read only mode
 *
 * Params:  (IN)    path        - path at which the file is placed
 *          (IN/OUT)pfileinfo   - swi file attributes structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:   1.  if pfileinfo->filesize is zero or larger than the file size,
 *              the size of the file is mapped. Otherwise, the size provided by
 *              pfileinfo->filesize is mapped.
 *
 *          2.  failure can be due to lack of memory or if the pfileinfo->fsize
 *              + pfileinfo->vmapfileoffset exceed the file's size.
 *
 **************/
swi_bool swi_ossdk_mmapro(
    const swi_char         *path,
    struct swi_osfilestats *pfileinfo )
{
    struct swi_osfile fp;
    swi_size_t fsize;

    /* open the file for reading */
    if( !swi_ossdkfileopenread( &fp, path ) )
    {
        LOGE("swi_ossdk_mmapro: failed to open file");
        return FALSE;
    }

    if(FALSE == swi_ossdkfilesize( &fp, &(fsize) ) )
    {
        LOGE("swi_ossdk_mmapro : failed to get file size");
        return FALSE;
    }

    /* fail if the sum of the requested size to map and the requested file
     * offset exceed the file's actual size.
     */
    if( pfileinfo->filesize + pfileinfo->vmapfileoffset > fsize  )
    {
        return FALSE;
    }

    if( pfileinfo->filesize == 0 || pfileinfo->filesize > fsize  )
    {
        pfileinfo->filesize = fsize;
    }

    /* calculate page aligned offset */
    ssize_t pgaligned_offset, diff;

    diff = pfileinfo->vmapfileoffset & (sysconf(_SC_PAGE_SIZE) - 1);
    pgaligned_offset = pfileinfo->vmapfileoffset & ~(sysconf(_SC_PAGE_SIZE) - 1);

    /* map file to virtual address space of calling process
     * - let kernel choose address at which to create the mapping
     * - offset based on image type and must be a multiple of sysconf(_SC_PAGE_SIZE)
     * - Protection: PROT_READ, Flags: MAP_PRIVATE | MAP_POPULATE
     */
    pfileinfo->pvunmap = mmap(  NULL,
                                pfileinfo->filesize + diff,
                                PROT_READ,
                                MAP_PRIVATE | MAP_POPULATE,
                                fp.osfiledescrip,
                                pgaligned_offset );

    if ( MAP_FAILED == pfileinfo->pvunmap )
    {
        LOGE("swi_ossdk_mmapro: File mapping failed");
        return FALSE;
    }

    /* Adjust client's pointer to the mapped file as per the requested offset */
    pfileinfo->pvmap = pfileinfo->pvunmap + diff;

    /* close the file */
    swi_ossdkfileclose( &fp);

    return TRUE;
}

/*************
 *
 * Name:    swi_ossdk_umapfile
 *
 * Purpose: wraper for munmap
 *
 * Params:  (IN)   pfileinfo - swi file attributes structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
swi_bool swi_ossdk_umapfile(
    struct swi_osfilestats *pfileinfo )
{
    if( 0 != munmap( pfileinfo->pvunmap, pfileinfo->filesize ) )
    {
        LOGE(__func__);
        return FALSE;
    }

    return TRUE;
}



