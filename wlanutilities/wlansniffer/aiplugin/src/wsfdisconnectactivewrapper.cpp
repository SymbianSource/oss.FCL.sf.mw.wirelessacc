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
* Description:  Implementation of CWsfDisconnectActiveWrapper.
*
*/


// INCLUDE FILES
#include "wsflogger.h"
#include "wsfmodel.h"
#include "wsfwlaninfoarray.h"
#include "wsfdisconnectactivewrapper.h"

// --------------------------------------------------------------------------
// CWsfDisconnectActiveWrapper::CWsfDisconnectActiveWrapper
// --------------------------------------------------------------------------
//
CWsfDisconnectActiveWrapper::CWsfDisconnectActiveWrapper() :
    CActive(EPriorityStandard) // Standard priority
    {
    }


// --------------------------------------------------------------------------
// CWsfDisconnectActiveWrapper::NewLC
// --------------------------------------------------------------------------
//
CWsfDisconnectActiveWrapper* CWsfDisconnectActiveWrapper::NewLC( CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfDisconnectActiveWrapper::NewLC" );
    CWsfDisconnectActiveWrapper* self =
            new (ELeave) CWsfDisconnectActiveWrapper();
    CleanupStack::PushL(self);
    self->ConstructL( aModel );
    return self;
    }


// --------------------------------------------------------------------------
// CWsfDisconnectActiveWrapper::NewL
// --------------------------------------------------------------------------
//
CWsfDisconnectActiveWrapper* CWsfDisconnectActiveWrapper::NewL(  CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfDisconnectActiveWrapper::NewL" );
    CWsfDisconnectActiveWrapper* self = 
            CWsfDisconnectActiveWrapper::NewLC( aModel );
    CleanupStack::Pop(); // self;
    return self;
    }


// --------------------------------------------------------------------------
// CWsfDisconnectActiveWrapper::ConstructL
// --------------------------------------------------------------------------
//
void CWsfDisconnectActiveWrapper::ConstructL( CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfDisconnectActiveWrapper::ConstructL" );
    CActiveScheduler::Add(this); // Add to scheduler
    iModel = aModel;
    }


// --------------------------------------------------------------------------
// CWsfDisconnectActiveWrapper::~CWsfDisconnectActiveWrapper
// --------------------------------------------------------------------------
//
CWsfDisconnectActiveWrapper::~CWsfDisconnectActiveWrapper()
    {
    LOG_ENTERFN( 
            "CWsfDisconnectActiveWrapper::~CWsfDisconnectActiveWrapper" );
    Cancel(); // Cancel any request, if outstanding
    // Delete instance variables if any
    }


// --------------------------------------------------------------------------
// CWsfDisconnectActiveWrapper::DoCancel
// --------------------------------------------------------------------------
//
void CWsfDisconnectActiveWrapper::DoCancel()
    {
    LOG_ENTERFN( "CWsfDisconnectActiveWrapper::DoCancel" );
    }


// --------------------------------------------------------------------------
// CWsfDisconnectActiveWrapper::StartL
// --------------------------------------------------------------------------
//
void CWsfDisconnectActiveWrapper::Start()
    {
    LOG_ENTERFN( "CWsfDisconnectActiveWrapper::Start" );
    Cancel(); // Cancel any request, just to be sure
    iState = EUninitialized;
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }


// --------------------------------------------------------------------------
// CWsfDisconnectActiveWrapper::RunL
// --------------------------------------------------------------------------
//
void CWsfDisconnectActiveWrapper::RunL()
    {
    LOG_ENTERFN( "CWsfDisconnectActiveWrapper::RunL" );
    if (iState == EUninitialized)
        {
        LOG_WRITE( "disconnect" );
        iModel->Disconnect( iPckg, iStatus );
        iState = EInitialized;
        SetActive(); // Tell scheduler a request is active
        }
    else if (iState == EInitialized )
        {
        LOG_WRITEF( "request result = %d", iPckg() );
        }
    else
        {
        LOG_WRITEF( "iState = %d", iState );
        }
    }


// --------------------------------------------------------------------------
// CWsfDisconnectActiveWrapper::RunError
// --------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CWsfDisconnectActiveWrapper::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfDisconnectActiveWrapper::RunError" );
    LOG_WRITEF( "aError = %d", aError );
    return aError;
    }
#else
TInt CWsfDisconnectActiveWrapper::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
#endif



