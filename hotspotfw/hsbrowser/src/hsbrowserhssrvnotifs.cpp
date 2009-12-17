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
* Description:   Implements notifications interface from Hotspot server in order
*                to receive wlan connection closed notification.
*
*/



// INCLUDE FILES
#include <apgtask.h>
#include <eikenv.h>

#include "hsbrowserhssrvnotifs.h"
#include "hsbrowsercontainer.h"
#include "am_debug.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CHsBrowserHsSrvNotifs::NewL
// ---------------------------------------------------------
//
CHsBrowserHsSrvNotifs* CHsBrowserHsSrvNotifs::NewL(
    CHsBrowserContainer* aContainer )
    {
    DEBUG( "CHsBrowserHsSrvNotifs::CHsBrowserHsSrvNotifs::NewL()()" );
    CHsBrowserHsSrvNotifs* self = new(ELeave) CHsBrowserHsSrvNotifs( aContainer );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CHsBrowserHsSrvNotifs::CHsBrowserHsSrvNotifs
// ---------------------------------------------------------
//
CHsBrowserHsSrvNotifs::~CHsBrowserHsSrvNotifs()
    {
    DEBUG( "CHsBrowserHsSrvNotifs::~CHsBrowserHsSrvNotifs()" );
    }
    
// -----------------------------------------------------------------------------
// CHsBrowserHsSrvNotifs::ConnectionStateChanged
// -----------------------------------------------------------------------------
//  

void CHsBrowserHsSrvNotifs::ConnectionStateChanged( TWlanConnectionMode aNewState )
	{
	    DEBUG1( "CHsBrowserHsSrvNotifs::ConnectionStateChanged() aNewState=%d", aNewState );
	    if ( aNewState == EWlanConnectionModeNotConnected )
	    	{
	    	iContainer->ProcessWlanConnModeNotConnected();
	    	}
	}

// ---------------------------------------------------------
// CHsBrowserHsSrvNotifs::CHsBrowserHsSrvNotifs
// ---------------------------------------------------------
//
CHsBrowserHsSrvNotifs::CHsBrowserHsSrvNotifs(
    CHsBrowserContainer* aContainer ) : iContainer( aContainer )
  	{
  	DEBUG( "CHsBrowserHsSrvNotifs::CHsBrowserHsSrvNotifs()" );
  	}      

// ---------------------------------------------------------
// CHsBrowserHsSrvNotifs::ConstructL
// ---------------------------------------------------------
//
void CHsBrowserHsSrvNotifs::ConstructL()
    {
    DEBUG( "CHsBrowserHsSrvNotifs::ConstructL()" );
    }
	
// End of File  
