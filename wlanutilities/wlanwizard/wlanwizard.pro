#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
# 
# Description: 
#	WLAN Settings Wizard
#

TEMPLATE = lib
TARGET = wlanwizard

DEFINES += BUILD_WLAN_WIZARD_DLL

VERSION = 0.0.1

CONFIG += hb

MOC_DIR = build
RCC_DIR = build

RESOURCES += resources/resource.qrc

TRANSLATIONS = wlanwizard.ts

HEADERS   += \
             inc/wlanwizard.h \
             inc/wlanwizard_p.h \
             inc/wlanwizardhelper.h \
             inc/wlanwizardpage.h \
             inc/wlanwizardpagegenericerror.h \             
             inc/wlanwizardpageinternal.h \
             inc/wlanwizardpagekeyquery.h \
             inc/wlanwizardpageprocessingsettings.h \
             inc/wlanwizardpagesummary.h \
             inc/wlanwizardsummaryviewitem.h \
             inc/wlanwizardutils.h
             
SOURCES   += src/wlanwizard.cpp \
             src/wlanwizard_p.cpp \
             src/wlanwizardpagegenericerror.cpp \
             src/wlanwizardpageinternal.cpp \
             src/wlanwizardpagekeyquery.cpp \
             src/wlanwizardpageprocessingsettings.cpp \
             src/wlanwizardpagesummary.cpp \
             src/wlanwizardsummaryviewitem.cpp \
             src/wlanwizardutils.cpp             
 
LIBS  += -lwlanqtutilities
 
symbian {
	INCLUDEPATH += \
		inc \
		..\inc \

	TARGET.UID3 = 0x2002C39A
	TARGET.CAPABILITY = CAP_GENERAL_DLL
	TARGET.EPOCALLOWDLLDATA = 1
	
	# Localition of directories for def files
    defFilePath = .
	
	# OST trace system requires traces directory to be in USERINCLUDES
  	MMP_RULES += "USERINCLUDE traces"
  	
  	BLD_INF_RULES.prj_exports += \ 
  		"rom/wlanwizard.iby CORE_MW_LAYER_IBY_EXPORT_PATH(wlanwizard.iby)"
  		
    BLD_INF_RULES.prj_exports += \
        "rom/wlanwizard_resources.iby LANGUAGE_APP_LAYER_IBY_EXPORT_PATH(wlanwizard_resources.iby)"
  	
  	# Export wlanwizard and wlanwizard plugin apis
  	BLD_INF_RULES.prj_exports += \ 
  		"inc/wlanwizard.h |../inc/wlanwizard.h" \
        "inc/wlanwizardhelper.h |../inc/wlanwizardhelper.h" \
        "inc/wlanwizardpage.h |../inc/wlanwizardpage.h" \
        "inc/wlanwizardplugin.h |../inc/wlanwizardplugin.h"
}

# this enables "eap wizard"
wlanwizard_rnd_eap|wlanwizard_rnd_all{
	DEFINES += WLAN_WIZARD_RND_EAP
	LIBS    += -leapwizard
}

# this enables "wps wizard"
wlanwizard_rnd_wps|wlanwizard_rnd_all{
	DEFINES += WLAN_WIZARD_RND_WPS
	LIBS    += -lwpswizard
}

# this enables "add wlan manual" feature
wlanwizard_rnd|wlanwizard_rnd_all|wlanwizard_rnd_eap|wlanwizard_rnd_wps{
	DEFINES += WLAN_WIZARD_RND
	HEADERS += \
             inc/wlanwizardpagenetworkmode.h \
             inc/wlanwizardpagescanning.h \
             inc/wlanwizardpagesecuritymode.h \
             inc/wlanwizardpagessid.h 
             
	SOURCES += \
             src/wlanwizardpagenetworkmode.cpp \
             src/wlanwizardpagescanning.cpp \
             src/wlanwizardpagesecuritymode.cpp \
             src/wlanwizardpagessid.cpp
}


