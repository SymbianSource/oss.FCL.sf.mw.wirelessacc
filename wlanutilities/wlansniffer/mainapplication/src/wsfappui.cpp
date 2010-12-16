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
* Description:  Implementation of CWsfAppUi.
*
*/

#include <avkon.hrh>
#include <aknnotewrappers.h> 
#include <eikmenub.h> 
#include <hlplch.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <cmmanager.h>
#include <cmconnectionmethod.h>
#include <centralrepository.h>
#include <ErrorUI.h>
#include <wlansniffer.rsg>
#include <ConnectionUiUtilities.h>

#include "wlansettingsui.h"
#include "wsfappui.h"
#include "wsfmainview.h"
#include "wsfdetailsview.h"
#include "wsfmainviewcontrollerif.h"
#include "wsfdetailsviewcontrollerif.h"
#include "wsfmainuiobserver.h"
#include "wsfmainapplication.hrh"
#include "wsfdocument.h"
#include "wsfconnecteddetailsdialog.h"
#include "wsfconnecteddetailsmodel.h"
#include "wsfmodel.h"
#include "wsflogger.h"

//  LOCAL DEFINITIONS
#ifdef _DEBUG
    _LIT( KAppUi, "CWsfAppUi" );
    #define _ASSERTD( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KAppUi, __LINE__) )
#else
    #define _ASSERTD( cond ) {}
#endif //_DEBUG

