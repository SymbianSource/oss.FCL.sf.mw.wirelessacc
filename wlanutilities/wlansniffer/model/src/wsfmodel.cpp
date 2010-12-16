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
    LOG_ENTERFN( "CWsfModel::~CWsfModel" );
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
    iRefreshing( EFalse )
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
    LOG_ENTERFN( "CWsfModel::SetEngineObserver" );
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
// CWsfModel::GetWlanListSize
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::GetWlanListSize( TPckgBuf<TUint>& aPckg, 
                                           TRequestStatus& aStatus )
    {
    LOG_ENTERFN( "CWsfModel::GetWlanListSize" );
    iSession.GetWlanListSize( aPckg, aStatus );
    }


// ----------------------------------------------------------------------------
// CWsfModel::GetWlanList
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::GetWlanList( TPckgBuf<TUint>& aPckg, TPtr8& aPtr, 
                                       TRequestStatus& aStatus )
    {
    LOG_ENTERFN( "CWsfModel::GetWlanList" );
    iSession.GetWlanList( aPckg, aPtr, aStatus );
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
EXPORT_C void CWsfModel::ConnectL( TPckgBuf<TInt>& aPckg, TUint32 aIapId,
                                   TBool aConnectOnly,
                                   TWsfIapPersistence aPersistence,
                                   TRequestStatus& aStatus )
    {
    LOG_ENTERFN( "CWsfModel::ConnectL" );
    
    if ( iObserver )
        {
        iObserver->ConnectingL( aIapId );
        }
    
    iSession.ConnectWlanBearer( aPckg, aIapId, aConnectOnly, aPersistence, aStatus );
    }


// ----------------------------------------------------------------------------
// CWsfModel::SetConnectResultL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::SetConnectResultL( TInt aResult, TUint /*aIapId*/ )
    {
    LOG_ENTERFN( "CWsfModel::SetConnectResultL" );
    
    iSession.SetConnectWlanBearerResult( aResult );
    
    if ( iObserver && aResult != KErrNone )
        {
        iObserver->ConnectingFinishedL( aResult );
        }
    }


// ----------------------------------------------------------------------------
// CWsfModel::Disconnect
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::Disconnect( TPckgBuf<TBool>& aPckg, TRequestStatus& aStatus )
    {
    LOG_ENTERFN( "CWsfModel::Disconnect" );
    iSession.DisconnectWlanBearer( aPckg, aStatus );
    iConnectedIapId = 0;
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
        iBrowserLauncher->LaunchBrowserL( *this, aIapId );
        }
    else if ( iBrowserLauncher->BrowserIap() == aIapId ) 
        {
        LOG_WRITE( "bringing browser to foreground..." );
        ContinueBrowsingL();
        }
        
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
// CWsfModel::RefreshScanL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::RefreshScanL()
    {
    LOG_ENTERFN( "CWsfModel::RefreshScanL" );
    iRefreshing = iSession.RequestScanL();
    LOG_WRITEF( "iRefreshing = %d", iRefreshing );
    return iRefreshing;
    }


// ----------------------------------------------------------------------------
// CWsfModel::RefreshScan
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::RefreshScan( TPckgBuf<TBool>& aPckg, 
                                       TRequestStatus& aStatus )
    {
    LOG_ENTERFN( "CWsfModel::RefreshScan" );
    iSession.RequestScan( aPckg, aStatus );
    }


// ----------------------------------------------------------------------------
// CWsfModel::CloseHelperApplication
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfModel::CloseHelperApplication()
    {
    LOG_ENTERFN( "CWsfModel::CloseHelperApplication" );
    TBool endTaskCalled = EFalse;
    // check if the app is already running ... and kill it.
    TUid id( TUid::Uid( KHelperApUid.iUid ) );
    TApaTaskList taskList( iEikEnv->WsSession() );
    TApaTask task = taskList.FindApp( id );

    if ( task.Exists() )
        {
        LOG_WRITE( "ending task" );
        task.EndTask();
        endTaskCalled = ETrue;
        }
    return endTaskCalled;
    }

    

// ----------------------------------------------------------------------------
// CWsfModel::LaunchHelperApplicationL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::LaunchHelperApplicationL( TWsfWlanInfo &aWlanInfo )
    {
    LOG_ENTERFN( "CWsfModel::LaunchHelperApplicationL" );
    TPckgC<TWsfWlanInfo> param( aWlanInfo );
    
    TBuf8<sizeof( TWsfWlanInfo )> temp;
    temp.Copy( param );
    
    TFileName fileName;
    fileName.Copy( temp );
    
    RApaLsSession appArcSession;
    
    User::LeaveIfError( appArcSession.Connect() ); // connect to AppArc server
    CleanupClosePushL( appArcSession );

    // check if the app is already running ... and kill it.
    TUid id( TUid::Uid( KHelperApUid.iUid ) );
    TApaTaskList taskList( iEikEnv->WsSession() );
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

    iSession.MonitorAccessPointL( iConnectedIapId  );
    
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
// CWsfModel::GetConnectedWlanDetails
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::GetConnectedWlanDetails( TPckgBuf<TBool>& aPckg,
                                                   TWsfWlanInfo& aWlanInfo,
                                                   TRequestStatus& aStatus )
    {
    iSession.GetConnectedWlanDetails( aPckg, aWlanInfo, aStatus );
    }


// ----------------------------------------------------------------------------
// CWsfModel::AbortConnectingL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::AbortConnectingL()
    {
    LOG_ENTERFN( "CWsfModel::AbortConnectingL" );
    iSession.AbortConnectingL();
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

// ----------------------------------------------------------------------------
// CWsfModel::CheckIsIapIdValidL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfModel::CheckIsIapIdValidL( TUint aIapId ) const
    {    
    LOG_ENTERFN( "CWsfModel::CheckIsIapIdValidL" );
    LOG_WRITEF( "Checking iapId= %d", aIapId );
    if( aIapId )
        {
        RCmManagerExt cmManager;
        cmManager.OpenL();
        CleanupClosePushL( cmManager );        

        RCmConnectionMethodExt cm = cmManager.ConnectionMethodL( aIapId );
        cm.Close();
        CleanupStack::PopAndDestroy( &cmManager );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }
    
// End of file
