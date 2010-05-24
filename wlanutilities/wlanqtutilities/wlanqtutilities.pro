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
# WLAN Qt Utilities project file.
#

TEMPLATE = lib
TARGET = wlanqtutilities
TARGET.CAPABILITY = CAP_GENERAL_DLL

#BUILD_DLL macro is used to define export macro
DEFINES += BUILD_WLANQTUTILITIES_DLL
DEPENDPATH += .

#Store generated .moc files to their own directory
MOC_DIR = moc

# Temporary solution to fix tracecompiler
# When tracecompiler is fixed, this can be removed
symbian: {
    MMP_RULES += "USERINCLUDE traces"
}

INCLUDEPATH += \
    base/inc \
    wrapper/inc \
    traces

HEADERS += \
    base/inc/wlanqtutils.h \
    base/inc/wlanqtutils_p.h \
    base/inc/wlanqtutilsap.h \
    base/inc/wlanqtutilsconnection.h \
    base/inc/wlanqtutilsiap.h \
    base/inc/wlanqtutilsiapsettings.h \
    wrapper/inc/wlanqtutilsconmonwrapper.h \
    wrapper/inc/wlanqtutilsconntestwrapper.h \
    wrapper/inc/wlanqtutilsesockwrapper.h \
    traces/OstTraceDefinitions.h

SOURCES += \
    base/src/wlanqtutils.cpp \ 
    base/src/wlanqtutils_p.cpp \
    base/src/wlanqtutilsap.cpp \
    base/src/wlanqtutilsconnection.cpp \
    base/src/wlanqtutilsiap.cpp \
    base/src/wlanqtutilsiapsettings.cpp \
    wrapper/src/wlanqtutilsconmonwrapper.cpp \
    wrapper/src/wlanqtutilsconntestwrapper.cpp \
    wrapper/src/wlanqtutilsesockwrapper.cpp

# Common libraries
LIBS += -lconnection_settings_shim

symbian: { 
    HEADERS += \
        wrapper/inc/wlanqtutilsconmonwrapperdisconnect_s60_p.h \
        wrapper/inc/wlanqtutilsconmonwrapperinfo_s60_p.h \
        wrapper/inc/wlanqtutilsconmonwrapperscan_s60_p.h \
        wrapper/inc/wlanqtutilsconntestwrapper_s60_p.h \
        wrapper/inc/wlanqtutilsesockwrapper_s60_p.h
    SOURCES += \
        wrapper/src/wlanqtutilsconmonwrapperdisconnect_s60.cpp \
        wrapper/src/wlanqtutilsconmonwrapperinfo_s60.cpp \
        wrapper/src/wlanqtutilsconmonwrapperscan_s60.cpp \
        wrapper/src/wlanqtutilsconntestwrapper_s60.cpp \
        wrapper/src/wlanqtutilsesockwrapper_s60.cpp \
        tsrc/stubs/wlanqtutilsconnmonstub.cpp \
        tsrc/stubs/wlanqtutilsesockstub.cpp
               
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x20029F52
    defFilePath = .
    
    BLD_INF_RULES.prj_exports += "rom/wlanqtutilities.iby CORE_MW_LAYER_IBY_EXPORT_PATH(wlanqtutilities.iby)"
    BLD_INF_RULES.prj_exports += \
        "base/inc/wlanqtutils.h |../inc/wlanqtutils.h" \
        "base/inc/wlanqtutilsap.h |../inc/wlanqtutilsap.h" \
        "base/inc/wlanqtutilsiap.h |../inc/wlanqtutilsiap.h"

    # S60 libraries                                 
    LIBS += -lconnmon -lesock -lextendedconnpref -lnetmeta -lictsclientinterface
}
