#-------------------------------------------------------------------------------
#
#  Name:
#    fms.mak
#
#  Description:
#    Makefile to build the pkgs/qa/fms package
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
SRCS_FMS =  fms/src/qaGobiApiFms.c \
            fms/src/qaFmsSetImagePath.c \
            fms/src/qaFmsSetFirmwarePreference.c \
            fms/src/qaFmsGetImagesPreference.c \
            fms/src/qaFmsSetImagesPreference.c \
            fms/src/qaFmsSlqsGetFirmwareInfo.c \
            fms/src/qaFmsSlqsGetImageInfo.c \
            fms/src/qaFmsGetFirmwareInfo.c \
            fms/src/qaFmsGetImageInfo.c \
            fms/src/qaFmsGetStoredImages.c \
            fms/src/qaFmsDeleteStoredImage.c

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
FMSOBJ   =  $(OBJSDIR)/qaGobiApiFms.o \
            $(OBJSDIR)/qaFmsSetImagePath.o \
            $(OBJSDIR)/qaFmsSetFirmwarePreference.o \
            $(OBJSDIR)/qaFmsGetImagesPreference.o \
            $(OBJSDIR)/qaFmsSetImagesPreference.o \
            $(OBJSDIR)/qaFmsSlqsGetFirmwareInfo.o \
            $(OBJSDIR)/qaFmsSlqsGetImageInfo.o \
            $(OBJSDIR)/qaFmsGetFirmwareInfo.o \
            $(OBJSDIR)/qaFmsGetImageInfo.o \
            $(OBJSDIR)/qaFmsGetStoredImages.o \
            $(OBJSDIR)/qaFmsDeleteStoredImage.o

