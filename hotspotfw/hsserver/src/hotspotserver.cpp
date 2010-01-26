/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:       Implementation of HotSpot Plugin Server
*
*/






// INCLUDE FILES
#include <e32std.h>
#include <f32file.h>
#include <wlanmgmtclient.h>
#include <wlanmgmtinterface.h>
#include <e32std.h>
#include <commsdattypesv1_1.h>

#include <cmconnectionmethodext.h>
#include <cmconnectionmethoddef.h>
#include <cmpluginwlandef.h>
#include <cmmanagerext.h>
#include <cmmanagerdef.h>
#include <cmdestinationext.h>

#include "hotspotserver.h"
#include "am_debug.h"
#include "hotspotsession.h"
#include "hssclientplugin.h"
#include "hssiapsettingshandler.h"

// LOCAL FUNCTION PROTOTYPES
GLDEF_C TInt E32Main();

using namespace CMManager;

// CONSTANTS
// Panic codes for HotSpot server
const TInt KHotSpotPanicCleanupStackCreationFailed  = 1;
const TInt KHotSpotPanicOpenSemaforeFailed          = 2;
const TInt KHotSpotPanicCreationOfSchedulerFailed   = 3;

// Default timeout values

const TUint KMillion = 1000000;
const TUint KHssDefaultLoginTimeMicroSecs = 180 * KMillion;  // 180 seconds (3 mins)
const TUint KHssMinLoginTime =      5;   // 5 seconds
const TUint KHssMaxLoginTime =      1200; // 1200 seconds (20 mins)
const TUint KHssDefaultLogoutTimeMicroSecs  = 12 * KMillion;   // 12 seconds
const TUint KHssMinLogoutTime =     1;    // 1 seconds
const TUint KHssMaxLogoutTime =     30;   // 30 seconds

// Panic category of HotSpot server 
_LIT( KHotSpotModuleName, "HOTSPOTSRV" );

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RunServerL
// -----------------------------------------------------------------------------
// 
static void RunServerL()
    {
    User::LeaveIfError( User::RenameThread( KHotSpotServerName ) );

    CActiveScheduler* s=new(ELeave) CActiveScheduler;
    if ( !s )
        {
        User::Panic( KHotSpotModuleName, KHotSpotPanicCreationOfSchedulerFailed );
        }
    CleanupStack::PushL(s);
    CActiveScheduler::Install(s);

    CHotSpotServer::NewLC(); // Is NewLC: Server was pushed into cleanup stack.

    RSemaphore started;
    TInt err = started.CreateGlobal( KHotSpotServerSemaphore, 0 );
    if ( err != KErrNone )
        {
        err = started.OpenGlobal( KHotSpotServerSemaphore );
        }
    
    __ASSERT_ALWAYS(
        err == KErrNone, 
        User::Panic( KHotSpotModuleName, KHotSpotPanicOpenSemaforeFailed )
        );
        
    // lets everyone know that the thread is ready to deal with requests            
    RProcess::Rendezvous(KErrNone);
        
    started.Signal();
    started.Close();

    DEBUG("**** HotspotServer: server fully running");
    CActiveScheduler::Start();
    CleanupStack::PopAndDestroy( 2, s ); // Cleanup both server and scheduler
    REComSession::FinalClose();
    }

// -----------------------------------------------------------------------------
// PanicClient
// -----------------------------------------------------------------------------
//    
void PanicClient(const RMessagePtr2& aMessage,THotspotPanic aPanic)
    {
    _LIT(KPanic,"HotspotServer");
    aMessage.Panic(KPanic,aPanic);
    }

// ============================ MEMBER FUNCTIONS ===============================
    
// -----------------------------------------------------------------------------
// CHotSpotServer
// -----------------------------------------------------------------------------
//
CHotSpotServer::CHotSpotServer()
    :CPolicyServer( EPriorityStandard, THotSpotServerPlatSecPolicy, ESharableSessions )
    {
    }

