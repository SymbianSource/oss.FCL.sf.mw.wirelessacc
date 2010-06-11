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
* WLAN Qt Utilities WLAN IAP settings handling.
*/

// System includes

#include <QScopedPointer>
#include <QSharedPointer>
#include <QVariant>

#include <cmmanager_shim.h>
#include <cmdestination_shim.h>
#include <cmconnectionmethod_shim.h>

// User includes

#include "wlanqtutils.h"
#include "wlanqtutilsap.h"
#include "wlanqtutilsiap.h"
#include "wlanqtutilsiapsettings.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutilsiapsettingsTraces.h"
#endif

/*!
    \class WlanQtUtilsIapSettings
    \brief WLAN IAP related settings handler.

    Provides functionality to manipulate WLAN IAPs via the CM Manager Shim
    interface.
    
    NOTE: Shim functions may throw exceptions that need to be catched in
    this class.
*/

// External function prototypes

// Local constants

// WEP key lengths used to determine key format
static const int WepHex64BitMaxLength = 10;
static const int WepHex128BitMaxLength = 26;
static const int WepAscii64BitMaxLength = 5;
static const int WepAscii128BitMaxLength = 13;

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
    
    @param [in] parent Parent object.
*/

WlanQtUtilsIapSettings::WlanQtUtilsIapSettings(QObject *parent) :
    QObject(parent),
    mCmManager(new CmManagerShim())
{
    OstTraceFunctionEntry0(WLANQTUTILSIAPSETTINGS_WLANQTUTILSIAPSETTINGS_ENTRY);
    OstTraceFunctionExit0(WLANQTUTILSIAPSETTINGS_WLANQTUTILSIAPSETTINGS_EXIT);
}

/*!
    Destructor.
*/

WlanQtUtilsIapSettings::~WlanQtUtilsIapSettings()
{
    OstTraceFunctionEntry0(DUP1_WLANQTUTILSIAPSETTINGS_WLANQTUTILSIAPSETTINGS_ENTRY);
    
    delete mCmManager;
    
    OstTraceFunctionExit0(DUP1_WLANQTUTILSIAPSETTINGS_WLANQTUTILSIAPSETTINGS_EXIT);
}

/*!
    Fetch all WLAN IAPs.
    
    @param [out] iapList List of WLAN IAPs.
*/

void WlanQtUtilsIapSettings::fetchIaps(
    QList< QSharedPointer<WlanQtUtilsIap> > &iapList) const
{
    OstTraceFunctionEntry0(WLANQTUTILSIAPSETTINGS_FETCHIAPS_ENTRY);
    
    // Clear the list content first for safety
    iapList.clear();
    
    QList<uint> iapIds;
    try {
        mCmManager->connectionMethod(iapIds, false);
    } catch (const std::exception &ex) {
        int error = qt_symbian_exception2Error(ex);
        OstTrace1(
            TRACE_NORMAL,
            WLANQTUTILSIAPSETTINGS_FETCHIAPS_EXCEPTION,
            "WlanQtUtilsIapSettings::fetchIaps exception;error=%d",
            error);
    }

    foreach (int iapId, iapIds) {
        QSharedPointer<WlanQtUtilsIap> wlanIap = fetchIap(iapId);
        if (wlanIap) {
            iapList.append(wlanIap);
        }
    }
    
    OstTraceFunctionExit0(WLANQTUTILSIAPSETTINGS_FETCHIAPS_EXIT);
}

/*!
    Fetch WLAN IAP with the given ID
    
    @param [in] iapId ID of IAP to fetch.
    
    @return Found IAP, NULL if not found.
*/

