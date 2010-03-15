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
* Description:  Implementation of CWsfEngine
*
*/



//  EXTERNAL INCLUDES
#include <commsdattypesv1_1.h>
#include <commsdat.h>
#include <cmconnectionmethodext.h>
#include <cmpluginwlandef.h>

//  CLASS HEADER
#include "wsfengine.h"


//  INTERNAL INCLUDES
#include "wsfwlanscanner.h"
#include "wsfwlansettingsaccessor.h"
#include "wsfwlanbearerconnectionmonitor.h"
#include "wsfengineobserver.h"
#include "wsfservercloseradapter.h"
#include "wsfactivewaiter.h"

#include "wsflogger.h"



//  LOCAL DEFINITIONS
using namespace CommsDat;


/**
* Maximum number of retries when trying to delete a temporary IAP but
* the connection is locking it
*/
static const TInt KMaxIapDeletionRetries = 10;


/**
* Time to wait betweed two IAP deletion retries, in microseconds
*/
static const TUint KDelayBetweenDeletionRetries = 500 * 1000;





//  CONSTRUCTION AND DESTRUCTION

// ----------------------------------------------------------------------------
// CWsfEngine::NewL
// ----------------------------------------------------------------------------
//
CWsfEngine* CWsfEngine::NewL( MWsfServerCloserAdapter& aServerCloser )
    {
    CWsfEngine* self = CWsfEngine::NewLC( aServerCloser );
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfEngine::NewLC
// ----------------------------------------------------------------------------
//
CWsfEngine* CWsfEngine::NewLC( MWsfServerCloserAdapter& aServerCloser )
    {
    CWsfEngine* self = new ( ELeave ) CWsfEngine( aServerCloser );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfEngine::~CWsfEngine
// ----------------------------------------------------------------------------
//
CWsfEngine::~CWsfEngine()
    {
    LOG_ENTERFN( "CWsfEngine::~CWsfEngine" );
    
    delete iWlanBearerMonitor;
    delete iScanner;
    delete iDbSession;
    iObservers.Reset();
    
    if ( iMonitoredIap && 
         ( iIapPersistence == EIapExpireOnDisconnect ||
           iIapPersistence == EIapNestedExpireOnDisconnect ||
           iIapPersistence == EIapExpireOnBrowserExit ||
           iIapPersistence == EIapExpireOnShutdown ) )
        {
        TRAP_IGNORE( DeleteIapL( iMonitoredIap ) );
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::CWsfEngine
// ----------------------------------------------------------------------------
//
CWsfEngine::CWsfEngine( MWsfServerCloserAdapter& aServerCloser ):
    iServerCloser( aServerCloser ),
    iEnableScanning( 0 ),
    iIapPersistence( EIapDeleted )
    {
    }


// ----------------------------------------------------------------------------
// CWsfEngine::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfEngine::ConstructL()
    {
    LOG_CREATE;
    LOG_ENTERFN( "CWsfEngine::ConstructL" );
    iDbSession = CMDBSession::NewL( CMDBSession::LatestVersion() );
    iScanner = CWsfWlanScanner::NewL( *iDbSession );
 
    iScanner->SetObserver( *this );

    iWlanBearerMonitor = CWsfWlanBearerConnectionMonitor::NewL( iServerCloser );
    iScanner->SetConnectionDetailProvider( *iWlanBearerMonitor );

    iWlanBearerMonitor->StartMonitoringL( this );

    if ( iWlanBearerMonitor->GetWlanBearerNameL() != KNullDesC() )
        {
        HBufC* name = ConnectedWlanSsidL();
        CleanupStack::PushL( name );
        iScanner->ConnectionEstablishedL( *name );
        CleanupStack::PopAndDestroy( name );
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::AttachL
// ----------------------------------------------------------------------------
//
void CWsfEngine::AttachL( MWsfEngineObserver* aObserver )
    {
    LOG_ENTERFN( "CWsfEngine::AttachL" );
    
    iObservers.AppendL( aObserver );
    }


// ----------------------------------------------------------------------------
// CWsfEngine::Detach
// ----------------------------------------------------------------------------
//
void CWsfEngine::Detach( MWsfEngineObserver* aObserver )
    {
    LOG_ENTERFN( "CWsfEngine::Detach" );
    TInt index = iObservers.Find( aObserver );
    if ( index != KErrNotFound )
        {
        iObservers.Remove( index );
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::GetScanResults
// ----------------------------------------------------------------------------
//
HBufC8* CWsfEngine::GetScanResults()
    {
    LOG_ENTERFN( "CWsfEngine::GetScanResults" );
    return iScanner->ScanResults();
    }


// ----------------------------------------------------------------------------
// CWsfEngine::EnableScanningL
// ----------------------------------------------------------------------------
//
void CWsfEngine::EnableScanningL()
    {
    LOG_ENTERFN( "CWsfEngine::EnableScanningL" );

    iEnableScanning++;
    LOG_WRITEF( "++iEnableScanning = %d", iEnableScanning );
    
    if ( iEnableScanning == 1 )
        {
        LOG_WRITE( "CWsfEngine::EnableScanningL - start scanner" );
        if ( !iScanner->IsActive() )
            {
            iScanner->StartScanningL();
            }
        LOG_WRITE( "CWsfEngine::EnableScanningL start scanner done" );
        for ( TInt i( 0 ); i< iObservers.Count(); ++i )
            {
            (iObservers[i])->ScanEnabledL();
            }
        }
    else 
        {
        // the scanning is already active - stop and reactivate to complete the
        // new monitor in sensible time        
        if ( iScanner->IsActive() )
            {
            iScanner->StopScanning();
            }
        iScanner->StartScanningL();
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::IsScanning
// ----------------------------------------------------------------------------
//
TBool CWsfEngine::IsScanning()
    {
    return iEnableScanning;
    }
    

// ----------------------------------------------------------------------------
// CWsfEngine::RefreshScanResults
// ----------------------------------------------------------------------------
//
TBool CWsfEngine::RefreshScanResults()
    {
    LOG_ENTERFN( "CWsfEngine::RefreshScanResults" );
    return iScanner->RestartScanning();
    }


// ----------------------------------------------------------------------------
// CWsfEngine::MonitorAccessPointL
// ----------------------------------------------------------------------------
//
void CWsfEngine::MonitorAccessPointL( TUint32 aIapId )
    {
    LOG_ENTERFN( "CWsfEngine::MonitorAccessPointL" );
    
    iMonitoredIap = aIapId;

    if ( iIapPersistence == EIapExpireOnShutdown )
        {
        // web browsing with a temporary IAP
        LOG_WRITE( "EIapExpireOnShutdown => EIapExpireOnBrowserExit" );
        SetIapPersistenceL( EIapExpireOnBrowserExit );
        iServerCloser.WaitForBrowserExit( ETrue );
        
        // semaphore to keep the order of calls
        iSuppressIapDeletion = ETrue;  
        }
    else if ( iIapPersistence == EIapExpireOnDisconnect )
        {
        LOG_WRITE( "EIapExpireOnDisconnect => EIapNestedExpireOnDisconnect" );
        SetIapPersistenceL( EIapNestedExpireOnDisconnect );
        }
 
    
    LOG_WRITEF( "iMonitoredIap = %d", iMonitoredIap );
    }


// ----------------------------------------------------------------------------
// CWsfEngine::DisableScanningL
// ----------------------------------------------------------------------------
//
void CWsfEngine::DisableScanningL()
    {
    LOG_ENTERFN( "CWsfEngine::DisableScanningL" );

    iEnableScanning--;
    LOG_WRITEF( "--iEnableScanning = %d", iEnableScanning );

    if ( !iEnableScanning )
        {
        LOG_WRITE( "CWsfEngine::DisableScanning stop scanner" );

        iScanner->StopScanning();
        LOG_WRITE( "CWsfEngine::DisableScanning stopped scanner" );

        for ( TInt i( 0 ); i < iObservers.Count(); ++i )
            {
            (iObservers[i])->ScanDisabledL();
            }
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::ConnectedWlanSsidL
// ----------------------------------------------------------------------------
//
HBufC* CWsfEngine::ConnectedWlanSsidL()
    {
    LOG_ENTERFN( "CWsfEngine::ConnectedWlanIapNameL" );
    return iWlanBearerMonitor->GetWlanBearerNameL().AllocL();
    }


// ----------------------------------------------------------------------------
// CWsfEngine::ConnectedWlanConnectionDetailsL
// ----------------------------------------------------------------------------
//
TBool CWsfEngine::ConnectedWlanConnectionDetailsL( TWsfWlanInfo& aWlanInfo )
    {
    LOG_ENTERFN( "CWsfEngine::ConnectedWlanConnectionDetailsL" );
    return iWlanBearerMonitor->ConnectedWlanConnectionDetailsL( &aWlanInfo );
    }


// ----------------------------------------------------------------------------
// CWsfEngine::WlanScanCompleteL
// ----------------------------------------------------------------------------
//
void CWsfEngine::WlanScanCompleteL()
    {
    LOG_ENTERFN( "CWsfEngine::WlanScanCompleteL" );

    for ( TInt i( 0 ); i < iObservers.Count(); ++i )
        {
        (iObservers[i])->WlanListChangedL();
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::WlanScanStarted
// ----------------------------------------------------------------------------
//
void CWsfEngine::WlanScanStarted()
    {
    LOG_ENTERFN( "CWsfEngine::WlanScanStarted" );
    }


// ----------------------------------------------------------------------------
// CWsfEngine::NotifyError
// ----------------------------------------------------------------------------
//
void CWsfEngine::NotifyError( TInt aError )
    {
    LOG_ENTERFN( "CWsfEngine::NotifyError" );

    for ( TInt i( 0 ); i < iObservers.Count(); ++i )
        {
        TRAP_IGNORE( (iObservers)[i]->NotifyErrorL( aError ) );
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::ConnectionEstablishedL
// ----------------------------------------------------------------------------
//
void CWsfEngine::ConnectionEstablishedL( const TDesC& aConnectionName )
    {
    LOG_ENTERFN( "CWsfEngine::ConnectionEstablishedL" );
    // set the scanner to check connected accounts
    iScanner->ConnectionEstablishedL( aConnectionName );
    
    iScanner->RestartScanning();

    // notify the observers
    for ( TInt i( 0 ); i < iObservers.Count(); ++i )
        {
        (iObservers)[i]->ConnectedL();
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::ConnectionLostL
// ----------------------------------------------------------------------------
//
void CWsfEngine::ConnectionLostL()
    {
    LOG_ENTERFN( "CWsfEngine::ConnectionLostL" );

    // check temporarity here as well since it is not guaranteed that 
    // the connection was owned and ConnectedIapReleasedL was called
    if ( iSuppressIapDeletion )
        {
        // web browsing with a temporary IAP
        LOG_WRITE( "IAP deletion suppressed" );
        iSuppressIapDeletion = EFalse;
        }
    else if ( iMonitoredIap )
        {
        if ( iIapPersistence == EIapExpireOnDisconnect ||
             iIapPersistence == EIapExpireOnBrowserExit )
            {
            DeleteIapL( iMonitoredIap );
            iIapPersistence = EIapDeleted;
            iMonitoredIap = 0;        
            }
        else if ( iIapPersistence == EIapNestedExpireOnDisconnect )
            {
            iIapPersistence = EIapExpireOnBrowserExit;
            }
        }

    iScanner->ConnectionLostL();
    iScanner->RestartScanning();

    // notify the observers
    for ( TInt i( 0 ); i < iObservers.Count(); ++i )
        {
        (iObservers)[i]->DisconnectedL();
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::ConnectingFailedL
// ----------------------------------------------------------------------------
//
void CWsfEngine::ConnectingFailedL( TInt aError )
    {
    LOG_ENTERFN( "CWsfEngine::ConnectingFailedL" );

    for ( TInt i( 0 ); i < iObservers.Count(); ++i )
        {
        (iObservers)[i]->ConnectingFailedL( aError );
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::ConnectedIapReleasedL
// ----------------------------------------------------------------------------
//
void CWsfEngine::ConnectedIapReleasedL()
    {
    LOG_ENTERFN( "CWsfEngine::ConnectedIapReleasedL" );
    
    // deleting temporary IAP if necessary
    if ( !iSuppressIapDeletion && iMonitoredIap )
        {
        if ( iIapPersistence == EIapExpireOnDisconnect ||
             iIapPersistence == EIapExpireOnBrowserExit )
            {
            DeleteIapL( iMonitoredIap );
            iIapPersistence = EIapDeleted;
            iMonitoredIap = 0;        
            }
        else if ( iIapPersistence == EIapNestedExpireOnDisconnect )
            {
            iIapPersistence = EIapExpireOnBrowserExit;
            iSuppressIapDeletion = ETrue;
            }
        }
    }


// ----------------------------------------------------------------------------
// CWsfEngine::DeleteIapL
// ----------------------------------------------------------------------------
//
void CWsfEngine::DeleteIapL( TUint32 aIapId )
    {
    LOG_ENTERFN( "CWsfEngine::DeleteIapL" );

    LOG_WRITEF( "deleting temporary IAP id = %d", aIapId );
    RCmManagerExt cmMgr;
    cmMgr.OpenLC();
    
    RCmConnectionMethodExt ictCm = cmMgr.ConnectionMethodL( aIapId );
    CleanupClosePushL( ictCm );

    CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewLC();

    RTimer timer;
    timer.CreateLocal();

    // trying to delete the temporary IAP
    // sometimes the RConnection is still locking the IAP when 
    // we want to delete it, so we might have to try a couple of times
    TInt counter( KMaxIapDeletionRetries );
    TInt err( KErrNone );
    
    while ( counter )
        {
        LOG_WRITEF( "trying to delete (#%d)", 
                    KMaxIapDeletionRetries - counter );
                    
        TRAP( err, ictCm.DeleteL() );

        if ( !err || err == KErrNotFound )
            {
            // if success or the IAP was not found (strange but happens), break
            break;
            }

        --counter;
        if ( err && counter )
            {
            LOG_WRITEF( "deletion failed with error = %d", err );
            LOG_WRITE( "RConnection is still locking the IAP, waiting..." );
            
            timer.After( waiter->iStatus, TTimeIntervalMicroSeconds32( 
                                      KDelayBetweenDeletionRetries ) );
            waiter->WaitForRequest();
            }
        }
    

    timer.Close();
    CleanupStack::PopAndDestroy( waiter );
    
    if ( !err )
        {
        LOG_WRITE( "IAP deleted." );
        }
    else
        {
        LOG_WRITE( "IAP couldn't be deleted!" );
        }
        
    CleanupStack::PopAndDestroy( &ictCm );
    CleanupStack::PopAndDestroy( &cmMgr );    
    }


// ----------------------------------------------------------------------------
// CWsfEngine::ConnectWlanL
// ----------------------------------------------------------------------------
//
TInt CWsfEngine::ConnectWlanL( TUint32 aIapId, 
                               TWsfIapPersistence aPersistence )
    {
    LOG_ENTERFN( "CWsfEngine::ConnectWlanL" );
    LOG_WRITEF( "iapId = %d, persistence = %d", aIapId, aPersistence );
    
    // if we have already been monitoring an IAP (different from the 
    // new one), clean that one up properly
    if ( iMonitoredIap && iIapPersistence == EIapExpireOnBrowserExit )
        {
        // user switched back from browser to sniffer and issued connect
        if ( iMonitoredIap != aIapId )
            {
            // selected network is different, so we must clean up the old one
            DeleteIapL( iMonitoredIap );
            iIapPersistence = EIapDeleted;
            iMonitoredIap = 0;        
            }
        else if ( iMonitoredIap == aIapId )
            {
            // selected networks are the same
            aPersistence = EIapNestedExpireOnDisconnect;
            }
        iSuppressIapDeletion = EFalse;
        }

    TInt ret = iWlanBearerMonitor->ConnectBearer( aIapId );
    
    if ( !ret )
        {
        // successfully initiated connection
        LOG_WRITE( "connection init succeded" );
        iMonitoredIap = aIapId;
        SetIapPersistenceL( aPersistence );
        }
    
    return ret;
    }


// ----------------------------------------------------------------------------
// CWsfEngine::DisconnectWlan
// ----------------------------------------------------------------------------
//
TBool CWsfEngine::DisconnectWlan()
    {
    LOG_ENTERFN( "CWsfEngine::DisconnectWlan" );
    return iWlanBearerMonitor->DisconnectBearer();
    }
    

// ----------------------------------------------------------------------------
// CWsfEngine::IsConnected
// ----------------------------------------------------------------------------
//
TBool CWsfEngine::IsConnected()
    {
    return iWlanBearerMonitor->IsConnected();
    } 


// ---------------------------------------------------------------------------
// CWsfEngine::AbortConnectingL
// ---------------------------------------------------------------------------
//
void CWsfEngine::AbortConnectingL()
    {
    LOG_ENTERFN("CWsfEngine::AbortConnectingL");

    TInt result = iWlanBearerMonitor->AbortConnecting();
    
    LOG_WRITEF( "abort connection result = %d", result );
    
    if ( result == KErrNone && iMonitoredIap )
        {
        if ( iIapPersistence == EIapExpireOnDisconnect )
            {
            LOG_WRITE( "EIapNestedExpireOnDisconnect => EIapDeleted" );
            DeleteIapL( iMonitoredIap );
            iIapPersistence = EIapDeleted;
            iMonitoredIap = 0;        
            }
        else if ( iIapPersistence == EIapNestedExpireOnDisconnect )
            {
            LOG_WRITE( "EIapNestedExpireOnDisconnect => "
                       L"EIapExpireOnBrowserExit" );
            iIapPersistence = EIapExpireOnBrowserExit;
            }
        }
    }
    

// ---------------------------------------------------------------------------
// CWsfEngine::AbortScanning
// ---------------------------------------------------------------------------
//
void CWsfEngine::AbortScanning()
    {
    LOG_ENTERFN("CWsfEngine::AbortScanning");    
    
    iScanner->AbortScanning();
    }


// ---------------------------------------------------------------------------
// CWsfEngine::SetIapPersistenceL
// ---------------------------------------------------------------------------
//
TBool CWsfEngine::SetIapPersistenceL( TWsfIapPersistence aPersistence )
    {
    LOG_ENTERFN("CWsfEngine::SetIapPersistenceL");
    if ( aPersistence == EIapForcedExpiry )
        {
        if ( iMonitoredIap && 
             ( iIapPersistence == EIapExpireOnBrowserExit ||
               iIapPersistence == EIapExpireOnShutdown ) )
            {
            LOG_WRITE( "forced IAP expiry" );
            DeleteIapL( iMonitoredIap );
            iIapPersistence = EIapDeleted;
            iMonitoredIap = 0;
            }
        else if ( iIapPersistence == EIapNestedExpireOnDisconnect )
            {
            LOG_WRITE( "reverting to EIapExpireOnDisconnect" );
            iIapPersistence = EIapExpireOnDisconnect;
            }

        iServerCloser.WaitForBrowserExit( EFalse );
        }
    else 
        {
        LOG_WRITEF( "persistence = %d", aPersistence );
        iIapPersistence = aPersistence;
        }        
    
    return ( iMonitoredIap );
    }


// ---------------------------------------------------------------------------
// CWsfEngine::ControlDisconnectTimer
// ---------------------------------------------------------------------------
//
TBool CWsfEngine::ControlDisconnectTimer( TUint aAdcCommand )
    {
    LOG_ENTERFN("CWsfEngine::ControlDisconnectTimer");    
    
    return iWlanBearerMonitor->ControlDisconnectTimer( aAdcCommand );
    }

// ---------------------------------------------------------------------------
// CWsfEngine::ResetSuppressIapDeletion
// ---------------------------------------------------------------------------
//
void CWsfEngine::ResetSuppressIapDeletion()
    {
    LOG_ENTERFN( "CWsfEngine::ResetSuppresIapDeletion" );
    
    iSuppressIapDeletion = EFalse;
    }

// End of file
