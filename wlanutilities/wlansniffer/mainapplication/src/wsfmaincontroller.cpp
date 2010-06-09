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
* Description:  Implementation of TWsfMainController.
*
*/

//  EXTERNAL INCLUDES
#include <e32def.h>
#include <wlansniffer.rsg>
#include <utf.h>
#include <apgcli.h>
#include <AknQueryDialog.h>
#include <StringLoader.h>

#include <cmmanagerext.h>

//  CLASS HEADER
#include "wsfmaincontroller.h"

//  INTERNAL INCLUDES
#include "wsfmodel.h"
#include "wsfwlaninfo.h"
#include "wsfwlaninfoarray.h"
#include "wsfappui.h"
#include "wsfmainviewcontrollerif.h"
#include "wsfdetailsviewcontrollerif.h"
#include "wsfmainapplication.hrh"
#include "wsfwlaninfoarrayvisitor.h"

#include "wsflogger.h"

//  LOCAL DEFINITIONS
#ifdef _DEBUG
    _LIT( KMainControllerPanic, "TWsfMainController" );
    #define _ASSERTD( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KMainControllerPanic, __LINE__) )
#else
    #define _ASSERTD( cond ) {}
#endif //_DEBUG

// ---------------------------------------------------------------------------
// TWsfMainController::Initialize
// ---------------------------------------------------------------------------
//
void TWsfMainController::Initialize( 
                            CWsfAppUi& aAppUi, 
                            CWsfModel& aModel, 
                            CWsfWlanInfoArrayVisitor &aWlanInfoBranding )
    {
    iAppUi = &aAppUi;
    iModel = &aModel;
    iWlanInfoBranding = &aWlanInfoBranding;
    iModel->SetConnecting( EFalse );
    }

// ---------------------------------------------------------------------------
// TWsfMainController::UpdateViewL
// ---------------------------------------------------------------------------
//
void TWsfMainController::UpdateViewL()
    {
    LOG_ENTERFN( "TWsfMainController::UpdateViewL" ); 
    CWsfWlanInfoArray* array = iModel->GetWlanListL();
    array->SortArrayL( *iWlanInfoBranding );
 
    LOG_WRITEF( "ActiveViewL: %x", iModel->ActiveViewL().iUid );
    switch ( iModel->ActiveViewL().iUid )
        {
        case EMainViewId:
            {  
            _ASSERTD( iMainView );          
            iMainView->UpdateWlanListL( array );  
            break;
            }

        case EDetailsViewId:
            {
            _ASSERTD( iDetailsView );
            iMainView->UpdateWlanListInBackgroundL( array );
            iDetailsView->WlanListChangedL( array );
            break;           
            }
        case EConnectedDetailsViewId:
            {
            iMainView->UpdateWlanListL( array );  
            iAppUi->UpdateConnectedDetailsL( array );
            break;           
            }
        default:
            {
            // Invalid view ID, make panic.
            _ASSERTD( 0 );   
            break;    
            }
        }
    }

// ---------------------------------------------------------------------------
// TWsfMainController::UpdateViewL
// ---------------------------------------------------------------------------
//
void TWsfMainController::UpdateViewL( CWsfWlanInfoArray* aInfoArray )
    {
    LOG_ENTERFN( "TWsfMainController::UpdateViewL( aInfoArray ) " );
    LOG_WRITEF( "ActiveViewL: %x", iModel->ActiveViewL().iUid );    
    switch ( iModel->ActiveViewL().iUid )
        {
        case EMainViewId:
            {            
            iMainView->UpdateWlanListL( aInfoArray );  
            break;
            }

        case EDetailsViewId:
            {
            iDetailsView->WlanListChangedL( aInfoArray );
            break;           
            }
        case EConnectedDetailsViewId:
            {
            iMainView->UpdateWlanListL( aInfoArray );  
            iAppUi->UpdateConnectedDetailsL( aInfoArray );
            break;           
            }
        default:
            {
            // Invalid view ID, make panic.
            _ASSERTD( 0 );   
            break;    
            }
        }    
    }

// Events from AppUi

