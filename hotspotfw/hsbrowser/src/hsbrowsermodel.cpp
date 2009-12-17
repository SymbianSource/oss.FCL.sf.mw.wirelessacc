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
* Description:   Implements the model for application.
*
*/



// INCLUDE FILES
#include <es_enum.h>
#include <coemain.h>

#include "hotspotclientserver.h"
#include "hsbrowsermodel.h"
#include "am_debug.h"

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// CHsBrowserModel::NewL
// ----------------------------------------------------
//
CHsBrowserModel* CHsBrowserModel::NewL()
	{
	DEBUG( "CHsBrowserModel::NewL()" );
    CHsBrowserModel* self = new(ELeave) CHsBrowserModel();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
	}

// ----------------------------------------------------
// CHsBrowserModel::~CHsBrowserModel
// ----------------------------------------------------
//
CHsBrowserModel::~CHsBrowserModel()
	{
	DEBUG( "CHsBrowserModel::~CHsBrowserModel()" );
	if ( iUrl )
		{
		delete iUrl;
		}
    if ( iState != EHsBrowserUiClosed )
        {
        iHsServer.SetUiState( iIapId, EHsBrowserUiClosed );
        }

	iHsServer.Close();
    // Removed iConnection and iSocketServ Close() calls.
    // It seems like Browser Control is freeing them as it's using them also.

	if ( iStaticTitle )
		{
		delete iStaticTitle;
		}
    if ( iPageTitle )
    	{
    	delete iPageTitle;
    	}
    DEBUG( "CHsBrowserModel::~CHsBrowserModel() DONE" );
	}

// ----------------------------------------------------
// CHsBrowserModel::SetState
// ----------------------------------------------------
//
void CHsBrowserModel::SetState( TUint state, TBool sendToHss )
	{
	DEBUG( "CHsBrowserModel::SetState()" );
    iState = state;
    if ( sendToHss )
    	{
    	iHsServer.SetUiState( iIapId, iState );
    	}
	}

// ----------------------------------------------------
// CHsBrowserModel::Attach
// ----------------------------------------------------
//
TInt CHsBrowserModel::Attach()
	{
	DEBUG( "CHsBrowserModel::Attach() 1" );
    TInt err = KErrNone;
    TConnectionInfoBuf currentConnInfo;
	// Open channel to Socket Server
    err = iSocketServ.Connect();
    if ( err != KErrNone)
    	{
    	return err;
    	}
    // Open connection
    err = iConnection.Open( iSocketServ );
    if ( err != KErrNone)
    	{
    	return err;
    	} 
    currentConnInfo().iIapId = iIapId;
    currentConnInfo().iNetId = iNetId;
    
	err = iConnection.Attach( currentConnInfo,
							  RConnection::EAttachTypeMonitor );
	
	if (err != KErrNone)
	    {
	    iConnection.Close(); // Just to be sure...
	    }
	
	return err;
	}

// ----------------------------------------------------
// CHsBrowserModel::CHsBrowserModel
// ----------------------------------------------------
//
CHsBrowserModel::CHsBrowserModel() :
    iIapId( 1 ), 
    iNetId( 0 ),
    iUrl( NULL ),
    iState( 0 ),
    iForeground( ETrue ),
    iLoginCompleteNoteShown( EFalse ),
    iStaticTitle( NULL ),
    iPageTitle( NULL)
    {
    DEBUG( "CHsBrowserModel::CHsBrowserModel()" );
    }

// ----------------------------------------------------
// CHsBrowserModel::ConstructL
// ----------------------------------------------------
//
void CHsBrowserModel::ConstructL()
    {
    DEBUG( "CHsBrowserModel::ConstructL()" );
    User::LeaveIfError( iHsServer.Connect() );
    iPosition = CCoeEnv::Static()->RootWin().OrdinalPosition();
    iPriority = CCoeEnv::Static()->RootWin().OrdinalPriority();
    }

// end of file
