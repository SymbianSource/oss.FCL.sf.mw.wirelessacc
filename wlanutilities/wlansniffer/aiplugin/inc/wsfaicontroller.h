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
* Description:  Class header for TWsfAiController
*
*/

#ifndef T_WSFAICONTROLLER_H
#define T_WSFAICONTROLLER_H

//  EXTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>
#include <eikenv.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"
#include "wsfstatechangeobserver.h"
#include "wsfmodelobserver.h"


//  FORWARD DECLARATIONS
class CWsfModel;
class CWsfAiModel;
class MWsfAiUiObserver;
class TWsfWlanInfo;
class CWsfWlanInfoArray;
class CWsfDbObserver;
class CWsfActiveWrappers;


/**
 *  The controller class of the component. All functionality is reached
 *  through this class and all needed events are observed by this.
 *
 *  @lib wsfaiplugin.lib
 *  @since S60 v5.0
 */

NONSHARABLE_CLASS( TWsfAiController ): public MWsfStateChangeObserver,
                                       public MWsfModelObserver
    {
    public:     // Constructors
        
        /**
        * Default C++ constructor 
        * @since S60 5.0 
        */
        TWsfAiController();
 
    public:     // New methods
        
        /**
         * DeInitialization for this class 
         */
        void DeInitializeL();

        /**
        * PreInitialization for this class (set the models references)
        * @since S60 5.0
        * @param aModel applicaton level model reference
        * @param aAiModel model reference of AiPlugin
        * @param aActiveWrappers reference of wrappers holder
        */
        void InitializeL( CEikonEnv* aEnv, CWsfModel* aModel, 
                          CWsfAiModel* aAiModel, CWsfDbObserver* aObserver, 
                          CWsfActiveWrappers* aActiveWrappers );
        
        /**
        * Set the UI for the controller
        * @since S60 5.0
        * @param aUi reference to AiPlugin UI control
        */
        void SetUi( MWsfAiUiObserver& aUi );
        

        /**
        * Handles keypresses coming from container
        * @since S60 5.0
        */
        void HandleSelectionKeyL();
        
        /**
        * Start the sequential scanning for wlans
        * @since S60 5.0
        */ 
        void EnableScanL();
   
        /**
        * Stop the sequential scanning for wlans
        * @since S60 5.0
        */ 
        void DisableScanL();
        
        /**
         * Start initial refresh
         * to the AI model
         * @since S60 5.0
         */
        void StartupRefresh();
        
        /**
         * Propagates the network data from the engine
         * to the AI model
         * @since S60 5.2
         */
        void StartupRefreshDataReadyL();
        
        /**
        * Refreshes the refreshing icon
        * @since S60 5.0
        */
        void RefreshRefreshingL();
        
        /**
        * Dismisses the AI menu/other dialogs if there was any open
        * @since S60 5.2
        */
        void DismissDialogsL();
        
        /**
        * Refreshes the connecting icon
        * @since S60 5.2
        */
        void RefreshConnectingL();
        
        /**
         * Propagates the network data from the engine
         * to the AI model
         * @since S60 5.2
         */
        void WlanListDataReadyL();
        
        /**
        * Connect to the given wlan
        * @since S60 5.2
        * @param aInfo Wlaninfo to connect with
        * @param aConnectOnly ETrue if we are only connecting, 
        *                     EFalse if we should also launch the browser
        * @param aTestAccessPoint ETrue if ICT is executed, 
        *                         EFalse if ICT is not executed   
        */
        void ConnectL( TWsfWlanInfo& aInfo, TBool aConnectOnly, 
                       TBool aTestAccessPoint );
        

    public:     // From MWsfStateChangeObserver

        /**
        * The current wlan data has changed and it is ready to be retrieved
        * @since S60 5.0
        */
        void WlanListChangedL();

        /**
        * Engine error occured
        * @since S60 5.0
        * @param aError System wide error code
        */
        void NotifyEngineError( TInt aError );
        
        /**
        * Scanning has been disabled
        * @since S60 5.0
        */
        void ScanDisabledL();
        
        /**
        * Scanning has been enabled
        * @since S60 5.0
        */
        void ScanEnabledL();

        /**
        * A WLAN connection has been established
        * @since S60 5.0
        * @param aIapId Access point id
        */
        void WlanConnectionActivatedL( TInt32 aIapid );
        
        /**
        * A WLAN connection has been closed
        * @since S60 5.0
        * @param aIapId Access point id
		*/
        void WlanConnectionClosedL( TInt32 aIapid );
        
        /**
        * Connection creation process finished
        * @since Symbian^3
        * @param aIapId Access point id
        * @param aError System wide error code
        */
        void ConnectionCreationProcessFinishedL( TInt32 aIapid, TInt aError );
        
        
    public:     //From MWsfModelObserver
        
        /**
         * Called if the launch has been completed successfully
         * @since S60 5.0 
         */
        void BrowserLaunchCompleteL();
        
        /**
         * Called if the browser launch failed for some reason
         * @since S60 5.0 
         * @param aError error code
         */
        void BrowserLaunchFailed( TInt aError );
    
        /**
         * Called when the previously launched browser exits
         * @since S60 5.0
         */
        void BrowserExitL();

        /**
         * Notification to wlan widget when browser is used
         * @since Symbian^3
         */
        void HandleMskIfConnectingL();
        
        /**
         * Notification to wlan widget when just connected
         * @since Symbian^3
         */
        void HandleMskIfOfflineL();
        
    public: // From MWsfModelObserver
        
        /**
         * Notification that connection is going to be created
         * @since S60 5.0
         * @param aIapId The IAP id of the connection being created
         */
        void ConnectingL( TUint32 aIapId );
    
        /**
         * Notification that the connection creation process ended
         * @since S60 5.0
         * @param aResult The result of the connection creation
         */
        void ConnectingFinishedL( TInt aResult );
           
       /**
        * Handle MSK if connected to a WLAN
        * @since S60 5.0
        */
       void HandleMskIfConnectedL();
       
       /**
        * Handle MSK if connected and browser is using the connection
        * @since S60 5.0
        */
       void HandleMskIfBrowsingL();
               
    private:
        
        /*
         * Start web browsing with the given wlaninfo
         * @since S60 5.0
         * @param aInfo Wlaninfo to connect with
         */ 
        void StartBrowsingL( TWsfWlanInfo& aInfo );
        
        /*
         * Start web browser with the given wlaninfo
         * @since Symbian^3
         * @param aInfo Wlaninfo to connect with
         */ 
        void StartWebBrowserL( TWsfWlanInfo& aInfo );
        
        /**
         * Cleanup function to release connecting state
         * @param aPtr Pointer for this class
         */
        static void CleanUpConnectingL( TAny* aPtr );

        /*
         * Brings browser to the foreground
         * @since S60 5.0
         */
        void ContinueBrowsingL();

        /*
         * Launches a listquerydialog which holds all available wlans.
         * @since S60 5.0
         * @param aInfo The wlaninfo resulting from the selection if 
         *              no error/cancellation occured 
         * @return ETrue if a valid network was selected, EFalse otherwise 
         */ 
        TBool LaunchSearchDialogL( TWsfWlanInfo& aInfo );
        
        /* 
         * Trapped routine of LaunchSearchDialogL
         * @param - aSelectedItem which list item was selected
         * @return - ETrue if connection should be made, otherwise EFalse 
         */
        TBool DoLaunchSearchDialogL( TInt& aSelectedItem );
        
        /*
         *  Refreshes the current ui when the ui changes
         */ 
        void RefreshUiL();

        /**
         * Handle error event
         * @since S60 5.0
         * @param aError System wide error code
         */
        void DoHandleEngineErrorL( TInt aError );

        /**
         * Checks whether the browser is using the currently active WLAN 
         * connection (if any)
         * @since S60 5.02
         * @return ETrue if there is a WLAN connection and browser is using it,
         *         EFalse otherwise
         */
        TBool CheckIsWlanUsedByBrowserL();

            
    private: // data
        
        /**
         * Handle to Eikon environment.
         * Own.
         */
        CEikonEnv* iEnv;

        /**
         * Cache of the used info
         */
        TWsfWlanInfo iUsedInfo;

        /**
         * Reference to Wlan array
         */
        CWsfWlanInfoArray* iInfoArray;
        
        /**
         * Reference to Model
         */
        CWsfModel* iModel;

        /**
         * Reference to Active Idle plugin's Model
         */
        CWsfAiModel* iAiModel;

        /**
         * A pointer to active idle plugin ui control observer instance
         */
        MWsfAiUiObserver* iUi;
        
        /**
         * Cache of the connected WLAN data
         */
        TWsfWlanInfo iConnectedWlan;
        
        /**
         * The Database change observer
         */
        CWsfDbObserver* iDbObserver;
        
        /**
          * Indicates whether Connect or Start Web browsing was selected
          */
        TBool iConnectOnly;
        
        /**
         * Indicates if UI dialog is open 
         */
        TBool iDialogActive;
        
        /**
         * A pointer to CWsfActiveWrappers
         */
        CWsfActiveWrappers* iActiveWrappers;
        
        /**
         * ETrue, if plugin has started any operation and should continue
         * processing some operations based on received signals. E.g.
         * If plugin start web browsing it has to continue the operation
         * after the connection has been established.
         */
        TBool iHaveControl;
        
    };

#endif      //  T_WSFAICONTROLLER_H

// End of file
