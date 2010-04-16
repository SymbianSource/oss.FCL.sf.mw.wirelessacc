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

// System includes

#include <HbGlobal>

// User includes

#include "wlanqtutils.h"
#include "wlanqtutilswlaniap.h"

#include "wlanstatusinfo.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanstatusinfoTraces.h"
#endif

/*!
    \class WlanStatusInfo
    \brief Class for maintaining and updating the WLAN status for the WLAN 
    Status Control Panel Plugin.

*/

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
*/

WlanStatusInfo::WlanStatusInfo(QObject *parent) :
    QObject(parent),
    mWlanQtUtils(new WlanQtUtils()),
    mStatusText(""),
    mStatus(WlanStatusOff)
{
    OstTraceFunctionEntry1(WLANSTATUSINFO_WLANSTATUSINFO_ENTRY, this);
    
    // Listen for WLAN ON/OFF switching
    bool connectStatus = connect(
        mWlanQtUtils,
        SIGNAL(masterWlanStatus(bool)),
        this,
        SLOT(updateStatus()));
    Q_ASSERT(connectStatus == true);

    // Listen for WLAN connection statuses
    connectStatus = connect(
        mWlanQtUtils,
        SIGNAL(wlanNetworkOpened(int)),
        this,
        SLOT(updateStatus()));
    Q_ASSERT(connectStatus == true);
    connectStatus = connect(
        mWlanQtUtils,
        SIGNAL(wlanNetworkClosed(int)),
        this,
        SLOT(updateStatus()));
    Q_ASSERT(connectStatus == true);
    
    // Set initial status
    updateStatus();
    
    OstTraceFunctionExit1(WLANSTATUSINFO_WLANSTATUSINFO_EXIT, this);
}

/*!
    Destructor.
*/

WlanStatusInfo::~WlanStatusInfo()
{
    OstTraceFunctionEntry1(DUP1_WLANSTATUSINFO_WLANSTATUSINFO_ENTRY, this);
    
    delete mWlanQtUtils;
    
    OstTraceFunctionExit1(DUP1_WLANSTATUSINFO_WLANSTATUSINFO_EXIT, this);
}

/*!
    Function for getting current WLAN status value (WlanStatusInfo::WlanStatus*).
*/

int WlanStatusInfo::status()
{
    OstTraceFunctionEntry1(WLANSTATUSINFO_STATUS_ENTRY, this);    
    OstTraceFunctionExit1(WLANSTATUSINFO_STATUS_EXIT, this);
    return mStatus;
}

/*!
    Returns the current WLAN status text.
*/

QString WlanStatusInfo::statusText()
{
    OstTraceFunctionEntry1(WLANSTATUSINFO_STATUSTEXT_ENTRY, this);
    OstTraceFunctionExit1(WLANSTATUSINFO_STATUSTEXT_EXIT, this);
    return mStatusText;
}

/*!
    Slot used for updating the WLAN status.
*/

void WlanStatusInfo::updateStatus()
{
    OstTraceFunctionEntry1(WLANSTATUSINFO_UPDATESTATUS_ENTRY, this);

    // Backup old status to detect changes
    QString oldStatusText = mStatusText;
    int connectedIapId = mWlanQtUtils->connectedWlanId();
    
    // Figure out current WLAN status
    if (mWlanQtUtils->masterWlan() == false) {
        // WLAN is switched OFF.
        mStatus = WlanStatusOff;
        mStatusText = hbTrId("txt_occ_dblist_wireless_lan_val_off");
    } else if (connectedIapId != WlanQtUtilsInvalidIapId) {
        mStatus = WlanStatusConnected;
        mStatusText = hbTrId("txt_occ_dblist_wireless_lan_val_connected_to_1")
            .arg(mWlanQtUtils->iap(connectedIapId)->name());
    } else {
        mStatus = WlanStatusIdle;
        mStatusText = hbTrId("txt_occ_dblist_wireless_lan_val_wlan_is_on");
    }

    // Inform about update if the status really changed.
    if (oldStatusText != mStatusText) {
        emit statusUpdated();
    }
    
    OstTraceFunctionExit1(WLANSTATUSINFO_UPDATESTATUS_EXIT, this);
}
