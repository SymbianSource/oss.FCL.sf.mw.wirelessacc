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
* Description:  Implementation of CWsfServerCloser
*
*/


//  CLASS HEADER
#include "wsfserver.h"


/**
* Timeout after all closing requirements are met (in microseconds)
*/
static const TInt KServerClosureWaitTimer = 2 * 1000 * 1000; 


// ---------------------------------------------------------------------------
// CWsfServer::CWsfServerCloser::NewL
// ---------------------------------------------------------------------------
//		
CWsfServer::CWsfServerCloser* CWsfServer::CWsfServerCloser::NewL()
	{
	CWsfServer::CWsfServerCloser* thisPtr = new (ELeave) CWsfServerCloser();
	CleanupStack::PushL( thisPtr );
	thisPtr->ConstructL();
	CleanupStack::Pop( thisPtr );
	return thisPtr;
	}


// ---------------------------------------------------------------------------
// CWsfServer::CWsfServerCloser::~CWsfServerCloser
// ---------------------------------------------------------------------------
//		
CWsfServer::CWsfServerCloser::~CWsfServerCloser()
	{
	Cancel();
	}


// ---------------------------------------------------------------------------
// CWsfServer::CWsfServerCloser::CWsfServerCloser
// ---------------------------------------------------------------------------
//		
CWsfServer::CWsfServerCloser::CWsfServerCloser(): 
    CTimer( CActive::EPriorityStandard ),
    iWaitForOwnedConnection( EFalse ),
    iWaitForBrowserExit( EFalse ),
    iWaitForClients( EFalse )
    {
	CActiveScheduler::Add( this );
	}


// ---------------------------------------------------------------------------
// CWsfServer::CWsfServerCloser::ConstructL
// ---------------------------------------------------------------------------
//		
void CWsfServer::CWsfServerCloser::ConstructL()
	{
	CTimer::ConstructL();
	}


// ---------------------------------------------------------------------------
// CWsfServer::CWsfServerCloser::RunL
// ---------------------------------------------------------------------------
//		
void CWsfServer::CWsfServerCloser::RunL()
	{
	CActiveScheduler::Stop();
	}


// ---------------------------------------------------------------------------
// CWsfServer::CWsfServerCloser::StartClosureWait
// ---------------------------------------------------------------------------
//		
void CWsfServer::CWsfServerCloser::StartClosureWait()
	{
	Cancel();
	
	if ( !iWaitForClients && !iWaitForBrowserExit && !iWaitForOwnedConnection )
	    {
	    After( TTimeIntervalMicroSeconds32( KServerClosureWaitTimer ) );
	    }	
	}


// ---------------------------------------------------------------------------
// CWsfServer::CWsfServerCloser::WaitForClients
// ---------------------------------------------------------------------------
//		
void CWsfServer::CWsfServerCloser::WaitForClients( TBool aWait )
    {
    iWaitForClients = aWait;
    if ( !aWait )
        {
        StartClosureWait();
        }
    else
        {
        Cancel();
        }
    }

// ---------------------------------------------------------------------------
// CWsfServer::CWsfServerCloser::WaitForOwnedConnection
// ---------------------------------------------------------------------------
//		
void CWsfServer::CWsfServerCloser::WaitForOwnedConnection( TBool aWait )
    {
    iWaitForOwnedConnection = aWait;
    if ( !aWait )
        {
        StartClosureWait();
        }
    else
        {
        Cancel();
        }
    }
    
    
// ---------------------------------------------------------------------------
// CWsfServer::CWsfServerCloser::WaitForBrowserExit
// ---------------------------------------------------------------------------
//		
void CWsfServer::CWsfServerCloser::WaitForBrowserExit( TBool aWait )
    {
    iWaitForBrowserExit = aWait;
    if ( !aWait )
        {
        StartClosureWait();
        }
    else
        {
        Cancel();
        }
    }

