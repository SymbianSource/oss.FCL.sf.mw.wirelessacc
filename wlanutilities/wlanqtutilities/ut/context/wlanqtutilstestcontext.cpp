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
* This is the source file for Connection Utilities test context.
*/

#include <QObject>
#include <QTest>
#include <rconnmon.h>
#include <nifvar.h>
#include "wlanqtutilscommon.h"
#include "wlanqtutilstestcontext.h"
#include "wlanqtutilswlanap.h"
#include "wlanqtutilsactiveconn.h"

// ---------------------------------------------------------
// class WlanQtUtilsCtxEsock
// ---------------------------------------------------------

WlanQtUtilsCtxEsock::WlanQtUtilsCtxEsock() : startRetValue_(KErrNone)
{
}

WlanQtUtilsCtxEsock::~WlanQtUtilsCtxEsock()
{
}

void WlanQtUtilsCtxEsock::initialize()
{
    startRetValue_ = KErrNone;
}

// ---------------------------------------------------------
// class WlanQtUtilsCtxActiveConn
// ---------------------------------------------------------

WlanQtUtilsCtxActiveConn::WlanQtUtilsCtxActiveConn() :
    WlanQtUtilsActiveConn(),
    connMonBearerType_(EBearerUnknown),
    connMonConnectionStatus_(KConnectionUninitialised),
    connMonWlanConnectionMode_(-1),
    connMonWlanSecurityMode_(-1),
    applicationUids_()
{
}

WlanQtUtilsCtxActiveConn::~WlanQtUtilsCtxActiveConn()
{
}

// ---------------------------------------------------------
// class WlanQtUtilsCtxActiveConnections
// ---------------------------------------------------------

WlanQtUtilsCtxActiveConnections::WlanQtUtilsCtxActiveConnections() : activeConnList_()
{
}

WlanQtUtilsCtxActiveConnections::~WlanQtUtilsCtxActiveConnections()
{
    clearActiveConnList();
}

void WlanQtUtilsCtxActiveConnections::initialize()
{
    clearActiveConnList();
}

void WlanQtUtilsCtxActiveConnections::createDefaultActiveConnList(int numberOfActiveConns, int firstIapId)
{
    clearActiveConnList();
    for (int i = 0; i < numberOfActiveConns; i++) {
        WlanQtUtilsCtxActiveConn *activeConn = new WlanQtUtilsCtxActiveConn();

        activeConn->setConnectionId((i + 1) * 100);
        activeConn->setIapId(firstIapId + i);
        activeConn->connMonConnectionStatus_ = KConnectionOpen;
        activeConn->setConnectionStatus(WlanQtUtilsConnectionStatusConnected);
        
        // WLAN connection.
        activeConn->connMonBearerType_ = EBearerWLAN;
        activeConn->setBearerType(WlanQtUtilsBearerTypeWlan);
        activeConn->connMonWlanSecurityMode_ = EConnMonSecurityOpen;
        activeConn->connMonWlanConnectionMode_ = EConnMonInfraStructure;
        activeConnList_.append(activeConn);
    }
}

void WlanQtUtilsCtxActiveConnections::clearActiveConnList()
{
    Q_FOREACH(WlanQtUtilsCtxActiveConn* activeConn, activeConnList_) {
        delete activeConn;
    }
    activeConnList_.clear();
}

void WlanQtUtilsCtxActiveConnections::verifyActiveConnList(QList<WlanQtUtilsActiveConn*> activeConnList)
{
    QCOMPARE(activeConnList.count(), activeConnList_.count());

    for (int i = 0; i < activeConnList_.count(); i++) {
        QCOMPARE(activeConnList[i]->connectionId(), activeConnList_[i]->connectionId());
        QCOMPARE(activeConnList[i]->iapId(), activeConnList_[i]->iapId());
        QCOMPARE(activeConnList[i]->connectionStatus(), activeConnList_[i]->connectionStatus());
        QCOMPARE(activeConnList[i]->bearerType(), activeConnList_[i]->bearerType());
    }
}

