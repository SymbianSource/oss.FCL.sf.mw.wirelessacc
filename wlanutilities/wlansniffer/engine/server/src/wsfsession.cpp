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
* Description:  Implementation of CWsfSession
*
*/


//  CLASS HEADER
#include "wsfsession.h"
#include "wsfserver.h"
#include "wsfengine.h"
#include "wsfservercommon.h"
#include "wsfwlaninfo.h"
#include "wsflogger.h"


// ----------------------------------------------------------------------------
// CWsfSession::CWsfSession
// ----------------------------------------------------------------------------
//
CWsfSession::CWsfSession( CWsfServer& aServer ): 
    iServer( aServer ), 
    iRequestNotify( EFalse ),
    iEnabledScanning( EFalse )
    {
    }


// ----------------------------------------------------------------------------
// CWsfSession::NewL
// ----------------------------------------------------------------------------
//
CWsfSession* CWsfSession::NewL( CWsfServer& aServer )
    {
    CWsfSession* thisPtr = new (ELeave) CWsfSession( aServer );
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    CleanupStack::Pop( thisPtr );
    return thisPtr; 
    }

    
// ----------------------------------------------------------------------------
// CWsfSession::~CWsfSession
// ----------------------------------------------------------------------------
//
CWsfSession::~CWsfSession()
    {
    LOG_ENTERFN( "CWsfSession::~CWsfSession" );
    CancelPendingTasks();
    iServer.Engine()->Detach( this );
    iServer.ClientRelease();
    iEventQueue.Close();
    }


// ----------------------------------------------------------------------------
// CWsfSession::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfSession::ConstructL()
    {
    iServer.ClientAttach();
    iServer.Engine()->AttachL( this );
    iEnabledScanning = iServer.Engine()->IsScanning();
    }


// ----------------------------------------------------------------------------
// CWsfSession::ServiceL
// ----------------------------------------------------------------------------
//
void CWsfSession::ServiceL( const RMessage2& aMessage )
    {
    TRAPD( err, DispatchMessageL( aMessage ) );
    if ( err != KErrNone )
        {
        LOG_WRITEF( "CWsfSession::ServiceL DispatchMessageL leave error = %d", 
                    err );
        aMessage.Complete( err );
        }
    }


// ----------------------------------------------------------------------------
// CWsfSession::DispatchMessageL
// ----------------------------------------------------------------------------
//
void CWsfSession::DispatchMessageL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::DispatchMessageL" );
    LOG_WRITEF( "CWsfSession::DispatchMessageL message = %d", 
                aMessage.Function() );
    
    switch ( aMessage.Function() )
        {
        case ESnifferCmdWlanDataSize:
            {
            ReplyWlanDataSizeL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }

        case ESnifferCmdNotifyEvents:
            {
            RequestNotifyL( aMessage );    
            break;
            }

        case ESnifferCmdCancelNotifyEvents:
            {
            CancelNotifyEvents();    
            aMessage.Complete( KErrNone );
            break;
            }

        case ESnifferCmdGetWlanInfo:
            {
            WriteWlanDataL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }    

        case ESnifferCmdConnect:
            {
            ConnectWlanL( aMessage );
            aMessage.Complete( KErrNone );    
            break;
            }

        case ESnifferCmdDisconnect:
            {
            DisconnectWlanL( aMessage );
            aMessage.Complete( KErrNone );    
            break;
            }

        case ESnifferCmdActivateScan:
            {
            ActivateScanL();    
            aMessage.Complete( KErrNone );
            break;
            }

        case ESnifferCmdDisableScan:
            {
            DisableScanL();
            aMessage.Complete( KErrNone ); 
            break;
            }

        case ESnifferCmdConnectionName:
            {
            GetConnectedAccountNameL( aMessage );          
            aMessage.Complete( KErrNone );    
            break;     
            }

        case ESnifferCmdCancelAll:
            {
            CancelPendingTasks();
            aMessage.Complete( KErrNone );
            break;
            }             

        case ESnifferCmdRequestScan:
            {
            RequestScanL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }

        case ESnifferCmdMonitorAp:
            {
            MonitorApL( aMessage );
            aMessage.Complete( KErrNone );                
            break;    
            }

        case ESnifferCmdIsScanEnabled:
            {
            ReplyIsScanningL(aMessage); 
            aMessage.Complete( KErrNone );    
            break;
            }
        
        case ESnifferCmdIsConnected:
            {
            IsConnectedL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }
            
        case ESnifferCmdConnectedDetails:
            {
            GetConnectedDetailsL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }
            
        case ESnifferCmdAbortScanning:
            {
            AbortScanning();
            aMessage.Complete( KErrNone );
            break;
            }
            
        case ESnifferCmdAbortConnecting:
            {
            AbortConnectingL();
            aMessage.Complete( KErrNone );
            break;
            }
            
        case ESnifferCmdSetIapPersistence:
            {
            SetIapPersistenceL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }
            
        case ESnifferCmdControlDisconnectTimer:
            {
            ControlDisconnectTimerL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }
            
       
        // requests we don't understand at all are a different thing,
        // so panic the client here, this function also completes the message
        default:
            {
            _LIT( KWsfServerPanic, "WlanSnifferServer" );
            aMessage.Panic( KWsfServerPanic, 1 );
            }
            
        }
    }


