#-------------------------------------------------------------------------------
#
#  Name:
#    nas.mak
#
#  Description:
#    Makefile to build the pkgs/qa/nas package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#
#   The above targets can be made with the following command:
#
# Copyright (c) 2010-2012 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
SRCS_NAS  = nas/src/qaGobiApiNas.c \
            nas/src/qaGobiApiNasCommon.c \
            nas/src/qaNasGetSignalStrength.c \
            nas/src/qaNasGetHomeNetwork.c \
            nas/src/qaNasGetServingSystem.c \
            nas/src/qaNasInitiateNetworkRegistration.c \
            nas/src/qaNasPerformNetworkScan.c \
            nas/src/qaNasSetTechPreference.c \
            nas/src/qaNasGetTechnologyPreference.c \
            nas/src/qaNasGetRFBandInfo.c \
            nas/src/qaNasInitiateAttach.c \
            nas/src/qaNasGetACCOLC.c \
            nas/src/qaNasSetACCOLC.c \
            nas/src/qaNasSetDeviceConfig.c \
            nas/src/qaNasGetDeviceConfig.c \
            nas/src/qaNasGetAnAAAStatus.c \
            nas/src/qaNasGetSLQSServingSystem.c \
            nas/src/qaNasSetSysSelectPreference.c \
            nas/src/qaNasIndicationRegister.c \
            nas/src/qaNasSLQSGetSignalStrength.c \
            nas/src/qaNasSLQSPerformNetworkScan.c \
            nas/src/qaNasGetSysSelectPreference.c \
            nas/src/qaNasSLQSNasGetSysInfo.c \
            nas/src/qaNasSLQSNasSwiModemStatus.c \
            nas/src/qaNasSLQSGetHDRColorCode.c \
            nas/src/qaNasSLQSGetTxRxInfo.c \
            nas/src/qaNasSLQSGetSigInfo.c \
            nas/src/qaNasSLQSGetPLMNName.c \
            nas/src/qaNasGetOperatorNameData.c \
            nas/src/qaNasSLQSGet3GPP2Subscription.c \
            nas/src/qaNasSLQSNasGetCellLocationInfo.c \
            nas/src/qaNasSLQSInitiateNetworkRegistration.c
#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------

NASOBJ  = $(OBJSDIR)/qaGobiApiNas.o \
          $(OBJSDIR)/qaGobiApiNasCommon.o \
          $(OBJSDIR)/qaNasGetSignalStrength.o \
          $(OBJSDIR)/qaNasGetHomeNetwork.o \
          $(OBJSDIR)/qaNasGetServingSystem.o \
          $(OBJSDIR)/qaNasInitiateNetworkRegistration.o \
          $(OBJSDIR)/qaNasPerformNetworkScan.o \
          $(OBJSDIR)/qaNasSetTechPreference.o \
          $(OBJSDIR)/qaNasGetTechnologyPreference.o \
          $(OBJSDIR)/qaNasGetRFBandInfo.o \
          $(OBJSDIR)/qaNasInitiateAttach.o \
          $(OBJSDIR)/qaNasGetACCOLC.o \
          $(OBJSDIR)/qaNasSetACCOLC.o \
          $(OBJSDIR)/qaNasSetDeviceConfig.o \
          $(OBJSDIR)/qaNasGetDeviceConfig.o \
          $(OBJSDIR)/qaNasGetAnAAAStatus.o \
          $(OBJSDIR)/qaNasGetSLQSServingSystem.o \
          $(OBJSDIR)/qaNasSetSysSelectPreference.o \
          $(OBJSDIR)/qaNasIndicationRegister.o \
          $(OBJSDIR)/qaNasSLQSGetSignalStrength.o \
          $(OBJSDIR)/qaNasSLQSPerformNetworkScan.o \
          $(OBJSDIR)/qaNasGetSysSelectPreference.o \
          $(OBJSDIR)/qaNasSLQSNasGetSysInfo.o \
          $(OBJSDIR)/qaNasSLQSNasSwiModemStatus.o \
          $(OBJSDIR)/qaNasSLQSGetHDRColorCode.o \
          $(OBJSDIR)/qaNasSLQSGetTxRxInfo.o \
          $(OBJSDIR)/qaNasSLQSGetSigInfo.o \
          $(OBJSDIR)/qaNasSLQSGetPLMNName.o \
          $(OBJSDIR)/qaNasGetOperatorNameData.o \
          $(OBJSDIR)/qaNasSLQSGet3GPP2Subscription.o \
          $(OBJSDIR)/qaNasSLQSNasGetCellLocationInfo.o \
          $(OBJSDIR)/qaNasSLQSInitiateNetworkRegistration.o
