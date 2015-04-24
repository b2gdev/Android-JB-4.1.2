#-------------------------------------------------------------------------------
#
#  Name:
#    dl.mak
#
#  Description:
#    Makefile to build the pkgs/dl package 
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     dltest        - builds the test program if available
#
#   The above targets can be made with the following command:
#
#     make -f sl.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted
#
# Copyright (c) 2008 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings- 
#-------------------------------------------------------------------------------
PKG  = dl
SRCS = dlpackage.c
OBJS = $(OBJSDIR)/dlpackage.o
HDRS = dlidefs.h dludefs.h dliproto.h dluproto.h

SDKOBJ = $(OBJS)

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(OBJS) $(CORELIB) $(SDKLIB)
TESTFLAGS = -lrt -lpthread

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
