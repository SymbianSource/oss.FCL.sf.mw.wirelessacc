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
* Description:  Class header for CWsfSession
*
*/



#ifndef C_WSFSESSION_H
#define C_WSFSESSION_H

//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfengineobserver.h"
#include "wsfservercommon.h"


//  FORWARD DECLARATIONS
class CWsfServer;

//  CLASS DEFINITION
/**
 * Server-side session class.
 * This class is instantiated and used each time a client connects to the 
 * sniffer server.
 *
 * @since S60 5.0
 * @lib wsfserver.exe
 */
NONSHARABLE_CLASS( CWsfSession ): public CSession2, 
                                  public MWsfEngineObserver
    {
    public: // Construction and destruction
        
        /**
        * Factory function.
        * @since S60 5.0
        * @param aServer Reference to the server object. Ownership not passed.
        * @return Session instance
        */
        static CWsfSession* NewL( CWsfServer& aServer );

        /**
        * Destructor.
        */
         ~CWsfSession();

    private:
        /**
        * Constructor
        * @since S60 5.0
        * @param aServer Reference to the server object. Ownership not passed.
        */
        CWsfSession( CWsfServer& aServer );
        
        /**
        * Second-phase constructor.
        * @since S60 5.0
        */
        void ConstructL();


    public: // from CSession2
        /**
        * Handles the servicing of a client request that has been passed to 
        * the server.
        * @since S60 5.0
        * @param aMessage The message containing the details of the 
        *                 client request
        */
        void ServiceL( const RMessage2& aMessage );
        

    public: // new functions
        /**
        * Handles messages by reading the function id and passing 
        * the request forward.
        * @since S60 5.0
        * @param aMessage The received message containing function and 
        *                 descriptors.
        */
        void DispatchMessageL( const RMessage2& aMessage );


    private: // new functions
        /**
        * Returns whether wlan scanning is scheduled at the moment
        * @since S60 5.0
        * @param aMessage The message to process
        * @return ETrue if active or efalse.
        */
        TBool ScanActiveL( const RMessage2& aMessage );    

        /**
        * Enables the engine scanning 
        * @since S60 5.0
        */
        void ActivateScanL();

        /**
        * Disables the engine scanning 
        * @since S60 5.0
        */
        void DisableScanL();

        /**
        * Returns the wlan bearer status
        * @since S60 5.0
        * @param aMessage The message to process
        */
        void IsConnectedL( const RMessage2& aMessage );
        
        /**
        * Activates asynchronous request to engine events
        * @since S60 5.0
        * @param aMessage The message to process
        */
        void RequestNotifyL( const RMessage2& aMessage );
        
        /**
        * Cancels pending notify request
        * @since S60 5.0
        */
        void CancelNotifyEvents();

        /**
        * Cancels all pending tasks
        * @since S60 5.0
        */ 
        void CancelPendingTasks();

        /**
        * Gets the connected wlan connection name
        * @since S60 5.0
        * @param aMessage The message to process
        */
        void GetConnectedAccountNameL( const RMessage2& aMessage );  
        
        /**
        * Gets the connected wlan's details in a TWsfWlanInfo
        * @since S60 5.0
        * @param aMessage The message to process
        */
        void GetConnectedDetailsL( const RMessage2& aMessage );
        
        /**
        * Checks the required buffer size for serialized wlan data 
        * @since S60 5.0
        * @param aMessage The message to process
        */        
        void ReplyWlanDataSizeL( const RMessage2& aMessage );

        /**
        * Copies serialized wlan data
        * @since S60 5.0
        * @param aMessage The message to process
        */
        void WriteWlanDataL( const RMessage2& aMessage );

        /**
        * Passes connect wlan commands to engine
        * @since S60 5.0
        * @param aMessage The message to process
        */ 
        void ConnectWlanL( const RMessage2& aMessage );

        /**
        * Passes disconnect wlan commands to engine
        * @since S60 5.0
        * @param aMessage The message to process
        */ 
        void DisconnectWlanL( const RMessage2& aMessage );

        /**
        * Request the engine to scan asap
        * @since S60 5.0
        * @param aMessage The message to process
        */
        void RequestScanL( const RMessage2& aMessage );

        /**
        * Starts to monitor UI defined AP
        * @since S60 5.0
        * @param aMessage The message to process
        */
        void MonitorApL( const RMessage2& aMessage );
        
        /**
        * Aborts the currently ongoing connecting process 
        * @since S60 5.0
        */
        void AbortConnectingL();
        
        /**
        * Aborts the currently ongoing scanning process 
        * @since S60 5.0
        */
        void AbortScanning();
        
        /**
        * Sets the persistence property for the currently monitored IAP
        * @param aMessage The message to process
        * @since S60 5.0
        */
        void SetIapPersistenceL( const RMessage2& aMessage );
        
        /**
        * Controls the auto-disconnect timer in the server 
        * for owned connections
        * @since S60 5.0
        * @param aMessage The message to process
        */
        void ControlDisconnectTimerL( const RMessage2& aMessage );

        /**
        * Returns whether scanning is enabled
        * @since S60 5.0
        * @param aMessage The message to process
        */
        void ReplyIsScanningL( const RMessage2& aMessage );
                

    public: // from MWsfEngineObserver
        /**
        * Called when the WLAN scanning is finished and results are available
        * @since S60 5.0
        */
        void WlanListChangedL();

        /**
        * Called when an error occured in the engine
        * @since S60 5.0
        * @param aError System wide error code
        */
        void NotifyErrorL( TInt aError );

        /**
        * Called when the WLAN scanning has been enabled
        * @since S60 5.0
        */
        void ScanEnabledL();
        
        /**
        * Called when the WLAN scanning has been disabled
        * @since S60 5.0
        */
        void ScanDisabledL();
        
        /**
        * Called when a WLAN connection has been established
        * @since S60 5.0
        */
        void ConnectedL();
        
        /**
        * Called when the active WLAN connection has been terminated
        * @since S60 5.0
        */
        void DisconnectedL();
        
        /**
        * Called when an error occured during the connecting process
        * @since S60 5.0
        * @param aError System wide error code
        */
        void ConnectingFailedL( TInt aError );


    private:    // new methods
        /**
        * Notifies the client of the next server event in the event queue
        * @since S60 5.0
        */
        void NotifyClientL();
        

    private:    // data
        /**
        * Reference to the server object (not owned)
        */
        CWsfServer& iServer;
        
        /**
        * Indicates if notification has been requested
        */
        TBool iRequestNotify;
        
        /**
        * Client message to complete when notifying
        */
        RMessage2 iNotifyChangesMessage;
        
        /**
        * Queue of server events
        */
        RArray<TWsfNotifyEventContainer> iEventQueue;
        
        /**
        * Indicates whether scanning is enabled
        */
        TBool iEnabledScanning;
        
        /**
        * Indicates if an event is being processed
        */
        TBool iProcessingEvent;
    };

#endif // C_WSFSESSION_H

