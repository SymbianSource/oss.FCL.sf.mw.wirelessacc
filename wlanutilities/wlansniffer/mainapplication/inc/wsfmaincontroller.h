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
* Description:  Class header for TWsfMainController
*
*/

#ifndef T_WSFMAINCONTROLLER_H
#define T_WSFMAINCONTROLLER_H

//  EXTERNAL INCLUDES
#include <e32def.h>

//  INTERNAL INCLUDES
#include "wsfmainuiobserver.h" 
#include "wsfmodelobserver.h"
#include "wsfmainviewpartner.h"
#include "wsfstatechangeobserver.h"
#include "wsfdetailsviewpartner.h"



//  FORWARD DECLARATIONS
class CWsfAppUi;
class CWsfModel;
class MWsfMainViewControllerIf;
class MWsfDetailsViewControllerIf;
class CWsfWlanInfoArray;
class CWsfWlanInfoArrayVisitor; 

//  CLASS DEFINITION
/**
 *  The main control class of the component. It is responsible for all
 *  the WLAN related functionalities.
 *  @since S60 v5.0
 */
class TWsfMainController: public MWsfMainUiObserver, 
                          public MWsfModelObserver, 
                          public MWsfMainViewPartner,
                          public MWsfDetailsViewPartner,
                          public MWsfStateChangeObserver
    {
    public:     // New methods

        /**
         * PreInitialization for this class
         * @since S60 5.0 
         * @param aAppUi reference for the AppUI
         * @param aModel reference for the Model
         * @param aWlanInfoBranding reference for the WlanInfoArrayVisitor
         */
        void Initialize( CWsfAppUi& aAppUi, 
                         CWsfModel& aModel, 
                         CWsfWlanInfoArrayVisitor& aWlanInfoBranding );
        
        /**
         * Updating the active View 
         * @since S60 5.0 
         */
        void UpdateViewL();
        
        /**
         * Updating the active View 
         * @since S60 5.0 
         * @param aInfoArray - Current Wlan Array
         */
        void UpdateViewL( CWsfWlanInfoArray* aInfoArray );

    public:     //From MWsfMainUiObserver
        
        /**
         * Activate (Switch back to) the Main View
         * @since S60 5.0 
         */
        void ShowWlanListL();            
        
        /**
         * Incialize Main View 
         * @since S60 5.0 
         */
        void MainViewCreatedL();
        
        /**
         * Sarts the Borwser
         * @since S60 5.0 
         */
        void StartBrowsingL();
        
        /**
         * Continues browsing (browser launched if needed)
         * @since S60 5.0 
         */
        void ContinueBrowsingL();
              
        /**
         * Connect to an IAP and create a WLAN connection
         * @since S60 5.0 
         */
        void ConnectL();
        
        /**
         * Disconnect a connection
         * @since S60 5.0 
         */
        void DisconnectL();
        
        /**
         * Start a refresh scanning 
         * @since S60 5.0 
         */
        void RefreshL();
        
        /**
         * Start to display the diteils view of the selected connection
         * @since S60 5.0 
         */
        void ShowDetailsL();           

        /**
         * Create an Access Point
         * @since S60 5.0 
         */
        void CreateAccessPointL(); 

        /**
         * Used in AppUi side to check if the 
         * wait dialog should be shown
         * @since S60 5.0 
         * @return ETrue if first refreshing
         */
        TBool EngineRefreshing();        

        /**
         * Callback function, when the Sniffer started 
         * @since S60 5.0 
         */
        void AppLaunchCompleteL();

        /**
         * Enable Scanning 
         * @since S60 5.0 
         */
        void AppSwitchedForegroundL();

        /**
         * Disable Scanning 
         * @since S60 5.0 
         */
        void AppSwitchedBackgroundL();  

        /**
         * Displays the "Filter out Wlan" dialog  
         * @since S60 5.0 
         */
        void EditBlackListItemsL();
           

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

    
    public:     //From MWsfMainViewPartner

        /**
         * Mainview request to show manubar
         * @since S60 5.0
         */
        void ShowMenuBarL();
       
    public:     //From MWsfDetailsViewPartner
        
        /**
        * Detailsview requests to close view
        * @since S60 5.0
        */    
        void CloseDetailsViewL();
        
        /**
        * Refresh scanning
        */
        void DetailsScanL();       
        
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
        * @param aAccesspointName The name of the active connection
        */
        void WlanConnectionActivatedL( const TDesC& aAccessPointName );
        
        /**
        * A WLAN connection has been closed
        * @since S60 5.0
        */
        void WlanConnectionClosedL();
        
        /**
        * Connection creation process finished
        * @since S60 5.2
        * @param aError System wide error code
        */
        void ConnectionCreationProcessFinishedL( TInt aError );
        
    private:
        
        /**
        * Cleanup function to release the key events suppressing
        * @since S60 5.0
        * @param aPtr Pointer for this class
        */
        static void ReleaseSuppressingKeyEvents( TAny* aPtr );
        
    private:    // Data

        /**
        * Reference to AppUi
        */
        CWsfAppUi* iAppUi;

        /**
        * Reference to Model
        */
        CWsfModel* iModel;
        
        /**
        * Reference to Main View
        */
        MWsfMainViewControllerIf* iMainView;
        
        /**
        * Reference to WlanInfoArrayVisitor
        */
        CWsfWlanInfoArrayVisitor* iWlanInfoBranding; 
        
        /**
        * Reference to Deatils View
        */
        MWsfDetailsViewControllerIf* iDetailsView;

        /**
        * Reference to WlanInfoArray
        */
        CWsfWlanInfoArray* iInfoArray;

    };


#endif      //  T_WSFMAINCONTROLLER_H

// End of file
