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

#ifndef WLANQTUTILS_P_H
#define WLANQTUTILS_P_H

#include <QObject>
#include <QMap>

#include "wlanqtutilscommon.h"

class WlanQtUtils;
class WlanQtUtilsIap;
class WlanQtUtilsWlanIap;
class WlanQtUtilsWlanAp;
class WlanQtUtilsActiveConn;
class ConMonWrapper;
class CmmWrapper;
class EsockWrapper;
class ConnTestWrapper;

/**
 * Private implementation of Wlan Qt Utilities.
 */
class WlanQtUtilsPrivate : public QObject
{
    Q_OBJECT

public:
    
    /**
     * Constructor.
     */
    WlanQtUtilsPrivate(WlanQtUtils *publicPtr);
    
    /**
     * Destructor.
     */
    ~WlanQtUtilsPrivate();

    // Functions duplicating WlanQtUtils public interface
    
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
    
private slots:

    /**
     * Slot for handling WLAN scan result event from wrapper. Results are stored in member variable
     * (possible duplicates are removed).
     * 
     * @param[in] availableWlans WLAN networks found in scan.
     */
    void updateAvailableWlanAps(QList<WlanQtUtilsWlanAp *> &availableWlans);

    /**
     * Slot for handling connection setup status event from wrapper.
     * 
     * @param[in] isOpened Was connection setup successful?
     */
    void updateConnectionStatus(bool isOpened);

    /**
     * Slot for handling connectivity test result event from wrapper. Tested IAP is stored to
     * Internet SNAP, if test was successful.
     * 
     * @param[in] result Was the test successful?
     */
    void updateConnectivityTestResult(bool result);
    
    /**
     * Slot for updating active connection status from wrapper.
     * 
     * @param[in] connectionId ID of the new connection.
     */
    void addActiveConnection(uint connectionId);
    
    /**
     * Slot for updating active connection status from wrapper.
     * 
     * @param[in] connectionId ID of the deleted connection.
     */
    void removeActiveConnection(uint connectionId);
    
    /**
     * Slot for updating active connection status from wrapper.
     * 
     * @param[in] connectionId ID of the updated connection.
     * @param[in] connectionStatus New status of the connection.
     */
    void updateActiveConnection(uint connectionId, WlanQtUtilsConnectionStatus connectionStatus);
    
private: 

    int fetchIaps();
    
    /**
     * This function searches for a WLAN IAP with the given WLAN parameters.
     *  
     * @param[in] ssid SSID to search for.
     * @param[in] secMode Security mode to search for.
     * @return True, if suitable WLAN IAP found, false otherwise.
     */
    bool wlanIapExists(QString ssid, WlanQtUtilsWlanSecMode secMode);

    /**
     * This function updates the connection status of:
     * - IAP with the given ID
     * - all other IAPs, which represent the same network (SSID & security mode match)
     *  
     * @param[in] iapId ID of the IAP whose status is to be updated.
     * @param[in] status Updated connection status.
     */
    void updateIapConnectionStatuses(int iapId, WlanQtUtilsConnectionStatus status);

private: // Data
    
    /** Pointer to public implementation */
    WlanQtUtils *q_ptr;
    
    /** Wrapper object for Connection Method Manager. */
    CmmWrapper *cmmWrapper_;
    
    /** Wrapper object for Connection Monitor and other parts of connmon library. */
    ConMonWrapper *conMonWrapper_;
    
    /** Wrapper object for esock library. */
    EsockWrapper *esockWrapper_;

    /** Wrapper object for Internet Connectivity Test library. */
    ConnTestWrapper *connTestWrapper_;

    /** List of available WLAN APs according to the latest scan. */
    QList<WlanQtUtilsWlanAp *> wlanScanList_;
    
    /** List of WLAN IAPs configured to the device. */
    QMap<int, WlanQtUtilsWlanIap *> wlanIapList_;
    
    /** ID of IAP requiring ICT running, -1 if not valid. */
    int toBeTestedIapId_;
    
    /** ID of IAP being connected by us (requested by UI), -1 if not valid. */
    int connectingIapId_;
    
    /** Information of possible active connection. */
    WlanQtUtilsActiveConn *activeConnection_;
    
private: // Friend classes

    // This is defined as a friend class in order to be able to
    // call event handlers of wrappers from test code.
    friend class TestWlanQtUtils;
};

#endif /* WLANQTUTILS_P_H */

// End of File
