#-------------------------------------------------------------------------------
#
#  Name:
#    pds.mak
#
#  Description:
#    Makefile to build the pkgs/qa/pds package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#
#   The above targets can be made with the following command:
#
# Copyright (c) 2011 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
SRCS_PDS = pds/src/qaGobiApiPds.c \
           pds/src/qaPdsGetGPSServiceState.c \
           pds/src/qaPdsSetGPSServiceState.c \
           pds/src/qaPdsStartTrackingSession.c \
           pds/src/qaPdsEndTrackingSession.c \
           pds/src/qaPdsGetDefaultTrackingSession.c \
           pds/src/qaPdsSetDefaultTrackingSession.c \
           pds/src/qaPdsSetXtraParameters.c \
           pds/src/qaPdsGetAGPSConfig.c \
           pds/src/qaPdsSetAGPSConfig.c \
           pds/src/qaPdsGetAutoTrackingState.c \
           pds/src/qaPdsSetAutoTrackingState.c \
           pds/src/qaPdsGetComPortConfig.c \
           pds/src/qaPdsSetComPortConfig.c \
           pds/src/qaPdsForceXtraDownload.c \
           pds/src/qaPdsGetXtraParameters.c \
           pds/src/qaPdsResetGPSServicedata.c \
           pds/src/qaPdsInjectRelativeTimeRef.c \
           pds/src/qaPdsInjectAbsoluteTimeRef.c \
           pds/src/qaPdsInjectPositionData.c


#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
PDSOBJ   = $(OBJSDIR)/qaGobiApiPds.o \
           $(OBJSDIR)/qaPdsGetGPSServiceState.o \
           $(OBJSDIR)/qaPdsSetGPSServiceState.o \
           $(OBJSDIR)/qaPdsStartTrackingSession.o \
           $(OBJSDIR)/qaPdsEndTrackingSession.o \
           $(OBJSDIR)/qaPdsGetDefaultTrackingSession.o \
           $(OBJSDIR)/qaPdsSetDefaultTrackingSession.o \
           $(OBJSDIR)/qaPdsSetXtraParameters.o \
           $(OBJSDIR)/qaPdsGetAGPSConfig.o \
           $(OBJSDIR)/qaPdsSetAGPSConfig.o \
           $(OBJSDIR)/qaPdsGetAutoTrackingState.o \
           $(OBJSDIR)/qaPdsSetAutoTrackingState.o \
           $(OBJSDIR)/qaPdsGetComPortConfig.o \
           $(OBJSDIR)/qaPdsSetComPortConfig.o \
           $(OBJSDIR)/qaPdsForceXtraDownload.o \
           $(OBJSDIR)/qaPdsGetXtraParameters.o \
           $(OBJSDIR)/qaPdsResetGPSServicedata.o \
           $(OBJSDIR)/qaPdsInjectRelativeTimeRef.o \
           $(OBJSDIR)/qaPdsInjectAbsoluteTimeRef.o \
           $(OBJSDIR)/qaPdsInjectPositionData.o




