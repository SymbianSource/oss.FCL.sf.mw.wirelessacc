#
# Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:  Scalable icons makefile for project WlanInfoSorting
#


ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32/release/$(PLATFORM)/$(CFG)/z
else
ZDIR=$(EPOCROOT)epoc32/data/z
endif

TARGETDIR=$(ZDIR)/resource/apps
ICONTARGETFILENAME=$(TARGETDIR)/wsfwlaninfosorting.mif

HEADERDIR=$(EPOCROOT)epoc32/include
HEADERFILENAME=$(HEADERDIR)/wsfwlaninfosorting.mbg 
	 
do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing


# Add your branding icons here like:
#
# RESOURCE :
#	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
# 		/c8,1 /path/to/graphics/hugeoperator.svg \
#       /c8,1 /path/to/graphics/hugeoperator2.svg
#
# and use the enums that result in $(HEADERFILENAME) when specifying
# the custfav networks in the resource.
#
# If there are no customer favourite networks defined, you must still invoke 
# mifconv the following way:
#
# RESOURCE: 
#   mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
#       /c8,1 qgn_prop_wlan_bearer.svg
#
# In case any of the defined networks doesn't need a specific icon, you must
# use the following invocation:
#
# RESOURCE: 
#   mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
#       /c8,1 qgn_prop_wlan_bearer.svg                \
#       /c8,1 /path/to/hugeoperator.svg               \
#       /c8,1 /path/to/yetanothernetworkicon.svg
#
# and use the enums that result in $(HEADERFILENAME) when specifying
# the custfav networks in the resource. For the networks without icon, use
# the enums corresponding to qgn_prop_wlan_bearer.svg.
#

RESOURCE : 
	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
        /c8,1 qgn_prop_wlan_bearer.svg


FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing


