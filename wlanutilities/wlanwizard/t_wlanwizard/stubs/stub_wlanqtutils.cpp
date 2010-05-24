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

#include "wlanqtutilsap.h"
#include "wlanqtutils.h"
#include <QDebug>

WlanQtUtils::WlanQtUtils() : mWlanAp(NULL)
{
    mWlanNetworkActiveIapId = -1;
    mCalledMethods.append("WlanQtUtils");
}

WlanQtUtils::~WlanQtUtils()
{
    mCalledMethods.append("~WlanQtUtils");
    delete mWlanAp;
}

void WlanQtUtils::availableWlanAps(QList<QSharedPointer<WlanQtUtilsAp> > &wlanApList)
{
    wlanApList = *mScanResult;
    mCalledMethods.append("availableWlanAps");
}

int WlanQtUtils::createIap(const WlanQtUtilsAp * wlanAp)
{
    mCalledMethods.append("createIap");
    delete mWlanAp;
    mWlanAp = new WlanQtUtilsAp(*wlanAp);
    return mCreateWlanIapReturn;
}

bool WlanQtUtils::updateIap(int iapId, const WlanQtUtilsAp * wlanAp)
{
    mCalledMethods.append("updateIap"); 
    qDebug("updateWlanIap iapId = %d", iapId);
    qDebug("mCreateWlanIapReturn = %d", mCreateWlanIapReturn);
    delete mWlanAp;
    mWlanAp = new WlanQtUtilsAp(*wlanAp);
    QCOMPARE(iapId, mCreateWlanIapReturn);
    return mUpdateWlanIapReturn;
}

void WlanQtUtils::deleteIap(int iapId)
{
    mCalledMethods.append("deleteIap");
    QCOMPARE(iapId, mCreateWlanIapReturn);
}

int WlanQtUtils::connectedWlanId()
{
    mCalledMethods.append("connectedWlanId");
    return mCreateWlanIapReturn;
}

void WlanQtUtils::connectIap(int iapId, bool runIct)
{
    qDebug("connectIap iapId = %d", iapId);
    qDebug("mCreateWlanIapReturn = %d", mCreateWlanIapReturn);
    mCalledMethods.append("connectIap");
    QCOMPARE(iapId, mCreateWlanIapReturn);
    QCOMPARE(runIct, true);
    
    if (mConnectionSuccess) {
        mWlanNetworkActiveIapId = iapId;
        emit wlanNetworkOpened(mWlanNetworkOpenedIapId);
        emit ictResult(mIctResultIapId, mIctResultResults);
    } else {
        emit wlanNetworkClosed(
            mWlanNetworkClosedIapId,
            mWlanNetworkClosedReason );
    }
}

void WlanQtUtils::disconnectIap(int iapId)
{
    qDebug("disconnectIap iapId = %d", iapId);
    qDebug("mCreateWlanIapReturn = %d", mCreateWlanIapReturn);
    mCalledMethods.append("disconnectIap");
    QCOMPARE(iapId, mCreateWlanIapReturn);
}

int WlanQtUtils::activeIap()
{
    mCalledMethods.append("activeIap");
    return mCreateWlanIapReturn;
}

void WlanQtUtils::scanWlanAps()
{
    mCalledMethods.append("scanWlanAps");
    mScanResult = &mScanWlanAps;
    
    emit wlanScanApReady();
}

void WlanQtUtils::scanWlanDirect(const QString &ssid)
{
    mCalledMethods.append("scanWlanDirect");
    QCOMPARE(ssid, mScanWlanDirectSsid);
    mScanResult = &mScanWlanDirect;
    
    emit wlanScanDirectReady();
}

void WlanQtUtils::stopWlanScan()
{
    mCalledMethods.append("stopWlanScan");
}

