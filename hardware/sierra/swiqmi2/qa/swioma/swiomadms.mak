#-------------------------------------------------------------------------------
#
#  Name:
#    swiomadms.mak
#
#  Description:
#    Makefile to build the pkgs/qa/swioma package
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
SRCS_SWIOMADMS = swioma/src/qaGobiApiSwiOmadms.c \
                 swioma/src/qaSwiOmaDmSessionStart.c \
                 swioma/src/qaSwiOmaDmSessionCancel.c \
                 swioma/src/qaSwiOmaDmSessionGetInfo.c \
                 swioma/src/qaSwiOmaDmSelection.c \
                 swioma/src/qaSwiOmaDmGetSettings.c \
                 swioma/src/qaSwiOmaDmSetSettings.c

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
SWIOMADMSOBJ   = $(OBJSDIR)/qaGobiApiSwiOmadms.o \
                 $(OBJSDIR)/qaSwiOmaDmSessionStart.o \
                 $(OBJSDIR)/qaSwiOmaDmSessionCancel.o \
                 $(OBJSDIR)/qaSwiOmaDmSessionGetInfo.o \
                 $(OBJSDIR)/qaSwiOmaDmSelection.o \
                 $(OBJSDIR)/qaSwiOmaDmGetSettings.o \
                 $(OBJSDIR)/qaSwiOmaDmSetSettings.o

