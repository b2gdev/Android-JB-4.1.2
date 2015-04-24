#-------------------------------------------------------------------------------
#
#  Name:
#    dms.mak
#
#  Description:
#    Makefile to build the pkgs/qa/dms package 
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#
#   The above targets can be made with the following command:
#
# Copyright (c) 2010 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
SRCS_DMS = dms/src/qaGobiApiDms.c \
           dms/src/qaDmsGetDeviceMfr.c \
           dms/src/qaDmsGetDeviceModelID.c \
           dms/src/qaDmsGetDeviceRevisionID.c \
           dms/src/qaDmsGetDeviceHardwareRev.c \
           dms/src/qaDmsGetMSISDN.c \
           dms/src/qaDmsUIMGetIMSI.c \
           dms/src/qaDmsGetPRLVersion.c \
           dms/src/qaDmsGetDeviceSerialNumbers.c \
           dms/src/qaDmsGetTime.c \
           dms/src/qaDmsUIMSetPINProtection.c \
           dms/src/qaDmsUIMVerifyPIN.c \
           dms/src/qaDmsUIMUnblockPIN.c \
           dms/src/qaDmsUIMChangePIN.c \
           dms/src/qaDmsSetOperatingMode.c \
           dms/src/qaDmsGetOperatingMode.c \
           dms/src/qaDmsUIMGetCkStatus.c \
           dms/src/qaDmsUIMGetICCID.c \
           dms/src/qaDmsUIMGetPINStatus.c \
           dms/src/qaDmsUIMSetCkProtection.c \
           dms/src/qaDmsUIMUnblockCk.c \
           dms/src/qaDmsGetDeviceCap.c \
           dms/src/qaDmsRestoreFactoryDefaults.c \
           dms/src/qaDmsValSerProgramCode.c \
           dms/src/qaDmsActivateAutomatic.c \
           dms/src/qaDmsGetActivationState.c \
           dms/src/qaDmsSlqsUIMGetState.c \
           dms/src/qaDmsGetBandCapability.c \
           dms/src/qaDmsSLQSGetCustFeatures.c \
           dms/src/qaDmsSLQSSetCustFeatures.c \
           dms/src/qaDmsSLQSGetCurrentPRLInfo.c

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
DMSOBJ = $(OBJSDIR)/qaGobiApiDms.o \
         $(OBJSDIR)/qaDmsGetDeviceMfr.o \
         $(OBJSDIR)/qaDmsGetDeviceModelID.o \
         $(OBJSDIR)/qaDmsGetDeviceRevisionID.o \
         $(OBJSDIR)/qaDmsGetDeviceHardwareRev.o \
         $(OBJSDIR)/qaDmsGetMSISDN.o \
         $(OBJSDIR)/qaDmsUIMGetIMSI.o \
         $(OBJSDIR)/qaDmsGetPRLVersion.o \
         $(OBJSDIR)/qaDmsGetDeviceSerialNumbers.o \
         $(OBJSDIR)/qaDmsGetTime.o \
         $(OBJSDIR)/qaDmsUIMSetPINProtection.o \
         $(OBJSDIR)/qaDmsUIMVerifyPIN.o \
         $(OBJSDIR)/qaDmsUIMUnblockPIN.o \
         $(OBJSDIR)/qaDmsUIMChangePIN.o \
         $(OBJSDIR)/qaDmsSetOperatingMode.o \
         $(OBJSDIR)/qaDmsGetOperatingMode.o \
         $(OBJSDIR)/qaDmsUIMGetCkStatus.o \
         $(OBJSDIR)/qaDmsUIMGetICCID.o \
         $(OBJSDIR)/qaDmsUIMGetPINStatus.o \
         $(OBJSDIR)/qaDmsUIMSetCkProtection.o \
         $(OBJSDIR)/qaDmsUIMUnblockCk.o \
         $(OBJSDIR)/qaDmsGetDeviceCap.o \
         $(OBJSDIR)/qaDmsRestoreFactoryDefaults.o \
         $(OBJSDIR)/qaDmsValSerProgramCode.o \
         $(OBJSDIR)/qaDmsActivateAutomatic.o \
         $(OBJSDIR)/qaDmsGetActivationState.o \
         $(OBJSDIR)/qaDmsSlqsUIMGetState.o \
         $(OBJSDIR)/qaDmsGetBandCapability.o \
         $(OBJSDIR)/qaDmsSLQSGetCustFeatures.o \
         $(OBJSDIR)/qaDmsSLQSSetCustFeatures.o \
         $(OBJSDIR)/qaDmsSLQSGetCurrentPRLInfo.o
