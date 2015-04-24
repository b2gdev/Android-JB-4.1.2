#-------------------------------------------------------------------------------
#
#  Name:
#    pkgs.trg
#
#  Description:
#    Define packages here for inclusion into pkgs.mak
#
# Copyright (c) 2008 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------
#
# Variable definition - Packages, Ordered Alphabetically
AMDIR = am
DLDIR = dl
DSDIR = ds
ERDIR = er
HDDIR = hd
ICDIR = ic
IMDIR = im
MMDIR = mm
OSDIR = os
PIDIR = pi
QADIR = qa
QLDIR = ql
QMDIR = qm
SDKDIR = sdk
SLDIR = sl
SMDIR = sm
SUDIR = su
USDIR = us
CIDIR = ci
IMDIR = im

# Order is important in the following line
SUBDIRS =  $(ERDIR) $(AMDIR) $(OSDIR) $(MMDIR) $(SLDIR) $(QLDIR) $(PIDIR) $(DLDIR) $(HDDIR) $(ICDIR) $(USDIR) $(CIDIR)  $(SMDIR) $(IMDIR) $(QMDIR) $(DSDIR) $(SUDIR) $(SDKDIR) $(QADIR)
