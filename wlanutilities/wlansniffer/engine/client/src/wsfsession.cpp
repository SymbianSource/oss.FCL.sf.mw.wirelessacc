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
* Description:  Implementation of RWsfSession
*
*/



//  CLASS HEADER
#include "wsfsession.h"

//  INTERNAL INCLUDES
#include "wsfwlaninfoarray.h"
#include "wsfeventhandler.h"
#include "wsfactivewaiter.h"
#include "wsfservercommon.h"
#include "wsflogger.h"



//  LOCAL DEFINITIONS
/**
* Number of message slots at the server
*/
static const TInt KIPCSlots = 6;

/**
* Number of retries to start the server
*/
static const TInt KRetries = 3;



// ---------------------------------------------------------------------------
// RWsfSession::RWsfSession
// ---------------------------------------------------------------------------
EXPORT_C RWsfSession::RWsfSession(): 
    RSessionBase(), 
    iEventHandler( 0 )
    {
    }


// ---------------------------------------------------------------------------
// RWsfSession::StartServer
// ---------------------------------------------------------------------------
TInt RWsfSession::StartServer()
    {
    LOG_ENTERFN( "RWsfSession::StartServerL" );
    
    TInt res( KErrNone );
    // create server - if one of this name does not already exist
    TFindServer findServer( KWlanSnifferServerName );
    TFullName name;

    if ( findServer.Next( name ) != KErrNone ) // we don't exist already
        {
        RProcess server;
        // Create the server process
        res = server.Create( KWlanSnifferServerExe, KNullDesC );

        if ( res != KErrNone ) // thread created ok - now start it going
            {
            return res;
            }

        // Rendezvous is used to detect server start
        TRequestStatus status;
        server.Rendezvous( status );                                                 
                                                                                   
        if ( status != KRequestPending )                                             
            {
            LOG_WRITEF( "StartServer Rendezvous ERROR: %d", status.Int() );
            server.Kill( 0 );    // abort startup                                 
            }                                                                      
        else                                                                       
            {                                                                      
            server.Resume();    // logon OK - start the server                     
            }                                                                      
        
        LOG_WRITE( "StartServer wait for start" );
        User::WaitForRequest( status ); // wait for start or death

        // We can't use the 'exit reason' if the server paniced as this                                                          
        // is the panic 'reason' and may be '0' which cannot be distinguished                                        
        // from KErrNone                                                                                             
        res = ( server.ExitType() == EExitPanic ) ? KErrGeneral : status.Int();
        
        // we're no longer interested in the other process
        server.Close(); 
        }
        
    return res;
    }


// ---------------------------------------------------------------------------
// RWsfSession::Connect
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RWsfSession::Connect()
    {
    LOG_ENTERFN( "RWsfSession::Connect" );
    TInt err = StartServer();
    
    for (TInt tries = 0; tries < KRetries; tries++)
        {
        LOG_WRITE( "server started" );

        err = CreateSession( KWlanSnifferServerName, Version(), KIPCSlots );

        LOG_WRITEF( "CreateSession() returned %d", err );
        
        if (!err)
            {
            LOG_WRITE( "connected" );
            break; 
            }
        if (err != KErrNotFound && err != KErrServerTerminated)
            {
            LOG_WRITE( "problems session creation - break" );
            break; 
            }
            
        TInt err = StartServer(); // try to launch the server

        if (!err)
            {
            LOG_WRITE( "ok - try to connect again" );
            continue; 
            }
            
        if (err == KErrAlreadyExists)
            {
            LOG_WRITE( "someone else started - try to connect again" );
            continue; 
            }
            
        LOG_WRITE( "problems - break" );
        break; 
        }
    
    LOG_WRITEF( "connect exit value = %d", err );

    return err;
    }


// ---------------------------------------------------------------------------
// RWsfSession::Version
// ---------------------------------------------------------------------------
//
TVersion RWsfSession::Version() const
    {
    return TVersion( KWlanSnifferServerMajor, 
                     KWlanSnifferServerMinor, 
                     KWlanSnifferServerBuild );
    }


