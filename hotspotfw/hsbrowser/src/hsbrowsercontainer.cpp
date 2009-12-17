/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Implementation of the container control for HotSpot Browser Application.
*
*/

// INCLUDE FILES
#include <eiklabel.h>
#include <avkon.hrh>
#include <hsbrowser.rsg>
#include <brctlinterface.h>
#include <charconv.h>
#include <gdi.h>
#include <bitdev.h>
#include <eikenv.h> 
#include <AknDef.h>
#include <AknUtils.h>
#include <apgtask.h>
#include <es_enum.h>
#include <coecobs.h>
#include <coecntrl.h>
#include <apgwgnam.h>
#include <aknnavi.h> 
#include <AknIndicatorContainer.h>
#include <aknnavilabel.h> 
#include <aknnavide.h>
#include <aknappui.h>
#include <aknnotewrappers.h> 
#include <StringLoader.h>
#include <bautils.h>
#include <data_caging_path_literals.hrh>
#include <AknGlobalNote.h>
#include <hlplch.h>

#include "hotspotclientserver.h"
#include "hsbrowsercontainer.h"
#include "hsbrowserloadeventobserver.h"
#include "hsbrowserloadeventobserver.h"
#include "hsbrowserspecialloadobserver.h"
#include "hsbrowserictsobserver.h"
#include "hsbrowserhssrvnotifs.h"
#include "hsbrowsermodel.h"
#include "hsbrowsercommon.h"
#include "am_debug.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CHsBrowserContainer::CHsBrowserContainer()
// Constructor.
// ---------------------------------------------------------
//
CHsBrowserContainer::CHsBrowserContainer( CHsBrowserModel* aModel ) :
    iModel( aModel ),
    iNaviPane( NULL ),
    iEditorIndicatorContainer( NULL),
    iIndiContainer( NULL ),
    iResource( 0 ),
    iActiveInputBox( EFalse )
	{
	DEBUG( "CHsBrowserContainer::CHsBrowserContainer()" );
	}


// ---------------------------------------------------------
// CHsBrowserContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CHsBrowserContainer::ConstructL( const TRect& aRect )
    {
    DEBUG( "CHsBrowserContainer::ConstructL()" );
    CreateWindowL();
        
    // create observers
    iHsBrowserSpecialLoadObserver = CHsBrowserSpecialLoadObserver::NewL();
    iHsBrowserLoadEventObserver = CHsBrowserLoadEventObserver::NewL( this );
    iHsBrowserIctsObserver = CHsBrowserIctsObserver::NewL( this );
    iHsBrowserHsSrvNotifs = CHsBrowserHsSrvNotifs::NewL( this );
    
    iCommandBase = TBrCtlDefs::ECommandIdBase;
    TUint brCtlCapabilities = TBrCtlDefs::ECapabilityLoadHttpFw |
                              TBrCtlDefs::ECapabilityDisplayScrollBar |
                              TBrCtlDefs::ECapabilityCursorNavigation;
    iBrCtlInterface = CreateBrowserControlL(
        this, 
        aRect, 
        brCtlCapabilities,
        iCommandBase, 
        NULL,  // softkey observer
        NULL,  // link resolver
        iHsBrowserSpecialLoadObserver,
        NULL,  // layout observer
        NULL,  // dialog provider
        NULL,  // window observer
        NULL );// download observer 
      
    // disable ESettingsSecurityWarnings setting,
    // or implement MBrCtlDialogsProvider interface
    // reason: default implementation of MBrCtlDialogsProvider::DialogConfirmL
    // returns EFalse and page is not loaded.
    iBrCtlInterface->SetBrowserSettingL(
        TBrCtlDefs::ESettingsSecurityWarnings, EFalse );
   
    // auto load on
    iBrCtlInterface->SetBrowserSettingL ( 
            TBrCtlDefs::ESettingsAutoLoadImages, 
            ETrue);

    iBrCtlInterface->SetBrowserSettingL ( 
            TBrCtlDefs::ESettingsCookiesEnabled, 
            ETrue);
    
    iBrCtlInterface->SetBrowserSettingL ( 
            TBrCtlDefs::ESettingsCSSFetchEnabled, 
            ETrue);
     
     iBrCtlInterface->SetBrowserSettingL ( 
            TBrCtlDefs::ESettingsECMAScriptEnabled, 
            ETrue);

    // this observer can be added and removed dynamically
    iBrCtlInterface->AddLoadEventObserverL( iHsBrowserLoadEventObserver );     
    
    // init navi pane indicators
    InitNaviPaneL();
            
    // load resource file
    TFileName fileName;           
    fileName.Append( KDriveZ );
    fileName.Append( KDC_APP_RESOURCE_DIR );   
    fileName.Append( KResourceFile );
    BaflUtils::NearestLanguageFile( CCoeEnv::Static()->FsSession(),
                                    fileName );
    iResource = CCoeEnv::Static()->AddResourceFileL( fileName );
    
    HBufC* title = StringLoader::LoadL( R_QTN_NETW_CONSET_WBA_STATIC_TITLE );

    iModel->SetStaticTitle( *title );
    delete title;
    
    // activate WLAN MGMT API notifications,
    // we need to know when network is lost
    iMgtClient = CWlanMgmtClient::NewL();
    iMgtClient->ActivateNotificationsL( *iHsBrowserHsSrvNotifs );
    
    SetRect( aRect ); // set the size
    ActivateL(); // activate the window
    SetFocus( ETrue );
    }

