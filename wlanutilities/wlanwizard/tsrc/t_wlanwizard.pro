#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
# WLAN Wizard unit testing Qt project file.
#

include(stubs/stubs.pri)
include(ut/ut.pri)

QT         += testlib
CONFIG     += hb qtestlib

TEMPLATE = app
TARGET = t_wlanwizard

DEPENDPATH += .
MOC_DIR    = build
RCC_DIR    = build

VERSION = 0.0.1

DEFINES += BUILD_WLAN_WIZARD_DLL
DEFINES += BUILD_EAP_WIZARD_DLL
DEFINES += BUILD_WPS_WIZARD_DLL

RESOURCES += ../resources/resource.qrc
RESOURCES += ../../eapwizard/resources/eapwizard_resource.qrc
RESOURCES += ../../wpswizard/resources/wpswizard_resource.qrc

INCLUDEPATH += \
    stubs

# Stub headers must be used instead of real ones
symbian {
	MMP_RULES += "USERINCLUDE stubs"
}

# Temporary solution to fix tracecompiler
# When tracecompiler is fixed, these can be removed
symbian: {
    MMP_RULES += \
        "USERINCLUDE ../traces" \
        "USERINCLUDE ../../eapwizard/traces" \
        "USERINCLUDE ../../wpswizard/traces"
}

# WLAN WIZARD HEADERS
HEADERS   += \
    ../inc/wlanwizard.h \
    ../inc/wlanwizard_p.h \
    ../inc/wlanwizardscanlist.h \
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
SOURCES   += \
    ../src/wlanwizard.cpp \
    ../src/wlanwizard_p.cpp \
    ../src/wlanwizardscanlist.cpp \
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
  
# EAP Wizard headers
HEADERS += \
    ../../eapwizard/inc/eapwizard.h \
    ../../eapwizard/inc/eapwizard_p.h \
    ../../eapwizard/inc/eapwizardpage.h \
    ../../eapwizard/inc/eapwizardpageoutertype.h \
    ../../eapwizard/inc/eapwizardpagecertca.h \
    ../../eapwizard/inc/eapwizardpagecertuser.h \
    ../../eapwizard/inc/eapwizardpageidentity.h \
    ../../eapwizard/inc/eapwizardpageinnertype.h \
    ../../eapwizard/inc/eapwizardpageusernamepassword.h \
    ../../eapwizard/inc/eapwizardpagenewpacstore.h \
    ../../eapwizard/inc/eapwizardpagepacstorepasswordconfirm.h

# EAP Wizard sources
SOURCES += \
    ../../eapwizard/src/eapwizard.cpp \
    ../../eapwizard/src/eapwizard_p.cpp \ 
    ../../eapwizard/src/eapwizardpage.cpp \
    ../../eapwizard/src/eapwizardpageoutertype.cpp \
    ../../eapwizard/src/eapwizardpagecertca.cpp \
    ../../eapwizard/src/eapwizardpagecertuser.cpp \
    ../../eapwizard/src/eapwizardpageidentity.cpp \
    ../../eapwizard/src/eapwizardpageinnertype.cpp \
    ../../eapwizard/src/eapwizardpageusernamepassword.cpp \
    ../../eapwizard/src/eapwizardpagenewpacstore.cpp \
    ../../eapwizard/src/eapwizardpagepacstorepasswordconfirm.cpp
    
# WPS Wizard sources
HEADERS += \
    ../../wpswizard/inc/wpswizard.h \
    ../../wpswizard/inc/wpswizard_p.h \
    ../../wpswizard/inc/wpswizardpage.h \
    ../../wpswizard/inc/wpswizardsteptwo.h \
    ../../wpswizard/inc/wpswizardstepthreebutton.h \
    ../../wpswizard/inc/wpswizardstepthreenumber.h \
    ../../wpswizard/inc/wpswizardstepfour.h \
    ../../wpswizard/inc/wpswizardstepfour_p.h \
    ../../wpswizard/inc/wpswizardstepfive.h \
    ../../wpswizard/inc/cwpsactiverunner.h \
    ../../wpswizard/inc/mwpsactiverunnercallback.h \
    
SOURCES += \ 
    ../../wpswizard/src/wpswizard.cpp \
    ../../wpswizard/src/wpswizard_p.cpp \    
    ../../wpswizard/src/wpswizardpage.cpp \
    ../../wpswizard/src/wpswizardsteptwo.cpp \
    ../../wpswizard/src/wpswizardstepthreebutton.cpp \
    ../../wpswizard/src/wpswizardstepthreenumber.cpp \
    ../../wpswizard/src/wpswizardstepfour.cpp \
    ../../wpswizard/src/wpswizardstepfour_p.cpp \
    ../../wpswizard/src/wpswizardstepfive.cpp \
    ../../wpswizard/src/cwpsactiverunner.cpp
    
LIBS      += -leapqtplugininfo
    
symbian: {
    TARGET.CAPABILITY = ALL -TCB 
    TARGET.EPOCHEAPSIZE = 0x20000 0x2000000
}
