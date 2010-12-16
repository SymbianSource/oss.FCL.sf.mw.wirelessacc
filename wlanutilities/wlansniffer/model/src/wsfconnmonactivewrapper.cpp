/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CWsfConnMonActiveWrapper.
*
*/



// INCLUDE FILES
#include "wsflogger.h"
#include "wsfconnmonactivewrapper.h"
#include "wsfmodelobserver.h"


// --------------------------------------------------------------------------
// CWsfConnMonActiveWrapper::CWsfConnMonActiveWrapper
// --------------------------------------------------------------------------
//
CWsfConnMonActiveWrapper::CWsfConnMonActiveWrapper( MWsfModelObserver &aModelObserver ) :
    CActive( EPriorityStandard ), 
    iState( EUninitialized ),
    iCancelCode( 0 ),
    iConnectionCount( 0 ), 
    iConnectionId( 0 ),
    iBearerType( EBearerUnknown ), 
    iModelObserver( aModelObserver )
    {
    }


// --------------------------------------------------------------------------
// CWsfConnMonActiveWrapper::NewLC
// --------------------------------------------------------------------------
//
CWsfConnMonActiveWrapper* CWsfConnMonActiveWrapper::NewLC( 
        MWsfModelObserver &aModelObserver )
    {
    LOG_ENTERFN( "CWsfConnMonActiveWrapper::NewLC" );
    CWsfConnMonActiveWrapper* self =
            new (ELeave) CWsfConnMonActiveWrapper( aModelObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }


// --------------------------------------------------------------------------
// CWsfConnMonActiveWrapper::NewL
// --------------------------------------------------------------------------
//
CWsfConnMonActiveWrapper* CWsfConnMonActiveWrapper::NewL( 
        MWsfModelObserver &aModelObserver )
    {
    LOG_ENTERFN( "CWsfConnMonActiveWrapper::NewL" );
    CWsfConnMonActiveWrapper* self = 
            CWsfConnMonActiveWrapper::NewLC( aModelObserver );
    CleanupStack::Pop(); // self;
    return self;
    }


// --------------------------------------------------------------------------
// CWsfConnMonActiveWrapper::ConstructL
// --------------------------------------------------------------------------
//
void CWsfConnMonActiveWrapper::ConstructL()
    {
    LOG_ENTERFN( "CWsfConnMonActiveWrapper::ConstructL" );
    CActiveScheduler::Add(this); // Add to scheduler
    iConnectionMonitor.ConnectL();
    }


// --------------------------------------------------------------------------
// CWsfConnMonActiveWrapper::~CWsfConnMonActiveWrapper
// --------------------------------------------------------------------------
//
CWsfConnMonActiveWrapper::~CWsfConnMonActiveWrapper()
    {
    LOG_ENTERFN( 
            "CWsfConnMonActiveWrapper::~CWsfConnMonActiveWrapper" );
    iConnectionMonitor.CancelAsyncRequest( iCancelCode );
    Cancel(); // Cancel any request, if outstanding
    iConnectionMonitor.Close();
    // Delete instance variables if any
    }


// --------------------------------------------------------------------------
// CWsfConnMonActiveWrapper::DoCancel
// --------------------------------------------------------------------------
//
void CWsfConnMonActiveWrapper::DoCancel()
    {
    LOG_ENTERFN( "CWsfConnMonActiveWrapper::DoCancel" );
    }


// --------------------------------------------------------------------------
// CWsfConnMonActiveWrapper::StartL
// --------------------------------------------------------------------------
//
void CWsfConnMonActiveWrapper::Start()
    {
    LOG_ENTERFN( "CWsfConnMonActiveWrapper::Start" );
    Cancel(); // Cancel any request, just to be sure
    iState = ECountingconnections;
    LOG_WRITEF( "iState = %d", iState );
    iCancelCode = EConnMonGetConnectionCount;
    iConnectionMonitor.GetConnectionCount( iConnectionCount, iStatus );
    LOG_WRITEF( "iStatus = %d", iStatus.Int() );
    SetActive();
    LOG_WRITEF( "EConnMonGetConnectionCount = %d", EConnMonGetConnectionCount );
    }


// --------------------------------------------------------------------------
// CWsfConnMonActiveWrapper::RunL
// --------------------------------------------------------------------------
//
void CWsfConnMonActiveWrapper::RunL()
    {
    LOG_ENTERFN( "CWsfConnMonActiveWrapper::RunL" );
    LOG_WRITEF( "iState = %d", iState );
    if ( iStatus == KErrNone )
        {
        TUint subConnectionCount( 0 );
        
        switch ( iState )
            {
            case ECountingconnections:
                {
                iState = ECheckingConnections;
                if ( iConnectionCount > 0 )
                    {
                    iConnectionMonitor.GetConnectionInfo( iConnectionCount, 
                                                         iConnectionId, 
                                                         subConnectionCount );
                    iConnectionCount--;
                    
                    iCancelCode = EConnMonGetIntAttribute;
                    iConnectionMonitor.GetIntAttribute( iConnectionId, 
                                                       0, 
                                                       KBearer, 
                                                       ( TInt& ) iBearerType, 
                                                       iStatus );
                    SetActive();
                    }
                }
                break;
            case ECheckingConnections: 
                {
                if ( iBearerType == EBearerWLAN )
                    {
                    iState = EConnectionsChecked;
                    }
                else if ( iConnectionCount > 0 )
                    {
                    iConnectionMonitor.GetConnectionInfo( iConnectionCount, 
                                                         iConnectionId, 
                                                         subConnectionCount );
                    iConnectionCount--;
                    
                    iCancelCode = EConnMonGetIntAttribute;
                    iConnectionMonitor.GetIntAttribute( iConnectionId, 
                                                       0, 
                                                       KBearer, 
                                                       ( TInt& ) iBearerType, 
                                                       iStatus );
                    SetActive(); 
                    break;
                    }
                }
            case EConnectionsChecked:
                {
                LOG_WRITEF( "iConnectionId: %d", iConnectionId );
                iState = EReady;
                iCancelCode = EConnMonGetPckgAttribute;    
                iConnectionMonitor.GetPckgAttribute( iConnectionId,
                                                    0,
                                                    KClientInfo,
                                                    iClientBuf,
                                                    iStatus );
                SetActive();
                }
                break;
            case EReady: 
                {
                TBool found ( EFalse );
                for ( TUint i = 0; i < iClientBuf().iCount; i++ )
                    {
                    if ( iClientBuf().iUid[i].iUid == KBrowserUid )
                        {
                        found = ETrue;
                        iModelObserver.HandleMskIfBrowsingL();
                        break;
                        }
                    }
                if ( !found )
                    {
                    iModelObserver.HandleMskIfConnectedL();
                    }
                }
                break;
            default:
                {
                LOG_WRITEF( "ConnMonActiveWrapper default iState = %d", iState );
                //Do nothing
                }
                break;
            }
        }
    else
        {
        LOG_WRITEF( "ConnMonActiveWrapper iStatus = %d", iStatus.Int() );
        // Do nothing
        }
    }


// --------------------------------------------------------------------------
// CWsfConnMonActiveWrapper::RunError
// --------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CWsfConnMonActiveWrapper::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfConnMonActiveWrapper::RunError" );
    LOG_WRITEF( "aError = %d", aError );
    TRAP_IGNORE( iModelObserver.HandleMskIfConnectedL() );
    return aError;
    }
#else
TInt CWsfConnMonActiveWrapper::RunError( TInt /*aError*/ )
    {
    TRAP_IGNORE( iModelObserver.HandleMskIfConnectedL() );
    return KErrNone;
    }
#endif