// ---------------------------------------------------------
// CHsBrowserContainer::~CHsBrowserContainer()
// Destructor.
// ---------------------------------------------------------
//
CHsBrowserContainer::~CHsBrowserContainer()
    {
    DEBUG( "CHsBrowserContainer::~CHsBrowserContainer()" );
    if ( iMgtClient )
        {
        iMgtClient->CancelNotifications();
        delete iMgtClient;
        }
    
    iBrCtlInterface->RemoveLoadEventObserver( iHsBrowserLoadEventObserver );
    delete iBrCtlInterface;
    
    // delete observers
    delete iHsBrowserSpecialLoadObserver;
    delete iHsBrowserLoadEventObserver;
    delete iHsBrowserIctsObserver;
    
    // cancels also notifications
    delete iHsBrowserHsSrvNotifs;
    
    if ( iResource )
        {
        CCoeEnv::Static()->DeleteResourceFile( iResource );        
        }
    
    delete iEditorIndicatorContainer;      
    DEBUG( "CHsBrowserContainer::~CHsBrowserContainer() DONE" );
    }


// ---------------------------------------------------------
// CHsBrowserContainer::InitNaviPaneL()
// Initializes the indicators in the navi pane.
// ---------------------------------------------------------
//
void CHsBrowserContainer::InitNaviPaneL()
	{
    DEBUG( "CHsBrowserContainer::InitNaviPaneL()" );
    if ( !iEditorIndicatorContainer )
        {
        CEikStatusPane* statusPane = iAvkonAppUi->StatusPane();        
        if ( statusPane )
            {
            iNaviPane = (CAknNavigationControlContainer*)
                statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) );
            if ( iNaviPane )
            	{
            	iEditorIndicatorContainer =
            	    iNaviPane->CreateEditorIndicatorContainerL();
            	}
            }
        }

   	if ( !iIndiContainer && iEditorIndicatorContainer )
        {
        iIndiContainer = (CAknIndicatorContainer*) iEditorIndicatorContainer->DecoratedControl();
    	}
    
    if ( iNaviPane && iEditorIndicatorContainer )
    	{
    	iNaviPane->PushL( *iEditorIndicatorContainer );
    	}
        	
	}

// ---------------------------------------------------------
// CHsBrowserContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CHsBrowserContainer::SizeChanged()
    {
    DEBUG( "CHsBrowserContainer::SizeChanged()" );
    iBrCtlInterface->SetRect(Rect());
    }

