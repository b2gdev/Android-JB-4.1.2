#-------------------------------------------------------------------------------
#
#  Name:
#    omadm.mak
#
#  Description:
#    Makefile to build the pkgs/qa/omadm package
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
SRCS_OMADM =     omadm/src/qaGobiApiOmadm.c \
                 omadm/src/qaOmaDmStartSession.c \
                 omadm/src/qaOmaDmCancelSession.c \
                 omadm/src/qaOmaDmGetSessionInfo.c

#-------------------------------------------------------------------------------
# Split the object files into their respective groups
#-------------------------------------------------------------------------------
OMADMOBJ   =     $(OBJSDIR)/qaGobiApiOmadm.o \
                 $(OBJSDIR)/qaOmaDmStartSession.o \
                 $(OBJSDIR)/qaOmaDmCancelSession.o \
                 $(OBJSDIR)/qaOmaDmGetSessionInfo.o

