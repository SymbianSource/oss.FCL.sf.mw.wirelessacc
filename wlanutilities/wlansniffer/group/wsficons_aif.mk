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
# Description:  scalable icons makefile for project WlanSniffer
#


ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif


TARGETDIR=$(ZDIR)\RESOURCE\APPS
ICONTARGETFILENAME=$(TARGETDIR)\wsficons_aif.mif

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE :
	mifconv $(ICONTARGETFILENAME) \
		/c8,1 qgn_menu_wlan_sniffer.svg

FREEZE : do_nothing
 
SAVESPACE : do_nothing 

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
 
