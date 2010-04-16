/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

// System includes

#ifdef WLANENTRYPLUGIN_SERVICETRACES    
#include <xqservicelog.h>
#endif
#include <xqservicerequest.h>

#include <HbLabel>
#include <HbDataForm>

#include <cpitemdatahelper.h>
#include <cpbasesettingview.h>

// User includes

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "cpwlanentryitemdataTraces.h"
#endif

#include "wlanstatusinfo.h"

#include "cpwlanentryitemdata.h"

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
*/

CpWlanEntryItemData::CpWlanEntryItemData(CpItemDataHelper &itemDataHelper) :
    CpSettingFormEntryItemData(itemDataHelper, hbTrId("txt_occ_dblist_wireless_lan")),
    mWlanStatusInfo(new WlanStatusInfo(this))
{
    OstTraceFunctionEntry1(CPWLANENTRYITEMDATA_CPWLANENTRYITEMDATA_ENTRY, this);
    
    // Listen for WLAN status updates
    bool connectStatus = connect(
        mWlanStatusInfo,
        SIGNAL(statusUpdated()),
        this,
        SLOT(statusUpdate()));
    Q_ASSERT(connectStatus == true);
    
    // Update to show initial WLAN status.
    statusUpdate();
    
    OstTraceFunctionExit1(CPWLANENTRYITEMDATA_CPWLANENTRYITEMDATA_EXIT, this);
}

/*!
    Destructor.
*/

CpWlanEntryItemData::~CpWlanEntryItemData()
{
    OstTraceFunctionEntry1(DUP1_CPWLANENTRYITEMDATA_CPWLANENTRYITEMDATA_ENTRY, this);
    OstTraceFunctionExit1(DUP1_CPWLANENTRYITEMDATA_CPWLANENTRYITEMDATA_EXIT, this);
}

/*!
    Function for handling the entry item click.
*/

CpBaseSettingView *CpWlanEntryItemData::createSettingView() const
{
    OstTraceFunctionEntry1(CPWLANENTRYITEMDATA_CREATESETTINGVIEW_ENTRY, this);
    
#ifdef WLANENTRYPLUGIN_SERVICETRACES    
    qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
    XQSERVICE_DEBUG_PRINT("CpWlanEntryItemData::createSettingView requesting listView()");
#endif

    // Execute synchronous WLAN Sniffer list view
    XQServiceRequest* snd = new XQServiceRequest("com.nokia.services.wlansniffer.list","listView()",true);
    bool status = snd->send();
#ifdef WLANENTRYPLUGIN_SERVICETRACES    
    XQSERVICE_DEBUG_PRINT("CpWlanEntryItemData::createSettingView listView() service request completed");
#endif    
    Q_ASSERT(status);
    delete snd;

    OstTraceFunctionExit1(CPWLANENTRYITEMDATA_CREATESETTINGVIEW_EXIT, this);
    return 0;
}

/*!
    Slot for updating the status shown by the plugin.
    Updates both the text and icon. 
 */
void CpWlanEntryItemData::statusUpdate()
{
    OstTraceFunctionEntry1(CPWLANENTRYITEMDATA_STATUSUPDATE_ENTRY, this);
    
    // Build icon with (possible) badge
    HbIcon wlanIcon("qtg_large_wlan");
    switch (mWlanStatusInfo->status()) {
    case WlanStatusInfo::WlanStatusOff:
#ifdef WLANSTATUSICONBADGING                                // TODO: Remove flagging when icon badging is supported       
        wlanIcon.addBadge(
            Qt::AlignTop | Qt::AlignRight,
            HbIcon("pri_small_super_off"));
#endif        
        break;
        
    case WlanStatusInfo::WlanStatusConnected:
#ifdef WLANSTATUSICONBADGING                                // TODO: Remove flagging when icon badging is supported       
        wlanIcon.addBadge(
            Qt::AlignTop | Qt::AlignRight,
            HbIcon("qtg_small_online"));
#endif        
        break;
    }
    
    // Set the updated WLAN status
    this->setEntryItemIcon(wlanIcon);
    this->setDescription(mWlanStatusInfo->statusText());
    
    OstTraceFunctionExit1(CPWLANENTRYITEMDATA_STATUSUPDATE_EXIT, this);
}
