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
* Description:    Implementation of HotSpot Server
*
*/



#ifndef HOTSPOTSERVER_H
#define HOTSPOTSERVER_H

//  INCLUDES
#include <e32base.h>
#include <e32svr.h>
#include <e32std.h>
#include "hotspotclientserver.h"
#include "hssscanlist.h"
#include "hssnotif.h"

#include <wlanmgmtcommon.h>
#include <wlanmgmtinterface.h>

// FORWARD DECLARATIONS
class CHotSpotSession;
class HssScanList;
class CNotificationBase;

// -----------------------------------------------------------------------------
// Server's policy
// -----------------------------------------------------------------------------

//Total number of ranges
const TUint KHotSpotServerPlatSecRangeCount = 3;

//Definition of the ranges of IPC numbers
const TInt THotSpotServerPlatSecRanges[ KHotSpotServerPlatSecRangeCount ] = 
        {
        0,      // Internal IPC Messages (Client, WLAN, NIfman, Shutdown, Internal, UI) 
        20,     // IPC Message ICTS API
        21		// Not supported IPC Messages 
        }; 

//Policy to implement for each of the above ranges        
const TUint8 THotSpotServerPlatSecElementsIndex[ KHotSpotServerPlatSecRangeCount ] = 
        {
        0,                            // Internal IPC Messages
        CPolicyServer::EAlwaysPass,	  // IPC Messages ICTS API
        CPolicyServer::ENotSupported  // Not supported IPC Messages
        };

//Specific capability checks
const CPolicyServer::TPolicyElement THotSpotServerPlatSecElements[] = 
        {
        { _INIT_SECURITY_POLICY_C1( ECapabilityNetworkControl ), CPolicyServer::EFailClient }
        };

//Package all the above together into a policy
const CPolicyServer::TPolicy THotSpotServerPlatSecPolicy =
        {
        CPolicyServer::EAlwaysPass,   // specifies all connect attempts should pass
        KHotSpotServerPlatSecRangeCount,	  // number of ranges                                   
        THotSpotServerPlatSecRanges,	      // ranges array
        THotSpotServerPlatSecElementsIndex,  // elements<->ranges index
        THotSpotServerPlatSecElements,		  // array of elements
        };
        
// ----------------------------------------------------------------------------------------
// Server's panic codes here
// ----------------------------------------------------------------------------------------
enum THotspotPanic
	{
	EPanicGeneral,
	EPanicIllegalFunction
	};

void PanicClient(const RMessagePtr2& aMessage,THotspotPanic aPanic);

struct SRequestMapEntry
        {
        RMessagePtr2 iMessage;
        THotSpotCommands iFunction;
        TUint iIapId;
        };
	
struct SLoginLogoutTimers
        {
        TUid clientUid;
        TUint loginTimeMicroSecs;
        TUint logoutTimeMicroSecs;
        };
    
// CLASS DECLARATION

