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
* WLAN Qt Utilities private implementation.
*/

// System includes

#include <QSharedPointer>

// User includes

#include "wlanqtutilsap.h"
#include "wlanqtutilsiap.h"
#include "wlanqtutilsconnection.h"
#include "wlanqtutilsiapsettings.h"
#include "wlanqtutilsconmonwrapper.h"
#include "wlanqtutilsesockwrapper.h"
#include "wlanqtutilsconntestwrapper.h"

#include "wlanqtutils.h"
#include "wlanqtutils_p.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutils_pTraces.h"
#endif

/*!
    \class WlanQtUtilsPrivate
    \brief Private implementation class for WLAN Qt Utilities.

    The private interface functions are identical to those in WlanQtUtils
    class, so refer to that for descriptions. Other functions contain
    implementation documentation.
*/


// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
*/

WlanQtUtilsPrivate::WlanQtUtilsPrivate(WlanQtUtils *q_ptr) :
    q_ptr(q_ptr),
    mSettings(new WlanQtUtilsIapSettings(this)),
    mConMonWrapper(new WlanQtUtilsConMonWrapper(this)),
    mEsockWrapper(new WlanQtUtilsEsockWrapper(this)),
    mConnTestWrapper(new ConnTestWrapper(this)),
    mWlanScanList(),
    mToBeTestedIapId(WlanQtUtils::IapIdNone), 
    mConnectingIapId(WlanQtUtils::IapIdNone),
    mConnection()
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_WLANQTUTILSPRIVATE_ENTRY, this);

    // Make all connections.
    bool connectStatus = connect(
        mConMonWrapper, 
        SIGNAL(availableWlanApsFromWrapper(
            QList< QSharedPointer<WlanQtUtilsAp> >&)), 
        this, 
        SLOT(updateAvailableWlanAps(
            QList< QSharedPointer<WlanQtUtilsAp> >&)));
    Q_ASSERT(connectStatus);

    connectStatus = connect(
        mEsockWrapper, 
        SIGNAL(connectionStatusFromWrapper(bool)), 
        this,
        SLOT(updateConnectionStatus(bool)));
    Q_ASSERT(connectStatus);

    connectStatus = connect(
        mConnTestWrapper, 
        SIGNAL(connectivityTestResult(bool)), 
        this,
        SLOT(updateConnectivityTestResult(bool)));
    Q_ASSERT(connectStatus);

    connectStatus = connect(
        mConMonWrapper, 
        SIGNAL(connCreatedEventFromWrapper(uint)), 
        this,
        SLOT(addActiveConnection(uint)));
    Q_ASSERT(connectStatus);

    connectStatus = connect(
        mConMonWrapper, 
        SIGNAL(connDeletedEventFromWrapper(uint)), 
        this,
        SLOT(removeActiveConnection(uint)));
    Q_ASSERT(connectStatus);

    connectStatus = connect(
        mConMonWrapper, 
        SIGNAL(connStatusEventFromWrapper(uint, WlanQtUtils::ConnStatus)),
        this, 
        SLOT(updateActiveConnection(uint, WlanQtUtils::ConnStatus)));
    Q_ASSERT(connectStatus);

    // Retrieve initial status of active connections
    mConnection = QSharedPointer<WlanQtUtilsConnection>(
        mConMonWrapper->activeConnection());

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_WLANQTUTILSPRIVATE_EXIT, this);
}

/*!
    Destructor. 
*/

WlanQtUtilsPrivate::~WlanQtUtilsPrivate()
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_WLANQTUTILSPRIVATEDESTR_ENTRY, this);
    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_WLANQTUTILSPRIVATEDESTR_EXIT, this);
}

/*!
   See WlanQtUtils::scanWlans().
*/

void WlanQtUtilsPrivate::scanWlans()
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_SCANWLANS_ENTRY, this);

    // Just forward the request to wrapper, which triggers a single WLAN scan
    mConMonWrapper->scanAvailableWlanAPs();

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_SCANWLANS_EXIT, this);
}