// ---------------------------------------------------------------------------
// CWsfSession::RequestNotify
// ---------------------------------------------------------------------------
//        
void CWsfSession::RequestNotifyL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::RequestNotifyL" );
    if ( !iRequestNotify )
        {
        LOG_WRITE( "CWsfSession::RequestNotifyL first subscription" );
        // first subscription
        iRequestNotify = ETrue;
        iEventQueue.Reset();
        }
        
    // initial value / processing finished
    iProcessingEvent = EFalse;
    
    // store the message to be completed
    iNotifyChangesMessage = aMessage;
    
    // deal with the queue
    if ( iEventQueue.Count() )
        {
        LOG_WRITE( "CWsfSession::RequestNotifyL queue not empty" );
        NotifyClientL();
        }
    }
    
    
// ---------------------------------------------------------------------------
// CWsfSession::CancelNotifyEvents
// ---------------------------------------------------------------------------
//        
void CWsfSession::CancelNotifyEvents()
    {
    LOG_ENTERFN( "CWsfSession::CancelNotifyEvents" );
    
    // interrupt ongoing event handling
    iProcessingEvent = EFalse;
    if ( !iNotifyChangesMessage.IsNull() )
        {
        iNotifyChangesMessage.Complete( KErrCancel );
        }

    // unsubscribe
    iRequestNotify = EFalse;
    
    // drop all waiting events
    iEventQueue.Reset();
    }


// ---------------------------------------------------------------------------
// CWsfSession::CancelPendingTasks
// ---------------------------------------------------------------------------
//        
void CWsfSession::CancelPendingTasks()
    {
    LOG_ENTERFN( "CWsfSession::CancelPendingTasks" );
    if ( iEnabledScanning )
        {
        TRAP_IGNORE( iServer.Engine()->DisableScanningL() );
        }

    CancelNotifyEvents();
    }

    

// from MWsfEngineObserver

// ---------------------------------------------------------------------------
// CWsfSession::WlanListChangedL
// ---------------------------------------------------------------------------
//    
void CWsfSession::WlanListChangedL()
    {    
    LOG_ENTERFN( "CWsfSession::WlanListChangedL" );
    TWsfNotifyEventContainer event = { EEngineWlanDataChanged, KErrNone };
    iEventQueue.AppendL( event );

    NotifyClientL();
    }

    
// ---------------------------------------------------------------------------
// CWsfSession::NotifyErrorL
// ---------------------------------------------------------------------------
//    
void CWsfSession::NotifyErrorL( TInt aError )
    {
    LOG_ENTERFN( "CWsfSession::NotifyErrorL" );
    TWsfNotifyEventContainer event = { EEngineError, aError };
    iEventQueue.AppendL( event );

    NotifyClientL();
    }
    

// ---------------------------------------------------------------------------
// CWsfSession::ScanEnabledL
// ---------------------------------------------------------------------------
//    
void CWsfSession::ScanEnabledL()
    {
    LOG_ENTERFN( "CWsfSession::ScanEnabledL" );
    iEnabledScanning = ETrue;
    TWsfNotifyEventContainer event = { EEngineScanEnabled, KErrNone };
    iEventQueue.AppendL( event );

    NotifyClientL();
    }
    

// ---------------------------------------------------------------------------
// CWsfSession::ScanDisabledL
// ---------------------------------------------------------------------------
//    
void CWsfSession::ScanDisabledL()
    {
    LOG_ENTERFN( "CWsfSession::ScanDisabledL" );
    iEnabledScanning = EFalse;
    TWsfNotifyEventContainer event = { EEngineScanDisabled, KErrNone };
    iEventQueue.AppendL( event );

    NotifyClientL();
    }
    

// ---------------------------------------------------------------------------
// CWsfSession::ConnectedL
// ---------------------------------------------------------------------------
//    
void CWsfSession::ConnectedL()
    {
    LOG_ENTERFN( "CWsfSession::ConnectedL" );
    TWsfNotifyEventContainer event = { EEngineConnected, KErrNone };
    iEventQueue.AppendL( event );

    NotifyClientL();
    }
    

