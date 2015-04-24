#-------------------------------------------------------------------------------
#
#  Name:
#    dcs.mak
#
#  Description:
#    Makefile to build the pkgs/qa/dcs package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#
#   The above targets can be made with the following command:
#
# Copyright (c) 2011 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
SRCS_DCS =  dcs/src/qaGobiApiDcs.c \
            dcs/src/qaDcsQCWWAN2kConnect.c \
            dcs/src/qaDcsQCWWAN2kEnumerateDevices.c \
            dcs/src/qaDcsQCWWAN2kGetConnectedDeviceID.c \
            dcs/src/qaDcsQCWWANDisconnect.c \
            dcs/src/qaDcsSLQSGetUsbPortNames.c \
            dcs/src/qaDcsSLQSGetDeviceMode.c

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
DCSOBJ  =   $(OBJSDIR)/qaGobiApiDcs.o \
            $(OBJSDIR)/qaDcsQCWWAN2kConnect.o \
            $(OBJSDIR)/qaDcsQCWWAN2kEnumerateDevices.o \
            $(OBJSDIR)/qaDcsQCWWAN2kGetConnectedDeviceID.o \
            $(OBJSDIR)/qaDcsQCWWANDisconnect.o \
            $(OBJSDIR)/qaDcsSLQSGetUsbPortNames.o \
            $(OBJSDIR)/qaDcsSLQSGetDeviceMode.o

