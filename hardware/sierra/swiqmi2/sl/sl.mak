#-------------------------------------------------------------------------------
#
#  Name:
#    sl.mak
#
#  Description:
#    Makefile to build the pkgs/sl package 
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     sltest        - builds the test program if available
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
PKG  = sl
SRCS = slcommon.c slmanage.c slvsprintf.c slucs2.c
OBJS = $(OBJSDIR)/slcommon.o $(OBJSDIR)/slmanage.o $\
       $(OBJSDIR)/slvsprintf.o $(OBJSDIR)/slucs2.o
HDR = slidefs.h sludefs.h sliproto.h sluproto.h

COREOBJ = $(OBJS)
SDKOBJ = $(OBJS)

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(CORELIB) $(SDKLIB) 

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
