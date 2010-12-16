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
* Description:   Handle load progress events
*
*/



// INCLUDE FILES
#include <e32cmn.h>
#include <eikenv.h>
#include <aknappui.h>
#include <akntitle.h>
#include <aknnavilabel.h>
#include <apgcli.h>
#include <ictsclientinterface.h>
#include "hotspotclientserver.h"

#include "hsbrowsercontainer.h"
#include "hsbrowserloadeventobserver.h"
#include "hsbrowserictsobserver.h"
#include "hsbrowsermodel.h"
#include "hsbrowsercommon.h"
#include "am_debug.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CHsBrowserLoadEventObserver::NewL
// ---------------------------------------------------------
//
CHsBrowserLoadEventObserver* 
CHsBrowserLoadEventObserver::NewL( CHsBrowserContainer* aContainer )
    {
    DEBUG( "CHsBrowserLoadEventObserver::NewL()" );
    CHsBrowserLoadEventObserver* self = new(ELeave)
	    CHsBrowserLoadEventObserver( aContainer );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CHsBrowserLoadEventObserver::~CHsBrowserLoadEventObserver
// ---------------------------------------------------------
//
CHsBrowserLoadEventObserver::~CHsBrowserLoadEventObserver()
    {
    DEBUG( "CHsBrowserLoadEventObserver::~CHsBrowserLoadEventObserver()" );
    if ( iIcts )
        {
        delete iIcts;	
        }
    
    }

// ---------------------------------------------------------
// CHsBrowserLoadEventObserver::HandleBrowserLoadEventL
// ---------------------------------------------------------
//
void CHsBrowserLoadEventObserver::HandleBrowserLoadEventL(
    TBrCtlDefs::TBrCtlLoadEvent aLoadEvent,
    TUint /* aSize */, 
    TUint16 /* aTransactionId */ ) 
    {
    DEBUG1( "CHsBrowserLoadEventObserver::HandleBrowserLoadEventL() event=%d", aLoadEvent );

    switch( aLoadEvent )
        {
        case TBrCtlDefs::EEventUrlLoadingStart:
        	{
        	if ( !iIsFirstLoad )
        		{
        		iContainer->UpdateNaviPane( CHsBrowserContainer::ELoadingUrl );
        		}
        	else
        		{
        		iIsFirstLoad = EFalse;
        		}
            // no break, fall through
        	}
		case TBrCtlDefs::EEventNewContentDisplayed:
        case TBrCtlDefs::EEventNewContentStart:
            {
            DEBUG( "CHsBrowserLoadEventObserver::HandleBrowserLoadEventL() EEventNewContentStart" );            
            if ( iDoIct )
            	{
            	DEBUG( "CHsBrowserLoadEventObserver::HandleBrowserLoadEventL() Start testing internet connectivity.");
            	iDoIct = EFalse;
				TRAPD( err, TestInternetConnectivityL() );
				if ( err != KErrNone )
					{
					DEBUG1( "CHsBrowserLoadEventObserver::HandleBrowserLoadEventL() err=%d.", err );
					}
           	    }
            break;
            }
        case TBrCtlDefs::EEventLoadFinished://EEventUrlLoadingFinished:
            {
            DEBUG( "CHsBrowserLoadEventObserver::HandleBrowserLoadEventL() EEventUrlLoadingFinished" );
            // We've finished loading one element of page
            iContainer->UpdateNaviPane( CHsBrowserContainer::ELoadingFinished );
            break;
            }
        case TBrCtlDefs::EEventTitleAvailable:
            {
            DEBUG( "CHsBrowserLoadEventObserver::HandleBrowserLoadEventL() EEventTitleAvailable" );
            UpdateNameL();
            break;
            }
        default:
        	{
            break;
        	}
        }   // end of switch
        
    UpdateDoIctFlagL();
    
    if ( iContainer )
    	{
        iContainer->DrawNow();
        iContainer->MakeVisible( ETrue );    	
    	}   
    }
    
// ---------------------------------------------------------
// CHsBrowserLoadEventObserver::CHsBrowserLoadEventObserver
// ---------------------------------------------------------
//
CHsBrowserLoadEventObserver::CHsBrowserLoadEventObserver(
    CHsBrowserContainer* aContainer ) : 
    iContainer( aContainer ),
    iIcts( NULL ),
    iDoIct( EFalse ),
    iIsFirstLoad( ETrue )
    {
    DEBUG( "CHsBrowserLoadEventObserver::CHsBrowserLoadEventObserver()" );
    }

// ---------------------------------------------------------
// CHsBrowserLoadEventObserver::ConstructL
// ---------------------------------------------------------
//
void 
CHsBrowserLoadEventObserver::ConstructL()
    {
    DEBUG( "CHsBrowserLoadEventObserver::ConstructL()" );
    }

// ----------------------------------------------------
// CHsBrowserLoadEventObserver::UpdateNaviLabel()
// ----------------------------------------------------
//
void CHsBrowserLoadEventObserver::UpdateNaviLabel( const TDesC& aName )
    {
    DEBUG( "CHsBrowserLoadEventObserver::UpdateNaviLabel()" );
    if ( iContainer )
        {
        TRAPD( err, iContainer->NaviLabel()->SetTextL( aName ) );
        if ( err == KErrNone )
        	{
        	iContainer->NaviLabel()->DrawDeferred( );
        	}
        else
          	{
            DEBUG1( "CHsBrowserLoadEventObserver::UpdateNaviLabel() err=%d", err );
            }
        }
    }

// ----------------------------------------------------
// CHsBrowserLoadEventObserver::UpdateFastSwapNameL()
// ----------------------------------------------------
//
void CHsBrowserLoadEventObserver::UpdateFastSwapNameL( const TDesC& aName )
	{
    DEBUG( "CHsBrowserLoadEventObserver::UpdateFastSwapNameL()" );

    RApaLsSession appArcSession;
    TInt err = KErrNone;
    TApaAppInfo info;
    
    err = appArcSession.Connect(); // connect to AppArc server
    if( KErrNone == err )
    	{
    	CleanupClosePushL( appArcSession );
    	appArcSession.GetAppInfo( info, KUidHsBrowserApp );

    	if ( aName.Length() > KApaMaxAppCaption )
    		{
    		TPtrC shortName = aName.Left( KApaMaxAppCaption );
    		err = appArcSession.SetAppShortCaption( shortName, ELangNone, KUidHsBrowserApp );
    		}
    	else
    		{
    		err = appArcSession.SetAppShortCaption( aName, ELangNone, KUidHsBrowserApp );
    		}
    	if ( err != KErrNone )
    		{
    		DEBUG1( "CHsBrowserLoadEventObserver::UpdateFastSwapNameL() err=%d", err );
    		}
    	CleanupStack::PopAndDestroy( &appArcSession );	
    	}
	}

// ----------------------------------------------------
// CHsBrowserLoadEventObserver::UpdateNameL()
// ----------------------------------------------------
//
void CHsBrowserLoadEventObserver::UpdateNameL()
    {
    DEBUG( "CHsBrowserLoadEventObserver::UpdateNameL()" );
    CBrCtlInterface* brCtlInterface = NULL;
    if ( iContainer )
    	{
    	brCtlInterface = iContainer->BrCtlInterface();
    	}
    if ( !brCtlInterface )
    	{
    	return;
    	}    
    HBufC* title = brCtlInterface->PageInfoLC( TBrCtlDefs::EPageInfoTitle );
    if ( title )
    	{
    	UpdateFastSwapNameL( *title );
   	    CHsBrowserModel* model = iContainer->Model();
        if ( model )
        	{
        	model->SetPageTitle( *title );
            iContainer->UpdateNaviPane( CHsBrowserContainer::EPageTitleAvailable );
            }
        }
    CleanupStack::PopAndDestroy( title );
    }

// -----------------------------------------------------------------------------
// CHsBrowserLoadEventObserver::TestInternetConnectivityL
// -----------------------------------------------------------------------------
//
void CHsBrowserLoadEventObserver::TestInternetConnectivityL()
    {
	DEBUG( "CHsBrowserLoadEventObserver::TestInternetConnectivity() 7" );
	
	if ( !iContainer )
		{
	    DEBUG( "CHsBrowserLoadEventObserver::TestInternetConnectivity() iContainer=NULL" );
	    return;	
		}
    CHsBrowserModel* model = iContainer->Model();
    if ( !model )
   	    {
   	    DEBUG( "CHsBrowserLoadEventObserver::TestInternetConnectivity() model=NULL" );
   	    return;
   	    }
    CHsBrowserIctsObserver* observer = iContainer->HsBrowserIctsObserver();
    if ( !observer )
        {
        DEBUG( "CHsBrowserLoadEventObserver::TestInternetConnectivity() observer=NULL" );
        return;
        }
    DEBUG1( "CHsBrowserLoadEventObserver::TestInternetConnectivity() iap=%d", model->IapId() );
    DEBUG1( "CHsBrowserLoadEventObserver::TestInternetConnectivity() netid=%d", model->NetId() );
    if ( !iIcts )
        {
        iIcts = CIctsClientInterface::NewL( model->IapId(), model->NetId(), *observer );
        }
    else
        {
        // stop previously started polling
        iIcts->StopPolling();
        }
    // Start polling
    DEBUG( "CHsBrowserLoadEventObserver::TestInternetConnectivity() starting ICT" );
    iIcts->StartPolling( KTotalPollingTime, KExecutionTimeInterval );
    }

// -----------------------------------------------------------------------------
// CHsBrowserLoadEventObserver::UpdateDoIctFlagL
// -----------------------------------------------------------------------------
//
void CHsBrowserLoadEventObserver::UpdateDoIctFlagL()
	{
	DEBUG( "CHsBrowserLoadEventObserver::UpdateDoIctFlagL()" );
    CBrCtlInterface* brCtlInterface = NULL;
    CHsBrowserModel* model = NULL;
    if ( iContainer )
    	{
    	brCtlInterface = iContainer->BrCtlInterface();
    	model = iContainer->Model();
    	}
    if ( brCtlInterface && model )
    	{
        // get url
        HBufC* url = brCtlInterface->PageInfoLC( TBrCtlDefs::EPageInfoUrl );
        if ( url )
        	{
            DEBUG1( "url=%S", url );        
            iDoIct = ETrue;
            DEBUG( "CHsBrowserLoadEventObserver::HandleBrowserLoadEventL() iDoIct = ETrue." );
    	    }	
        else
        	{
        	DEBUG( "CHsBrowserLoadEventObserver::UpdateDoIctFlagL() url=NULL" );
        	}
        CleanupStack::PopAndDestroy( url );
    	}
	}
