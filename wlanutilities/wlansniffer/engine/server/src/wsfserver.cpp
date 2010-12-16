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
* Description:  Implementation of CWsfServer
*
*/


//  CLASS HEADER
#include "wsfserver.h"

//  INTERNAL INCLUDES
#include "wsfengine.h"
#include "wsfsession.h"
#include "wsfservercommon.h"
#include "wsflogger.h"


//  LOCAL DEFINITIONS
/**
* Number of security ranges
*/
static const TInt KRangeCount = 1;


/**
* Security ranges
*/
static const TInt KSecurityRanges[KRangeCount] = 
	{
	0,
	};


/**
* Policy for the security ranges
*/	
static const TUint8 KSecurityRangesPolicy[KRangeCount] =
	{
	CPolicyServer::ECustomCheck
	};


/**
* The complete policy structure
*/
static const CPolicyServer::TPolicy KPolicy =
	{
	CPolicyServer::EAlwaysPass,
	KRangeCount,
	KSecurityRanges,
	KSecurityRangesPolicy,
	NULL,
	};
	

/**
* Secure ID of the Active Idle (ailaunch.exe)
*/
static _LIT_SECURE_ID( KSidActiveIdle, 0x102750F0 );

	
	
// ---------------------------------------------------------------------------
// CWsfServer::NewLC
// ---------------------------------------------------------------------------
//
CWsfServer* CWsfServer::NewLC()
	{
	CWsfServer* snifferServer = new (ELeave) CWsfServer( 
	                                                  CActive::EPriorityHigh );
	CleanupStack::PushL( snifferServer );
	snifferServer->ConstructL();
	snifferServer->StartL( KWlanSnifferServerName );
	
	return snifferServer;
	}


// ---------------------------------------------------------------------------
// CWsfServer::NewSessionL
// ---------------------------------------------------------------------------
//
CSession2* CWsfServer::NewSessionL( const TVersion& aVersion, 
                                    const RMessage2& /*aMessage*/) const
	{
	LOG_ENTERFN( "CWsfServer::NewSessionL" );
	// check we're the right version
	TVersion v( KWlanSnifferServerMajor, 
	            KWlanSnifferServerMinor, 
	            KWlanSnifferServerBuild );
	
	if ( !User::QueryVersionSupported( v, aVersion ) )
	    {
	    User::Leave( KErrNotSupported );
	    }
	
	// make new session
	CSession2* newSession = CWsfSession::NewL( 
	                                        const_cast<CWsfServer&>( *this ) );
	
	return newSession;
	}
	
	
// ---------------------------------------------------------------------------
// CWsfServer::CWsfServer
// ---------------------------------------------------------------------------
//
CWsfServer::CWsfServer( TInt aPriority ): 
    CPolicyServer( aPriority, KPolicy ),
	iClientCount( 0 )
	{
	}


// ---------------------------------------------------------------------------
// CWsfServer::~CWsfServer
// ---------------------------------------------------------------------------
//
CWsfServer::~CWsfServer()
	{
	delete iCloser;
	delete iEngine;
	}	


// ---------------------------------------------------------------------------
// CWsfServer::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfServer::ConstructL()
	{
	iCloser = CWsfServerCloser::NewL();

	iEngine = CWsfEngine::NewL( *iCloser );

	// start the closing up procedure
	// so we won't be left running if no clients would appear
	iCloser->WaitForClients( EFalse );
	
	}


// ---------------------------------------------------------------------------
// CWsfServer::ClientRelease
// ---------------------------------------------------------------------------
//
void CWsfServer::ClientRelease()
	{
	LOG_ENTERFN( "CWsfServer::ClientRelease" );
	--iClientCount;
	
	// even if there are no clients, we might need to keep the server alive
	// to maintain the connection or clean up IAPs properly
	if ( !iClientCount )
		{
		
		// The last client is released.
        // Iap deletion must not suppress anymore. Make sure the flag is off.
        iEngine->ResetSuppressIapDeletion();
        
		// From this point on, the server is kept alive to wait for connection notifications if necessary. 
		// E.g. browser could be launched by sniffer and WLAN connection still exists after sniffer shut down, 
		// server waits for connection lost callback and cleans up temporary IAPs when connection is closed. 
		
	    // If no connections are available, make sure the the server is NOT kept alive to wait
	    // browser exit, because the server is not set to monitor those notifications directly. 
		if( !iEngine->IsConnected() )
		    {
		    iCloser->WaitForBrowserExit( EFalse );
		    }
		
		iCloser->WaitForClients( EFalse );
		}
	}


