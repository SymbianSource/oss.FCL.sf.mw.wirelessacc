/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QSharedPointer>
#include <QTest>
#include <rconnmon.h>
#include <nifvar.h>
#include <cmmanager_shim.h>

#include "wlanqtutils.h"
#include "wlanqtutilstestcontext.h"
#include "wlanqtutilsap.h"
#include "wlanqtutilsconnection.h"

// ---------------------------------------------------------
// class WlanQtUtilsCtxEsock
// ---------------------------------------------------------

WlanQtUtilsCtxEsock::WlanQtUtilsCtxEsock() :
    startRetValue_(KErrNone)
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
    WlanQtUtilsConnection(), connMonBearerType_(EBearerUnknown), connMonConnectionStatus_(
        KConnectionUninitialised), connMonWlanConnectionMode_(-1), connMonWlanSecurityMode_(-1),
        applicationUids_()
{
}

WlanQtUtilsCtxActiveConn::~WlanQtUtilsCtxActiveConn()
{
}

// ---------------------------------------------------------
// class WlanQtUtilsCtxActiveConnections
// ---------------------------------------------------------

WlanQtUtilsCtxActiveConnections::WlanQtUtilsCtxActiveConnections() :
    activeConnList_()
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

void WlanQtUtilsCtxActiveConnections::createDefaultActiveConnList(int numberOfActiveConns,
    int firstIapId)
{
    clearActiveConnList();
    for (int i = 0; i < numberOfActiveConns; i++) {
        WlanQtUtilsCtxActiveConn *activeConn = new WlanQtUtilsCtxActiveConn();

        activeConn->setConnectionId((i + 1) * 100);
        activeConn->setIapId(firstIapId + i);
        activeConn->connMonConnectionStatus_ = KLinkLayerOpen;
        activeConn->setConnectionStatus(WlanQtUtils::ConnStatusConnected);

        // WLAN connection.
        activeConn->connMonBearerType_ = EBearerWLAN;
        activeConn->connMonWlanSecurityMode_ = EConnMonSecurityOpen;
        activeConn->connMonWlanConnectionMode_ = EConnMonInfraStructure;
        activeConnList_.append(activeConn);
    }
}

void WlanQtUtilsCtxActiveConnections::clearActiveConnList()
{
    Q_FOREACH(WlanQtUtilsCtxActiveConn* activeConn, activeConnList_)
        {
            delete activeConn;
        }
    activeConnList_.clear();
}

void WlanQtUtilsCtxActiveConnections::verifyActiveConnList(
    QList<WlanQtUtilsConnection*> activeConnList)
{
    QCOMPARE(activeConnList.count(), activeConnList_.count());

    for (int i = 0; i < activeConnList_.count(); i++) {
        QCOMPARE(activeConnList[i]->connectionId(), activeConnList_[i]->connectionId());
        QCOMPARE(activeConnList[i]->iapId(), activeConnList_[i]->iapId());
        QCOMPARE(activeConnList[i]->connectionStatus(), activeConnList_[i]->connectionStatus());
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

WlanQtUtilsWlanScanResult::WlanQtUtilsWlanScanResult() :
    wlanScanResultList_(),
    completeWlanScan_(true),
    scanRetValue_(KErrNone)
{
}

WlanQtUtilsWlanScanResult::~WlanQtUtilsWlanScanResult()
{
    clearWlanScanResultList();
}

void WlanQtUtilsWlanScanResult::initialize()
{
    clearWlanScanResultList();
    completeWlanScan_ = true;
    scanRetValue_ = KErrNone;
}

void WlanQtUtilsWlanScanResult::createDefaultWlanScanResultList(int numberOfWlanAps)
{
    clearWlanScanResultList();
    for (int i = 0; i < numberOfWlanAps; i++) {
        WlanQtUtilsAp *ap = new WlanQtUtilsAp();
        ap->setValue(WlanQtUtilsAp::ConfIdSsid, "TestWlanAp" + QString::number(i + 1));
        ap->setValue(WlanQtUtilsAp::ConfIdSignalStrength, 20);
        ap->setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
        ap->setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeOpen);
        ap->setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);
        // TODO: Create constructor to WlanQtUtilsWlanAp which takes all member variables as parameter.
        wlanScanResultList_.append(ap);
    }
}

void WlanQtUtilsWlanScanResult::clearWlanScanResultList()
{
    Q_FOREACH(WlanQtUtilsAp* ap, wlanScanResultList_)
        {
            delete ap;
        }
    wlanScanResultList_.clear();
}

void WlanQtUtilsWlanScanResult::verifyWlanScanResultList(
    QList<QSharedPointer<WlanQtUtilsAp> > wlanApList)
{
    QCOMPARE(wlanApList.count(), wlanScanResultList_.count());

    for (int i = 0; i < wlanScanResultList_.count(); i++) {
        QCOMPARE(wlanApList[i]->value(WlanQtUtilsAp::ConfIdSsid),
            wlanScanResultList_[i]->value(WlanQtUtilsAp::ConfIdSsid));
        QCOMPARE(wlanApList[i]->value(WlanQtUtilsAp::ConfIdSignalStrength),
            wlanScanResultList_[i]->value(WlanQtUtilsAp::ConfIdSignalStrength));
        QCOMPARE(wlanApList[i]->value(WlanQtUtilsAp::ConfIdConnectionMode),
            wlanScanResultList_[i]->value(WlanQtUtilsAp::ConfIdConnectionMode));
        QCOMPARE(wlanApList[i]->value(WlanQtUtilsAp::ConfIdSecurityMode),
            wlanScanResultList_[i]->value(WlanQtUtilsAp::ConfIdSecurityMode));
        QCOMPARE(wlanApList[i]->value(WlanQtUtilsAp::ConfIdWpaPskUse),
            wlanScanResultList_[i]->value(WlanQtUtilsAp::ConfIdWpaPskUse));
    }
}

// ---------------------------------------------------------
// class WlanQtUtilsCtxConnMon
// ---------------------------------------------------------

WlanQtUtilsCtxConnMon::WlanQtUtilsCtxConnMon() :
    wlanScanResult_()
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

WlanQtUtilsTestContext::WlanQtUtilsTestContext() :
    esock_(), connMon_(), ict_()
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
