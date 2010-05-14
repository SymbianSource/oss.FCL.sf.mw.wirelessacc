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
 * This is a header file for following classes:
 * ConnMonScanWlanAps.
 */

#ifndef WLANQTUTILSCONMONWRAPPER_S60_P_H_
#define WLANQTUTILSCONMONWRAPPER_S60_P_H_

// INCLUDES
#include <e32base.h>
#include <rconnmon.h>
#include "wlanqtutilscommon.h"

class ConMonWrapper;

QT_BEGIN_NAMESPACE

// CLASS DECLARATION

/**
 * ConnMonScanWlanAps class.
 */
class ConnMonScanWlanAps : public CActive
    {

public:
    // Constructor and destructor

    /**
     * Default constructor
     */
    ConnMonScanWlanAps(ConMonWrapper *wrapper);

    /**
     * Destructor
     */
    ~ConnMonScanWlanAps();

public:
    // New functions    

    /**
     * scanAvailableWlanAPs
     * scans all available WLAN APs
     * @return error code
     */
    int scanAvailableWlanAPs();

private:
    // From CActive

    /**
     * Implements cancellation of an outstanding request.
     */
    void DoCancel();

    /**
     * Handles an active object's request completion event.
     */
    void RunL();

    /**
     * conMon2WlanQtUtilsSecModeMap
     * Maps Connection monitor's WLAN security mode value into WlanQtUtilsWlanSecMode.
     * @aSecMode: security mode, ConnMon style,
     * @return: security mode, Wlan Qt Utilities style.
     */
    WlanQtUtilsWlanSecMode conMon2WlanQtUtilsSecModeMap(TUint aSecMode);

private:
    // Data

    /**
     * q_ptr pointer to ConMonWrapper
     * Owned by ConnMonScanWlanAps object, instantiated in
     * constructor.
     */
    ConMonWrapper *q_ptr;

    /**
     * Handle to Connection Monitor Server
     */
    RConnectionMonitor iMonitor;

    /**
     * Scan package used with Connection Monitor Server scan request
     * Owned by ConnMonScanWlanAps object.
     */
    CConnMonWlanNetworksPtrArrayPckg* wlanBuf;

    /**
     * wlan pointer variable used with Connection Monitor Server scan request
     */
    TPtr wlanPtr;
    };

// CLASS DECLARATION

/**
 * ConnMonConnInfo class.
 */
class ConnMonConnInfo : public MConnectionMonitorObserver
    {
public:
    // Constructor and destructor

    /**
     * Default constructor
     */
    ConnMonConnInfo(ConMonWrapper *wrapper);

    /**
     * Destructor
     */
    ~ConnMonConnInfo();

public:
    // New functions    

    /**
     * Return active connection information.
     * 
     * @return Information of active connection, if one exists.
     */ 
    WlanQtUtilsActiveConn *activeConnection();

    /**
     * Returns information of a connection with the given connection ID.
     * 
     * @param[in] connectionId Connection ID.
     * @return Information of the given connection, if one exists.
     */ 
    WlanQtUtilsActiveConn *connectionInfo(uint connectionId);

    /**
     * Updates information of a given active connection. Missing information will be filled in
     * and information that is dynamic in nature will be updated.
     * 
     * @param[in] activeConn Current data of an active connection which will be updated.
     *                       Connection ID and bearer type have to be valid.
     */
    void activeConnInfoUpdate( WlanQtUtilsActiveConn *activeConn );

private:
    // New private functions

    /** 
     * Starts monitoring observer events
     */
    void StartMonitoringL();

    /** 
     * Stops monitoring observer events
     */
    void StopMonitoring();

    /**
     * Event method is called every time an event occures
     */
    void EventL(const CConnMonEventBase& aConnMonEvent);

    /**
     * Maps Connection Monitor's WLAN connection mode value into
     * WlanQtUtilsWlanConnMode.
     *
     * @param aConnMode Connection mode in Connection Monitor's style.
     * @return Connection mode in Wlan Qt Utilities style.
     */
    static WlanQtUtilsWlanConnMode connMon2WlanQtUtilsConnModeMap(TInt aConnMode);

    /**
     * Maps Connection Monitor's connection status value into WlanQtUtilsConnectionStatus.
     *
     * @param aConnectionStatus Connection status as used by Connection Monitor server.
     *                          These status are defined in nifvar.h.
     * @return Connection status in Wlan Qt Utilities style.
     */
    static WlanQtUtilsConnectionStatus connMon2WlanQtUtilsConnectionStatusMap(TInt aConnectionStatus);

    /**
     * Maps Connection Monitor's bearer type value into WlanQtUtilsBearerType.
     *
     * @param aBearerType Bearer type as used by Connection Monitor server.
     * @return Bearer type in Wlan Qt Utilities style.
     */
    static WlanQtUtilsBearerType connMon2WlanQtUtilsBearerTypeMap(TInt aBearerType);

    bool isWlan(uint connectionId);

private:
    // Data

    /**
     * q_ptr pointer to ConMonWrapper
     * Owned by ConMonWrapperPrivate object, instantiated in
     * constructor.
     */
    ConMonWrapper *q_ptr;

    /** Handle to Connection Monitor Server. */
    RConnectionMonitor iMonitor;

    /** Request status. */
    TRequestStatus iStatus;

    /** Indicates whether connection monitoring is ON. */
    TBool iMonitoring;
    
private: // Friend classes

    // TestWlanQtUtils is defined as a friend class in order to be able to
    // call event handlers of wrappers.
    friend class TestWlanQtUtils;
    };

// CLASS DECLARATION

/**
 * ConnMonConnDisconnect class.
 */
class ConnMonConnDisconnect : public CActive
    {

public:
    // Constructor and destructor

    /**
     * Default constructor
     */
    ConnMonConnDisconnect(ConMonWrapper *wrapper);

    /**
     * Destructor
     */
    ~ConnMonConnDisconnect();

public:
    // New functions    

    /**
     * disconnectConnection
     * Stop connection
     */
    void disconnectConnection(int iapId);

private:
    // From CActive

    /**
     * Implements cancellation of an outstanding request.
     */
    void DoCancel();

    /**
     * Handles an active object's request completion event.
     */
    void RunL();

private:
    // Data

    /**
     * q_ptr pointer to ConMonWrapper
     * Owned by ConMonWrapperPrivate object, instantiated in
     * constructor.
     */
    ConMonWrapper *q_ptr;

    /**
     * Handle to Connection Monitor Server
     */
    RConnectionMonitor iMonitor;
    };

QT_END_NAMESPACE

#endif /* WLANQTUTILSCONMONWRAPPER_S60_P_H_ */

// End of file

