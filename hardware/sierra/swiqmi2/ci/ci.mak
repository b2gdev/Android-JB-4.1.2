#-------------------------------------------------------------------------------
#
#  Name:
#    ci.mak
#
#  Description:
#    Makefile to build the pkgs/ci package 
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     ictest        - builds the test program if available
#
#   The above targets can be made with the following command:
#
#     make -f ci.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted

# Copyright (c) 2011 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
PKG  = ci
SRCS = cipackage.c ci_sdk.c ci_api.c
OBJS = $(OBJSDIR)/cipackage.o $(OBJSDIR)/ci_sdk.o $(OBJSDIR)/ci_api.o
       
HDRS = ciudefs.h ciidefs.h ciuproto.h

SLQSAPIOBJ = $(OBJSDIR)/ci_api.o
DEMUXOBJ = 
SDKOBJ = $(OBJSDIR)/ci_sdk.o
COREOBJ = $(OBJSDIR)/cipackage.o 

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(QMIAPILIB) $(CORELIB) $(SDKLIB) 
#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
