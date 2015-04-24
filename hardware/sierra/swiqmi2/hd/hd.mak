#-------------------------------------------------------------------------------
#
#  Name:
#    hd.mak
#
#  Description:
#    Makefile to build the pkgs/hd package 
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     hdtest        - builds the test program if available
#
#   The above targets can be made with the following command:
#
#     make -f hd.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted

# Copyright (c) 2008 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
PKG  = hd
SRCS = hdrx.c hdtx.c hduser.c 
OBJS = $(OBJSDIR)/hdrx.o $(OBJSDIR)/hdtx.o $(OBJSDIR)/hduser.o 
HDRS = hdidefs.h hdiproto.h hdudefs.h hduproto.h

SDKOBJ = $(OBJS)

# LIB and OBJ files required by $(PKG) test program
TESTLIBS =  $(CORELIB) $(SDKLIB) 

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
