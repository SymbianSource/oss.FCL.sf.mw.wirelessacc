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
ICONTARGETFILENAME=$(TARGETDIR)\wsficons.mif

HEADERDIR=$(EPOCROOT)epoc32\include
HEADERFILENAME=$(HEADERDIR)\wsficons.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE :
	
	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
	 	/c8,1 qgn_indi_wlan_secure_network_add.svg \
	 	/c8,1 empty13x13.svg \
	 	/c8,1 qgn_indi_wlan_signal_low_add.svg \
 		/c8,1 qgn_indi_wlan_signal_med_add.svg \
	 	/c8,1 qgn_indi_wlan_signal_good_add.svg \
	 	/c8,1 qgn_prop_cmon_wlan_conn.svg \
	 	/c8,1 qgn_prop_wlan_bearer.svg \
		/c8,1 qgn_prop_empty.svg \
	 	/c8,1 qgn_indi_wlan_sniffer_plugin_off.svg \
	 	/c8,1 qgn_indi_wlan_sniffer_plugin_on.svg \
	 	/c8,1 qgn_indi_wlan_sniffer_plugin_on_0.svg \
	 	/c8,1 qgn_indi_wlan_sniffer_plugin_on_1.svg \
	  /c8,1 qgn_menu_wlan_sniffer.svg

	

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