// ---------------------------------------------------------------------------
// TWsfMainController::ShowWlanListL
// ---------------------------------------------------------------------------
//
void TWsfMainController::ShowWlanListL()
    {
    LOG_ENTERFN( "TWsfMainController::ShowWlanListL" ); 
    _ASSERTD( iAppUi );    
    _ASSERTD( iModel );
        
    iModel->SetActiveViewL( EMainViewId );
    iAppUi->ActivateMainViewL();
    }
        
// ---------------------------------------------------------------------------
// TWsfMainController::ShowDetailsL
// ---------------------------------------------------------------------------
//
void TWsfMainController::ShowDetailsL()
   {
   LOG_ENTERFN( "TWsfMainController::ShowDetailsL" ); 
    _ASSERTD( iAppUi );    
    _ASSERTD( iModel );
    _ASSERTD( iModel->ActiveViewL().iUid != EDetailsViewId );

    TWsfWlanInfo* selectedWlan = iMainView->SelectedItem();
    
    if ( selectedWlan ) 
        {
        TBool connected = selectedWlan->Connected();
        
        if( connected ) // Connected Details View - Replaced with dialog
            {
            // Work around - Emulated view
            iModel->SetActiveViewL( EConnectedDetailsViewId );
            // Holds selected wlan Ssid
            HBufC8* ssid = selectedWlan->GetSsidAsUtf8LC(); 
        
            // Holds current wlan array 
            if ( !iInfoArray )
                {
                iInfoArray = iModel->GetWlanListL();
                iInfoArray->SortArrayL( *iWlanInfoBranding );
                }
            // Start and execute the Connected Details dialog
            // Set array and Ssid to detail view
            iAppUi->StartConnectedDetailsL( iInfoArray, *ssid );
            CleanupStack::PopAndDestroy( ssid ); 
            
            // After the dismissal of the Connected Details dialog, 
            // change back to the main view 
            ShowWlanListL();
            }
        else
            {
            iDetailsView = &( iAppUi->DetailsView( EDetailsViewId ) ); 
              iModel->SetActiveViewL( EDetailsViewId );
            iDetailsView->SetPartner( *this );
        
            // Holds selected wlan Ssid
            HBufC8* ssid = selectedWlan->GetSsidAsUtf8LC(); 
        
            // Holds current wlan array 
            if ( !iInfoArray )
                {
                iInfoArray = iModel->GetWlanListL();
                iInfoArray->SortArrayL( *iWlanInfoBranding );
                }
            //Set array and Ssid to detail view
            iDetailsView->SetWlanListL( iInfoArray, *ssid );
        
            CleanupStack::PopAndDestroy( ssid );        
            
            iAppUi->ActivateDetailsViewL( iModel->ActiveViewL() );
            }
        }
    }

// ---------------------------------------------------------------------------
// TWsfMainController::MainViewCreatedL
// ---------------------------------------------------------------------------
//
void TWsfMainController::MainViewCreatedL()
    {
    LOG_ENTERFN( "TWsfMainController::MainViewCreatedL" );
    iMainView = &( iAppUi->MainView() ); 
    iMainView->SetPartner( *this );
    iModel->SetActiveViewL( EMainViewId );
    
    if ( !iModel->Refreshing() )
            { 
            iModel->SetRefreshState( ETrue );               
            }
     }

// ---------------------------------------------------------------------------
// TWsfMainController::WlanListChangedL
// ---------------------------------------------------------------------------
//  
void TWsfMainController::WlanListChangedL()
    {
    LOG_ENTERFN( "TWsfMainController::WlanListChangedL" ); 
    
    if ( iModel->IsConnecting() )
        {
        LOG_WRITE( "We are connecting just return from here" );
        return;
        }
    
    if ( iModel->Refreshing() )
        {
        // Make sure if leave occures that we don't block ourselves for retry
        CleanupStack::PushL( TCleanupItem( RestoreRefreshState, this ) );
        iModel->SetRefreshState( EFalse );
        if ( iAppUi )
            {            
            iAppUi->HideWaitNoteL();
            iInfoArray = iModel->GetWlanListL();
            if ( !iInfoArray->Count() )
                {        
                iAppUi->ShowNoWlansFoundInfoL();       
                }
            UpdateViewL();        
            }
        // pop cleanup item RestoreRefreshState
        CleanupStack::Pop();
        }
    }
    
