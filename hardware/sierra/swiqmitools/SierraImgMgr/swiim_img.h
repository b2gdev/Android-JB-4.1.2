/**
 *
 * @ingroup swiim
 *
 * @file
 * SWI Image manager data and function types
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */


#ifndef __SWI_IMG_MANAGER_H
#define __SWI_IMG_MANAGER_H

#include "SWIWWANCMAPI.h"
#include "im/imudefs.h"
#include <stdbool.h>

#define local                   static /* function/variable accessed within local file only */
#define YES                     1
#define NO                      0
#define FALSE                   0

#define MAXDATASIZE             512    /* max number of bytes we can get at once */
#define MAXPATHSIZE             512    /* max number of bytes for image path */
#define NUM_MAX_STR_LEN         0xff

/* Need to allocate enough memory for 6 AMSS and 50 PRI */
#define MAX_IMAGE_LIST_SIZE     2560
#define LEN_IMAGE_ID            16
#define LEN_BUILD_ID            32

/* Technology Constants */
#define TECHNOLOGY_CDMA         0x00
#define TECHNOLOGY_UMTS         0x01
#define TECHNOLOGY_LTE          0x02

/* Carrier Constants */
#define CARRIER_GENERIC         1
#define CARRIER_FACTORY         2
#define CARRIER_NORF            3
#define CARRIER_VERIZON         101
#define CARRIER_SPRINT          102
#define CARRIER_ALLTEL          103
#define CARRIER_BELL            104
#define CARRIER_TELUS           105
#define CARRIER_USCELLUAR       106
#define CARRIER_TELSTRA         107
#define CARRIER_CHINA_UNICOM    108
#define CARRIER_TELECOM_NZ      109
#define CARRIER_SK_TELCOM       110
#define CARRIER_RELIANCE        111
#define CARRIER_TATA            112
#define CARRIER_METRO_PCS       113
#define CARRIER_LEAP            114
#define CARRIER_KDDI            115
#define CARRIER_GRUPO_IUSACALL  116
#define CARRIER_CHINA_MOBILE    117
#define CARRIER_OMH             118
#define CARRIER_ATT             201
#define CARRIER_VODAFONE        202
#define CARRIER_TMOBILE         203
#define CARRIER_ORANGE          204
#define CARRIER_TELEFONICA      205
#define CARRIER_TELECOM_ITALIA  206
#define CARRIER_3               207
#define CARRIER_O2              208
#define CARRIER_SFR             209
#define CARRIER_SWISSCOM        210
#define CARRIER_CHINA_MOBILE2   211
#define CARRIER_TELSTRA2        212
#define CARRIER_SIGNAPORE       213
#define CARRIER_RELIANCE2       214
#define CARRIER_BHARTI_AIRTEL   215
#define CARRIER_NTT_DOCOMO      216
#define CARRIER_EMOBILE         217
#define CARRIER_SOFTBANK        218
#define CARRIER_KOREA           219
#define CARRIER_SK_TELECOM2     220
#define CARRIER_TELNOR          221
#define CARRIER_NETCOME_NORWAY  222
#define CARRIER_TELIA_SONERA    223
#define CARRIER_AMX_TELCEL      224
#define CARRIER_BRASIL_VIVO     225

/* Region Constants */
#define REGION_NORTH_AMERICA    0x00
#define REGION_LATIN_AMERICA    0x01
#define REGION_EUROPE           0x02
#define REGION_ASIA             0x03
#define REGION_AUSTRILIA        0x04
#define REGION_GLOBAL           0x05

#define MBN_BUILD_ID_LEN        32
/* Maximum length for  an AMCC/UQCN Image fileunique image ID */
#define MBN_UNIQUE_ID_LEN       16


typedef struct device_info_param{
  CHAR deviceNode[256];
  CHAR deviceKey[16];
} device_info_t;

enum eGobiMBNType
{
   eGOBI_MBN_TYPE_ENUM_BEGIN = -1, 

   eGOBI_MBN_TYPE_MODEM,         /* 0 - Modem/AMSS*/
   eGOBI_MBN_TYPE_PRI,           /* 1 - PRI/UQCN */

   eGOBI_MBN_TYPE_ENUM_END,
};

/*=========================================================================
 * Struct sImageList
 *    Struct to represent the image information for both modem and Pri
=========================================================================*/
struct sImageList
{
    BYTE ImageType;
    BYTE StorageIndex;
    BYTE FailureCount;
    BYTE ImageId[LEN_IMAGE_ID];
    BYTE BuildIdLen;
    CHAR BuildId[LEN_BUILD_ID];
    struct sImageList *mpNext;
};

/*=========================================================================
 * struct sUQCNVersionID
 *   Struct to represent the UQCN version ID
=========================================================================*/
struct sUQCNVersionID
{
    ULONG mMinorID       : 7;
    ULONG mXTRADisabled  : 1;
    ULONG mGPSDisabled   : 1;
    ULONG mReserved      : 7;
    ULONG mMajorID       : 8;
    ULONG mSystem        : 2;
    ULONG mCompatibility : 6;
};

/*=========================================================================
 * Struct sUQCNInfoRecord
 *    Struct to represent the UQCN information record
=========================================================================*/
struct sUQCNInfoRecord
{
    ULONG mMagic;                 // UQCN_INFO_MAGIC
    struct sUQCNVersionID mVersionID;    // Version ID
    CHAR mInfo[MBN_BUILD_ID_LEN]; // Build info string
};

