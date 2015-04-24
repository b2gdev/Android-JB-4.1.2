#-------------------------------------------------------------------------------
#
#  Name:
#    mm.mak
#
#  Description:
#    Makefile to build the pkgs/mm package 
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     mmtest       - builds the test program if available
#
#   The above targets can be made with the following command:
#
#     make -f mm.mak [target]
#
# Copyright (c) 2008 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
PKG  = mm
SRCS = mm.c 
OBJS = $(OBJSDIR)/mm.o 
HDRS = mmidefs.h mmudefs.h mmiproto.h mmuproto.h

SDKOBJ = $(OBJS)

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(SDKLIB)

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