/*!
   See WlanQtUtils::scanWlanAps().
*/

void WlanQtUtilsPrivate::scanWlanAps()
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_SCANWLANAPS_ENTRY, this);
    
    emit q_ptr->wlanScanApReady();
    // TODO: implement me (wizard requires this)

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_SCANWLANAPS_EXIT, this);
}

/*!
   See WlanQtUtils::scanWlanDirect().
*/

void WlanQtUtilsPrivate::scanWlanDirect(const QString &ssid)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_SCANWLANDIRECT_ENTRY, this);
    
    Q_UNUSED(ssid);
    
    emit q_ptr->wlanScanDirectReady();
    // TODO: implement me (wizard requires this)
    
    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_SCANWLANDIRECT_EXIT, this);
}

/*!
   See WlanQtUtils::stopWlanScan().
*/

void WlanQtUtilsPrivate::stopWlanScan()
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_STOPWLANSCAN_ENTRY, this);
    
    mConMonWrapper->stopScan();
    
    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_STOPWLANSCAN_EXIT, this);
}

/*!
   See WlanQtUtils::availableWlans().
*/

void WlanQtUtilsPrivate::availableWlans(
    QList< QSharedPointer<WlanQtUtilsIap> > &wlanIapList,
    QList< QSharedPointer<WlanQtUtilsAp> > &wlanApList) const
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_AVAILABLEWLAN_ENTRY, this);

    wlanIapList.clear();
    wlanApList.clear();

    // Read the list of configured IAPs
    QList<QSharedPointer<WlanQtUtilsIap> > configuredIapList;
    mSettings->fetchIaps(configuredIapList);

    // Match IAPs against WLAN scan results
    foreach (QSharedPointer<WlanQtUtilsIap> iap, configuredIapList) {
        foreach (QSharedPointer<WlanQtUtilsAp> scanAp, mWlanScanList) {
            if (WlanQtUtilsAp::compare(iap.data(), scanAp.data()) == true) {
                // IAP found, add it to caller's list of known IAPs
                // (signal strength needs to be updated manually since
                // the IAP in our list does not have that information yet)
                iap->setValue(
                    WlanQtUtilsAp::ConfIdSignalStrength,
                    scanAp->value(WlanQtUtilsAp::ConfIdSignalStrength));
                wlanIapList.append(iap);
                break;
            }
        }
    }

    // Go through the scan results to find unknown APs
    for (int i = 0; i < mWlanScanList.count(); i++) {
        // Check whether an IAP with these parameters exists (in which
        // case this network is already added as an IAP in the loop above)
        if (!wlanIapExists(configuredIapList, mWlanScanList[i].data())) {
            // No IAP found in, copy the AP to caller's list of
            // unknown APs
            wlanApList.append(mWlanScanList[i]);
        }
    }

    traceIapsAndAps(wlanIapList, wlanApList);

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_AVAILABLEWLAN_EXIT, this);
}

/*!
   See WlanQtUtils::availableWlanAps().
*/

void WlanQtUtilsPrivate::availableWlanAps(
    QList< QSharedPointer<WlanQtUtilsAp> > &wlanApList) const
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_AVAILABLEWLANAPS_ENTRY, this);
    
    Q_UNUSED(wlanApList);
    // TODO: implement me (wizard requires this)
    
    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_AVAILABLEWLANAPS_EXIT, this);
}

/*!
   See WlanQtUtils::createIap().
*/

int WlanQtUtilsPrivate::createIap(const WlanQtUtilsAp *wlanAp)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_CREATEIAP_ENTRY, this);

    // Create the new IAP and return its ID
    int newIapId = mSettings->createIap(wlanAp);

    OstTrace1(
        TRACE_BORDER,
        WLANQTUTILSPRIVATE_CREATEIAP,
        "WlanQtUtilsPrivate::createIap;New IAP ID=%d",
        newIapId);
        
    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_CREATEIAP_EXIT, this);
    return newIapId;
}

