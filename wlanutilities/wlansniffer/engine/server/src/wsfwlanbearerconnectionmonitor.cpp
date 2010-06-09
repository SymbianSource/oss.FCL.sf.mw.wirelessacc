/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CWsfWlanBearerConnectionMonitor
*
*/



//  EXTERNAL INCLUDES
#include <w32std.h> 
#include <apgtask.h>
#include <utf.h>
#include <cmpluginwlandef.h>
#include <commdbconnpref.h>
#include <wlanerrorcodes.h>
#include <cmconnectionmethodext.h>
#include <commdb.h>
#include <WlanCdbCols.h>



//  CLASS HEADER
#include "wsfwlanbearerconnectionmonitor.h"

//  INTERNAL INCLUDES
#include "wsfwlanmonitorobserver.h"
#include "wsflogger.h"
#include "wsfactivewaiter.h"
#include "wsfservercloseradapter.h"
#include "wsfcommon.h"


//  LOCAL DEFINITIONS
using namespace CMManager;


#ifdef _DEBUG
    _LIT( KWlanConnMonPanic, "wsfwlanconnmon" );
    #define __ASSERTD(x) __ASSERT_DEBUG( (x), \
                                User::Panic( KWlanConnMonPanic, __LINE__ ) );
#else
    #define __ASSERTD(x)
#endif                                


//  LOCAL CONSTANTS
/**
* Connection id referring to a non-existing connection
*/
static const TInt KNoConnection = -1;

/**
* Max allowed connection inactivity time in seconds
*/
static const TUint KMaxConnectionInactivityTime = 60;


/**
* Client polling interval in microseconds
*/
static const TUint KClientPollInterval = 5 * 1000 * 1000;


/**
* Number of trivial clients of a connection (Sniffer, AI/CP plugin etc)
*/
static const TUint KTrivialClientCount = 2;

/**
* List of UIDs of the trivial clients
*/
static const TUid KTrivialClientUids[KTrivialClientCount] = 
    {
    { 0x10281CAB },         // Sniffer server (wsfserver.exe)
    { 0x101fD9C5 }          // DHCP server (dhcpserv.exe)
    };

/**
* Browser UID
*/
const TUid KCRUidBrowser   = {0x10008D39};



