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
#	WLAN Settings Wizard
#

include(wlanwizard.pro)

DEFINES += WLAN_WIZARD_RND

HEADERS   += \
             inc/wlanwizardpagenetworkmode.h \
             inc/wlanwizardpagescanning.h \
             inc/wlanwizardpagesecuritymode.h \
             inc/wlanwizardpagessid.h 
             
SOURCES   += \
             src/wlanwizardpagenetworkmode.cpp \
             src/wlanwizardpagescanning.cpp \
             src/wlanwizardpagesecuritymode.cpp \
             src/wlanwizardpagessid.cpp
