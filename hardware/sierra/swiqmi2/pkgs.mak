#-------------------------------------------------------------------------------
#
#  Name:
#    pkgs.mak
#
#  Description:
#    Top level Makefile to build the pkgs
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)and test program
#                     where available
#     clean         - delete object directory and image files
#     $(PKG)        - builds the selected package
#
#   The above targets can be made with the following command:
#
#     make -f pkgs.mak [CPU={i386, arm9}] [target] [TECHNOLOGY={ALL,CDMA,UMTS}]
#       defaults to i386 if CPU is omitted
#       defaults to UMTS if TECHNOLOGY is omitted
#
#    make -f pkgs.mak build
#       copies binaries, libraries and include files into area for Applications
#
# Copyright (c) 2011-2012 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------
SHELL := /bin/bash
DOXY = /usr/bin/doxygen
SLQSHOSTTYPE := $(shell uname -m)
#
ifndef QMI_BUILD_BASE
export QMI_BUILD_BASE:=$(shell until [ -d pkgs ]; do cd ..; done; pwd)
endif

DISTDIR := $(QMI_BUILD_BASE)/build
API_HDRS_SRC_FOLDER = $(QMI_BUILD_BASE)/pkgs/qa
API_HDRS_DEST_FOLDER = $(QMI_BUILD_BASE)/api
SAPPSDIR = $(QMI_BUILD_BASE)/SampleApps

# Do not export the CPU variable defined in this make file as it will
# interfere with the CPU variable define in slqsrelease.mak,
# which is included in other make files.
ifndef CPU
  CPU:=host$(SLQSHOSTTYPE)
else
  UPPER_CPU:=$(shell echo $(CPU) | tr [a-z] [A-Z])
endif

OBJSDIR := $(DISTDIR)/obj/$(CPU)/pkgs/$(PKG)

include pkgs.trg

#-------------------------------------------------------------------------------
# Default Target
#-------------------------------------------------------------------------------
all:
	@echo
	@echo  "#######################################"
	@echo  "###        BUILDING ALL TARGETS     ###"
	@echo  "#######################################"
	@echo
# sleep is required for dependency to work since build
# happens so fast, the time compare fails
	set -e; for i in $(SUBDIRS) ; do \
        ( $(MAKE) -C $$i -f $$i.mak ; \
#        echo "result: $$?" ; \
		if [ "$$?" -ne 0 ]; then \
		    echo "Build $$i failed"; \
		    exit 2; \
		fi; sleep 1; ) \
	done
#
# Always copy slqssdk to DISTDIR so that qatest and Sample apps can startup SDK
# Leave the sdk/$(CPU) for the developers.
#
	@if [ -d $(DISTDIR)/bin/$(CPU) ] ; then echo ".$(DISTDIR)/bin/$(CPU) exists..." ;\
	else mkdir -p $(DISTDIR)/bin/$(CPU);\
	fi;
	@cp ./sdk/$(CPU)/slqssdk $(DISTDIR)/bin/$(CPU)/slqssdk
	@echo "sdk executable slqssdk copied to $(DISTDIR)/bin/$(CPU)/slqssdk"
#
# - Create API folders containing header files necessary for client application
#   compilation.
#
	@echo  "###  Creating API folder ###"
	@if [ -e $(API_HDRS_DEST_FOLDER) ]; then rm -r $(API_HDRS_DEST_FOLDER); fi;
	( mkdir -p $(API_HDRS_DEST_FOLDER);\
    cd $(API_HDRS_SRC_FOLDER);\
    cp -r --parents ???/inc/qaGobiApi*.h $(API_HDRS_DEST_FOLDER);\
    cp -r --parents swi???/inc/qaGobiApi*.h $(API_HDRS_DEST_FOLDER);\
    cp -r --parents oma??/inc/qaGobiApi*.h $(API_HDRS_DEST_FOLDER);\
    cp -r --parents voice/inc/qaGobiApi*.h $(API_HDRS_DEST_FOLDER);\
    cp SWIWWANCMAPI.h $(API_HDRS_DEST_FOLDER);\
    cp -r --parents inc/SwiDataTypes.h $(API_HDRS_DEST_FOLDER);\
    cp ../qm/qmerrno.h $(API_HDRS_DEST_FOLDER)   )

#-------------------------------------------------------------------------------
# Build tools
#-------------------------------------------------------------------------------
.PHONY: tools
tools:
	@if [ -d "../tools/logging/dm/" ] ; \
        then echo "### Building DM logging tool ####" ;\
        pushd ../tools/logging/dm; \
        $(MAKE) -s -f Makefile ;\
        rm -r ./obj/*/*.o ;\
        popd ;\
	fi
	@if [ -d "../tools/logging/ramdump/" ] ; \
        then echo "### Building ramdump tool ####" ;\
        pushd ../tools/logging/ramdump; \
        $(MAKE) -s ;\
        rm -r ./obj/*/*.o ;\
        popd ;\
	fi
#
#-------------------------------------------------------------------------------
# Manually build everything. Similar to all the following steps.
# - make -f pkgs.mak clean
# - make -f pkgs.mak
#-------------------------------------------------------------------------------
.PHONY: complete
complete:
	$(MAKE) -f pkgs.mak clean;
	$(MAKE) -f pkgs.mak;
#
#-------------------------------------------------------------------------------
# Manually generate Doxygen files
#-------------------------------------------------------------------------------
.PHONY: doxygen
doxygen:
#
# if usr/bin/doxygen does not exist, bail out
#
	@if [ -e $(DOXY) ] ;\
        then echo "$(DOXY) exists - good." ;\
        else echo  ;\
             echo ">>>>>ERROR!!! $(DOXY) does not exist, please install first.";\
             exit 2;\
        fi;
	( cd qa; doxygen ../../tools/scripts/Doxyfile; cd ..;)

.PHONY: sapps
sapps:
	@echo "--------------------------"
	@echo "-- Building Sample Apps --"
	@echo "--------------------------"
	@pushd $(SAPPSDIR)/Gobi_Image_Management; make; popd;
	@pushd $(SAPPSDIR)/MC77xx_Image_Management; make; popd;
	@pushd $(SAPPSDIR)/Connection_Manager; make; popd;
	@pushd $(SAPPSDIR)/SMS_Application; make; popd;
	@pushd $(SAPPSDIR)/SLQS_Tutorial_Application; make; popd;
	@pushd $(SAPPSDIR)/CallHandling_Application; make; popd;

info:
	echo $(CPU) $(MYHOSTTYPE)

#-------------------------------------------------------------------------------
# Clean up top level if run at the pkgs level.
#-------------------------------------------------------------------------------
.PHONY: clean
CURRENT_DIR = $(notdir $(PWD))
clean:
	@rm -f *~
ifeq ($(CURRENT_DIR),pkgs)
	@echo "Deleting top level build directory..."
	rm -fr $(OBJSDIR)
	rm -fr $(DISTDIR)/bin/$(CPU)
	rm -fr $(DISTDIR)/lib/$(CPU)
endif