QSharedPointer<WlanQtUtilsIap> WlanQtUtilsIapSettings::fetchIap(int iapId) const
{
    OstTraceFunctionEntry0(WLANQTUTILSIAPSETTINGS_FETCHIAP_ENTRY);
    
    OstTrace1(
        TRACE_NORMAL,
        WLANQTUTILSIAPSETTINGS_FETCHIAP,
        "WlanQtUtilsIapSettings::fetchIap;iapId=%d",
        iapId);
    
    QSharedPointer<WlanQtUtilsIap> wlanIap;
    try {
        QScopedPointer<CmConnectionMethodShim> iap(
            mCmManager->connectionMethod(iapId));
        if (iap && iap->getIntAttribute(CMManagerShim::CmBearerType) ==
            KUidWlanBearerType) {

            // Get WLAN IAP parameters
            int netId = iap->getIntAttribute(CMManagerShim::CmNetworkId);
            QString name = iap->getStringAttribute(CMManagerShim::CmName);
            QString ssid = iap->getStringAttribute(CMManagerShim::WlanSSID);
            int secMode = iap->getIntAttribute(
                CMManagerShim::WlanSecurityMode);
            bool wpaPskUse = iap->getBoolAttribute(
                CMManagerShim::WlanEnableWpaPsk);
    
            // Create a WLAN Qt Utils IAP
            wlanIap = QSharedPointer<WlanQtUtilsIap>(new WlanQtUtilsIap());
            wlanIap->setValue(WlanQtUtilsIap::ConfIdIapId, iapId);
            wlanIap->setValue(WlanQtUtilsIap::ConfIdNetworkId, netId);
            wlanIap->setValue(WlanQtUtilsIap::ConfIdName, name);
            wlanIap->setValue(WlanQtUtilsAp::ConfIdSsid, ssid);
            wlanIap->setValue(WlanQtUtilsAp::ConfIdSecurityMode, secMode);
            wlanIap->setValue(WlanQtUtilsAp::ConfIdWpaPskUse, wpaPskUse);
        }
    } catch (const std::exception &ex) {
        int error = qt_symbian_exception2Error(ex);
        OstTrace1(
            TRACE_NORMAL,
            WLANQTUTILSIAPSETTINGS_FETCHIAP_EXCEPTION,
            "WlanQtUtilsIapSettings::fetchIap exception;error=%d",
            error);
    }
    
    OstTraceFunctionExit0(WLANQTUTILSIAPSETTINGS_FETCHIAP_EXIT);
    return wlanIap;
}

/*!
    Create a new WLAN IAP as an uncategorized IAP.
    
    @param [in] wlanAp Information about the WLAN AP.
    
    @return ID of the created IAP, IapIdNone in error cases.
*/

int WlanQtUtilsIapSettings::createIap(const WlanQtUtilsAp *wlanAp)
{
    OstTraceFunctionEntry0(WLANQTUTILSIAPSETTINGS_CREATEIAP_ENTRY);
    
    int iapId = WlanQtUtils::IapIdNone;
    
    try {
        // Create the new IAP
        QScopedPointer<CmConnectionMethodShim> iap(
            mCmManager->createConnectionMethod(KUidWlanBearerType));
        storeSettings(wlanAp, iap.data());
        iapId = iap->getIntAttribute(CMManagerShim::CmIapId);
    } catch (const std::exception &ex) {
        // Trace error cause and return failure (default value) to caller.
        int error = qt_symbian_exception2Error(ex);
        OstTrace1(
            TRACE_NORMAL,
            WLANQTUTILSIAPSETTINGS_CREATEIAP_EXCEPTION,
            "WlanQtUtilsIapSettings::createIap exception;error=%d",
            error);
    }
    
    OstTraceFunctionExit0(WLANQTUTILSIAPSETTINGS_CREATEIAP_EXIT);
    return iapId;
}

/*!
    Update the WLAN IAP given as parameter. All settings are stored to
    database (again) without checking whether they have actually changed
    or not.
    
    @param [in] iapId ID of IAP to update.
    @param [in] wlanAp Information about the WLAN AP.
    
    @return Was update succesful or not?
*/

bool WlanQtUtilsIapSettings::updateIap(
    int iapId,
    const WlanQtUtilsAp *wlanAp)
{
    OstTraceFunctionEntry0(WLANQTUTILSIAPSETTINGS_UPDATEIAP_ENTRY);
    
    bool success = false;
    
    try {
        QScopedPointer<CmConnectionMethodShim> iap(
            mCmManager->connectionMethod(iapId));
        storeSettings(wlanAp, iap.data());
        success = true;
    } catch (const std::exception &ex) {
        // Trace error cause and return failure (default value) to caller.
        int error = qt_symbian_exception2Error(ex);
        OstTrace1(
            TRACE_NORMAL,
            WLANQTUTILSIAPSETTINGS_UPDATEIAP_EXCEPTION,
            "WlanQtUtilsIapSettings::updateIap exception;error=%d",
            error);
    }
    
    OstTraceFunctionExit0(WLANQTUTILSIAPSETTINGS_UPDATEIAP_EXIT);
    return success;
}