// ---------------------------------------------------------------------------
// CWsfSession::DisconnectedL
// ---------------------------------------------------------------------------
//    
void CWsfSession::DisconnectedL()
    {
    LOG_ENTERFN( "CWsfSession::DisconnectedL" );
    TWsfNotifyEventContainer event = { EEngineDisconnected, KErrNone };
    iEventQueue.AppendL( event );
    
    NotifyClientL();
    }


// ---------------------------------------------------------------------------
// CWsfSession::ConnectingFailedL
// ---------------------------------------------------------------------------
//    
void CWsfSession::ConnectingFailedL( TInt aError )
    {
    LOG_ENTERFN( "CWsfSession::ConnectingFailedL" );
    TWsfNotifyEventContainer event = { EEngineConnectingFailed, aError };
    iEventQueue.AppendL( event );
    
    NotifyClientL();
    }
    

// ---------------------------------------------------------------------------
// CWsfSession::NotifyClientL
// ---------------------------------------------------------------------------
//    
void CWsfSession::NotifyClientL()
    {
    LOG_ENTERFN( "CWsfSession::NotifyClientL" );

    if ( iRequestNotify && !iProcessingEvent )
        {
        LOG_WRITE( "CWsfSession::NotifyClientL notify" );
        TPckgBuf<TWsfNotifyEventContainer> container( iEventQueue[0] );

        iNotifyChangesMessage.WriteL( 0, container );

        // remove the item that we have just handed over
        iEventQueue.Remove( 0 );

        iProcessingEvent = ETrue;
        iNotifyChangesMessage.Complete( KErrNone );
        }
    }
    

// ----------------------------------------------------------------------------
// CWsfSession::GetConnectedAccountNameL
// ----------------------------------------------------------------------------
//
void CWsfSession::GetConnectedAccountNameL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::GetConnectedAccountNameL" );    
    HBufC* iapName = iServer.Engine()->ConnectedWlanSsidL();
    CleanupStack::PushL( iapName );
    TPckgBuf<TWsfQueryConnectedAccount> package;

    TInt maxLen = package().iConnectedAccountName.MaxLength();

    if ( iapName->Length() )
        {
        if ( iapName->Length() > maxLen )
            {
            package().iConnectedAccountName.Copy( iapName->Ptr(), maxLen );    
            }
        else
            {
            package().iConnectedAccountName.Copy( *iapName );
            }
        package().iConnected = ETrue;
        }
    else
        {
        package().iConnected = EFalse;
        }    
    aMessage.WriteL( 0, package );
    CleanupStack::PopAndDestroy( iapName );        
    }


// ----------------------------------------------------------------------------
// CWsfSession::GetConnectedDetailsL
// ----------------------------------------------------------------------------
//
void CWsfSession::GetConnectedDetailsL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::GetConnectedDetailsL" );    
    TPckgBuf<TWsfWlanInfo> package;
    TPckgBuf<TBool> ret( ETrue );
    
    if ( !iServer.Engine()->ConnectedWlanConnectionDetailsL( package() ) )
        {
        LOG_WRITE( "engine returned false, so wlaninfo is marked as invalid" );
        ret() = EFalse;
        }
    
    aMessage.WriteL( 0, ret );
    aMessage.WriteL( 1, package );
    }
    
    
// ----------------------------------------------------------------------------
// CWsfSession::ReplyWlanDataSizeL
// ----------------------------------------------------------------------------
//
void CWsfSession::ReplyWlanDataSizeL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::ReplyWlanDataSizeL" );
    const TInt KInvalidLength = 0;
    TPckgBuf<TUint> p( KInvalidLength );

    HBufC8* results = iServer.Engine()->GetScanResults();
    if ( results )
        {
        p() = results->Length();
        }
    
    aMessage.WriteL( 0, p );
    }
    

// ----------------------------------------------------------------------------
// CWsfSession::WriteWlanDataL
// ----------------------------------------------------------------------------
//
void CWsfSession::WriteWlanDataL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::WriteWlanDataL" );
    TPckgBuf<TUint> sizeData;
    
    // don't take the ownership
    HBufC8* scanData = iServer.Engine()->GetScanResults();
        
    if ( scanData )
        {
        LOG_WRITEF( "scanData->Length() = %d aMessage.GetDesMaxLength(1) = %d", 
					scanData->Length(), aMessage.GetDesMaxLength( 1 ) );
        if ( scanData->Length() > aMessage.GetDesMaxLength( 1 ) )
            {
            // the data has changed
            // return error to client to alloc more buffer
            sizeData() = scanData->Length();
            aMessage.WriteL( 0, sizeData );
            }
        else
            {
             // else we have enough buffer... copy the data
             sizeData() = scanData->Length();
             aMessage.WriteL( 0, sizeData );
             aMessage.WriteL( 1, *scanData, 0 );
            }
        }
    }
    

