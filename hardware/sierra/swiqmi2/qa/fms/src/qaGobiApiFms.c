/**
 * \ingroup fms
 *
 * \file    qaGobiApiFms.c
 *
 * \brief   Entry points for Gobi APIs for the Firmware Management Service (FMS)
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */
#include "sludefs.h"
#include "SwiDataTypes.h"
#include "qaQmiBasic.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "qaGobiApiFms.h"
#include "qaFmsSetImagePath.h"
#include "qaFmsSetFirmwarePreference.h"
#include "qaFmsGetImagesPreference.h"
#include "qaFmsSetImagesPreference.h"
#include "qaFmsSlqsGetFirmwareInfo.h"
#include "qaFmsSlqsGetImageInfo.h"
#include "qaFmsGetImageInfo.h"
#include "qaGobiApiDms.h"
#include "qaGobiApiDcs.h"
#include "swi_osapi.h"
#include "qaDmsGetDeviceRevisionID.h"
#include "qaFmsGetFirmwareInfo.h"
#include "qaFmsGetStoredImages.h"
#include "qaFmsDeleteStoredImage.h"

/* Local structures */
local ULONG GobiImageInfoTable[][3] =
{
    {0,     eGOBI_IMG_CAR_FACTORY,          eGOBI_IMG_REG_NA    },
    {1,     eGOBI_IMG_CAR_VERIZON,          eGOBI_IMG_REG_NA    },
    {2,     eGOBI_IMG_CAR_SPRINT,           eGOBI_IMG_REG_NA    },
    {3,     eGOBI_IMG_CAR_ATT,              eGOBI_IMG_REG_NA    },
    {4,     eGOBI_IMG_CAR_VODAFONE,         eGOBI_IMG_REG_EU    },
    {5,     eGOBI_IMG_CAR_TMOBILE,          eGOBI_IMG_REG_EU    },
    {9,     eGOBI_IMG_CAR_GENERIC,          eGOBI_IMG_REG_GLOBAL},
    {11,    eGOBI_IMG_CAR_ORANGE,           eGOBI_IMG_REG_EU    },
    {12,    eGOBI_IMG_CAR_TELEFONICA,       eGOBI_IMG_REG_EU    },
    {13,    eGOBI_IMG_CAR_NTT_DOCOMO,       eGOBI_IMG_REG_ASIA  },
    {14,    eGOBI_IMG_CAR_TELCOM_ITALIA,    eGOBI_IMG_REG_EU    },
    {18,    eGOBI_IMG_CAR_TELCOM_NZ,        eGOBI_IMG_REG_AUS   },
    {19,    eGOBI_IMG_CAR_CHINA_TELECOM,    eGOBI_IMG_REG_ASIA  },
    {20,    eGOBI_IMG_CAR_OMH,              eGOBI_IMG_REG_GLOBAL},
    {22,    eGOBI_IMG_CAR_AMX_TELCEL,       eGOBI_IMG_REG_LA    },
    {23,    eGOBI_IMG_CAR_NORF,             eGOBI_IMG_REG_GLOBAL},
    {24,    eGOBI_IMG_CAR_FACTORY,          eGOBI_IMG_REG_GLOBAL},
    {25,    eGOBI_IMG_CAR_BRASIL_VIVO,      eGOBI_IMG_REG_LA}
};

BOOL IsGobiDevice()
{
    BYTE stringsize = 128;
    CHAR ModelId[stringsize];
    CHAR *pstr = NULL;

    GetModelID( stringsize,
                ModelId );

    pstr = strstr (ModelId, "MC77");
    if( NULL == pstr )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

ULONG GetImageStore(
    WORD  imageStorePathSize,
    CHAR  *pImageStorePath )
{
    UNUSEDPARAM( imageStorePathSize );
    UNUSEDPARAM( pImageStorePath );

    return eQCWWAN_ERR_SWICM_NOT_IMPLEMENTED;
}

local ULONG GetImageInfoInternal(
    LPCSTR pPath,
    ULONG  mbntype,
    struct qm_qmi_response_tlvs_values *presp )
{
    BYTE    *pInParam;    /* ptr to param field rx'd from modem */
    BYTE    *pOutParam;   /* ptr to outbound param field */
    BYTE    *pReqBuf;     /* Pointer to outgoing request buffer */
    WORD    ParamLength;  /* Ret'd length of the QMI Param field */

    ULONG eRCode;

    /* initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack the QMI request */
    eRCode = PkQmiGetMbnInfo( &ParamLength,
                              pOutParam,
                              pPath,
                              mbntype );

    if( eRCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* send the QMI request */
    eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_FMS,
                              ParamLength,
                              0, /* no timeout */
                              &pInParam,
                              &ParamLength );

    if( eRCode == eQCWWAN_ERR_NONE )
    {
        /* unpack the QMI response */
        eRCode = UpkQmiGetMbnInfo( pInParam, presp);

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !presp->qmiresult.validresponse )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( presp->qmiresult.result != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return presp->qmiresult.error;
        }
    }

    qmrelreqbkp();
    return eRCode;
}

