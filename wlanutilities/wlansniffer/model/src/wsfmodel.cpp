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
* Description:  Implementation of CWsfModel
*
*/


//  EXTERNAL INCLUDES
#include <eikenv.h>
#include <apgcli.h>
#include <apgtask.h>
#include <ictsclientinterface.h>
#include <cmmanagerext.h>
#include <cmdestinationext.h>
#include <cmpluginwlandef.h>
#include <centralrepository.h>
#include <internetconnectivitycrkeys.h>
#include <featmgr.h>
#include <sysutil.h>
#include <wlanerrorcodes.h>

//  CLASS HEADER
#include "wsfmodel.h"

//  INTERNAL INCLUDES
#include "wsfwlaninfoarray.h"
#include "wsfwlaniapwizard.h"
#include "wsfapplauncher.h"
#include "wsfmodelobserver.h"
#include "wsfstatechangeobserver.h"
#include "wsfscreensaverwatcher.h"
#include "wsflogger.h"

using namespace CMManager;


/**
* UID of helper application 
* used when model is instantiated in the active idle
*/
static const TUid KHelperApUid = { 0x10281CEB };

/**
* UID of Wlan Login application (hsbrowser)
* used when launching WLAN Login application
*/
static const TInt KBrowserUid = { 0x2000AFCC };

/**
* Estimated overhead for access point creation 
*/
const TInt KEstimatedOverhead = 8192;


