# slqscompile.mak
#-------------------------------------------------------------------------------
# Global Settings
#-------------------------------------------------------------------------------
.EXPORT_ALL_VARIABLES:
ifndef QMI_BUILD_BASE
  QMI_BUILD_BASE:=$(shell until [ -d pkgs ]; do cd ..; done; pwd)
endif
SLQSHOSTTYPE := $(shell uname -m)
CFLAGS       := -c -Wall -Werror -Wextra
INCLUDE      := -I.. $(QMI_INCLUDE)
LDFLAGS      := -static -lrt -lpthread
DEPSDIR      := ./dep
INSTALLDIR   :=
#
SLQSFLAG=-D SLQS
SYMBOLS := ON
#-------------------------------------------------------------------------------
# DEBUG ON means the compiler should build for possible GDB debugging sessions
#
# Default is for no debugging which considerably reduces the size of the final
# image, especially when preparing a release for distribution
#
ifeq ($(SYMBOLS),ON)
  DBGFLAGS := -g
  CFLAGS += $(DBGFLAGS)
endif

#-------------------------------------------------------------------------------
# if CPU is not specified, default to native host CPU: PC, PowerBook or
# Pandaboard, otherwise it has to be one of arm, ppc, mips, mipsel
#-------------------------------------------------------------------------------
ifndef CPU
  CPU:=host$(SLQSHOSTTYPE)
  UPPER_CPU:=$(CPU) # no need for uppercase here, 'host' to differentiate
#                         from conflicting cross-compile CPU defines
  CROSS_COMPILE :=
  SDK_EXE:=$(QMI_BUILD_BASE)/build/bin/$(CPU)/slqssdk
else
  UPPER_CPU:=$(shell echo $(CPU) | tr [a-z] [A-Z])
  SDK_EXE:=./slqssdk
endif
CFLAGS += -D $(UPPER_CPU) -DSDK_EXE='"$(SDK_EXE)"'

#-------------------------------------------------------------------------------
# For arm, powerpc and MIPs, platform specific toolchains are used.
# These must be installed to the $(HOME) directory once and can be used for all
# SDK workspaces. If they do not exist,
#   make CPU=arm
#   make CPU=ppc
#   make CPU=mips
#   make CPU=mipsel
# will simply fail.
#-------------------------------------------------------------------------------
ifeq ($(CPU),arm)
  CROSS_COMPILE := $(HOME)/toolchain/arm/bin/arm-none-linux-gnueabi-
  INCLUDE       += -I$(HOME)/toolchain/arm/arm-none-linux-gnueabi/libc/usr/include
  LDFLAGS       += -L$(HOME)/toolchain/arm/lib/gcc/arm-none-linux-gnueabi/4.2.0/
  CFLAGS        += -march=armv5te
endif
# this is for backward compatibility
ifeq ($(CPU),arm9)
  CROSS_COMPILE := $(HOME)/toolchain/arm/bin/arm-none-linux-gnueabi-
  INCLUDE       += -I$(HOME)/toolchain/arm/arm-none-linux-gnueabi/libc/usr/include
  LDFLAGS       += -L$(HOME)/toolchain/arm/lib/gcc/arm-none-linux-gnueabi/4.2.0/
  CFLAGS        += -march=armv5te
endif
#
# If LTIB (Linux Target Image Build) is installed, use:
#  /opt/freescale/usr/local/gcc-4.3.74-eglibc-2.8.74-dp-2/powerpc-none-linux-gnuspe/
# otherwise if toolchain is available, use :
#  $(HOME)/toolchain/powerpc-none-linux-gnuspe
#
ifeq ($(CPU),ppc)
  CROSS_COMPILE := $(HOME)/toolchain/ppc/powerpc-none-linux-gnuspe/bin/powerpc-none-linux-gnuspe-
  INCLUDE       += -I$(HOME)/toolchain/ppc/powerpc-none-linux-gnuspe/powerpc-none-linux-gnuspe/libc/usr/include
  LDFLAGS       += -L$(HOME)/toolchain/ppc/powerpc-none-linux-gnuspe/powerpc-none-linux-gnuspe/libc/usr/lib
endif
#
# Download from:
# http://www.denx.de/wiki/view/ELDK-5/WebHome#Section_1.6.
#
ifeq ($(CPU),ppc_4xx)
  CROSS_COMPILE := $(HOME)/toolchain/ELDK/usr/bin/ppc_4xx-
  INCLUDE       := -I.. $(QMI_INCLUDE) -I$(HOME)/toolchain/ELDK/usr/lib/gcc/powerpc-linux/4.2.2/include
  LDFLAGS       := -L$(HOME)/toolchain/ELDK/usr/lib -static -lrt -lpthread
endif

#
# Downloaded from:
# http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/lite-edition
# Note: the original file is an installer file.
#       This is an unzipped tarballed copy of the installed files.
# Note: uclibc is used here. Some applications may need libc and is overridden
#       in the make file for those.
#
ifeq ($(CPU),mips)
  CROSS_COMPILE := $(HOME)/toolchain/mips/bin/mips-linux-gnu-
  INCLUDE       += -I$(HOME)/toolchain/mips/mips-linux-gnu/libc/ulibc/usr/include
  LDFLAGS       += -L$(HOME)/toolchain/mips/mips-linux-gnu/libc/uclibc/lib -muclibc
endif

ifeq ($(CPU),mipsel)
  CROSS_COMPILE := $(HOME)/toolchain/mips/bin/mips-linux-gnu-
  INCLUDE       += -I$(HOME)/toolchain/mips/mips-linux-gnu/libc/ulibc/usr/include
  LDFLAGS       += -L$(HOME)/toolchain/mips/mips-linux-gnu/libc/uclibc/lib -muclibc -EL
  CFLAGS        += -EL
endif

# $(CROSS_COMPILE) is empty if not defined
CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
RANLIB := $(CROSS_COMPILE)ranlib
#

