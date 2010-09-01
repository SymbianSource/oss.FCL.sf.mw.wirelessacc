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
* Description:  Class header for CWsfWlanBearerConnectionMonitor
*
*/



#ifndef C_WSFWLANBEARERCONNECTIONMONITOR_H
#define C_WSFWLANBEARERCONNECTIONMONITOR_H


//  EXTERNAL INCLUDES
#include <e32base.h>
#include <rconnmon.h>
#include <es_sock.h>
#include <cmdefconnvalues.h>
#include <cmmanagerext.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"
#include "wsfwlanconnectiondetailsprovider.h"


//  FORWARD DECLARATIONS
class MWsfWlanMonitorObserver;
class MWsfServerCloserAdapter;
class CWsfIct;


//  CLASS DEFINITION
/**
 * Class to monitor and control all WLAN bearer related events and actions
 *
 * @since S60 5.0
 * @lib wsfserver.exe
 */
NONSHARABLE_CLASS( CWsfWlanBearerConnectionMonitor ): public CActive, 
                                     public MConnectionMonitorObserver, 
                                     public MWsfWlanConnectionDetailsProvider
    {
    private:
        /**
        * Connecting states when creating an owned connection
        */
        enum TWsfConnectingState 
            {
            /**
            * Not connected, neither requested
            */
            ECsIdle,
            
            /**
            * Requested to connect
            */
            ECsNotConnected,
            
            /**
            * Socket server and connection handler opened
            */
            ECsSocketOpened,
            
            /**
            * Connection has been established
            */
            ECsConnectionCreated,
            
            /**
            * Connection environment fully initialized
            */
            ECsConnected
            };
        
        
    public:     // Constructors and destructor
        /**
        * Factory function.
        * @since S60 5.0
        * @param aServerCloser Reference to the server closer object. 
        *                      Ownership not passed.
        * @return Class instance
        */
        static CWsfWlanBearerConnectionMonitor* NewL( 
                                 MWsfServerCloserAdapter& aServerCloser );
                                 
        /**
        * Factory function.
        * @since S60 5.0
        * @param aServerCloser Reference to the server closer object. 
        *                      Ownership not passed.
        * @return Class instance
        */
        static CWsfWlanBearerConnectionMonitor* NewLC( 
                                 MWsfServerCloserAdapter& aServerCloser );
                                 
        /**
        * Destructor.
        */
        ~CWsfWlanBearerConnectionMonitor();

    private:    // Constructors
        /**
        * Constructor
        * @since S60 5.0
        * @param aServerCloser Reference to the server closer object. 
        *                      Ownership not passed.
        */
        CWsfWlanBearerConnectionMonitor( 
                                 MWsfServerCloserAdapter& aServerCloser );

        /**
        * Second-phase constructor.
        * @since S60 5.0
        */
        void ConstructL();

    public:     // New methods
        
        /**
        * Returns the name of the active wlan connection
        * @since S60 5.0
        * @return Connection name or KNullDesC if not connected
        */
        const TDesC& GetWlanBearerNameL();

        /* 
        * Starts monitoring WLAN events through the given observer
        * @since S60 5.0
        * @param aObserver The object to use for monitoring. Ownership 
        *                  not passed.
        */    
        void StartMonitoringL( MWsfWlanMonitorObserver* aObserver );
        
        /*
        * Stops monitoring WLAN events
        * @since S60 5.0
        */
        void StopMonitoring();
        
        /*
        * Connects to the given IAP
        * @since S60 5.0
        * @param aIapId WLAN IAP id to connect to.
        * @param aConnectOnly ETrue if Connect selected
        * @param aTestAccessPoint ETrue if accesspoint needs testing
        * @return KErrNone on success, error code otherwise
        */
        TInt ConnectBearer( TUint32 aIapId,
                            TBool aConnectOnly,
                            TBool aTestAccessPoint );

        /*
        * Disconnects active connection
        * @since S60 5.0
        * @return ETrue if the was something to disconnect otherwise EFalse
        */
        TBool DisconnectBearer();
 
        /**
        * Starts to monitor Access Point
        * @since S60 5.0
        * @param aIapId Access point to be monitored
        */
        void MonitorAccessPoint( TUint32 aIapId );   
        
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
        * @return KErrNone on success, error code otherwise
        */
        TInt AbortConnecting();
    
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
        
   
    private:    // From CActive
        /**
        * Implements cancellation of an outstanding request.
        * @since S60 5.0
        */
        void DoCancel();

        /**
        * Handles an active object's request completion event.
        * @since S60 5.0
        */
        void RunL();

        /**
        * Handles a leave occurring in RunL().
        * @since S60 5.0
        * @param aError Leave code
        * @return aError
        */
        TInt RunError( TInt aError );
    
    
    protected:  // From MConnectionMonitorObserver
    
        /**
        * Event method is called every time an event occures.
        * @since S60 5.0
        * @param aConnMonEvent The event information.
        */        
        void EventL( const CConnMonEventBase& aConnMonEvent );
    
        
    public: // from MWsfWlanConnectionDetailsProvider
    
        /**
        * Fills TWsfWlanInfo with the connected wlan's properties
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo structure to be filled
        * @return ETrue if there is a connection and returned data is valid, 
        *         EFalse otherwise
        */ 
        TBool ConnectedWlanConnectionDetailsL( TWsfWlanInfo* aWlanInfo );
    
        
    private:    // New methods
    
        /**
        * Loops through the list of current connections and searches for one
        * with wlan bearer
        * @since S60 5.0
        */ 
        void FindWlanBearerConnectedL();

        /**
        * Checks connection details and retrieves WLAN connection name
        * @since S60 5.0
        * @param aConnectionId The connection in question
        * @return ETrue on success (WLAN connection was found), 
        *         EFalse otherwise
        */
        TBool CheckConnectionDetailsL( TUint aConnectionId );
        
        /**
        * Shuts down the owned connection 
        * @since S60 5.0
        */
        void ShutdownOwnedConnectionL();

        /**
        * Cleanup function to release the shutdown mutex if shutdown leaves
        * @since S60 5.0
        * @param aPtr Pointer for this class
        */
        static void ReleaseShutdownMutex( TAny* aPtr );

        /**
        * Callback for CPeriodic to check the client count and 
        * start inactivity timer if needed.
        * @since S60 5.0
        * @param aPtr Pointer for this class
        * @return KErrNone if successful, system-wide error code otherwise.
        */
        static TInt CheckClientCount( TAny* aPtr );

        /**
        * Function that does actual processing in checking the client count.
        * Called by CheckClientCount().
        * @since S60 5.0
        */
        void CheckClientCountL();
        
        
    private:    // Data
        
        /**
        * Reference to the observer of this class (not owned)
        */
        MWsfWlanMonitorObserver* iObserver;
        
        /**
        * Handle to the connection monitor
        */
        RConnectionMonitor iMonitor;
        
        /**
        * Name of the currently active WLAN connection
        */
        TBuf<KWlanMaxAccessPointNameLength> iWlanNetworkName;
        
        /**
        * Id of the currently active WLAN connection
        */
        TInt iConnectionId;
        
        /**
        * Indicates whether connection monitoring is on
        */
        TBool iMonitoring;
        
        /**
        * The access point to be monitored
        */
        TUint32 iMonitoredAp;

        /**
        * Handle to the socket server for making a connection
        */
        RSocketServ iSocketServ;
        
        /**
        * Handle to the owned connection
        */
        RConnection iConnection;
        
        /**
        * Indicates whether the connection was initiated from within Sniffer
        */
        TBool iConnectionOwned;
        
        /**
        * Default connection settings before creating an own connection
        */
        TCmDefConnValue iOriginalDefConn;
        
        /**
        * Default connection settings after the owned connection has been
        * established
        */
        TCmDefConnValue iCurrentDefConn;
        
        /**
        * Periodic polling for connection client count
        */    
        CPeriodic* iClientPoll;
        
        /**
        * Indicates whether timer-based auto-disconnect is enabled
        */
        TBool iAutoDisconnect;
        
        /**
        * Count of clients using the connection
        */
        TInt iClientCount;
        
        /**
        * Timer to measure time elapsed without activity          
        */
        TTime iInactivityStart;
    
        /**
        * Status of currently ongoing connecting process
        */
        TWsfConnectingState iConnectingState;

        /**
        * IAP id to connect to. Used by the connecting active object.
        */
        TUint32 iConnIap;        
        
        /**
        * Indicates whether the connection is being aborted
        */
        TBool iAborting;
        
        /**
        * Asserts that the shutdown process is not reentered
        */
        TBool iShutdownMutex;
        
        /**
        * Asserts that the client counting process is not reentered
        */
        TBool iClientCountMutex;
        
        /**
        * Handle to the CmManager
        */
        RCmManagerExt iCmMgr;
        
        /**
        * Reference to the server closer (not owned)
        */
        MWsfServerCloserAdapter& iServerCloser;
        
        /**
        * Reference to ICT handler
        */
        CWsfIct* iIct;
        
    };


#endif // C_WSFWLANBEARERCONNECTIONMONITOR_H