ULONG GetImageInfoInt(
    CHAR  *pPath,
    ULONG *pFirmwareID,
    ULONG *pTechnology,
    ULONG *pCarrier,
    ULONG *pRegion,
    ULONG *pGPSCapability )
{
    enum eQCWWANError eRCode;       /* Result of SwiQmiSendnWait() */

    struct qm_qmi_response_tlvs_values response;

    /* input parameter validation */
    if ( !pPath       ||
         !pFirmwareID ||
         !pTechnology ||
         !pCarrier    ||
         !pRegion     ||
         !pGPSCapability )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* output parameter initialization - set values to unknown */
    *pTechnology    = 0xFFFFFFFF;
    *pCarrier       = 0xFFFFFFFF;
    *pRegion        = 0xFFFFFFFF;
    *pGPSCapability = 0xFFFFFFFF;

    GetImageInfoInternal( pPath, MBN_IMG_TYPE_UQCN, &response);

    /* pointer to mbn image response data */
    struct qm_fms_mbn_response_tlv_values *pdat =
        &response.tlvvalues.qmfmstlvs.mbn_rsp;

    /* copy firmware ID */
     *pFirmwareID = pdat->firmwareid;

    /* parse the UQCN version ID to extract technology and gps capability */
    eRCode = qaParseFirmwareID( *pFirmwareID,
                                *pCarrier,
                                pTechnology,
                                pGPSCapability );

    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* determine firmware major version id */
    ULONG majorid =
    (*pFirmwareID & GOBI_FW_MAJOR_ID_MASK) >> GOBI_FW_MAJOR_ID_OFFSET;

    /* determine carrier and region */
    BYTE n = sizeof(GobiImageInfoTable)/sizeof(GobiImageInfoTable[0]);
    BYTE i;
    for( i = 0 ; i < n ; i++ )
    {
        if( majorid == GobiImageInfoTable[i][0] )
        {
            *pCarrier = GobiImageInfoTable[i][1];
            *pRegion = GobiImageInfoTable[i][2];
            break;
        }
    }

    return eRCode;
}