// ---------------------------------------------------------------------------
// RWsfSession::CancelNotifyEvent
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::CancelNotifyEvent()
    {
    LOG_ENTERFN( "RWsfSession::CancelNotifyEvent" );
    if ( iEventHandler )
        {
        iEventHandler->Cancel();
        }
    }

// ---------------------------------------------------------------------------
// RWsfSession::GetConnectedWlanDetailsL
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RWsfSession::GetConnectedWlanDetailsL( TWsfWlanInfo& aWlanInfo )
    {
    LOG_ENTERFN("RWsfSession::GetConnectedWlanDetailsL");
    TPckgBuf<TBool> ret( EFalse );
    TPckg<TWsfWlanInfo> package( aWlanInfo );

    CWsfActiveWaiter *waiter = CWsfActiveWaiter::NewL();
    CleanupStack::PushL( waiter );

    SendReceive( ESnifferCmdConnectedDetails, TIpcArgs( &ret, &package ), 
                                                             waiter->iStatus );
    waiter->WaitForRequest();
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdConnectedDetails,
                waiter->iStatus.Int() );
    User::LeaveIfError( waiter->iStatus.Int() );
    CleanupStack::PopAndDestroy( waiter );

    return ret();
    }


// ---------------------------------------------------------------------------
// RWsfSession::GetConnectedWlanDetails
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::GetConnectedWlanDetails( TPckgBuf<TBool>& aPckg,
                                                    TWsfWlanInfo& aWlanInfo,
                                                    TRequestStatus& aStatus )
    {
    LOG_ENTERFN("RWsfSession::GetConnectedWlanDetails");
    TPckg<TWsfWlanInfo> package( aWlanInfo );

    SendReceive( ESnifferCmdConnectedDetails, TIpcArgs( &aPckg, &package ), 
                 aStatus );
    }


// ---------------------------------------------------------------------------
// RWsfSession::NotifyEventL
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::NotifyEventL( MWsfStateChangeObserver& aObserver )
    {
    LOG_ENTERFN("RWsfSession::NotifyEventL");
    if ( !iEventHandler )
        {
        iEventHandler = CWsfEventHandler::NewL( *this, aObserver );
        }
    iEventHandler->ResetObserver( aObserver );
    if ( !iEventHandler->IsActive() )
        {
        iEventHandler->Start();
        SendReceive( ESnifferCmdNotifyEvents, 
                     TIpcArgs( &iEventHandler->EventContainer() ), 
                     iEventHandler->iStatus );
        }
    }


// ---------------------------------------------------------------------------
// RWsfSession::EnableScanL
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RWsfSession::EnableScanL()
    {
    LOG_ENTERFN("RWsfSession::EnableScanL");

    TInt err = Send( ESnifferCmdActivateScan );
    LOG_WRITEF( "message[%d] call returned %d", ESnifferCmdActivateScan, err );
    
    User::LeaveIfError( err );
                                                         
    return !err;
    }
    

// ---------------------------------------------------------------------------
// RWsfSession::DisableScanL
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RWsfSession::DisableScanL()
    {
    LOG_ENTERFN("RWsfSession::DisableScanL");    

    TInt err = Send( ESnifferCmdDisableScan );
    LOG_WRITEF( "message[%d] call returned %d", ESnifferCmdDisableScan, err );

    User::LeaveIfError( err );

    return !err;
    }
    

