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
* This is a source file for WlanQtUtilsWlanIap class.
*/

#include <QString>
#include "wlanqtutilscommon.h"
#include "wlanqtutilsiap.h"
#include "wlanqtutilswlaniap.h"

WlanQtUtilsWlanIap::WlanQtUtilsWlanIap() : WlanQtUtilsIap(), ssid_(""), signalStrength_(WlanQtUtilsWlanSignalUnavailable), securityMode_(WlanQtUtilsWlanSecModeOpen) 
{
}

WlanQtUtilsWlanIap::WlanQtUtilsWlanIap(int id, int netId, QString name, WlanQtUtilsBearerType bearerType, QString ssid, WlanQtUtilsWlanSecMode secMode) :
    WlanQtUtilsIap(id, netId, name, bearerType), ssid_(ssid), signalStrength_(WlanQtUtilsWlanSignalUnavailable), securityMode_(secMode)
{
}

WlanQtUtilsWlanIap::~WlanQtUtilsWlanIap()
{
}

/** Returns IAP's SSID */
QString WlanQtUtilsWlanIap::ssid() const
{
    return ssid_;
}

/** Returns IAP's signal strength */
int WlanQtUtilsWlanIap::signalStrength() const
{
    return signalStrength_;
}

/** Returns IAP's security mode */
WlanQtUtilsWlanSecMode WlanQtUtilsWlanIap::securityMode() const
{
    return securityMode_;
}

/** Sets IAP's SSID */
void WlanQtUtilsWlanIap::setSsid(QString ssid)
{
    ssid_ = ssid;
}

/** Sets IAP's signal strength */
void WlanQtUtilsWlanIap::setSignalStrength(int signalStrength)
{
    signalStrength_ = signalStrength;
}

/** Sets IAP's security mode */
void WlanQtUtilsWlanIap::setSecurityMode(WlanQtUtilsWlanSecMode securityMode)
{
    securityMode_ = securityMode;
}
