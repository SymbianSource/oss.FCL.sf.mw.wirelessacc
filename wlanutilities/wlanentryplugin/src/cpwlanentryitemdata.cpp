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
* WLAN Entry plugin item data implementation.
*/

// System includes

#include <HbApplication>
#include <HbLabel>
#include <HbDataForm>

#ifdef WLANENTRYPLUGIN_SERVICETRACES    
#include <xqservicelog.h>
#endif
#include <xqappmgr.h>

#include <cpitemdatahelper.h>
#include <cpbasesettingview.h>

// User includes

#include "wlanstatusinfo.h"
#include "cpwlanentryitemdata.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "cpwlanentryitemdataTraces.h"
#endif

/*!
    \class CpWlanEntryItemData
    \brief Implementation of WLAN Status Plugin entry item data.

*/

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
    
    @param[in,out] itemDataHelper Control Panel item data helper object.
*/

CpWlanEntryItemData::CpWlanEntryItemData(CpItemDataHelper &itemDataHelper) :
    CpSettingFormEntryItemData(itemDataHelper, hbTrId("txt_occ_dblist_wireless_lan")),
    mWlanStatusInfo(new WlanStatusInfo(this))
{
    OstTraceFunctionEntry0(CPWLANENTRYITEMDATA_CPWLANENTRYITEMDATA_ENTRY);
    
    // Listen for WLAN status updates
    bool connectStatus = connect(
        mWlanStatusInfo,
        SIGNAL(statusUpdated()),
        this,
        SLOT(statusUpdate()));
    Q_ASSERT(connectStatus == true);
    
    // Update to show initial WLAN status.
    statusUpdate();
    
    OstTraceFunctionExit0(CPWLANENTRYITEMDATA_CPWLANENTRYITEMDATA_EXIT);
}

/*!
    Destructor.
*/

CpWlanEntryItemData::~CpWlanEntryItemData()
{
    OstTraceFunctionEntry0(DUP1_CPWLANENTRYITEMDATA_CPWLANENTRYITEMDATA_ENTRY);
    OstTraceFunctionExit0(DUP1_CPWLANENTRYITEMDATA_CPWLANENTRYITEMDATA_EXIT);
}

/*!
    Function for handling the entry item click.
*/

CpBaseSettingView *CpWlanEntryItemData::createSettingView() const
{
    OstTraceFunctionEntry0(CPWLANENTRYITEMDATA_CREATESETTINGVIEW_ENTRY);
    
#ifdef WLANENTRYPLUGIN_SERVICETRACES    
    qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
    XQSERVICE_DEBUG_PRINT("CpWlanEntryItemData::createSettingView requesting listView()");
#endif

    // Execute synchronous WLAN Sniffer list view
    // TODO: Start using defines in xqaiwdeclplat.h when available
    XQApplicationManager aiwMgr;
    XQAiwRequest *request = aiwMgr.create(
        "wlansniffer",
        "com.nokia.symbian.IWlanSniffer",
        "listView()",
        true);

    // The WLAN Sniffer service must always exist
    Q_ASSERT(request);

    // The service is synchronous & embedded
    request->setSynchronous(true);
    
    // Window title needs to be set to "Control Panel"
    XQRequestInfo reqInfo;
    // TODO: Start using the official define when available
    reqInfo.setInfo("WindowTitle", hbTrId("txt_cp_title_control_panel"));
    request->setInfo(reqInfo);
    
    bool status = request->send();
#ifdef WLANENTRYPLUGIN_SERVICETRACES    
    XQSERVICE_DEBUG_PRINT("CpWlanEntryItemData::createSettingView listView() service request completed");
#endif    
    Q_ASSERT(status);
    delete request;

    OstTraceFunctionExit0(CPWLANENTRYITEMDATA_CREATESETTINGVIEW_EXIT);
    return 0;
}

/*!
    Slot for updating the status shown by the plugin.
    Updates both the text and icon. 
 */
void CpWlanEntryItemData::statusUpdate()
{
    OstTraceFunctionEntry0(CPWLANENTRYITEMDATA_STATUSUPDATE_ENTRY);
    
    // Build icon with (possible) badge
    HbIcon wlanIcon;
    switch (mWlanStatusInfo->status()) {
    case WlanStatusInfo::WlanStatusIdle:
        wlanIcon = HbIcon("qtg_large_wlan");
        break;
        
    case WlanStatusInfo::WlanStatusOff:
        wlanIcon = HbIcon("qtg_large_wlan_off");
        break;
        
    case WlanStatusInfo::WlanStatusConnected:
        wlanIcon = HbIcon("qtg_large_wlan");
        wlanIcon.addBadge(
            Qt::AlignBottom | Qt::AlignRight,
            HbIcon("qtg_small_connection"));
        break;
        
#ifndef QT_NO_DEBUG
    default:
        // Unsupported status type detected
        Q_ASSERT(0);
        break;
#endif        
    }
    
    // Set the updated WLAN status
    this->setEntryItemIcon(wlanIcon);
    this->setDescription(mWlanStatusInfo->statusText());
    
    OstTraceFunctionExit0(CPWLANENTRYITEMDATA_STATUSUPDATE_EXIT);
}
