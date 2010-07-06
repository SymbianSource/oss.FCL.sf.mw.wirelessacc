#
# Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
# Test library for WlanQtUtils class.
#

QT += testlib

TEMPLATE = app
TARGET = t_wlanqtutils

TARGET.CAPABILITY = ALL -TCB

DEPENDPATH += .

#Store generated files to their own directory
MOC_DIR = build

# Temporary solution to fix tracecompiler
# When tracecompiler is fixed, this can be removed
symbian: {
    MMP_RULES += "USERINCLUDE traces"
}

#BUILD_DLL macro is used to define export macro
DEFINES += BUILD_WLANQTUTILITIES_DLL

INCLUDEPATH += \
    . \
    ../../inc \
    stubs \
    ../base/inc \
    ../wrapper/inc \
    ../traces

# Input
HEADERS += \
    testwlanqtutils.h \
    context/wlanqtutilstestcontext.h \
    ../base/inc/wlanqtutils.h \
    ../base/inc/wlanqtutils_p.h \
    ../base/inc/wlanqtutilsiap.h \
    ../base/inc/wlanqtutilsiapsettings.h \
    ../base/inc/wlanqtutilsap.h \
    ../base/inc/wlanqtutilsconnection.h \
    ../wrapper/inc/wlanqtutilsconmonwrapper.h \
    ../wrapper/inc/wlanqtutilsesockwrapper.h \
    ../wrapper/inc/wlanqtutilsscan.h \
    ../traces/OstTraceDefinitions.h

SOURCES += \
    testwlanqtutils.cpp \
    context/wlanqtutilstestcontext.cpp \
    ../base/src/wlanqtutils.cpp \
    ../base/src/wlanqtutils_p.cpp \
    ../base/src/wlanqtutilsiap.cpp \
    ../base/src/wlanqtutilsap.cpp \
    ../base/src/wlanqtutilsconnection.cpp \
    ../base/src/wlanqtutilsiapsettings.cpp \
    ../wrapper/src/wlanqtutilsconmonwrapper.cpp \
    ../wrapper/src/wlanqtutilsesockwrapper.cpp \
    ../wrapper/src/wlanqtutilsscan.cpp
    
symbian: { 
    HEADERS += \
        ../wrapper/inc/wlanqtutilsconmonwrapperdisconnect_symbian.h \
        ../wrapper/inc/wlanqtutilsconmonwrapperinfo_symbian.h \
        ../wrapper/inc/wlanqtutilsesockwrapper_symbian.h \
        ../wrapper/inc/wlanqtutilsscan_symbian.h \
        stubs/wlanmgmtclient.h \
        stubs/wlanscaninfo.h

    SOURCES += \
        ../wrapper/src/wlanqtutilsconmonwrapperdisconnect_symbian.cpp \
        ../wrapper/src/wlanqtutilsconmonwrapperinfo_symbian.cpp \
        ../wrapper/src/wlanqtutilsesockwrapper_symbian.cpp \
        ../wrapper/src/wlanqtutilsscan_symbian.cpp \
        stubs/stub_connmon.cpp \
        stubs/stub_esock.cpp \
        stubs/stub_wlanmgmtclient.cpp \
        stubs/stub_wlanscaninfo.cpp
}

LIBS += -lconnmon -lconnection_settings_shim -lextendedconnpref -lnetmeta -lesock -lictswlanlogininterface