// ---------------------------------------------------------------------------
// TWsfMainController::NotifyEngineError
// ---------------------------------------------------------------------------
//  
void TWsfMainController::NotifyEngineError( TInt aError )
    {
    LOG_ENTERFN( "TWsfMainController::NotifyEngineError" );
    switch ( aError )
        {
        case KErrServerTerminated:
            {
            break;
            }
            
        default:
            {
            TRAP_IGNORE( iAppUi->ShowGlobalErrorNoteL( aError ) );
            }
        }
    }

// ---------------------------------------------------------------------------
// TWsfMainController::ScanDisabledL
// ---------------------------------------------------------------------------
//          
void TWsfMainController::ScanDisabledL()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// TWsfMainController::ScanEnabledL
// ---------------------------------------------------------------------------
//  
void TWsfMainController::ScanEnabledL()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// TWsfMainController::WlanConnectionActivatedL
// ---------------------------------------------------------------------------
// 
void TWsfMainController::WlanConnectionActivatedL()
    {
    LOG_ENTERFN( "TWsfMainController::WlanConnectionActivatedL" ); 
    
    // update the model and refresh ui
    if ( iInfoArray )
        {
        TBool found = EFalse; 
        TWsfWlanInfo info;
        iModel->GetConnectedWlanDetailsL( info );
        if ( info.iIapId )
            {
            // find the entry for iapid
            // mark it connected - sort the array and refresh
            TWsfWlanInfo* temp = iInfoArray->Match( info.iIapId, 
                                                    iInfoArray->Count() );
            if ( temp )
               {
               found = ETrue;
               temp->iConnectionState = EConnected;
               iInfoArray->SortArrayL();
               UpdateViewL( iInfoArray );
               }
            }

        if ( !found )
            {
            // find the entry 
            // mark it connected - sort the array and refresh
            TWsfWlanInfo* temp = iInfoArray->Match( info.iSsid, 
                                                    iInfoArray->Count() );
            if ( temp )
                {
                temp->iConnectionState = EConnected;
                iInfoArray->SortArrayL();
                UpdateViewL( iInfoArray );
                }
            }
        }

    //Update icon
    iModel->RefreshScanL();   
    }

// ---------------------------------------------------------------------------
// TWsfMainController::WlanConnectionClosedL
// ---------------------------------------------------------------------------
//         
void TWsfMainController::WlanConnectionClosedL()
    {
    LOG_ENTERFN( "TWsfMainController::WlanConnectionClosedL" ); 
    // update the model and refresh ui
    if ( iInfoArray )
        {
        // check if the array has any items
        if ( iInfoArray->Count() )
            {
            TWsfWlanInfo* firstItem = iInfoArray->At( 0 );
            if ( firstItem && ( firstItem->Connected() ||
                                firstItem->iConnectionState == EConnecting ) )
                {
                LOG_WRITEF( "info state = %d", firstItem->iConnectionState );
                firstItem->iConnectionState = ENotConnected;
                TRAPD( error, iModel->CheckIsIapIdValidL( firstItem->iIapId ) );
                if ( error )
                    {
                    LOG_WRITEF( "Iap Id is not valid - error=%d", error );
                    if ( firstItem->iRawSsid.Length() )
                        {
                        firstItem->iSsid.Copy( firstItem->iRawSsid );
                        }
                    firstItem->iIapId = 0;
                    }
                iInfoArray->SortArrayL();
                UpdateViewL( iInfoArray );                    
                }
            }
        }

    // Abort current scan if exists so that we get 
    // newest scan results propagated to ui
    iModel->AbortScanningL();
    iModel->RefreshScanL();     
    }

// ---------------------------------------------------------------------------
// TWsfMainController::ConnectingFinishedL
// ---------------------------------------------------------------------------
//
void TWsfMainController::ConnectionCreationProcessFinishedL( TInt /*aResult*/ )
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// TWsfMainController::ShowMenuBarL
// ---------------------------------------------------------------------------
//
void TWsfMainController::ShowMenuBarL()
    {
    LOG_ENTERFN( "TWsfMainController::ShowMenuBarL" ); 
    iAppUi->ShowMenuBarL();
    }
    
