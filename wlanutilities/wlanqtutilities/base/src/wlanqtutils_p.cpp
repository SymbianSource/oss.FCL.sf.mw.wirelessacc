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
#include "wlanqtutilsactiveconn.h"
#include "wlanqtutilscmmwrapper.h"
#include "wlanqtutilsconmonwrapper.h"
#include "wlanqtutilsesockwrapper.h"
#include "wlanqtutilsconntestwrapper.h"

#include "wlanqtutils.h"
#include "wlanqtutils_p.h"

#ifdef WLANQTUTILS_NO_OST_TRACES_FLAG
#include <opensystemtrace.h>
#else
#include "OstTraceDefinitions.h"
#endif
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutils_pTraces.h"
#endif


WlanQtUtilsPrivate::WlanQtUtilsPrivate(WlanQtUtils *publicPtr) :
    q_ptr(publicPtr),
    wlanScanList_(),
    wlanIapList_(),
    toBeTestedIapId_(WlanQtUtilsInvalidIapId),
    connectingIapId_(WlanQtUtilsInvalidIapId),
    activeConnection_(NULL)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_WLANQTUTILSPRIVATE_ENTRY, this );
    
    // Let's allocate memory for wrapper objects.
    // Engine is set as a parent for wrapper objects.
    cmmWrapper_ = new CmmWrapper(this);
    conMonWrapper_ = new ConMonWrapper(this);
    esockWrapper_ = new EsockWrapper(this);
    connTestWrapper_ = new ConnTestWrapper(this);
    
    // Make all connections.
    // Todo: having these as signals from the wrappers doesn't bring much benefit
    // -> could be optimized as normal callbacks
    connect(
        conMonWrapper_,
        SIGNAL(availableWlanApsFromWrapper(QList<WlanQtUtilsWlanAp *>&)),
        this,
        SLOT(updateAvailableWlanAps(QList<WlanQtUtilsWlanAp *>&)));

    connect(
        esockWrapper_,
        SIGNAL(connectionStatusFromWrapper(bool)),
        this,
        SLOT(updateConnectionStatus(bool)));

    connect(
        connTestWrapper_,
        SIGNAL(connectivityTestResult(bool)),
        this,
        SLOT(updateConnectivityTestResult(bool)));

    connect(
        conMonWrapper_,
        SIGNAL(connCreatedEventFromWrapper(uint)),
        this,
        SLOT(addActiveConnection(uint)));

    connect(
        conMonWrapper_,
        SIGNAL(connDeletedEventFromWrapper(uint)),
        this,
        SLOT(removeActiveConnection(uint)));
    
    connect(
        conMonWrapper_,
        SIGNAL(connStatusEventFromWrapper(uint, WlanQtUtilsConnectionStatus)),
        this,
        SLOT(updateActiveConnection(uint, WlanQtUtilsConnectionStatus)));

    // Fetch the initial IAP configuration
    fetchIaps();

    // Retrieve initial status of active connections and update IAPs, if needed.
    activeConnection_ = conMonWrapper_->activeConnection();
    if (activeConnection_ != NULL) {
        updateIapConnectionStatuses(activeConnection_->iapId(), activeConnection_->connectionStatus());
    }

    // TODO: error handling

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_WLANQTUTILSPRIVATE_EXIT, this );
}

WlanQtUtilsPrivate::~WlanQtUtilsPrivate()
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_WLANQTUTILSPRIVATEDESTR_ENTRY, this );
    
    // Delete WLAN scan list
    for (int i = 0; i < wlanScanList_.count(); i++) {
        delete wlanScanList_[i];
    }
    wlanScanList_.clear();
    
    delete activeConnection_;
    
    delete cmmWrapper_;
    delete conMonWrapper_;
    delete esockWrapper_;
    delete connTestWrapper_;

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_WLANQTUTILSPRIVATEDESTR_EXIT, this );
}

