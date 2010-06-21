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
* Description:  Class header for CWsfWlanScanner
*
*/



#ifndef C_WSFWLANSCANNER_H
#define C_WSFWLANSCANNER_H

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <wlanmgmtcommon.h>
#include <commsdattypesv1_1.h>
#include <cmmanagerext.h>



//  INTERNAL INCLUDES
#include "wsfwlanscannerobserver.h"
#include "wsfwlanmonitorobserver.h"
#include "wsfwlanscanintervalchangeobserver.h"
#include "wsfwlaninfo.h"


const TUint8 TX_RATE_BASIC_MASK = 0x80;


/**
 * Defines the possible TX rate values.
 */
typedef enum _core_tx_rate_e
    {
    core_tx_rate_none       = 0x00000000,
    core_tx_rate_1mbit      = 0x00000001,
    core_tx_rate_2mbit      = 0x00000002,
    core_tx_rate_5p5mbit    = 0x00000004,
    core_tx_rate_6mbit      = 0x00000008,
    core_tx_rate_9mbit      = 0x00000010,
    core_tx_rate_11mbit     = 0x00000020,
    core_tx_rate_12mbit     = 0x00000040,
    core_tx_rate_18mbit     = 0x00000080,
    core_tx_rate_22mbit     = 0x00000100,
    core_tx_rate_24mbit     = 0x00000200,
    core_tx_rate_33mbit     = 0x00000400,
    core_tx_rate_36mbit     = 0x00000800,
    core_tx_rate_48mbit     = 0x00001000,
    core_tx_rate_54mbit     = 0x00002000
    } core_tx_rate_e;

const TUint32 CORE_TX_RATES_802P11B =
    core_tx_rate_1mbit |
    core_tx_rate_2mbit |
    core_tx_rate_5p5mbit |
    core_tx_rate_11mbit;

const TUint32 CORE_TX_RATES_802P11G =
    core_tx_rate_6mbit |
    core_tx_rate_9mbit |
    core_tx_rate_12mbit |
    core_tx_rate_18mbit |
    core_tx_rate_22mbit |
    core_tx_rate_24mbit |
    core_tx_rate_33mbit |
    core_tx_rate_36mbit |
    core_tx_rate_48mbit |
    core_tx_rate_54mbit;

/**
 * Define the possible TX rate value as units of 500kbit/s.
 */
typedef enum _core_tx_rate_value_e
    {
    core_tx_rate_value_none    = 0,
    core_tx_rate_value_1mbit   = 2,
    core_tx_rate_value_2mbit   = 4,
    core_tx_rate_value_5p5mbit = 11,
    core_tx_rate_value_6mbit   = 12,
    core_tx_rate_value_9mbit   = 18,
    core_tx_rate_value_11mbit  = 22,
    core_tx_rate_value_12mbit  = 24,
    core_tx_rate_value_18mbit  = 36,
    core_tx_rate_value_22mbit  = 44,
    core_tx_rate_value_24mbit  = 48,
    core_tx_rate_value_33mbit  = 66,
    core_tx_rate_value_36mbit  = 72,
    core_tx_rate_value_48mbit  = 96,
    core_tx_rate_value_54mbit  = 108,
    } _core_tx_rate_value_e;


//  FORWARD DECLARATIONS
class CWlanMgmtClient;
class CWlanScanInfo;
class CWsfWlanSettingsAccessor;
class CWsfWlanInfoArray;
class TWsfWlanInfo;
class MWsfWlanConnectionDetailsProvider;
class MWsfKnownIapRecognizer;


//  CLASS DEFINITION
/**
 * Class to schedule WLAN scanning and process the resultant data
 *
 * @since S60 5.0
 * @lib wsfserver.exe
 */