// ---------------------------------------------------------------------------
// CWsfAppUi::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::ConstructL()
    {
    LOG_CREATE;
    LOG_WRITE( "Constructing Wlan Sniffer" );
    LOG_ENTERFN( "CWsfAppUi::ConstructL" );
    
    BaseConstructL( EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible );
    
    // Initialize feature manager
    FeatureManager::InitializeLibL();
    iFeatureManagerInitialized = ETrue;
    
    iIdle = CIdle::NewL( CActive::EPriorityIdle );
    
    // check if the app. launched in embedded mode
    TBool isEmbeddedApp = IsEmbedded();
    LOG_WRITEF( "IsEmbedded(): %d", isEmbeddedApp );
     
    //Create mainview
    LOG_WRITE( "Creating main view" );
    CWsfMainView* mainView = CWsfMainView::NewLC( isEmbeddedApp );
    LOG_WRITE( "Adding main view" );    
    AddViewL( mainView );           // transfer ownership to CAknViewAppUi
    CleanupStack::Pop( mainView );
    LOG_WRITE( "main view creating done" );
    SetDefaultViewL( *mainView );
    iObserver->MainViewCreatedL();

    //Create details view (not connected)
    LOG_WRITE( "Create details view (not connected)" );    
    CWsfDetailsView* detailsView = CWsfDetailsView::NewLC();    
    LOG_WRITE( "Adding details view (not connected)" );    
    AddViewL( detailsView );        // transfer ownership to CAknViewAppUi
    CleanupStack::Pop( detailsView );
    LOG_WRITE( "details view (not connected) creating done" );
    
    ActivateMainViewL();

    iIdle->Start( TCallBack( AppLaunchCompleteL, this ) );
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::CWsfAppUi
// ---------------------------------------------------------------------------
//
CWsfAppUi::CWsfAppUi():
    iConnectedDetailsDialog( NULL ),
    iForeground( ETrue ),
    iFeatureManagerInitialized( EFalse )
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::~CWsfAppUi
// ---------------------------------------------------------------------------
//
CWsfAppUi::~CWsfAppUi()
    {
    LOG_ENTERFN( "CWsfAppUi::~CWsfAppUi" );
    if ( iWaitDialog )
        {
        iWaitDialog->SetCallback( NULL );
        TRAP_IGNORE( iWaitDialog->ProcessFinishedL() );
        }

    if ( iFeatureManagerInitialized )
        {
        FeatureManager::UnInitializeLib();
        }
    
    delete iIdle;
    delete iConnectedDetailsDialog;
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::HandleCommandL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::HandleCommandL( TInt aCommand )
    {
    LOG_ENTERFN( "CWsfAppUi::HandleCommandL" );
    switch ( aCommand )
        {
        case EEikCmdExit:
        case EAknCmdExit:
        case EAknSoftkeyExit:
            {
            if ( iAppLaunchCompleted )
                {
                Exit();
                }
            break;
            }

        case ESnifferCmdStartBrowsing:
            {
            iObserver->StartBrowsingL();
            break;       
            }

        case ESnifferCmdContinueBrowsing:
            {
            iObserver->ContinueBrowsingL();
            break;       
            }

        case ESnifferCmdShowWlans:
            {
            iObserver->ShowWlanListL();
            break;       
            }
            
        case ESnifferCmdConnect:
            {
            iObserver->ConnectL();
            break;           
            }
        case ESnifferCmdDisconnect:
            {
            iObserver->DisconnectL();
            break;                           
            }        
        case ESnifferCmdRefresh:
            {
            iObserver->RefreshL();  
            break;  
            }
        case ESnifferCmdDetails:
            {
            iObserver->ShowDetailsL();
            break;    
            }
        case ESnifferCmdFilterWlans:
            {
            iObserver->EditBlackListItemsL();
            break;    
            }
        case ESnifferCmdSettings:
            {
            // launch settings dialog
            CWlanSettingsUi* wlanSettings = 
                                 CWlanSettingsUi::NewL( *iEikonEnv );
            CleanupStack::PushL( wlanSettings );
            iForeground = EFalse;
            wlanSettings->RunSettingsL();
            iForeground = ETrue;
            CleanupStack::PopAndDestroy( wlanSettings );
            break;
            }
        case EAknCmdHelp:
            {
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(), 
                                                     AppHelpContextL() );
                }
            break;
            }
        
        default:
            {                             
            break;
            }            
        }
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::SetUiObserver
// ---------------------------------------------------------------------------
//
void CWsfAppUi::SetUiObserver( MWsfMainUiObserver* aObserver )
    {
    iObserver = aObserver;
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::HandleResourceChangeL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::HandleResourceChangeL( TInt aType )
    {
    CAknAppUi::HandleResourceChangeL( aType );
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::HandleForegroundEventL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::HandleForegroundEventL( TBool aForeground )
    {
    LOG_ENTERFN( "CWsfAppUi::HandleForegroundEventL" );
    //Application switched to foreground.
    if ( aForeground )
        {
        CWsfMainView* mainView = static_cast<CWsfMainView*>( 
                                            View( TUid::Uid( EMainViewId ) ) );
        if ( mainView )
            {
            TRAPD( err, mainView->UpdateBrowserUsageInfoL() );
            if ( err )
                {
                LOG_WRITEF( "Browser iap usage update failed - err=%d", err );
                }
            }
    
        if ( iObserver )
            {
            iObserver->AppSwitchedForegroundL();
            }
        }
    else
        {
        if ( iObserver )
            {            
            iObserver->AppSwitchedBackgroundL();
            }
            
        }
        
    CAknViewAppUi::HandleForegroundEventL( aForeground );
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::ActivateDetailsViewL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::ActivateDetailsViewL( TUid aDetailsViewId )
    {
    ActivateLocalViewL( aDetailsViewId );
    LOG_WRITE( "CWsfAppUi::ActivateDetailsViewL" );
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::ActivateMainViewL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::ActivateMainViewL()
    {
    ActivateLocalViewL( TUid::Uid( EMainViewId ) );
    LOG_WRITE( "CWsfAppUi::ActivateMainViewL" );    
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::MainView
// ---------------------------------------------------------------------------
//
MWsfMainViewControllerIf& CWsfAppUi::MainView()
    {   
    CWsfMainView* mainView = static_cast<CWsfMainView*>( 
                                  View( TUid::Uid( EMainViewId ) ) ); 
    return mainView->Controller();
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::DetailsView
// ---------------------------------------------------------------------------
//
MWsfDetailsViewControllerIf& CWsfAppUi::DetailsView( TInt aDetailsViewType )
    {
    CWsfDetailsView* detailsView = static_cast<CWsfDetailsView*>( 
                                   View( TUid::Uid( aDetailsViewType ) ) ); 
    return detailsView->Controller();
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::StartWaitNoteL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::StartWaitNoteL( TBool aVisibilityDelayOff )
    {
    LOG_ENTERFN( "CWsfAppUi::StartWaitNoteL" );
    // wait dialog shown only if engine updating
    // the wlan list
    if( iObserver->EngineRefreshing() )
        {
        iWaitDialog = new (ELeave) CAknWaitDialog( 
                              reinterpret_cast<CEikDialog**>( &iWaitDialog ), 
                              aVisibilityDelayOff );
        iWaitDialog->SetCallback( this );
        iWaitDialog->ExecuteLD( R_WAITNOTE_SEARCHING_WLANS );
        }
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::HideWaitNoteL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::HideWaitNoteL()
    {
    LOG_ENTERFN( "CWsfAppUi::HideWaitNoteL" );
    if ( iWaitDialog )
        {        
        iWaitDialog->ProcessFinishedL();
        iWaitDialog = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::DialogDismissedL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::DialogDismissedL( TInt aButtonId )
    {
    LOG_ENTERFN( "CWsfAppUi::DialogDismissedL" );
    iWaitDialog = NULL;
    
    if ( aButtonId == EEikBidCancel )
        {
        // dialog was cancelled by the user
        
        if ( Doc()->Model().IsConnecting() )
            {
            // connecting wait note was active
            LOG_WRITE( "cancelling 'connecting' wait note" );
            Doc()->Model().AbortConnectingL();
            Doc()->Model().SetConnecting( EFalse );
            }
        else
            {
            // searching for wlan wait note was active
            LOG_WRITE( "cancelling 'searching for wlan' wait note" );
            Doc()->Model().AbortScanningL();
            Doc()->Model().SetRefreshState( EFalse );
            }
        }
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::ShowMenuBarL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::ShowMenuBarL()
    {
    CWsfMainView* mainview = static_cast<CWsfMainView*>( iView );
    mainview->SelectionKeyPressed();
    iView->MenuBar()->TryDisplayMenuBarL();
    }
    

// ---------------------------------------------------------------------------
// CWsfAppUi::ShowNoWlansFoundInfoL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::ShowNoWlansFoundInfoL()
    {
    LOG_ENTERFN( "CWsfAppUi::ShowNoWlansFoundInfoL" );
    
    CConnectionUiUtilities* connUiUtils = CConnectionUiUtilities::NewL();                                
    connUiUtils->NoWLANNetworksAvailableNote();
    delete connUiUtils;
    }
    

// ---------------------------------------------------------------------------
// CWsfAppUi::ShowErrorNoteL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::ShowErrorNoteL( TInt aResourceId )
    {
    LOG_ENTERFN( "CWsfAppUi::ShowErrorNoteL" );
    HBufC* txt = StringLoader::LoadLC( aResourceId );
    
    CAknErrorNote* errorNote = new (ELeave) CAknErrorNote;
    errorNote->ExecuteLD( *txt );
    
    CleanupStack::PopAndDestroy( txt );
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::ShowGlobalErrorNoteL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::ShowGlobalErrorNoteL( TInt aError )
    {
    LOG_ENTERFN( "CWsfAppUi::ShowGlobalErrorNoteL" );
    CErrorUI* errorUi = CErrorUI::NewLC( *CCoeEnv::Static() );
    errorUi->ShowGlobalErrorNoteL( aError );
    CleanupStack::PopAndDestroy( errorUi );
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::AppLaunchCompleteL
// ---------------------------------------------------------------------------
//
TInt CWsfAppUi::AppLaunchCompleteL( TAny* aObject )
    {
    LOG_ENTERFN( "CWsfAppUi::AppLaunchCompleteL" );
    _ASSERTD( aObject );
    CWsfAppUi* appUi = static_cast<CWsfAppUi*>( aObject );
    appUi->iAppLaunchCompleted = ETrue;
    appUi->StartWaitNoteL( EFalse );
    return 0;   // Return false (zero) value not be called again. (Stops timer)
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::IsEmbedded
// ---------------------------------------------------------------------------
//
TBool CWsfAppUi::IsEmbedded() const
    {
    LOG_ENTERFN( "CWsfAppUi::IsEmbedded" );
    return iEikonEnv->StartedAsServerApp();
    }    
    

// ---------------------------------------------------------------------------
// CWsfAppUi::Doc
// ---------------------------------------------------------------------------
//
CWsfDocument* CWsfAppUi::Doc()
     {
     return static_cast<CWsfDocument*>( Document() );
     }


// ---------------------------------------------------------------------------
// CWsfAppUi::Foreground
// ---------------------------------------------------------------------------
//
TBool CWsfAppUi::Foreground()
    {
    return iForeground;
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::StartConnectedDetailsL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::StartConnectedDetailsL( CWsfWlanInfoArray* aWlanArray, 
                                        const TDesC8& aSelectedWlanSsid )
    {
    LOG_ENTERFN( "CWsfAppUi::StartConnectedDetailsL" );
    CWsfConnectedDetailsModel* model = CWsfConnectedDetailsModel::NewLC();
    iConnectedDetailsDialog = CWsfConnectedDetailsDialog::NewL();
    iConnectedDetailsDialog->SetListModel(model);
    iConnectedDetailsDialog->Controller().SetWlanListL( aWlanArray, 
                                                        aSelectedWlanSsid );
    iConnectedDetailsDialog->ExecuteLD( R_QTN_CONNECTED_DETAILS_DIALOG );
    iConnectedDetailsDialog = NULL;
    CleanupStack::PopAndDestroy(model);
    }
    
    
// ---------------------------------------------------------------------------
// CWsfAppUi::UpdateConnectedDetailsL
// ---------------------------------------------------------------------------
//
void CWsfAppUi::UpdateConnectedDetailsL( CWsfWlanInfoArray* aWlanArray )
    {
    LOG_ENTERFN( "CWsfAppUi::UpdateConnectedDetailsL" );
    if ( iConnectedDetailsDialog != NULL )
        {
        iConnectedDetailsDialog->Controller().WlanListChangedL( aWlanArray );
        }
    }    


// ---------------------------------------------------------------------------
// CWsfAppUi::SuppressingKeyEvents
// ---------------------------------------------------------------------------
//
TBool CWsfAppUi::SuppressingKeyEvents() const
    {
    return iSuppressingKeyEvents;
    }


// ---------------------------------------------------------------------------
// CWsfAppUi::SetSuppressingKeyEvents
// ---------------------------------------------------------------------------
//
void CWsfAppUi::SetSuppressingKeyEvents( TBool aSuppressing )
    {
    iSuppressingKeyEvents = aSuppressing;
    }

// End of file
