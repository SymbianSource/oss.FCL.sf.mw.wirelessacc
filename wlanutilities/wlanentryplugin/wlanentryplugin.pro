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

TEMPLATE = lib
TARGET = cpwlanentryplugin
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += traces
# wlanutilities Private API
INCLUDEPATH += ../inc
CONFIG += hb plugin
MOC_DIR = moc
OBJECTS_DIR = obj
RCC_DIR = rcc
LIBS += -lwlanqtutilities -lcpframework -lxqservice -lqtsysteminfo

# Sources
HEADERS += \
    inc/cpwlanentryplugin.h \
    inc/cpwlanentryitemdata.h \
    inc/wlanstatusinfo.h \
    traces/OstTraceDefinitions.h
SOURCES += \
    src/cpwlanentryplugin.cpp \
    src/cpwlanentryitemdata.cpp \
    src/wlanstatusinfo.cpp

TRANSLATIONS = wlanentryplugin.ts

symbian: { 
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x2002BCE0
    SYMBIAN_PLATFORMS = WINSCW ARMV5
    BLD_INF_RULES.prj_exports += "rom/wlanentryplugin.iby CORE_MW_LAYER_IBY_EXPORT_PATH(wlanentryplugin.iby)"
    BLD_INF_RULES.prj_exports += "rom/wlanentryplugin_resources.iby LANGUAGE_APP_LAYER_IBY_EXPORT_PATH(wlanentryplugin_resources.iby)"
    
    # Enable for QtHighway tracing
    # DEFINES += WLANENTRYPLUGIN_SERVICETRACES
}
symbian { 
    deploy.path = C:
    qtplugins.path = /resource/qt/plugins/controlpanel
    qtplugins.sources += qmakepluginstubs/cpwlanentryplugin.qtplugin
    
    # This is for new exporting system coming in garden
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin $$deploy.path$$qtplugins.path/$$basename(qtplugin)"
}
TARGET.CAPABILITY = CAP_GENERAL_DLL
plugin.sources = cpwlanentryplugin.dll xqservice.dll
plugin.path = /resource/qt/plugins/controlpanel
DEPLOYMENT += plugin
