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
*/

#include "wlanqtutilswlanap.h"
#include "wlanqtutilswlaniap.h"

#include "wlanqtutils_p.h"
#include "wlanqtutils.h"

WlanQtUtils::WlanQtUtils()
{
    d_ptr = new WlanQtUtilsPrivate(this);
}

WlanQtUtils::~WlanQtUtils()
{
    delete d_ptr;
}

void WlanQtUtils::availableWlanAps(
    QList<WlanQtUtilsWlanIap *> &wlanIapList,
    QList<WlanQtUtilsWlanAp *> &wlanApList)
{
    d_ptr->availableWlanAps(wlanIapList, wlanApList);
}

int WlanQtUtils::createWlanIap(const WlanQtUtilsWlanAp *wlanAp)
{
    return d_ptr->createWlanIap(wlanAp);
}

void WlanQtUtils::connectIap(int iapId)
{
    d_ptr->connectIap(iapId);
}

void WlanQtUtils::disconnectIap(int iapId)
{
    d_ptr->disconnectIap(iapId);
}

WlanQtUtilsIap *WlanQtUtils::iap(int iapId) const
{
    return d_ptr->iap(iapId);
}

bool WlanQtUtils::masterWlan() const
{
    return d_ptr->masterWlan();
}

void WlanQtUtils::setMasterWlan(bool enabled)
{
    d_ptr->setMasterWlan(enabled);
}

int WlanQtUtils::connectedWlanId() const
{
    return d_ptr->connectedWlanId();
}

void WlanQtUtils::scanWlans()
{
    d_ptr->scanWlans();
}

// End of File
