#-------------------------------------------------------------------------------
#
#  Name:
#    ic.mak
#
#  Description:
#    Makefile to build the pkgs/ic package 
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
#     make -f ic.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted

# Copyright (c) 2011 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
PKG  = ic
SRCS = icticktask_sdk.c icipc_api.c icipc_sdk.c icsmt_sdk.c 
OBJS = $(OBJSDIR)/icipc_api.o $(OBJSDIR)/icipc_sdk.o $(OBJSDIR)/icsmt_sdk.o \
        $(OBJSDIR)/icticktask_sdk.o 
       
HDRS = icudefs.h icidefs.h icuproto.h iciproto.h  ../ql/qludefs.h

SLQSAPIOBJ = $(OBJSDIR)/icipc_api.o
DEMUXOBJ = $(OBJSDIR)/icipc_api.o
SDKOBJ = $(OBJSDIR)/icipc_sdk.o $(OBJSDIR)/icsmt_sdk.o \
            $(OBJSDIR)/icticktask_sdk.o

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(QMIAPILIB) $(CORELIB) $(SDKLIB) 
#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
