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
* Description:  Implementation of CWsfMainView.
*
*/

//External Includes
#include <coemain.h>
#include <eikmenub.h>
#include <aknViewAppUi.h>
#include <wlansniffer.rsg>
#include <featmgr.h>
#include <rconnmon.h>
#include <apgcli.h>

//Internal Includes
#include "wsfdocument.h"
#include "wsfwlaninfoarrayvisitor.h"
#include "wsfwlaninfoarray.h"
#include "wsfmainview.h"
#include "wsfmainviewmodel.h"
#include "wsfmainviewcontainer.h"
#include "wsfmainapplication.hrh"
#include "wsfactivewaiter.h"
#include "wsfappui.h"

#include "wsflogger.h"

    
// Standard construction sequence

// ---------------------------------------------------------------------------
// CWsfMainView::NewL
// ---------------------------------------------------------------------------
//
CWsfMainView* CWsfMainView::NewL( TBool aIsEmbedded )
    {
    LOG_ENTERFN( "CWsfMainView::NewL" );
    CWsfMainView* self = CWsfMainView::NewLC( aIsEmbedded );
    CleanupStack::Pop( self );
    return self;
    } 


// ---------------------------------------------------------------------------
// CWsfMainView::NewLC
// ---------------------------------------------------------------------------
//
CWsfMainView* CWsfMainView::NewLC( TBool aIsEmbedded )
    {
    LOG_ENTERFN( "CWsfMainView::NewLC" );
    CWsfMainView* self = new ( ELeave ) CWsfMainView;
    CleanupStack::PushL( self );
    self->ConstructL( aIsEmbedded );
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfMainView::CWsfMainView
// ---------------------------------------------------------------------------
//
CWsfMainView::CWsfMainView()
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfMainView::~CWsfMainView
// ---------------------------------------------------------------------------
//
CWsfMainView::~CWsfMainView()
    {
    LOG_ENTERFN( "CWsfMainView::~CWsfMainView" );
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    delete iModel;
    }


// ---------------------------------------------------------------------------
// CWsfMainView::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfMainView::ConstructL( TBool aIsEmbedded )
    {
    LOG_ENTERFN( "CWsfMainView::ConstructL" );
    TInt actView( NULL );
    
    if ( aIsEmbedded ) // started from CP
        {
        actView = R_SNIFFER_CP_MAINVIEW;
        }
    else
        {
        actView = R_SNIFFER_MAINVIEW;
        }
                
    BaseConstructL( actView );
    iModel = CWsfMainViewModel::NewL();
    iController.SetModel( iModel );    
    }


// ---------------------------------------------------------------------------
// CWsfMainView::Id
// ---------------------------------------------------------------------------
//
TUid CWsfMainView::Id() const
    {
    return TUid::Uid( EMainViewId );  
    }


// ---------------------------------------------------------------------------
// CWsfMainView::HandleCommandL
// ---------------------------------------------------------------------------
//
void CWsfMainView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyOk:
            {
            break;
            }
        case EAknSoftkeyBack:
            {
            AppUi()->HandleCommandL( EAknCmdExit );
            break;
            }
        case EAknSoftkeyContextOptions:
            {
            MenuBar()->SetMenuTitleResourceId( R_SNIFFER_CONTEXT_MENUBAR );
            MenuBar()->SetMenuType( CEikMenuBar::EMenuContext ); 
            MenuBar()->TryDisplayMenuBarL();
            MenuBar()->SetMenuTitleResourceId( R_SNIFFER_MENUBAR );
            MenuBar()->SetMenuType( CEikMenuBar::EMenuOptions ); 
            break;
            }
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }    
    }


// ---------------------------------------------------------------------------
// CWsfMainView::DoActivateL
// ---------------------------------------------------------------------------
//
void CWsfMainView::DoActivateL( const TVwsViewId& /*aPrevViewId*/, 
                                TUid /*aCustomMessageId*/, 
                                const TDesC8& /*aCustomMessage*/)
    {
    LOG_ENTERFN( "CWsfMainView::DoActivateL" );
    if ( !iContainer )
        { 
        iContainer = CWsfMainViewContainer::NewL( ClientRect() );
        // set icons array to list        
        iContainer->SetMopParent( this );
        iContainer->SetPartner( iController );
        iContainer->ListBox()->SetListBoxObserver( this );
        AppUi()->AddToViewStackL( *this, iContainer );
        iController.SetContainer( iContainer );        
        }
   
    iContainer->MakeVisible( ETrue );     
    iController.RefreshL();
    MenuBar()->SetContextMenuTitleResourceId( R_SNIFFER_CONTEXT_MENUBAR );
    }


