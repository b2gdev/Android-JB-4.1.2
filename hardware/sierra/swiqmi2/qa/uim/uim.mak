#-------------------------------------------------------------------------------
#
#  Name:
#    uim.mak
#
#  Description:
#    Makefile to build the pkgs/qa/uim package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#
#   The above targets can be made with the following command:
#
# Copyright (c) 2012 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
SRCS_UIM =    uim/src/qaGobiApiUim.c \
              uim/src/qaUimSLQSUimReset.c \
              uim/src/qaUimSLQSUimPowerDown.c \
              uim/src/qaUimSLQSUimGetCardStatus.c \
              uim/src/qaGobiApiUimCommon.c \
              uim/src/qaUimSLQSUimSetPinProtection.c \
              uim/src/qaUimSLQSUimVerifyPin.c \
              uim/src/qaUimSLQSUimChangePin.c \
              uim/src/qaUimSLQSUimUnblockPin.c \
              uim/src/qaUimSLQSUimEventRegister.c \
              uim/src/qaUimSLQSUimRefreshOK.c \
              uim/src/qaUimSLQSUimRefreshRegister.c \
              uim/src/qaUimSLQSUimRefreshComplete.c \
              uim/src/qaUimSLQSUimRefreshGetLastEvent.c \
              uim/src/qaUimSLQSUimGetFileAttributes.c \
              uim/src/qaUimSLQSUimDepersonalization.c \
              uim/src/qaUimSLQSUimAuthenticate.c

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
UIMOBJ   =    $(OBJSDIR)/qaGobiApiUim.o \
              $(OBJSDIR)/qaUimSLQSUimReset.o \
              $(OBJSDIR)/qaUimSLQSUimPowerDown.o \
              $(OBJSDIR)/qaUimSLQSUimGetCardStatus.o \
              $(OBJSDIR)/qaGobiApiUimCommon.o \
              $(OBJSDIR)/qaUimSLQSUimSetPinProtection.o \
              $(OBJSDIR)/qaUimSLQSUimVerifyPin.o \
              $(OBJSDIR)/qaUimSLQSUimChangePin.o \
              $(OBJSDIR)/qaUimSLQSUimUnblockPin.o \
              $(OBJSDIR)/qaUimSLQSUimEventRegister.o \
              $(OBJSDIR)/qaUimSLQSUimRefreshOK.o \
              $(OBJSDIR)/qaUimSLQSUimRefreshRegister.o \
              $(OBJSDIR)/qaUimSLQSUimRefreshComplete.o \
              $(OBJSDIR)/qaUimSLQSUimRefreshGetLastEvent.o \
              $(OBJSDIR)/qaUimSLQSUimGetFileAttributes.o \
              $(OBJSDIR)/qaUimSLQSUimDepersonalization.o \
              $(OBJSDIR)/qaUimSLQSUimAuthenticate.o