// ----------------------------------------------------------------------------
// CWsfModel::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfModel* CWsfModel::NewL( MWsfStateChangeObserver& aObserver,
                                     const TBool aScreenSaverAware )
    {
    CWsfModel* self = CWsfModel::NewLC( aObserver, aScreenSaverAware );
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfModel::NewLC
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfModel* CWsfModel::NewLC( MWsfStateChangeObserver& aObserver,
                                      const TBool aScreenSaverAware )
    {
    CWsfModel* self = new( ELeave ) CWsfModel;
    CleanupStack::PushL( self );
    self->ConstructL( aObserver, aScreenSaverAware );
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfModel::~CWsfModel
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfModel::~CWsfModel()
    {
    iSession.CancelNotifyEvent();
    iSession.Close();
    delete iScreenSaverWatcher;
    delete iArray;   
    delete iObservedWlan;
    delete iBrowserLauncher;
    }


// ----------------------------------------------------------------------------
// CWsfModel::CWsfModel
// ----------------------------------------------------------------------------
//
CWsfModel::CWsfModel(): 
    iEikEnv( CEikonEnv::Static() ),
    iRefreshing( EFalse ),
    iIctEnded( EFalse ),
    iKeepConnection( EFalse ),
    iConnectOnly( EFalse )
    {
    }


// ----------------------------------------------------------------------------
// CWsfModel::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfModel::ConstructL( MWsfStateChangeObserver& aObserver, 
                            const TBool aScreenSaverAware )
    {
    iArray = CWsfWlanInfoArray::NewL(); 
    iBrowserLauncher = CWsfAppLauncher::NewL();
    User::LeaveIfError( iSession.Connect() ); 
    iSession.NotifyEventL( aObserver );
    if ( aScreenSaverAware )
        {
        iScreenSaverWatcher = CWsfScreenSaverWatcher::NewL( *this );
        }
    }


// ----------------------------------------------------------------------------
// CWsfModel::SetEngineObserver
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::SetEngineObserver( MWsfModelObserver* aObserver )
    {
    iObserver = aObserver;
    }


// ----------------------------------------------------------------------------
// CWsfModel::GetWlanListL
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfWlanInfoArray* CWsfModel::GetWlanListL()
    {
    LOG_ENTERFN( "CWsfModel::GetWlanListL" );
    iArray->Reset();
    iSession.UpdateWlanListL( iArray );
    return iArray;
    }


// ----------------------------------------------------------------------------
// CWsfModel::SetActiveViewL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::SetActiveViewL( TInt aViewId )
    {
    TUid id;
    id.iUid = aViewId;
    iActiveView = id;
    }
        

// ----------------------------------------------------------------------------
// CWsfModel::ActiveViewL
// ----------------------------------------------------------------------------
//
EXPORT_C TUid CWsfModel::ActiveViewL()
    {
    return iActiveView;
    }

    
// ----------------------------------------------------------------------------
// CWsfModel::SetObservedWlanL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::SetObservedWlanL( const TDesC8& aSsid )
    {
    HBufC8* ssid = aSsid.AllocL();
    delete iObservedWlan;
    iObservedWlan = ssid;
    }
    

// ----------------------------------------------------------------------------
// CWsfModel::ObservedWlan
// ----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CWsfModel::ObservedWlan()
    {
    return *iObservedWlan;
    }


// ----------------------------------------------------------------------------
// CWsfModel::ConnectL
// ----------------------------------------------------------------------------
//
EXPORT_C int CWsfModel::ConnectL( TUint32 aIapId )
    {
    LOG_ENTERFN( "CWsfModel::ConnectL" );
    
    if ( iObserver )
        {
        iObserver->ConnectingL( aIapId );
        }
    
    TInt err = iSession.ConnectWlanBearerL( aIapId, EIapPersistent );
    if ( err == KErrNone )
        {
        iConnectedIapId = aIapId;
        }
    else
        {
        if ( iObserver )
            {
            iObserver->BrowserLaunchFailed( err );            
            }
        }

    if ( iObserver )
        {
        iObserver->ConnectingFinishedL( err );
        }

    TBool timerStarted( EFalse );
    timerStarted = iSession.ControlDisconnectTimerL( 
                                         EAdcStartTimer | EAdcTimerReset );
    
    if ( !timerStarted )
        {
        LOG_WRITE( "auto-disconnect timer couldn't be started!" );
        }
    
    
    iRefreshing = iSession.RequestScanL();   
    
    return err;
    }


// ----------------------------------------------------------------------------
// CWsfModel::ConnectWithoutConnWaiterL
// ----------------------------------------------------------------------------
//
EXPORT_C int CWsfModel::ConnectWithoutConnWaiterL( TUint32 aIapId, 
                                                   TBool aTestedAccessPoint )
    {
    LOG_ENTERFN( "CWsfModel::ConnectWithoutConnWaiterL" );
    
    if ( iObserver )
        {
        iObserver->ConnectingL( aIapId );
        }
    if ( aTestedAccessPoint )
        {
        return iSession.ConnectWlanBearerWithoutConnWaiterL( aIapId, 
                                                       EIapPersistent );
        }
    else
        {
        return iSession.ConnectWlanBearerWithoutConnWaiterL( aIapId, 
                                                       EIapExpireOnDisconnect );
        }
    }


// ----------------------------------------------------------------------------
// CWsfModel::FinalizeConnectL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::FinalizeConnectL()
    {
    LOG_ENTERFN( "CWsfModel::FinalizeConnectL" );
    
    TBool timerStarted( EFalse );
    timerStarted = iSession.ControlDisconnectTimerL( 
                                              EAdcStartTimer | EAdcTimerReset );
    
    if ( !timerStarted )
        {
        LOG_WRITE( "auto-disconnect timer couldn't be started!" );
        }
    }


// ----------------------------------------------------------------------------
// CWsfModel::DisconnectL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::DisconnectL()
    {
    LOG_ENTERFN( "CWsfModel::DisconnectL" );
    iSession.DisconnectWlanBearerL();
    iConnectedIapId = 0;
    iConnectedNetId = 0;
    iConnectOnly = EFalse;
    iRefreshing = iSession.RequestScanL();    
    }


// ----------------------------------------------------------------------------
// CWsfModel::Refreshing
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::Refreshing()
    {
    return iRefreshing;
    }


// ----------------------------------------------------------------------------
// CWsfModel::SetRefreshState
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::SetRefreshState( TBool aRefreshing )
    {
    iRefreshing = aRefreshing;
    }


// ----------------------------------------------------------------------------
// CWsfModel::EnableScanL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::EnableScanL()
    {
    LOG_ENTERFN( "CWsfModel::EnableScanL" );
    TBool enableScan( iSession.EnableScanL() );
    if ( iScreenSaverWatcher )
        {
        iScreenSaverWatcher->Cancel();
        iScreenSaverWatcher->StartStatusScanning();
        }
    return enableScan;
    }


// ----------------------------------------------------------------------------
// CWsfModel::DisableScanL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::DisableScanL()
    {
    LOG_ENTERFN( "CWsfModel::DisableScanL" );
    TBool disableScan( iSession.DisableScanL() );   
    if ( iScreenSaverWatcher )
        {
        iScreenSaverWatcher->Cancel();
        }
    return disableScan; 
    }


// ----------------------------------------------------------------------------
// CWsfModel::IsScanEnabledL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::IsScanEnabledL()
    {
    return iSession.IsScanEnabledL(); 
    }


// ----------------------------------------------------------------------------
// CWsfModel::LaunchBrowserL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::LaunchBrowserL( TUint32 aIapId )
    {
    LOG_ENTERFN( "CWsfModel::LaunchBrowserL" );
    
    if ( !iBrowserLauncher->Launching() ||
         iBrowserLauncher->BrowserIap() != aIapId ) 
        {
        LOG_WRITE( "launching browser..." );
        iConnectedIapId = aIapId;        
        iSession.MonitorAccessPointL( aIapId );
        iSession.ControlDisconnectTimerL( EAdcStopTimer );
        iBrowserLauncher->LaunchBrowserL( *this, aIapId );
        }
    else if ( iBrowserLauncher->BrowserIap() == aIapId ) 
        {
        LOG_WRITE( "bringing browser to foreground..." );
        ContinueBrowsingL();
        }
        
    }


// ----------------------------------------------------------------------------
// CWsfModel::CleanUpCancelledLaunchL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::CleanUpCancelledLaunchL()
    {
    LOG_ENTERFN( "CWsfModel::CleanUpCancelledLaunchL" );
    iSession.SetIapPersistenceL( EIapForcedExpiry );
    }
    

// ----------------------------------------------------------------------------
// CWsfModel::ContinueBrowsingL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::ContinueBrowsingL()
    {
    LOG_ENTERFN( "CWsfModel::ContinueBrowsingL(void)" );
    iBrowserLauncher->ContinueBrowsingL();
    }
    

// ----------------------------------------------------------------------------
// CWsfModel::ContinueBrowsingL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::ContinueBrowsingL( TUint32 aIapId )
    {
    LOG_ENTERFN( "CWsfModel::ContinueBrowsingL(TUint32)" );
    iBrowserLauncher->ContinueBrowsingL( *this, aIapId );
    }    


// ----------------------------------------------------------------------------
// CWsfModel::ConnectivityObserver
// ----------------------------------------------------------------------------
//
void CWsfModel::ConnectivityObserver( TIctsTestResult aResult, 
                                      const TDesC& aString )
    {
    LOG_ENTERFN( "CWsfModel::ConnectivityObserver" );
    LOG_WRITEF( "ICTS result: %d", aResult );
    
    TBool makePersistent( EFalse );
    // check the result
    switch ( aResult )
        {
        case EConnectionOk:
            {
            // test succeeded
            TRAP_IGNORE( MoveToInternetSnapL( iConnectedIapId ) );
            makePersistent = ETrue;
            LOG_WRITE( "ICT: EConnectionOk" );
            break;            
            }
            
        case EConnectionNotOk:
            {
            // test was run but it failed
            LOG_WRITE( "ICT: EConnectionNotOk" );
            break;
            }
        case EHttpAuthenticationNeeded:
            {
            // test was run but HTTP authentication is required
            LOG_WRITE( "ICT: EHttpAuthenticationNeeded" );
            if ( iConnectOnly )
                {
                // Connect selected. WLAN Login needed.
                TRAP_IGNORE( LaunchWlanLoginL(aString) );
                }    
            break;
            }    
        case ETimeout:
            {
            LOG_WRITE( "ICT: ETimeout" );
            break;
            }
            
        default:
            {
            _LIT( KIctPanic, "ICT result" );
            User::Panic( KIctPanic, aResult );
            }
        }

    if ( makePersistent )
        {
        TWsfIapPersistence pt = ( iConnectedIapId )? 
                                     EIapPersistent: 
                                     EIapExpireOnShutdown;
                                     
        TRAPD( err, MakeIctIapPersistentL( pt ) );
        if ( err )
            {
            LOG_WRITEF( "MakeIctIapPersistentL leaved with error = %d", err );
            }
        }

    if ( iKeepConnection )
        {
        // trigger the auto-disconnect timer as well
        TBool timerStarted( EFalse );
        TRAP_IGNORE( timerStarted = iSession.ControlDisconnectTimerL( 
                                          EAdcStartTimer | EAdcTimerReset ) );
        
        if ( !timerStarted )
            {
            LOG_WRITE( "auto-disconnect timer couldn't be started!" );
            }
        }
        
    LOG_WRITE( "before AsyncStop" );
    // finally stop blocking the caller
    iIctEnded = ETrue; 
    if ( iIctWait.IsStarted() )
        {
        LOG_WRITE( "ICT: AsyncStop" );
        iIctWait.AsyncStop();
        } 
     

    }

// -----------------------------------------------------------------------------
// CWsfModel::LaunchWlanLoginL()
// -----------------------------------------------------------------------------
//    
void CWsfModel::LaunchWlanLoginL( const TDesC& aString )
    {   
    LOG_ENTERFN( "WsfModel::LaunchWlanLoginL" );
    HBufC* param = HBufC::NewLC( KMaxFileName );
    _LIT(tmpString, "%d, %d, %S");
    param->Des().Format( tmpString, 
                         iConnectedIapId, 
                         iConnectedNetId, 
                         &aString );
    TUid uid( TUid::Uid( KBrowserUid ) );
    TThreadId id;
    
    RApaLsSession appArcSession;
    User::LeaveIfError( appArcSession.Connect() ); 
    CleanupClosePushL( appArcSession );
        
    TInt err = appArcSession.StartDocument( *param, TUid::Uid( KBrowserUid ), id );
    if ( err != KErrNone )
        {
        LOG_ENTERFN( "WsfModel::LaunchWlanLoginL failed" );
        }
    CleanupStack::PopAndDestroy( &appArcSession );
    CleanupStack::PopAndDestroy( param );
    }

// ----------------------------------------------------------------------------
// CWsfModel::MakeIctIapPersistentL
// ----------------------------------------------------------------------------
//
void CWsfModel::MakeIctIapPersistentL( TWsfIapPersistence aPersistence )
    {
    LOG_ENTERFN( "CWsfModel::MakeIctIapPersistentL" );
    LOG_WRITEF( "temp ICT IAP id = %d", iIctWlanInfo.iIapId );
    
    if ( !iSession.SetIapPersistenceL( aPersistence ) )
        {
        LOG_WRITE( "setting temporary flag FAILED" );
        }
    
    }

// ----------------------------------------------------------------------------
// CWsfModel::MoveToInternetSnapL
// ----------------------------------------------------------------------------
//
void CWsfModel::MoveToInternetSnapL( const TUint32 aIapId )
    {
	LOG_ENTERFN( "CWsfModel::MoveToInternetSnapL" );
    // Read all destination(SNAP) settings into an array
    RArray<TUint32> destinations;
    CleanupClosePushL(destinations);
    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );      
    cmManager.AllDestinationsL(destinations);
    RCmDestinationExt destination;
    // Loop through each destination
    for(TInt i = 0; i < destinations.Count(); i++)
        {
        destination = cmManager.DestinationL(destinations[i]);
        CleanupClosePushL(destination); 
        // Internet destination will always exist in the system.
        // Internet destination will have ESnapPurposeInternet set in its metadata.
        if (destination.MetadataL(CMManager::ESnapMetadataPurpose) == CMManager::ESnapPurposeInternet)
            {
            RCmConnectionMethodExt iap = cmManager.ConnectionMethodL( aIapId );
            CleanupClosePushL( iap );     
            LOG_WRITE( "Move Iap to internet destination" );
            destination.AddConnectionMethodL( iap );
            destination.UpdateL();
            CleanupStack::PopAndDestroy( &iap ); 
            }
        CleanupStack::PopAndDestroy( &destination ); 
        }
    CleanupStack::PopAndDestroy( &cmManager ); 
    CleanupStack::PopAndDestroy( &destinations ); 
    }

