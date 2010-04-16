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
# Description: 
#

TEMPLATE = lib
TARGET = wpswizard
DEFINES += BUILD_WPS_WIZARD_DLL
VERSION = 0.0.1
CONFIG += hb
MOC_DIR = _moc
RCC_DIR = _resources
RESOURCES += resources/resource.qrc
TRANSLATIONS += resources/wpswizardplugin.ts
HEADERS += ../inc/wpswizard.h \
    inc/wpswizard_p.h \
    inc/wpswizardpage.h \
    inc/wpswizardsteptwo.h \
    inc/wpswizardstepthreebutton.h \
    inc/wpswizardstepthreenumber.h \
    inc/wpswizardstepfour.h \
    inc/wpswizardstepfive.h \
    inc/cwpsactiverunner.h \
    inc/mwpsactiverunnercallback.h \
    traces/OstTraceDefinitions.h
SOURCES += src/wpswizardprivate.cpp \
    src/wpswizard.cpp \
    src/wpswizardpage.cpp \
    src/wpswizardsteptwo.cpp \
    src/wpswizardstepthreebutton.cpp \
    src/wpswizardstepthreenumber.cpp \
    src/wpswizardstepfour.cpp \
    src/wpswizardstepfive.cpp \
    src/cwpsactiverunner.cpp
LIBS += -lecom
symbian { 
    HEADERS += 
    SOURCES += 
    INCLUDEPATH += inc \
        ../../inc \
        ../wlanwizard/inc \
        $$MW_LAYER_SYSTEMINCLUDE \
        $$OS_LAYER_SYSTEMINCLUDE
    TARGET.UID3 = 0x2003D2FE
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    BLD_INF_RULES.prj_exports += "rom/wpswizard.iby CORE_MW_LAYER_IBY_EXPORT_PATH(wpswizard.iby)"
    TARGET.EPOCALLOWDLLDATA = 1
    MMP_RULES += EXPORTUNFROZEN
}
