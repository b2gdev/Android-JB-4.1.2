#-------------------------------------------------------------------------------
#
#  Name:
#    am.mak
#
#  Description:
#    Makefile to build the pkgs/am package 
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     amtest        - builds the test program if available
#
#   The above targets can be made with the following command:
#
#     make -f am.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted
#
# Copyright (c) 2011 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings-
#
#-------------------------------------------------------------------------------
PKG  = am
SRCS = am.c amipctask_sdk.c am_api.c amqmi.c amsdk.c
OBJS = $(OBJSDIR)/am.o $(OBJSDIR)/amipctask_sdk.o \
       $(OBJSDIR)/am_api.o \
       $(OBJSDIR)/amqmi.o \
       $(OBJSDIR)/amsdk.o

HDRS = amidefs.h amudefs.h amiproto.h amuproto.h

SLQSAPIOBJ = $(OBJSDIR)/am_api.o 
COREOBJ = $(OBJSDIR)/am.o \
          $(OBJSDIR)/amqmi.o
SDKOBJ = $(OBJSDIR)/amipctask_sdk.o $(OBJSDIR)/amsdk.o
DEMUXOBJ =

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(SLQSAPILIB) $(CORELIB) $(SDKLIB)

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
