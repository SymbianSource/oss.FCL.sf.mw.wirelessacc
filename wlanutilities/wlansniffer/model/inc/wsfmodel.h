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



#ifndef C_WSFMODEL_H
#define C_WSFMODEL_H


//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfsession.h"
#include "wsfbrowserlaunchobserver.h"
#include "wsfscreensaverstateobserver.h"
#include "wsfbrowserlaunchadapter.h"
#include "wsfwlaninfo.h"


//  FORWARD DECLARATIONS
class CWsfAppLauncher;
class MWsfModelObserver;
class CWsfWlanInfoArray;
class MWsfStateChangeObserver;
class CEikonEnv;
class CWsfScreenSaverWatcher;
class CWsfAsyncOperationQueue;


//  CLASS DEFINITION
/**
 * Class acts as an application level model and is also entry point to the 
 * application engine which is implemented as server
 *
 * @lib wsfmodel.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CWsfModel ): public CBase, 
                                public MWsfBrowserLaunchObserver, 
                                public MWsfScreenSaverStateObserver,
                                public MWsfBrowserLaunchAdapter
    {
    public:     // Constructors and destructor

        /**
        * Factory function.
        * @since S60 5.0
        * @param aObserver The object to notify about server events
        * @param aScreenSaverAware Whether to react to screensaver events
        * @return Model instance
        */
        IMPORT_C static CWsfModel* NewL( MWsfStateChangeObserver& aObserver, 
                                         const TBool aScreenSaverAware );

        /**
        * Factory function.
        * @since S60 5.0
        * @param aObserver The object to notify about server events
        * @param aScreenSaverAware Whether to react to screensaver events
        * @return Model instance
        */
        IMPORT_C static CWsfModel* NewLC( MWsfStateChangeObserver& aObserver, 
                                          const TBool aScreenSaverAware );

        IMPORT_C ~CWsfModel();

    private:    // Constructors
        CWsfModel();

        /**
        * Factory function.
        * @since S60 5.0
        * @param aObserver The object to notify about server events
        * @param aScreenSaverAware Whether to react to screensaver events 
        */
        void ConstructL( MWsfStateChangeObserver& aObserver, 
                         const TBool aScreenSaverAware );

    public:     // New methods

        /**
        * Attach model observer 
        * @since S60 5.0
        * @param aObserver Pointer to the model observer
        */
        IMPORT_C void SetEngineObserver( MWsfModelObserver* aObserver );

        /**
        * Return pointer to the list of WLANs. Ownership not passed.
        * @since S60 5.2
        * @return Array of WLANs
        */
        inline CWsfWlanInfoArray* WlanList();
        
        /**
        * Return the latest list of found WLANs. Ownership not passed.
        * @since S60 5.0
        * @return Array of WLANs
        */
        IMPORT_C CWsfWlanInfoArray* GetWlanListL();
        
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
        * Store active views UID
        * @since S60 5.0
        * @param aActiveViewId Active view's UID
        */
        IMPORT_C void SetActiveViewL( TInt aActiveViewId );
        
        /**
        * Get UID of the active view
        * @since S60 5.0
        * @return UID of active view
        */
        IMPORT_C TUid ActiveViewL();

        /**
        * Store SSID of active WLAN
        * @since S60 5.0
        * @param aSsid WLAN's name (SSID)
        */
        IMPORT_C void SetObservedWlanL( const TDesC8& aSsid );
        
        /**
        * Get SSID of active WLAN
        * @since S60 5.0
        * @return Active WLAN's name (SSID) 
        */
        IMPORT_C const TDesC8& ObservedWlan();

        /**
        * Connects to the given WLAN IAP
        * @since S60 5.0
        * @param aIapId WLAN IAP id to connect to.
        * @param aConnectOnly ETrue if Connect selected
        * @param aPersistence The value of the persistence property
        */
        IMPORT_C int ConnectL( TUint32 aIapId, 
                               TBool aConnectOnly,
                               TWsfIapPersistence aPersistence );
        
        /**
        * Asyncronous request to the server to connect to the given WLAN IAP
        * @since S60 5.2
        * @param aPckg request result
        * @param aIapId WLAN IAP id to connect to.
        * @param aConnectOnly ETrue if Connect selected
        * @param aPersistence The value of the persistence property
        * @param aStatus The request status object used to contain 
        *        the completion status of the request.
        */
        IMPORT_C void ConnectL( TPckgBuf<TBool>& aPckg, TUint32 aIapId,
                                TBool aConnectOnly,
                                TWsfIapPersistence aPersistence,
                                TRequestStatus& aStatus );

        /**
        * Sets connection result
        * @since S60 5.2
        * @param aResult connection result
        * @param aIapId WLAN IAP
        */
        IMPORT_C void SetConnectResultL( TInt aResult, TUint aIapId );

        /**
        * Disconnects WLAN
        * @since S60 5.0
        */
        IMPORT_C void DisconnectL();
        
        /**
        * Asyncronous request to the server to disconnect
        * @since S60 5.2
        * @param aPckg request result
        * @param aStatus The request status object used to contain 
        *        the completion status of the request.
        */
        IMPORT_C void Disconnect( TPckgBuf<TBool>& aPckg, 
                                  TRequestStatus& aStatus );
        
        /**
        * Check if the engine is refreshing
        * @since S60 5.0
        * @return ETrue if first refreshing
        */
        IMPORT_C TBool Refreshing();    

        /**
        * Change refreshing state
        * @since S60 5.0
        * @param aRefreshing Refreshing state
        */
        IMPORT_C void SetRefreshState( TBool aRefreshing );    

        /**
        * Notifies server to start scanning if not scanning already
        * @since S60 5.0
        * @return ETrue on success
        */
        IMPORT_C TBool EnableScanL();

        /**
        * Notifies server to disable scanning
        * However, scanning will continue if some other client 
        * has enabled scanning.
        * @since S60 5.0
        * @return ETrue on success
        */
        IMPORT_C TBool DisableScanL();
        
        /**
        * Checks if scanning is enabled in the server
        * @since S60 5.0
        * @return EFalse if not scanning
        */
        IMPORT_C TBool IsScanEnabledL();
             
        /**
        * Continues browsing
        * @since S60 5.0
        */
        IMPORT_C void ContinueBrowsingL();        

        /**
        * Continues browsing (browser launched if needed)
        * @since S60 5.0
        * @param aIapId The IAP id to use with the browser
        */
        IMPORT_C void ContinueBrowsingL( TUint32 aIapId );  
        
        /**
        * Creates new WLAN access point based on wlaninfo passed as parameter.
        * If aExplicitDefine is ETrue, the connection method is saved to the 
        * destination selected by the user, otherwise it is put into the 
        * Uncategorized folder.
        * @since S60 5.0
        * @param aWlan Reference to Wlan which is used as base 
        *              for new access point
        * @param aExplicitDefine Should be ETrue in case of 
        *                        'Define access point'
        * @return ETrue if access point creation succeeds
        */
        IMPORT_C TBool CreateAccessPointL( TWsfWlanInfo& aWlan,
                                           TBool aExplicitDefine );

        /**
        * Requests a rescan for available wlans
        * @since S60 5.0
        * @return ETrue on success
        */
        IMPORT_C TBool RefreshScanL();
        
		/**
        * Asyncronous request to the server to make a scan
        * @since S60 5.2
        * @param aPckg request result
        * @param aStatus The request status object used to contain 
        *        the completion status of the request.
        */
        IMPORT_C void RefreshScan( TPckgBuf<TBool>& aPckg, 
                                   TRequestStatus& aStatus );
        
        /**
        * Closes helper application if it is launched 
        * @since S60 5.2
        * @return ETrue if helper application task was closed
        */
        IMPORT_C TBool CloseHelperApplication();
        
        /**
        * Launches and helper application to create an accesspoint 
        * @since S60 5.2
        * @param aWlanInfo The WlanInfo object to be passed to the helper app
        */
        IMPORT_C void LaunchHelperApplicationL( TWsfWlanInfo& aWlanInfo );
        
        /**
        * Cancel notify events from server
        * @since S60 5.0
        */
        IMPORT_C void CancelNotifyEvents();
        
        /**
        * Request notify events from server
        * @since S60 5.0
        * @param aObserver Instance of MWsfStateChangeObserver to handle events
        */ 
        IMPORT_C void RequestNotifyEventsL( 
                                        MWsfStateChangeObserver& aObserver );
        
        /**
        * Queries if there is an active WLAN connection.
        * @since S60 5.0
        * @return ETrue if there is an active WLAN connection, 
        *         EFalse otherwise.
        */
        IMPORT_C TBool IsConnectedL();
        
        /**
        * If there is an active WLAN connection, returns its WLAN info.
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo object to fill
        * @return ETrue if success, EFalse otherwise.
        */
        IMPORT_C TBool GetConnectedWlanDetailsL( TWsfWlanInfo& aWlanInfo );
        
        /**
        * Asyncronous request to the server to return connected network
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
        * Queries whether a connecting process is going on
        * @since S60 5.0
        * @return ETrue if connection is in progress
        */
        inline TBool IsConnecting() const;
        
        /**
        * Sets connecting status
        * @since S60 5.0
        * @param aIsConnecting The new status of the connecting process
        */
        inline void SetConnecting( TBool aIsConnecting );
        
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
        * Check if Iap Id is valid
        * Leaves with system wide error code if Iap ID is 
        * not valid or checking fails
        * @since S60 5.2
		* @param aIapId IAP id passed as a parameter
        */        
        IMPORT_C void CheckIsIapIdValidL( TUint aIapId ) const;

    public: // from  MWsfBrowserLaunchAdapter        
        /**
        * Launches Browser application using selected iap
        * @since S60 5.0
        * @param aIapId IAP id passed as a parameter
        */
        IMPORT_C void LaunchBrowserL( TUint32 aIapId );

        
    public:     // From MWsfBrowserLaunchObserver

        /**
        * Called if browser launch succeeds
        * @since S60 5.0
        */
        void BrowserLaunchCompleteL();

        /**
        * Called if browser launch failed
        * @since S60 5.0
        * @param aError System-wide error code    
        */
        void BrowserLaunchFailed( TInt aError );
     
        /**
        * Called when the previously launched browser exits
        * @since S60 5.0
        */
        void BrowserExitL();
        
        
    private:    // from MWsfScreenSaverStateObserver
        
        /**
        * Called if the status of the screensaver has been changed
        * @since S60 5.0
        * @param aScreenSaverActive Current status of the screensaver
        */
        void ScreenSaverStatusChangedL( const TBool aScreenSaverActive );
        

    private:  // New methods
        
        /**
        * Check if there is disk space left 
        * Leaves with KErrDiskFull is space is below CL
        * @since S60 5.0
        */
        void CheckSpaceBelowCriticalLevelL() const;
        
        /**
        * Check if access point to be created is
        * unknow and Network encryption is WAPI
        * Leaves with KErrWlanProtectedSetupSetupLocked 
        * in the case of unknown WAPI AP.
        * @since S60 5.2
        */        
        void CheckUnknownWapiL( TWsfWlanInfo& aWlan ) const;

    private:    // Data
        /**
        * Reference to the current UI environment (not owned)
        */
        CEikonEnv* iEikEnv;

        /**
        * Reference to the model observer (not owned)
        */
        MWsfModelObserver* iObserver;

        /**
        * Browser launcher class. Owned.
        */
        CWsfAppLauncher* iBrowserLauncher;

        /**
        * Screen saver watcher class. Owned.
        */
        CWsfScreenSaverWatcher* iScreenSaverWatcher;

        /**
        * Client-side handle for server session
        */
        RWsfSession iSession;

        /**
        * Array to store WLAN scan results. Owned.
        */
        CWsfWlanInfoArray* iArray;

        /**
        * UID of active view
        */
        TUid iActiveView;        

        /**
        * Name of currently observed WLAN
        */
        HBufC8* iObservedWlan;

        /**
        * Id of currently connected IAP (if applicable)
        */
        TUint iConnectedIapId;

        /**
        * Refreshing status
        */
        TBool iRefreshing;
        
        /**
        * Indicates whether connection creation has been started but not
        * finished yet.
        */
        TBool iConnecting;

    };


#include "wsfmodel.inl"



#endif      //  C_WSFMODEL_H

// End of file