// ---------------------------------------------------------------------------
// CWsfServer::ClientAttach
// ---------------------------------------------------------------------------
//
void CWsfServer::ClientAttach()
	{
	LOG_ENTERFN( "CWsfServer::ClientAttach" );
	++iClientCount;
	iCloser->WaitForClients( ETrue );
	}


// ---------------------------------------------------------------------------
// CWsfServer::Engine
// ---------------------------------------------------------------------------
//
CWsfEngine* CWsfServer::Engine()
	{
	return iEngine;
	}	
	

// ---------------------------------------------------------------------------
// CWsfServer::RunServer
// ---------------------------------------------------------------------------
//
TInt CWsfServer::RunServer()
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt ret = KErrNoMemory;
	if ( cleanup )
		{
		TRAP( ret, CWsfServer::RunServerL() );
		delete cleanup;
		}

	if ( ret != KErrNone )
		{
		// Signal the client that server creation failed
		RProcess::Rendezvous( ret );
		}

	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CWsfServer::RunServerL
// ---------------------------------------------------------------------------
//
void CWsfServer::RunServerL()
	{
	// Create and install the active scheduler we need
	CActiveScheduler *as = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( as );
	CActiveScheduler::Install( as );

	// Create server
	CWsfServer::NewLC();

	// Initialisation complete, now signal the client
	User::LeaveIfError( RThread().RenameMe( KWlanSnifferServerName ) );
	RProcess::Rendezvous( KErrNone );

	// Ready to run
	CActiveScheduler::Start();

	// Cleanup the server and scheduler
	CleanupStack::PopAndDestroy( 2, as );
	}


// ---------------------------------------------------------------------------
// CWsfServer::RunError
// ---------------------------------------------------------------------------
//
TInt CWsfServer::RunError( TInt /*aError*/ )
	{
	return KErrNone;
	}
	

// ---------------------------------------------------------------------------
// CWsfServer::E32Main
// ---------------------------------------------------------------------------
//
TInt E32Main()
	{
	TInt error( KErrNone );
	error = CWsfServer::RunServer();
	return error;
	}

	
// ---------------------------------------------------------------------------
// CWsfServer::CustomSecurityCheckL
// ---------------------------------------------------------------------------
//
CPolicyServer::TCustomResult CWsfServer::CustomSecurityCheckL( 
                                                const RMessage2& aMsg, 
                                                TInt& /*aAction*/, 
                                                TSecurityInfo& /*aMissing*/ )
	{	
	TCustomResult ret = EFail;

	switch ( aMsg.Function() )
        {
        // NULL CAPABILITIES
        case ESnifferCmdNotifyEvents:
        case ESnifferCmdCancelNotifyEvents:
        case ESnifferCmdIsScanEnabled:
        case ESnifferCmdCancelAll:
        case ESnifferCmdConnectionName:	
        case ESnifferCmdIsConnected:
        case ESnifferCmdMonitorAp:
            {
        	ret = EPass;
        	break;
            }        	
        	
        // CAPABILITY NEEDED
        case ESnifferCmdActivateScan:
        case ESnifferCmdDisableScan:
        case ESnifferCmdWlanDataSize:
        case ESnifferCmdGetWlanInfo:
        case ESnifferCmdConnectedDetails:
        case ESnifferCmdConnect:
        case ESnifferCmdDisconnect:
        case ESnifferCmdRequestScan:
        case ESnifferCmdAbortScanning:
        case ESnifferCmdAbortConnecting:
        case ESnifferCmdSetIapPersistence:
        case ESnifferCmdControlDisconnectTimer:
            {
        	// if it is called from ailaunch.exe
        	// disgusting workaround
            if ( aMsg.SecureId() == KSidActiveIdle )
            	{
            	ret = EPass;
            	}
            else
            	{
           		ret = ( aMsg.HasCapability( ECapabilityNetworkServices ) && 
           		        aMsg.HasCapability( ECapabilityNetworkControl ) )? 
        			        EPass : EFail;
        		}
        	break;
            }

        default:
            {
        	ret = EPass;
        	break;
            }

        }
	
	
	return ret;
	}