// ---------------------------------------------------------------------------
// RWsfSession::IsScanEnabledL
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RWsfSession::IsScanEnabledL()
    {
    LOG_ENTERFN("RWsfSession::IsScanEnabledL");
    TBool res = 0;
    TPckgBuf<TInt> pckg;

    CWsfActiveWaiter *waiter = CWsfActiveWaiter::NewL();
    CleanupStack::PushL( waiter );
    
    SendReceive( ESnifferCmdIsScanEnabled, TIpcArgs( &pckg ), 
                                                            waiter->iStatus );
    waiter->WaitForRequest();
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdIsScanEnabled,
                waiter->iStatus.Int() );
    User::LeaveIfError( waiter->iStatus.Int() );
    CleanupStack::PopAndDestroy( waiter );
                                                         
    res = pckg();
    return res;
    }
    

// ---------------------------------------------------------------------------
// RWsfSession::UpdateWlanListL
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::UpdateWlanListL( CWsfWlanInfoArray* aInfoArray )
    {
    LOG_ENTERFN("RWsfSession::UpdateWlanListL");    
    // first request the buffer size for wlan data...
    TPckgBuf<TUint> pckg;
    TUint infoArrayElementCount = aInfoArray->Count();

    CWsfActiveWaiter *waiter = CWsfActiveWaiter::NewL();
    CleanupStack::PushL( waiter );

    SendReceive( ESnifferCmdWlanDataSize, TIpcArgs( &pckg ), waiter->iStatus );
    waiter->WaitForRequest();
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdWlanDataSize,
                waiter->iStatus.Int() );
    User::LeaveIfError( waiter->iStatus.Int() );

    // alloc the required size buffer...
    TInt requiredSize = pckg();
    LOG_WRITEF( "WLAN data buffer size = %d", requiredSize );

    if ( !requiredSize ) 
        {
        // no data ...
        CleanupStack::PopAndDestroy( waiter );
        return;
        }

    HBufC8* buffer = HBufC8::NewLC( requiredSize );
    TPtr8 ptr = buffer->Des();
    
    SendReceive( ESnifferCmdGetWlanInfo, TIpcArgs( &pckg, &ptr ), 
                                                            waiter->iStatus );
    waiter->WaitForRequest();
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdGetWlanInfo,
                waiter->iStatus.Int() );
    User::LeaveIfError( waiter->iStatus.Int() );
                                                    
    
    LOG_WRITEF( "actual bytes occupied = %d", pckg() );
    
    if ( requiredSize != pckg() )
        {
        // the buffer is not long enough... we must try again...
        User::Leave( KErrOverflow );
        }
    
    // Check that aInfoArray has not been modified 
    // while we waited server response
    if ( aInfoArray->Count() != infoArrayElementCount )
        {
        LOG_WRITEF( "aInfoArray data has changed %d != %d ", 
                aInfoArray->Count(), infoArrayElementCount );
        aInfoArray->Reset();
        }
    
    aInfoArray->AppendFromStreamBufferL( ptr );
    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy( waiter );
    }


// ---------------------------------------------------------------------------
// RWsfSession::GetWlanListSize
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::GetWlanListSize( TPckgBuf<TUint> &aPckg, TRequestStatus& aStatus )
    {
    LOG_ENTERFN("RWsfSession::GetWlanListSize");    
    SendReceive( ESnifferCmdWlanDataSize, TIpcArgs( &aPckg ), aStatus );
    }


// ---------------------------------------------------------------------------
// RWsfSession::GetWlanList
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::GetWlanList( TPckgBuf<TUint>& aPckg, TPtr8& aPtr, TRequestStatus& aStatus )
    {
    LOG_ENTERFN("RWsfSession::GetWlanList");    
    SendReceive( ESnifferCmdGetWlanInfo, TIpcArgs( &aPckg, &aPtr ), aStatus );
    }


