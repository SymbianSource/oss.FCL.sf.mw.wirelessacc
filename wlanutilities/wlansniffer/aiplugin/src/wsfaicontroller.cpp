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
* Description:  Implementation of TWsfAiController.
*
*/



//  EXTERNAL INCLUDES
#include <apgcli.h>
#include <centralrepository.h>
#include <browseruisdkcrkeys.h>
#include <AknIconArray.h>
#include <AknQueryDialog.h>
#include <cmpluginwlandef.h>
#include <ErrorUI.h>
#include <StringLoader.h>
#include <aknnotewrappers.h>
#include <wsfaipluginrsc.rsg>
#include <rconnmon.h>
#include <utf.h>
#include <cmmanager.h>
#include <internetconnectivitycrkeys.h>
#include <wlanerrorcodes.h>
#include <ConnectionUiUtilities.h>

//  CLASS HEADER
#include "wsfaicontroller.h"

//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"
#include "wsfwlaninfoarray.h"
#include "wsfaiuiobserver.h"
#include "wsfaimodel.h"
#include "wsficonarraycreator.h"
#include "wsfmodel.h"
#include "wsfsearchwlansdialog.h"
#include "wsfaicontrollercmds.h"
#include "wsfactivewaiter.h"
#include "wsflogger.h"
#include "wsfdbobserver.h"
#include "wsfactivewrappers.h"


//  MACROS
#ifdef _DEBUG
    _LIT( KControllerPanic, "CWsfAiController" );
    #define _ASS_D( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KControllerPanic, __LINE__) )
#else
    #define _ASS_D( cond )
#endif //_DEBUG


//  LOCAL DEFINITIONS
LOCAL_D const TInt KFirstItemArrayIndex = 0;

// index value to suppress 1 second refreshing state
static const TInt KSuppressRefreshIndex = 1024;



// --------------------------------------------------------------------------
// TWsfAiController::TWsfAiController
// --------------------------------------------------------------------------
//
TWsfAiController::TWsfAiController():
    iModel( NULL ),
    iAiModel( NULL ),
    iUi( NULL ),
    iDbObserver( NULL ),
    iShouldConnectOnly( EFalse ),
    iTestAccessPoint( EFalse )
    {
    // null the pointers
    iConnectedWlan.iConnectionState = ENotConnected;
    }


// ---------------------------------------------------------
// CWsfActiveWaiter::DeInitializeL
// ---------------------------------------------------------
//    
void TWsfAiController::DeInitializeL()
    {
    LOG_ENTERFN( "TWsfAiController::DeInitializeL" );
    }


// --------------------------------------------------------------------------
// TWsfAiController::Initialize
// --------------------------------------------------------------------------
//
void TWsfAiController::InitializeL( CEikonEnv* aEnv, CWsfModel* aModel, 
                                    CWsfAiModel* aAiModel, 
                                    CWsfDbObserver* aObserver, 
                   					CWsfActiveWrappers* aActiveWrappers )
    {
    LOG_ENTERFN( "TWsfAiController::InitializeL" );
    iEnv = aEnv;
    iModel = aModel;
    iAiModel = aAiModel;
    iDbObserver = aObserver;
    iActiveWrappers = aActiveWrappers;
    iDbObserver->SetController( this );
    iDbObserver->ActivateItL();
    iModel->SetEngineObserver( this );
    }


// ---------------------------------------------------------------------------
// TWsfMainController::BrowserLaunchCompleteL
// ---------------------------------------------------------------------------
// 
void TWsfAiController::BrowserLaunchCompleteL()
    {
    LOG_ENTERFN( "TWsfAiController::BrowserLaunchCompleteL" );
    // no implementation required
    }


// ---------------------------------------------------------------------------
// TWsfMainController::BrowserExitL
// ---------------------------------------------------------------------------
// 
void TWsfAiController::BrowserExitL()
    {
    LOG_ENTERFN( "TWsfAiController::BrowserExitL" ); 
    // no implementation required
    }


