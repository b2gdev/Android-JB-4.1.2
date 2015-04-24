#-------------------------------------------------------------------------------
#
#  Name:
#    ql.mak
#
#  Description:
#    Makefile to build the pkgs/ql package 
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
#     make -f ql.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted

# Copyright (c) 2008 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
PKG  = ql
SRCS = ql.c
OBJS = $(OBJSDIR)/ql.o
HDRS = qludefs.h

SDKOBJ = $(OBJS)

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(OBJS)

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
