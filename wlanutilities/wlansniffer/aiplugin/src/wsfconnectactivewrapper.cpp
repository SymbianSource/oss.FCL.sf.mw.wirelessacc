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
* Description:  Implementation of CWsfConnectActiveWrapper.
*
*/



// INCLUDE FILES
#include "wsflogger.h"
#include "wsfmodel.h"
#include "wsfwlaninfoarray.h"
#include "wsfconnectactivewrapper.h"


// --------------------------------------------------------------------------
// CWsfConnectActiveWrapper::CWsfConnectActiveWrapper
// --------------------------------------------------------------------------
//
CWsfConnectActiveWrapper::CWsfConnectActiveWrapper() :
    CActive(EPriorityStandard) // Standard priority
    {
    }


// --------------------------------------------------------------------------
// CWsfConnectActiveWrapper::NewLC
// --------------------------------------------------------------------------
//
CWsfConnectActiveWrapper* CWsfConnectActiveWrapper::NewLC( CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfConnectActiveWrapper::NewLC" );
    CWsfConnectActiveWrapper* self =
            new (ELeave) CWsfConnectActiveWrapper();
    CleanupStack::PushL(self);
    self->ConstructL( aModel );
    return self;
    }


// --------------------------------------------------------------------------
// CWsfConnectActiveWrapper::NewL
// --------------------------------------------------------------------------
//
CWsfConnectActiveWrapper* CWsfConnectActiveWrapper::NewL(  CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfConnectActiveWrapper::NewL" );
    CWsfConnectActiveWrapper* self = 
            CWsfConnectActiveWrapper::NewLC( aModel );
    CleanupStack::Pop(); // self;
    return self;
    }


// --------------------------------------------------------------------------
// CWsfConnectActiveWrapper::ConstructL
// --------------------------------------------------------------------------
//
void CWsfConnectActiveWrapper::ConstructL( CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfConnectActiveWrapper::ConstructL" );
    CActiveScheduler::Add(this); // Add to scheduler
    iModel = aModel;
    }


// --------------------------------------------------------------------------
// CWsfConnectActiveWrapper::~CWsfConnectActiveWrapper
// --------------------------------------------------------------------------
//
CWsfConnectActiveWrapper::~CWsfConnectActiveWrapper()
    {
    LOG_ENTERFN( 
            "CWsfConnectActiveWrapper::~CWsfConnectActiveWrapper" );
    Cancel(); // Cancel any request, if outstanding
    // Delete instance variables if any
    }


// --------------------------------------------------------------------------
// CWsfConnectActiveWrapper::DoCancel
// --------------------------------------------------------------------------
//
void CWsfConnectActiveWrapper::DoCancel()
    {
    LOG_ENTERFN( "CWsfConnectActiveWrapper::DoCancel" );
    TRAP_IGNORE( iModel->AbortConnectingL() );
    }


// --------------------------------------------------------------------------
// CWsfConnectActiveWrapper::StartL
// --------------------------------------------------------------------------
//
void CWsfConnectActiveWrapper::Start(TUint aIapID, TWsfIapPersistence aPersistence )
    {
    LOG_ENTERFN( "CWsfConnectActiveWrapper::Start" );
    Cancel(); // Cancel any request, just to be sure
    iState = EUninitialized;
    iIapID = aIapID;
    iPersistence = aPersistence;
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }


// --------------------------------------------------------------------------
// CWsfConnectActiveWrapper::RunL
// --------------------------------------------------------------------------
//
void CWsfConnectActiveWrapper::RunL()
    {
    LOG_ENTERFN( "CWsfConnectActiveWrapper::RunL" );
    if (iState == EUninitialized)
        {
        LOG_WRITE( "Start connect" );
        iModel->ConnectL( iPckg, iIapID, iPersistence, iStatus );
        iState = EInitialized;
        SetActive(); // Tell scheduler a request is active
        }
    else if (iState == EInitialized )
        {
        LOG_WRITEF( "request result = %d", iPckg() );
        iModel->SetConnectResultL(iPckg(), iIapID );
        } 
    else
        {
        LOG_WRITEF( "iState = %d", iState );
        }
    }


// --------------------------------------------------------------------------
// CWsfConnectActiveWrapper::RunError
// --------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CWsfConnectActiveWrapper::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfConnectActiveWrapper::RunError" );
    LOG_WRITEF( "aError = %d", aError );
    TRAP_IGNORE( iModel->SetConnectResultL(iPckg(), iIapID ) );
    return aError;
    }
#else
TInt CWsfConnectActiveWrapper::RunError( TInt /*aError*/ )
    {
    TRAP_IGNORE( iModel->SetConnectResultL(iPckg(), iIapID ) );
    return KErrNone;
    }
#endif



