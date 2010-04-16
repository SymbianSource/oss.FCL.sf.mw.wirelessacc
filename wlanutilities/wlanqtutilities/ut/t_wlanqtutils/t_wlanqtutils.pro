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

QT += testlib

TEMPLATE = app
TARGET = t_wlanqtutils

TARGET.CAPABILITY = ALL -TCB

DEPENDPATH += .

#Store generated .moc files to their own directory
MOC_DIR = moc

#BUILD_DLL macro is used to define export macro
DEFINES += BUILD_WLANQTUTILITIES_DLL

#Following macros MW_LAYER_SYSTEMINCLUDE and OS_LAYER_SYSTEMINCLUDE are defined 
#in X:\QT\mkspecs\features\symbian\platform_paths.prf that is always inluded in
#QT compilation

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
               $$OS_LAYER_SYSTEMINCLUDE \
               . \
               ../../base/inc \
               ../../wrapper/inc \
               ../../traces \
               /epoc32/include/domain/osextensions

# Input
HEADERS += testwlanqtutils.h \
    ../context/wlanqtutilstestcontext.h \
    ../../base/inc/wlanqtutils.h \
    ../../base/inc/wlanqtutils_p.h \
    ../../base/inc/wlanqtutilsiap.h \
    ../../base/inc/wlanqtutilswlaniap.h \
    ../../base/inc/wlanqtutilswlanap.h \
    ../../base/inc/wlanqtutilsactiveconn.h \
    ../../wrapper/inc/wlanqtutilscmmwrapper.h \
    ../../wrapper/inc/wlanqtutilsconmonwrapper.h \
    ../../wrapper/inc/wlanqtutilsconntestwrapper.h \
    ../../wrapper/inc/wlanqtutilsesockwrapper.h \
    ../../traces/OstTraceDefinitions.h

SOURCES += testwlanqtutils.cpp \
    ../context/wlanqtutilstestcontext.cpp \
    ../../base/src/wlanqtutils.cpp \
    ../../base/src/wlanqtutils_p.cpp \
    ../../base/src/wlanqtutilsiap.cpp \
    ../../base/src/wlanqtutilswlaniap.cpp \
    ../../base/src/wlanqtutilswlanap.cpp \
    ../../base/src/wlanqtutilsactiveconn.cpp \
    ../../wrapper/src/wlanqtutilscmmwrapper.cpp \
    ../../wrapper/src/wlanqtutilsconmonwrapper.cpp \
    ../../wrapper/src/wlanqtutilsconntestwrapper.cpp \
    ../../wrapper/src/wlanqtutilsesockwrapper.cpp
    
symbian: { 
    HEADERS += ../../wrapper/inc/wlanqtutilscmmwrapper_s60_p.h \
               ../../wrapper/inc/wlanqtutilsconmonwrapper_s60_p.h \
               ../../wrapper/inc/wlanqtutilsconntestwrapper_s60_p.h \
               ../../wrapper/inc/wlanqtutilsesockwrapper_s60_p.h

    SOURCES += ../../wrapper/src/wlanqtutilscmmwrapper_s60.cpp \
               ../../wrapper/src/wlanqtutilsconmonwrapper_s60.cpp \
               ../../wrapper/src/wlanqtutilsconntestwrapper_s60.cpp \
               ../../wrapper/src/wlanqtutilsesockwrapper_s60.cpp \
               ../stubs/wlanqtutilsconnmonstub.cpp \
               ../stubs/wlanqtutilsconnteststub.cpp \
               ../stubs/wlanqtutilsesockstub.cpp
}

LIBS += -lcmmanager -lconnmon -lcommdb -lesock -lictsclientinterface

#UT flag is set
DEFINES += WLANQTUTILS_T_WLANQTUTILS_UT_FLAG

coverage_test: {
message(*** Coverage test mode build - no OST traces.)
#NO_OST_TRACES flag is set
DEFINES += WLANQTUTILS_NO_OST_TRACES_FLAG 'Q_ASSERT(test)='
}
else {
message(*** Normal Symbian-mode build.)
}