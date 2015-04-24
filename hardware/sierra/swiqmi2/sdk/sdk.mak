#
#  Name:
#    sdk.mak
#
#  Description:
#    Makefile to build the pkgs/sdk package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     qltest        - builds the test program if available
#
#   The above targets can be made with the following command:
#
#     make -f sdk.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted

# Copyright (c) 2011 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
PKG  = sdk
SRCS = sdk.c
OBJS = $(OBJSDIR)/sdk.o
HDRS = sdkidefs.h

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(OBJS) $(SDKLIB) $(CORELIB)

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