/*!
   See WlanQtUtils::updateIap().
*/

bool WlanQtUtilsPrivate::updateIap(int iapId, const WlanQtUtilsAp *wlanAp)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_UPDATEIAP_ENTRY, this);

    bool success = mSettings->updateIap(iapId, wlanAp);
    
    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_UPDATEIAP_EXIT, this);
    return success;
}

/*!
   See WlanQtUtils::deleteIap().
*/

void WlanQtUtilsPrivate::deleteIap(int iapId)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_DELETEIAP_ENTRY, this);

    mSettings->deleteIap(iapId);

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_DELETEIAP_EXIT, this);
}

/*!
   See WlanQtUtils::connectIap().
*/

void WlanQtUtilsPrivate::connectIap(int iapId, bool runIct)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_CONNECTIAP_ENTRY, this);
    
    OstTraceExt2(
        TRACE_BORDER,
        WLANQTUTILSPRIVATE_CONNECTIAP,
        "WlanQtUtilsPrivate::connectIap;IAP ID=%d;runIct=%hhu",
        iapId,
        runIct);

    Q_ASSERT(iapId != WlanQtUtils::IapIdNone);

    if (runIct) {
        // Mark this IAP for ICT testing after it has been opened
        mToBeTestedIapId = iapId;
    }
    mConnectingIapId = iapId;
    mEsockWrapper->connectIap(iapId);

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_CONNECTIAP_EXIT, this);
}

/*!
   See WlanQtUtils::disconnectIap().
*/

void WlanQtUtilsPrivate::disconnectIap(int iapId)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_DISCONNECTIAP_ENTRY, this);
    
    OstTrace1(
        TRACE_BORDER,
        WLANQTUTILSPRIVATE_DISCONNECTIAP,
        "WlanQtUtilsPrivate::disconnectIap;IAP ID=%d", iapId );

    if (iapId != WlanQtUtils::IapIdNone) {
        // Close our RConnection handle, if needed. Wrapper ignores call, if
        // no handle exists.
        mEsockWrapper->disconnectIap();
        
        // In order to close connection even if there are other users for the
        // IAP, close also via ConMon
        mConMonWrapper->disconnectIap(iapId);
    } // else: just ignore call

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_DISCONNECTIAP_EXIT, this);
}

/*!
   See WlanQtUtils::connectionStatus().
*/

WlanQtUtils::ConnStatus WlanQtUtilsPrivate::connectionStatus() const
{
    OstTraceFunctionEntry0(WLANQTUTILSPRIVATE_CONNECTIONSTATUS_ENTRY);

    WlanQtUtils::ConnStatus status = WlanQtUtils::ConnStatusDisconnected;
    
    if (mConnection) {
        status = mConnection->connectionStatus();
    }
    OstTrace1(
        TRACE_BORDER,
        WLANQTUTILSPRIVATE_CONNECTIONSTATUS,
        "WlanQtUtilsPrivate::connectionStatus;status=%{ConnStatus};",
        (TUint)status);
    
    OstTraceFunctionExit0(WLANQTUTILSPRIVATE_CONNECTIONSTATUS_EXIT);
    return status;
}

/*!
   See WlanQtUtils::activeIap().
*/

int WlanQtUtilsPrivate::activeIap() const
{
    OstTraceFunctionEntry0(WLANQTUTILSPRIVATE_ACTIVEIAP_ENTRY);

    int iapId = WlanQtUtils::IapIdNone;
    
    if (mConnection) {
        iapId = mConnection->iapId();
    }
    OstTrace1(
        TRACE_BORDER,
        WLANQTUTILSPRIVATE_ACTIVEIAP,
        "WlanQtUtilsPrivate::activeIap;iapId=%d",
        iapId);
    
    OstTraceFunctionExit0(WLANQTUTILSPRIVATE_ACTIVEIAP_EXIT);
    return iapId;
}

/*!
   See WlanQtUtils::iapName().
*/

