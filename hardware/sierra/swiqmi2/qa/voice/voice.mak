#-------------------------------------------------------------------------------
#
#  Name:
#    voice.mak
#
#  Description:
#    Makefile to build the pkgs/qa/voice package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#
#   The above targets can be made with the following command:
#
# Copyright (c) 2011-12 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
SRCS_VOICE =    voice/src/qaGobiApiVoice.c \
                voice/src/qaGobiApiVoiceCommon.c \
                voice/src/qaVoiceOriginateUSSD.c \
                voice/src/qaVoiceAnswerUSSD.c \
                voice/src/qaVoiceCancelUSSD.c \
                voice/src/qaVoiceSLQSVoiceDialCall.c \
                voice/src/qaVoiceSLQSVoiceEndCall.c \
                voice/src/qaVoiceSLQSVoiceSetSUPSService.c \
                voice/src/qaVoiceSLQSVoiceSetConfig.c \
                voice/src/qaVoiceSLQSVoiceAnswerCall.c \
                voice/src/qaVoiceSLQSVoiceGetCLIR.c \
                voice/src/qaVoiceSLQSVoiceGetCLIP.c \
                voice/src/qaVoiceSLQSVoiceGetCallWaiting.c \
                voice/src/qaVoiceSLQSVoiceGetCallBarring.c \
                voice/src/qaVoiceSLQSVoiceGetCallForwardingStatus.c \
                voice/src/qaVoiceSLQSVoiceSetCallBarringPassword.c \
                voice/src/qaVoiceSLQSVoiceGetCallInfo.c \
                voice/src/qaVoiceSLQSVoiceGetAllCallInfo.c \
                voice/src/qaVoiceSLQSVoiceManageCalls.c \
                voice/src/qaVoiceSLQSVoiceBurstDTMF.c \
                voice/src/qaVoiceSLQSVoiceStartContDTMF.c \
                voice/src/qaVoiceSLQSVoiceStopContDTMF.c \
                voice/src/qaVoiceSLQSVoiceSendFlash.c \
                voice/src/qaVoiceSLQSVoiceSetPreferredPrivacy.c \
                voice/src/qaVoiceSLQSVoiceIndicationRegister.c \
                voice/src/qaVoiceSLQSVoiceGetConfig.c \
                voice/src/qaVoiceSLQSVoiceOrigUSSDNoWait.c \
                voice/src/qaVoiceSLQSVoiceBindSubscription.c \
                voice/src/qaVoiceSLQSVoiceALSSetLineSwitching.c \
                voice/src/qaVoiceSLQSVoiceALSSelectLine.c \
                voice/src/qaVoiceSLQSVoiceGetCOLP.c \
                voice/src/qaVoiceSLQSVoiceGetCOLR.c \
                voice/src/qaVoiceSLQSVoiceGetCNAP.c 

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
VOICEOBJ   =    $(OBJSDIR)/qaGobiApiVoice.o \
                $(OBJSDIR)/qaGobiApiVoiceCommon.o \
                $(OBJSDIR)/qaVoiceOriginateUSSD.o \
                $(OBJSDIR)/qaVoiceAnswerUSSD.o \
                $(OBJSDIR)/qaVoiceCancelUSSD.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceDialCall.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceEndCall.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceSetSUPSService.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceSetConfig.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceAnswerCall.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetCLIR.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetCLIP.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetCallWaiting.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetCallBarring.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetCallForwardingStatus.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceSetCallBarringPassword.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetCallInfo.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetAllCallInfo.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceManageCalls.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceBurstDTMF.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceStartContDTMF.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceStopContDTMF.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceSendFlash.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceSetPreferredPrivacy.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceIndicationRegister.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetConfig.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceOrigUSSDNoWait.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceBindSubscription.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceALSSetLineSwitching.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceALSSelectLine.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetCOLP.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetCOLR.o \
                $(OBJSDIR)/qaVoiceSLQSVoiceGetCNAP.o  