/**
*  The server of HotSpot services.
*
*  @lib HotSpotServer.exe
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS ( CHotSpotServer ) : public CPolicyServer, public MWlanMgmtNotifications
	{
	
    public: // Constructors and destructor
    
        /**
        * Two-phased constructor.
        */
	    static CHotSpotServer* NewLC();
    	
    	/**
        * Destructor.
        */
        virtual ~CHotSpotServer();
        
    
    public: // New functions
        
        virtual void NotifyAdd( CNotificationBase& aNotification );
        virtual void NotifyRemove(CNotificationBase& aNotification );
        
        /**
        * Saves Iap Id, RMessage2, THotSpotCommands as one entry
        * @since Series 60 3.0
        * @param aIapId IAP id
        * @param aMessage Client/Server message from client
        * @param aCommand Command/Function of message
        * @return None
        */
        TInt SaveMessage( TUint aIapId, const RMessage2& aMessage,
                            THotSpotCommands aCommand );
        
        /**
        * Complete RMessage2 
        * @since Series 60 3.0
        * @param aIndex Index of completed message in array
        * @param aResult Return value in complete
        * @return None
        */
        void CompleteMessage( TInt aIndex, TInt aResult );
        
		/**
        * Find message & add data to message
        * @since Series 60 3.0
        * @param aIndex, Index of message in array
        * @param aData, TPckg type of data  
        * @return Index of message in array, otherwise KErrNotFound
        */
		TInt EditMessage( TInt aIndex, TPckg<HssScanList>& aData);
        
        /**
        * Check if IAP id has uncomplete messages
        * @since Series 60 3.0
        * @param aIapId IAP id
        * @param aCommand Command/Function of message
        * @return Index of message in array, otherwise KErrNotFound
        */
        TInt FindMessage( TUint aIapId, THotSpotCommands aCommand );
        
        /**
        * Set value whether Logout() is sent or not
        * @since Series 60 3.0
        * @param aValue New value of flag
        * @return None
        */
        void SetLogoutFlag( TBool aValue );
        
        /**
        * Gets value if Logout is sent to client or not
        * @since Series 60 3.0
        * @return ETrue if Login is sent. Otherwise EFalse
        */
        TBool GetLogoutFlagValue();
        
        /**
        * Set value whether Login() is sent or not
        * @since Series 60 3.0
        * @param aValue New value of flag
        * @return None
        */
        void SetLoginFlag( TBool aValue );
        
        /**
        * Gets value if Login is sent to client or not
        * @since Series 60 3.0
        * @return ETrue if Login can be sent. Otherwise EFalse
        */
        TBool GetLoginFlagValue();
        
        /**
        * Set value whether WLAN association status can be sent
        * @param aValue New value of flag
        * @return None
        */
        void SetAssociationFlag( TBool aValue );
                
        /**
        * Gets value whether WLAN association status can be sent
        * @return ETrue if can be sent. Otherwise EFalse.
        */
        TBool GetAssociationFlagValue();
        
        /**
        * Set service id of IAP in use
        * @since Series 60 3.0
        * @param aServiceId service id
        * @return None
        */
        void SetServiceId( TInt aServiceId );
        
        /**
        * Gets service id of IAP in use
        * @since Series 60 3.0
        * @return service ID
        */
        TInt GetServiceId();
    
        /**
        * Checks IAPs if there's any unused Hotspot IAPs.
        * @since Series 60 3.0
        * @return None
        */
        void CheckIapsL();
        
        /**
        * Get login timer value for the specified client.
        * @param aClientUid, client's user ID,
        * @return Login timer value.
        */        
        TUint GetLoginTimeMicroSecs( TUid aClientUid );
    
        /**
        * Get logout timer value for the specified client.
        * @param aClientUid, client's user ID,
        * @return Logout timer value.
        */        
        TUint GetLogoutTimeMicroSecs( TUid aClientUid );

        /**
        * Set both login and logout timer values for the specified client.
        * @param aClientUid, client's user ID,
        * @param aLoginTimerValue, set login timer value,
        * @param aLogoutTimerValue, set logout timer value.
        */        
        void CHotSpotServer::SetTimerValues(
                TUid aClientUid,
                TUint aLoginTimerValue,
                TUint aLogoutTimerValue );
        
    private:
        
        /**
        * Find matching Client UID from the timer array.
        * @param aClientUid, client UID to find.
        * @return matching index if UID was found, otherwise KErrNotFound.
        */        
        TInt CHotSpotServer::FindClientUid( TUid aClientUid );
        
    public: // Functions from base classes

        /**
        * From CServer creates a new session
        * @since Series 60 3.0
        * @param aVersion Version
        * @return Pointer to created server side session
        */
        CSession2* NewSessionL( const TVersion& aVersion,
                                const RMessage2& aMessage ) const;
                
        /**
        * From MWlanMgmtNotifications
        * @since Series 60 3.0
        * @param aNewState State of WLAN
        * @return None
        */
        virtual void ConnectionStateChanged( TWlanConnectionMode aNewState );

    protected:
        
        /**
        * Overrides CActive::RunError()
        */
        virtual TInt RunError( TInt aError );
        
    private:
    
        /**
        * C++ default constructor.
        */
	    CHotSpotServer();
	    
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
	    void ConstructL();

    private:
	    /**
        * Messages that are needed to be saved for later completion
        * Own
        */
        RArray<SRequestMapEntry> iMap;
        /** 
         * List of clients who have subscribed notifications
         */
        RArray<CNotificationBase*> iNotificationArray;
        
        /**
        * Flag for sending client Logout -message
        */
        TBool iLogoutSent;
        
        /**
        * Flag for sending client Login -message
        */
        TBool iLoginValue;
        
        /**
        * Flag for sending WLAN association status to client
        */
        TBool iAssociationValue;
        
        /**
        * Boolean flag whether the IAPs has been checked or not
        */
        TBool iIapCheckValue;

        /**
        * WLAN management API
        */
        CWlanMgmtClient* iMgtClient;
        
        /**
        * Service id of IAP in use
        */
        TInt iCurrentServiceIdInUse;
        
        /**
        * Array for storing client dependent login and logout timer values.
        */
        RArray<SLoginLogoutTimers> iLoginLogoutTimerArray;
	};

#endif
