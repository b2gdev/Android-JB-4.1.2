#-------------------------------------------------------------------------------
#
#  Name:
#    er.mak
#
#  Description:
#    Makefile to build the pkgs/er package 
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     ertest        - builds the test program if available-
#
#   The above targets can be made with the following command:
#
#     make -f er.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted
#
# Copyright (c) 2011 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
PKG = er
SRCS = er_api.c er_sdk.c
OBJS = $(OBJSDIR)/er_api.o $(OBJSDIR)/er_sdk.o
HDRS = erudefs.h eruproto.h

SDKOBJ = $(OBJSDIR)/er_sdk.o
SLQSAPIOBJ = $(OBJSDIR)/er_api.o
DEMUXOBJ  = $(OBJSDIR)/er_api.o

# LIB and OBJ files required by $(PKG) test program
APITESTLIBS = $(SLQSAPILIB) $(CORELIB) 
SDKTESTLIBS = $(CORELIB) $(SDKLIB)

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
