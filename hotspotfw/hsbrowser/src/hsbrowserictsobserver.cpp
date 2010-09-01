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
* Description:   Implements callback interface for Internet Connectivity Test Service
*                notifications.
*
*/



// INCLUDE FILES
#include <apgtask.h>
#include <eikenv.h>

#include "hsbrowserictsobserver.h"
#include "hsbrowsercontainer.h"
#include "hotspotclientserver.h"
#include "hsbrowsermodel.h"
#include "am_debug.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CHsBrowserIctsObserver::NewL
// ---------------------------------------------------------
//
CHsBrowserIctsObserver* CHsBrowserIctsObserver::NewL(
    CHsBrowserContainer* aContainer )
    {
    DEBUG( "CHsBrowserIctsObserver::CHsBrowserIctsObserver::NewL()()" );
    CHsBrowserIctsObserver* self = new(ELeave) CHsBrowserIctsObserver( aContainer );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CHsBrowserIctsObserver::~CHsBrowserIctsObserver
// ---------------------------------------------------------
//
CHsBrowserIctsObserver::~CHsBrowserIctsObserver()
    {
    DEBUG( "CHsBrowserIctsObserver::~CHsBrowserIctsObserver()" );
    }
    
// -----------------------------------------------------------------------------
// CHsBrowserIctsObserver::ConnectivityObserver
// -----------------------------------------------------------------------------
//  
void CHsBrowserIctsObserver::ConnectivityObserver( TIctsTestResult aResult,
    const TDesC& /* aString */ )
    {
    DEBUG1("CHsBrowserIctsObserver::ConnectivityObserver() result: %d", aResult);
    switch ( aResult )
        {
        case EConnectionOk :
            ProcessConnectionOk();
            break;
        case EHttpAuthenticationNeeded :
        case EConnectionNotOk :
        case ETimeout :
        default:
            break;
        }
    }

// ---------------------------------------------------------
// CHsBrowserIctsObserver::ProcessConnectionOk
// ---------------------------------------------------------
//
void CHsBrowserIctsObserver::ProcessConnectionOk()
	{
    DEBUG( "CHsBrowserIctsObserver::ProcessConnectionOk()" );
    if ( iContainer )
        {
        CHsBrowserModel* model = iContainer->Model();
        if ( model && !iAuthenticatedOkSent )
            {
            // Send this only once per session
            iAuthenticatedOkSent = ETrue;
            model->SetState( EHsBrowserUiAuthenticatedOk, ETrue );
            // Do this only once too 
            iContainer->ShowLoginCompleteNote();
            iContainer->SendToBackground();
            iContainer->UpdateSoftkeys();
            }
        }
    }

// ---------------------------------------------------------
// CHsBrowserIctsObserver::CHsBrowserIctsObserver
// ---------------------------------------------------------
//
CHsBrowserIctsObserver::CHsBrowserIctsObserver(
    CHsBrowserContainer* aContainer ) : iContainer( aContainer ),
    iAuthenticatedOkSent( EFalse )
  	{
  	DEBUG( "CHsBrowserIctsObserver::CHsBrowserIctsObserver()" );
  	}      

// ---------------------------------------------------------
// CHsBrowserIctsObserver::ConstructL
// ---------------------------------------------------------
//
void CHsBrowserIctsObserver::ConstructL()
    {
    DEBUG( "CHsBrowserIctsObserver::ConstructL()" );
    }
	
// End of File  
