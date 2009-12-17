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
* Description:  Implementation of CWsfActiveWaiter
*
*/


// INCLUDE FILES
#include "wsfactivewaiter.h"
#include "wsflogger.h"



// ---------------------------------------------------------
// CWsfActiveWaiter* CWsfActiveWaiter::NewL
// ---------------------------------------------------------
//
CWsfActiveWaiter* CWsfActiveWaiter::NewL()
    {
    CWsfActiveWaiter* self = NewLC();
    CleanupStack::Pop( self );
    
    return self;        
    }


// ---------------------------------------------------------
// CWsfActiveWaiter* CWsfActiveWaiter::NewLC
// ---------------------------------------------------------
//
CWsfActiveWaiter* CWsfActiveWaiter::NewLC()
    {
    CWsfActiveWaiter* self = new ( ELeave ) CWsfActiveWaiter();
    CleanupStack::PushL( self );
    
    return self;        
    }


// ---------------------------------------------------------
// CWsfActiveWaiter::CWsfActiveWaiter
// ---------------------------------------------------------
//
CWsfActiveWaiter::CWsfActiveWaiter(): 
    CActive( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }
    

// ---------------------------------------------------------
// CWsfActiveWaiter::~CWsfActiveWaiter
// ---------------------------------------------------------
//    
CWsfActiveWaiter::~CWsfActiveWaiter()
    {
    Cancel();
    }


// ---------------------------------------------------------
// CWsfActiveWaiter::DoCancel
// ---------------------------------------------------------
// 
void CWsfActiveWaiter::DoCancel()
    {
    }
     
 
// ---------------------------------------------------------
// CWsfActiveWaiter::RunL
// ---------------------------------------------------------
//     
void CWsfActiveWaiter::RunL()
    {
    LOG_ENTERFN( "CWsfActiveWaiter::RunL" );
    if ( iWait.IsStarted() )
        {
        LOG_WRITE("async call completed, resuming.");
        iWait.AsyncStop();
        }
    }

// ---------------------------------------------------------
// CWsfActiveWaiter::WaitForRequest
// ---------------------------------------------------------
// 
TInt CWsfActiveWaiter::WaitForRequest()
    {
    LOG_ENTERFN("CWsfActiveWaiter::WaitForRequest");
    SetActive(); 
    iWait.Start();

    return iStatus.Int();
    }    
    
// End of File