// ---------------------------------------------------------------------------
// RWsfSession::MonitorAccessPointL
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::MonitorAccessPointL( TUint32 aMonitoredIap )
    {
    LOG_ENTERFN("RWsfSession::MonitorAccessPointL");

    TInt err = Send( ESnifferCmdMonitorAp, TIpcArgs( aMonitoredIap ));
    LOG_WRITEF( "message[%d] call returned %d", ESnifferCmdMonitorAp, err );
        
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// RWsfSession::RequestScanL
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RWsfSession::RequestScanL()
    {
    LOG_ENTERFN("RWsfSession::RequestScanL");
    TPckgBuf<TBool> pckg;
    
    CWsfActiveWaiter *waiter = CWsfActiveWaiter::NewL();
    CleanupStack::PushL( waiter );

    SendReceive( ESnifferCmdRequestScan, TIpcArgs( &pckg ), waiter->iStatus );
    waiter->WaitForRequest();
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdRequestScan,
                waiter->iStatus.Int() );
    User::LeaveIfError( waiter->iStatus.Int() );
    CleanupStack::PopAndDestroy( waiter );
    return pckg();
    }


// ---------------------------------------------------------------------------
// RWsfSession::RequestScan
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::RequestScan(TPckgBuf<TBool>& aPckg, TRequestStatus& aStatus )
    {
    LOG_ENTERFN("RWsfSession::RequestScan");
    SendReceive( ESnifferCmdRequestScan, TIpcArgs( &aPckg ), aStatus );
    }


// ---------------------------------------------------------------------------
// RWsfSession::ConnectWlanBearer
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::ConnectWlanBearer( TPckgBuf<TInt>& aPckg,
                                               TUint32 aIapId, 
                                               TBool aConnectOnly,
                                               TWsfIapPersistence aPersistence,
                                               TRequestStatus& aStatus )
    {
    LOG_ENTERFN( "RWsfSession::ConnectWlanBearer" );
    LOG_WRITEF( "IAP id = %d", aIapId );

    // mark the beginning of the connection process
    iEventHandler->UnBlockNextConnectedEvent();
    iEventHandler->SetConnecting( ETrue );

    SendReceive( ESnifferCmdConnect, 
                 TIpcArgs( &aPckg, aIapId, aConnectOnly, aPersistence ), 
                 aStatus );
    }


