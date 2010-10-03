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

CONFIG += qtestlib console hb

HEADERS += wlanindicatorutcases.h \
           ../../inc/wlanindicatorplugin.h \
           ../../traces/OstTraceDefinitions.h

SOURCES += wlanindicatorutcases.cpp wlanindicatorutmain.cpp \
           ../../src/wlanindicatorplugin.cpp


symbian: {
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1
    LIBS += -wlanindicatorplugin -HbCore  -HbWidgets
    # Enable UT for plugin
    DEFINES += WLANINDICATOR_UT
}
