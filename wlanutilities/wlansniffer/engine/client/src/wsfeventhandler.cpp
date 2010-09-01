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
* Description:  Implementation of CWsfEventHandler
*
*/




//  CLASS HEADER
#include "wsfeventhandler.h"

//  INTERNAL INCLUDES
#include "wsfstatechangeobserver.h"
#include "wsflogger.h"


// ---------------------------------------------------------------------------
// CWsfEventHandler::NewL
// ---------------------------------------------------------------------------
//
CWsfEventHandler* CWsfEventHandler::NewL( RWsfSession& aSession, 
                                          MWsfStateChangeObserver& aObserver )
    {
    CWsfEventHandler* thisPtr = new (ELeave) CWsfEventHandler( aSession, 
                                                               aObserver );
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    CleanupStack::Pop( thisPtr );
    return thisPtr;
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::~CWsfEventHandler
// ---------------------------------------------------------------------------
//
CWsfEventHandler::~CWsfEventHandler()
    {
    Cancel();
    iSession = NULL;
    iObserver = NULL;
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::CWsfEventHandler
// ---------------------------------------------------------------------------
//
CWsfEventHandler::CWsfEventHandler( RWsfSession& aSession, 
                                    MWsfStateChangeObserver& aObserver ): 
    CActive( CActive::EPriorityStandard )
    {
    iSession = &aSession;
    iObserver = &aObserver;
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfEventHandler::ConstructL()
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::DoRunL
// ---------------------------------------------------------------------------
//
void CWsfEventHandler::DoRunL()
    {
    LOG_ENTERFN( "CWsfEventHandler::DoRunL" );    
    LOG_WRITEF( "CWsfEventHandler::DoRunL event = %d", iEvent().iEvent );    

    switch ( iEvent().iEvent )
        {
        case EEngineWlanDataChanged:
            {
            iObserver->WlanListChangedL();
            break;
            }

        case EEngineScanDisabled:
            {
            iObserver->ScanDisabledL();
            break;
            }

        case EEngineScanEnabled:
            {
            iObserver->ScanEnabledL();
            break;
            }

        case EEngineConnected:
            {
            if ( !iBlockConnectedEvent )
                {
                // if the event wasn't blocked, notify the observer
                iObserver->WlanConnectionActivatedL();
                }
            else
                {
                // if it was blocked, just clear the flag
                iBlockConnectedEvent = EFalse;
                }
                
            // anyhow, connecting process is finished
            if ( iConnecting )
                {
                iConnecting = EFalse;
                iSession->StopConnectingWait();
                iObserver->ConnectionCreationProcessFinishedL( 
                                        KErrNone );
                }
            break;
            }

        case EEngineDisconnected:
            {
            iObserver->WlanConnectionClosedL();
            break;
            }

        case EEngineError:
            {
            iObserver->NotifyEngineError( iEvent().iError );
            break;
            }
            
        case EEngineConnectingFailed:
            {
            if ( iConnecting )
                {
                // connecting is over
                iConnecting = EFalse;

                // make client return with KErrCancel
                iSession->iConnectingResult = iEvent().iError;
                iSession->StopConnectingWait();
                iObserver->ConnectionCreationProcessFinishedL( 
                        iSession->iConnectingResult );
                }
            break;
            }

        default:
            __ASSERT_DEBUG(0, _L("CWsfEventHandler - Invalid EngineEvent"));
            break;        
        }

    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::RunL
// ---------------------------------------------------------------------------
//
void CWsfEventHandler::RunL()
    {
    LOG_ENTERFN( "CWsfEventHandler::RunL" );
    TInt notifyError( KErrNone );
    TBool reissue( ETrue );
    
    if ( iStatus.Int() >= KErrNone && iObserver )
        {
        TRAP( notifyError, DoRunL() );
        }
        
    if ( notifyError )
        {
        // notified class leaved
        LOG_WRITEF( "CWsfEventHandler::RunL notify error %d", notifyError );
        }
    else if ( iStatus.Int() != KErrNone )
        {
        if ( iStatus.Int() == KErrCancel )
            {
            // KErrCancel is the signal that we are going down
            LOG_WRITE( "going down..." );
            reissue = EFalse;
            }
        else
            {
            // error on the server side
            LOG_WRITEF( "CWsfEventHandler::RunL engine error %d", 
                        iStatus.Int() );

            TBool notify( ETrue );
                      
            if ( iStatus.Int() == KErrServerTerminated )
                {
                reissue = EFalse;
                
                if ( iObserver
                        && iSession->RecoverFromServerTermination( *iObserver ) )
                    {
                    // no need to reissue if recovered since it is already
                    // done in the recovery process
                    notify = EFalse;
                    }
                }

            if ( notify && iObserver )
                {
                // let the client decide what to do
                iObserver->NotifyEngineError( iStatus.Int() );
                }
            }
        }
        

    if ( reissue && iObserver )
        {
        // reissue callback request
        iSession->NotifyEventL( *iObserver );
        }
    }
  

// ---------------------------------------------------------------------------
// CWsfEventHandler::DoCancel
// ---------------------------------------------------------------------------
//
void CWsfEventHandler::DoCancel()
    {
    LOG_ENTERFN( "CWsfEventHandler::DoCancel" );
    iSession->DoCancelNotifyEvent();
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::RunError
// ---------------------------------------------------------------------------
//
TInt CWsfEventHandler::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfEventHandler::RunError" );
    LOG_WRITEF( "CWsfEventHandler::RunError error = %d", aError );
    if ( iObserver )
        {
        iObserver->NotifyEngineError( aError ) ;
        }
    return KErrNone; // we don't wan't to panic euser-47 - do we?
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::Start
// ---------------------------------------------------------------------------
//
void CWsfEventHandler::Start()
    {
    SetActive();
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::EventContainer
// ---------------------------------------------------------------------------
//
TPckgBuf<TWsfNotifyEventContainer>& CWsfEventHandler::EventContainer()
    {
    return iEvent;
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::ResetObserver
// ---------------------------------------------------------------------------
//
void CWsfEventHandler::ResetObserver( MWsfStateChangeObserver& aObserver )
    {
    LOG_ENTERFN( "CWsfEventHandler::ResetObserver" );
    iObserver = &aObserver;
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::BlockNextConnectedEvent
// ---------------------------------------------------------------------------
//
void CWsfEventHandler::BlockNextConnectedEvent()
    {
    LOG_ENTERFN( "CWsfEventHandler::BlockNextConnectedEvent" );
    if ( iConnecting )
        {
        LOG_WRITE( "iBlockConnectedEvent to true" );
        // set the flag only if we are the ones who are connecting
        iBlockConnectedEvent = ETrue;
        }
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::UnBlockNextConnectedEvent
// ---------------------------------------------------------------------------
//
void CWsfEventHandler::UnBlockNextConnectedEvent()
    {
    LOG_ENTERFN( "CWsfEventHandler::UnBlockNextConnectedEvent" );
    LOG_WRITE( "iBlockConnectedEvent to false" );
    iBlockConnectedEvent = EFalse;
    }


// ---------------------------------------------------------------------------
// CWsfEventHandler::SetConnecting
// ---------------------------------------------------------------------------
//
void CWsfEventHandler::SetConnecting( TBool aConnecting )
    {
    LOG_ENTERFN( "CWsfEventHandler::SetConnecting" );
    LOG_WRITEF( "iConnecting=%d, aConnecting=%d", iConnecting, aConnecting );
    iConnecting = aConnecting;
    }
    
    
// ---------------------------------------------------------------------------
// CWsfEventHandler::Connecting
// ---------------------------------------------------------------------------
//
TBool CWsfEventHandler::Connecting() const
    {
    return iConnecting;
    }