// ---------------------------------------------------------------------------
// CWsfMainView::DoDeactivate
// ---------------------------------------------------------------------------
//
void CWsfMainView::DoDeactivate()
    {
    LOG_ENTERFN( "CWsfMainView::DoDeactivate" );
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        iController.SetContainer( NULL );
        delete iContainer;
        iContainer = NULL;   
        }
    }


// ---------------------------------------------------------------------------
// CWsfMainView::Controller
// ---------------------------------------------------------------------------
//
MWsfMainViewControllerIf& CWsfMainView::Controller()
    {
    return iController;
    }
    
    
// ---------------------------------------------------------------------------
// CWsfMainView::DynInitMenuPaneL
// ---------------------------------------------------------------------------
//
void CWsfMainView::DynInitMenuPaneL( TInt aResourceId, 
                                     CEikMenuPane* aMenuPane )
    {
    iController.UpdateSelectedItemL();
    
    if ( aResourceId == R_SNIFFER_MENU )
        {
        LOG_ENTERFN( "CWsfMainView::DynInitMenuPaneL" );
        const CWsfDocument* doc = static_cast<const CWsfDocument*>( 
                                                         AppUi()->Document() );
        TBool blacklistEmpty( !doc->WlanInfoBranding().BlackListItemCount() );
        TWsfWlanInfo *selectedItem = iController.SelectedItem();
        CWsfWlanInfoArray *infoArray = iModel->GetInfoArray();
        
        CWsfAppUi* appui = static_cast<CWsfAppUi*>( AppUi() );
        TBool connecting( appui->SuppressingKeyEvents() );
        
        if ( !selectedItem )
            {
            // no real items available, but hidden item is present
            aMenuPane->SetItemDimmed( ESnifferCmdStartBrowsing, EFalse );
            aMenuPane->SetItemDimmed( ESnifferCmdContinueBrowsing, ETrue );
            aMenuPane->SetItemDimmed( ESnifferCmdConnect, EFalse );
            aMenuPane->SetItemDimmed( ESnifferCmdDisconnect, ETrue );
            aMenuPane->SetItemDimmed( ESnifferCmdRefresh, connecting ); 
            aMenuPane->SetItemDimmed( ESnifferCmdFilterWlans, 
                                  connecting || !infoArray || blacklistEmpty );
            aMenuPane->SetItemDimmed( ESnifferCmdDetails, ETrue );
            aMenuPane->SetItemDimmed( ESnifferCmdSettings, connecting );
            return;
            }

        TBool connectedAndUsed( StartWebBrowsingVisibleL() );
        TBool connected( selectedItem->Connected() );
        TBool known( selectedItem->Known() );
        
        TBool selectedItemConnecting ( 
                selectedItem->ConnectionStatus() == EConnecting );

        aMenuPane->SetItemDimmed( ESnifferCmdStartBrowsing, 
                                  connectedAndUsed || selectedItemConnecting );
        aMenuPane->SetItemDimmed( ESnifferCmdContinueBrowsing, 
                                  !connectedAndUsed || selectedItemConnecting );

        aMenuPane->SetItemDimmed( ESnifferCmdConnect, 
                                  connected || selectedItemConnecting );
        
        aMenuPane->SetItemDimmed( ESnifferCmdDisconnect, 
                                  !connected && !selectedItemConnecting); 
        
        aMenuPane->SetItemDimmed( ESnifferCmdRefresh, connecting ); 
      
        // dim filtering if no scan has been performed 
        // or there is nothing to filter
        aMenuPane->SetItemDimmed( ESnifferCmdFilterWlans, 
                                  connecting ||
                                  !infoArray ||
                                  ( blacklistEmpty && 
                                    ( !infoArray->Count() || 
                                      ( infoArray->Count() == 1 && 
                                        (*infoArray)[0]->Connected() ) ) ) );
        
        aMenuPane->SetItemDimmed( ESnifferCmdDetails, 
                                  connecting || 
                                  selectedItem->Hidden() && !known );
        aMenuPane->SetItemDimmed( ESnifferCmdSettings, connecting );
        }
    else if ( aResourceId == R_SNIFFER_CONTEXT_MENU )
        {
        TWsfWlanInfo *selectedItem = iController.SelectedItem();
        
        // there must be always at least one item: "Other (unlisted)..."
        TBool connectedAndUsed( StartWebBrowsingVisibleL() );
        TBool connected( EFalse );
        TBool known( EFalse );
        TBool hidden( ETrue );
        
        CWsfAppUi* appui = static_cast<CWsfAppUi*>( AppUi() );
        TBool connecting( appui->SuppressingKeyEvents() );
        TBool selectedItemConnecting( EFalse );
        
        if ( selectedItem )
            {
            // workaround: even if the model reports no elements, we know that
            // the hidden item must always be there
            connected = selectedItem->Connected();
            known = selectedItem->Known();
            hidden = selectedItem->Hidden();
            selectedItemConnecting = 
                    ( selectedItem->ConnectionStatus() == EConnecting );
            }

        aMenuPane->SetItemDimmed( ESnifferCmdStartBrowsing, 
                                  connectedAndUsed || selectedItemConnecting );
        aMenuPane->SetItemDimmed( ESnifferCmdContinueBrowsing, 
                                 !connectedAndUsed || selectedItemConnecting );

        aMenuPane->SetItemDimmed( ESnifferCmdConnect, 
                                  connected || selectedItemConnecting  );
        aMenuPane->SetItemDimmed( ESnifferCmdDisconnect, 
                                  !connected && !selectedItemConnecting );  
        aMenuPane->SetItemDimmed( ESnifferCmdDetails, 
                                  connecting || hidden && !known );
        }
    
    }


