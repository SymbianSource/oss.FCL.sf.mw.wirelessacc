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
TARGET = wlanqtutilities
TARGET.CAPABILITY = ALL -TCB

#BUILD_DLL macro is used to define export macro
DEFINES += BUILD_WLANQTUTILITIES_DLL
DEPENDPATH += .

#Store generated .moc files to their own directory
MOC_DIR = moc

#Following macros MW_LAYER_SYSTEMINCLUDE and OS_LAYER_SYSTEMINCLUDE are defined 
#in X:\QT\mkspecs\features\symbian\platform_paths.prf that is always inluded in
#QT compilation

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
               $$OS_LAYER_SYSTEMINCLUDE \
               base/inc \
               wrapper/inc \
               traces

HEADERS += base/inc/wlanqtutils.h \
           base/inc/wlanqtutils_p.h \
           base/inc/wlanqtutilsactiveconn.h \
           base/inc/wlanqtutilscommon.h \
           base/inc/wlanqtutilswlanap.h \
           base/inc/wlanqtutilswlaniap.h \
           base/inc/wlanqtutilsiap.h \
           wrapper/inc/wlanqtutilsconmonwrapper.h \
           wrapper/inc/wlanqtutilscmmwrapper.h \
           wrapper/inc/wlanqtutilsconntestwrapper.h \
           wrapper/inc/wlanqtutilsesockwrapper.h \
           traces/OstTraceDefinitions.h

SOURCES += base/src/wlanqtutils.cpp \ 
           base/src/wlanqtutils_p.cpp \
           base/src/wlanqtutilsactiveconn.cpp \
           base/src/wlanqtutilswlanap.cpp \
           base/src/wlanqtutilswlaniap.cpp \
           base/src/wlanqtutilsiap.cpp \
           wrapper/src/wlanqtutilsconmonwrapper.cpp \
           wrapper/src/wlanqtutilscmmwrapper.cpp \
           wrapper/src/wlanqtutilsconntestwrapper.cpp \
           wrapper/src/wlanqtutilsesockwrapper.cpp

mmpRuleDeffile = \
 "$${LITERAL_HASH}ifdef WINSCW" \
 "DEFFILE ./bwins/wlanqtutilities.def" \
 "$${LITERAL_HASH}else" \
 "DEFFILE ./eabi/wlanqtutilities.def" \
 "$${LITERAL_HASH}endif"

symbian: { 
    HEADERS += wrapper/inc/wlanqtutilsconmonwrapper_s60_p.h \
               wrapper/inc/wlanqtutilscmmwrapper_s60_p.h \
               wrapper/inc/wlanqtutilsconntestwrapper_s60_p.h \
               wrapper/inc/wlanqtutilsesockwrapper_s60_p.h
    SOURCES += wrapper/src/wlanqtutilsconmonwrapper_s60.cpp \
               wrapper/src/wlanqtutilscmmwrapper_s60.cpp \
               wrapper/src/wlanqtutilsconntestwrapper_s60.cpp \
               wrapper/src/wlanqtutilsesockwrapper_s60.cpp \
               ut/stubs/wlanqtutilsconnmonstub.cpp \
               ut/stubs/wlanqtutilsesockstub.cpp
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x20029F52
    SYMBIAN_PLATFORMS = WINSCW ARMV5
    MMP_RULES += mmpRuleDeffile
    BLD_INF_RULES.prj_exports += "rom/wlanqtutilities.iby CORE_MW_LAYER_IBY_EXPORT_PATH(wlanqtutilities.iby)"
    BLD_INF_RULES.prj_exports += "base/inc/wlanqtutils.h |../inc/wlanqtutils.h" \
                                 "base/inc/wlanqtutilscommon.h |../inc/wlanqtutilscommon.h" \
                                 "base/inc/wlanqtutilswlanap.h |../inc/wlanqtutilswlanap.h" \
                                 "base/inc/wlanqtutilsiap.h |../inc/wlanqtutilsiap.h" \
                                 "base/inc/wlanqtutilswlaniap.h |../inc/wlanqtutilswlaniap.h"
}

symbian*::LIBS += -lconnmon -lcmmanager -lesock -lcommdb -lictsclientinterface