// -----------------------------------------------------------------------------
// NewLC
// -----------------------------------------------------------------------------
//
CHotSpotServer* CHotSpotServer::NewLC()
    {
    DEBUG("**** HotSpotServer: CHotSpotServer::NewLC");
    CHotSpotServer* self = new(ELeave) CHotSpotServer;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// ~CHotSpotServer
// -----------------------------------------------------------------------------
//
CHotSpotServer::~CHotSpotServer()
    {
    DEBUG("**** HotSpotServer: CHotSpotServer::~CHotSpotServer");
    
    iMap.Close();
    iNotificationArray.Close();
    iLoginLogoutTimerArray.Close();
    
     if ( iMgtClient != NULL )
        {
#ifndef __WINS__    
        iMgtClient->CancelNotifications();
#endif
        delete iMgtClient;
        }
    }

// -----------------------------------------------------------------------------
// ConstructL
// -----------------------------------------------------------------------------
//
void CHotSpotServer::ConstructL()
    {
    DEBUG("**** HotSpotServer: CHotSpotServer::ConstructL");
    StartL( KHotSpotServerName );

    iIapCheckValue = EFalse;
    iLogoutSent = EFalse;
    iLoginValue = ETrue;
    iAssociationValue = EFalse;
    
    // Activate notifications for IAP check purposes. Done with every server startup.
    // When EWlanConnectionModeNotConnected is received we can cancel this and 
    // we know that it safe to go through IAPs.
    iMgtClient = CWlanMgmtClient::NewL();
#ifndef __WINS__
    iMgtClient->ActivateNotificationsL( *this );
#endif 
    }

// -----------------------------------------------------------------------------
// ConnectionStateChanged
// -----------------------------------------------------------------------------
//
void CHotSpotServer::ConnectionStateChanged( TWlanConnectionMode  aNewState ) 
    {
    DEBUG1( "CHotSpotServer::ConnectionStateChanged() aNewState=%d", aNewState );
    if ( aNewState == EWlanConnectionModeNotConnected )
        {
        if ( iMgtClient != NULL )
            {
#ifndef __WINS__
            iMgtClient->CancelNotifications();
#endif
            }
        
         TRAPD(err, CheckIapsL());
         if ( err != KErrNone )
             {
             DEBUG1("CHotSpotServer::ConnectionStateChanged(): %d", err);
             }
        }
    }

// -----------------------------------------------------------------------------
// CheckIapsL
// -----------------------------------------------------------------------------
//
void CHotSpotServer::CheckIapsL()
    {
    DEBUG("CHotSpotServer::CheckIapsL");
          
     _LIT(KMarkFirst, "[" );
     _LIT(KMarkLast, "]" );
     // This is needed to be checked only once per boot
    if ( iIapCheckValue == EFalse )
        {
        iIapCheckValue = ETrue;
        
        RCmManagerExt cmManager;
        cmManager.OpenL();
        CleanupClosePushL(cmManager);

        TBool supportedBearersOnly = ETrue;
        TBool legacyCmsOnly = EFalse;
        
        RArray<TUint32> cmArray;
        CleanupClosePushL( cmArray );
        
        cmManager.ConnectionMethodL( cmArray, supportedBearersOnly, legacyCmsOnly );
        DEBUG1("CHotSpotServer::CheckIapsL count: %d", cmArray.Count());  
        for( TInt i = 0; i < cmArray.Count(); i++ )
            {
            RCmConnectionMethodExt cm;
            TRAPD( err, cm = cmManager.ConnectionMethodL( cmArray[i] ) );
            DEBUG1("CHotSpotServer::CheckIapsL: err %d", err ); 
            if ( KErrNone == err )
                {
                HBufC* client( NULL );
                TRAPD( errr, client = cm.GetStringAttributeL( EWlanServiceExtensionTableName ));
                DEBUG1("CHotSpotServer::CheckIapsL: errr %d", errr );  
                if( KErrNone == errr )
                    {
                    TBuf<KIapNameLength> clientUid;
                    TUid uid(TUid::Null());
                    
                    clientUid.Copy( client->Des() );
                    delete client;
                    TInt indx = clientUid.Find( KMarkFirst );
                    if ( KErrNotFound != indx )
                        {
                        DEBUG("CHotSpotServer::CheckIapsL Client is found");
                        clientUid.Delete( indx, 1 );
                        indx = clientUid.Find( KMarkLast );
                        if ( KErrNotFound != indx )
                            {
                            clientUid.Delete( indx, 1 );
                            }
                        // Convert TBuf to TUid
                        TLex lex( clientUid );
                        TUint value( 0 );
                        User::LeaveIfError( lex.Val( value, EHex ) );
                        uid.iUid = value; 
                                        
                        // Try to find if 3rd party client exists.
                        // Delete IAP if no client.
                        CHssClientPlugin* plugin(NULL);
                        TBuf8<KExtensionAPILength> nullBuf;
                        TRAPD( error, plugin = CHssClientPlugin::NewL( uid, nullBuf ) );
                        delete plugin;

                        DEBUG1("CHotSpotServer::CheckIapsL find client error: %d", error );
                        if ( error == KErrNotFound )
                            {
                            cm.DeleteL();
                            }
                           }
                    }
                }
            DEBUG("CHotSpotServer::CheckIapsLOK");
            }
        CleanupStack::PopAndDestroy( &cmArray );
        CleanupStack::PopAndDestroy( &cmManager );
        }
    DEBUG("CHotSpotServer::CheckIapsL Done");
    }

// -----------------------------------------------------------------------------
// NewSessionL
// -----------------------------------------------------------------------------
//
CSession2* CHotSpotServer::NewSessionL( const TVersion& aVersion, 
                                        const RMessage2& /* aMessage */ ) const
    {
    TVersion version( KHotSpotMajorVersionNumber,
                      KHotSpotMinorVersionNumber,
                      KHotSpotBuildVersionNumber );

    if ( !User::QueryVersionSupported( version, aVersion ) )
        {
        User::Leave( KErrNotSupported );
        }

    DEBUG("**** CHotSpotServer::NewSessionL");
    CHotSpotSession* session = 
        CHotSpotSession::NewL( const_cast<CHotSpotServer&>( *this ) );
    return session;
    }

// ---------------------------------------------------------
// NotifyAdd
// ---------------------------------------------------------
//
void CHotSpotServer::NotifyAdd(
    CNotificationBase& aNotification )
    {
    DEBUG( "CHotSpotServer::NotifyAdd()" );
    iNotificationArray.Insert( &aNotification, 0 );
    }

// ---------------------------------------------------------
// NotifyRemove
// ---------------------------------------------------------
//
void CHotSpotServer::NotifyRemove(
    CNotificationBase& aNotification )
    {
    DEBUG( "CHotSpotServer::NotifyRemove()" );
    TInt index = iNotificationArray.Find( &aNotification );
    iNotificationArray.Remove( index );
    }

// -----------------------------------------------------------------------------
// SaveMessage
// -----------------------------------------------------------------------------
//
TInt CHotSpotServer::SaveMessage( TUint aIapId, const RMessage2& aMessage, 
                                  THotSpotCommands aCommand )
    {
    DEBUG("**** HotspotServer: SaveMessage");
    TInt ret( KErrNone );
    SRequestMapEntry entry;
    entry.iMessage = aMessage;
    entry.iFunction = aCommand;
    entry.iIapId = aIapId;
    ret = iMap.Append(entry);
    return ret;
    }

// -----------------------------------------------------------------------------
// CompleteMessage
// -----------------------------------------------------------------------------
//
void CHotSpotServer::CompleteMessage( TInt aIndex, TInt aResult )
    {
    DEBUG("**** HotspotServer: CompleteMessage");
    SRequestMapEntry entry = iMap[aIndex];
    
    RMessagePtr2 message = entry.iMessage;
    message.Complete( aResult );
    iMap.Remove( aIndex );
    }

// -----------------------------------------------------------------------------
// EditMessage
// -----------------------------------------------------------------------------
//
TInt CHotSpotServer::EditMessage( TInt aIndex, TPckg<HssScanList>& aData)
    {
    DEBUG("**** HotspotServer: EditMessage");
    TInt ret( KErrNone );
    
    iMap[aIndex].iMessage.Write(0, aData);
    
    DEBUG1("**** HotspotServer: EditMessage ret: %d", ret);
    return ret;
    }


// -----------------------------------------------------------------------------
// FindMessage
// -----------------------------------------------------------------------------
//
TInt CHotSpotServer::FindMessage( TUint aIapId, THotSpotCommands aCommand )
    {
    DEBUG("**** HotspotServer: FindMessage");
    TInt ret( KErrNotFound );
    SRequestMapEntry entry;
    
    for ( TInt i = 0; i < iMap.Count(); i++ ) 
        {
        entry = iMap[i];
        if ( ( entry.iFunction == aCommand ) && ( entry.iIapId == aIapId ) )
            {
            // Correct message found
            // Now stop loop and return index value, so that CompeteMessage can
            // be called with that
            ret = i;
            i = iMap.Count();
            }
        }
    DEBUG1("**** HotspotServer: FindMessage ret: %d", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// SetLogoutFlag
// -----------------------------------------------------------------------------
//
void CHotSpotServer::SetLogoutFlag( TBool aValue )
    {
    DEBUG("HotspotServer::SetLogoutFlag");
    iLogoutSent = aValue;
    }
    
// -----------------------------------------------------------------------------
// GetLogoutFlagValue
// -----------------------------------------------------------------------------
//
TBool CHotSpotServer::GetLogoutFlagValue()
    {
    DEBUG("HotspotServer::GetLogoutFlagValue()");
    return iLogoutSent;
    }

// -----------------------------------------------------------------------------
// SetLoginFlag
// -----------------------------------------------------------------------------
//
void CHotSpotServer::SetLoginFlag( TBool aValue )
    {
    DEBUG("HotspotServer::SetLoginFlag");
    iLoginValue = aValue;
    }
    
// -----------------------------------------------------------------------------
// GetLoginFlagValue
// -----------------------------------------------------------------------------
//
TBool CHotSpotServer::GetLoginFlagValue()
    {
    DEBUG("HotspotServer::GetLoginFlagValue()");
    return iLoginValue;
    }


// -----------------------------------------------------------------------------
// SetAssociationFlag
// -----------------------------------------------------------------------------
//
void CHotSpotServer::SetAssociationFlag( TBool aValue )
    {
    DEBUG("HotspotServer::SetAssociationFlag");
    iAssociationValue = aValue;
    }
        
// -----------------------------------------------------------------------------
// GetAssociationFlagValue
// -----------------------------------------------------------------------------
//
TBool CHotSpotServer::GetAssociationFlagValue()
    {
    DEBUG("HotspotServer::GetAssociationFlagValue()");
    return iAssociationValue;
    }

// -----------------------------------------------------------------------------
// SetServiceId
// -----------------------------------------------------------------------------
//
void CHotSpotServer::SetServiceId( TInt aServiceId )
    {
    DEBUG("HotspotServer::SetServiceId");
    iCurrentServiceIdInUse = aServiceId;
    }
    
// -----------------------------------------------------------------------------
// GetServiceId
// -----------------------------------------------------------------------------
//
TInt CHotSpotServer::GetServiceId()
    {
    DEBUG("HotspotServer::GetServiceId()");
    return iCurrentServiceIdInUse;
    }
    
// -----------------------------------------------------------------------------
// RunError
// -----------------------------------------------------------------------------
//
TInt CHotSpotServer::RunError( TInt aError )
    {
    // error from CHotSpotSession::ServiceL
    Message().Complete( aError );
    // Continue  reading client requests
    ReStart();
    return (KErrNone);
    }

// -----------------------------------------------------------------------------
// GetLoginTimerMicroSecs
// -----------------------------------------------------------------------------
//
TUint CHotSpotServer::GetLoginTimeMicroSecs( TUid aClientUid )
    {
    // Default timer value will be returned if matching client UID isn't found.
    TUint retval = KHssDefaultLoginTimeMicroSecs ;
    
    TInt ret = CHotSpotServer::FindClientUid( aClientUid );
    if (ret != KErrNotFound)
        {
        // ret is the matching element's index.
        retval = iLoginLogoutTimerArray[ret].loginTimeMicroSecs;
        }

    return retval;
    }

// -----------------------------------------------------------------------------
// GetLogoutTimerMicroSecs
// -----------------------------------------------------------------------------
//
TUint CHotSpotServer::GetLogoutTimeMicroSecs( TUid aClientUid )
    {
    // Default timer value will be returned if matching client UID isn't found.
    TUint retval = KHssDefaultLogoutTimeMicroSecs ;
    
    TInt ret = CHotSpotServer::FindClientUid( aClientUid );
    if (ret != KErrNotFound)
        {
        // ret is the matching element's index.
        retval = iLoginLogoutTimerArray[ret].logoutTimeMicroSecs;
        }

    return retval;
    }

// -----------------------------------------------------------------------------
// SetTimerValues
// -----------------------------------------------------------------------------
//
void CHotSpotServer::SetTimerValues(
        TUid aClientUid,
        TUint aLoginTimerValue,   // in seconds
        TUint aLogoutTimerValue ) // in seconds
    {    
    TInt ret = CHotSpotServer::FindClientUid( aClientUid );
    TUint loginTimeMicroSecs = KHssDefaultLoginTimeMicroSecs ;
    TUint logoutTimeMicroSecs = KHssDefaultLogoutTimeMicroSecs ;
    
    // Check that values are in bounds and modify them into micro seconds.
    if( aLoginTimerValue >= KHssMinLoginTime &&
            aLoginTimerValue <= KHssMaxLoginTime )
        {
        loginTimeMicroSecs = KMillion * aLoginTimerValue;
        }

    if( aLogoutTimerValue >= KHssMinLogoutTime &&
            aLogoutTimerValue <= KHssMaxLogoutTime )
        {
        logoutTimeMicroSecs = KMillion * aLogoutTimerValue;
        }

    if (ret != KErrNotFound)
        {
        DEBUG("CHotSpotServer::SetTimerValues(): Existing client modified.");
        // ret is the matching element's index.
        iLoginLogoutTimerArray[ret].loginTimeMicroSecs = loginTimeMicroSecs;
        iLoginLogoutTimerArray[ret].logoutTimeMicroSecs = logoutTimeMicroSecs;
        }
    else
        {
        DEBUG("CHotSpotServer::SetTimerValues(): New Client added.");
        // Create a new element and append it to the array.
        const SLoginLogoutTimers addedElement = {
                aClientUid,
                loginTimeMicroSecs,
                logoutTimeMicroSecs };
        iLoginLogoutTimerArray.Append( addedElement );
        }
    }

// -----------------------------------------------------------------------------
// FindClientUid
// -----------------------------------------------------------------------------
//
TInt CHotSpotServer::FindClientUid( TUid aClientUid )
    {
    TInt count = iLoginLogoutTimerArray.Count();
    TInt i = 0;
    TInt ret = KErrNotFound;
    
    while (i < count)
        {
        if (aClientUid == iLoginLogoutTimerArray[i].clientUid)
            {
            ret = i;
            break;
            }
        i++;
        }

    return ret;    
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// E32Main implements the executable entry function.
// Note that because the target type of the IPWServer Hss
// is EXEDLL, the entry point has different signature depending
// on the build platform.
// Creates a cleanup stack and runs the server. 
// Returns: Zero
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    DEBUG("**** HotspotServer: E32Main");
    CTrapCleanup* cleanup=CTrapCleanup::New();
    TInt r=KErrNoMemory;
    if (cleanup)
        {
        TRAP(r,RunServerL());
        delete cleanup;
        }
    else
        {
        User::Panic( KHotSpotModuleName, KHotSpotPanicCleanupStackCreationFailed );
        }
    __UHEAP_MARKEND;
    return r;
    }
 
// end of file
