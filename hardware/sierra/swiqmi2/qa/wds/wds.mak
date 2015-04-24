#-------------------------------------------------------------------------------
#
#  Name:
#    wds.mak
#
#  Description:
#    Makefile to build the pkgs/qa/wds package 
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
SRCS_WDS =  wds/src/qaGobiApiWds.c \
            wds/src/qaWdsStartNetworkInterface.c \
            wds/src/qaWdsStopNetworkInterface.c \
            wds/src/qaWdsSetMipMode.c \
            wds/src/qaWdsSetMipParams.c \
            wds/src/qaWdsGetAutoconnectSetting.c \
            wds/src/qaWdsGetDefaultSettings.c \
            wds/src/qaWdsGetPktSrvcStatus.c \
            wds/src/qaWdsGetPktStatistics.c \
            wds/src/qaWdsGetDormancyState.c \
            wds/src/qaWdsGetDataBearerTechnology.c \
            wds/src/qaWdsGetCurDataBearerTechnology.c \
            wds/src/qaWdsGetCallDuration.c \
            wds/src/qaWdsGetRuntimeSettings.c \
            wds/src/qaWdsSLQSGetRuntimeSettings.c \
            wds/src/qaWdsGetCurrentChannelRate.c \
            wds/src/qaWdsModifyProfile.c \
            wds/src/qaWdsGetMipMode.c \
            wds/src/qaWdsReadMipProfile.c \
            wds/src/qaWdsGetLastMipStatus.c \
            wds/src/qaWdsSLQSGetProfileSettings.c \
            wds/src/qaWdsSLQSSetIPFamilyPreference.c \
            wds/src/qaWdsSLQSDeleteProfile.c \
            wds/src/qaWdsSLQSCreateProfile.c \
            wds/src/qaWdsSLQSACSettings.c \
            wds/src/qaWdsSLQSModifyProfile.c \
            wds/src/qaWdsSLQSSet3GPPConfigItem.c \
            wds/src/qaWdsSLQSGet3GPPConfigItem.c \
            wds/src/qaWdsModifyMipProfile.c \
            wds/src/qaWdsSLQSWdsSetEventReport.c \
            wds/src/qaWdsSLQSWdsSwiPDPRuntimeSettings.c

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
WDSOBJ  =   $(OBJSDIR)/qaGobiApiWds.o \
            $(OBJSDIR)/qaWdsStartNetworkInterface.o \
            $(OBJSDIR)/qaWdsStopNetworkInterface.o \
            $(OBJSDIR)/qaWdsSetMipMode.o \
            $(OBJSDIR)/qaWdsSetMipParams.o \
            $(OBJSDIR)/qaWdsGetAutoconnectSetting.o \
            $(OBJSDIR)/qaWdsGetDefaultSettings.o \
            $(OBJSDIR)/qaWdsGetPktSrvcStatus.o \
            $(OBJSDIR)/qaWdsGetPktStatistics.o \
            $(OBJSDIR)/qaWdsGetDormancyState.o \
            $(OBJSDIR)/qaWdsGetDataBearerTechnology.o \
            $(OBJSDIR)/qaWdsGetCurDataBearerTechnology.o \
            $(OBJSDIR)/qaWdsGetCallDuration.o \
            $(OBJSDIR)/qaWdsGetRuntimeSettings.o \
            $(OBJSDIR)/qaWdsSLQSGetRuntimeSettings.o \
            $(OBJSDIR)/qaWdsGetCurrentChannelRate.o \
            $(OBJSDIR)/qaWdsModifyProfile.o \
            $(OBJSDIR)/qaWdsGetMipMode.o \
            $(OBJSDIR)/qaWdsReadMipProfile.o \
            $(OBJSDIR)/qaWdsGetLastMipStatus.o \
            $(OBJSDIR)/qaWdsSLQSGetProfileSettings.o \
            $(OBJSDIR)/qaWdsSLQSSetIPFamilyPreference.o \
            $(OBJSDIR)/qaWdsSLQSDeleteProfile.o \
            $(OBJSDIR)/qaWdsSLQSCreateProfile.o \
            $(OBJSDIR)/qaWdsSLQSACSettings.o \
            $(OBJSDIR)/qaWdsSLQSModifyProfile.o \
            $(OBJSDIR)/qaWdsSLQSSet3GPPConfigItem.o \
            $(OBJSDIR)/qaWdsSLQSGet3GPPConfigItem.o \
            $(OBJSDIR)/qaWdsModifyMipProfile.o \
            $(OBJSDIR)/qaWdsSLQSWdsSetEventReport.o \
            $(OBJSDIR)/qaWdsSLQSWdsSwiPDPRuntimeSettings.o