void WlanQtUtilsPrivate::availableWlanAps(
    QList<WlanQtUtilsWlanIap *> &wlanIapList,
    QList<WlanQtUtilsWlanAp *> &wlanApList)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_AVAILABLEWLANAPS_ENTRY, this );
    
    wlanIapList.clear();
    wlanApList.clear();
    
    // Add possible connected or connecting IAP as the first item in the list
    int activeIapId = WlanQtUtilsInvalidIapId;
    if (activeConnection_ != NULL) {
        activeIapId = activeConnection_->iapId();
    } else if (connectingIapId_ != WlanQtUtilsInvalidIapId) {
        activeIapId = connectingIapId_;
    }
    if (wlanIapList_.contains(activeIapId)) {
        wlanIapList_[activeIapId]->setSignalStrength(WlanQtUtilsWlanSignalStrengthMax);
        wlanIapList_[activeIapId]->setConnectionStatus(WlanQtUtilsConnectionStatusConnected);
        wlanIapList.append(wlanIapList_[activeIapId]);
    }
    
    // Todo: IAPs with same SSID and security mode? probably we want to show only one of them?
    
    // Match IAPs against WLAN scan results
    foreach (WlanQtUtilsWlanIap *iap, wlanIapList_) {
        foreach (WlanQtUtilsWlanAp *scanAp, wlanScanList_) {
            // Todo: security mode check
            if (iap->ssid() == scanAp->ssid()) {
                // IAP found, add it to caller's list of known IAPs (signal strength needs to be
                // updated manually since the IAP in our list does not have that information yet)
                iap->setSignalStrength(scanAp->signalStrength());
                if (iap->id() != activeIapId) {
                    wlanIapList.append(iap);
                } // else: connected IAP, added as the first item in the list already
                break;
            }
        }
    }

    // Go through the scan results to find unknown APs
    for (int i = 0; i < wlanScanList_.count(); i++) {
        // Skip networks with empty SSID (hidden networks are handled by UI currently)
        if (wlanScanList_[i]->ssid().isEmpty() == FALSE) {
            // Check whether an IAP with these parameters exists in any SNAP (in which case this
            // network should not be shown as available in this SNAP)
            if (wlanIapExists(wlanScanList_[i]->ssid(), wlanScanList_[i]->securityMode()) == false) {
                // No IAP found in any SNAP, copy the AP to caller's list of unknown APs
                wlanApList.append(wlanScanList_[i]);
            }
        }
    }
    
    // Sort APs by their SSIDs.
    QMap<QString, WlanQtUtilsWlanAp *> wlanApMap;
    // Loop the AP list copying the keys (lower case SSID) and elements
    // into the map.
    for (int i = 0; i < wlanApList.count(); i++ ) {
        wlanApMap.insertMulti(wlanApList[i]->ssid().toLower(), wlanApList[i]);
    }
    wlanApList = wlanApMap.values();

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_AVAILABLEWLANAPS_EXIT, this );
}

int WlanQtUtilsPrivate::createWlanIap(const WlanQtUtilsWlanAp *wlanAp)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_CREATEWLANIAP_ENTRY, this );
    
    // Create the new IAP. Store its ID, because we need to run ICT for it later
    WlanQtUtilsWlanIap *newIap = cmmWrapper_->createWlanIap(wlanAp);
    toBeTestedIapId_ = newIap->id();
    wlanIapList_.insert(newIap->id(), newIap);
    
    OstTrace1(
        TRACE_API,
        WLANQTUTILSPRIVATE_CREATEWLANIAP,
        "WlanQtUtilsPrivate::createWlanIap;New IAP ID=%d", toBeTestedIapId_ );
    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_CREATEWLANIAP_EXIT, this );
    return toBeTestedIapId_;
}

