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

#ifndef WLANQTUTILS_H
#define WLANQTUTILS_H

#include <QObject>

#include "wlanqtutilscommon.h"

class WlanQtUtilsIap;
class WlanQtUtilsWlanIap;
class WlanQtUtilsWlanAp;
class WlanQtUtilsPrivate;

const int WlanQtUtilsInvalidIapId = -1;

/**
 * Wlan Qt Utilities.
 * 
 * This class provides a Qt API to UI components for retrieving different kind of information
 * related to WLAN functionality.
 */
class WLANQTUTILITIESDLL_EXPORT WlanQtUtils : public QObject
{
    Q_OBJECT

public:
    
    /**
     * Constructor.
     */
    WlanQtUtils();
    
    /**
     * Destructor.
     */
    ~WlanQtUtils();

    /**
     * Function to request details of available WLAN networks. Can be called at any time, calling right
     * after wlanScanReady() signal ensures you get the most recent results.
     *
     * @param[out] wlanIapList List of available WLAN IAPs.
     * @param[out] wlanApList List of unknown WLAN APs.
     */
    void availableWlanAps(
        QList<WlanQtUtilsWlanIap *> &wlanIapList,
        QList<WlanQtUtilsWlanAp *> &wlanApList);
    
    /**
     * Function to create an IAP from the given WLAN access point.
     *
     * @param[in] wlanAp Access point containing parameters to include in the new IAP.
     * @return ID of the newly created IAP.
     */
    int createWlanIap(const WlanQtUtilsWlanAp *wlanAp);

    /**
     * Function to start connection creation for the given IAP. connectionStatus() signal will be emitted
     * when connection creation succeeds or fails.
     *
     * @param[in] iapId ID of the IAP to be connected.
     */
    void connectIap(int iapId);

    /**
     * Function to disconnect the given IAP.
     * 
     * @param[in] iapId ID of the IAP to be disconnected.
     */
    void disconnectIap(int iapId);

    /**
     * Function to retrieve a pointer to the IAP with the given ID.
     * 
     * @param[in] iapId ID of the requested IAP.
     * @return Pointer to the found IAP, NULL if not found.
     */
    WlanQtUtilsIap *iap(int iapId) const;
    
    /**
     * Function for getting the master WLAN status.
     * 
     * @return Master WLAN status: true if enabled, otherwise false.
     */
    bool masterWlan() const;
    
    /**
     * Function for switching the master WLAN status ON or OFF.
     * 
     * @param[in] enabled If set to true, WLAN is switched ON, and vice versa.
     */
    void setMasterWlan(bool enabled);
    
    /**
     * Function for getting the ID of the (possibly) connected WLAN IAP.
     * 
     * @return ID of the connected IAP, WlanQtUtilsInvalidIapId if not valid.
     */
    int connectedWlanId() const;

    /**
     * Function for requesting a WLAN scan to be triggered. Currently triggers
     * only a single scan, but can be extended to perform also periodic scans,
     * if needed in the future.
     * 
     * Signal wlanScanReady() is emitted when new scan results are available.
     */
    void scanWlans();
    
signals:
    
    /**
     * Signal indicating that WLAN scan results are available. 
     */
    void wlanScanReady();

    /**
     * Signal indicating that new WLAN network has been opened. 
     * 
     * @param[in] iapId ID of the opened IAP.
     */
    void wlanNetworkOpened(int iapId);

    /**
     * Signal indicating that a WLAN network has been closed. 
     * 
     * @param[in] iapId ID of the closed IAP.
     */
    void wlanNetworkClosed(int iapId);

    /**
     * Signal indicating that the WLAN master status has changed.
     */
    void masterWlanStatus(bool enabled);
    
private: // Data

    /** Pointer to private implementation */
    WlanQtUtilsPrivate *d_ptr;

private: // Friend classes

    // This is defined as a friend class in order to be able to emit public signals
    // directly from private implementation code.
    friend class WlanQtUtilsPrivate;

    // This is defined as a friend class in order to be able to
    // call event handlers of wrappers from test code.
    friend class TestWlanQtUtils;
};

#endif /* WLANQTUTILS_H */

// End of File
