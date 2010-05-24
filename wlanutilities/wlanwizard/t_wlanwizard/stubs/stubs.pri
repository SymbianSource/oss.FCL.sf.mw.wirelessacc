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
# 
# Description: 
#	Stubs for Wlan wizard testing
#

DEFINES += BUILD_WLANQTUTILITIES_DLL

HEADERS   += \
             stubs/wlanqtutils.h \
             stubs/wlanqtutilscommon.h \
             stubs/wlanqtutilsap.h \
             stubs/context_wlanqtutils.h
             
SOURCES   += \
             stubs/stub_wlanqtutils.cpp \
             stubs/stub_wlanqtutilsap.cpp \
             stubs/context_wlanqtutils.cpp