/*!
    Delete the WLAN IAP given as parameter.
    
    @param [in] iapId ID of IAP to delete.
*/

void WlanQtUtilsIapSettings::deleteIap(int iapId)
{
    OstTraceFunctionEntry0(WLANQTUTILSIAPSETTINGS_DELETEIAP_ENTRY);
    
    try {
        QScopedPointer<CmConnectionMethodShim> iap(
            mCmManager->connectionMethod(iapId));
        iap->deleteConnectionMethod();
    } catch (const std::exception &ex) {
        // Just trace error cause. It is not fatal, if we are not able to
        // delete the IAP. No need to retry, since errors should be very
        // rare and user can delete the IAP later from Control Panel, if
        // needed.
        int error = qt_symbian_exception2Error(ex);
        OstTrace1(
            TRACE_NORMAL,
            WLANQTUTILSIAPSETTINGS_DELETEIAP_EXCEPTION,
            "WlanQtUtilsIapSettings::deleteIap exception;error=%d",
            error);
    }

    OstTraceFunctionExit0(WLANQTUTILSIAPSETTINGS_DELETEIAP_EXIT);
}

/*!
    Move IAP to the Internet SNAP.
    
    @param [in] iapId ID of the IAP to move.
*/

void WlanQtUtilsIapSettings::moveIapToInternetSnap(int iapId)
{
    OstTraceFunctionEntry0(WLANQTUTILSIAPSETTINGS_MOVEIAPTOINTERNETSNAP_ENTRY);
    
    // Read all destination (SNAP) IDs
    QList<uint> destinations;
    try {
        mCmManager->allDestinations(destinations);
        foreach (int destId, destinations) {
            QScopedPointer<CmDestinationShim> destination(
                mCmManager->destination(destId));
        
            // Internet destination will always exist in the system. It has 
            // SnapPurposeInternet set in its metadata.
            if (destination->metadata(CMManagerShim::SnapMetadataPurpose)
                == CMManagerShim::SnapPurposeInternet) {
                QScopedPointer<CmConnectionMethodShim> iap(
                    mCmManager->connectionMethod(iapId));
                destination->addConnectionMethod(iap.data());
                destination->update();
                break;
            }
        }
    } catch (const std::exception &ex) {
        int error = qt_symbian_exception2Error(ex);
        OstTrace1(
            TRACE_NORMAL,
            WLANQTUTILSIAPSETTINGS_MOVEIAPTOINTERNETSNAP_EXCEPTION,
            "WlanQtUtilsIapSettings::moveIapToInternetSnap exception;error=%d",
            error);
    }

    OstTraceFunctionExit0(WLANQTUTILSIAPSETTINGS_MOVEIAPTOINTERNETSNAP_EXIT);
}

/*!
    Stores the given Wlan AP settings to database using CM Manager Shim.
   
    @param [in] wlanAp WLAN AP settings to store.
    @param [in] iap WLAN IAP to store to.
*/

