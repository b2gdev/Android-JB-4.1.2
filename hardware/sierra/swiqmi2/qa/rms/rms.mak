#-------------------------------------------------------------------------------
#
#  Name:
#    rms.mak
#
#  Description:
#    Makefile to build the pkgs/qa/rms package
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#
#   The above targets can be made with the following command:
#
# Copyright (c) 2010 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings
#-------------------------------------------------------------------------------
SRCS_RMS = rms/src/qaGobiApiRms.c \
           rms/src/qaRmsGetEventSmsWake.c \
           rms/src/qaRmsSetEventSmsWake.c

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
RMSOBJ   = $(OBJSDIR)/qaGobiApiRms.o \
           $(OBJSDIR)/qaRmsGetEventSmsWake.o \
           $(OBJSDIR)/qaRmsSetEventSmsWake.o

