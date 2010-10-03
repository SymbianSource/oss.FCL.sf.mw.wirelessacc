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
TARGET = wlanindicatortest
TARGET.CAPABILITY = ALL -TCB

DEPENDPATH += .

INCLUDEPATH += ./inc

CONFIG += hb
QT += core \
    gui

INCLUDEPATH += ../inc

HEADERS += inc/wlanindicatortestmainwindow.h\
		inc/wlanindicatortestview.h\
		inc/wlanindicatortest.h
				
SOURCES += src/main.cpp\
		src/wlanindicatortest.cpp\
		src/wlanindicatortestview.cpp\
		src/wlanindicatortestmainwindow.cpp
		 
RESOURCES += res/wlanindicatortest.qrc

DOCML += \
    res/wlanindicatortestview.docml \
    res/wlanindicatortestview_portrait.docml

symbian*: {
    TARGET.UID3 = 0xE4E6D7C8
    SYMBIAN_PLATFORMS = WINSCW ARMV5       
    addFiles.path = /sys/bin
    DEPLOYMENT += addFiles    
} 

symbian*::LIBS +=  -HbCore  -HbWidgets 