local ULONG GetFirmwareInfoInternal(
    ULONG *pFirmwareID,
    ULONG *pTechnology,
    ULONG *pCarrier,
    ULONG *pRegion,
    ULONG *pGPSCapability )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the  QMI Param field */
    CHAR   AMSSString[QMI_MAX_REV_ID_LENGTH];
    CHAR   BootString[QMI_MAX_REV_ID_LENGTH];
    CHAR   PRIString[QMI_MAX_REV_ID_LENGTH];

    /* Storage for results and response variable */
    struct QmiDmsGetDeviceRevIDResp response;

    /* Input parameter validation  */
    if( !pFirmwareID ||
        !pTechnology ||
        !pCarrier    ||
        !pRegion     ||
        !pGPSCapability )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* output parameter initialization - set values to unknown */
    *pTechnology    = 0xFFFFFFFF;
    *pCarrier       = 0xFFFFFFFF;
    *pRegion        = 0xFFFFFFFF;
    *pGPSCapability = 0xFFFFFFFF;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiDmsGetDeviceRevID( &ParamLength,
                                         pOutParam  );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    if (resultcode == eQCWWAN_ERR_NONE)
    {
        slmemset(AMSSString, 0, sizeof(AMSSString) );
        slmemset(BootString, 0, sizeof(BootString) );
        slmemset(PRIString,  0, sizeof(PRIString) );

        /* initialize response structure */
        response.pAMSSString = AMSSString;
        response.pBootString = BootString;
        response.pPRIString  = PRIString;
        response.amssSize    = sizeof(AMSSString);
        response.bootSize    = sizeof(BootString);
        response.priSize     = sizeof(PRIString);

        /* Copy to the caller's buffer */
        resultcode = UpkQmiDmsGetDeviceRevID(pInParam, &response);
    }

    if( resultcode == eQCWWAN_ERR_NONE )
    {
        if( slstrlen(PRIString) != QMI_PRI_STRING_LENGTH )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }
        else
        {
            /* convert PRI string to firmware ID */
            *pFirmwareID = slahtol(PRIString);

            /* parse the firmware ID to extract technology and gps capability */
            resultcode = qaParseFirmwareID( *pFirmwareID,
                                            *pCarrier,
                                            pTechnology,
                                            pGPSCapability );

            if( eQCWWAN_ERR_NONE != resultcode )
            {
                qmrelreqbkp();
                return resultcode;
            }

            /* determine firmware major version id */
            ULONG majorid =
            (*pFirmwareID & GOBI_FW_MAJOR_ID_MASK) >> GOBI_FW_MAJOR_ID_OFFSET;

            /* determine carrier and region */
            BYTE n = sizeof(GobiImageInfoTable)/sizeof(GobiImageInfoTable[0]);
            BYTE i;
            for( i = 0 ; i < n ; i++ )
            {
                if( majorid == GobiImageInfoTable[i][0] )
                {
                    *pCarrier = GobiImageInfoTable[i][1];
                    *pRegion = GobiImageInfoTable[i][2];
                    break;
                }
            }
        }
    }

    qmrelreqbkp();
    return resultcode;
}