// ----------------------------------------------------------------------------
// CWsfModel::CreateAccessPointL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::CreateAccessPointL( TWsfWlanInfo& aWlan, 
                                              TBool aExplicitDefine )
    {
    LOG_ENTERFN( "CWsfModel::CreateAccessPointL" );
    if ( aExplicitDefine )
        {
        LOG_WRITE( "called from 'Define access point'" );
        }
    
    CheckSpaceBelowCriticalLevelL();
    CheckUnknownWapiL( aWlan );

#pragma message("TODO: oursource UI to client interfaces!")        
    CWsfWlanIapWizard* iapWizard = CWsfWlanIapWizard::NewLC();
    
    // the wlaninfo must be persistent to avoid nullpointer crashes due to
    // background refreshing 
    TBool ret( ETrue );
    
    // query necessary data
    if ( !iapWizard->LaunchWizardL( aWlan, aExplicitDefine ) )
        {
        LOG_WRITE( "iapWizard.LaunchWizardL failed" );
        ret = EFalse;
        }

    // then create accesspoint
    if ( ret )
        {
        if ( iapWizard->CreateAccessPointL() )
            {
            // copy back the IAP id
            LOG_WRITEF( "IAP id = %d", aWlan.iIapId );
            }
        else
            {
            LOG_WRITE( "iapWizard.CreateAccessPointL failed" );
            ret = EFalse;
            }
        }

    CleanupStack::PopAndDestroy( iapWizard );
    
    return ret;
    }