// ----------------------------------------------------------------------------
// CWsfSession::ActivateScanL
// ----------------------------------------------------------------------------
//
void CWsfSession::ActivateScanL()
    {
    LOG_ENTERFN( "CWsfSession::ActivateScanL" );    
    iServer.Engine()->EnableScanningL();
    iEnabledScanning = ETrue;
    }
    

// ----------------------------------------------------------------------------
// CWsfSession::DisableScanL
// ----------------------------------------------------------------------------
//
void CWsfSession::DisableScanL()
    {
    LOG_ENTERFN( "CWsfSession::DisableScanL" );    
    if ( iEnabledScanning )
        {
        iServer.Engine()->DisableScanningL();
        iEnabledScanning = EFalse;
        }
    }


// ----------------------------------------------------------------------------
// CWsfSession::ConnectWlanL
// ----------------------------------------------------------------------------
//
void CWsfSession::ConnectWlanL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::ConnectWlanL" );    
    TUint32 iapId( aMessage.Int1() );
    TWsfIapPersistence persistence( TWsfIapPersistence( aMessage.Int2() ) );
    TPckgBuf<TInt> p( iServer.Engine()->ConnectWlanL( iapId, persistence ) );
    aMessage.WriteL( 0, p );
    }
    

// ----------------------------------------------------------------------------
// CWsfSession::DisconnectWlanL
// ----------------------------------------------------------------------------
//
void CWsfSession::DisconnectWlanL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::DisconnectWlanL" );    
    TPckgBuf<TInt> p( iServer.Engine()->DisconnectWlan() );
    aMessage.WriteL( 0, p );
    }
    

// ----------------------------------------------------------------------------
// CWsfSession::RequestScanL
// ----------------------------------------------------------------------------
//
void CWsfSession::RequestScanL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::RequestScanL" );    
    TPckgBuf<TBool> requestMade( EFalse );
    // check if the client has enabled scanning - if not do not request scans
    if ( !iEnabledScanning )
        {
        LOG_WRITE( "CWsfSession::RequestScanL error, not scanning" );
        aMessage.WriteL( 0, requestMade );
        return;
        }
    
    requestMade = iServer.Engine()->RefreshScanResults();
    aMessage.WriteL( 0, requestMade );
    }
    

// ----------------------------------------------------------------------------
// CWsfSession::MonitorApL
// ----------------------------------------------------------------------------
//
void CWsfSession::MonitorApL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::MonitorApL" );    
    TUint32 accessPoint( (TUint32) aMessage.Int0() );
    iServer.Engine()->MonitorAccessPointL( accessPoint );
    }


// ----------------------------------------------------------------------------
// CWsfSession::ReplyIsScanningL
// ----------------------------------------------------------------------------
//
void CWsfSession::ReplyIsScanningL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::ReplyIsScanningL" );    
    TPckgBuf<TBool> p( iServer.Engine()->IsScanning() );
    aMessage.WriteL( 0, p );
    }


// ----------------------------------------------------------------------------
// CWsfSession::IsConnectedL
// ----------------------------------------------------------------------------
//
void CWsfSession::IsConnectedL( const RMessage2& aMessage )
    {
    TBool b = iServer.Engine()->IsConnected();
    TPckgBuf<TBool> p( b );
    aMessage.WriteL( 0, p );
    }


// ----------------------------------------------------------------------------
// CWsfSession::AbortScanning
// ----------------------------------------------------------------------------
//
void CWsfSession::AbortScanning()
    {
    LOG_ENTERFN( "CWsfSession::AbortScanningL" );    
    iServer.Engine()->AbortScanning();
    }
    

// ----------------------------------------------------------------------------
// CWsfSession::AbortConnectingL
// ----------------------------------------------------------------------------
//
void CWsfSession::AbortConnectingL()
    {
    LOG_ENTERFN( "CWsfSession::AbortConnectingL" );    
    iServer.Engine()->AbortConnectingL();
    }


// ----------------------------------------------------------------------------
// CWsfSession::SetIapPersistenceL
// ----------------------------------------------------------------------------
//
void CWsfSession::SetIapPersistenceL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::SetIapPersistenceL" );    
    
    TWsfIapPersistence persistence( TWsfIapPersistence( aMessage.Int1() ) );
    TPckgBuf<TBool> ret( iServer.Engine()->SetIapPersistenceL( persistence ) );
    
    aMessage.WriteL( 0, ret );
    }
    

// ----------------------------------------------------------------------------
// CWsfSession::ControlDisconnectTimerL
// ----------------------------------------------------------------------------
//
void CWsfSession::ControlDisconnectTimerL( const RMessage2& aMessage )
    {
    LOG_ENTERFN( "CWsfSession::ControlDisconnectTimerL" );

    TPckgBuf<TBool> ret( iServer.Engine()->ControlDisconnectTimer( 
                                                 TUint( aMessage.Int1() ) ) );
    
    aMessage.WriteL( 0, ret );
    }