// ---------------------------------------------------------
// CHsBrowserContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CHsBrowserContainer::CountComponentControls() const
    {
    return KComponentsNumber;
    }

// ---------------------------------------------------------
// CHsBrowserContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CHsBrowserContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case KBrCtlComponentIndex:
            return iBrCtlInterface;
        default:
            return NULL;
        }
    }


// ------------------------------------------------------------------
// CHsBrowserContainer::UpdateNaviPane( TNaviPaneEvent aUpdateEvent )
// ------------------------------------------------------------------
//
void CHsBrowserContainer::UpdateNaviPane( TNaviPaneEvent aUpdateEvent )
	{
	DEBUG( "CHsBrowserContainer::UpdateNaviPane()" );
	
    iIndiContainer->SetIndicatorState(
        TUid::Uid( EAknNaviPaneEditorIndicatorMessageInfo ),
        EAknIndicatorStateOn );
                
	switch( aUpdateEvent )
        {
        case ELoadingLoginPage:
            {
            TRAP_IGNORE( iIndiContainer->SetIndicatorValueL(
                TUid::Uid( EAknNaviPaneEditorIndicatorMessageInfo ),
                *iModel->StaticTitle() ) );
            iIndiContainer->SetIndicatorState(
                TUid::Uid( EAknNaviPaneEditorIndicatorWaitBar ),  
                EAknIndicatorStateAnimate );
            break;
            }
        case ELoadingUrl:
            {
            iIndiContainer->SetIndicatorState(
                TUid::Uid( EAknNaviPaneEditorIndicatorWaitBar ),  
                EAknIndicatorStateAnimate );
            break;
            }
        case EPageTitleAvailable:
            {
            TRAP_IGNORE( iIndiContainer->SetIndicatorValueL(
                TUid::Uid( EAknNaviPaneEditorIndicatorMessageInfo ),
                *iModel->PageTitle() ) );
            break;
            }
        case ELoadingFinished:
            {
            iIndiContainer->SetIndicatorState(
                TUid::Uid( EAknNaviPaneEditorIndicatorWaitBar ),  
                EAknIndicatorStateOff );
            break;
            }
        }
        
        iIndiContainer->DrawNow();
	}

// ----------------------------------------------------
// CHsBrowserContainer::HandleCommandL(TInt aCommand)
// ----------------------------------------------------
//
void CHsBrowserContainer::HandleCommandL(TInt aCommand)
    {
    DEBUG1( "CHsBrowserContainer::HandleCommandL() aCommand=%d", aCommand );
    switch ( aCommand )
        {
        case EAknSoftkeyNext:
            ShowLoginCompleteNote();
            SendToBackground();
            break;
        case EAknSoftkeyCancel:
        case EAknSoftkeyBack:
        case EAknSoftkeyExit:
        case EEikCmdExit:
            {
            iBrCtlInterface->HandleCommandL( 
                    TBrCtlDefs::ECommandCancelFetch + 
                    TBrCtlDefs::ECommandIdBase );
            iModel->SetState( EHsBrowserUiClosed, ETrue );
            break;
            }
        case EAknCmdHelp:
            {
            HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(),
                    iEikonEnv->EikAppUi()->AppHelpContextL());
            break;
            }
        default:
            if ( aCommand >= iCommandBase &&
                    aCommand < iCommandBase + TBrCtlDefs::ECommandIdRange )
                {
                iBrCtlInterface->HandleCommandL( aCommand );
                }              
            break;      
        }    
    }