// ----------------------------------------------------------------------------
// CWsfModel::IctsTestPermission
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CWsfModel::IctsTestPermission()
    {
    LOG_ENTERFN( "CWsfModel::IctsTestPermission" );
    TInt ictTestPermission( 0 );
    CRepository* repository( NULL );
    
    TRAPD( err, repository = CRepository::NewL( 
                                        KCRUidInternetConnectivitySettings ) );
    if ( err == KErrNone )
        {
        repository->Get( KIctsTestPermission, ictTestPermission );
        delete repository;
        LOG_WRITEF( "ICT is set to %d", ictTestPermission );
        }
    return ictTestPermission;
    }


// ----------------------------------------------------------------------------
// CWsfModel::TestAccessPointL
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CWsfModel::TestAccessPointL( TWsfWlanInfo& aWlan,
                                           TBool aKeepConnection, 
                                           TBool aConnectOnly )
    {
    LOG_ENTERFN( "CWsfModel::TestAccessPointL" );    
    TInt err( KErrNone );
    iKeepConnection = aKeepConnection;
    iConnectOnly = aConnectOnly;
    if ( !aWlan.iIapId )
        {
        // the wlaninfo must already contain a valid IAP id
        LOG_WRITE( "invalid IAP id" );
        return KErrCorrupt;
        }
    
    // the wlaninfo must be persistent to avoid nullpointer crashes due to
    // background refreshing 
    iIctWlanInfo = aWlan;

    // create connection and test connectivity if needed

    // check ICT settings
    TInt ictTestPermission( IctsTestPermission() );
    
    
    if ( aKeepConnection || ictTestPermission != EIctsNeverRun )
        {
        // make connection if Connect was selected or if ICT needs it
        LOG_WRITE( "creating connection..." );
        if ( iObserver )
            {
            iObserver->ConnectingL( iIctWlanInfo.iIapId );
            }

        // create the connection with temporary IAP by default
        err = iSession.ConnectWlanBearerL( iIctWlanInfo.iIapId, 
                                           EIapExpireOnDisconnect );
        
        if ( err == KErrNone )
            {
            LOG_WRITE( "connection OK." )
            }
        else
            {
            LOG_WRITEF( "connection creation failed with error = %d", err );
            // either the connection creation failed or was aborted, 
            // the server already cleaned up the mess, so nothing to do
            }
            
        if ( iObserver )
            {
            iObserver->ConnectingFinishedL( err );
            }
            
        }

    if ( err == KErrNone && ictTestPermission != EIctsNeverRun )
        {
        // do the connectivity test
        iConnectedIapId = iIctWlanInfo.iIapId;
        
        RCmManagerExt cmManager;
        cmManager.OpenL();
        CleanupClosePushL( cmManager );        

        RCmConnectionMethodExt cm = cmManager.ConnectionMethodL( 
                                                            iConnectedIapId );
        CleanupClosePushL( cm );
        
        iConnectedNetId = cm.GetIntAttributeL( CMManager::ECmNetworkId ); 

        CleanupStack::PopAndDestroy( &cm );
        CleanupStack::PopAndDestroy( &cmManager );

        LOG_WRITE( "starting ICT test..." );
        CIctsClientInterface* ict = CIctsClientInterface::NewL( 
                                                    iConnectedIapId, 
                                                    iConnectedNetId,
                                                    *this );
        LOG_WRITE( "ICT created" );
        CleanupStack::PushL( ict );
        ict->StartL();
        LOG_WRITE( "ICT: started" );
        
        // enter a waitloop since ICT is a kind of asynchronous service
        if ( !iIctEnded )
            {
            LOG_WRITE( "ICT: iIctWait started" );
            iIctWait.Start();
            }
            
        iIctEnded = EFalse;
        CleanupStack::PopAndDestroy( ict );
        LOG_WRITE( "ICT test done." );
        }
    
        
    if ( ( err == KErrNone && !aKeepConnection && 
                                     ictTestPermission != EIctsNeverRun ) ||
         ( err != KErrNone && err != KErrCancel ) )
        {
        // drop the connection in case of Start web browsing, and if an error
        // different from KErrCancel occured (on cancel the connection is
        // closed automatically)
        LOG_WRITE( "disconnecting..." );
        iSession.DisconnectWlanBearerL();
        LOG_WRITE( "Disconnected." );
        }
        
    
        
    if ( err == KErrNone && ictTestPermission == EIctsNeverRun )
        {
        LOG_WRITE( "ICT is set to never run, IAP remains temporary" );
        
        if ( !iKeepConnection )
            {
            //get the engine monitor the IAP
            iSession.MonitorAccessPointL( iIctWlanInfo.iIapId );
            iSession.SetIapPersistenceL( EIapExpireOnShutdown );
            iSession.MonitorAccessPointL( iIctWlanInfo.iIapId );
            }

        ConnectivityObserver( EConnectionNotOk, KNullDesC );
        }

    return err;
    }


