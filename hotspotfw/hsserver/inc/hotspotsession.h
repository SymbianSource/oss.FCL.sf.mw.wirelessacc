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
* Description:   Implementation of HotSpot Server Session
*
*/


#ifndef HOTSPOTSESSION_H
#define HOTSPOTSESSION_H

//  INCLUDES
#include "hssclientplugin.h"
#include <e32base.h>
#include <e32svr.h>
#include <wlanmgmtclient.h> 
#include <wlanmgmtinterface.h>
#include <wlanmgmtcommon.h>
#include <commdb.h>
#include <ictscommon.h>
#include <ictsclientinterface.h>
#include "hssnotif.h"
#include "hssiapsettingshandler.h"
#include "hssclientinterface.h"
#include "hsslogintimerobserver.h"
#include "hsslogouttimerobserver.h"

// FORWARD DECLARATIONS
class CHotSpotServer;
class CWlanMgmtClient;
class HssNotifications;
class CSessionNotification;
class CHssLoginTimer;
class CHssLogoutTimer;


// CLASS DECLARATION
/**
* Server side counterpart for client's Session classes.
* Each connection has own session in server.
* @lib HotSpotServer.exe
* @since Series 60 3.0
*/
NONSHARABLE_CLASS( CHotSpotSession ) : public CSession2, 
                                       public MIctsObserver,
                                       public MHssLoginTimerObserver,
                                       public MHssLogoutTimerObserver,
                                       public MWlanMgmtNotifications
	{

    public:

        /**
        * Two-phased constructor.
        */
        static CHotSpotSession* NewL( CHotSpotServer& aServer );
        
         /**
        * Destructor.
        */
        ~CHotSpotSession();
   
       struct TNotification
        {
        TInt id;
        TBuf8<KHssMaxNotificationLength> data;
        };
    public: // Functions from base classes

        /**
        * From CSession called when a client asks for service
        * @param aMessage the message
        * @return None
        */
        void ServiceL( const RMessage2& aMessage );

        /**
        * Called by ServiceL when a client asks for service
        * @param aMessage the message
        * @return None
        */
        void DispatchMessageL( const RMessage2& aMessage );
        
        /**
        * From MICTSObserver. Called when internet connectivity tests are
        * being done.
        * @param aResult Result of tests
        * @param aString String containing possible HTML response
        * @return None
        */
        void ConnectivityObserver( TIctsTestResult aResult, 
                                   const TDesC& aString );
               
        /**
        * From MHssLoginTimerObserver. Called when time used for Login(..)
        * has run out for client.
        * @return None
        */
        void LoginTimeout();
        
         /**
        * From MHssLogoutTimerObserver. Called when time used for Logout(..)
        * has run out for client.
        * @return None
        */
        void LogoutTimeout();
        
        /**
                * From MWlanMgmtNotifications
                * @since Series 60 3.0
                * @param aNewState State of WLAN
                * @return None
                */
                virtual void ConnectionStateChanged( TWlanConnectionMode aNewState );
        
    public: // New functions

 
        /**
        * Metods for handling passthrough notifications
        */
        void HandleCancelNotifications( const RMessage2& aMessage );
        void AddNotification( TInt aNotification, TDes8& aData );
        void HandleOrderNotifications( const RMessage2& aMessage );
        void HandleNotification();
		
    private:

        /**
        * C++ default constructor.
        */
        CHotSpotSession( CHotSpotServer& aServer );

        /**
        * By default Symbian 2nd phase constructor is private.
        */

        void ConstructL();	
         /**
        * Initialize Scan to WlanEngine
        * @return None
        */

         /**
        * Launch Intenet Connectivity Test Service
        * @return None
        */
        void TestInternetConnectivityL();
        
        /**
        * Starts client register procedures
        * @param aMessage, RMessage from client side
        * @return None
        */
        void ProcessRegisterL( const RMessage2& aMessage );
        
        /**
        * Starts client unregister procedures
        * @param aMessage, RMessage from client side
        * @return None
        */
        void ProcessUnRegisterL( const RMessage2& aMessage );
        
        /**
        * Starts login procedures
        * @param aIapId, IAP id
        * @param aNetId, Network id
        * @return KErrNone, if successful, otherwice one of system wide errorcodes.
        */
        TInt ProcessStartLoginL( const TUint aIapId, const TUint aNetId );
        
          /**
        * Starts Join procedures
        * @param aIapId, IAP id
        * @return KErrNone, if successful, otherwice one of system wide errorcodes.
        */
        TInt ProcessStartL( const TUint aIapId );
 
        /**
        * Starts Configure & reJoin procedures
        * @param aIapId, IAP id
        * @return KErrNone, if successful, otherwice one of system wide errorcodes.
        */
       TInt ProcessStartAgain( const TUint aIapId );
        
        /**
        * Sends WLAN association status to client
        * @param aIapId, IAP id
        * @param aResult, ETrue is successful, otherwise EFalse
        * @return KErrNone, if successful, otherwice one of system wide errorcodes.
        */
        TInt ProcessAssociationStatus( const TUint aIapId, TBool aResult );        

        /**
        * Starts CloseConnection procedures
        * @param aIapId, IAP id
        * @return KErrNone, if successful, otherwice one of system wide errorcodes.
        */
       TInt ProcessCloseL( const TUint aIapId );
        
        /**
        * Creates client instance through ECom
        * @param aUid, ECom implementation UID of client
        * @return KErrNone, if successful, otherwice one of system wide errorcodes.
        */
        TInt CreateClient( const TUid aUid, TDesC8& aUidText );
        
        /**
        * Makes server shutdown processes (agent change and IAP removals)
        * @param aMessage, Message for completing request
        */
        void ProcessServerShutdown( const RMessage2& aMessage );
        
        /**
        * Updates UI state of HotSpot browser logon application.
        * @param aMessage, Message for completing request
        */
        void ProcessUiState( const RMessage2& aMessage );
        
        /**
        * Start HotSpot browser logon application.
        * @param aString, redirect URL
        */        
        void AuthenticateL( const TDesC& aString );
        
        /**
        * Reads EasyWLAN IAP id
        * @return None 
        */
        void EasyWlanIdL();
    
    private:    // Data
    
        /** List of pending (not sent) notifications. */
        RArray<TNotification> iPendingNotifications;
        
        /** The request from the client pending for a notification. */
        RMessagePtr2 iPendingNotificationRequest;

        /** Is there a pending request from the client. */
        TBool iIsNotificationRequestPending;

	    /**
        * Handle to server
        */	
        CHotSpotServer& iServer;    
    
        /**
        * Instance of 3rd party client (ECom)
        * Own
        */
        CHssClientPlugin* iClient;

        /**
        * handle to MHssSrvNotifications
        */
		MHssSrvNotifications *iSrvNotifications;

        /** Handle to remove notification. */
        CSessionNotification* iNotificationHandle;

	    /**
        * RMessage instance
        */	
	    RMessage2 iMessage;
	
	    /**
        * IAP id in use
        */	
	    TInt iIapId;
	    
	    /**
        * Network Id in use
        */
	    TInt iNetId;
 
        /**
        * handle to WlanMgmtClient
        */
    	CWlanMgmtClient *iMgtClient;

        /**
        * handle to HssNotifications
        */
 	    HssNotifications *iNotifications;


        /**
        * statusinformation of Async call
        */
		TRequestStatus iStatus;

        /**
        * Handler for changing IAP settings
        * Own
        */
        CHssIapSettingsHandler* iIapSettingsHandler;
        
        /**
        * Internet Connectivity testing instance
        * Own
        */
        CIctsClientInterface* iIcts;

        /**
        * Timer used for client Login procedure
        * Own
        */       
        CHssLoginTimer* iLoginTimer;
        
        /**
        * Timer used for client Logout procedure
        * Own
        */       
        CHssLogoutTimer* iLogoutTimer;
        
        /**
        * Flag for notification sending
        */       
		TBool iAllowNotifications;
        
        /**
        * Client UID of this session
        */
        TUid iClientUid;
        
        /**
        * Flag for indicate if extended HSFW API is used
        */
        TBool iHotspotExtension;
        
        /**
        * Easy WLAN IAP Id
        */
        TUint32 iEasyWlanId;
	};
	
#endif