// ----------------------------------------------------
// CHsBrowserContainer::HandleKeyEventL(
//     const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
// ----------------------------------------------------
//
TKeyResponse CHsBrowserContainer::HandleKeyEventL(
    const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
    {
    return EKeyWasNotConsumed;
    }

// ----------------------------------------------------
// CHsBrowserContainer::SendToForeground()
// ----------------------------------------------------
//
void CHsBrowserContainer::SendToForeground()
	{
	DEBUG( "CHsBrowserContainer::SendToForeground()" );
	    
    // because "Connecting via..." note hides hsbrowser,
    // send hsbrowser to foreground
    CCoeEnv::Static()->RootWin().SetOrdinalPosition( 0,
    		ECoeWinPriorityNormal );
    CCoeEnv::Static()->WsSession().Flush();  
	}

// ----------------------------------------------------
// CHsBrowserContainer::RestorePositionAndPriority()
// ----------------------------------------------------
//
void CHsBrowserContainer::RestorePositionAndPriority()
	{
	DEBUG( "CHsBrowserContainer::RestorePositionAndPriority()" );

    RWindowGroup& wg = CEikonEnv::Static()->RootWin();
	
    // because "Connecting via..." note hides hsbrowser,
    // it was assigned ECoeWinPriorityAlwaysAtFront priority,
    // restore original now
    if ( wg.OrdinalPriority() == ECoeWinPriorityAlwaysAtFront )
    	{	
    	CCoeEnv::Static()->RootWin().SetOrdinalPosition(
    	        iModel->Position(), iModel->Priority() ); 	    	
	    }
	}

// ---------------------------------------------------------
// CHsBrowserContainer::SendToBackground
// ---------------------------------------------------------
//
void CHsBrowserContainer::SendToBackground()
	{
	DEBUG( "CHsBrowserContainer::SendToBackground()" );

    RWindowGroup& wg = CEikonEnv::Static()->RootWin();
    
    // Construct en empty TApaTask object
    // giving it a reference to the Window Server session    
    TApaTask task( CEikonEnv::Static()->WsSession() );
                
    // Initialise the object with the window group id of
    // our application (so that it represent our app)
    task.SetWgId( wg.Identifier() );

    // Request window server to bring our application
    // to foreground
    DEBUG("CHsBrowserContainer::SendToBackground() sending to background" );
    task.SendToBackground();	  
	}

// ----------------------------------------------------
// CHsBrowserContainer::ProcessForegroundEvent()
// ----------------------------------------------------
//
void CHsBrowserContainer::ProcessForegroundEvent( TBool aForeground )
	{
	DEBUG1( "CHsBrowserContainer::ProcessForegroundEvent() aForeground = %d", aForeground );
	iModel->SetForeground( aForeground );
	UpdateSoftkeys();
	}

// ----------------------------------------------------
// CHsBrowserContainer::OfferKeyEventL(const 
//               TKeyEvent& aKeyEvent,TEventCode aType)
// ----------------------------------------------------
//
TKeyResponse 
CHsBrowserContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    DEBUG( "CHsBrowserContainer::OfferKeyEventL()" );
  
    TUint state = iModel->State();
    TBrCtlDefs::TBrCtlElementType elem = iBrCtlInterface->FocusedElementType();
    DEBUG1( "CHsBrowserContainer::OfferKeyEventL() type: %d", elem);

    if ( elem == TBrCtlDefs::EElementActivatedInputBox || elem == TBrCtlDefs::EElementInputBox )
    {
        if ( !iActiveInputBox && state != EHsBrowserUiAuthenticatedOk )
        {
            // change softkey
            CEikButtonGroupContainer* cba = CEikonEnv::Static()->
            AppUiFactory()->Cba();
            if ( cba )
            {
                cba->SetCommandSetL( R_HSBROWSER_SOFTKEYS_CANCEL );
                cba->DrawNow();
            }
        }
        iActiveInputBox = EFalse;
    }
    else 
    {
        CEikButtonGroupContainer* cba = CEikonEnv::Static()->
        AppUiFactory()->Cba();
        if ( cba && state != EHsBrowserUiAuthenticatedOk )
        {
            cba->SetCommandSetL( R_AVKON_SOFTKEYS_CANCEL );
            cba->DrawNow();
        }
    }
    DEBUG( "CHsBrowserContainer::OfferKeyEventL() done to iBrCtl" );
    return iBrCtlInterface->OfferKeyEventL(aKeyEvent, aType);
    }

