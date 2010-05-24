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
* WLAN Qt Utilities implementation.
*/

// System includes

// User includes

#include "wlanqtutilsap.h"
#include "wlanqtutilsiap.h"
#include "wlanqtutils_p.h"
#include "wlanqtutils.h"

/*!
    \class WlanQtUtils
    \brief Wlan Qt Utilities. This class provides a Qt API for UI components
           for retrieving different kind of information related to WLAN
           functionality.
*/


// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
*/

WlanQtUtils::WlanQtUtils() :
    d_ptr(new WlanQtUtilsPrivate(this))
{
}

/*!
    Destructor.
*/

WlanQtUtils::~WlanQtUtils()
{
}

/*!
    Function for requesting a single WLAN scan to be triggered.
    Signal wlanScanReady() is emitted when new scan results are available.
*/

void WlanQtUtils::scanWlans()
{
    d_ptr->scanWlans();
}

/*!
    Function for requesting a WLAN AP scan to be triggered.
    Signal availableWlanAps() is emitted when new scan results are available.
*/

void WlanQtUtils::scanWlanAps()
{
    d_ptr->scanWlanAps();
}

/*!
    Function for requesting a direct WLAN scan with given SSID.

    Signal wlanScanDirectReady() is emitted when new scan results are
    available.

    @param [in] ssid Network name to be found
*/

void WlanQtUtils::scanWlanDirect(const QString &ssid)
{
    d_ptr->scanWlanDirect(ssid);
}

/*!
    Function for stopping a (possibly) ongoing WLAN scan. No scan result
    signal will be sent before a new scan request is made.
*/

void WlanQtUtils::stopWlanScan()
{
    d_ptr->stopWlanScan();
}

/*!
    Function to request details of available WLAN networks. Can be called 
    at any time. Calling right after wlanScanReady() signal ensures you get
    the most recent results.

    @param [out] wlanIapList List of available WLAN IAPs.
    @param [out] wlanApList List of unknown WLAN APs.
*/

void WlanQtUtils::availableWlans(
    QList< QSharedPointer<WlanQtUtilsIap> > &wlanIapList,
    QList< QSharedPointer<WlanQtUtilsAp> > &wlanApList) const
{
    d_ptr->availableWlans(wlanIapList, wlanApList);
}

/*!
    Function to request details of available WLAN networks. This function is
    used to get the results that are informed by following signals:
    -wlanScanApReady()
    -wlanScanDirectReady()

    @param [out] wlanApList List of unknown WLAN APs.
*/

void WlanQtUtils::availableWlanAps(
    QList< QSharedPointer<WlanQtUtilsAp> > &wlanApList) const
{
    d_ptr->availableWlanAps(wlanApList);
}

/*!
    Function to create an IAP from the given WLAN access point.

    @param [in] wlanAp Access point containing parameters to include in the new IAP.
    
    @return IAP ID if creation succeeds, IapIdNone otherwise.
*/

int WlanQtUtils::createIap(const WlanQtUtilsAp *wlanAp)
{
    return d_ptr->createIap(wlanAp);
}

/*!
    Function to update an IAP from the given WLAN access point.

    @param [in] iapId ID of the IAP to be updated.
    @param [in] wlanAp Access point containing parameters to update in the IAP.
    
    @return Was update successful?
*/

bool WlanQtUtils::updateIap(int iapId, const WlanQtUtilsAp *wlanAp)
{
    return d_ptr->updateIap(iapId, wlanAp);
}

/*!
    Function to delete an IAP.

    @param [in] iapId ID of the IAP to be deleted.
*/

void WlanQtUtils::deleteIap(int iapId)
{
    d_ptr->deleteIap(iapId);
}

/*!
    Function to start connection creation for the given IAP. Runs also
    Internet Connectivity Test (ICT), if requested with the optional
    parameter.
    
    Connecting while there is already a connection is also supported.

    wlanNetworkOpened() or wlanNetworkClosed() signal will be emitted
    when connection status changes.
    
    ictResult() signal will be emitted when (possible) ICT result is
    available.

    @param [in] iapId ID of the IAP to be connected.
    @param [in] runIct Should ICT be run or not?
*/

void WlanQtUtils::connectIap(int iapId, bool runIct)
{
    d_ptr->connectIap(iapId, runIct);
}

/*!
    Function to disconnect the given IAP.
    Disconnecting when there is no connection is also supported.

    @param [in] iapId ID of the IAP to be disconnected.
*/

void WlanQtUtils::disconnectIap(int iapId)
{
    d_ptr->disconnectIap(iapId);
}

/*!
    Function to retrieve the name of the IAP with the given ID.

    @param [in] iapId ID of the requested IAP.
    
    @return Name of the IAP or empty QString if IAP is not found.
*/

QString WlanQtUtils::iapName(int iapId) const
{
    return d_ptr->iapName(iapId);
}

/*!
    Connection status getter.
    
    @return Current WLAN connection status. 
*/

WlanQtUtils::ConnStatus WlanQtUtils::connectionStatus() const
{
    return d_ptr->connectionStatus();
}

/*!
    Function for getting the ID of the (possibly) active WLAN IAP.
    Active here means non-disconnected.
    Use connectionStatus() for retrieving the actual status.

    @return ID of the active IAP, IapIdNone if not valid.
*/

int WlanQtUtils::activeIap() const
{
    return d_ptr->activeIap();
}