// ---------------------------------------------------------------------------
// TWsfMainController::BrowserLaunchFailed
// ---------------------------------------------------------------------------
//
#ifdef _DEBUG
void TWsfAiController::BrowserLaunchFailed( TInt aError )
    {
    LOG_ENTERFN( "TWsfAiController::BrowserLaunchFailed" ); 
    LOG_WRITEF( "error = %d", aError );
#else
void TWsfAiController::BrowserLaunchFailed( TInt /*aError*/ )
    {   
    // no implementation required
#endif
    }


// ---------------------------------------------------------------------------
// TWsfMainController::ConnectingL
// ---------------------------------------------------------------------------
//
void TWsfAiController::ConnectingL( TUint32 /*aIapId*/ )
    {
    LOG_ENTERFN( "TWsfAiController::ConnectingL" ); 
    iModel->SetConnecting( ETrue );
    iAiModel->SetConnecting( ETrue );
    iUi->StartConnectingAnimationL();
    RefreshUiL();
    }


// ---------------------------------------------------------------------------
// TWsfMainController::ConnectingFinishedL
// ---------------------------------------------------------------------------
//
#ifdef _DEBUG
void TWsfAiController::ConnectingFinishedL( TInt aResult )
    {
    LOG_ENTERFN( "TWsfAiController::ConnectingFinishedL" );
    LOG_WRITEF( "aResult = %d", aResult );
#else
void TWsfAiController::ConnectingFinishedL( TInt /*aResult*/ )
    {
#endif
    if ( iModel->IsConnecting() )
        {
        iModel->SetConnecting( EFalse );
        }
    iAiModel->SetConnecting( EFalse );
    RefreshUiL();
    }


// --------------------------------------------------------------------------
// TWsfAiController::StartupRefresh
// --------------------------------------------------------------------------
//
void TWsfAiController::StartupRefresh()
    {
    LOG_ENTERFN( "TWsfAiController::StartupRefresh" );
    iActiveWrappers->RefreshWLANList( ETrue );
    }


// --------------------------------------------------------------------------
// TWsfAiController::StartupRefreshDataReadyL
// --------------------------------------------------------------------------
//
void TWsfAiController::StartupRefreshDataReadyL()
    {
    LOG_ENTERFN( "TWsfAiController::StartupRefreshDataReadyL" );
    
    iInfoArray = iActiveWrappers->GetWLANList();
    iConnectedWlan = iActiveWrappers->GetConnectedWLANNetwork();
    iAiModel->SetConnected( iConnectedWlan.Connected() );
    iAiModel->SetConnectedWlanInfo( iConnectedWlan );
    
    LOG_WRITEF( "Connected = %d", iConnectedWlan.Connected() );
    
    MDesCArray* data = iAiModel->FormatSingleLineWlanListL( iInfoArray );
    
    iUi->UpdateHotSpotsL( data, KSuppressRefreshIndex );    
    }


// --------------------------------------------------------------------------
// TWsfAiController::SetUi
// --------------------------------------------------------------------------
//
void TWsfAiController::SetUi( MWsfAiUiObserver& aUi )
    {
    LOG_ENTERFN( "TWsfAiController::SetUi" );
    iUi = &aUi;    
    }


// --------------------------------------------------------------------------
// TWsfAiController::HandleSelectionKeyL
// --------------------------------------------------------------------------
//
void TWsfAiController::HandleSelectionKeyL()
    {
    LOG_ENTERFN( "TWsfAiController::HandleSelectionKeyL" );
    _ASS_D( iAiModel );
    
    if ( !iAiModel->ScanningOn() && !iAiModel->Connected() )
        {
        // scanning was off and there wasn't a connected network
        EnableScanL();
        }
    else
        {
        // scanning was on
        // menu contents depend on the connection and browser status
        if ( iModel->IsConnecting() )
            {
            // connecting
            HandleMskIfConnectingL();
            }
        else if ( !iAiModel->Connected() )
            {
            // not connected
            HandleMskIfOfflineL();
            }
        else
            {
            // connected
            if ( IsWlanUsedByBrowserL() )
                {
                // browser is using active WLAN connection
                HandleMskIfBrowsingL();
                }
            else
                {
                // browser is not running or not using the WLAN
                HandleMskIfConnectedL();
                }
            }
        }
    }


// --------------------------------------------------------------------------
// TWsfAiController::WlanListChangedL
// --------------------------------------------------------------------------
//
void TWsfAiController::WlanListChangedL()
    {
    LOG_ENTERFN("TWsfAiController::WlanListChangedL" );
 
    _ASS_D( iAiModel );
    
    if ( !iAiModel->ScanningOn() )
        {
        return;
        }
    
    iActiveWrappers->RefreshWLANList( EFalse );
    }


// --------------------------------------------------------------------------
// TWsfAiController::WlanListDataReadyL
// --------------------------------------------------------------------------
//
void TWsfAiController::WlanListDataReadyL()
    {
    LOG_ENTERFN("TWsfAiController::WlanListDataReadyL" );
 
    _ASS_D( iAiModel );
    
    iInfoArray = iActiveWrappers->GetWLANList();
    iConnectedWlan = iActiveWrappers->GetConnectedWLANNetwork();
    iAiModel->SetConnected( iConnectedWlan.Connected() );
    iAiModel->SetConnectedWlanInfo( iConnectedWlan );
    
    LOG_WRITEF( "Connected = %d", iConnectedWlan.Connected() );
    
    if ( !iAiModel->ScanningOn() && !iAiModel->Connected() )
        {
        return;
        }
    
    _ASS_D( iModel );
    _ASS_D( iUi );
    
    TBool match( EFalse );    
    TWsfWlanInfo previousSelected;        
    CWsfWlanInfoArray* modelArr = iAiModel->GetInfoArray();   
    
    if ( modelArr )
        {
        TInt selectedItem = iUi->SelectedItem();
        if ( selectedItem >= KFirstItemArrayIndex && 
             selectedItem < modelArr->Count() )
            {
            TWsfWlanInfo* temp = modelArr->At( selectedItem );
            if ( temp )
                {
                match = ETrue;
                previousSelected = *temp;
                }                
            }
        }
        
    MDesCArray* data = iUi->MultilineControl() ? 
            iAiModel->FormatWlanListL( iInfoArray, ETrue ): 
            iAiModel->FormatSingleLineWlanListL( iInfoArray );
    
    
    TInt index( KErrNotFound );
    if ( match )
        {            
        TWsfWlanInfo* currentMatch = iInfoArray->Match( 
                                                    previousSelected.iSsid, 
                                                    iInfoArray->Count() );
        if ( currentMatch )
            {
            index = iInfoArray->GetArrayIndex( currentMatch );
            }
        }
    
    if ( !iUi->MultilineControl() && iModel->IsConnecting() )
        {
        index = KSuppressRefreshIndex;
        }
    
    iUi->UpdateHotSpotsL( data, index );
    }


// --------------------------------------------------------------------------
// TWsfAiController::NotifyEngineError
// --------------------------------------------------------------------------
//
void TWsfAiController::NotifyEngineError( TInt aError )
    {
    LOG_ENTERFN( "TWsfAiController::NotifyEngineError" );
    //Let the user know about the error.
    LOG_WRITEF( "error = %d", aError );
    TRAP_IGNORE( DoHandleEngineErrorL( aError ) ); 
    }


// --------------------------------------------------------------------------
// TWsfAiController::ScanDisabledL
// --------------------------------------------------------------------------
//
void TWsfAiController::ScanDisabledL()
    {
    LOG_ENTERFN( "TWsfAiController::ScanDisabledL" );
    // no implementation required
    }


// --------------------------------------------------------------------------
// TWsfAiController::ScanEnabledL
// --------------------------------------------------------------------------
//
void TWsfAiController::ScanEnabledL()
    {
    LOG_ENTERFN( "TWsfAiController::ScanEnabledL" );
    iActiveWrappers->RefreshScan();
    }


// --------------------------------------------------------------------------
// TWsfAiController::WlanConnectionActivatedL
// --------------------------------------------------------------------------
//
void TWsfAiController::WlanConnectionActivatedL()
    {
    LOG_ENTERFN( "TWsfAiController::WlanConnectionActivatedL" );
    iModel->SetConnecting( EFalse );
    iAiModel->SetConnected( ETrue );
    iAiModel->SetConnecting( EFalse );

    iActiveWrappers->RefreshWLANList( EFalse ); 
    } 


// --------------------------------------------------------------------------
// TWsfAiController::WlanConnectionClosedL
// --------------------------------------------------------------------------
//
void TWsfAiController::WlanConnectionClosedL()
    {
    LOG_ENTERFN( "TWsfAiController::WlanConnectionClosedL" );
    iModel->SetConnecting( EFalse );
    iAiModel->SetConnected( EFalse );
    iAiModel->SetConnecting( EFalse );
    if ( iAiModel->ScanningOn() )
        {
        iActiveWrappers->RefreshWLANList( EFalse ); 
        }
    else
        {
        iUi->DisplayEngineOffL();
        }
    }


// ---------------------------------------------------------------------------
// TWsfMainController::ConnectionCreationProcessFinished
// ---------------------------------------------------------------------------
//
void TWsfAiController::ConnectionCreationProcessFinishedL( TInt aResult )
    {
    LOG_ENTERFN( "TWsfAiController::ConnectionCreationProcessFinished" );
    LOG_WRITEF( "aResult = %d", aResult );
    // release flags if we leave occures
    CleanupStack::PushL( TCleanupItem( CleanUpConnectingL, this ) );
    if ( iModel->IsConnecting() )
        {
        iModel->SetConnecting( EFalse );
        }
    iAiModel->SetConnecting( EFalse );
    if ( aResult == KErrNone )
        {
        iAiModel->SetConnected( ETrue );
        if ( !iShouldConnectOnly )
            {
            StartWebBrowserL( iUsedInfo );
            }
        if ( iTestAccessPoint )
            {
            iModel->TestConnectedAccessPointL( iUsedInfo, iShouldConnectOnly );
            }
        }
    else
        {
        iAiModel->SetConnected( EFalse );
        }
    // pop cleanup item 
    CleanupStack::Pop();
    }


// --------------------------------------------------------------------------
// TWsfAiController::StartBrowsingL
// --------------------------------------------------------------------------
//
void TWsfAiController::StartBrowsingL( TWsfWlanInfo& aInfo )
    {
    LOG_ENTERFN( "TWsfAiController::StartBrowsingL" );
    
    _ASS_D( iModel );
    
    iUsedInfo = aInfo;
    
    // Prevent connections to ad-hoc + WPA 
    if ( iUsedInfo.iNetMode == CMManager::EAdhoc 
            && iUsedInfo.iSecurityMode == CMManager::EWlanSecModeWpa )
        {
        CErrorUI* errorUi = CErrorUI::NewLC();
        errorUi->ShowGlobalErrorNoteL( KErrNotSupported );
        CleanupStack::PopAndDestroy( errorUi ); // errorUi
        return;
        }
    
    // make sure we don't show connecting stuff forever
    CleanupStack::PushL( TCleanupItem( CleanUpConnectingL, this ) );
    
    if ( !iUsedInfo.Connected() )
        {
        ConnectL( iUsedInfo, EFalse, EFalse );
        }
    else
        {
        LOG_WRITE("Start browser to connected network...");
        iShouldConnectOnly = EFalse;
        StartWebBrowserL( iUsedInfo );
        }
    
    // pop cleanup item 
    CleanupStack::Pop();
    }


// --------------------------------------------------------------------------
// TWsfAiController::StartWebBrowserL
// --------------------------------------------------------------------------
//
void TWsfAiController::StartWebBrowserL( TWsfWlanInfo& aInfo )
    {
    LOG_ENTERFN( "TWsfAiController::StartWebBrowserL" );
    
    _ASS_D( iModel );
    
    TUint32 passedIap( aInfo.iIapId );

    if ( aInfo.Connected() && !passedIap )
        {
        // in case we have an EasyWLAN here
        RCmManager cmmanager;
        cmmanager.OpenLC();

        passedIap = cmmanager.EasyWlanIdL();

        CleanupStack::PopAndDestroy( &cmmanager );
        }
    
    // launch the browser
    iModel->LaunchBrowserL( passedIap );
    }


// --------------------------------------------------------------------------
// TWsfAiController::ConnectL
// --------------------------------------------------------------------------
//
void TWsfAiController::ConnectL( TWsfWlanInfo& aInfo, TBool aConnectOnly, 
                                 TBool aTestAccessPoint )
    {
    LOG_ENTERFN( "TWsfAiController::ConnectL" );
    
    _ASS_D( iModel );
    iTestAccessPoint = aTestAccessPoint;
    iShouldConnectOnly = aConnectOnly;
    iUsedInfo = aInfo;
    
    // Prevent connections to ad-hoc + WPA 
    if ( iUsedInfo.iNetMode == CMManager::EAdhoc 
            && iUsedInfo.iSecurityMode == CMManager::EWlanSecModeWpa )
        {
        CErrorUI* errorUi = CErrorUI::NewLC();
        errorUi->ShowGlobalErrorNoteL(KErrNotSupported);
        CleanupStack::PopAndDestroy(errorUi); // errorUi
        return;
        }
    
    // make sure we don't show connecting stuff forever
    CleanupStack::PushL( TCleanupItem( CleanUpConnectingL, this ) );
    
    if ( !iUsedInfo.iIapId && !iUsedInfo.Hidden() &&
            iUsedInfo.iSecurityMode == CMManager::EWlanSecModeOpen )
        {
        TBool success( EFalse );
        // just create the IAP
        TRAPD( accessPointCreatedError, 
                success = iModel->CreateAccessPointL( 
                iUsedInfo, EFalse ); 
        );
        
        // notify user that Disk is full or creation of a WAPI IAP 
        // is not supported on-the-fly and leave
        if ( accessPointCreatedError == KErrDiskFull ||
             accessPointCreatedError == KErrWlanProtectedSetupSetupLocked )
            {
            LOG_WRITE( "showing error note" );
            CErrorUI* errorUi = CErrorUI::NewLC();
            errorUi->ShowGlobalErrorNoteL( accessPointCreatedError );
            CleanupStack::PopAndDestroy( errorUi );
            User::Leave( accessPointCreatedError );
            }
        // just leave because we failed to create accesspoint
        else if ( accessPointCreatedError != KErrNone || !success )
            {
            User::Leave( accessPointCreatedError );
            }
        ConnectingL( iUsedInfo.iIapId );
        iTestAccessPoint = ETrue;
        }
    else if ( !iUsedInfo.iIapId )
        {
        // let the helper app do the query if necessary
        LOG_WRITE("AiHelper needs to be launched");
        iTestAccessPoint = ETrue;
        iActiveWrappers->LaunchHelperApplicationL( iUsedInfo, 
                                                   iShouldConnectOnly,
                                                   iTestAccessPoint );
        }
    
    // Connect
    if ( iUsedInfo.iIapId )
        {
        if ( iTestAccessPoint )
            {
            iActiveWrappers->Connect( iUsedInfo.iIapId, EIapExpireOnDisconnect );
            
            }
        else
            {
            iActiveWrappers->Connect( iUsedInfo.iIapId, EIapPersistent );
            }
        }
    // pop cleanup item 
    CleanupStack::Pop();
    }


// ---------------------------------------------------------------------------
// TWsfAiController::CleanUpConnectingL
// ---------------------------------------------------------------------------
//
void TWsfAiController::CleanUpConnectingL( TAny* aPtr )
    {
    LOG_ENTERFN( "TWsfAiController::CleanUpConnectingL" );
    TWsfAiController* self = static_cast<TWsfAiController*>( aPtr );
    self->iModel->SetConnecting( EFalse );
    self->iAiModel->SetConnecting( EFalse );
    TRAP_IGNORE( self->RefreshUiL());
    }


// --------------------------------------------------------------------------
// TWsfAiController::ContinueBrowsingL
// --------------------------------------------------------------------------
//
void TWsfAiController::ContinueBrowsingL()
    {
    LOG_ENTERFN( "TWsfAiController::ContinueBrowsingL" );

    _ASS_D( iModel );

    iModel->ContinueBrowsingL();
    }


// --------------------------------------------------------------------------
// TWsfAiController::LaunchSearchDialogL
// --------------------------------------------------------------------------
//
TBool TWsfAiController::LaunchSearchDialogL( TWsfWlanInfo& aInfo )
    {
    LOG_ENTERFN( "TWsfAiController::LaunchSearchDialogL" );
    
    _ASS_D( iAiModel );
    _ASS_D( iUi );
    
    MWsfAiUiObserver* temp = iUi;
    TInt selectedWlan( KFirstItemArrayIndex );
    TBool success( EFalse );

    TRAPD( err, success = DoLaunchSearchDialogL( selectedWlan ) );
    
    iUi = temp;
    User::LeaveIfError( err );
    
    if ( success )
        {
        if ( iAiModel->Connected() )
            {
            // show the error note of double connections
            HBufC* txt = StringLoader::LoadLC( 
                                 R_QTN_WLAN_INFO_CONNECTION_ALREADY_ACTIVE );
            
            CAknErrorNote* errorNote = new (ELeave) CAknErrorNote;
            errorNote->ExecuteLD( *txt );
            
            CleanupStack::PopAndDestroy( txt );
            success = EFalse;
            }
        else
            {
            CWsfWlanInfoArray* wlanArray = iAiModel->GetInfoArray(); 
            // check that the model still has the entry
            if ( wlanArray && selectedWlan < wlanArray->Count() )
                {
                // get wlan info
                aInfo = *wlanArray->At( selectedWlan );
                }
            else
                {
                // other unlisted item was selected
                }
            }
        }
    return success;
    }


// --------------------------------------------------------------------------
// TWsfAiController::DoLaunchSearchDialogL
// --------------------------------------------------------------------------
//
TBool TWsfAiController::DoLaunchSearchDialogL( TInt& aSelectedItem )
    {
    LOG_ENTERFN( "TWsfAiController::DoLaunchSearchDialogL" );
    
    _ASS_D( iAiModel );
    _ASS_D( iModel );
    
    CWsfSearchWlansDialog* dialog = CWsfSearchWlansDialog::NewL( 
                                                             aSelectedItem );
    dialog->PrepareLC( R_QTN_PLUGIN_SEARCH_WLAN_NETWORKS );

    // ownership of the icon array is passed
    CAknIconArray* icons = CWsfIconArrayCreator::CreateArrayL();
    CleanupStack::PushL( icons );
    iAiModel->AppendBrandIconsL( *icons );
    dialog->SetIconArrayL( icons ); // ownership of the icon array is passed
    CleanupStack::Pop( icons );

    // the dialog is ready to receive data...
    iUi = dialog;           
    iUi->UpdateHotSpotsL( iAiModel->FormatWlanListL( 
									iActiveWrappers->GetWLANList(), EFalse ),
                          			KFirstItemArrayIndex );
    
    return dialog->RunLD();
    }
    

// --------------------------------------------------------------------------
// TWsfAiController::RefreshUiL
// --------------------------------------------------------------------------
//
void TWsfAiController::RefreshUiL()
    {
    LOG_ENTERFN( "TWsfAiController::RefreshUiL" );

    _ASS_D( iAiModel );
    _ASS_D( iUi );

    if ( !iInfoArray )
        {
        return;
        }
    
    MDesCArray* data = iUi->MultilineControl() ? 
            iAiModel->FormatWlanListL( iInfoArray, ETrue ): 
            iAiModel->FormatSingleLineWlanListL( iInfoArray );
            
    TInt currentItem = ( iModel->IsConnecting() )? 
                          KSuppressRefreshIndex: KFirstItemArrayIndex;  

    iUi->UpdateHotSpotsL( data, currentItem );    
    }


// --------------------------------------------------------------------------
// TWsfAiController::EnableScanL
// --------------------------------------------------------------------------
//
void TWsfAiController::EnableScanL()
    {
    LOG_ENTERFN( "TWsfAiController::EnableScanL" );
    
    _ASS_D( iAiModel );
    _ASS_D( iModel );
    _ASS_D( iUi );
    
    if ( iAiModel->ScanningOn() )
        {
        return;
        }
    iModel->EnableScanL();
    iAiModel->SetScanningOn();
    
    LOG_WRITE( "DbObserver enable scan" );
    iDbObserver->EnableScanL();

    MDesCArray *data = iAiModel->FormatRefreshingL();
    iUi->UpdateHotSpotsL( data, KFirstItemArrayIndex );
    }


// --------------------------------------------------------------------------
// TWsfAiController::DisableScanL
// --------------------------------------------------------------------------
//
void TWsfAiController::DisableScanL()
    {
    LOG_ENTERFN( "TWsfAiController::DisableScanL" );
    
    _ASS_D( iAiModel );
    _ASS_D( iModel );
    _ASS_D( iUi );

    if ( !iAiModel->ScanningOn() )
        {
        return;
        } 
    iModel->DisableScanL();
    iAiModel->SetScanningOff();
    iDbObserver->DisableScanL();
    iUi->DisplayEngineOffL();
    }


// --------------------------------------------------------------------------
// TWsfAiController::DoHandleEngineErrorL
// --------------------------------------------------------------------------
//
void TWsfAiController::DoHandleEngineErrorL( TInt aError )
    {
    LOG_ENTERFN( "TWsfAiController::DoHandleEngineErrorL" );
    // start by freeing some resources... 
    // we could start by freeing wlan search results
    _ASS_D( iAiModel );

    // show error to user...
    iEnv->ResolveError( aError );
    }


// ---------------------------------------------------------------------------
// TWsfAiController::IsWlanUsedByBrowserL
// ---------------------------------------------------------------------------
//
TBool TWsfAiController::IsWlanUsedByBrowserL()
    {
    LOG_ENTERFN( "CWsfMainView::IsWlanUsedByBrowserL" );
    
    _ASS_D( iAiModel );
    
    const TInt KBrowserUid = 0x10008D39;
    TUid id( TUid::Uid( KBrowserUid ) );
    
    // Check if the Browser application is already running.
    TApaTaskList taskList( iEnv->WsSession() );
    TApaTask task = taskList.FindApp( id );
    TBool isWlanUsed( EFalse );
    
#ifndef __WINS__

    if ( task.Exists() )
        {
        LOG_WRITE( "The Browser is running." );
        // The Browser is already running.
        // Check if the selected WLAN is connected.
        TBool connected( iAiModel->Connected() );
        LOG_WRITEF("Connected? %d", connected);

        if ( connected )
            {
            RConnectionMonitor connectionMonitor;
            connectionMonitor.ConnectL();
            // We have to check if the Browser using the selected 
            // connection.
            // We have get the connectiond ID first.
            CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewL();
            CleanupStack::PushL( waiter );
            
            // first call, need to get the connection parameters
            TUint connectionCount( 0 );
            
            connectionMonitor.GetConnectionCount( connectionCount, 
                                                  waiter->iStatus );
            waiter->WaitForRequest();
            
            // find the connection with WLAN bearer 
            // that will be the selected WLAN
            TUint connectionId( 0 );
            for ( TUint i = 1; i <= connectionCount; ++i )
                {
                TUint tmpConnectionId( 0 );
                TUint subConnectionCount( 0 );
                TConnMonBearerType bearerType( EBearerUnknown );
                
                connectionMonitor.GetConnectionInfo( i, 
                                                     tmpConnectionId, 
                                                     subConnectionCount );
                connectionMonitor.GetIntAttribute( tmpConnectionId, 
                                                   0, 
                                                   KBearer, 
                                                   ( TInt& )bearerType, 
                                                   waiter->iStatus );
                waiter->WaitForRequest();
                if ( bearerType == EBearerWLAN )
                    {
                    connectionId = tmpConnectionId;
                    break;
                    }
                }
            // Now we have the connectiond ID.
            LOG_WRITEF( "connectionId: %d", connectionId );
            
            TInt count( 0 );
            TConnMonClientEnumBuf clientBuf;
                
            connectionMonitor.GetPckgAttribute( connectionId,
                                                0,
                                                KClientInfo,
                                                clientBuf,
                                                waiter->iStatus );
            waiter->WaitForRequest();
            if ( !waiter->iStatus.Int() )
                {
                count = clientBuf().iCount;
                }
            
            RApaLsSession appSess;
            TApaAppInfo appInfo;
            
            // The connection could be shared by several applications
            User::LeaveIfError( appSess.Connect() );
            
            for ( TUint i = 0; i < count; i++ )
                {
                appInfo.iCaption.Zero();
                TInt result = appSess.GetAppInfo( appInfo, 
                                                  clientBuf().iUid[i] );
                
                LOG_WRITEF( "result: %d", result ); 
                LOG_WRITEF( "iClientInfo.iUid[i].iUid: %x", 
                             clientBuf().iUid[i].iUid );
                
                if ( clientBuf().iUid[i].iUid == KBrowserUid )
                    {
                    isWlanUsed = ETrue;
                    }
                }
            CleanupStack::PopAndDestroy( waiter );
            appSess.Close();
            connectionMonitor.Close();
            }                
        }
    else 
        {
        LOG_WRITE( "No Browser running." );
        }
        
#endif // __WINS__
    
    LOG_WRITEF( "isWlanUsedByBrowser: %d", isWlanUsed ); 
    return isWlanUsed;
    }


// --------------------------------------------------------------------------
// TWsfAiController::HandleMskIfOfflineL
// --------------------------------------------------------------------------
//
void TWsfAiController::HandleMskIfOfflineL()
    {
    LOG_ENTERFN( "TWsfAiController::HandleMskIfOfflineL" );
    
    // create the dialog with options
    TInt selectedMode( 0 );
    CAknListQueryDialog* listQuery = new (ELeave) CAknListQueryDialog( 
                                                           &selectedMode );
    listQuery->PrepareLC( R_QTN_AIPLUGIN_OPTIONS_OFFLINE );

    if ( !listQuery->RunLD() )
        {
        // cancelled
        return;
        }
    
    CWsfWlanInfoArray* wlanArray = iAiModel->GetInfoArray(); 
    
    switch ( selectedMode )
        {
        case EAiOfflineStartBrowsing:
            {
            // Start web browsing was selected
            if ( !wlanArray || !wlanArray->Count() ||
                 !wlanArray->At( KFirstItemArrayIndex )->Known() )
                {
                // no known networks, let the user choose one
                TWsfWlanInfo info;
                
                if ( LaunchSearchDialogL( info ) )
                    {
                    StartBrowsingL( info );
                    }
                }
            else
                {
                // there are known networks
                StartBrowsingL( *wlanArray->At( KFirstItemArrayIndex ) );
                }
            
            break;
            }
            
        case EAiOfflineConnect:
            {
            // Connect was selected
            if ( !wlanArray || !wlanArray->Count() ||
                 !wlanArray->At( KFirstItemArrayIndex )->Known() )
                {
                // no known networks, let the user choose one
                TWsfWlanInfo info;
                
                if ( LaunchSearchDialogL( info ) )
                    {
                    ConnectL( info, ETrue, EFalse );
                    }
                }
            else
                {
                // there are known networks
                ConnectL( *wlanArray->At( KFirstItemArrayIndex ), ETrue, 
                          EFalse );
                }
            
            break;
            }
            
        case EAiOfflineSearchWlans:
            {
            // Search for wlans was selected
            if ( !wlanArray || !wlanArray->Count() )
                {
                // there are no available networks
                CConnectionUiUtilities* connUiUtils = CConnectionUiUtilities::NewL();        
                connUiUtils->NoWLANNetworksAvailableNote();
                delete connUiUtils;
                }

            // launch the search dialog
            TWsfWlanInfo info;
            
            if ( LaunchSearchDialogL( info ) )
                {
                ConnectL( info, ETrue, EFalse );
                }
            else
                {
                // User canceled Search Wlans dialog and probably did not 
                // find anything interesting from the list - request scan
                iActiveWrappers->RefreshScan();
                }

            break;
            }
            
        case EAiOfflineDisableScan:
            {
            // disable the scan
            DisableScanL();
            break;
            }
            
        default:
            {
            }
        }
    }



// --------------------------------------------------------------------------
// TWsfAiController::HandleMskIfConnectedL
// --------------------------------------------------------------------------
//
void TWsfAiController::HandleMskIfConnectedL()
    {
    LOG_ENTERFN( "TWsfAiController::HandleMskIfConnectedL" );
    
    _ASS_D( iModel );
    
    TInt selectedMode( 0 );
    CAknListQueryDialog* listQuery = new (ELeave) CAknListQueryDialog( 
                                                           &selectedMode );
    listQuery->PrepareLC( R_QTN_AIPLUGIN_OPTIONS_CONNECTED );
    
    if ( listQuery->RunLD() )
        {
        if ( selectedMode == EAiConnectedStartBrowsing )
            {
            StartBrowsingL( iConnectedWlan );
            }
        else if ( selectedMode == EAiConnectedDisconnect )
            {
            // disconnect wlan
            iActiveWrappers->Disconnect();     
            }
        }
    }


// --------------------------------------------------------------------------
// TWsfAiController::HandleMskIfConnectingL
// --------------------------------------------------------------------------
//
void TWsfAiController::HandleMskIfConnectingL()
    {
    LOG_ENTERFN( "TWsfAiController::HandleMskIfConnectingL" );
    
    _ASS_D( iModel );
    
    TInt selectedMode( 0 );
    CAknListQueryDialog* listQuery = new (ELeave) CAknListQueryDialog( 
                                                           &selectedMode );
    listQuery->PrepareLC( R_QTN_AIPLUGIN_OPTIONS_CONNECTING );
    
    if ( listQuery->RunLD() )
        {
        if ( selectedMode == EAiConnectingDisconnect )
            {
            if ( !iModel->IsConnecting() )
                {
                // disconnect wlan
                iActiveWrappers->Disconnect();
                }
            else
                {
                // disconnect wlan
                iModel->AbortConnectingL(); 
                ConnectingFinishedL( KErrCancel );
                }
            }
        }
    }


// --------------------------------------------------------------------------
// TWsfAiController::HandleMskIfBrowsingL
// --------------------------------------------------------------------------
//
void TWsfAiController::HandleMskIfBrowsingL()
    {
    LOG_ENTERFN( "TWsfAiController::HandleMskIfBrowsingL" );
    
    _ASS_D( iModel );
    
    TInt selectedMode( KFirstItemArrayIndex );
    CAknListQueryDialog* listQuery = new (ELeave) CAknListQueryDialog( 
                                                           &selectedMode );
    listQuery->PrepareLC( R_QTN_AIPLUGIN_OPTIONS_BROWSING );

    if ( listQuery->RunLD() )
        {
        if ( selectedMode == EAiBrowsingContinueBrowsing )
            {
            // launch the browser...
            ContinueBrowsingL();
            }
        else if ( selectedMode == EAiBrowsingDisconnect )
            {
            // disconnect wlan
            iActiveWrappers->Disconnect();
            }
        }
    }

// --------------------------------------------------------------------------
// TWsfAiController::RefreshRefreshingL
// --------------------------------------------------------------------------
//
void TWsfAiController::RefreshRefreshingL()
	{
	LOG_ENTERFN( "TWsfAiController::RefreshRefreshingL" );
	if ( !iUi->MultilineControl() )
	    {
        MDesCArray *data = iAiModel->FormatRefreshingL();
        iUi->UpdateViewL( data );
	    }
	else
	    {
        LOG_WRITE( "Multiline control in use - no update done" );
	    }
	}

// --------------------------------------------------------------------------
// TWsfAiController::RefreshConnectingL
// --------------------------------------------------------------------------
//
void TWsfAiController::RefreshConnectingL()
    {
    LOG_ENTERFN( "TWsfAiController::RefreshConnectingL" );
    if ( !iUi->MultilineControl() )
        {
        MDesCArray *data = iAiModel->FormatConnectingL();
        iUi->UpdateViewL( data );
        }
    else       
        {
        LOG_WRITE( "Multiline control in use - no update done" );
        }
    }

// End of file