// --------------------------- Events from the details view ------------------
    
// ---------------------------------------------------------------------------
// TWsfMainController::CloseDetailsViewL
// ---------------------------------------------------------------------------
//    
void TWsfMainController::CloseDetailsViewL()
    {
    LOG_ENTERFN( "TWsfMainController::CloseDetailsViewL" ); 
    ShowWlanListL();
    // After the Details View closed the mainview's 
    // WlanList will be refreshed.
    iModel->RefreshScanL();
    }

// ------------------------------- Menu press handler ------------------------


// ----------------------------------------------------------------------------
// TWsfMainController::StartBrowsingL
// ----------------------------------------------------------------------------
//
void TWsfMainController::StartBrowsingL()
    {
    LOG_ENTERFN( "TWsfMainController::StartBrowsingL" ); 
    if ( iAppUi->SuppressingKeyEvents() || iModel->IsConnectedL() && 
             ( !iMainView->SelectedItem() || 
               !iMainView->SelectedItem()->Connected() ) )
        {
        iAppUi->ShowErrorNoteL( R_QTN_WLAN_INFO_CONNECTION_ALREADY_ACTIVE );
        return;
        }
    
    // make sure we don't suppress our key events forever
    CleanupStack::PushL( TCleanupItem( ReleaseSuppressingKeyEvents, this ) );
    iAppUi->SetSuppressingKeyEvents( ETrue );
    
    TWsfWlanInfo* selectedItem = iMainView->SelectedItem();
    TWsfWlanInfo info;

    if ( selectedItem )
        {
        info = *selectedItem;
        }
    
    // Prevent connections to ad-hoc + WPA 
    if ( info.iNetMode == CMManager::EAdhoc 
            && info.iSecurityMode == CMManager::EWlanSecModeWpa )
        {
        iAppUi->ShowGlobalErrorNoteL( KErrNotSupported );
        CleanupStack::PopAndDestroy();
        return;
        }

    TInt result( KErrNone );
    
    if ( !info.Known() && !info.Connected() )
        {
        // if not known yet, create an IAP
        if ( iModel->CreateAccessPointL( info, EFalse ) )
            {
            // update iapID to list
            UpdateIapIdToInfoArrayL( info );
            
            // on success, test it and save it as well
            result = iModel->TestAccessPointL( info, ETrue, EFalse );
            }
        else
            {
            result = KErrGeneral;
            }
        }
    else if ( info.iIapId && !info.Connected() )
        {
        // hopefully we have a valid IAP id inside
        result = iModel->ConnectL( info.iIapId );
        }
    
    // pop cleanup item ReleaseSuppressingKeyEvents
    CleanupStack::Pop();
    iAppUi->SetSuppressingKeyEvents( EFalse );

    if ( !result )
        {
        TUint32 passedIap( info.iIapId );

        if ( info.Connected() && !passedIap )
            {
            // in case we have an EasyWLAN here
            RCmManagerExt cmmanager;
            cmmanager.OpenLC();

            passedIap = cmmanager.EasyWlanIdL();
            
            CleanupStack::PopAndDestroy( &cmmanager );
            }
        
        // launch the browser
        iModel->LaunchBrowserL( passedIap );
        }
    else if ( result == KErrCancel )
        {
        // connection creation was cancelled, refresh the view
        iModel->RefreshScanL();
        }
    
    }

    
