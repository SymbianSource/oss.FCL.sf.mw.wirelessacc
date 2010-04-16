/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* This is the source file for WlanQtUtilsWlanAp class.
*/

#include <QString>
#include "wlanqtutilscommon.h"
#include "wlanqtutilswlanap.h"

WlanQtUtilsWlanAp::WlanQtUtilsWlanAp() : ssid_(""), bssid_(""), signalStrength_(0), connectionMode_(0),
    securityMode_(WlanQtUtilsWlanSecModeOpen), securityKey_("")
{
}

WlanQtUtilsWlanAp::WlanQtUtilsWlanAp(const WlanQtUtilsWlanAp &ref) :
    ssid_(ref.ssid()),
    bssid_(ref.bssid()),
    signalStrength_(ref.signalStrength()),
    connectionMode_(ref.connectionMode()),
    securityMode_(ref.securityMode()),
    securityKey_(ref.securityKey())
{
}

WlanQtUtilsWlanAp::WlanQtUtilsWlanAp(
    QString ssid,
    QString bssid,
    int signalStrength,
    int connectionMode,
    WlanQtUtilsWlanSecMode securityMode) : ssid_(ssid), bssid_(bssid), signalStrength_(signalStrength),
    connectionMode_(connectionMode), securityMode_(securityMode), securityKey_("")
{
}

WlanQtUtilsWlanAp::~WlanQtUtilsWlanAp()
{
}

QString WlanQtUtilsWlanAp::ssid() const
{
    return ssid_;
}

QString WlanQtUtilsWlanAp::bssid() const
{
    return bssid_;
}

int WlanQtUtilsWlanAp::signalStrength() const
{
    return signalStrength_;
}

int WlanQtUtilsWlanAp::connectionMode() const
{
    return connectionMode_;
}

WlanQtUtilsWlanSecMode WlanQtUtilsWlanAp::securityMode() const
{
    return securityMode_;
}

QString WlanQtUtilsWlanAp::securityKey() const
{
    return securityKey_;
}

void WlanQtUtilsWlanAp::setSsid(QString ssid)
{
    ssid_ = ssid;
}

void WlanQtUtilsWlanAp::setBssid(QString bssid)
{
    bssid_ = bssid;
}

void WlanQtUtilsWlanAp::setSignalStrength(int signalStrength)
{
    signalStrength_ = signalStrength;
}

void WlanQtUtilsWlanAp::setConnectionMode(int connectionMode)
{
    connectionMode_ = connectionMode;
}

void WlanQtUtilsWlanAp::setSecurityMode(WlanQtUtilsWlanSecMode securityMode)
{
    securityMode_ = securityMode;
}

void WlanQtUtilsWlanAp::setSecurityKey(QString securityKey)
{
    securityKey_ = securityKey;
}