// ---------------------------------------------------------------------------
// CWsfMainView::HandleListBoxEventL
// ---------------------------------------------------------------------------
//
void CWsfMainView::HandleListBoxEventL( CEikListBox* /*aListBox*/, 
                                        TListBoxEvent aEventType )
    {
    switch ( aEventType )
        {
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked: 
            {
            HandleCommandL( EAknSoftkeyContextOptions );
            }
            break;
        default:
            break;
        }
    }    
    

// ---------------------------------------------------------------------------
// CWsfMainView::SelectionKeyPressed
// ---------------------------------------------------------------------------
//
void CWsfMainView::SelectionKeyPressed()
    {
    iSelectionKey = ETrue;
    }

    
// ---------------------------------------------------------------------------
// CWsfMainView::StartWebBrowsingVisibleL
// ---------------------------------------------------------------------------
//
TBool CWsfMainView::StartWebBrowsingVisibleL()
    {
    LOG_ENTERFN( "CWsfMainView::StartWebBrowsingVisibleL" );
    const TInt KBrowserUid = 0x10008D39;
    TUid id( TUid::Uid( KBrowserUid ) );
    
    // Check if the Browser application is already running.
    TApaTaskList taskList( iEikonEnv->WsSession() );
    TApaTask task = taskList.FindApp( id );
    TBool isBrowserRuning( EFalse );
    
#ifndef __WINS__

    if ( task.Exists() )
        {
        LOG_WRITE( "The Browser is running." );
        // The Browser is already running.
        // Check if the selected WLAN is connected.
        TWsfWlanInfo* selectedItem = iController.SelectedItem();
        TBool connected( EFalse );
        if( selectedItem )
        	{
        	connected = selectedItem->Connected();
        	}
        LOG_WRITEF("Selected network Connected? %d", connected);

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
                if( bearerType == EBearerWLAN )
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
            
            TUint i;
            for ( i = 0; i < count; i++ )
                {
                appInfo.iCaption.Zero();
                TInt result = appSess.GetAppInfo( appInfo, 
                                                  clientBuf().iUid[i] );
                
                LOG_WRITEF( "result: %d", result ); 
                LOG_WRITEF( "iClientInfo.iUid[i].iUid: %x", 
                             clientBuf().iUid[i].iUid );
                
                if ( clientBuf().iUid[i].iUid == KBrowserUid )
                    {
                    isBrowserRuning = ETrue;
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
    
    LOG_WRITEF( "isBrowserRuning: %d", isBrowserRuning ); 
    return isBrowserRuning;
    }



// End of file
