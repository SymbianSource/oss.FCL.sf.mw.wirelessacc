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
 * This is a source file for following classes:
 * ConnMonScanWlanAps,
 * ConnMonConnInfo,
 * ConnMonConnDisconnect
 */

// INCLUDE FILES
#include <cmconnectionmethodext.h>
#include <rconnmon.h>
#include <nifvar.h>

#include "wlanqtutilscommon.h"
#include "wlanqtutilswlanap.h"
#include "wlanqtutilsactiveconn.h"
#include "wlanqtutilsconmonwrapper.h"
#include "wlanqtutilsconmonwrapper_s60_p.h"

#ifdef WLANQTUTILS_NO_OST_TRACES_FLAG
#include <opensystemtrace.h>
#else
#include "OstTraceDefinitions.h"
#endif
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutilsconmonwrapper_s60Traces.h"
#endif


// =========== PRIVATE CLASS MEMBER FUNCTIONS ===============
//
// ---------------------------------------------------------
// ConnMonScanWlanAps::ConnMonScanWlanAps()
// Constructor
// ---------------------------------------------------------
//
ConnMonScanWlanAps::ConnMonScanWlanAps(ConMonWrapper *wrapper) :
    CActive(EPriorityStandard), q_ptr(wrapper), wlanPtr(0, 0)
    {
    OstTraceFunctionEntry1( CONNMONSCANWLANAPS_CONNMONSCANWLANAPS_ENTRY, this );
    
    CActiveScheduler::Add(this);
    // TBD: What if this leaves?
    // Beware! iConnMon.ConnectL() can
    // both leave AND return an error code..
    // User::LeaveIfError( iMonitor.ConnectL() );
    iMonitor.ConnectL();

    OstTraceFunctionExit1( CONNMONSCANWLANAPS_CONNMONSCANWLANAPS_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonScanWlanAps::ConnMonScanWlanAps()
// Destructor
// ---------------------------------------------------------
//
ConnMonScanWlanAps::~ConnMonScanWlanAps()
    {
    OstTraceFunctionEntry1( CONNMONSCANWLANAPS_CONNMONSCANWLANAPSDESTR_ENTRY, this );
    
    Cancel();
    // Cancel notifications and close connection monitor session
    iMonitor.Close();
    if (wlanBuf)
        {
        delete wlanBuf;
        }

    OstTraceFunctionExit1( CONNMONSCANWLANAPS_CONNMONSCANWLANAPSDESTR_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonScanWlanAps::DoCancel()
// 
// ---------------------------------------------------------
//
void ConnMonScanWlanAps::DoCancel()
    {
    OstTraceFunctionEntry1( CONNMONSCANWLANAPS_DOCANCEL_ENTRY, this );
    // TODO: error handling
    OstTraceFunctionExit1( CONNMONSCANWLANAPS_DOCANCEL_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonScanWlanAps::RunL()
// Called when scan is ready. Emits scan results in a signal.
// ---------------------------------------------------------
//
void ConnMonScanWlanAps::RunL()
    {
    OstTraceFunctionEntry1( CONNMONSCANWLANAPS_RUNL_ENTRY, this );
    
    if (iStatus == KErrNone)
        {
        RConnMonWlanNetworksPtrArray wlanPtrArray;
        wlanBuf->UnpackToL(wlanPtrArray);
        QList<WlanQtUtilsWlanAp*> wlanScanResult;

        for (TInt i = 0; i < wlanPtrArray.Count(); i++)
            {
            WlanQtUtilsWlanAp *ap = new WlanQtUtilsWlanAp();

            // Security mode values defined in enum TConnMonSecurityMode, rconnmon.h.
            TUint connMonWlanSecMode;
            WlanQtUtilsWlanSecMode wlanQtUtilsWlanSecMode;

            TBuf<CConnMonWlanNetwork::KMaxNameLength> wlanName =
                    wlanPtrArray[i]->Name();

            QString wlanNameInQt = QString::fromUtf16(wlanName.Ptr(),
                    wlanName.Length());

            if (wlanNameInQt.length() == 1 && wlanNameInQt.at(0).unicode()
                    == 0)
                {
                // QString handles empty strings (single \0 char) strangely,
                // so let's initialize them in a different way:
                wlanNameInQt = "";
                }

            ap->setSsid(wlanNameInQt);

            // TODO: Won't compile to ARMV5 for some reason. Commented for awhile. 
            //TBuf8<CConnMonWlanNetwork::KWlanBssId> wlanBssid = wlanPtrArray[i]->WlanBssid();
            //ap->setBssid(QString::fromUtf8(wlanBssid.Ptr(), wlanBssid.Length()));
            ap->setBssid("addMAC");

            TUint wlanConnectionMode = wlanPtrArray[i]->ConnectionMode();
            ap->setConnectionMode(wlanConnectionMode);

            TUint wlanSignalStrength = wlanPtrArray[i]->SignalStrength();
            ap->setSignalStrength(wlanSignalStrength);

            connMonWlanSecMode = wlanPtrArray[i]->SecurityMode();
            wlanQtUtilsWlanSecMode = conMon2WlanQtUtilsSecModeMap(connMonWlanSecMode);
            ap->setSecurityMode(wlanQtUtilsWlanSecMode);

            wlanScanResult.append(ap);
            delete wlanPtrArray[i];
            }
        q_ptr->emitAvailableWlans(wlanScanResult);
        wlanPtrArray.Close();
        delete wlanBuf;
        wlanBuf = NULL;
        // TODO: Is this enough for clearing QList containing ConnUtilsWlanAps?
        wlanScanResult.clear();
        }
    else
        {
        // TODO: add error handling
        }
    
    OstTraceFunctionExit1( CONNMONSCANWLANAPS_RUNL_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonScanWlanAps::scanAvailableWlanAPs()
// Gets APs from Connection Monitor
// ---------------------------------------------------------
//
int ConnMonScanWlanAps::scanAvailableWlanAPs()
    {
    OstTraceFunctionEntry1( CONNMONSCANWLANAPS_SCANAVAILABLEWLANAPS_ENTRY, this );

    if (!IsActive())
        {
        // TODO: What size for wlanBuf? How to know exact size?
        // 2560 ~ 35 wlan APs
        wlanBuf = new (ELeave) CConnMonWlanNetworksPtrArrayPckg(2560);
        wlanPtr.Set(wlanBuf->Buf()->Des());

        iMonitor.GetPckgAttribute(EBearerIdWLAN, 0, KWlanNetworks, wlanPtr,
                iStatus);
        SetActive();            
        }
    
    OstTraceFunctionExit1( CONNMONSCANWLANAPS_SCANAVAILABLEWLANAPS_EXIT, this );
    return KErrNone;
    }

// ---------------------------------------------------------
// ConMonScanWlanAps::connMon2WlanQtUtilsSecModeMap()
// Map security mode from Connection Monitor into WlanQtUtilsWlanSecMode.
// ---------------------------------------------------------
//
WlanQtUtilsWlanSecMode ConnMonScanWlanAps::conMon2WlanQtUtilsSecModeMap(TUint connMonSecMode)
    {
    WlanQtUtilsWlanSecMode ret = WlanQtUtilsWlanSecModeNone;
    switch (connMonSecMode)
        {
        case EConnMonSecurityWep:
            ret = WlanQtUtilsWlanSecModeWep;
            break;
        case EConnMonSecurity802d1x:
            ret = WlanQtUtilsWlanSecMode802_1x;
            break;
        case EConnMonSecurityWpa:
            ret = WlanQtUtilsWlanSecModeWpa;
            break;
        case EConnMonSecurityWpaPsk:
            ret = WlanQtUtilsWlanSecModeWpa2;
            break;
        default:
            // TODO: should default be None/unknown or should there be assert against EConnMonSecurityOpen.
            ret = WlanQtUtilsWlanSecModeOpen;
            break;
        }
    
    return ret;
    }


// ---------------------------------------------------------
// ConnMonConnInfo::ConnMonConnInfo()
// Constructor
// ---------------------------------------------------------
//
ConnMonConnInfo::ConnMonConnInfo(ConMonWrapper *wrapper) :
    q_ptr(wrapper)
    {
    OstTraceFunctionEntry1( CONNMONCONNINFO_CONNMONCONNINFO_ENTRY, this );
    
    iMonitor.ConnectL();
    StartMonitoringL();
    
    OstTraceFunctionExit1( CONNMONCONNINFO_CONNMONCONNINFO_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnInfo::~ConnMonConnInfo()
// Destructor
// ---------------------------------------------------------
//
ConnMonConnInfo::~ConnMonConnInfo()
    {
    OstTraceFunctionEntry1( CONNMONCONNINFO_CONNMONCONNINFODESTR_ENTRY, this );
    
    StopMonitoring();
    iMonitor.Close();
    
    OstTraceFunctionExit1( CONNMONCONNINFO_CONNMONCONNINFODESTR_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnInfo::StartMonitoringL()
// Starts monitoring of Connection Monitor Server events
// ---------------------------------------------------------
//
void ConnMonConnInfo::StartMonitoringL()
    {
    OstTraceFunctionEntry1( CONNMONCONNINFO_STARTMONITORINGL_ENTRY, this );
    
    iMonitoring = ETrue;
    iMonitor.NotifyEventL(*this);
    
    OstTraceFunctionExit1( CONNMONCONNINFO_STARTMONITORINGL_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnInfo::StopMonitoring()
// Stops monitoring of Connection Monitor Server events
// ---------------------------------------------------------
//
void ConnMonConnInfo::StopMonitoring()
    {
    OstTraceFunctionEntry1( CONNMONCONNINFO_STOPMONITORING_ENTRY, this );
    
    iMonitoring = EFalse;
    iMonitor.CancelNotifications();

    OstTraceFunctionExit1( CONNMONCONNINFO_STOPMONITORING_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnInfo::EventL()
// Handler of Connection Monitor Server events
// ---------------------------------------------------------
//
void ConnMonConnInfo::EventL(const CConnMonEventBase& aConnMonEvent)
    {
    OstTraceFunctionEntry1( CONNMONCONNINFO_EVENTL_ENTRY, this );
    
    TUint connectionId = aConnMonEvent.ConnectionId();
    OstTraceExt2(
        TRACE_NORMAL,
        CONNMONCONNINFO_EVENTL_EVENTTYPE,
        "ConnMonConnInfo::EventL;connectionId=%u;aConnMonEvent.EventType()=%d", connectionId, aConnMonEvent.EventType() );

    // Note: Conversions from CConnMonEventBase to sub classes cannot be made to use dynamic_cast
    // because constructors of CConnMonEventBase and its sub classes are not public in
    // connmon library's interface.

    switch (aConnMonEvent.EventType())
        {
        case EConnMonCreateConnection:
            q_ptr->emitConnCreatedEvent( connectionId );
            break;
        case EConnMonDeleteConnection:

            // TODO: take the following into use.
            // const CConnMonDeleteConnection* eventDelete; 
            // eventDelete = static_cast< const CConnMonDeleteConnection*>( &aConnMonEvent );
            //TUint totalDlVolume = eventDelete->DownlinkData();
            //TUint totalUlVolume = eventDelete->UplinkData();
            //TBool authDelete    = eventDelete->AuthoritativeDelete();

            q_ptr->emitConnDeletedEvent( connectionId );
            break;
        case EConnMonConnectionStatusChange:
            const CConnMonConnectionStatusChange* eventConnectionStatusChange;
            eventConnectionStatusChange =
                static_cast< const CConnMonConnectionStatusChange* >( &aConnMonEvent );
            WlanQtUtilsConnectionStatus connectionStatus;
            connectionStatus = connMon2WlanQtUtilsConnectionStatusMap(
                eventConnectionStatusChange->ConnectionStatus() );
            
            OstTraceExt2(
                TRACE_NORMAL,
                CONNMONCONNINFO_EVENTL_CONNSTATUS,
                "ConnMonConnInfo::EventL;connectionId=%u;eventConnectionStatusChange->ConnectionStatus()=%d",
                connectionId,
                eventConnectionStatusChange->ConnectionStatus() );

            if (connectionStatus != WlanQtUtilsConnectionStatusNone) {
                q_ptr->emitConnStatusEvent( connectionId, connectionStatus );
            }
            break;
        default:
            break;
        }

    OstTraceFunctionExit1( CONNMONCONNINFO_EVENTL_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnInfo::activeConnection()
// ---------------------------------------------------------
//
WlanQtUtilsActiveConn *ConnMonConnInfo::activeConnection()
    {
    OstTraceFunctionEntry1( CONNMONCONNINFO_ACTIVECONNECTION_ENTRY, this );
    
    // Get number of active connections
    TRequestStatus status;

    TUint numConnections = 0;
    iMonitor.GetConnectionCount( numConnections, status );
    User::WaitForRequest( status );
    if ( status.Int() == KErrNone )
        {
        // Get connection info of this connection
        for ( TUint i = 1; i <= numConnections; i++ )
            {
            TUint numSubConnections = 0;
            TUint connectionId = 0;
            TInt ret = iMonitor.GetConnectionInfo( i, connectionId, numSubConnections );
            Q_ASSERT( ret == KErrNone ); // TODO: error handling
    
            WlanQtUtilsActiveConn* activeConn = connectionInfo( connectionId );
            if ( activeConn != NULL )
                {
                // connectionInfo() only returns WLAN connections, and there may be only
                // one active WLAN connection, so this is the one we want to return
                OstTraceFunctionExit1( CONNMONCONNINFO_ACTIVECONNECTION_EXIT, this );
                return activeConn;
                }
            }
        }

    OstTraceFunctionExit1( DUP1_CONNMONCONNINFO_ACTIVECONNECTION_EXIT, this );
    return NULL;
    }

// ---------------------------------------------------------
// ConnMonConnInfo::connectionInfo()
// ---------------------------------------------------------
//
WlanQtUtilsActiveConn *ConnMonConnInfo::connectionInfo( uint connectionId )
    {
    OstTraceFunctionEntry1( CONNMONCONNINFO_CONNECTIONINFO_ENTRY, this );
    
    TRequestStatus status;
    WlanQtUtilsActiveConn *activeConn = NULL;

    // Get the connection bearer.
    TInt bearer = 0;
    iMonitor.GetIntAttribute(
            connectionId,
            0,
            KBearer,
            bearer,
            status );
    User::WaitForRequest( status );
    if ( status.Int() == KErrNone )
        {
        // We are only interested in WLAN connections. 
        WlanQtUtilsBearerType bearerType = connMon2WlanQtUtilsBearerTypeMap( bearer );
        if ( bearerType == WlanQtUtilsBearerTypeWlan )
            {
            activeConn = new WlanQtUtilsActiveConn();
            activeConn->setConnectionId( connectionId );
            activeConn->setBearerType( bearerType );
            activeConnInfoUpdate( activeConn );
            }
        }

    OstTraceFunctionExit1( CONNMONCONNINFO_CONNECTIONINFO_EXIT, this );
    return activeConn;
    }

// ---------------------------------------------------------
// ConnMonConnInfo::activeConnInfoUpdate()
// Requests active connection info from Connection Monitor Server
// ---------------------------------------------------------
//
// TODO: divide this function into smaller pieces depending on the need.
// TODO: optimize so that all the information is not always requested from connmon.
//       Only information that is missing is requested.
void ConnMonConnInfo::activeConnInfoUpdate( WlanQtUtilsActiveConn *activeConn )
    {
    OstTraceFunctionEntry1( CONNMONCONNINFO_ACTIVECONNINFOUPDATE_ENTRY, this );
    
    Q_ASSERT( activeConn != NULL );
    Q_ASSERT( activeConn->bearerType() != WlanQtUtilsBearerTypeNone );

    TUint connectionId = activeConn->connectionId();
    TRequestStatus status;

    // Get connection IAP ID.
    TUint iapId = 0;
    iMonitor.GetUintAttribute(
            connectionId,
            0,
            KIAPId,
            iapId,
            status );
    User::WaitForRequest( status );
    if ( status.Int() == KErrNone )
        {
        activeConn->setIapId( iapId );
        }

    // Get connection status.
    TInt connectionStatus = 0;
    iMonitor.GetIntAttribute(
            connectionId,
            0,
            KConnectionStatus,
            connectionStatus,
            status );
    User::WaitForRequest( status );
    if ( status.Int() == KErrNone ) {
        activeConn->setConnectionStatus( connMon2WlanQtUtilsConnectionStatusMap( connectionStatus ) );
    }

    OstTraceFunctionExit1( CONNMONCONNINFO_ACTIVECONNINFOUPDATE_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnInfo::connMon2WlanQtUtilsConnectionStatusMap()
// Maps connection status of Connection Monitor Server to connection
// status of Wlan Qt Utilities.
// ---------------------------------------------------------
//
WlanQtUtilsConnectionStatus ConnMonConnInfo::connMon2WlanQtUtilsConnectionStatusMap( TInt aConnectionStatus )
    {    
    // TODO: This mapping is not specific to current status which is unknown to wrapper.
    // TODO: This is only current implementation and will need serious modifications and
    //       verification from connmon guys.
    WlanQtUtilsConnectionStatus connUtilsConnectionStatus = WlanQtUtilsConnectionStatusNone;
    switch (aConnectionStatus)
        {
        // KConnectionOpen is not final status for WLAN, because DHCP is run after that
        case KConnectionOpen:
        case KStartingConnection:
            connUtilsConnectionStatus = WlanQtUtilsConnectionStatusConnecting;
            break;
        case KLinkLayerOpen:
            connUtilsConnectionStatus = WlanQtUtilsConnectionStatusConnected;
            break;
        case KConnectionClosed:
        case KLinkLayerClosed:
            connUtilsConnectionStatus = WlanQtUtilsConnectionStatusDisconnected;
            break;
        default:
            // Ignore all other values
            connUtilsConnectionStatus = WlanQtUtilsConnectionStatusNone;
            break;
        }
    
    return connUtilsConnectionStatus;
    }

// ---------------------------------------------------------
// ConnMonConnInfo::connMon2WlanQtUtilsBearerTypeMap()
// Maps bearer type of Connection Monitor Server to bearer
// type of Wlan Qt Utilities.
// ---------------------------------------------------------
//
WlanQtUtilsBearerType ConnMonConnInfo::connMon2WlanQtUtilsBearerTypeMap( TInt aBearerType )
    {    
    WlanQtUtilsBearerType connUtilsBearerType = WlanQtUtilsBearerTypeNone;
    switch ( aBearerType ) {
    case EBearerWCDMA:
    case EBearerGPRS:
    case EBearerEdgeGPRS:
        connUtilsBearerType = WlanQtUtilsBearerTypeCellular;
        break;
    case EBearerWLAN:
        connUtilsBearerType = WlanQtUtilsBearerTypeWlan;
        break;
    default:
        break;
    }

    return connUtilsBearerType;
    }

// ---------------------------------------------------------
// ConnMonConnDisconnect::ConnMonConnDisconnect()
// Constructor
// ---------------------------------------------------------
//
ConnMonConnDisconnect::ConnMonConnDisconnect(ConMonWrapper *wrapper) :
    CActive(EPriorityStandard), q_ptr(wrapper)
    {
    OstTraceFunctionEntry1( CONNMONCONNDISCONNECT_CONNMONCONNDISCONNECT_ENTRY, this );
    
    CActiveScheduler::Add(this);
    iMonitor.ConnectL();

    OstTraceFunctionExit1( CONNMONCONNDISCONNECT_CONNMONCONNDISCONNECT_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnDisconnect::ConnMonConnDisconnect()
// Destructor
// ---------------------------------------------------------
//
ConnMonConnDisconnect::~ConnMonConnDisconnect()
    {
    OstTraceFunctionEntry1( CONNMONCONNDISCONNECT_CONNMONCONNDISCONNECTDESTR_ENTRY, this );

    // Cancel notifications and close connection monitor session
    //iMonitor.CancelNotifications();
    iMonitor.Close();
    
    OstTraceFunctionExit1( CONNMONCONNDISCONNECT_CONNMONCONNDISCONNECTDESTR_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnDisconnect::DoCancel()
// 
// ---------------------------------------------------------
//
void ConnMonConnDisconnect::DoCancel()
    {
    OstTraceFunctionEntry1( CONNMONCONNDISCONNECT_DOCANCEL_ENTRY, this );
    // TODO: error handling
    OstTraceFunctionExit1( CONNMONCONNDISCONNECT_DOCANCEL_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnDisconnect::RunL()
// 
// ---------------------------------------------------------
//
void ConnMonConnDisconnect::RunL()
    {
    OstTraceFunctionEntry1( CONNMONCONNDISCONNECT_RUNL_ENTRY, this );
    // TODO: Is anything needed? 
    OstTraceFunctionExit1( CONNMONCONNDISCONNECT_RUNL_EXIT, this );
    }

// ---------------------------------------------------------
// ConnMonConnDisconnect::disconnectConnection(int iapId)
// Stop active connection.
// ---------------------------------------------------------
//
void ConnMonConnDisconnect::disconnectConnection(int iapId)
    {
    OstTraceFunctionEntry1( CONNMONCONNDISCONNECT_DISCONNECTCONNECTION_ENTRY, this );
    OstTrace1(
        TRACE_NORMAL,
        CONNMONCONNDISCONNECT_DISCONNECTCONNECTION,
        "ConnMonConnDisconnect::disconnectConnection;iapId=%d", iapId );

    // TODO: Error handling
    TUint count;
    TRequestStatus status;
    iMonitor.GetConnectionCount(count, status);
    User::WaitForRequest(status);

    for (TInt i = 1; i <= count; i++)
        {
        TUint numSubConnections;
        TUint iap = 0;
        TUint connectionId = 0;
        TInt ret = iMonitor.GetConnectionInfo(i, connectionId,
                numSubConnections);
        if (ret != KErrNone)
            {
            // error
            }
        iMonitor.GetUintAttribute(connectionId, 0, KIAPId, iap, status);
        User::WaitForRequest(status);
        if (status.Int() != KErrNone)
            {
            // error  
            }
        if (iap == iapId)
            {
            TInt KErr = iMonitor.SetBoolAttribute(connectionId, 0,
                    KConnectionStop, ETrue);
            }
        }

    OstTraceFunctionExit1( CONNMONCONNDISCONNECT_DISCONNECTCONNECTION_EXIT, this );
    }

//end of file