local ULONG SLQSGetFirmwareInfoInternal(
    struct slqsfwinfo_s *pinfo )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiFmsSlqsGetFirmwareInfoResp response;

    /* pinfo is an OUT parameter and hence should not be NULL */
    if ( !pinfo )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiFmsSlqsGetFirmwareInfo( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy the obtained values to the function OUT parameters */
        slmemset((char *)pinfo, 0, sizeof(struct slqsfwinfo_s));
        response.pinfo = pinfo;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiFmsSlqsGetFirmwareInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetFirmwareInfo(
    struct qmifwinfo_s *pinfo )
{
    ULONG  resultCode;

    if (IsGobiDevice() == FALSE)
    {
        /* Found a non-Gobi device */
        resultCode =  SLQSGetFirmwareInfoInternal( &pinfo->dev.s );

    }
    else
    {
        resultCode =   GetFirmwareInfoInternal( &pinfo->dev.g.FirmwareID,
                                        &pinfo->dev.g.Technology,
                                        &pinfo->dev.g.Carrier,
                                        &pinfo->dev.g.Region,
                                        &pinfo->dev.g.GPSCapability );
    }
    return resultCode;
}

ULONG SLQSGetImageInfoInternal(
    LPCSTR              path,
    struct slqsfwinfo_s *pinfo )
{
    BYTE              *pInParam;    /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;   /* ptr to outbound param field */
    BYTE              *pReqBuf;     /* Pointer to outgoing request buffer */
    WORD              ParamLength;  /* Ret'd length of the QMI Param field */
    enum eQCWWANError eRCode;       /* Result of SwiQmiSendnWait() */

    struct qm_qmi_response_tlvs_values response;

    /* input argument validation */
    if ( !pinfo || !path  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack the QMI request */
    eRCode = PkQmiGetCweSpkgsInfo(  &ParamLength,
                                    pOutParam,
                                    path  );

    if( eRCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* send the QMI request */
    eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_FMS,
                              ParamLength,
                              0, /* no timeout  */
                              &pInParam,
                              &ParamLength );

    if( eRCode == eQCWWAN_ERR_NONE )
    {
        /* unpack the QMI response */
        eRCode = UpkQmiGetCweSpkgsInfo( pInParam, &response );

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !response.qmiresult.validresponse )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( response.qmiresult.result != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return response.qmiresult.error;
        }
    }

    /* clear the user memory before copying any data */
    slmemset( (void *)pinfo,
              0 ,
              sizeof(struct slqsfwinfo_s) );

    /* intialize the buffer provided by the user */
    struct qm_fms_cwe_spkgs_response_tlv_values *pdat =
        &response.tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    /* copy application version */
    slstrncpy( pinfo->appversion_str,
               pdat->fwversion_str,
               slstrlen(pdat->fwversion_str) );

    /* copy boot version */
    slstrncpy( pinfo->bootversion_str,
               pdat->fwversion_str,
               slstrlen(pdat->fwversion_str) );

    /* copy carrier string */
    slstrncpy( pinfo->carrier_str,
               pdat->carrier_str,
               slstrlen(pdat->carrier_str) );

    /* copy model id */
    slstrncpy( pinfo->modelid_str,
               pdat->modelid_str,
               slstrlen(pdat->modelid_str) );

    /* copy package id */
    slstrncpy( pinfo->packageid_str,
               pdat->packageid_str,
               slstrlen(pdat->packageid_str) );

    /* copy pri version */
    slstrncpy( pinfo->priversion_str,
               pdat->priversion_str,
               slstrlen(pdat->priversion_str) );

    /* copy sku string */
    slstrncpy( pinfo->sku_str,
               pdat->sku_str,
               slstrlen(pdat->sku_str) );

    qmrelreqbkp();
    return eRCode;
}

ULONG SLQSGetImageInfoMC83xx(
    LPCSTR              path,
    struct qmifwinfo_s *pinfo )
{
    return  GetImageInfoInt( (CHAR *)path,
                             &pinfo->dev.g.FirmwareID,
                             &pinfo->dev.g.Technology,
                             &pinfo->dev.g.Carrier,
                             &pinfo->dev.g.Region,
                             &pinfo->dev.g.GPSCapability );
}

ULONG SLQSGetImageInfoMC77xx(
    LPCSTR              path,
    struct qmifwinfo_s *pinfo )
{
    return SLQSGetImageInfoInternal( path, &pinfo->dev.s );
}

ULONG SLQSGetImageInfo(
    LPCSTR              path,
    struct qmifwinfo_s *pinfo )
{
    if( IsGobiDevice() )
    {
        /* Found a MC83xx device */
        return SLQSGetImageInfoMC83xx( (CHAR *)path,
                                       pinfo);
    }

    return SLQSGetImageInfoMC77xx( path, pinfo );
}

/*
 *  NOTE: This API is not to be exposed to a client application!
 *
 *  Used by UpgradeFirmware2k to set the fully qualified path of directory
 *  containing candidate firmware to be downloaded to the device.
 *
 *  \param  path[IN]
 *          - fully qualified path to firmware image to download
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Support: UMTS/CDMA\n
 *          Timeout: N/A
 *
 */
ULONG SetFirmwarePreference()
{
    BYTE                *pInParam;    /* ptr to param field rx'd from modem */
    BYTE                *pOutParam;   /* ptr to outbound param field */
    BYTE                *pReqBuf;     /* Pointer to outgoing request buffer */
    WORD                ParamLength;  /* Ret'd length of the QMI Param field */
    enum eQCWWANError   eRCode;       /* Result of SwiQmiSendnWait() */

    struct qm_qmi_response_tlvs_values response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    eRCode = PkQmiFmsSetFirmwarePreference( &ParamLength,
                                            pOutParam  );

    if (eRCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* send the request to the SDK process */
    eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_DMS,
                              ParamLength,
                              0, /* no timeout  */
                              &pInParam,
                              &ParamLength );

    if (eRCode == eQCWWAN_ERR_NONE)
    {
        /* unpack QMI response */
        eRCode = UpkQmiFmsSetFirmwarePreference( pInParam, &response );

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !response.qmiresult.validresponse )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( response.qmiresult.result != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return response.qmiresult.error;
        }
    }

    qmrelreqbkp();
    return eRCode;
}

/*
 *  NOTE: This API is not to be exposed to a client application!
 *
 *  Used by UpgradeFirmware2k to set the fully qualified path of directory
 *  containing candidate firmware to be downloaded to the device.
 *
 *  \param  path[IN]
 *          - fully qualified path to firmware image to download
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Support: UMTS/CDMA\n
 *          Timeout: N/A
 *
 */