void WlanQtUtilsPrivate::connectIap(int iapId)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_CONNECTIAP_ENTRY, this );
    OstTrace1( TRACE_API, WLANQTUTILSPRIVATE_CONNECTIAP, "WlanQtUtilsPrivate::connectIap;IAP ID=%d", iapId );
    
    Q_ASSERT(activeConnection_ == NULL);
    connectingIapId_ = iapId;
    esockWrapper_->connectIap(iapId);

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_CONNECTIAP_EXIT, this );
}

void WlanQtUtilsPrivate::disconnectIap(int iapId)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_DISCONNECTIAP_ENTRY, this );
    OstTrace1( TRACE_API, WLANQTUTILSPRIVATE_DISCONNECTIAP, "WlanQtUtilsPrivate::disconnectIap;IAP ID=%d", iapId );
    
    Q_ASSERT(activeConnection_ != NULL);
    Q_ASSERT(activeConnection_->iapId() == iapId);
    Q_ASSERT(wlanIapList_.contains(iapId));
    
    // Todo: IAP may have been opened by someone else... how to know when to disconnect RConnection?    
    esockWrapper_->disconnectIap();
    
    // In order to close connection even if there are other users for the IAP, close also via ConMon
    conMonWrapper_->disconnectIap(iapId);
    // Change status of the IAP to non-connected
    wlanIapList_[iapId]->setConnectionStatus(WlanQtUtilsConnectionStatusDisconnected);

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_DISCONNECTIAP_EXIT, this );
}

WlanQtUtilsIap *WlanQtUtilsPrivate::iap(int iapId) const
{
    if (wlanIapList_.contains(iapId)) {
        return wlanIapList_.value(iapId);
    }
    // else: no match
    return NULL;
}

bool WlanQtUtilsPrivate::masterWlan() const
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_MASTERWLAN_ENTRY, this );
    
    // TODO: Add actual implementation, this is just temporary dummy for testing wlanentryplugin!

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_MASTERWLAN_EXIT, this );
    return true;
}

void WlanQtUtilsPrivate::setMasterWlan(bool enabled)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_SETMASTERWLAN_ENTRY, this );
    
    // TODO: Add actual implementation.
    (void)enabled;

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_SETMASTERWLAN_EXIT, this );
}

int WlanQtUtilsPrivate::connectedWlanId() const
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_CONNECTEDWLANID_ENTRY, this );

    int retVal = WlanQtUtilsInvalidIapId;
    if (activeConnection_ != NULL) {
        retVal = activeConnection_->iapId();
    }
    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_CONNECTEDWLANID_EXIT, this );
    return retVal;
}

void WlanQtUtilsPrivate::scanWlans()
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_SCANWLANS_ENTRY, this );

    // Just forward the request to wrapper, which triggers a single WLAN scan
    conMonWrapper_->scanAvailableWlanAPs();

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_SCANWLANS_EXIT, this );
}

void WlanQtUtilsPrivate::updateAvailableWlanAps(QList<WlanQtUtilsWlanAp *> &availableWlanList)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_UPDATEAVAILABLEWLANAPS_ENTRY, this );
    
    // Old results are removed
    wlanScanList_.clear();
    // Copy available WLAN's to engine's scan result list (duplicates are removed)
    for (int i = 0; i < availableWlanList.count(); i++) {
        bool duplicate = false;
        for (int j = 0; j < wlanScanList_.count(); j++) {
            if (availableWlanList[i]->ssid() == wlanScanList_[j]->ssid()
                && availableWlanList[i]->securityMode() == wlanScanList_[j]->securityMode()) {
                duplicate = true;
                break;
            }
        }
        if (duplicate == false) {
            wlanScanList_.append(availableWlanList[i]);
        }
        // Todo: else deallocate?
    }

    // the information is forwarded to the UI
    emit q_ptr->wlanScanReady();

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_UPDATEAVAILABLEWLANAPS_EXIT, this );
}

