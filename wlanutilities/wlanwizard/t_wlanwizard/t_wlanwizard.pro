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
# Description:
#

include(stubs/stubs.pri)
include(ut/ut.pri)

QT 		+= testlib
CONFIG 	+= hb qtestlib

TEMPLATE = app
TARGET = t_wlanwizard

DEPENDPATH += .
MOC_DIR    = _moc
RCC_DIR    = _resources

VERSION = 0.0.1

DEFINES += BUILD_WLAN_WIZARD_DLL
DEFINES += WLAN_WIZARD_RND

RESOURCES += ../resources/resource.qrc

INCLUDEPATH += \
		stubs \
		$$MW_LAYER_SYSTEMINCLUDE \
		$$OS_LAYER_SYSTEMINCLUDE

# OST trace system requires traces directory to be in USERINCLUDES
MMP_RULES += "USERINCLUDE ../traces"

# WLAN WIZARD HEADERS
HEADERS   += \
             ../inc/wlanwizard.h \
             ../inc/wlanwizard_p.h \
             ../inc/wlanwizardpage.h \
             ../inc/wlanwizardhelper.h \
             ../inc/wlanwizardutils.h \
             ../inc/wlanwizardpageinternal.h \
             ../inc/wlanwizardpagekeyquery.h \
             ../inc/wlanwizardpageprocessingsettings.h \
             ../inc/wlanwizardpagesummary.h \
             ../inc/wlanwizardsummaryviewitem.h \
             ../inc/wlanwizardpagegenericerror.h \
             ../inc/wlanwizardpagenetworkmode.h \
             ../inc/wlanwizardpagescanning.h \
             ../inc/wlanwizardpagesecuritymode.h \
             ../inc/wlanwizardpagessid.h 

# WLAN WIZARD SOURCES             
SOURCES   += ../src/wlanwizard.cpp \
             ../src/wlanwizard_p.cpp \
             ../src/wlanwizardutils.cpp \
             ../src/wlanwizardpageinternal.cpp \
             ../src/wlanwizardpagekeyquery.cpp \
             ../src/wlanwizardpageprocessingsettings.cpp \
             ../src/wlanwizardpagesummary.cpp \
             ../src/wlanwizardsummaryviewitem.cpp \
             ../src/wlanwizardpagegenericerror.cpp \
             ../src/wlanwizardpagenetworkmode.cpp \
             ../src/wlanwizardpagescanning.cpp \
             ../src/wlanwizardpagesecuritymode.cpp \
             ../src/wlanwizardpagessid.cpp
    
symbian: {
	TARGET.CAPABILITY = ALL -TCB 
}

ict_result_enum_used: {
	message(*** ict result enumerated instead of boolean.)
	DEFINES+=ICT_RESULT_ENUM
}

coverage_test: {
	message(*** Coverage test mode build - no OST traces.)
	#NO_OST_TRACES flag is set
}
else {
	message(*** Normal Symbian-mode build.)
}