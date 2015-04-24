#-------------------------------------------------------------------------------
#
#  Name:
#    sm.mak
#
#  Description:
#    Makefile to build the pkgs/sm package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     smtest        - builds the test program if available
#
#   The above targets can be made with the following command:
#
#     make -f ds.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted
#
# Copyright (c) 2011 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings-
#
#-------------------------------------------------------------------------------
PKG  = sm
SRCS = sm.c
OBJS = $(OBJSDIR)/sm.o

HDRS = smudefs.h smuproto.h

SLQSAPIOBJ  =
DEMUXOBJ    =
COREOBJ     =
SDKOBJ      = $(OBJS)

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(CORELIB) $(SDKLIB) $(CORELIB)

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
