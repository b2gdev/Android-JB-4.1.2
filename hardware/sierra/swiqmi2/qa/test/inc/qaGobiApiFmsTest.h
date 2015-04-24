/**************
 *
 *  Filename: qaGobiApiFmsTest.h
 *
 *  Purpose:  Firmware Management Service (FMS) API test driver function prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_FMS_TEST_H__
#define __GOBI_API_FMS_TEST_H__

/* Defines */
#define MAXNUM                                  10
#define nMaxStrLen                              255
#define MAX_GET_IMAGE_STORE_TESTCASE_NUM        1
#define MAX_GET_IMAGE_INFO_TESTCASE_NUM         12
#define MAX_GET_FIRMWARE_INFO_TESTCASE_NUM      1
#define MAX_UPGRADE_FIRMWARE_2K_TESTCASE_NUM    3
#define MAX_SLQS_GET_FW_INFO_TESTCASE_NUM       2
#define MAX_SLQS_GET_IMAGE_INFO_TESTCASE_NUM    5
#define MAX_GET_IMAGES_PREFERENCE_TESTCASE_NUM  1
#define MAX_SET_IMAGES_PREFERENCE_TESTCASE_NUM  1
#define MAX_GET_STORED_IMAGES_TESTCASE_NUM      3
#define MAX_SLQS_UPGRADE_FIRMWARE_TESTCASE_NUM  10
#define MAX_DELETE_STORED_IMAGE_TESTCASE_NUM    3

/* Structur test cases step */
typedef struct _GetImageStoreTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    WORD              imageStorePathSize;
    CHAR              desc[100];
} GetImageStoreTestCase_t;

/* Define the test cases to be executed */
GetImageStoreTestCase_t
      GetImageStoreTestCases[MAX_GET_IMAGE_STORE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 10, "valid Storage Type" }
};

/* Structure for test cases step */
typedef struct _GetImageInfoTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              path[nMaxStrLen];
    struct qmifwinfo_s  *pInfo;
    CHAR              desc[100];
} GetImageInfoTestCase_t;

/* Structure for test cases step */
typedef struct _GetFirmwareInfoTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetFirmwareInfoTestCase_t;

/* Define the test cases to be executed */
const GetFirmwareInfoTestCase_t \
      GetFirmwareInfoTestCases[MAX_GET_FIRMWARE_INFO_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, "valid Storage Type" }
};

typedef struct UpgradeFirmware2kTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              path[nMaxStrLen];
    CHAR              desc[100];
} UpgrdFw2kTC_t;

/* Structure for test cases step */
typedef struct _SLQSGetFWInfoTestCase_t
{
    enum eQCWWANError   expectedReturnValue;
    ULONG               testCaseId;
    struct qmifwinfo_s  *pInfo;
    CHAR                desc[100];
} SLQSGetFWInfoTestCase_t;

/* Structure for test cases step */
typedef struct _SlqsGetImageInfoTestCase_t
{
    enum eQCWWANError   expectedReturnValue;
    ULONG               testCaseId;
    LPCSTR              path;
    struct qmifwinfo_s  *pInfo;
    CHAR                desc[100];
} SlqsGetImageInfoTestCase_t;

typedef struct _GetImagesPrefTestCase_t
{
    enum eQCWWANError    expectedReturnValue;
    ULONG                testCaseId;
    ULONG                *pImageListSize;
    struct PrefImageList *pImageList;
    CHAR                 desc[100];
} GetImagesPrefTestCase_t;

struct PrefImageList ImageList;
ULONG imageListSize = sizeof(struct PrefImageList);

/* Define the test cases to be executed */
const GetImagesPrefTestCase_t
     GetImagesPrefTestCases[MAX_GET_IMAGES_PREFERENCE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, &imageListSize, &ImageList , "All valid" }
};

typedef struct _SetImagesPrefTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             imageListSize;
    ULONG             bForceDownload;
    BYTE              modemIndex;
    ULONG             *pImageTypesSize;
    BYTE              *pImageTypes;
    CHAR              desc[100];
} SetImagesPrefTestCase_t;

typedef struct _GetStoredImagesTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              *pImageList;
    ULONG             imageListsize;
    CHAR              desc[100];
} GetStoredImagesTestCase_t;

ULONG imageTypesSize = MAXNUM;
BYTE  imageTypes[MAXNUM];
/* Define the test cases to be executed */
const SetImagesPrefTestCase_t
     SetImagesPrefTestCases[MAX_SET_IMAGES_PREFERENCE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, 1000, 2, 0x05, &imageTypesSize,
      imageTypes, "All valid" }
};

typedef struct SLQSUpgradeFirmwareTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              path[nMaxStrLen];
    CHAR              desc[100];
} SLQSUpgrdFwTC_t;

typedef struct SLQSUpgradeFirmwareTest_t
{
    ULONG TestCaseIdx;
    SLQSUpgrdFwTC_t TestCase[MAX_SLQS_UPGRADE_FIRMWARE_TESTCASE_NUM];
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    BOOL wait;
}SLQSUpgrdfwT_t;

typedef struct _DeleteStoredImageTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             imageInfoSize;
    BYTE*             pImageInfo;
    CHAR              desc[100];
} DeleteStoredImageTestCase_t;

#endif /* __GOBI_API_FMS_TEST_H__ */