void WlanQtUtilsIapSettings::storeSettings(
    const WlanQtUtilsAp *wlanAp,
    CmConnectionMethodShim *iap)
{
    OstTraceFunctionEntry0(WLANQTUTILSIAPSETTINGS_STORESETTINGS_ENTRY);
    
    int secMode = wlanAp->value(WlanQtUtilsAp::ConfIdSecurityMode).toInt();
    QString ssid = wlanAp->value(WlanQtUtilsAp::ConfIdSsid).toString();
    
    // Store general values
    iap->setStringAttribute(CMManagerShim::CmName, ssid);
    iap->setStringAttribute(CMManagerShim::WlanSSID, ssid);
    iap->setIntAttribute(CMManagerShim::WlanSecurityMode, secMode);
    iap->setIntAttribute(
        CMManagerShim::WlanConnectionMode, 
        wlanAp->value(WlanQtUtilsAp::ConfIdConnectionMode).toInt());
    
    switch (secMode) {
    case CMManagerShim::WlanSecModeWep:
        // Store the 4 WEP keys (function does nothing, if a key is not set)
        storeWepKey(
            wlanAp->value(WlanQtUtilsAp::ConfIdWepKey1).toString(),
            1,
            iap);
        storeWepKey(
            wlanAp->value(WlanQtUtilsAp::ConfIdWepKey2).toString(),
            2,
            iap);
        storeWepKey(
            wlanAp->value(WlanQtUtilsAp::ConfIdWepKey3).toString(),
            3,
            iap);
        storeWepKey(
            wlanAp->value(WlanQtUtilsAp::ConfIdWepKey4).toString(),
            4,
            iap);
        
        iap->setIntAttribute(
            CMManagerShim::WlanWepKeyIndex,
            wlanAp->value(WlanQtUtilsAp::ConfIdWepDefaultIndex).toInt());
        break;

    case CMManagerShim::WlanSecModeWpa:
    case CMManagerShim::WlanSecModeWpa2:
        // Store WPA PSK values
        bool usePsk = wlanAp->value(WlanQtUtilsAp::ConfIdWpaPskUse).toBool();
        iap->setBoolAttribute(CMManagerShim::WlanEnableWpaPsk, usePsk);
        if (usePsk) {
            QString wpaKey(wlanAp->value(WlanQtUtilsAp::ConfIdWpaPsk ).toString());
            iap->setString8Attribute(CMManagerShim::WlanWpaPreSharedKey, wpaKey);
        }
        break;
    }
    
    iap->update();

    OstTraceFunctionExit0(WLANQTUTILSIAPSETTINGS_STORESETTINGS_EXIT);
}

/*!
    Stores the given valid WEP key to database using CM Manager Shim. Ignores
    keys with zero length.
  
    @note This method MUST not be called for invalid WEP Keys. Wlanwizard
          validates keys, before accepting user input.
          
    @param [in] key Key to write.
    @param [in] index Key index. Valid range is [0,4].
    @param [in] iap WLAN IAP to store the key into.
*/

void WlanQtUtilsIapSettings::storeWepKey(
    const QString &key,
    int index,
    CmConnectionMethodShim *iap)
{
    OstTraceFunctionEntry0(WLANQTUTILSIAPSETTINGS_STOREWEPKEY_ENTRY);
    
    // Default value is set just to silence compiler, Q_ASSERTs take care
    // that valid attribute is set below
    CMManagerShim::ConnectionMethodAttribute attribute = 
        CMManagerShim::WlanWepKey1InHex;
    
    int length = key.length();
    if (length == WepHex64BitMaxLength || length == WepHex128BitMaxLength) {
        // HEX
        switch (index) {
        case 1:
            attribute = CMManagerShim::WlanWepKey1InHex;
            break;
            
        case 2:
            attribute = CMManagerShim::WlanWepKey2InHex;
            break;
            
        case 3:
            attribute = CMManagerShim::WlanWepKey3InHex;
            break;
            
        case 4:
            attribute = CMManagerShim::WlanWepKey4InHex;
            break;
            
#ifndef QT_NO_DEBUG
        default:
            // Invalid key index detected
            Q_ASSERT(0);
            break;
#endif            
        }
    } else if (length == WepAscii64BitMaxLength || length == WepAscii128BitMaxLength) {
        // ASCII
        switch (index) {
        case 1:
            attribute = CMManagerShim::WlanWepKey1InAscii;
            break;
            
        case 2:
            attribute = CMManagerShim::WlanWepKey2InAscii;
            break;
            
        case 3:
            attribute = CMManagerShim::WlanWepKey3InAscii;
            break;

        case 4:
            attribute = CMManagerShim::WlanWepKey4InAscii;
            break;
            
#ifndef QT_NO_DEBUG
        default:
            // Invalid key index detected
            Q_ASSERT(0);
            break;
#endif            
        }
    }
    
    if (length > 0) {
        iap->setString8Attribute(attribute, key);
    } // else: key is not set, ignore

    OstTraceFunctionExit0(WLANQTUTILSIAPSETTINGS_STOREWEPKEY_EXIT);
}