// ----------------------------------------------------------------------------
// CWsfModel::TestConnectedAccessPointL
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CWsfModel::TestConnectedAccessPointL( TWsfWlanInfo& aWlan,
                                                    TBool aConnectOnly )
    {
    LOG_ENTERFN( "CWsfModel::TestConnectedAccessPointL" );    
    TInt err( KErrNone );
    iConnectOnly = aConnectOnly;
    if ( !aWlan.iIapId )
        {
        // the wlaninfo must already contain a valid IAP id
        LOG_WRITE( "invalid IAP id" );
        return KErrCorrupt;
        }
    
    // the wlaninfo must be persistent to avoid nullpointer crashes due to
    // background refreshing 
    iIctWlanInfo = aWlan;

    // check ICT settings
    TInt ictTestPermission( IctsTestPermission() );

    if ( ictTestPermission != EIctsNeverRun )
        {
        // do the connectivity test
        iConnectedIapId = iIctWlanInfo.iIapId;
        
        RCmManagerExt cmManager;
        cmManager.OpenL();
        CleanupClosePushL( cmManager );        

        RCmConnectionMethodExt cm = cmManager.ConnectionMethodL( 
                                                            iConnectedIapId );
        CleanupClosePushL( cm );
        
        iConnectedNetId = cm.GetIntAttributeL( CMManager::ECmNetworkId ); 

        CleanupStack::PopAndDestroy( &cm );
        CleanupStack::PopAndDestroy( &cmManager );

        LOG_WRITE( "starting ICT test..." );
        CIctsClientInterface* ict = CIctsClientInterface::NewL( 
                                                    iConnectedIapId, 
                                                    iConnectedNetId,
                                                    *this );
        LOG_WRITE( "ICT created" );
        CleanupStack::PushL( ict );
        ict->StartL();
        LOG_WRITE( "ICT: started" );
        
        // enter a waitloop since ICT is a kind of asynchronous service
        if ( !iIctEnded )
            {
            LOG_WRITE( "ICT: iIctWait started" );
            iIctWait.Start();
            }
            
        iIctEnded = EFalse;
        CleanupStack::PopAndDestroy( ict );
        LOG_WRITE( "ICT test done." );
        }

    if ( ictTestPermission == EIctsNeverRun )
        {
        LOG_WRITE( "ICT is set to never run, IAP remains temporary" );
        ConnectivityObserver( EConnectionNotOk, KNullDesC );
        }

    return err;
    }