ULONG SetImagePath(
    CHAR *path )
{
    BYTE                *pInParam;    /* ptr to param field rx'd from modem */
    BYTE                *pOutParam;   /* ptr to outbound param field */
    BYTE                *pReqBuf;     /* Pointer to outgoing request buffer */
    WORD                ParamLength;  /* Ret'd length of the QMI Param field */
    enum eQCWWANError   eRCode;       /* Result of SwiQmiSendnWait() */

    struct qm_qmi_response_tlvs_values response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    eRCode = PkQmiFmsSetImagePath(  &ParamLength,
                                    pOutParam,
                                    path );

    if (eRCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* send the request to the SDK process */
    eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_FMS,
                              ParamLength,
                              0, /* no timeout  */
                              &pInParam,
                              &ParamLength );

    if (eRCode == eQCWWAN_ERR_NONE)
    {
        eRCode = UpkQmiFmsSetImagePath( pInParam, &response );

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !response.qmiresult.validresponse )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( response.qmiresult.result != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return response.qmiresult.error;
        }
    }

    qmrelreqbkp();
    return eRCode;
}

local ULONG upgrade_gobi3k_fw(
    LPCSTR path )
{
    ULONG eRCode;

    /* get the UQCN image info */
    struct qm_qmi_response_tlvs_values response;
    eRCode = GetImageInfoInternal(path, MBN_IMG_TYPE_UQCN, &response);
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* pointer to mbn info response data */
    struct qm_fms_mbn_response_tlv_values *pdat =
        &response.tlvvalues.qmfmstlvs.mbn_rsp;

    /* image preference list construction object */
    struct PrefImageList prefimglist;
    prefimglist.listSize = 2;

    /* construct first entry of the image preference list */
    struct ImageElement *p = &prefimglist.listEntries[0];
    p->imageType = 1; /* 1 => UQCN */
    slmemcpy( p->imageId, pdat->fwidstr, sizeof(pdat->fwidstr) );
    slmemcpy( p->buildId, pdat->fwversionstr, sizeof(pdat->fwversionstr) );
    p->buildIdLength = slstrlen(p->buildId);

    /* Get the AMSS image info. If there's an error, the AMSS image may not
     * exist in the current directory - try the directory for the generic UMTS
     * image.
     */
    CHAR generic[IM_IMAGE_PATH_MAX_SZ];
    slmemcpy( generic, (CHAR *)path, slstrlen(path) + 1);
    ULONG offset = slstrlen(generic) - 2;
    while( generic[offset] != '/' )
    {
        offset--;
    }
    generic[offset + 1] = IM_AMSS_GENERIC_UMTS_MBN_DIR;
    generic[offset + 2] = 0; /* null termination */

    eRCode = GetImageInfoInternal(path, MBN_IMG_TYPE_AMSS, &response);
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        eRCode = GetImageInfoInternal(generic, MBN_IMG_TYPE_AMSS, &response);
        if( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    /* construct second entry of the image preference list */
    p = &prefimglist.listEntries[1];
    p->imageType = 0; /* 0 => AMSS */
    slmemcpy( p->imageId, pdat->fwidstr, sizeof(pdat->fwidstr) );
    slmemcpy( p->buildId, pdat->fwversionstr, sizeof(pdat->fwversionstr) );
    p->buildIdLength = slstrlen(p->buildId);

    /* Set the image preferences on the device */
    BYTE imageTypes[8];
    ULONG imageTypesSize = sizeof(imageTypes);
    ULONG bForceDownload = 0x00;
    if ( TRUE == IsSierraGobiDevice() )
    {
        bForceDownload = 0x01;
    }
    else
    {
        bForceDownload = 0x00;
    }
    if( eQCWWAN_ERR_NONE !=
            ( eRCode = SetImagesPreference( prefimglist.listSize,
                                            (BYTE *)&prefimglist,
                                            bForceDownload, /* force download */
                                            0xFF, /* device manages modem index */
                                            &imageTypesSize,
                                            imageTypes) ) )
    {
        return eRCode;
    }

    CHAR *pr = strrchr( generic, '/');
    *(pr + 1) = 0;

    /* set the firmware download image path */
    eRCode = SetImagePath(generic);
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* reset the device to initiate the download */
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        eRCode = SetPower( DEVICE_RESET );
    }

    return eRCode;
}