QString WlanQtUtilsPrivate::iapName(int iapId) const
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_IAPNAME_ENTRY, this);
    
    QString name;
    // Read the IAP from settings and return its name
    QSharedPointer<WlanQtUtilsIap> iap = mSettings->fetchIap(iapId);
    if (iap) {
        name = iap->value(WlanQtUtilsIap::ConfIdName).toString();
    }

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_IAPNAME_EXIT, this);
    return name;
}

/*!
    This function searches for a WLAN IAP matching the given WLAN AP.

    @param [in] list List to search from.
    @param [in] ap Access point to search for.
    
    @return True, if suitable WLAN IAP found, false otherwise.
*/

bool WlanQtUtilsPrivate::wlanIapExists(
    const QList< QSharedPointer<WlanQtUtilsIap> > list,
    const WlanQtUtilsAp *ap) const
{
    bool match = false;     // Return value
    
    foreach (QSharedPointer<WlanQtUtilsIap> iap, list) {
        if (WlanQtUtilsAp::compare(iap.data(), ap) == true) {
            // Match found
            match = true;
            break;
        }
    }
    
    return match;
}

/*!
    This function traces the given IAPs and APs.

    @param [in] iaps IAPs to trace.
    @param [in] aps APs to trace.
*/

void WlanQtUtilsPrivate::traceIapsAndAps(
    const QList<QSharedPointer<WlanQtUtilsIap> > &iaps,
    const QList<QSharedPointer<WlanQtUtilsAp> > &aps) const
{
#ifndef OST_TRACE_COMPILER_IN_USE
    Q_UNUSED(iaps);
    Q_UNUSED(aps);
#else
    foreach (QSharedPointer<WlanQtUtilsIap> iap, iaps) {
        QString tmp(iap->value(WlanQtUtilsIap::ConfIdName).toString());
        TPtrC16 name(tmp.utf16(), tmp.length());
        OstTraceExt3(
            TRACE_NORMAL,
            WLANQTUTILSPRIVATE_TRACEAVAILABLEWLANAPS_IAP,
            "WlanQtUtilsPrivate::traceAvailableWlanAps IAP;name=%S;iap->securityMode()=%d;iap->signalStrength()=%d",
            name,
            iap->value(WlanQtUtilsAp::ConfIdSecurityMode).toInt(),
            iap->value(WlanQtUtilsAp::ConfIdSignalStrength).toInt());
    }
    foreach (QSharedPointer<WlanQtUtilsAp> ap, aps) {
        QString tmp(ap->value(WlanQtUtilsAp::ConfIdSsid).toString());
        TPtrC16 ssid(tmp.utf16(), tmp.length());
        OstTraceExt3(
            TRACE_NORMAL,
            WLANQTUTILSPRIVATE_TRACEAVAILABLEWLANAPS_AP,
            "WlanQtUtilsPrivate::traceAvailableWlanAps AP;ssid=%S;ap->securityMode()=%d;ap->signalStrength()=%d",
            ssid,
            ap->value(WlanQtUtilsAp::ConfIdSecurityMode).toInt(),
            ap->value(WlanQtUtilsAp::ConfIdSignalStrength).toInt() );
    }
#endif
}

/*!
    Slot for handling WLAN scan result event from wrapper. Results are
    stored in member variable (possible duplicates are removed).

    @param [in] availableWlanList WLAN networks found in scan.
*/

void WlanQtUtilsPrivate::updateAvailableWlanAps(
    QList<QSharedPointer<WlanQtUtilsAp> > &availableWlanList)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_UPDATEAVAILABLEWLANAPS_ENTRY, this);

    // Old results are removed
    mWlanScanList.clear();
    // Copy available WLANs to scan result list (duplicates are removed)
    for (int i = 0; i < availableWlanList.count(); i++) {
        bool duplicate = false;
        for (int j = 0; j < mWlanScanList.count(); j++) {
            if (WlanQtUtilsAp::compare(
                availableWlanList[i].data(),
                mWlanScanList[j].data()) == true) {
                duplicate = true;
                break;
            }
        }
        if (duplicate == false) {
            mWlanScanList.append(availableWlanList[i]);
        }
    }

    // The information is forwarded to the UI
    OstTrace0(
        TRACE_BORDER,
        WLANQTUTILSPRIVATE_WLANSCANREADY,
        "WlanQtUtilsPrivate::emit wlanScanReady");
    emit q_ptr->wlanScanReady();
    
    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_UPDATEAVAILABLEWLANAPS_EXIT, this);
}

