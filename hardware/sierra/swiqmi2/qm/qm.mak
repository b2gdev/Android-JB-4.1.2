#-------------------------------------------------------------------------------
#
#  Name:
#    qm.mak
#
#  Description:
#    Makefile to build the pkgs/qm package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     qmtest        - builds the test program if available
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
PKG  =  qm
SRCS =  qmtask_sdk.c qmcommon.c qm_api.c qmdcs.c qmdcscommon.c qmfms.c\
        qmfmscommon.c qmqmisvc.c qmparser.c

OBJS =  $(OBJSDIR)/qmtask_sdk.o $(OBJSDIR)/qmcommon.o $(OBJSDIR)/qm_api.o\
        $(OBJSDIR)/qmqmisvc.o $(OBJSDIR)/qmdcs.o $(OBJSDIR)/qmdcscommon.o\
        $(OBJSDIR)/qmfms.o $(OBJSDIR)/qmfmscommon.o $(OBJSDIR)/qmparser.o

HDRS =  qmidefs.h qmudefs.h qmiproto.h qmuproto.h qmqmisvc.h qmdcs.h qmfms.h

SLQSAPIOBJ  =   $(OBJSDIR)/qm_api.o
DEMUXOBJ    =
COREOBJ     =   $(OBJSDIR)/qmcommon.o $(OBJSDIR)/qmdcscommon.o\
                $(OBJSDIR)/qmfmscommon.o

SDKOBJ      =   $(OBJSDIR)/qmtask_sdk.o $(OBJSDIR)/qmdcs.o $(OBJSDIR)/qmfms.o\
                $(OBJSDIR)/qmqmisvc.o $(OBJSDIR)/qmparser.o

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(SLQSAPILIB) $(CORELIB) $(SDKLIB)

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include qmconfig.mak
include ../gen.mak