//  CONSTRUCTION AND DESTRUCTION


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::NewL
// ---------------------------------------------------------------------------
//
CWsfWlanBearerConnectionMonitor* CWsfWlanBearerConnectionMonitor::NewL( 
                                  MWsfServerCloserAdapter& aServerCloser )
    {
    CWsfWlanBearerConnectionMonitor* thisPtr = NewLC( aServerCloser );
    CleanupStack::Pop( thisPtr );
    return thisPtr;
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::NewLC
// ---------------------------------------------------------------------------
//
CWsfWlanBearerConnectionMonitor* CWsfWlanBearerConnectionMonitor::NewLC( 
                                  MWsfServerCloserAdapter& aServerCloser )
    {
    CWsfWlanBearerConnectionMonitor* thisPtr = 
           new (ELeave) CWsfWlanBearerConnectionMonitor( aServerCloser );
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    return thisPtr;
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::~CWsfWlanBearerConnectionMonitor
// ---------------------------------------------------------------------------
//
CWsfWlanBearerConnectionMonitor::~CWsfWlanBearerConnectionMonitor()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::"
                 L"~CWsfWlanBearerConnectionMonitor" );
    iMonitor.CancelNotifications();
    iMonitor.Close();

    Cancel();

    if ( iConnectionOwned )
        {
        // in case Cancel() hasn't done the job
        TRAP_IGNORE( ShutdownOwnedConnectionL() );
        }
    else if ( iMonitoredAp )
        {
        
        }

    delete iClientPoll;

    iCmMgr.Close();
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::CWsfWlanBearerConnectionMonitor
// ---------------------------------------------------------------------------
//
CWsfWlanBearerConnectionMonitor::CWsfWlanBearerConnectionMonitor( 
                                MWsfServerCloserAdapter& aServerCloser ):
    CActive( CActive::EPriorityStandard ),
    iConnectionId( KNoConnection ),
    iMonitoring( EFalse ),
    iConnectingState( ECsIdle ),
    iServerCloser( aServerCloser )
    {
    CActiveScheduler::Add( this );
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::ConstructL()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::ConstructL" );
    iCmMgr.OpenL();
    iMonitor.ConnectL();
    iClientPoll = CPeriodic::NewL( CActive::EPriorityLow );
    FindWlanBearerConnectedL();
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::GetWlanBearerNameL
// ---------------------------------------------------------------------------
//
const TDesC& CWsfWlanBearerConnectionMonitor::GetWlanBearerNameL()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::GetWlanBearerNameL" );
    if ( iConnectionId == KNoConnection )
        {
        LOG_WRITE( "[no connection]" );
        return KNullDesC();
        }
        
    LOG_WRITEF( "connection name: %S", &iWlanNetworkName );
    return iWlanNetworkName;
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::FindWlanBearerConnectedL
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::FindWlanBearerConnectedL()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::FindWlanBearerConnectedL" );
    CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewLC();
    TUint connectionNumber( 0 );

    iMonitor.GetConnectionCount( connectionNumber, waiter->iStatus );
    waiter->WaitForRequest();
    
    if ( !waiter->iStatus.Int() )
        {
        TBool connectionIDFound = EFalse;
        for ( TUint i = 1; i <= connectionNumber; ++i )
            {
            TUint connectionId( 0 );
            TUint subConnectionCount( 0 );
            
            User::LeaveIfError( iMonitor.GetConnectionInfo( i, connectionId, 
                                                        subConnectionCount ) );
            if ( CheckConnectionDetailsL( connectionId ) )
                {
                LOG_WRITEF( "found connection %d", connectionId );
                connectionIDFound = ETrue;
                iConnectionId = connectionId;
                break;
                }
            }
        if ( !connectionIDFound )
            {
            LOG_WRITE( "Reset connection ID" );
            iConnectionId = KNoConnection;
            }
        }
    else
        {
        LOG_WRITEF( "GetConnectionCount failed error = %d", 
                                                    waiter->iStatus.Int() );
        }
        
    CleanupStack::PopAndDestroy( waiter );
    }
 
            
// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::EventL
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::EventL( 
                                       const CConnMonEventBase& aConnMonEvent )
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::EventL" );

    TConnMonEvent event = TConnMonEvent( aConnMonEvent.EventType() );
    TInt connectionId = aConnMonEvent.ConnectionId();

    LOG_WRITEF( "event = %d", (TInt)event );
    
    switch ( event )
        {
        case EConnMonCreateConnection:
            {
            LOG_WRITEF( "EConnMonCreateConnection id = %d", 
                        connectionId );
            break;
            }

        case EConnMonConnectionStatusChange:
            {
            LOG_WRITEF( "EConnMonConnectionStatusChange id = %d", 
                        connectionId );
                        
            const CConnMonConnectionStatusChange* statusChangeEvent = 
                        static_cast<const CConnMonConnectionStatusChange*>( 
                                                              &aConnMonEvent );
                        
            LOG_WRITEF( "EConnMonConnectionStatusChange status = %d", 
                        statusChangeEvent->ConnectionStatus() );
            
            if ( statusChangeEvent->ConnectionStatus() == KConnectionOpen &&
                 !iConnectionOwned )
                {
                LOG_WRITE( "connection status: open" );
                TBool wlanEvent( EFalse );
                
                wlanEvent = CheckConnectionDetailsL( connectionId );
                if ( wlanEvent )
                    {
                    LOG_WRITEF( "[%S] connected, id = %d", 
                                &iWlanNetworkName, connectionId );

                    // notify observer ...
                    iConnectionId = connectionId;
                    iObserver->ConnectionEstablishedL( iWlanNetworkName );
                    }
                }
            break;
            }                
            
        case EConnMonDeleteConnection:
            {
            LOG_WRITEF( "EConnMonDeleteConnection id = %d", connectionId );

            if ( connectionId == iConnectionId )
                {
                if ( iConnectionOwned )
                    {
                    LOG_WRITE( "connection was owned, manual closing" );
                    ShutdownOwnedConnectionL();
                    }
                else
                    {
                    LOG_WRITE( "connection was not owned" );
                    iWlanNetworkName.Zero();
                    iConnectionId = KNoConnection;
                    
                    // notify observer
                    iObserver->ConnectionLostL();
                    }
                }
            break;
            }
        
        default:
            {
            }
        }
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::CheckConnectionDetailsL
// ---------------------------------------------------------------------------
//
TBool CWsfWlanBearerConnectionMonitor::CheckConnectionDetailsL( 
                                                         TUint aConnectionId )
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::CheckConnectionDetailsL" );
    LOG_WRITEF( "aConnectionId: %d", aConnectionId );
    TConnMonBearerType bearerType( EBearerUnknown );

    TBool foundWlanBearer( EFalse );

    // Add only connections with valid id
    if ( aConnectionId > 0 )
        {
        CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewLC();
        iMonitor.GetIntAttribute( aConnectionId, 0, KBearer, 
                                  (TInt&)bearerType, 
                                  waiter->iStatus );
        waiter->WaitForRequest();
        LOG_WRITEF( "found bearer: %d", bearerType );        

        if ( waiter->iStatus.Int() == KErrNone )
            {
            LOG_WRITE( "status: KErrNone" );
            if ( bearerType == EBearerWLAN )
                {
                
                iMonitor.GetStringAttribute( aConnectionId, 0, KNetworkName, 
                                         iWlanNetworkName, waiter->iStatus );
                waiter->WaitForRequest();        
                if ( waiter->iStatus.Int() == KErrNone )
                    {
                    LOG_WRITEF( "WLAN network name: %S", &iWlanNetworkName );
                    foundWlanBearer = ETrue;
                    }
                
                }
            }
        else
            {
            LOG_WRITEF( "status: %d", waiter->iStatus.Int() );
            }
        CleanupStack::PopAndDestroy( waiter );
        }

    return foundWlanBearer;
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::StartMonitoringL
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::StartMonitoringL( 
                                           MWsfWlanMonitorObserver* aObserver )
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::StartMonitoringL" );        
    __ASSERTD( aObserver );

    iObserver = aObserver;
    if ( !iMonitoring )
        {
        iMonitoring = ETrue;
        iMonitor.NotifyEventL( *this );
        }
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::StopMonitoring
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::StopMonitoring()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::StopMonitoring" );        

    if ( iMonitoring )
        {
        iMonitoredAp = 0;
        iMonitoring = EFalse;
        iMonitor.CancelNotifications();
        }
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::ConnectBearer
// ---------------------------------------------------------------------------
//
TInt CWsfWlanBearerConnectionMonitor::ConnectBearer( TUint32 aIapId )
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::ConnectBearer" );
    
    if ( iConnectionId != KNoConnection || iConnectingState != ECsIdle )
        {
        // there is already a connection
        LOG_WRITE( "there is already a WLAN connection" );
        return KErrWlanConnAlreadyActive;
        }
    
    // self-completion
    if ( iConnectingState == ECsIdle )
        {
        LOG_WRITE( "initiating connection process" );
        iConnIap = aIapId;
        iConnectingState = ECsNotConnected;
        iConnectionOwned = ETrue;
        iServerCloser.WaitForOwnedConnection( ETrue );

        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::DisconnectBearer
// ---------------------------------------------------------------------------
//
TBool CWsfWlanBearerConnectionMonitor::DisconnectBearer()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::DisconnectBearer" );

    if ( iConnectionId != KNoConnection )
        {
        LOG_WRITEF( "closing connection id = %d", iConnectionId );
        iMonitor.SetBoolAttribute( iConnectionId, 0, KConnectionStop, ETrue );
        }

    return ( iConnectionId != KNoConnection );
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::AbortConnecting
// ---------------------------------------------------------------------------
//
TInt CWsfWlanBearerConnectionMonitor::AbortConnecting()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::AbortConnecting" );
    TInt result = KErrGeneral;
    
    if ( iConnectingState != ECsIdle )
        {
        LOG_WRITE( "connection in progress, aborting");
        iAborting = ETrue;
        
        if ( IsActive() && iConnectingState == ECsSocketOpened )
            {
            LOG_WRITE( "forcing connection to stop" );
            result = iConnection.Stop();
            }
        }
    return result;
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::MonitorAccessPoint
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::MonitorAccessPoint( TUint32 aIapId )    
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::MonitorAccessPoint" );
    iMonitoredAp = aIapId;
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::ConnectedWlanConnectionDetailsL
// ---------------------------------------------------------------------------
//
TBool CWsfWlanBearerConnectionMonitor::ConnectedWlanConnectionDetailsL( 
                                                     TWsfWlanInfo* aWlanInfo )
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::"
                 L"ConnectedWlanConnectionDetailsL" );
    
    LOG_WRITEF( "Monitor iConnectingState =%d and iConnectionId = %d", 
                         iConnectingState, iConnectionId );
    
    if ( iConnectingState == ECsIdle && iConnectionId == KNoConnection )
        {
        // Make sure that we have connection id
        FindWlanBearerConnectedL();
        }
    
    if ( iConnectionId == KNoConnection )
        {
        return EFalse;
        }
    
    // get the data's from the monitor add them to aWlanInfo..
    aWlanInfo->iConnectionState = EConnected;
    aWlanInfo->iVisibility = 1;
    aWlanInfo->iCoverage = 1;
    aWlanInfo->iNetworkName = KNullDesC8;
    
    CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewLC();
    
    // security mode
    TInt securityAttribute( EWlanConnectionSecurityOpen );
    iMonitor.GetIntAttribute( iConnectionId, 0, KSecurityMode, 
                              securityAttribute, waiter->iStatus );
    waiter->WaitForRequest();
    
    if ( waiter->iStatus.Int() != KErrNone )
        {
        aWlanInfo->iConnectionState = ENotConnected;
        CleanupStack::PopAndDestroy( waiter );
        return EFalse;
        }

    switch ( securityAttribute )
        {
        case EConnMonSecurityWep:
            {
            aWlanInfo->iSecurityMode = EWlanSecModeWep;
            break;
            }

        case EConnMonSecurity802d1x:
            {
            aWlanInfo->iSecurityMode = EWlanSecMode802_1x;
            break;
            }

        case EConnMonSecurityWpa:       // fall-through
        case EConnMonSecurityWpaPsk:
            {
            aWlanInfo->iSecurityMode = EWlanSecModeWpa;
            break;
            }

        case EConnMonSecurityOpen:      // fall-through
        default:
            {
            aWlanInfo->iSecurityMode = EWlanSecModeOpen;
            }
        }    
    LOG_WRITEF( "conn.secmode = %d", aWlanInfo->iSecurityMode );
    
    aWlanInfo->SetUsesPreSharedKey( 
            EConnMonSecurityWpaPsk == securityAttribute );
    
    LOG_WRITEF( "conn.usespresharedkey = %d", aWlanInfo->UsesPreSharedKey() );

    // network mode    
    TInt networkModeAttribute( 0 );
    
    iMonitor.GetIntAttribute( iConnectionId, 0, KNetworkMode, 
                              networkModeAttribute, waiter->iStatus );
    waiter->WaitForRequest();  
    
    if ( waiter->iStatus.Int() != KErrNone )
        {
        aWlanInfo->iConnectionState = ENotConnected;
        CleanupStack::PopAndDestroy( waiter );
        return EFalse;
        }

    if ( networkModeAttribute == EConnMonAdHoc )
        {
        aWlanInfo->iNetMode = EAdhoc;
        }
    else
        {
        aWlanInfo->iNetMode = EInfra;
        }
        
    LOG_WRITEF( "conn.netmode = %d", aWlanInfo->iNetMode );

    // iap id
    TUint iapIdAttribute( 0 );
    iMonitor.GetUintAttribute( iConnectionId, 0, KIAPId, 
                              iapIdAttribute, waiter->iStatus );
    waiter->WaitForRequest();  
    
    if ( waiter->iStatus.Int() != KErrNone )
        {
        aWlanInfo->iConnectionState = ENotConnected;
        CleanupStack::PopAndDestroy( waiter );
        return EFalse;
        }
    
    aWlanInfo->iIapId = iapIdAttribute;
    
    LOG_WRITEF( "conn.iap = %d", aWlanInfo->iIapId );

    // signal strength
    TInt signStrengthAttribute( 0 );          
    iMonitor.GetIntAttribute( iConnectionId, 0, KSignalStrength, 
                              signStrengthAttribute, waiter->iStatus );
    waiter->WaitForRequest(); 
	
	if ( waiter->iStatus.Int() != KErrNone )
        {
        aWlanInfo->iConnectionState = ENotConnected;
        CleanupStack::PopAndDestroy( waiter );
        return EFalse;
        }
	
    aWlanInfo->iStrengthLevel = signStrengthAttribute;
    LOG_WRITEF( "conn.signalstrength = %d", aWlanInfo->iStrengthLevel );
    
    if ( aWlanInfo->iIapId )
        {
        LOG_WRITE( "IAP id != 0" );
        
        RCmConnectionMethodExt cm = iCmMgr.ConnectionMethodL( aWlanInfo->iIapId );
        CleanupClosePushL( cm );
        HBufC* buf = NULL;
        
        if ( iCmMgr.EasyWlanIdL() != aWlanInfo->iIapId )
            {
            LOG_WRITE( "not EasyWLAN" );
            buf = cm.GetStringAttributeL( ECmName );
            }
        else
            {
            LOG_WRITE("Sniffer:EasyWLAN IAP ");
            aWlanInfo->iIapId = 0;
            buf = cm.GetStringAttributeL( EWlanUsedSSID );
            }
        
        if ( buf )
            {
            TInt error = CnvUtfConverter::ConvertFromUnicodeToUtf8( 
                                                        aWlanInfo->iSsid, 
                                                        *buf  );
            if ( error )
                {
                LOG_WRITE( "ConvertFromUnicodeToUtf8 failed");
                aWlanInfo->iSsid.Copy( *buf );
                }
            delete buf;
            }
        CleanupStack::PopAndDestroy( &cm ); //cm
        }
    else
        {
        LOG_WRITE( "IAP id = 0" );
        }

#ifdef _DEBUG
    // Iap & Ssid logging   
    HBufC* nameUnicode = aWlanInfo->GetIapNameAsUnicodeLC();
    LOG_WRITEF( "aWlanInfo->iNetworkName: [%S]", nameUnicode );
    CleanupStack::PopAndDestroy( nameUnicode );
    
    HBufC* ssidUnicode = aWlanInfo->GetSsidAsUnicodeLC();
    LOG_WRITEF( "aWlanInfo->tmpSsid: [%S]", ssidUnicode );
    CleanupStack::PopAndDestroy( ssidUnicode );
#endif

    CleanupStack::PopAndDestroy( waiter );
    
    return ETrue;    
    }

   
// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::IsConnected
// ---------------------------------------------------------------------------
//
TBool CWsfWlanBearerConnectionMonitor::IsConnected()
    {
    return ( iConnectionId != KNoConnection );
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::CheckClientCount
// ---------------------------------------------------------------------------
//
TInt CWsfWlanBearerConnectionMonitor::CheckClientCount( TAny* aPtr )
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::CheckClientCount" );
    CWsfWlanBearerConnectionMonitor* self = 
                         static_cast<CWsfWlanBearerConnectionMonitor*>( aPtr );
    TInt err( KErrNone );
    
    if ( !self->iClientCountMutex )
        {
        // in some rare cases this function is reentered because of the 
        // active scheduler waits, so we need a mutex
        self->iClientCountMutex = ETrue;
        
        TRAP( err, self->CheckClientCountL() );
        
        self->iClientCountMutex = EFalse;
        }    

    return err;
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::CheckClientCountL
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::CheckClientCountL()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::CheckClientCountL" );
    
    RWsSession wsSession;
    if ( KErrNone == wsSession.Connect() )
        {
        LOG_WRITE( "Find browser task" );
        TApaTaskList taskList( wsSession );
        TApaTask task = taskList.FindApp( KCRUidBrowser );
        if ( task.Exists() )
            {
            LOG_WRITE( "Browser is running - auto disconnect to false" );
            iAutoDisconnect = EFalse;
            iInactivityStart.UniversalTime();
            }
        else
            {
            LOG_WRITE( "Browser is not running - auto disconnect to true" );
            iAutoDisconnect = ETrue;
            }
        wsSession.Close();
        }
    else
        {
        LOG_WRITE( "Session connect failed" );
        }
    
    if ( iAutoDisconnect )
        {
        CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewL();
        
        TConnMonClientEnumBuf clientInfo;
    
        iMonitor.GetPckgAttribute( iConnectionId, 0, 
                                   KClientInfo, 
                                   clientInfo, 
                                   waiter->iStatus );
        waiter->WaitForRequest();
        delete waiter;
    
        // get the client count
        iClientCount = clientInfo().iCount;
        
        // decrease count by each trivial client (Sniffer server, DHCP etc)
        for ( TInt i( 0 ); i < clientInfo().iCount; ++i )
            {
            for ( TInt j( 0 ); j < KTrivialClientCount; ++j )
                {
                if ( clientInfo().iUid[i] == KTrivialClientUids[j] )
                    {
                    --iClientCount;
                    LOG_WRITEF( "trivial client [0x%08X] discarded", 
                                clientInfo().iUid[i].iUid );                
                    break;
                    }
                }
            }
        
        LOG_WRITEF( "iClientCount = %d (trivial clients:%d)", 
                    iClientCount,
                    clientInfo().iCount - iClientCount );
    
        if ( iClientCount )
            {
            LOG_WRITE( "reset the inactivity start time to current time" );
            // there are more clients than the default ones ->
            // connection is considered active ->
            // reset the inactivity start time to current time
            iInactivityStart.UniversalTime();
            }
        else
            {
            // there are only trivial clients of the connection
            // this means inactivity, so check the time elapsed
            LOG_WRITEF( "Connection monitor state = %d", iConnectingState );
            TTime now;
            now.UniversalTime();

            if ( iInactivityStart + TTimeIntervalSeconds( 
                                       KMaxConnectionInactivityTime ) <= now )
                {
                // inactivity time limit elapsed, connection should be stopped
                LOG_WRITE( "inactivity threshold reached, disconnecting..." );
                DisconnectBearer();
                }
            
            }
        
        }
    
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::ControlDisconnectTimer
// ---------------------------------------------------------------------------
//
TBool CWsfWlanBearerConnectionMonitor::ControlDisconnectTimer( 
                                                           TUint aAdcCommand )
    {
    LOG_ENTERFN("CWsfWlanBearerConnectionMonitor::ControlDisconnectTimer");    
    
    if ( iConnectionOwned )
        {
        if ( aAdcCommand & EAdcTimerReset )
            {
            // reset inactivity time
            LOG_WRITE( "timer reset" );
            iInactivityStart.UniversalTime();
            }

        if ( aAdcCommand & EAdcStartTimer ) 
            {
            if ( !iAutoDisconnect )
                {
                LOG_WRITE( "timer started" );
                iAutoDisconnect = ETrue;
                }
            else
                {
                LOG_WRITE( "timer had already been started!" );
                }
            }
        else
            {
            iAutoDisconnect = EFalse;
            LOG_WRITE( "timer stopped" );
            }

        }
    else
        {
        LOG_WRITE( "connection is not owned!" );
        }
        
    return iAutoDisconnect;
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::ReleaseShutdownMutex
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::ReleaseShutdownMutex( TAny* aPtr )
    {
    CWsfWlanBearerConnectionMonitor* self = 
                        static_cast<CWsfWlanBearerConnectionMonitor*>( aPtr );
    self->iShutdownMutex = EFalse;
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::ShutdownOwnedConnectionL
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::ShutdownOwnedConnectionL()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::ShutdownOwnedConnectionL" );
    
    // Make sure that the internal status is reset.
    iConnectionId = KNoConnection;
    
    if ( iShutdownMutex )
        {
        return;
        }
        
    // make sure we don't lock ourselves forever
    CleanupStack::PushL( TCleanupItem( ReleaseShutdownMutex, this ) );
    iShutdownMutex = ETrue;
    
    
    // roll-back the changes
    switch ( iConnectingState )
        {
        case ECsConnected:
            {
            // cancelling the periodic callback
            iClientPoll->Cancel();
            }
            
        case ECsConnectionCreated:          // fall-through
            {

            }
            
        case ECsSocketOpened:               // fall-through
            {
            // closing the connection handle
            LOG_WRITE( "closing the connection handle" );
            iConnection.Close();
            iSocketServ.Close();
            }
            
        case ECsNotConnected:               // fall-through
            {
            // nothing to put here
            }
            
        }


    // reset the state machine
    iConnectingState = ECsIdle;
    iConnectionOwned = EFalse;

    // tell the engine we have released the IAP 
    iObserver->ConnectedIapReleasedL();

    
    iServerCloser.WaitForOwnedConnection( EFalse );
    
    if ( !iAborting )
        {
        // notify observers only if connection creation 
        // was not aborted
        iObserver->ConnectionLostL();
        }

    iAborting = EFalse;
    
    CleanupStack::PopAndDestroy( 1 ); // ReleaseShutdownMutex()
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::RunL
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::RunL()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::RunL" );

    if ( iAborting )
        {
        // if connection creation was aborted, do the cleanup
        ShutdownOwnedConnectionL();
        return;
        }
       
    switch ( iConnectingState )
        {
        case ECsNotConnected:
            {
            LOG_WRITE( "<ENotConnected>" );

            TInt err( KErrNone );
            err = iSocketServ.Connect();
            
            if ( !err )
                {
                err = iConnection.Open( iSocketServ );
                }
            
            if ( err )
                {
                // there was an error while connecting
                LOG_WRITEF( "connection.Open error = %d", err );
                ShutdownOwnedConnectionL();
        
                iObserver->ConnectingFailedL( err );
                break;
                }            

            TCommDbConnPref connPref;
            connPref.SetIapId( iConnIap );
            connPref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );

            iConnectingState = ECsSocketOpened;
            SetActive();
            iConnection.Start( connPref, iStatus );
            break;
            }
            
        case ECsSocketOpened:
            {
            LOG_WRITE( "<ESocketOpened>" );
            if ( iStatus.Int() != KErrNone )
                {
                // there was an error while connecting
                LOG_WRITE( "connection.Start error" );
                ShutdownOwnedConnectionL();
        
                iObserver->ConnectingFailedL( iStatus.Int() );

                break;
                }

            LOG_WRITE( "connection.Start OK" );

            // get the connection id
            TRAPD( error, FindWlanBearerConnectedL(); );
            if ( error || iConnectionId == KNoConnection )
                {
                // At this point we really need the connection id if it exists
                // without connection id server is in very unbalanced state
                LOG_WRITEF( "FindWlanBearerConnectedL error=%d", error );
                ShutdownOwnedConnectionL();
                        
                iObserver->ConnectingFailedL( ( error )? error : KErrGeneral );

                break;
                }

            iConnectingState = ECsConnectionCreated;
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, KErrNone );
            break;
            }
            
        case ECsConnectionCreated:
            {
            LOG_WRITE( "<EConnectionCreated>" );
            
            // start monitoring the iap
            MonitorAccessPoint( iConnIap );
            
            // reset inactivity time
            iInactivityStart.UniversalTime();
            iAutoDisconnect = EFalse;
            
            iClientPoll->Start( 
                           TTimeIntervalMicroSeconds32( KClientPollInterval ),
                           TTimeIntervalMicroSeconds32( KClientPollInterval ),
                           TCallBack( CheckClientCount, this ) );
                                
            LOG_WRITE( "connection client polling started" );

            // notify observers of the connection name
            iObserver->ConnectionEstablishedL( iWlanNetworkName );
            
            iConnectingState = ECsConnected;
            break;
            }
            
        default:
            {
            }
        }
    
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::DoCancel
// ---------------------------------------------------------------------------
//
void CWsfWlanBearerConnectionMonitor::DoCancel()
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::DoCancel" );

    // we were in the middle of a connection creation process    
    iAborting = ETrue;
    TRAP_IGNORE( ShutdownOwnedConnectionL() );
    }


// ---------------------------------------------------------------------------
// CWsfWlanBearerConnectionMonitor::RunError
// ---------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CWsfWlanBearerConnectionMonitor::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfWlanBearerConnectionMonitor::RunError" );    
    LOG_WRITEF( "error = %d", aError );
#else
TInt CWsfWlanBearerConnectionMonitor::RunError( TInt /*aError*/ )
    {
#endif
    return KErrNone;
    }

    
