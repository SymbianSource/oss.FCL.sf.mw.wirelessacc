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
# Project file for wlanutilities directory in wirelessacc package.
#

TEMPLATE = subdirs

SUBDIRS  += \
    wlanqtutilities \
    cpwlanentryplugin \
    eapwizard \
    wpswizard \
    wlanwizard \
    wlansniffer \
    wlanindicatorplugin \
    cpwlansettingsplugin \
    wlanlogin

CONFIG += ordered
