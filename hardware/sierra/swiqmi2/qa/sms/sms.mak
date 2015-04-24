#-------------------------------------------------------------------------------
#
#  Name:
#    sms.mak
#
#  Description:
#    Makefile to build the pkgs/qa/sms package
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
SRCS_SMS = sms/src/qaGobiApiSms.c \
           sms/src/qaSmsListMessages.c \
           sms/src/qaSmsDelete.c \
           sms/src/qaSmsGetSMSCAddress.c \
           sms/src/qaSmsSetSMSCAddress.c \
           sms/src/qaSmsRawSend.c \
           sms/src/qaSmsRawWrite.c \
           sms/src/qaSmsRawRead.c \
           sms/src/qaSmsModifyTag.c \
           sms/src/qaSmsSLQSGetBroadcastConfig.c \
           sms/src/qaSmsSLQSSetBroadcastConfig.c \
           sms/src/qaSmsSLQSSetBroadcastActivation.c \
           sms/src/qaEncodeDecodePDU.c \
           sms/src/qaSmsSLQSGetTransLayerInfo.c \
           sms/src/qaSmsSLQSGetTransNWRegInfo.c \
           sms/src/qaSmsSLQSGetIndicationRegister.c \
           sms/src/qaSmsSLQSSetIndicationRegister.c \
           sms/src/qaSmsSLQSSetRoutes.c \
           sms/src/qaSmsSLQSGetMessageProtocol.c \
           sms/src/qaSmsSLQSGetMaxStorageSize.c

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
SMSOBJ   = $(OBJSDIR)/qaGobiApiSms.o \
           $(OBJSDIR)/qaSmsListMessages.o \
           $(OBJSDIR)/qaSmsDelete.o \
           $(OBJSDIR)/qaSmsGetSMSCAddress.o \
           $(OBJSDIR)/qaSmsSetSMSCAddress.o \
           $(OBJSDIR)/qaSmsRawSend.o \
           $(OBJSDIR)/qaSmsRawWrite.o \
           $(OBJSDIR)/qaSmsRawRead.o \
           $(OBJSDIR)/qaSmsModifyTag.o \
           $(OBJSDIR)/qaSmsSLQSGetBroadcastConfig.o \
           $(OBJSDIR)/qaSmsSLQSSetBroadcastConfig.o \
           $(OBJSDIR)/qaSmsSLQSSetBroadcastActivation.o \
           $(OBJSDIR)/qaEncodeDecodePDU.o \
           $(OBJSDIR)/qaSmsSLQSGetTransLayerInfo.o \
           $(OBJSDIR)/qaSmsSLQSGetTransNWRegInfo.o \
           $(OBJSDIR)/qaSmsSLQSGetIndicationRegister.o \
           $(OBJSDIR)/qaSmsSLQSSetIndicationRegister.o \
           $(OBJSDIR)/qaSmsSLQSSetRoutes.o \
           $(OBJSDIR)/qaSmsSLQSGetMessageProtocol.o \
           $(OBJSDIR)/qaSmsSLQSGetMaxStorageSize.o