// ---------------------------------------------------------------------------
// RWsfSession::SetConnectWlanBearerResult
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::SetConnectWlanBearerResult( TInt aResult )
    {
    LOG_ENTERFN( "RWsfSession::SetConnectWlanBearerResult" );
    LOG_WRITEF( "Result = %d", aResult );
    
    if ( aResult != KErrNone )
        {
        // on any error the flag should be cleared
        iEventHandler->SetConnecting( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// RWsfSession::DisconnectWlanBearer
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::DisconnectWlanBearer( TPckgBuf<TBool>& aPckg, TRequestStatus& aStatus )
    {
    LOG_ENTERFN("RWsfSession::DisconnectWlanBearer");
    SendReceive( ESnifferCmdDisconnect, TIpcArgs( &aPckg ), aStatus );
    }


// ---------------------------------------------------------------------------
// RWsfSession::Close
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::Close()
    {
    LOG_ENTERFN("RWsfSession::Close");
    delete iEventHandler; iEventHandler = NULL;
    
    LOG_WRITE( "close session" );
    RSessionBase::Close();
    }


// ---------------------------------------------------------------------------
// RWsfSession::DoCancelNotifyEvent
// ---------------------------------------------------------------------------
//
void RWsfSession::DoCancelNotifyEvent()
    {
    LOG_ENTERFN("RWsfSession::DoCancelNotifyEvent");    
    // used only by CWsfEventHandler
    TInt err = Send( ESnifferCmdCancelNotifyEvents );
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdCancelNotifyEvents, err );
    }


// ---------------------------------------------------------------------------
// RWsfSession::CancelAll
// ---------------------------------------------------------------------------
//
void RWsfSession::CancelAll()
    {
    LOG_ENTERFN("RWsfSession::CancelAll");
    TInt err = Send( ESnifferCmdCancelAll );

    LOG_WRITEF( "message[%d] call returned %d", ESnifferCmdCancelAll, err );
    }
    

// ---------------------------------------------------------------------------
// RWsfSession::IsConnectedL
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RWsfSession::IsConnectedL()
    {
    LOG_ENTERFN("RWsfSession::IsConnectedL");
    TPckgBuf<TBool> pckg;

    CWsfActiveWaiter *waiter = CWsfActiveWaiter::NewL();
    CleanupStack::PushL( waiter );

    SendReceive( ESnifferCmdIsConnected, TIpcArgs( &pckg ), waiter->iStatus );
    waiter->WaitForRequest();
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdIsConnected,
                waiter->iStatus.Int() );
    User::LeaveIfError( waiter->iStatus.Int() );
    CleanupStack::PopAndDestroy( waiter );
    
    return pckg();
    }


// ---------------------------------------------------------------------------
// RWsfSession::RecoverFromServerTermination
// ---------------------------------------------------------------------------
//
TBool RWsfSession::RecoverFromServerTermination( 
                                           MWsfStateChangeObserver& aObserver )
    {
    RSessionBase::Close();
    
    TBool ret( EFalse );
    
    ret = ( Connect() == KErrNone );
    if ( ret )
        {
        TRAPD( err, NotifyEventL( aObserver ) );
        ret = ( err == KErrNone );
        }
        
    return ret;
    }


// ---------------------------------------------------------------------------
// RWsfSession::AbortConnectingL
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::AbortConnectingL()
    {
    LOG_ENTERFN("RWsfSession::AbortConnectingL");

    if ( iEventHandler->Connecting() )
        {
        iEventHandler->BlockNextConnectedEvent();

        TInt err = Send( ESnifferCmdAbortConnecting );
        LOG_WRITEF( "message[%d] call returned %d", 
                    ESnifferCmdAbortConnecting, err );
        
        User::LeaveIfError( err );
        }
    
    }
    

// ---------------------------------------------------------------------------
// RWsfSession::AbortScanningL
// ---------------------------------------------------------------------------
//
EXPORT_C void RWsfSession::AbortScanningL()
    {
    LOG_ENTERFN("RWsfSession::AbortScanningL");    

    TInt err = Send( ESnifferCmdAbortScanning );
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdAbortScanning, err );

    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// RWsfSession::SetIapPersistenceL
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RWsfSession::SetIapPersistenceL( 
                                              TWsfIapPersistence aPersistence )
    {
    LOG_ENTERFN("RWsfSession::SetIapPersistenceL");
    TPckgBuf<TBool> ret( EFalse );

    CWsfActiveWaiter *waiter = CWsfActiveWaiter::NewL();
    CleanupStack::PushL( waiter );

    SendReceive( ESnifferCmdSetIapPersistence, TIpcArgs( &ret, aPersistence ), 
                                                             waiter->iStatus );
    waiter->WaitForRequest();
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdSetIapPersistence,
                waiter->iStatus.Int() );
    User::LeaveIfError( waiter->iStatus.Int() );
    CleanupStack::PopAndDestroy( waiter );
    
    return ret();    
    }


// ---------------------------------------------------------------------------
// RWsfSession::ControlDisconnectTimerL
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RWsfSession::ControlDisconnectTimerL( TUint aAdcCommand )
    {
    LOG_ENTERFN("RWsfSession::ControlDisconnectTimerL"); 
    
    TPckgBuf<TBool> ret( EFalse );

    CWsfActiveWaiter *waiter = CWsfActiveWaiter::NewL();
    CleanupStack::PushL( waiter );

    SendReceive( ESnifferCmdControlDisconnectTimer, 
                 TIpcArgs( &ret, aAdcCommand ), 
                 waiter->iStatus );
                 
    waiter->WaitForRequest();
    LOG_WRITEF( "message[%d] call returned %d", 
                ESnifferCmdControlDisconnectTimer,
                waiter->iStatus.Int() );
    User::LeaveIfError( waiter->iStatus.Int() );
    CleanupStack::PopAndDestroy( waiter );
    
    return ret();    
    }


