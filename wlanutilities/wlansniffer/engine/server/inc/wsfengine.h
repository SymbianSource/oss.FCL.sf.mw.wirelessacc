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
* Description:  Class header for CWsfModel
*
*/



#ifndef C_WSFENGINE_H
#define C_WSFENGINE_H

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <commsdattypesv1_1.h>
#include <cmmanagerext.h>

//  INTERNAL INCLUDES
#include "wsfwlanscannerobserver.h"
#include "wsfwlanmonitorobserver.h"
#include "wsfcommon.h"


//  FORWARD DECLARATIONS
class CWsfWlanScanner;
class MWsfEngineObserver;
class CWsfWlanBearerConnectionMonitor;
class TWsfWlanInfo;
class MWsfServerCloserAdapter;

//  CLASS DEFINITION
/**
 * Sniffer engine class.
 * This class manages the objects responsible for connection creation, 
 * closing and monitoring, communication with the WLAN engine etc.
 *
 * @since S60 5.0
 * @lib wsfserver.exe
 */
NONSHARABLE_CLASS( CWsfEngine ): public CBase, 
                                 public MWsfWlanScannerObserver, 
                                 public MWsfWlanMonitorObserver
    {
    public:     // Constructors and destructor
        /**
        * Factory function.
        * @since S60 5.0
        * @param aServerCloser Reference to the server closer object.
        *                      Ownership not passed.
        * @return Server instance
        */
        static CWsfEngine* NewL( MWsfServerCloserAdapter& aServerCloser );

        /**
        * Factory function.
        * @since S60 5.0
        * @param aServerCloser Reference to the server closer object.
        *                      Ownership not passed.
        * @return Server instance
        */
        static CWsfEngine* NewLC( MWsfServerCloserAdapter& aServerCloser );

        /**
        * Destructor
        */        
        ~CWsfEngine();
        

    private:    // Constructors
        /**
        * Constructor
        * @since S60 5.0
        * @param aServerCloser Reference to the server closer object.
        *                      Ownership not passed.
        */
        CWsfEngine( MWsfServerCloserAdapter& aServerCloser );

        /**
        * Second-phase constructor.
        * @since S60 5.0
        */
        void ConstructL();


    public:     // New methods
 
        /** 
        * Gets the result of complete wlan scan
        * @since S60 5.0
        * @return Buffer pointer containing a serialized array of 
        *         wlaninfo items
        */ 
        HBufC8* GetScanResults();

        /**
        * Sets wlan scanning on
        * @since S60 5.0
        */
        void EnableScanningL();
        
        /**
        * Sets wlan scanning off
        * @since S60 5.0
        */ 
        void DisableScanningL();
        
        /**
        * Returns whether scanning is enabled
        * @since S60 5.0
        * @return ETrue if scanning has been enabled, EFalse otherwise
        */
        TBool IsScanning();

        /**
        * Checks if there is an active wlan connection and returns connection
        * name (SSID) if appropriate.
        * @since S60 5.0
        * @return The SSID of connected access point or empty descriptor 
        *         if not connected
        */
        HBufC* ConnectedWlanSsidL();

        /**
        * Fills the wlaninfo with the details of the active WLAN connection
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo record to be filled
        *                  Ownership not passed.
        * @return ETrue if success, EFalse on failure
        */
        TBool ConnectedWlanConnectionDetailsL( TWsfWlanInfo& aWlanInfo );

        /**
        * Connects to the given IAP
        * @since S60 5.0
        * @param aIapId The IAP to connect to
        * @param aConnectOnly ETrue if Connect selected
        * @param aPersistence Persistence property of the IAP
        * @return KErrNone if successful, otherwise error code
        */
        TInt ConnectWlanL( TUint32 aIapId,
                           TBool aConnectOnly,
                           TWsfIapPersistence aPersistence );

        /**
        * Disconnects the active WLAN connection
        * @since S60 5.0
        * @return ETrue on success, EFalse if there was nothing to disconnect
        */
        TBool DisconnectWlan();
    
        /**
        * Requests a wlan scan
        * @since S60 5.0
        * @return ETrue if scanning is restarted
        */
        TBool RefreshScanResults();
        
        /**
        * Requests the server to monitor an access point
        * @since S60 5.0
        * @param aApId Access Point to be monitored
        */
        void MonitorAccessPointL( TUint32 aApId );

        /**
        * Queries if there is an active WLAN connection.
        * @since S60 5.0
        * @return ETrue if there is an active WLAN connection, 
        *         EFalse otherwise.
        */
        TBool IsConnected();
        
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
        * @since S60 5.0
        * @param aPersistence The value of the property
        * @return ETrue on success
        */
        TBool SetIapPersistenceL( TWsfIapPersistence aPersistence );

        /**
        * Controls the auto-disconnect timer in the server 
        * for owned connections
        * @since S60 5.0
        * @param aAdcCommand Command for the timer,
        *                    see TWsfAutoDisconnectCommand
        * @return ETrue if the timer has been started (also if it had been 
        *         started earlier), EFalse if the timer couldn't be started
        *         (e.g. because there is no owned connection).
        */
        TBool ControlDisconnectTimer( TUint aAdcCommand );
        
        /**
         * Resets the iSuppressIapDeletion flag.
         * @since S60 5.0
         */
        void ResetSuppressIapDeletion ();
                
    
    public:  // new methods

        /**
        * Adds the given observer to the notification list
        * @since S60 5.0
        * @param aObserver The observer to add. Ownership not passed.
        */
        void AttachL( MWsfEngineObserver* aObserver );

        /**
        * Deletes the given observer from the notification list
        * @since S60 5.0
        * @param aObserver The observer to remove. Ownership not passed.
        */
        void Detach( MWsfEngineObserver* aObserver );
   

       protected: // from MWsfWlanScannerObserver
       
        /**
        * When the scan cycle has completed this function is called to report 
        * the completion.
        * @since S60 5.0
        */
        void WlanScanCompleteL();

        /**
        * Called when the scan cycle has started  
        * @since S60 5.0
        */
        void WlanScanStarted();

        /**
        * Called when an error occured while scanning wlan data  
        * @since S60 5.0
        * @param aError System wide error code
        */
        void NotifyError( TInt aError );        
           
          
       protected: // from MWsfWlanMonitorObserver       

        /**
        * Called when a wlan connection is established
        * @since S60 5.0
        * @param aConnectionName WLAN connection name (SSID)
        */
        virtual void ConnectionEstablishedL( const TDesC& aConnectionName );

        /**
        * Called when wlan connection has been lost
        * @since S60 5.0
        */
        virtual void ConnectionLostL();
        
        /**
        * Called when the connection process failed for some reason
        * @since S60 5.0
        * @param aError System wide error code
        */
        virtual void ConnectingFailedL( TInt aError );
        
        /**
        * Called when the connection no longer needs the IAP it was using
        * @since S60 5.0
        */
        virtual void ConnectedIapReleasedL();

        
    private:
    
        /**
        * Tries to delete the corresponding IAP
        * @since S60 5.0
        * @param aIapId The IAP to be deleted
        */
        void DeleteIapL( TUint32 aIapId );


    private:    // Data
        /**
        * Reference to the server closer (not owned)
        */
        MWsfServerCloserAdapter& iServerCloser;
        
        /**
        * WLAN scanner object (owned)
        */
        CWsfWlanScanner* iScanner;
        
        /**
        * Handle to the commsdat database (owned)
        */
        CommsDat::CMDBSession* iDbSession;
        
        /**
        * WLAN connection monitor object (owned)
        */
        CWsfWlanBearerConnectionMonitor* iWlanBearerMonitor;
 
        /**
        * Reference counter for scan enabling requests
        */
        TInt iEnableScanning;
        
        /**
        * Array of references to the observers of the engine 
        * (elements not owned)
        */        
        RPointerArray<MWsfEngineObserver> iObservers;
        
        /**
        * Accesspoint to be monitored
        */
        TUint32 iMonitoredIap;

        /**
        * Indicates the persistence property of monitored IAP
        */
        TWsfIapPersistence iIapPersistence;

        /**
        * Indicates whether to postpone IAP deletion
        */
        TBool iSuppressIapDeletion;
    };

#endif      //  C_WSFENGINE_H

// End of file