// ----------------------------------------------------
// CHsBrowserContainer::HandleResourceChange()
// ----------------------------------------------------
//
void CHsBrowserContainer::HandleResourceChange( TInt aType )
    {
    DEBUG( "CHsBrowserContainer::HandleResourceChange" );
    CCoeControl::HandleResourceChange( aType );
 
    // Adjust Browser size to screen when screen layout is changed
    // Vertical vs. horizontal
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        DEBUG( "CHsBrowserContainer::HandleResourceChange = KEikDynamicLayoutVariantSwitch" );
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, 
                                           mainPaneRect );
        SetRect( mainPaneRect );
        DrawDeferred();
        }
    }


// ----------------------------------------------------
// CHsBrowserContainer::UpdateSoftkeys()
// ----------------------------------------------------
//
void CHsBrowserContainer::UpdateSoftkeys()
	{
	DEBUG( "CHsBrowserContainer::UpdateSoftkeys()" );
	
	TBool foreground = iModel->Foreground();
	TUint state = iModel->State();

	if ( !foreground && state == EHsBrowserUiAuthenticatedOk )
	    {
	    // R_HSBROWSER_SOFTKEYS_CONTINUE_EXIT
	    CEikButtonGroupContainer* cba = CEikonEnv::Static()->AppUiFactory()->Cba();
	    if ( cba )
	        {
	        TRAP_IGNORE( cba->SetCommandSetL( R_HSBROWSER_SOFTKEYS_CONTINUE_EXIT ) );
	        cba->DrawNow();
	        }
	    }        
	}

// ----------------------------------------------------
// CHsBrowserContainer::LoadRedirect
// ----------------------------------------------------
//
TInt CHsBrowserContainer::LoadRedirect( const TPtrC& aUrl,
    TInt aIapId, TInt aNetId )
    {
    DEBUG( "CHsBrowserContainer::LoadRedirect()" );
    
    TInt err = KErrNone;
    
    UpdateNaviPane( ELoadingLoginPage );
    
    // init model
    iModel->SetIapId( aIapId );
    iModel->SetNetId( aNetId );
    iModel->SetUrl( aUrl ); 	
    // attach
    err = iModel->Attach();
    if ( err != KErrNone )
        {
        DEBUG1( "CHsBrowserContainer::LoadRedirect() Attach() err=%d", err );	
        }        
    else    
        {
        TInt connPtr = REINTERPRET_CAST( TInt, &iModel->Connection() );
        if ( iHsBrowserSpecialLoadObserver )
            {
            iHsBrowserSpecialLoadObserver->SetConnectionPtr( connPtr );
            iHsBrowserSpecialLoadObserver->SetSockSvrHandle(
                    iModel->SocketServ().Handle() );            	
            }
        }

    if ( err == KErrNone )
    	{
    	TRAP( err, iBrCtlInterface->LoadUrlL( aUrl ) );
    	DEBUG1( "CHsBrowserContainer::LoadRedirect() iBrCtlInterface->\
            LoadUrlL() err=%d", err );
    	}
    return err;
    }
    
// ---------------------------------------------------------
// CHsBrowserContainer::ProcessWlanConnModeNotConnected
// ---------------------------------------------------------
//
void CHsBrowserContainer::ProcessWlanConnModeNotConnected()
	{
	DEBUG( "CHsBrowserContainer::ProcessWlanConnModeNotConnected()" );
	
	// Workaround for "WLAN login application closed" error:
	TRAP_IGNORE( iBrCtlInterface->HandleCommandL( 
            TBrCtlDefs::ECommandCancelFetch + 
            TBrCtlDefs::ECommandIdBase ));

	ShowConnClosedNote();
	iAvkonAppUi->Exit();
	}
    
