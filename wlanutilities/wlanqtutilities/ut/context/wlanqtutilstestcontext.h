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
* This is the header file for Wlan Qt Utilities test context.
*/
 
#ifndef WLANQTUTILSTESTCONTEXT_H
#define WLANQTUTILSTESTCONTEXT_H

#include <QObject>

#include "wlanqtutilsactiveconn.h"

class WlanQtUtilsWlanAp;

/**
 * This is context for esock library.
 */
class WlanQtUtilsCtxEsock : public QObject
{
    Q_OBJECT

public:
    WlanQtUtilsCtxEsock();
    ~WlanQtUtilsCtxEsock();

    /**
     * Initializes context.
     */
    void initialize();

    /** Return value of RConnection::Start(). */
    int startRetValue_;
};

/**
 * This class is a helper class for testing purposes. It derives all properties of WlanQtUtilsActiveConn.
 * In addition, this class contains member variables for setting some Connection Monitor specific
 * values which cannot be easily generated from member variables of WlanQtUtilsActiveConn.
 * E.g., connection status has different enum in Connection Monitor and Wlan Qt Utilities. This class
 * enables setting both of those. Connmon version is set so that the stub can return it when
 * requested, and Wlan Qt Utilities version is used for verifying that tested code returns right
 * value.
 */
class WlanQtUtilsCtxActiveConn : public WlanQtUtilsActiveConn
{
public:
    WlanQtUtilsCtxActiveConn();
    ~WlanQtUtilsCtxActiveConn();

    /** Bearer type in connmon format. */
    int connMonBearerType_;
    /** Connection status in connmon format. */
    int connMonConnectionStatus_;
    /** WLAN connection mode in connmon format. */
    int connMonWlanConnectionMode_;
    /** WLAN security mode in connmon format. */
    int connMonWlanSecurityMode_;
    /** Application UIDs that map to application strings. */
    QList<int> applicationUids_;
};

/**
 * This class represents active connections.
 */
class WlanQtUtilsCtxActiveConnections : public QObject
{
    Q_OBJECT

public:
    WlanQtUtilsCtxActiveConnections();
    ~WlanQtUtilsCtxActiveConnections();

    /**
     * Initializes context.
     */
    void initialize();

    /**
     * Creates a list of active connections for scan results.
     * 
     * @param[in] numberOfActiveConns Number of active connections that are created.
     * @param[in] firstIapId IAP ID of the first connection to be created.
     */
    void createDefaultActiveConnList(int numberOfActiveConns, int firstIapId);

    /**
     * Verifies that the given active connection list returned by the real application matches
     * with the one in the context.
     * 
     * @param[in] activeConnList Active connections that are verified against the context.
     */
    void verifyActiveConnList(QList<WlanQtUtilsActiveConn *> activeConnList);

    /**
     * Finds an active connection matching with the given connection ID from the context.
     * Asserts if connection with given connection ID is not found.
     * 
     * @param connectionId Connection ID.
     * @return Active connection.
     */
    WlanQtUtilsCtxActiveConn *findActiveConn(uint connectionId) const;

    /**
     * List of active connections that RConnectionMonitor stub will return in response to
     * RConnectionMonitor::Get*Attribute() functions.
     */
    QList<WlanQtUtilsCtxActiveConn *> activeConnList_;

    // TODO: We need return values for all different RConnectionMonitor::Get*Attribute() functions
    //       to test error cases.

private:
    
    /**
     * Destroys and clears the list of WLAN APs.
     */
    void clearActiveConnList();
};

/**
 * This class represents WLAN APs that represent the results of WLAN scanning triggered by
 * RConnectionMonitor::GetPckgAttribute()
 */
class WlanQtUtilsWlanScanResult : public QObject
{
    Q_OBJECT

public:
    WlanQtUtilsWlanScanResult();
    ~WlanQtUtilsWlanScanResult();

    /**
     * Initializes context.
     */
    void initialize();

    /**
     * Creates a list of WLAN APs for scan results.
     * 
     * @param[in] numberOfWlanAps Number of WLAN APs that are created.
     */
    void createDefaultWlanScanResultList(int numberOfWlanAps);

    /**
     * Verifies that the given WLAN AP list returned by the real application matches
     * with the one in the context.
     * 
     * @param[in] wlanApList WLAN APs that are verified against the context.
     */
    void verifyWlanScanResultList(QList<WlanQtUtilsWlanAp *> wlanApList); // TODO: Maybe boolean return value

    /**
     * List of WLAN access points that RConnectionMonitor stub will return in response to
     * RConnectionMonitor::GetPckgAttribute().
     */
    QList<WlanQtUtilsWlanAp *> wlanScanResultList_;
    
private:
    
    /**
     * Destroys and clears the list of WLAN APs.
     */
    void clearWlanScanResultList();
};

/**
 * Test context for RConnectionMonitor.
 */
class WlanQtUtilsCtxConnMon : public QObject
{
    Q_OBJECT

public:
    WlanQtUtilsCtxConnMon();
    ~WlanQtUtilsCtxConnMon();

    /**
     * Initializes context.
     */
    void initialize();

    /** Results of WLAN scanning. */
    WlanQtUtilsWlanScanResult wlanScanResult_;

    /** List of active connections. */
    WlanQtUtilsCtxActiveConnections activeConnections_;
};

/**
 * Test context for Internet connectivity test library.
 */
class WlanQtUtilsCtxIct : public QObject
{
    Q_OBJECT

public:
    WlanQtUtilsCtxIct();
    ~WlanQtUtilsCtxIct();

    /**
     * Initializes context.
     */
    void initialize();
};

/**
 * Test context. The purpose of this context is to provide values that are used in stubs
 * and in verification of outputs in test cases.
 * 
 * In stubs, context is used to verify the inputs of parameters and to set output
 * parameters into a specific value.
 * 
 * In test cases, context is used to verify that the output parameters match with the once
 * set in the stub.
 */
class WlanQtUtilsTestContext : public QObject
{
    Q_OBJECT

public:
    WlanQtUtilsTestContext();
    ~WlanQtUtilsTestContext();
    
    /**
     * Initializes context.
     */
    void initialize();

    /** Context for esock library. */
    WlanQtUtilsCtxEsock esock_;
    /** Context for connmon library. */
    WlanQtUtilsCtxConnMon connMon_;
    /** Context for ICTS library. */
    WlanQtUtilsCtxIct ict_;
};

#endif /* WLANQTUTILSTESTCONTEXT_H */

