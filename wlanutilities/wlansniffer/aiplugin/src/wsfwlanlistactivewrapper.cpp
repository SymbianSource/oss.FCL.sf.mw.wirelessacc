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
 * Description:  Implementation of CWsfWLANListActiveWrapper.
 *
 */


// INCLUDE FILES
#include "wsflogger.h"
#include "wsfmodel.h"
#include "wsfwlaninfoarray.h"
#include "wsfaicontroller.h"
#include "wsfwlanlistactivewrapper.h"


/**
* Number of retries to fetch wlan data
*/
static const TInt KRetries = 5;


// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::CWsfWLANListActiveWrapper
// ----------------------------------------------------------------------------
//
CWsfWLANListActiveWrapper::CWsfWLANListActiveWrapper() :
    CActive( EPriorityStandard ), // Standard priority
    iPtr( NULL, 0 )
    {
    }


// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::NewLC
// ----------------------------------------------------------------------------
//
CWsfWLANListActiveWrapper* CWsfWLANListActiveWrapper::NewLC( CWsfModel* aModel, 
                                                TWsfAiController &aController )
    {
    LOG_ENTERFN( "CWsfWLANListActiveWrapper::NewLC" );
    CWsfWLANListActiveWrapper* self =
            new (ELeave) CWsfWLANListActiveWrapper();
    CleanupStack::PushL(self);
    self->ConstructL( aModel, aController );
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::NewL
// ----------------------------------------------------------------------------
//
CWsfWLANListActiveWrapper* CWsfWLANListActiveWrapper::NewL( CWsfModel* aModel, 
                                                TWsfAiController &aController )
    {
    LOG_ENTERFN( "CWsfWLANListActiveWrapper::NewL" );
    CWsfWLANListActiveWrapper* self = CWsfWLANListActiveWrapper::NewLC(
             aModel, aController );
    CleanupStack::Pop(); // self;
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfWLANListActiveWrapper::ConstructL( CWsfModel* aModel,
                                            TWsfAiController &aController )
    {
    LOG_ENTERFN( "CWsfWLANListActiveWrapper::ConstructL" );
    CActiveScheduler::Add(this); // Add to scheduler
    iModel = aModel;
    iController = &aController;
    iArray = CWsfWlanInfoArray::NewL();
    }

// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::~CWsfWLANListActiveWrapper
// ----------------------------------------------------------------------------
//
CWsfWLANListActiveWrapper::~CWsfWLANListActiveWrapper()
    {
    LOG_ENTERFN( "CWsfWLANListActiveWrapper::~CWsfWLANListActiveWrapper" );
    Cancel(); // Cancel any request, if outstanding
    // Delete instance variables if any
    if ( iBuffer )
        {
        delete iBuffer;
        }
    delete iArray;
    }

// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::DoCancel
// ----------------------------------------------------------------------------
//
void CWsfWLANListActiveWrapper::DoCancel()
    {
    }

// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::Start
// ----------------------------------------------------------------------------
//
void CWsfWLANListActiveWrapper::Start( TBool aStarUp )
    {
    LOG_ENTERFN( "CWsfWLANListActiveWrapper::Start" );
    Cancel(); // Cancel any request, just to be sure
    iStartUp = aStarUp;
    iState = EUninitialized;
    iRetriesLeft = KRetries;
    iPckgAllocatedSize() = 0;
    iPckgNeededSize() = 0;
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
    }

// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::GetWlanList
// ----------------------------------------------------------------------------
//
CWsfWlanInfoArray* CWsfWLANListActiveWrapper::GetWlanList()
    {
    LOG_ENTERFN( "CWsfWLANListActiveWrapper::GetWlanList" );
    return iArray;
    }

// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::GetConnectedWLANNetwork
// ----------------------------------------------------------------------------
//
TWsfWlanInfo CWsfWLANListActiveWrapper::GetConnectedWLANNetwork()
    {
    LOG_ENTERFN( "CWsfWLANListActiveWrapper::GetConnectedWLANNetwork" );
    return iConnectedWlan;
    }

// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::RunL
// ----------------------------------------------------------------------------
//
void CWsfWLANListActiveWrapper::RunL()
    {
    LOG_ENTERFN( "CWsfWLANListActiveWrapper::RunL" );
    if (iStatus == KErrNone)
        {
        if ( iState == EUninitialized )
            {
            LOG_WRITE( "Get WLAN list size" );
            iModel->GetWlanListSize( iPckgNeededSize, iStatus );
            iState = EInitialized;
            SetActive(); // Tell scheduler a request is active
            }
        else if ( iState == EInitialized )
            {
            LOG_WRITEF( "WLAN data buffer size = %d", iPckgNeededSize() );
        
            if ( !iPckgNeededSize() )
                {
                LOG_WRITE( "no data.." );
                iState = EProcessWLANListData;
                SetActive();
                TRequestStatus* status = &iStatus;
                User::RequestComplete(status, KErrNone);
                }
            else
                {
                // alloc the required size buffer...
                delete iBuffer;
                iBuffer = NULL;
                iBuffer = HBufC8::NewL( iPckgNeededSize() );
                iPtr.Set( iBuffer->Des() );
            
                LOG_WRITE( "Get WLAN list" );
                iModel->GetWlanList( iPckgAllocatedSize, iPtr, iStatus );
                iState = EProcessWLANListData;
                SetActive(); // Tell scheduler a request is active
                }
            }
        else if ( iState == EProcessWLANListData )
            {
            LOG_WRITEF( "actual bytes occupied = %d", iPckgAllocatedSize() );
        
            if ( iPckgNeededSize() != iPckgAllocatedSize() )
                {
                // the buffer is not long enough... stop 
                if ( iRetriesLeft > 0 )
                    {
                    LOG_WRITEF( "iRetriesLeft = %d", iRetriesLeft );
                    iRetriesLeft--;
                    iState = EUninitialized;
                    SetActive();
                    TRequestStatus* status = &iStatus;
                    User::RequestComplete( status, KErrNone );
                    return;
                    }
                else
                    {
                    // no more retries
                    User::Leave( KErrOverflow );
                    }
                }
            
            iArray->Reset();
            
            if ( iPckgAllocatedSize() )
                {
                iArray->AppendFromStreamBufferL( iPtr );
                }
            
            LOG_WRITEF( "Array count=%d startup=%d", iArray->Count(), iStartUp );
            
            iModel->GetConnectedWlanDetails( iPckg, iConnectedWlan, iStatus );
            iState = EGetConnectedNetwork;
            SetActive(); // Tell scheduler a request is active
            }
        else if ( iState == EGetConnectedNetwork )
            {
            LOG_WRITEF( "request result = %d", iPckg() );
            
            if ( !iPckg() )
                {
                LOG_WRITE( "result is false, so wlaninfo is marked not connected" );
                iConnectedWlan.iConnectionState = ENotConnected;
                }
            
            LOG_WRITEF( "Connected = %d", iConnectedWlan.Connected() );
            
            LOG_WRITEF( "iConnectedWlan state = %d", 
                                                iConnectedWlan.iConnectionState );
            
            if ( iStartUp )
                {
                iController->StartupRefreshDataReadyL();
                }
            else
                {
                iController->WlanListDataReadyL();
                }
            }
        else
            {
            LOG_WRITEF( "iState = %d", iState );
            }
        }
    else
        {
        LOG_WRITEF( "WLANListActiveWrapper iStatus = %d", iStatus.Int() );
        }
    }

// ----------------------------------------------------------------------------
// CWsfWLANListActiveWrapper::RunError
// ----------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CWsfWLANListActiveWrapper::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfWLANListActiveWrapper::RunError" );
    LOG_WRITEF( "aError = %d", aError );
    return aError;
    }
#else
TInt CWsfWLANListActiveWrapper::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
#endif