// ---------------------------------------------------------
// CHsBrowserContainer::ShowLoginCompleteNote
// ---------------------------------------------------------
//
void CHsBrowserContainer::ShowLoginCompleteNote()
    {
    TBool noteShown = iModel->LoginCompleteNoteShown();
    DEBUG1( "CHsBrowserContainer::ShowLoginCompleteNote() noteShown=%d", noteShown );

    TInt pos = CCoeEnv::Static()->RootWin().OrdinalPosition();
    DEBUG1( "CHsBrowserContainer::ShowLoginCompleteNote() pos=%d", pos );

    if ( !noteShown && pos == 0 )		
        {
        DEBUG( "CHsBrowserContainer::ShowLoginCompleteNote() showing note" );

        HBufC* message = NULL;
        TRAPD( err, message = StringLoader::LoadLC( R_QTN_NETW_CONSET_WBA_INFO_COMPLETE ); CleanupStack::Pop( message ) ); 
        if (err == KErrNone)
            {
            // Global note needed here, because CAknInformationNote was sometimes causing chrashes
            CAknGlobalNote* note = NULL;
            TRAP( err, note = CAknGlobalNote::NewLC(); CleanupStack::Pop( note ) );
            if (err == KErrNone)
                {            
                TRAP( err, note->ShowNoteL( EAknGlobalInformationNote, *message ) );
                if (err == KErrNone)
                    {
                    DEBUG( "CHsBrowserContainer::ShowLoginCompleteNote(): Showing note DONE" );
                    }
                delete note;
                }
            delete message;
            }
        iModel->SetLoginCompleteNoteShown( ETrue );
        }
    }

// ----------------------------------------------------
// CHsBrowserContainer::ShowConnClosedNote
// ----------------------------------------------------
//
void CHsBrowserContainer::ShowConnClosedNote()
	{
	DEBUG( "CHsBrowserContainer::ShowConnClosedNote()" );
    if ( IsForeground() )
        {
        DEBUG( "CHsBrowserContainer::ShowConnClosedNote() Foreground" );
	    HBufC* message = NULL;
        TRAPD( err, message = StringLoader::LoadLC( R_QTN_NETW_CONSET_WBA_INFO_CLOSED ); CleanupStack::Pop( message ) );
 	    if (err == KErrNone)
 	        {
 	        // use global note here, otherwise overlap problem
 	        // appears with "Wlan network lost" note, which is
 	        // also a global note.
 	        CAknGlobalNote* note = NULL;
 	        TRAP( err, note = CAknGlobalNote::NewLC(); CleanupStack::Pop( note ) );
 	        if (err == KErrNone)
 	            { 	        
 	            TRAP( err, note->ShowNoteL( EAknGlobalInformationNote, *message ) );
 	            if (err == KErrNone)
 	                {
 	                DEBUG( "CHsBrowserContainer::ShowConnClosedNote(): Showing note DONE" );
 	                }
 	            delete note;
 	            }
 	        delete message;
 	        }
		}
	}

// ----------------------------------------------------
// CHsBrowserContainer::IsForeground()
// ----------------------------------------------------
//
TBool CHsBrowserContainer::IsForeground()
	{
	DEBUG( "CHsBrowserContainer::IsForeground()" );
	TInt32 foregroundUid(0);
    RWsSession ws;
    TInt ret = ws.Connect();
    if ( KErrNone == ret )
    	{
    	TApaTaskList taskList = TApaTaskList( ws );
    	TApaTask foregroundTask = taskList.FindByPos( 0 );
    
    	CApaWindowGroupName* wgName = NULL;
    	TRAPD( err, wgName = CApaWindowGroupName::NewLC( ws, foregroundTask.WgId() ); CleanupStack::Pop( wgName ) );
    	if (err == KErrNone)
    	    {
            foregroundUid = wgName->AppUid().iUid;
            delete wgName;
    	    }
        ws.Close();
    	}
    return foregroundUid == KUidHsBrowserApp.iUid;
	}

// End of File