WlanQtUtilsCtxActiveConn *WlanQtUtilsCtxActiveConnections::findActiveConn(uint connectionId) const
{
    WlanQtUtilsCtxActiveConn *activeConn = NULL;
    for (int i = 0; i < activeConnList_.count(); i++) {
        if (activeConnList_[i]->connectionId() == connectionId) {
            activeConn = activeConnList_[i];
        }
    }
    Q_ASSERT(activeConn != NULL);
    return activeConn;
}

// ---------------------------------------------------------
// class WlanQtUtilsWlanScanResult
// ---------------------------------------------------------

WlanQtUtilsWlanScanResult::WlanQtUtilsWlanScanResult() : wlanScanResultList_()
{
}

WlanQtUtilsWlanScanResult::~WlanQtUtilsWlanScanResult()
{
    clearWlanScanResultList();
}

void WlanQtUtilsWlanScanResult::initialize()
{
    clearWlanScanResultList();
}

void WlanQtUtilsWlanScanResult::createDefaultWlanScanResultList(int numberOfWlanAps)
{
    clearWlanScanResultList();
    for (int i = 0; i < numberOfWlanAps; i++) {
        WlanQtUtilsWlanAp *ap = new WlanQtUtilsWlanAp();
        ap->setSsid("TestWlanAp" + QString::number(i+1));
        ap->setBssid("addMAC"); // TODO: Generate something when bssid retrieval works.
        ap->setSignalStrength(20);
        ap->setConnectionMode(EConnMonInfraStructure);
        ap->setSecurityMode(WlanQtUtilsWlanSecModeOpen);
        // TODO: Create constructor to WlanQtUtilsWlanAp which takes all member variables as parameter.
        wlanScanResultList_.append(ap);
    }
}

void WlanQtUtilsWlanScanResult::clearWlanScanResultList()
{
    Q_FOREACH(WlanQtUtilsWlanAp* ap, wlanScanResultList_) {
        delete ap;
    }
    wlanScanResultList_.clear();
}

void WlanQtUtilsWlanScanResult::verifyWlanScanResultList(QList<WlanQtUtilsWlanAp*> wlanApList)
{
    QCOMPARE(wlanApList.count(), wlanScanResultList_.count());

    for (int i = 0; i < wlanScanResultList_.count(); i++) {
        QCOMPARE(wlanApList[i]->ssid(), wlanScanResultList_[i]->ssid());
        QCOMPARE(wlanApList[i]->bssid(), wlanScanResultList_[i]->bssid());
        QCOMPARE(wlanApList[i]->signalStrength(), wlanScanResultList_[i]->signalStrength());
        QCOMPARE(wlanApList[i]->connectionMode(), wlanScanResultList_[i]->connectionMode());
        QCOMPARE(wlanApList[i]->securityMode(), wlanScanResultList_[i]->securityMode());
    }
}

// ---------------------------------------------------------
// class WlanQtUtilsCtxConnMon
// ---------------------------------------------------------

WlanQtUtilsCtxConnMon::WlanQtUtilsCtxConnMon() : wlanScanResult_()
{
}

WlanQtUtilsCtxConnMon::~WlanQtUtilsCtxConnMon()
{
}

void WlanQtUtilsCtxConnMon::initialize()
{
    wlanScanResult_.initialize();
    activeConnections_.initialize();
}

// ---------------------------------------------------------
// class WlanQtUtilsCtxIct
// ---------------------------------------------------------

WlanQtUtilsCtxIct::WlanQtUtilsCtxIct()
{
}

WlanQtUtilsCtxIct::~WlanQtUtilsCtxIct()
{
}

void WlanQtUtilsCtxIct::initialize()
{
}

// ---------------------------------------------------------
// class WlanQtUtilsTestContext
// ---------------------------------------------------------

WlanQtUtilsTestContext::WlanQtUtilsTestContext() : esock_(), connMon_(), ict_()
{
}

WlanQtUtilsTestContext::~WlanQtUtilsTestContext()
{
}

void WlanQtUtilsTestContext::initialize()
{
    esock_.initialize();
    connMon_.initialize();
    ict_.initialize();
}