/*!
    Slot for handling connection setup status event from wrapper.

    @param [in] isOpened Was connection setup successful?
*/

void WlanQtUtilsPrivate::updateConnectionStatus(bool isOpened)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_UPDATECONNECTIONSTATUS_ENTRY, this);

    if (isOpened == false) {
        // Opening failed, update connection status and inform UI
        if (mConnection) {
            mConnection->setConnectionStatus(
                WlanQtUtils::ConnStatusDisconnected);
        }
        
        // Include the status code from connection wrapper
        int status = mEsockWrapper->lastStatusCode();
        OstTraceExt2(
            TRACE_BORDER,
            WLANQTUTILSPRIVATE_WLANNETWORKCLOSED,
            "WlanQtUtilsPrivate::emit wlanNetworkClosed;IAP ID=%d;status=%d",
            mConnectingIapId,
            status);
        emit q_ptr->wlanNetworkClosed(mConnectingIapId, status);
    } else {
        // Opening succeeded, update connection status and inform UI
        if (mConnection) {
            mConnection->setConnectionStatus(
                WlanQtUtils::ConnStatusConnected);
        }
        
        OstTrace1(
            TRACE_BORDER,
            WLANQTUTILSPRIVATE_WLANNETWORKOPENED,
            "WlanQtUtilsPrivate::emit wlanNetworkOpened;IAP ID=%d",
            mConnectingIapId);
        emit q_ptr->wlanNetworkOpened(mConnectingIapId);

        // Start ICT, if needed
        if (mConnectingIapId == mToBeTestedIapId) {
            QSharedPointer<WlanQtUtilsIap> iap = 
                mSettings->fetchIap(mToBeTestedIapId);
            mConnTestWrapper->startConnectivityTest(
                mToBeTestedIapId,
                iap->value(WlanQtUtilsIap::ConfIdNetworkId).toInt());
        }
    }
    // IAP is no more in connecting state
    mConnectingIapId = WlanQtUtils::IapIdNone;

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_UPDATECONNECTIONSTATUS_EXIT, this);
}

/*!
    Slot for handling connectivity test result event from wrapper. Tested
    IAP is stored to Internet SNAP, if test was successful.

    @param [in] result Was the test successful?
*/

void WlanQtUtilsPrivate::updateConnectivityTestResult(bool result)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_UPDATECONNECTIVITYTESTRESULT_ENTRY, this);

    if (result == true) {
        // Move the tested IAP to Internet SNAP since the test passed
        Q_ASSERT(mToBeTestedIapId != WlanQtUtils::IapIdNone);
        mSettings->moveIapToInternetSnap(mToBeTestedIapId);
    } // else: do nothing, IAP remains to be uncategorized

    // Inform UI
    OstTraceExt2(
        TRACE_NORMAL,
        WLANQTUTILSPRIVATE_ICTRESULT,
        "WlanQtUtilsPrivate::emit ictResult;result=%hhu;IAP ID=%d",
        result,
        mToBeTestedIapId);

    emit q_ptr->ictResult(mToBeTestedIapId, result);

    // This IAP is now tested
    mToBeTestedIapId = WlanQtUtils::IapIdNone;

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_UPDATECONNECTIVITYTESTRESULT_EXIT, this);
}

/*!
    Slot for updating active connection status from wrapper.

    @param [in] connectionId ID of the new connection.
*/