// ----------------------------------------------------------------------------
// TWsfMainController::ConnectL
// ----------------------------------------------------------------------------
//
void TWsfMainController::ConnectL()
    {
    LOG_ENTERFN( "TWsfMainController::ConnectL" ); 
    if ( iAppUi->SuppressingKeyEvents() || iModel->IsConnectedL() )
        {
        iAppUi->ShowErrorNoteL( R_QTN_WLAN_INFO_CONNECTION_ALREADY_ACTIVE );
        return;
        }
    
    // make sure we don't suppress our key events forever
    CleanupStack::PushL( TCleanupItem( ReleaseSuppressingKeyEvents, this ) );
    iAppUi->SetSuppressingKeyEvents( ETrue );
    
    TWsfWlanInfo* selectedItem = iMainView->SelectedItem();
    TWsfWlanInfo info;

    if ( selectedItem )
        {
        info = *selectedItem;
        }
    
    // Prevent connections to ad-hoc + WPA 
    if ( info.iNetMode == CMManager::EAdhoc 
            && info.iSecurityMode == CMManager::EWlanSecModeWpa )
        {
        iAppUi->ShowGlobalErrorNoteL( KErrNotSupported );
        CleanupStack::PopAndDestroy();
        return;
        }
        
    if ( !info.Known() )
        {
        // a new access point needs to be created
        if ( iModel->CreateAccessPointL( info, EFalse ) )
            {
            // update iapID to list
            UpdateIapIdToInfoArrayL( info );

            // on success, test it and save it as well
            // (testing actually creates the connection)
            if ( iModel->TestAccessPointL( info, ETrue, ETrue ) == KErrCancel )
                {
                // connection creation was cancelled, refresh the view
                iModel->RefreshScanL();
                }
            }
        }
    else if ( info.iIapId )
        {
        // hopefully we have a valid IAP id inside
        iModel->ConnectL( info.iIapId );
        }

    // pop cleanup item ReleaseSuppressingKeyEvents
    CleanupStack::Pop();
    iAppUi->SetSuppressingKeyEvents( EFalse );  

    }

// ---------------------------------------------------------------------------
// TWsfMainController::UpdateIapIdToInfoArrayL
// ---------------------------------------------------------------------------
//
void TWsfMainController::UpdateIapIdToInfoArrayL( TWsfWlanInfo& aInfo )
    {
    LOG_ENTERFN( "TWsfMainController::UpdateIapIdToInfoArrayL" ); 
    TWsfWlanInfo* temp = iInfoArray->Match( aInfo.iSsid, iInfoArray->Count() );
    if ( temp && !aInfo.Hidden() )
        {
        LOG_WRITE( "Info found" );
        
        // Check that there aren't any IAPs with same id
        TWsfWlanInfo* wlanInfoWithSameIapId = iInfoArray->Match( aInfo.iIapId, 
                                                iInfoArray->Count() );
        if ( wlanInfoWithSameIapId )
            {
            // info with same id found set its iap id to zero
            LOG_WRITE( "info with same id found" );
            wlanInfoWithSameIapId->iIapId = 0;
            }
        
        temp->iIapId = aInfo.iIapId;
        
        if ( aInfo.iNetworkName.Length() )
            {
            // Replace ssid as well since scanner does this same thing
            temp->iSsid.Copy( aInfo.iNetworkName );
            }
        }
    else
        {
        LOG_WRITE( "Info not found" );
        TWsfWlanInfo* createdInfo = new (ELeave) TWsfWlanInfo( aInfo );
        createdInfo->iVisibility = ETrue;
        createdInfo->iStrengthLevel = EWlanSignalStrengthMax;
        iInfoArray->AppendL( createdInfo );
        }
    }
    
// ---------------------------------------------------------------------------
// TWsfMainController::DisconnectL
// ---------------------------------------------------------------------------
//
void TWsfMainController::DisconnectL()
    {
    LOG_ENTERFN( "TWsfMainController::DisconnectL" ); 
    if ( iModel->IsConnecting() )
        {
        LOG_WRITE( "Abort connecting" );
        iModel->AbortConnectingL();
        }
    else
        {
        LOG_WRITE( "Disconnect" );
        iModel->DisconnectL();
        }
    }

// ---------------------------------------------------------------------------
// TWsfMainController::RefreshL
// ---------------------------------------------------------------------------
//
void TWsfMainController::RefreshL()
    {
    LOG_ENTERFN( "TWsfMainController::RefreshL" ); 
    TBool refreshing = iModel->RefreshScanL();
    if ( refreshing )
        {    
        iAppUi->StartWaitNoteL( ETrue );
        }
    }