void WlanQtUtilsPrivate::updateConnectionStatus(bool isOpened)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_UPDATECONNECTIONSTATUS_ENTRY, this );    
    OstTrace1(
        TRACE_API,
        WLANQTUTILSPRIVATE_UPDATECONNECTIONSTATUS,
        "WlanQtUtilsPrivate::updateConnectionStatus;isOpened=%d", isOpened );
    
    if (isOpened == false) {
        // Opening failed, update connection statuses, but don't inform UI about
        // connection closing here. It is done always in removeActiveConnection().
        if (activeConnection_ != NULL) {
            activeConnection_->setConnectionStatus(WlanQtUtilsConnectionStatusDisconnected);
        }
        updateIapConnectionStatuses(connectingIapId_, WlanQtUtilsConnectionStatusDisconnected);
    } else {
        // Opening succeeded, update connection statuses and inform UI
        if (activeConnection_ != NULL) {
            activeConnection_->setConnectionStatus(WlanQtUtilsConnectionStatusConnected);
        }
        updateIapConnectionStatuses(connectingIapId_, WlanQtUtilsConnectionStatusConnected);
        emit q_ptr->wlanNetworkOpened(connectingIapId_);

        // Start ICT, if needed
        if (connectingIapId_ == toBeTestedIapId_) {
            WlanQtUtilsIap *iap = WlanQtUtilsPrivate::iap(toBeTestedIapId_);
            connTestWrapper_->startConnectivityTest(toBeTestedIapId_, iap->networkId());
        }
    }
    // IAP is no more in connecting state
    connectingIapId_ = WlanQtUtilsInvalidIapId;

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_UPDATECONNECTIONSTATUS_EXIT, this );
}

void WlanQtUtilsPrivate::updateConnectivityTestResult(bool result)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_UPDATECONNECTIVITYTESTRESULT_ENTRY, this );
    OstTrace1(
        TRACE_API,
        WLANQTUTILSPRIVATE_UPDATECONNECTIVITYTESTRESULT,
        "WlanQtUtilsPrivate::updateConnectivityTestResult;result=%d", result );
    
    if (result == true) {
        // Move the tested IAP to Internet SNAP since the test passed
        Q_ASSERT(toBeTestedIapId_ != WlanQtUtilsInvalidIapId);
        cmmWrapper_->moveIapToInternetSnap(toBeTestedIapId_);
    } // else: do nothing, IAP remains to be uncategorized
    else
    {
        // TODO: Remove this. This is needed now since the connectivity test fails
        // and thus the IAP is not shown correctly.
        cmmWrapper_->moveIapToInternetSnap(toBeTestedIapId_);

    }
    // This IAP is now tested
    toBeTestedIapId_ = WlanQtUtilsInvalidIapId;

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_UPDATECONNECTIVITYTESTRESULT_EXIT, this );
}

void WlanQtUtilsPrivate::addActiveConnection(uint connectionId)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_ADDACTIVECONNECTION_ENTRY, this );
    OstTrace1(
        TRACE_API,
        WLANQTUTILSPRIVATE_ADDACTIVECONNECTION,
        "WlanQtUtilsPrivate::addActiveConnection;connectionId=%u", connectionId );

    Q_ASSERT(activeConnection_ == NULL);
    activeConnection_ = conMonWrapper_->connectionInfo(connectionId);
    Q_ASSERT(wlanIapList_.contains(activeConnection_->iapId()));
    updateIapConnectionStatuses(activeConnection_->iapId(), activeConnection_->connectionStatus());

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_ADDACTIVECONNECTION_EXIT, this );
}