NONSHARABLE_CLASS( CWsfWlanScanner ): public CActive, 
                                      public MWlanMgmtNotifications, 
                                      public MWsfWlanMonitorObserver,
                                      public MWsfWlanScanIntervalChangeObserver
    {
    private:     // Type definitions
    
        /**
        * States of the scanning process
        */
        enum TWsfScanState 
            {
            /**
            * Not scanning but scheduled
            */
            EIdle,
            
            /**
            * Do broadcast scan
            */
            EBroadcastScan,
            
            /**
            * Processing broadcast scan results
            */
            EProcessBroadcastScan,
            
            /**
            * Processing direct scan results
            */
            EDirectScan,
            
            /**
            * Scanning finished, cleanup and notification
            */
            EFinished
            };
            
    public:     // Constructors and destructor

        /**
        * Factory function.
        * @since S60 5.0
        * @param aDbSession Reference to the database session to use
        * @return Class instance
        */
        static CWsfWlanScanner* NewL( CommsDat::CMDBSession& aDbSession );

        /**
        * Factory function.
        * @since S60 5.0
        * @param aDbSession Reference to the database session to use
        * @return Class instance
        */
        static CWsfWlanScanner* NewLC( CommsDat::CMDBSession& aDbSession );

        /**
        * Destructor.
        */
        ~CWsfWlanScanner();


    private:    // Constructors
    
        /**
        * Constructor
        * @since S60 5.0
        * @param aDbSession Reference to the database session to use
        */
        CWsfWlanScanner( CommsDat::CMDBSession& aSession );

        /**
        * Second-phase constructor.
        * @since S60 5.0
        */
        void ConstructL();


    public:     // New methods
        /**
        * Sets the observer of this class
        * @since S60 5.0
        * @param aObserver The observer to be notified of scanning events
        */
        void SetObserver( MWsfWlanScannerObserver& aObserver );

        /**
        * Enables scanning for wlans
        * @since S60 5.0
        */
        void StartScanningL();

        /**
        * Stops the activation timer and cancels the mgmt client to 
        * give notifications
        * @since S60 5.0
        */
        void StopScanning();      
        
        /**
        * Initiates a new scanning if it was already enabled but was idle.
        * @since S60 5.0
        * @return ETrue if scanning was in fact restarted
        */
        TBool RestartScanning();

        /**
        * Aborts the scanning process if it was active. However, it does not 
        * disable scanning
        * @since S60 5.0
        */
        void AbortScanning();
        
        /**
        * Returns the serialized contents of scan results 
        * @since S60 5.0
        * @return A serialized array of wlaninfo items
        */       
        HBufC8* ScanResults();
        
        /**
        * Sets ConnectionDetails provider handle to get data about 
        * connected wlans
        * @since S60 5.0
        * @param aProvider The object to be asked for connection details
        */ 
        void SetConnectionDetailProvider( 
                               MWsfWlanConnectionDetailsProvider& aProvider );


    public:     // From MWlanMgmtNotifications
        /**
        * Connection status has changed.
        * @since S60 5.0
        * @param aNewState The new connection state
        */
        void ConnectionStateChanged( TWlanConnectionMode aNewState );
    
        /**
        * BSSID has changed (i.e. AP handover).
        * @since S60 5.0
        * @param aNewBssid The new BSSID
        */
        void BssidChanged( TWlanBssid& aNewBssid );

        /**
        * Connection has been lost.
        * @since S60 5.0
        */
        void BssLost();

        /**
        * Connection has been regained.
        * @since S60 5.0
        */
        void BssRegained();

        /**
        * New networks have been detected during scan.
        * @since S60 5.0
        */
        void NewNetworksDetected();

        /**
        * One or more networks have been lost since the last scan.
        * @since S60 5.0
        */
        void OldNetworksLost();

        /**
        * The used transmit power has been changed.
        * @since S60 5.0
        * @param aPower The transmit power in mW.
        */
        void TransmitPowerChanged( TUint aPower );
        
        /**
        * Received signal strength level has been changed.
        * @since S60 5.0
        * @param aRssClass specifies the current class of the received signal
        * @param aRss RSS level in absolute dBm values.
        */
        void RssChanged( TWlanRssClass aRssClass, TUint aRss );
        

    private:    // New methods
        /** 
        * Processes the results of the broadcast scan
        * @since S60 5.0
        */
        void DoScanForNetworksL();
           
        /**
        * Compares two SSIDs
        * @since S60 5.0
        * @param aSsid1 First SSID
        * @param aSsid2 Second SSID
        * @return ETrue if the SSIDs are the same, EFalse otherwise
        */
        static TBool SsidIdentity( const TWlanSsid& aSsid1, 
                                   const TWlanSsid& aSsid2 );

#ifdef _DEBUG
        /**
        * Dumps the scan results to the log
        * @since S60 5.0
        * @param aArray Array of wlaninfo items added during scanning
        */
        static void DumpScanResultsL( CWsfWlanInfoArray* aArray );
#endif

        /**
        * Check if the given SSID implies a hidden WLAN
        * @since S60 5.0
        * @param aSsidLength Length of the SSID passed
        * @param aSsid SSID as array of bytes
        * @return ETrue if the network in question is hidden, EFalse otherwise
        */
        TBool IsHiddenSsid( TUint aSsidLength, const TUint8* aSsid );

        /**
        * Parses the scan results for the network name
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo to put the results in
        * @return ETrue if the network is a hidden one
        */
        TBool RefreshNetworkNameL( TWsfWlanInfo& aWlanInfo );

        /**
        * Parses the scan results for the wlan signal stregth 
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo to put the results in
        */
        void RefreshSignalStrength( TWsfWlanInfo& aWlanInfo );

        /**
        * Parses the scan results for the network mode
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo to put the results in
        */
        void RefreshNetworkMode( TWsfWlanInfo& aWlanInfo );

        /**
        * Parses the scan results for the security mode
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo to put the results in
        */
        void RefreshSecurityMode( TWsfWlanInfo& aWlanInfo );

        /**
        * Convert the given rate value (500kbit/s per unit) 
        * to a corresponding enum.
        * @since S60 5.2
        * @param aRate Rate value to be converted.
        * @return Corresponding rate enum.
        */
        core_tx_rate_e ConvertTxRateToTxRateEnum( TUint8 aRate );

        /**
        * Parses the scan results for the technology information
        * @since S60 5.2
        * @param aWlanInfo The wlaninfo to put the results in
        */
        void RefreshTechnology( TWsfWlanInfo& aWlanInfo );

        /**
        * Prepares for direct scanning
        * @since S60 5.0
        */
        void PrepareDirectScan();

        /**
        * Processes the result of the last direct scan
        * @since S60 5.0
        */
        void ProcessDirectScanResultL();
        
        /**
        * Checks iScanArray and replaces SSIDs of known networks
        * with their IAP names. 
        * @param aWlanInfo The wlaninfo to put the results in
        * @since S60 5.0
        */
        void ReplaceSsidsWithIapName(TWsfWlanInfo& aWlanInfo);
        
        /**
        * Updates IAPs priority
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo to put the results in
        */
        void UpdatePriorityL( TWsfWlanInfo& aWlanInfo );
        
        /**
         * Finds the security mode, network mode from WLAN table
         * with their IAP names. 
         * @param aWlanInfo The wlaninfo to put the results in
         * @since S60 5.0
         */
        void GetWlanInfoFromIapL( TWsfWlanInfo& aWlanInfo );
        
        /**
         * Add connected wlan info to scan results
         * @since S60 5.0
         */
        void AddConnectedWLANInfoL();
        
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
        

    public: // from MWsfWlanMonitorObserver

        /**
        * Called when a wlan connection is established
        * @since S60 5.0
        * @param aConnectionName WLAN connection name (SSID)
        */
        void ConnectionEstablishedL( const TDesC& aConnectionName );

        /**
        * Called when wlan connection has been lost
        * @since S60 5.0
        */
        void ConnectionLostL();
        
        /**
        * Called when the connection process failed for some reason
        * @since S60 5.0
        * @param aError System wide error code
        */
        void ConnectingFailedL( TInt aError );
        
        /**
        * Called when the connection no longer needs the IAP it was using
        * @since S60 5.0
        */
        void ConnectedIapReleasedL();
        
        
    public: // from MWsfWlanScanIntervalChangeObserver
        /**
        * Called when the value of the background scan interval or the
        * show availability flag has been changed
        * @since S60 5.0
        * @param aNewScanInterval The new value for the scan interval
        * @param aShowAvailability The new value for the 
        *                          Show WLAN availability flag
        */
        void WlanScanIntervalChangedL( TUint aNewScanInterval,
                                       TBool aShowAvailability );

    private:    // Data
        /**
        * Reference to the observer of this class (not owned)
        */
        MWsfWlanScannerObserver* iObserver; 
        
        /**
        * Reference to the database session to use (not owned)
        */
        CommsDat::CMDBSession* iDbSession; 
        
        /**
        * Reference to the connection details providing object (not owned)
        */
        MWsfWlanConnectionDetailsProvider* iConnectionDetailsProvider;
        
        /**
        * Handle to the WLAN management client (owned)
        */
        CWlanMgmtClient* iWlanMgmtClient;
        
        /**
        * Scan info object (owned)
        */
        CWlanScanInfo* iScanInfo;
        
        /**
        * WLAN settings monitoring object (owned)
        */        
        CWsfWlanSettingsAccessor* iWlanSettingsAccessor;
        
        /**
        * The array containing the wlaninfo items found during scanning 
        * (owned)
        */
        CWsfWlanInfoArray* iScanArray;
        
        /**
        * Name of the active connection (owned)
        */
        HBufC* iActiveConnectionName;
        
        /**
        * Timer for scheduling WLAN scans
        */
        RTimer iTimer;
        
        /**
        * Handle to the CmManager
        */
        RCmManagerExt iCmManagerExt;
        
        /**
        * Buffer to store the serialized array of wlaninfo items (owned)
        */
        HBufC8* iScanResults;
        
        /**
        * The background scan interval in microseconds
        */
        TUint iScanningInterval;

        /**
        * Value of Show WLAN availability flag
        */
        TBool iShowAvailability;
        
        /**
        * Array of SSIDs that are in CommsDat but have not been found 
        * by broadcast scan
        */
        RArray<TWlanSsid> iDirectScanSsids;
        
        /**
        * Array of IapIDs that are in CommsDat but have not been found 
        * by broadcast scan
        */
        RArray<TUint> iDirectScanIapIDs;
        
        /**
        * Indicates the scanning state
        */
        TWsfScanState iScanState;
             
        /**
        * Connected SSID or IAP name
        */
        TBuf8<KWlanMaxAccessPointNameLength> iConnectedSsidOrIap;
        
		/**
        * Array containing the available IAP IDs and Signal Strengths
        */
        RArray<TWlanIapAvailabilityData> iAvailableIaps;
        
        /**
        * Cache lifetime.
        * Parameter needed for issuing "get available WLAN IAPs"
        * request.
        */
        TInt iCacheLifetime;
        
        /**
        * Max Delay.
        * Parameter needed for issuing "get available WLAN IAPs"
        * request.
        */
        TUint iMaxDelay; 

        
    };


#endif      //  C_WSFWLANSCANNER_H

// End of file
