#-------------------------------------------------------------------------------
#
#  Name:
#    os.mak
#
#  Description:
#    Makefile to build the pkgs/os package
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
#     make -f os.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted

# Copyright (c) 2011-2012 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
PKG  = os
SRCS = swi_osapi.c swi_ossdk.c swi_ossdkcheck.c

OBJS = $(OBJSDIR)/swi_osapi.o $(OBJSDIR)/swi_ossdk.o \
       $(OBJSDIR)/swi_ossdkcheck.o

HDRS = swi_ossdk.h swi_osapi.h

SLQSAPIOBJ  = $(OBJSDIR)/swi_osapi.o
DEMUXOBJ    = $(OBJSDIR)/swi_osapi.o
SDKOBJ      = $(OBJS)

# LIB and OBJ files required by $(PKG) test program
TESTLIBS =

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include osconfig.mak
include ../gen.mak