// ---------------------------------------------------------------------------
// TWsfMainController::ContinueBrowsingL
// ---------------------------------------------------------------------------
//        
void TWsfMainController::ContinueBrowsingL()
    {
    LOG_ENTERFN( "TWsfMainController::ContinueBrowsingL" ); 
    // pass selected WLAN info to engine
    TWsfWlanInfo *selectedItem = iMainView->SelectedItem();

    if ( selectedItem )
        {
        // Prevent connections to ad-hoc + WPA 
        if ( selectedItem->iNetMode == CMManager::EAdhoc 
                && selectedItem->iSecurityMode == CMManager::EWlanSecModeWpa )
            {
            iAppUi->ShowGlobalErrorNoteL( KErrNotSupported );
            return;
            }
        
        TUint iapId = selectedItem->iIapId;
        iModel->ContinueBrowsingL( iapId );
        }
    }

// ---------------------------------------------------------------------------
// TWsfMainController::EngineRefreshing
// ---------------------------------------------------------------------------
//     
TBool TWsfMainController::EngineRefreshing()
    {
    LOG_ENTERFN( "TWsfMainController::EngineRefreshing" );
    _ASSERTD( iModel );    
    return iModel->Refreshing();
    }
    
// ---------------------------------------------------------------------------
// TWsfMainController::AppLaunchCompleteL
// ---------------------------------------------------------------------------
// 
void TWsfMainController::AppLaunchCompleteL()
    {
    LOG_ENTERFN( "TWsfMainController::AppLaunchCompleteL" ); 
    iAppUi->StartWaitNoteL( EFalse );
    iModel->EnableScanL();
    }

// ---------------------------------------------------------------------------
// TWsfMainController::AppSwitchedForegroundL
// ---------------------------------------------------------------------------
// 
void TWsfMainController::AppSwitchedForegroundL()
    {
    LOG_ENTERFN( "TWsfMainController::AppSwitchedForegroundL" ); 

    //turn on scanning for our point of view
    iModel->EnableScanL();

    if ( !iModel->IsConnecting() )
        {
        LOG_WRITE( "Scan now" ); 
        TBool scan = iModel->RefreshScanL();
        if( !scan )
            {
            LOG_WRITE( "Set refresh state to true" );
            //if the refresh call was called when the server was
            //actually making a scan then the refresh state of the
            //model is turned to false, but in this case the scanning
            //wait note will stuck on the screen, That's why the
            //refresh state must be turned to true
            iModel->SetRefreshState( ETrue );
            }
        }
    }

// ---------------------------------------------------------------------------
// TWsfMainController::AppSwitchedBackgroundL
// ---------------------------------------------------------------------------
// 
void TWsfMainController::AppSwitchedBackgroundL()
    {
    LOG_ENTERFN( "TWsfMainController::AppSwitchedBackgroundL" ); 
    iModel->DisableScanL();
    }


// ---------------------------------------------------------------------------
// TWsfMainController::BrowserLaunchCompleteL
// ---------------------------------------------------------------------------
// 
void TWsfMainController::BrowserLaunchCompleteL()
    {
    LOG_ENTERFN( "TWsfMainController::BrowserLaunchCompleteL" );
    // no implementation required
    }


// ---------------------------------------------------------------------------
// TWsfMainController::BrowserExitL
// ---------------------------------------------------------------------------
// 
void TWsfMainController::BrowserExitL()
    {
    LOG_ENTERFN( "TWsfMainController::BrowserExitL" ); 
    iModel->AbortScanningL();
    iModel->RefreshScanL();
    }


