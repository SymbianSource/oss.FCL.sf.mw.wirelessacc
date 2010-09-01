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
* Description:   Handle special load requests like Non-HTTP(S) load requests,
*                and Non-HTML content. Control over network connections
*
*/



// INCLUDE FILES

#include "hsbrowserspecialloadobserver.h"
#include "am_debug.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CHsBrowserSpecialLoadObserver::NetworkConnectionNeededL
// ---------------------------------------------------------
//
void CHsBrowserSpecialLoadObserver::NetworkConnectionNeededL(
    TInt* aConnectionPtr,
    TInt* aSockSvrHandle,
    TBool* /*aNewConn*/,
    TApBearerType* /*aBearerType*/) 
    {
    DEBUG( "CHsBrowserSpecialLoadObserver::NetworkConnectionNeededL()" );
    *aConnectionPtr = iConnectionPtr;
    *aSockSvrHandle = iSockSvrHandle;
    }

// ---------------------------------------------------------
// CHsBrowserSpecialLoadObserver::HandleRequestL
// ---------------------------------------------------------
//
TBool CHsBrowserSpecialLoadObserver::HandleRequestL(
    RArray<TUint>* /*aTypeArray*/,
    CDesCArrayFlat* /*aDesArray*/) 
    {
    DEBUG( "CHsBrowserSpecialLoadObserver::HandleRequestL()" );
    return EFalse;
    }

// ---------------------------------------------------------
// CHsBrowserSpecialLoadObserver::HandleDownloadL
// ---------------------------------------------------------
//
TBool CHsBrowserSpecialLoadObserver::HandleDownloadL(
    RArray<TUint>* /*aTypeArray*/,
    CDesCArrayFlat* /*aDesArray*/)
    {
    DEBUG( "CHsBrowserSpecialLoadObserver::HandleDownloadL()" );
    return EFalse;
    }

// ---------------------------------------------------------
// CHsBrowserSpecialLoadObserver::ConstructL
// ---------------------------------------------------------
//
void CHsBrowserSpecialLoadObserver::ConstructL()
    {
    DEBUG( "CHsBrowserSpecialLoadObserver::ConstructL()" );
    }

// ---------------------------------------------------------
// CHsBrowserSpecialLoadObserver::~CHsBrowserSpecialLoadObserver
// ---------------------------------------------------------
//
CHsBrowserSpecialLoadObserver::~CHsBrowserSpecialLoadObserver()
    {
    DEBUG( "CHsBrowserSpecialLoadObserver::~CHsBrowserSpecialLoadObserver()" );
    }


// ---------------------------------------------------------
// CHsBrowserSpecialLoadObserver::NewL
// ---------------------------------------------------------
//
CHsBrowserSpecialLoadObserver* CHsBrowserSpecialLoadObserver::NewL()
    {
    DEBUG( "CHsBrowserSpecialLoadObserver::CHsBrowserSpecialLoadObserver::NewL()()" );
    CHsBrowserSpecialLoadObserver* self = new(ELeave)CHsBrowserSpecialLoadObserver;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CHsBrowserSpecialLoadObserver::SetConnectionPtr
// ---------------------------------------------------------
//
void CHsBrowserSpecialLoadObserver::SetConnectionPtr( TInt aConnectionPtr )
	{
	iConnectionPtr = aConnectionPtr;
	}

// ---------------------------------------------------------
// CHsBrowserSpecialLoadObserver::SetSockSvrHandle
// ---------------------------------------------------------
//        
void CHsBrowserSpecialLoadObserver::SetSockSvrHandle( TInt aSockSvrHandle )
	{
	iSockSvrHandle = aSockSvrHandle;
	}
	
// End of File  
