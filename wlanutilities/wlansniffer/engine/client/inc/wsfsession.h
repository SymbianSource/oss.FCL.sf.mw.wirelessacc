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
* Description:   Class header for RWsfSession
*
*/





#ifndef R_WSFSESSION_H
#define R_WSFSESSION_H

//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfcommon.h"


//  FORWARD DECLARATIONS
class CWsfWlanInfoArray;
class CWsfEventHandler;
class MWsfStateChangeObserver;
class TWsfWlanInfo;
class CWsfActiveWaiter;


//  CLASS DEFINITION
/**
 * Client-side handle to server session
 *
 * RWsfSession is the client-side session of a connection to the server. 
 * It provides an interface through which the different scanning services 
 * can be reached and results obtained.
 *
 * @lib wsfclient.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( RWsfSession ): public RSessionBase
    {
        friend class CWsfEventHandler;    


    public:     // Constructor

        IMPORT_C RWsfSession();


    public:     // New methods

        /**
        * Initiates a connection to the server and also launches it 
        * if it was not running yet.
        * @since S60 5.0
        */
        IMPORT_C TInt Connect();
        
        /**
        * Returns the version number of the session class
        * @since S60 5.0
        * @return A version structure
        */
        inline TVersion Version() const;
        
        /**
        * Closes this session.
        * @since S60 5.0
        */    
        IMPORT_C void Close();

        /**
        * Subscribes to server events
        * @since S60 5.0
        * @param aObserver The object which should be notified on event
        */
        IMPORT_C void NotifyEventL( MWsfStateChangeObserver& aObserver );
        
        /**
        * Cancels the subscription
        * @since S60 5.0
        */ 
        IMPORT_C void CancelNotifyEvent();

        /**
        * Sets the wlan scanning on if not already active
        * @since S60 5.0
        * @return ETrue on success.
        */ 
        IMPORT_C TBool EnableScanL();

        /**
        * Disables the scanning 
        * @since S60 5.0
        * @return ETrue on success.
        */
        IMPORT_C TBool DisableScanL();

        /**
        * Queries the current scanning status.
        * @since S60 5.0
        * @return ETrue if scanning is enabled, EFalse otherwise.    
        */
        IMPORT_C TBool IsScanEnabledL();
        
        /**
        * Requests the scan results from the server 
        * @since S60 5.0
        * @param aInfoArray Array for the scan results. Ownership not changed.
        */ 
        IMPORT_C void UpdateWlanListL( CWsfWlanInfoArray* aInfoArray );
        
        /**
        * Asyncronous request to the server to return wlan list size
        * @since S60 5.2
        * @param aPckg request result
        * @param aStatus The request status object used to contain 
        *        the completion status of the request.
        */
        IMPORT_C void GetWlanListSize( TPckgBuf<TUint>& aPckg, 
                                       TRequestStatus& aStatus );

        
        /**
        * Asyncronous request to the server to return wlan list
        * @since S60 5.2
        * @param aPckg request result
		* @param aPtr wlan list
        * @param aStatus The request status object used to contain 
        *        the completion status of the request.
        */
        IMPORT_C void GetWlanList( TPckgBuf<TUint>& aPckg, TPtr8& aPtr, 
                                   TRequestStatus& aStatus );

        /**
        * Cancel all pending tasks.
        * @since S60 5.0
        */
        IMPORT_C void CancelAll();

        /**
        * Return the connected wlan network name
        * @since S60 5.0
        * @return The connected account name, or NULL if not connected
        */ 
        IMPORT_C HBufC* ConnectedAccountNameL();

        /**
        * Return the connected wlan's details
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo object to fill
        * @return ETrue if success, EFalse otherwise
        */ 
        IMPORT_C TBool GetConnectedWlanDetailsL( TWsfWlanInfo& aWlanInfo );
        
        /**
        * Asyncronous request to the server to return connected wlan network
        * @since S60 5.2
        * @param aPckg request result
        * @param aWlanInfo The wlaninfo object to fill
        * @param aStatus The request status object used to contain 
        *        the completion status of the request.
        */
        IMPORT_C void GetConnectedWlanDetails( TPckgBuf<TBool>& aPckg,
                                               TWsfWlanInfo& aWlanInfo,
                                               TRequestStatus& aStatus );
        
        /**
        * Start monitoring events for the specified access point
        * @since S60 5.0
        * @param aMonitoredIap The id of the IAP to be monitored
        */
        IMPORT_C void MonitorAccessPointL( TUint32 aMonitoredIap );
        
        /**
        * Connect to the given WLAN IAP. The function requests the server, 
        * then starts a synchronous wait until the connection is established.
        * @since S60 5.0
        * @param aIapId The id of the IAP to connect to
        * @param aPersistence The value of the persistence property
        * @return KErrNone if managed to connect, otherwise error code
        */
        IMPORT_C TInt ConnectWlanBearerL( TUint32 aIapId, 
                           TWsfIapPersistence aPersistence = EIapPersistent );
        
        /**
         * Asyncronous request to the server to connect
         * @since S60 5.2
         * @param aPckg request result
         * @param aIapId The id of the IAP to connect to
         * @param aPersistence The value of the persistence property
         * @param aStatus The request status object used to contain 
         *        the completion status of the request.
         */
        IMPORT_C void ConnectWlanBearer( TPckgBuf<TBool>& aPckg,
                                         TUint32 aIapId, 
                                         TWsfIapPersistence aPersistence,
                                         TRequestStatus& aStatus );
        
        /**
        * Sets connection result
        * @since S60 5.2
        * @param aResult The connection request result
        */
        IMPORT_C void SetConnectWlanBearerResult( TInt aResult );

        /**
        * Sets the persistence property of the currently monitored IAP
        * @since S60 5.0
        * @param aPersistence The value of the property
        * @return ETrue on success, otherwise false
        */
        IMPORT_C TBool SetIapPersistenceL( TWsfIapPersistence aPersistence );

        /**
        * Disconnect the current wlan connection
        * @since S60 5.0
        * @return ETrue if there was an active connection and it was 
        *         disconnected successfully, otherwise EFalse
        */
        IMPORT_C TBool DisconnectWlanBearerL();
        
        /**
        * Asyncronous request to the server to disconnect
        * @since S60 5.2
        * @param aPckg request result
        * @param aStatus The request status object used to contain 
        *        the completion status of the request.
        */
        IMPORT_C void DisconnectWlanBearer( TPckgBuf<TBool>& aPckg, 
                                            TRequestStatus& aStatus );
        
        /**
        * Request the server to make a forced scan
        * @since S60 5.0
        * @return ETrue on success
        */
        IMPORT_C TBool RequestScanL();
        
		/**
        * Asyncronous request to the server to make a scan
        * @since S60 5.2
        * @param aPckg request result
        * @param aStatus The request status object used to contain 
        *        the completion status of the request.
        */
        IMPORT_C void RequestScan( TPckgBuf<TBool>& aPckg, 
                                   TRequestStatus& aStatus );
        
        /**
        * Queries if there is an active WLAN connection.
        * @since S60 5.0
        * @return ETrue if there is an active WLAN connection, 
        * *       EFalse otherwise.
        */
        IMPORT_C TBool IsConnectedL();
        
        /**
        * Aborts the currently ongoing connecting process
        * @since S60 5.0
        */
        IMPORT_C void AbortConnectingL();
        
        /**
        * Aborts the currently ongoing scanning process 
        * @since S60 5.0
        */
        IMPORT_C void AbortScanningL();
        
        /**
        * Controls the auto-disconnect timer in the server 
        * for owned connections
        * @since S60 5.0
        * @param aAdcCommand Command bitmask for the timer, 
        *                    see TWsfAutoDisconnectCommand
        * @return ETrue if the timer has been started (also if it had been 
        *         started earlier), EFalse if the timer couldn't be started
        *         (e.g. because there is no owned connection).
        */
        IMPORT_C TBool ControlDisconnectTimerL( TUint aAdcCommand );
        
        
    private:
        /**
        * Starts the server if it was not running already.
        * @since S60 5.0
        * @return KErrNone on success, otherwise error code
        */
        TInt StartServer();

        
    private:
        /**
        * Cancels the notifications from the server. 
        * Only called by CWsfEventHandler.
        * @since S60 5.0
        */    
        void DoCancelNotifyEvent();

        /**
        * Tries to restart the server after it died because of an error.
        * Only called by CWsfEventHandler.
        * @since S60 5.0
        * @param aObserver The object which should be notified on 
        *                  server events. (This call just passes the current 
        *                  observer of the event handler, which is needed 
        *                  for the initialisation.)
        * @return ETrue if the restart succeeded
        */    
        TBool RecoverFromServerTermination( 
                                        MWsfStateChangeObserver& aObserver );

        /**
        * Stops the waiting for connection creation started by 
        * ConnectWlanBearerL.
        * @since S60 5.0
        */
        void StopConnectingWait();
        

    private: // Data
        /**
        * Event handler active object for the server-generated events. Owned.
        */
        CWsfEventHandler* iEventHandler;
        
        /**
        * Indicates the result of the connecting process
        */
        TInt iConnectingResult;
        
        /**
        * Waiter to make the asynchronous connect call synchronous 
        * on client-side
        */
        CActiveSchedulerWait* iConnWaiter;
    };

#endif // R_WSFSESSION_H