// ---------------------------------------------------------------------------
// TWsfMainController::BrowserLaunchFailed
// ---------------------------------------------------------------------------
//
void  TWsfMainController::BrowserLaunchFailed( TInt aError )
    {
    LOG_ENTERFN( "TWsfMainController::BrowserLaunchFailed" ); 
    LOG_WRITEF( "error = %d", aError );
    switch ( aError )
        {
        case KErrCancel:
            {
            // user pressed cancel to connection creation
            break;
            }
        
        default:
            {
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// TWsfMainController::ConnectingL
// ---------------------------------------------------------------------------
//
void TWsfMainController::ConnectingL( TUint32 aIapId )
    {
    LOG_ENTERFN( "TWsfMainController::ConnectingL" ); 
    iModel->SetConnecting( ETrue );
    
    // update the model and refresh ui
    if ( iInfoArray )
        {
        LOG_WRITEF( "Match array with iapID = %d", aIapId );
        // find the entry for aIapId
        // mark it connecting - sort the array and refresh
        TWsfWlanInfo* temp = iInfoArray->Match( aIapId, iInfoArray->Count() );
        if ( temp )
            {
            LOG_WRITEF( "Info found iapID = %d", aIapId );
            temp->iConnectionState = EConnecting;
            iInfoArray->SortArrayL();
            UpdateViewL( iInfoArray );
            }
        }
    }


// ---------------------------------------------------------------------------
// TWsfMainController::ConnectingFinishedL
// ---------------------------------------------------------------------------
//
void TWsfMainController::ConnectingFinishedL( TInt aResult )
    {
    LOG_ENTERFN( "TWsfMainController::ConnectingFinishedL" ); 
    if ( iModel->IsConnecting() )
        {
        iModel->SetConnecting( EFalse );
        iModel->SetRefreshState( ETrue );
        iAppUi->HideWaitNoteL();
        }
    
    if ( aResult != KErrNone && aResult != KErrCancel )
        {
        LOG_WRITEF( "Connection failed with error=%d", aResult );
        if ( iInfoArray && iInfoArray->Count() && 
                iInfoArray->At(0)->iConnectionState == EConnecting )
            {
            iInfoArray->At(0)->iConnectionState = ENotConnected;
            TRAPD( error, iModel->CheckIsIapIdValidL( 
                                                iInfoArray->At(0)->iIapId ) );
            if ( error )
                {
                LOG_WRITEF( "Iap Id %d is not valid - error=%d", 
                            iInfoArray->At(0)->iIapId, error );
                
                if ( iInfoArray->At(0)->iRawSsid.Length() )
                    {
                    iInfoArray->At(0)->iSsid.Copy( 
                                                iInfoArray->At(0)->iRawSsid );
                    }
                
                iInfoArray->At(0)->iIapId = 0;
                }
            iInfoArray->SortArrayL();
            UpdateViewL( iInfoArray );
            iModel->AbortScanningL();
            iModel->RefreshScanL();
            }
        }
    }

    
// ---------------------------------------------------------------------------
// TWsfMainController::EditBlackListItemsL
// ---------------------------------------------------------------------------
//
void TWsfMainController::EditBlackListItemsL()
    {
    LOG_ENTERFN( "TWsfMainController::EditBlackListItemsL" ); 
    _ASSERTD( iWlanInfoBranding );
    iWlanInfoBranding->EditBlackListL( *iInfoArray );
    iModel->RefreshScanL();    
    }

// ---------------------------------------------------------------------------
// TWsfMainController::DetailsScanL
// ---------------------------------------------------------------------------
//    
void TWsfMainController::DetailsScanL()
    {
    LOG_ENTERFN( "TWsfMainController::DetailsScanL" ); 
    iModel->RefreshScanL();
    }    

// ---------------------------------------------------------------------------
// TWsfMainController::ReleaseSuppressingKeyEvents
// ---------------------------------------------------------------------------
//
void TWsfMainController::ReleaseSuppressingKeyEvents( TAny* aPtr )
    {
    TWsfMainController* self = 
                        static_cast<TWsfMainController*>( aPtr );
    self->iAppUi->SetSuppressingKeyEvents( EFalse );
    self->iModel->SetConnecting( EFalse );
    TRAP_IGNORE( self->iModel->RefreshScanL() );
    }

// ---------------------------------------------------------------------------
// TWsfMainController::RestoreRefreshState
// ---------------------------------------------------------------------------
//
void TWsfMainController::RestoreRefreshState( TAny* aPtr )
    {
    TWsfMainController* self = 
                        static_cast<TWsfMainController*>( aPtr );
    LOG_WRITE( "Restore refresh state")
    self->iModel->SetRefreshState( ETrue );
    }
      
// End of file
