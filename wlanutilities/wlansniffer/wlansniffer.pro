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

TEMPLATE = app
TARGET = wlansniffer
TARGET.CAPABILITY = ALL -TCB
DEPENDPATH += .
# Own headers
INCLUDEPATH += inc \
               traces
# wlanutilities Private API
INCLUDEPATH += ../inc
CONFIG += hb no_icon service

#Store generated .moc files to their own directory
MOC_DIR = moc

TRANSLATIONS = wlansniffer.ts

# Input
SOURCES += src/main.cpp \
    src/wlansniffer.cpp \
    src/wlansnifferlistview.cpp \
    src/wlansniffermainwindow.cpp \
    src/wlansnifferservice.cpp

HEADERS += inc/wlansniffer.h \
    inc/wlansnifferlistview.h \
    inc/wlansniffermainwindow.h \
    inc/wlansnifferservice.h \
    traces/OstTraceDefinitions.h

RESOURCES = res/wlansniffer.qrc

SERVICE.FILE = res/service_conf.xml
SERVICE.OPTIONS = embeddable

symbian*::LIBS += -lwlanqtutilities -lxqservice -lxqserviceutil -lqtsysteminfo

symbian*: {
    TARGET.UID3 = 0x10281CAA
    SYMBIAN_PLATFORMS = WINSCW ARMV5
    BLD_INF_RULES.prj_exports += "rom/wlansniffer.iby CORE_MW_LAYER_IBY_EXPORT_PATH(wlansniffer.iby)"
    BLD_INF_RULES.prj_exports += "rom/wlansniffer_resources.iby LANGUAGE_APP_LAYER_IBY_EXPORT_PATH(wlansniffer_resources.iby)"
    addFiles.sources = /epoc32/release/$(PLATFORM)/$(CFG)/wlanqtutilities.dll xqservice.dll xqserviceutil.dll
    addFiles.path = /sys/bin
    DEPLOYMENT += addFiles
    # Enable for QtHighway tracing
    # DEFINES += WLANSNIFFER_SERVICETRACES
} 