// ----------------------------------------------------------------------------
// CWsfModel::RefreshScanL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::RefreshScanL()
    {
    LOG_ENTERFN( "CWsfModel::RefreshScanL" );
    iRefreshing = iSession.RequestScanL();
    return iRefreshing;
    }
    

// ----------------------------------------------------------------------------
// CWsfModel::LaunchHelperApplicationL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::LaunchHelperApplicationL( TWsfWlanInfo &aWlanInfo,
                                                   TBool aConnecting, 
                                                   TBool aConnectOnly )
    {
    LOG_ENTERFN( "CWsfModel::LaunchHelperApplicationL" );
    TPckgC<TWsfWlanInfo> param( aWlanInfo );
    TPckgC<TBool> param2( aConnecting );
    TPckgC<TBool> param3( aConnectOnly );
    
    TBuf8<sizeof( TWsfWlanInfo ) + sizeof( TBool ) + sizeof( TBool )> temp;
    temp.Copy( param );
    temp.Append( param2 );
    temp.Append( param3 );
    
    TFileName fileName;
    fileName.Copy( temp );
    
    RApaLsSession appArcSession;
    
    User::LeaveIfError( appArcSession.Connect() ); // connect to AppArc server
    CleanupClosePushL( appArcSession );

    // check if the app is already running ... and kill it.
    TUid id( TUid::Uid( KHelperApUid.iUid ) );
    TApaTaskList taskList( CEikonEnv::Static()->WsSession() );
    TApaTask task = taskList.FindApp( id );

    if ( task.Exists() )
        {
        task.EndTask();
        }    

    TThreadId threadId;
    User::LeaveIfError( appArcSession.StartDocument( fileName, TUid::Uid( 
                                             KHelperApUid.iUid ), threadId ) );

    CleanupStack::PopAndDestroy( &appArcSession );
    }