/*=========================================================================
 * Struct sMBNBootRecord
 *    Struct to represent the MBN boot flash record
=========================================================================*/
struct sMBNBootRecord
{
    ULONG mMagic;  /* MBN_BOOT_MAGIC */
    WORD mMajorID; /* Boot flash major version */
    WORD mMinorID; /* Boot flash minor version */
};

/*=========================================================================
 * Struct sMBNBuildRecord
 *    Struct to represent the build ID record
=========================================================================*/
struct sMBNBuildIDRecord
{
    ULONG mMagic;                    /* MBN_BUILD_MAGIC */
    CHAR mBuildID[MBN_BUILD_ID_LEN]; /* Build ID string */
};

/* Gobi MBN image defines */
#define GOBI_MBN_IMG_INFO_BUF_SIZE  256

/* CWE Sierra Package String parameter defines */
#define CWE_SKU_SZ           10
#define CWE_PARTNO_SZ        10
#define CWE_MODELID_SZ       10
#define CWE_FWVERSION_SZ     15
#define CWE_BOOTBLK_SZ       2
#define CWE_CARRIER_SZ       15
#define CWE_PRIVERSION_SZ    7
#define CWE_PACKAGEID_SZ     3

/* Defines for device image management */
#define IMG_BUF_SIZE               24
#define MAX_FIELD_SIZE             30
#define IMG_TYPE_MODEM             0
#define IMG_TYPE_PRI               1
#define UNKNOWN_EXECUTING_IMG_IDX  0xFF
#define FAIL_SAFE_IMG_IDX          0xFF
#define MAX_IMAGES                 10
#define RESET_MODEM                5
#define GOBI_BUILD_ID_CARRIER_INITIALS_OFFSET 10
#define GOBI33_BUILD_ID_VER_INITIALS_AMSS     14
#define GOBI33_BUILD_ID_VER_INITIALS_UQCN     13
#define GOBI33_CARRIER_INIT_OFFSET_FROM_END    4
#define MAX_VER_STR_LEN                       15
#define VERSION_ID_OFFSET                     8
#define SL9090_BUILD_ID_CARRIER_INITIALS_OFFSET 21

/* disk file I/O */
struct swi_osfile {
    int osfiledescrip;
};

/* file structure */
struct swi_osfilestats{
    swi_size_t  filesize;
    void        *pvmap;
    void        *pvunmap;
    swi_uint32  vmapfileoffset;
};

/* Image information in verbose mode - structure */
struct imgInfoVb{
    CHAR carrierName[MAX_FIELD_SIZE];
    CHAR technologyName[MAX_FIELD_SIZE];
    CHAR regionName[MAX_FIELD_SIZE];
    CHAR activeStatus[MAX_FIELD_SIZE];
};

/* Image information table */
struct imgInfoTable{
    CHAR  *pTechnology;
    CHAR  carrierIntials;
    ULONG carrier;
    ULONG region;
    ULONG technology;
};

/* Display Active Image field enumeration */
enum eActiveImageField{
    eIMG_INACTIVE,
    eIMG_ACTIVE,
};

/* Final PRI Image list which has to be displayed to the user */
struct ImageFinalList
{
    struct qmifwinfo_s imgInfo;
    bool   IsLatest;
};

/* Enumeration to hold carrier Image Status */
enum carrierImgStatus
{
    FW_SAME_CARRIER_NEW,
    FW_SAME_CARRIER_OLD,
    FW_DIFFERENT_CARRIER,
};

/* functions */
swi_bool LoadDeviceImages(
    struct sImageList *lstAppImg,
    struct sImageList *lstPriImg );
void ReleaseDeviceImages( 
    struct sImageList *lstAppImg,
    struct sImageList *lstPriImg );

bool ListImage(char *chListPath);
bool SelectImage(int iSelectID, char *chSelectPath);
bool UpdateAllImages(char *chUpdatePath);
bool GetImageActive(swi_uint32 PathImagId, bool *fActive);
bool ListDeviceImages( bool displayStatus );
bool GetPRIImageIdx( BYTE *pImgListIdx);
bool SelectDeviceImage( BYTE imageIndex );
bool CheckModemImage(
BYTE imageIndex, BYTE modemImdIdx, CHAR *pModemImgBuildID, ULONG BuildIDlen );
enum carrierImgStatus IsFwNew( CHAR *pPRIImgBuildID,
                               CHAR *pOldPRIImgBuildID,
                               ULONG PRIBuildIdLen,
                               ULONG OldPRIBuildLen );

/* OS functions*/
swi_bool swi_ossdkfileopenread(struct swi_osfile *fp, const char *pathp);
swi_bool swi_ossdkfilesize(struct swi_osfile *fp, swi_size_t *sizep);
void swi_sdkfdclose(int fd);
void swi_ossdkfileclose(struct swi_osfile *fp);
swi_bool swi_ossdk_mmapro(
    const swi_char         *path,
    struct swi_osfilestats *pfileinfo );
swi_bool swi_ossdk_umapfile(
    struct swi_osfilestats *pfileinfo );


#endif /* __SWI_IMG_MANAGER_H */

