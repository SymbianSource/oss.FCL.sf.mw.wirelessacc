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
* Description:  Class header for CWsfAiHelperAppUi
*
*/


#ifndef C_WSFAIHELPERAPPUI_H
#define C_WSFAIHELPERAPPUI_H

//  EXTERNAL INCLUDES
#include <aknViewAppUi.h>
#include <AknWaitDialog.h>

//  INTERNAL INCLUDES
#include "wsfmodelobserver.h"
#include "wsfstatechangeobserver.h"


//  FORWARD DECLARATIONS
class TWsfWlanInfo;
class CWsfAiHelperDocument;


//  CLASS DEFINITION
/**
 * An instance of class CWsfApHelperAppUi is the UserInterface part of 
 * the AVKON application framework
 * 
 * @since S60 5.0
 * @lib wsfaihelper.exe
 */
NONSHARABLE_CLASS( CWsfAiHelperAppUi ): public CAknViewAppUi,    
                                        public MWsfModelObserver, 
                                        public MWsfStateChangeObserver
    {
    public:
        /**
         * Second-phase constructor.
         * @since S60 5.0
         */
        void ConstructL();

        /**
         * Constructor
         * @since S60 5.0
         */
         CWsfAiHelperAppUi();
    
         /**
          * Destructor
          * @since S60 5.0
          */
        ~CWsfAiHelperAppUi();
    
    
    public: // from CAknAppUi
        /**
         * Handle user menu selections
         * @since S60 5.0
         * @param aCommand The enumerated code for the option selected
         */
        void HandleCommandL( TInt aCommand );
    
        
        /**
         * Processes shell commands.
         * @since S60 5.0
         * @param aCommand The shell command sent to the application
         * @param aDocumentName The name of the document as specified on 
         *                      the command line.
         * @param aTail Command line tail
         * @return Whether the final value of aDocumentName represents 
         *         an existing file
         */ 
        TBool ProcessCommandParametersL( TApaCommand aCommand,
                                         TFileName& aDocumentName,
                                         const TDesC8& aTail );


    public:     // new methods
        /**
         * Returns the handle of application document instance
         * @since S60 5.0
         * @return Pointer to the document. Ownership not passed.
         */ 
        CWsfAiHelperDocument* Doc();
    
        /**
        * Called when the filename to be opened has been resolved
        * @since S60 5.0
        */
        void NotifyFileName();
        
        /**
        * Sends Helper To Background
        * @since S60 5.2
        */
        void SendHelperToBackground();
    
        
    public: // from MWsfModelObserver
    
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
        

    public:     // from MWsfStateChangeObserver
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
        
    private:    // new methods
    
        /**
         * Called when application launch has been completed
         * @since S60 5.0
         * @param aObject Pointer to this object
         * @return Zero value means not be called again.(Stops timer)
         */
        static TInt LaunchCompletedL( TAny* aObject );
        
        /**
        * Cleanup function to release the key events suppressing
        * @since S60 5.0
        * @param aPtr Pointer for this class
        */
        static void ReleaseSuppressingKeyEventsL( TAny* aPtr );
    
        
    private:    // data
        /**
         * Used to begin accesspoint creation when app launching has completed
         * (owned)
         */ 
        CIdle* iIdle;

        /**
         * Indicates whether keypress events are suppressed
         */
        TBool iSuppressingKeyEvents;
        
        /**
         * IAP creation return value
         */
        TInt iResult;
    };


#endif // C_WSFAIHELPERAPPUI_H