// ----------------------------------------------------------------------------
// CWsfModel::CancelNotifyEvents
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::CancelNotifyEvents()
    {
    LOG_ENTERFN( "CWsfModel::CancelNotifyEvents" );
    iSession.CancelNotifyEvent();
    }


// ----------------------------------------------------------------------------
// CWsfModel::RequestNotifyEventsL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::RequestNotifyEventsL( 
                                           MWsfStateChangeObserver& aObserver )
    {
    LOG_ENTERFN( "CWsfModel::RequestNotifyEventsL" );
    iSession.NotifyEventL( aObserver );
    }


// ------------------------- From MWsfBrowserLaunchObserver -------------------
    
// ----------------------------------------------------------------------------
// CWsfModel::BrowserLaunchFailed
// ----------------------------------------------------------------------------
//
void CWsfModel::BrowserLaunchFailed( TInt aError )
    {
    LOG_ENTERFN( "CWsfModel::BrowserLaunchFailed" );
    LOG_WRITEF( "error = %d", aError );
    
    // do the cleanup if necessary
    TRAP_IGNORE( 
        iSession.SetIapPersistenceL( EIapForcedExpiry );
        iSession.ControlDisconnectTimerL( EAdcStartTimer | EAdcTimerReset );
    );
    
    if ( iObserver )
        {
        iObserver->BrowserLaunchFailed( aError );    
        }
    }


