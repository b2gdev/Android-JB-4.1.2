#-------------------------------------------------------------------------------
#
#  Name:
#    im.mak
#
#  Description:
#    Makefile to build the pkgs/im package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     imtest        - builds the test program if available
#
#   The above targets can be made with the following command:
#
#     make -f im.mak [CPU={i386, arm9, ppc}] [target]
#       defaults to i386 if CPU is omitted
#
# Copyright (c) 2011-2012 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings-
#
#-------------------------------------------------------------------------------
PKG  =  im

SRCS =  imssdp_dload.c\
        imssdp_done.c\
        imssdp_donefinal.c\
        imssdp_error.c\
        imssdp_hello.c\
        imssdp_image_preference.c\
        imssdp_open_unframed.c\
        imssdp_unframed_write.c\
        imssdp.c\
        imapi.c\
        imcommon.c\
        imssdp_dload_to_sdp.c\
        imtask_sdk.c\
        imutils.c\
        imuser.c\
        imtaskgobi.c\
        imssdpwrapper.c

HDRS =  imudefs.h \
        imidefs.h \
        imssdp_dload.h\
        imssdp_done.h\
        imssdp_donefinal.h\
        imssdp_error.h\
        imssdp_hello.h\
        imssdp_image_preference.h\
        imssdp_open_unframed.h\
        imssdp_unframed_write.h\
        imssdp.h\
        imapi.h\
        imcommon.h\
        imssdp_dload_to_sdp.h\
        imidefs.h

OBJS =  $(OBJSDIR)/imssdp_dload.o\
        $(OBJSDIR)/imssdp_done.o\
        $(OBJSDIR)/imssdp_donefinal.o\
        $(OBJSDIR)/imssdp_error.o\
        $(OBJSDIR)/imssdp_hello.o\
        $(OBJSDIR)/imssdp_image_preference.o\
        $(OBJSDIR)/imssdp_open_unframed.o\
        $(OBJSDIR)/imssdp_unframed_write.o\
        $(OBJSDIR)/imssdp.o\
        $(OBJSDIR)/imapi.o\
        $(OBJSDIR)/imcommon.o\
        $(OBJSDIR)/imssdp_dload_to_sdp.o\
        $(OBJSDIR)/imtask_sdk.o\
        $(OBJSDIR)/imutils.o\
        $(OBJSDIR)/imuser.o\
        $(OBJSDIR)/imtaskgobi.o\
        $(OBJSDIR)/imssdpwrapper.o

SLQSAPIOBJ  =
DEMUXOBJ    =
COREOBJ     =
SDKOBJ      =   $(OBJS)

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(SDKLIB)

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include imconfig.mak
include ../gen.mak