void WlanQtUtilsPrivate::addActiveConnection(uint connectionId)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_ADDACTIVECONNECTION_ENTRY, this);
    OstTrace1(
        TRACE_BORDER,
        WLANQTUTILSPRIVATE_ADDACTIVECONNECTION,
        "WlanQtUtilsPrivate::addActiveConnection;connectionId=%u",
        connectionId);

    Q_ASSERT(mConnection == NULL);
    mConnection = QSharedPointer<WlanQtUtilsConnection>(
        mConMonWrapper->connectionInfo(connectionId));

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_ADDACTIVECONNECTION_EXIT, this);
}

/*!
    Slot for updating active connection status from wrapper.

    @param [in] connectionId ID of the deleted connection.
*/

void WlanQtUtilsPrivate::removeActiveConnection(uint connectionId)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_REMOVEACTIVECONNECTION_ENTRY, this);
    OstTrace1(
        TRACE_BORDER,
        WLANQTUTILSPRIVATE_REMOVEACTIVECONNECTION,
        "WlanQtUtilsPrivate::removeActiveConnection;connectionId=%u",
        connectionId);

    Q_ASSERT(mConnection);
    if (mConnection->connectionId() == connectionId) {
        int closedIapId = mConnection->iapId();
        mConnection.clear();

        if (mConnectingIapId != closedIapId) {
            // Connection is closed, inform UI. wlanNetworkClosed is sent
            // from here instead of updateActiveConnection(closed),
            // because, in some cases, connection may be removed without
            // any connection status updates.
            // Note: reason parameter is not accurate here, because it is
            // only relevant for connections opened by this dll
            // (updateConnectionStatus)
            OstTraceExt2(
                TRACE_BORDER,
                DUP1_WLANQTUTILSPRIVATE_WLANNETWORKCLOSED,
                "WlanQtUtilsPrivate::emit wlanNetworkClosed;IAP ID=%d;status=%d",
                closedIapId,
                KErrNone);
            emit q_ptr->wlanNetworkClosed(closedIapId, KErrNone);
        }
        // else: connection creation started by thid dll, but creation failed
        // -> wlanNetworkClosed is sent from updateConnectionStatus
    }

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_REMOVEACTIVECONNECTION_EXIT, this);
}

/*!
    Slot for updating active connection status from wrapper.

    @param [in] connectionId ID of the updated connection.
    @param [in] connectionStatus New status of the connection.
*/

void WlanQtUtilsPrivate::updateActiveConnection(
    uint connectionId,
    WlanQtUtils::ConnStatus connectionStatus)
{
    OstTraceFunctionEntry1(WLANQTUTILSPRIVATE_UPDATEACTIVECONNECTION_ENTRY, this);
    OstTraceExt2(
        TRACE_BORDER,
        WLANQTUTILSPRIVATE_UPDATEACTIVECONNECTION,
        "WlanQtUtilsPrivate::updateActiveConnection;connectionId=%u;connectionStatus=%u",
        connectionId,
        (uint)connectionStatus);

    Q_ASSERT(mConnection);
    if (mConnection->connectionId() == connectionId
        && mConnection->connectionStatus() != connectionStatus) {
        // Update connection status and inform UI, if necessary
        mConnection->setConnectionStatus(connectionStatus);
        if (connectionStatus == WlanQtUtils::ConnStatusConnected) {
            OstTrace1(
                TRACE_BORDER,
                DUP1_WLANQTUTILSPRIVATE_WLANNETWORKOPENED,
                "WlanQtUtilsPrivate::emit wlanNetworkOpened;IAP ID=%d",
                mConnection->iapId());
            emit q_ptr->wlanNetworkOpened(mConnection->iapId());
        }
        // Do not inform UI about connection closing here. It is done in
        // removeActiveConnection(), because that may occur without any
        // connection status updates.
    } // else: connection status did not change

    OstTraceFunctionExit1(WLANQTUTILSPRIVATE_UPDATEACTIVECONNECTION_EXIT, this);
}
