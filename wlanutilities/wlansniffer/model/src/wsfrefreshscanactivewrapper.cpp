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
* Description:  Implementation of CWsfRefreshScanActiveWrapper.
*
*/


// INCLUDE FILES
#include "wsflogger.h"
#include "wsfmodel.h"
#include "wsfwlaninfoarray.h"
#include "wsfrefreshscanactivewrapper.h"

// --------------------------------------------------------------------------
// CWsfRefreshScanActiveWrapper::CWsfRefreshScanActiveWrapper
// --------------------------------------------------------------------------
//
CWsfRefreshScanActiveWrapper::CWsfRefreshScanActiveWrapper() :
    CActive(EPriorityStandard) // Standard priority
    {
    }


// --------------------------------------------------------------------------
// CWsfRefreshScanActiveWrapper::NewLC
// --------------------------------------------------------------------------
//
CWsfRefreshScanActiveWrapper* CWsfRefreshScanActiveWrapper::NewLC( CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfRefreshScanActiveWrapper::NewLC" );
    CWsfRefreshScanActiveWrapper* self =
            new (ELeave) CWsfRefreshScanActiveWrapper();
    CleanupStack::PushL(self);
    self->ConstructL( aModel );
    return self;
    }


// --------------------------------------------------------------------------
// CWsfRefreshScanActiveWrapper::NewL
// --------------------------------------------------------------------------
//
CWsfRefreshScanActiveWrapper* CWsfRefreshScanActiveWrapper::NewL(  CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfRefreshScanActiveWrapper::NewL" );
    CWsfRefreshScanActiveWrapper* self = 
            CWsfRefreshScanActiveWrapper::NewLC( aModel );
    CleanupStack::Pop(); // self;
    return self;
    }


// --------------------------------------------------------------------------
// CWsfRefreshScanActiveWrapper::ConstructL
// --------------------------------------------------------------------------
//
void CWsfRefreshScanActiveWrapper::ConstructL( CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfRefreshScanActiveWrapper::ConstructL" );
    CActiveScheduler::Add(this); // Add to scheduler
    iModel = aModel;
    }


// --------------------------------------------------------------------------
// CWsfRefreshScanActiveWrapper::~CWsfRefreshScanActiveWrapper
// --------------------------------------------------------------------------
//
CWsfRefreshScanActiveWrapper::~CWsfRefreshScanActiveWrapper()
    {
    LOG_ENTERFN( 
            "WsfRefreshScanActiveWrapper::~WsfRefreshScanActiveWrapper" );
    Cancel(); // Cancel any request, if outstanding
    // Delete instance variables if any
    }


// --------------------------------------------------------------------------
// CWsfRefreshScanActiveWrapper::DoCancel
// --------------------------------------------------------------------------
//
void CWsfRefreshScanActiveWrapper::DoCancel()
    {
    LOG_ENTERFN( "CWsfRefreshScanActiveWrapper::DoCancel" );
    }


// --------------------------------------------------------------------------
// CWsfRefreshScanActiveWrapper::StartL
// --------------------------------------------------------------------------
//
void CWsfRefreshScanActiveWrapper::Start()
    {
    LOG_ENTERFN( "CWsfRefreshScanActiveWrapper::Start" );
    Cancel(); // Cancel any request, just to be sure
    iState = EUninitialized;
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    }


// --------------------------------------------------------------------------
// CWsfRefreshScanActiveWrapper::RunL
// --------------------------------------------------------------------------
//
void CWsfRefreshScanActiveWrapper::RunL()
    {
    LOG_ENTERFN( "CWsfRefreshScanActiveWrapper::RunL" );
    if (iStatus == KErrNone)
       {
        if (iState == EUninitialized)
            {
            LOG_WRITE( "request scan" );
            iModel->RefreshScan( iPckg, iStatus );
            iState = EInitialized;
            SetActive(); // Tell scheduler a request is active
            }
        else if (iState == EInitialized )
            {
            LOG_WRITEF( "request result = %d", iPckg() );
            iModel->SetRefreshState( iPckg() );
            }
        else
            {
            LOG_WRITEF( "iState = %d", iState );
            }
       }
    else
        {
        LOG_WRITEF( "RefreshScanActiveWrapper iStatus = %d", iStatus.Int() );
        }
    }


// --------------------------------------------------------------------------
// CWsfRefreshScanActiveWrapper::RunError
// --------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CWsfRefreshScanActiveWrapper::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfRefreshScanActiveWrapper::RunError" );
    LOG_WRITEF( "aError = %d", aError );
    return aError;
    }
#else
TInt CWsfRefreshScanActiveWrapper::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
#endif



