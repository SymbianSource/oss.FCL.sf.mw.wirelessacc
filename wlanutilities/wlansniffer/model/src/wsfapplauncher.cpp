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
* Description:  Implementation of CWsfAppLauncher
*
*/




//  EXTERNAL INCLUDES
#include <apgtask.h>
#include <apgcli.h>
#include <centralrepository.h>
#include <browseruisdkcrkeys.h>
#include <cmmanager.h>
#include <cmconnectionmethod.h>
 
//  CLASS HEADER
#include "wsfapplauncher.h"

//  INTERNAL INCLUDES
#include "wsfbrowserlaunchobserver.h"
#include "wsflogger.h"


//  LOCAL DEFINITIONS
/**
* Delay that we wait for the browser to start or terminate
*/
static const TInt KTimerTickInterval = 2 * 1000 * 1000;    

/**
* Repository key ID for the browser's homepage URL
*/
static const TUint32 KBrowserNGHomepageURL = 0x00000030;


#ifdef _DEBUG
    _LIT( KBrowserLauncherPanic, "CWsfAppLauncher" );
    #define _ASSERTD( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KBrowserLauncherPanic, __LINE__) )
#else
    #define _ASSERTD( cond ) {}
#endif //_DEBUG


// ---------------------------------------------------------------------------
// CWsfAppLauncher::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CWsfAppLauncher* CWsfAppLauncher::NewL()
    {
    CWsfAppLauncher* self = CWsfAppLauncher::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CWsfAppLauncher* CWsfAppLauncher::NewLC()
    {
    CWsfAppLauncher* self = new( ELeave ) CWsfAppLauncher();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::~CWsfAppLauncher
// ---------------------------------------------------------------------------
//
CWsfAppLauncher::~CWsfAppLauncher()
    {
    Cancel();
    delete iURL;
    delete iRepository;
    iTimer.Close();
    iWsSession.Close();     
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::CWsfAppLauncher
// ---------------------------------------------------------------------------
//
CWsfAppLauncher::CWsfAppLauncher(): 
    CActive( CActive::EPriorityStandard ),
    iIapId( 0 ), 
    iLaunchState( EIdle ),
    iLaunchBookMarks( EFalse )
    {
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfAppLauncher::ConstructL()
    {
    LOG_ENTERFN( "CWsfAppLauncher::ConstructL" );
    iBrowserUid = KCRUidBrowser;
    iHomepageKey = KBrowserNGHomepageURL; 
    iBrowserStartingPageKey = KBrowserNGHomepageType;

    // Common settings for both browsers
    iRepository = CRepository::NewL( KCRUidBrowser );
    
    User::LeaveIfError( iWsSession.Connect() );    
    User::LeaveIfError( iTimer.CreateLocal() );

    iURL = KNullDesC().AllocL();    
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::LaunchBrowserL
// ---------------------------------------------------------------------------
//
void CWsfAppLauncher::LaunchBrowserL( MWsfBrowserLaunchObserver& aObserver, 
                                      TUint aIapId,
                                      const TDesC& aURL )
    {
    LOG_ENTERFN( "CWsfAppLauncher::LaunchBrowserL_3" );
    Cancel();
    LOG_WRITE( "CWsfAppLauncher::LaunchBrowserL_3 -->> afer cancel" );
    iObserver = &aObserver;  
    iIapId = aIapId;
    HBufC* url = aURL.AllocL();
    delete iURL;
    iURL = url;

    iLaunchState = EIdle;    
    LOG_WRITE( "CWsfAppLauncher::LaunchBrowserL_3 -->> before set active" );
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::LaunchBrowserL
// ---------------------------------------------------------------------------
//
void CWsfAppLauncher::LaunchBrowserL( MWsfBrowserLaunchObserver& aObserver, 
                                      TUint aIapId )
    {
    LOG_ENTERFN( "CWsfAppLauncher::LaunchBrowserL_2" );
    LOG_WRITEF( "CWsfAppLauncher::LaunchBrowserL_2 -->> iIapid %d", aIapId );
    HBufC* url = HBufC::NewLC( 
                        NCentralRepositoryConstants::KMaxUnicodeStringLength );
    TPtr urlPtr( url->Des() );
    
    iLaunchBookMarks = EFalse;
    
    RCmManager cmManager;
    
    cmManager.OpenL();
    LOG_WRITE( "CWsfAppLauncher::LaunchBrowserL_2 -->> cmmanager opened" );
    CleanupClosePushL( cmManager );

    RCmConnectionMethod plugin;
    CleanupClosePushL( plugin );
    LOG_WRITE( "CWsfAppLauncher::LaunchBrowserL_2 -->> cm before open" );
    LOG_WRITEF( "CWsfAppLauncher::LaunchBrowserL_2 -->> iIapid %d", aIapId );
    plugin = cmManager.ConnectionMethodL( aIapId );
    LOG_WRITE( "CWsfAppLauncher::LaunchBrowserL_2 -->> cm after open" );
    
    // do we have start page for Access Point?
    HBufC* apHomepage = NULL;
    apHomepage = plugin.GetStringAttributeL( CMManager::ECmStartPage );
    LOG_WRITE( "CWsfAppLauncher::LaunchBrowserL_2 -->> String Attrib got" );
    CleanupStack::PushL( apHomepage );
 
    if( apHomepage->Length() == 0 )
        {
        LOG_WRITE( "CWsfAppLauncher::LaunchBrowserL_2 -->> hplength = 0" );
        // if we can't have Access Point URL then
        // we try to get browser homepage URL
        TInt err = BrowserHomepage( urlPtr );   
        
        // if browser homepage is not defined either,
        // launch bookmarks view
        if ( err || url->Length() == 0 )
            {
            LOG_WRITE( 
                   "CWsfAppLauncher::LaunchBrowserL_2 -->> err in url length" );
            iLaunchBookMarks = ETrue;
            }
        }
    else
        {
        LOG_WRITE( "CWsfAppLauncher::LaunchBrowserL_2 -->> hplength>0" );
        url->Des().Copy( *apHomepage );
        }
    
    LaunchBrowserL( aObserver, aIapId, *url );
    
    LOG_WRITE( "CWsfAppLauncher::LaunchBrowserL_2 -->> after launch _3" );
    CleanupStack::PopAndDestroy( apHomepage );
    CleanupStack::PopAndDestroy( &plugin );
    CleanupStack::PopAndDestroy( &cmManager );
    CleanupStack::PopAndDestroy( url );
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::DoLaunchBrowserL
// ---------------------------------------------------------------------------
//
void CWsfAppLauncher::DoLaunchBrowserL()
    {
    LOG_ENTERFN( "CWsfAppLauncher::DoLaunchBrowserL" );
    _LIT( KFormatCommand, "%d %S" );
    const TInt KBrowserFirstParamUrlFollows = 4;
    
    iLaunchState = EStartingUp;    
    HBufC* param = NULL; 
    if ( iLaunchBookMarks )
        {
        param = KNullDesC().AllocLC();
        }
    else
        {
        param = HBufC::NewLC( KFormatCommand().Length() + iURL->Length() );
        param->Des().Format( KFormatCommand, 
                             KBrowserFirstParamUrlFollows, iURL );
        }
        
    RApaLsSession appArcSession;
    User::LeaveIfError( appArcSession.Connect() ); // connect to AppArc server
    CleanupClosePushL( appArcSession );
                     
    User::LeaveIfError( appArcSession.StartDocument( *param, iBrowserUid, 
                                                                  iThreadId ) );

    CleanupStack::PopAndDestroy( &appArcSession );
    CleanupStack::PopAndDestroy( param );


    iTimer.Cancel();
    iTimer.After( iStatus, KTimerTickInterval );
    SetActive();
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::BrowserExists
// ---------------------------------------------------------------------------
//
TBool CWsfAppLauncher::BrowserExists()
    {
    LOG_ENTERFN( "CWsfAppLauncher::BrowserExists" );
    TApaTaskList taskList( iWsSession );            
    TApaTask startedtask = taskList.FindApp( iBrowserUid );
    return startedtask.Exists();   
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::BrowserHomepage
// ---------------------------------------------------------------------------
//
TInt CWsfAppLauncher::BrowserHomepage( TDes& aHomePageURL )
    {
    LOG_ENTERFN( "CWsfAppLauncher::BrowserHomepage" );
    CRepository* repository( iRepository );

    // get the default starting page setting
    TInt startingPageMode( 0 );
    TInt err = repository->Get( iBrowserStartingPageKey, startingPageMode );
    if ( err == KErrNone )
        {
        switch ( startingPageMode )
            {
            case 1:
            case 2:
                {
                // user defined or current page
                 err = repository->Get( iHomepageKey, aHomePageURL );
                break;
                }
            default:
                {
                aHomePageURL.Zero();                     
                }
            }
        }

    return err;    
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::KillBrowserIfAlreadyExists
// ---------------------------------------------------------------------------
//
TBool CWsfAppLauncher::KillBrowserIfAlreadyExists()
    {
    LOG_ENTERFN( "CWsfAppLauncher::KillBrowserIfAlreadyExists" );
    _ASSERTD( iLaunchState == EIdle);    
    _ASSERTD( !IsActive() );       
         
    TBool killing = EFalse;
    
    TApaTaskList taskList( iWsSession );
    TApaTask task = taskList.FindApp( iBrowserUid );

    if ( task.Exists() )
        {    
        // kill the browser...
        task.EndTask();
        killing = ETrue;

        iTimer.Cancel();
        iTimer.After( iStatus, KTimerTickInterval );
        }    
    else
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );  
        }
        
    SetActive();
    iLaunchState = EShuttingDown;
    return killing;
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::ContinueBrowsingL
// ---------------------------------------------------------------------------
//
void CWsfAppLauncher::ContinueBrowsingL()
    {
    LOG_ENTERFN( "CWsfAppLauncher::ContinueBrowsingL_0" );
    //Check if the application is already running
    TBool exists = BrowserExists();
    if ( exists )
        {
        TApaTaskList taskList( iWsSession );
        TApaTask task = taskList.FindApp( iBrowserUid );
        task.BringToForeground();
        }
    else
        {
        User::Leave( KErrNotFound );    
        }       
    }

    
// ---------------------------------------------------------------------------
// CWsfAppLauncher::ContinueBrowsingL
// ---------------------------------------------------------------------------
//
void CWsfAppLauncher::ContinueBrowsingL( MWsfBrowserLaunchObserver& aObserver, 
                                         TUint aIapId )
    {
    LOG_ENTERFN( "CWsfAppLauncher::ContinueBrowsingL_2" );
    //Check if the application is already running
    TBool exists = BrowserExists();
    if ( exists )
        {
        TApaTaskList taskList( iWsSession );
        TApaTask task = taskList.FindApp( iBrowserUid );
        task.BringToForeground();
        }
    else    // browser not running - launch browser
        {
        LaunchBrowserL( aObserver, aIapId );
        }       
    }    


// ---------------------------------------------------------------------------
// CWsfAppLauncher::Launching
// ---------------------------------------------------------------------------
//
TWsfLaunchState CWsfAppLauncher::Launching()
    {
    LOG_ENTERFN( "CWsfAppLauncher::Launching" );
    return iLaunchState;    
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::BrowserIap
// ---------------------------------------------------------------------------
//
TUint32 CWsfAppLauncher::BrowserIap() const
    {
    return iIapId;
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::DoCancel
// ---------------------------------------------------------------------------
//
void CWsfAppLauncher::DoCancel()
    {
    LOG_ENTERFN( "CWsfAppLauncher::DoCancel" ); 
    iThread.LogonCancel( iStatus );
    
    iThread.Close();
    iTimer.Cancel();
    iLaunchState = EIdle;
    }


// ---------------------------------------------------------------------------
// CWsfAppLauncher::RunL
// ---------------------------------------------------------------------------
//
void CWsfAppLauncher::RunL()
    {
    LOG_ENTERFN( "CWsfAppLauncher::RunL" ); 
    _ASSERTD( iObserver ); 
    User::LeaveIfError( iStatus.Int() );
    
    switch( iLaunchState )
        {
        case EIdle: // Starting to launch
            {
            LOG_WRITE( "CWsfAppLauncher::RunL -->> EIdle" );
            KillBrowserIfAlreadyExists();  
            break;    
            }
        case EShuttingDown: // Existing browser was succesfully closed 
            {
            LOG_WRITE( "CWsfAppLauncher::RunL -->> EShuttingDown" );
            DoLaunchBrowserL();
            break;    
            }
        case EStartingUp:  // Start-up completed, check browser exists 
            {
            LOG_WRITE( "CWsfAppLauncher::RunL -->> EStartingUp" );
            TBool exists = BrowserExists();
            TInt err = exists ? KErrNone : KErrNotFound;
            iLaunchState = ECompleted;
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, err );
            SetActive();    
            break;    
            }
        case ECompleted:  //Browser exists, notify observer about completion
            {
            LOG_WRITE( "CWsfAppLauncher::RunL -->> ECompleted" );  
            iLaunchState = EFinished;
            LOG_WRITE( "CWsfAppLauncher::RunL -->> Before thread open" );
            User::LeaveIfError( iThread.Open( iThreadId, EOwnerProcess ) );
            LOG_WRITE( "CWsfAppLauncher::RunL -->> Thread opened" );
            iObserver->BrowserLaunchCompleteL(); 
            TRequestStatus* status = &iStatus;
            iThread.Logon( *status );
            SetActive();    
            break;    
            }
        case EFinished:  //Browser exists, notify observer about completion
            {
            LOG_WRITE( "CWsfAppLauncher::RunL -->> EFinished" );
            iObserver->BrowserExitL();  
            iLaunchState = EIdle;
            break;    
            }
        default:
            {
            _ASSERTD( EFalse ); 
            }
        }    
    }

// ---------------------------------------------------------------------------
// CWsfAppLauncher::RunError
// ---------------------------------------------------------------------------
//
TInt CWsfAppLauncher::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfAppLauncher::RunError" ); 
    _ASSERTD( iObserver ); 

    switch( iLaunchState )
        {
        case EIdle:         // 
        case EShuttingDown: // Shuttind down existing browser failed
        case EStartingUp:   // Starting up new browser failed
        case ECompleted:   // Starting up new browser failed
            {   
            iObserver->BrowserLaunchFailed( aError );
            break;    
            }
        default:
            {
            _ASSERTD( EFalse ); 
            }
        }    
        
    iLaunchState = EIdle;
    return aError;
    }