// ----------------------------------------------------------------------------
// CWsfModel::BrowserLaunchCompleteL
// ----------------------------------------------------------------------------
//
void CWsfModel::BrowserLaunchCompleteL()
    {
    LOG_ENTERFN( "CWsfModel::BrowserLaunchCompleteL" );

    if ( iObserver )
        {
        iObserver->BrowserLaunchCompleteL();    
        }
    }


// ----------------------------------------------------------------------------
// CWsfModel::BrowserExitL
// ----------------------------------------------------------------------------
//
void CWsfModel::BrowserExitL()
    {
    LOG_ENTERFN( "CWsfModel::BrowserExitL" );
    
    // browser has been terminated, do the cleanup if necessary
    iSession.SetIapPersistenceL( EIapForcedExpiry );
    iSession.ControlDisconnectTimerL( EAdcStartTimer | EAdcTimerReset );
    
    if ( iObserver )
        {
        iObserver->BrowserExitL();    
        }
    }
    
// ------------------------- From MWsfScreenSaverStateObserver ----------------
 
// ----------------------------------------------------------------------------
// CWsfModel::ScreenSaverStatusChangedL
// ----------------------------------------------------------------------------
//
void CWsfModel::ScreenSaverStatusChangedL( const TBool aScreenSaverActive )
    {
    LOG_ENTERFN( "CWsfModel::ScreenSaverStatusChangedL" );
    LOG_WRITEF( "status = %d", aScreenSaverActive );
    if ( aScreenSaverActive )
        {
        iSession.DisableScanL();
        }
    else 
        {
        iSession.EnableScanL();
        }
    }
    

// ----------------------------------------------------------------------------
// CWsfModel::IsConnectedL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::IsConnectedL()
    {
    return iSession.IsConnectedL();
    }
    

// ----------------------------------------------------------------------------
// CWsfModel::GetConnectedWlanDetailsL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::GetConnectedWlanDetailsL( TWsfWlanInfo& aWlanInfo )
    {
    return iSession.GetConnectedWlanDetailsL( aWlanInfo );
    }


// ----------------------------------------------------------------------------
// CWsfModel::AbortConnectingL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::AbortConnectingL()
    {
    LOG_ENTERFN( "CWsfModel::AbortConnectingL" );
    if ( iConnecting )
        {
        iSession.AbortConnectingL();
        }
    }


// ----------------------------------------------------------------------------
// CWsfModel::AbortScanningL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::AbortScanningL()
    {
    LOG_ENTERFN( "CWsfModel::AbortScanningL" );
    if ( iRefreshing )
        {
        iSession.AbortScanningL();
        }
    }


// ----------------------------------------------------------------------------
// CWsfModel::CheckSpaceBelowCriticalLevelL
// ----------------------------------------------------------------------------
//
void CWsfModel::CheckSpaceBelowCriticalLevelL() const
    {
    // OOD handling. If disk space is low user is notified.
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL<RFs>( fs );
    
    // Checks the FFS space "after" addition
    TBool belowCL = SysUtil::FFSSpaceBelowCriticalLevelL
            ( &fs, KEstimatedOverhead );
    
    CleanupStack::PopAndDestroy(); // fs
    
    if( belowCL )
        {
        User::Leave( KErrDiskFull );
        }
    }

// ----------------------------------------------------------------------------
// CWsfModel::CheckUnknownWapiL
// ----------------------------------------------------------------------------
//
void CWsfModel::CheckUnknownWapiL( TWsfWlanInfo& aWlan ) const
    {    
    if( !aWlan.Known() && aWlan.SecurityMode() == EWlanSecModeWAPI )
        {
        User::Leave( KErrWlanProtectedSetupSetupLocked );
        }
    }
    
// End of file