void WlanQtUtilsPrivate::removeActiveConnection(uint connectionId)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_REMOVEACTIVECONNECTION_ENTRY, this );
    OstTrace1(
        TRACE_API,
        WLANQTUTILSPRIVATE_REMOVEACTIVECONNECTION,
        "WlanQtUtilsPrivate::removeActiveConnection;connectionId=%u", connectionId );
    
    Q_ASSERT(activeConnection_ != NULL);
    if (activeConnection_->connectionId() == connectionId) {
        // Connection is closed, update connection statuses and inform UI.
        // wlanNetworkClosed is sent from here (and only from here), because, in some cases,
        // connection may be removed without any connection status updates.
        int closedIapId = activeConnection_->iapId();
        updateIapConnectionStatuses(activeConnection_->iapId(), WlanQtUtilsConnectionStatusDisconnected);
        delete activeConnection_;
        activeConnection_ = NULL;
        emit q_ptr->wlanNetworkClosed(closedIapId);
    }

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_REMOVEACTIVECONNECTION_EXIT, this );
}

void WlanQtUtilsPrivate::updateActiveConnection(uint connectionId, WlanQtUtilsConnectionStatus connectionStatus)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_UPDATEACTIVECONNECTION_ENTRY, this );
    OstTraceExt2(
        TRACE_API,
        WLANQTUTILSPRIVATE_UPDATEACTIVECONNECTION,
        "WlanQtUtilsPrivate::updateActiveConnection;connectionId=%u;connectionStatus=%u", connectionId, connectionStatus );
    
    Q_ASSERT(activeConnection_ != NULL);
    if (activeConnection_->connectionId() == connectionId
        && activeConnection_->connectionStatus() != connectionStatus) {
        // Update connection statuses and inform UI, if necessary
        activeConnection_->setConnectionStatus(connectionStatus);
        updateIapConnectionStatuses(activeConnection_->iapId(), connectionStatus);
        if (connectionStatus == WlanQtUtilsConnectionStatusConnected) {
            emit q_ptr->wlanNetworkOpened(activeConnection_->iapId());
        }
        // Do not inform UI about connection closing here. It is done always in
        // removeActiveConnection(), because that may occur without any connection status updates.
    } // else: connection status did not change

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_UPDATEACTIVECONNECTION_EXIT, this );
}

int WlanQtUtilsPrivate::fetchIaps()
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_FETCHIAPS_ENTRY, this );
    
    QList<WlanQtUtilsIap *> list;
    int error = 0; 
    // Read the IAP list and store WLAN IAPs in our internal map data structure
    error = cmmWrapper_->fetchIaps(list);    
    foreach (WlanQtUtilsIap *iap, list) {
        if (iap->bearerType() == WlanQtUtilsBearerTypeWlan) {
            // Notice that insertMulti not used, because IAP IDs should be unique
            wlanIapList_.insert(iap->id(), qobject_cast<WlanQtUtilsWlanIap *>(iap));            
        }
    }
        
    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_FETCHIAPS_EXIT, this );
    return error;
}

bool WlanQtUtilsPrivate::wlanIapExists(QString ssid, WlanQtUtilsWlanSecMode secMode)
{    
    foreach (WlanQtUtilsWlanIap *iap, wlanIapList_) {
        // todo: secmode
        (void)secMode;
        if (iap->ssid() == ssid) {
            return true;
        }
    }
    
    return false;
}

void WlanQtUtilsPrivate::updateIapConnectionStatuses(int iapId, WlanQtUtilsConnectionStatus status)
{
    OstTraceFunctionEntry1( WLANQTUTILSPRIVATE_UPDATEIAPCONNECTIONSTATUSES_ENTRY, this );
    
    const WlanQtUtilsWlanIap *referenceIap = qobject_cast<WlanQtUtilsWlanIap *>(iap(iapId));
    foreach (WlanQtUtilsWlanIap *iap, wlanIapList_) {
        // todo: security mode checking
        if (iap->id() == referenceIap->id() || iap->ssid() == referenceIap->ssid()) {
            iap->setConnectionStatus(status);
        }
    }

    OstTraceFunctionExit1( WLANQTUTILSPRIVATE_UPDATEIAPCONNECTIONSTATUSES_EXIT, this );
}