local ULONG upgrade_mc77xx_fw(
    LPCSTR path )
{
    ULONG eRCode;
    BYTE devicemode;

    /* set the firmware download image path */
    if( eQCWWAN_ERR_NONE != (eRCode = SetImagePath((CHAR *)path)) )
    {
        return eRCode;
    }

    eRCode = SLQSGetDeviceMode ((BYTE *)&devicemode);

    if ( devicemode != DCS_DEVICE_MODE_BOOT_READY )
    {
        /* set the firmware preferences on the device */
        if( eQCWWAN_ERR_NONE != (eRCode = SetFirmwarePreference()) )
        {
            return eRCode;
        }
    }

    return eRCode;
}

ULONG UpgradeFirmware2k(
    CHAR *pDestinationPath )
{
    enum eQCWWANError eRCode;
    BYTE devicemode;

    /* validate input args */
    if( (pDestinationPath == NULL) || (swi_osapi_stat(pDestinationPath) != 0) )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    eRCode = SLQSGetDeviceMode ((BYTE *)&devicemode);

    CHAR modelID[128];
    if ( devicemode != DCS_DEVICE_MODE_BOOT_READY )
    {
        if( eQCWWAN_ERR_NONE != (eRCode = GetModelID(sizeof(modelID), modelID)) )
        {
            return eRCode;
        }
    }

    if( (0 == slstrncmp("MC77", modelID, slstrlen("MC77"))) ||
        (devicemode == DCS_DEVICE_MODE_BOOT_READY ) )
    {
        /* MC77xx fw upgrade */
        eRCode = upgrade_mc77xx_fw(pDestinationPath);
    }
    else
    {
        /* Gobi 3000 fw upgrade */
        eRCode = upgrade_gobi3k_fw(pDestinationPath);
    }

    return eRCode;
}

ULONG GetImagesPreference(
    ULONG *pImageListSize,
    BYTE  *pImageList )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiFmsGetImagesPreferenceResp response;

    /*
     * pImageListSize and pImagePref are OUT parameters and hence
     * should not be NULL
     */
    if ( !pImageListSize ||
         !pImageList )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiFmsGetImagesPreference( &paramLength,
                                              pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pImageListSize = pImageListSize;
        slmemset( (char *)pImageList, 0, *pImageListSize );
        response.pImageList     = (struct PrefImageList *)pImageList;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiFmsGetImagesPreference( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetImagesPreference(
    ULONG imageListSize,
    BYTE  *pImageList,
    ULONG bForceDownload,
    BYTE  modemIndex,
    ULONG *pImageTypesSize,
    BYTE  *pImageTypes )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiFmsSetImagesPreferenceResp response;

    /* pImageTypesSize and pImageTypes are OUT parameter and hence
     * should not be NULL
     */
    if ( !pImageTypesSize ||
         !pImageTypes )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiFmsSetImagesPreference( &paramLength,
                                              pOutParam,
                                              imageListSize,
                                              pImageList,
                                              bForceDownload,
                                              modemIndex );
    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pImageTypesSize = pImageTypesSize;
        response.pImageTypes     = pImageTypes;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiFmsSetImagesPreference( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetStoredImages(
    ULONG *pImageListSize,
    BYTE  *pImageList )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiFmsGetStoredImagesResp response;

    /* OUT parameters should not be NULL.*/
    if ( !pImageListSize ||
         !pImageList )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Pack QMI message */
    resultCode = PkQmiFmsGetStoredImages( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Send QMI request */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Intialize the buffer provided by user with 0 */
        slmemset( (char *)pImageList, 0, *( pImageListSize ) );

        /* Copy the obtained values to the function OUT parameters */
        response.pImagelistSize = pImageListSize;
        response.pImageList = (struct ImageList  *)pImageList;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiFmsGetStoredImages( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG DeleteStoredImage(
    ULONG imageInfoSize,
    BYTE  *pImageInfo )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiFmsDeleteStoredImageResp response;

    /*
     * imageInfoSize should be greater than 0 and pImageInfo should not be NULL
     */
    if ( !( 0 < imageInfoSize ) ||
         !pImageInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiFmsDeleteStoredImage( &paramLength,
                                            pOutParam,
                                            pImageInfo );
    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiFmsDeleteStoredImage( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

BOOL IsSierraGobiDevice()
{
    BYTE stringsize = 128;
    CHAR ModelId[stringsize];
    CHAR *pstr = NULL;

    GetModelID( stringsize,
                ModelId );

    pstr = strstr (ModelId, "SL9090");
    if( NULL != pstr )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
