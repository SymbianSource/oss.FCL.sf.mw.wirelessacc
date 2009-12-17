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
* Description:  Class header for CWsfAppLauncher
*
*/




#ifndef C_WSFAPPLAUNCHER_H
#define C_WSFAPPLAUNCHER_H

#include <e32std.h> 
#include <w32std.h> 

class MWsfBrowserLaunchObserver;
class CRepository;
class CMDBSession;


/**
* Browser launching states
*/
enum TWsfLaunchState
    {
    EIdle,
    EShuttingDown,
    EStartingUp,
    ECompleted,
    EFinished
    };



//  CLASS DEFINITION
/**
 * Class to manage browser launching and closure.
 *
 * @lib wsfmodel.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CWsfAppLauncher ): public CActive 
    {
    public: // Constuctors and destructors

        /**
        * Factory function.
        * @since S60 5.0
        * @return Class instance
        */
        IMPORT_C static CWsfAppLauncher* NewL();

        /**
        * Factory function.
        * @since S60 5.0
        * @return Class instance
        */
        IMPORT_C static CWsfAppLauncher* NewLC();

        /**
        * Destructor
        * @since S60 5.0
        */
        virtual ~CWsfAppLauncher();


    private: //Constuctors

        /**
        * Constructor
        * @since S60 5.0
        */
        CWsfAppLauncher();

        /**
        * Second-phase constructor
        * @since S60 5.0
        */
        void ConstructL();
        

    public: //New public functions

        /**
        * Launches Browser application asynchronously using selected iap
        * @since S60 5.0
        * @param aObserver Observer to be notified about launch completion
        * @param aIapId IAP id passed as a parameter
        */
        void LaunchBrowserL( MWsfBrowserLaunchObserver& aObserver, 
                             TUint aIapId );

        /**
        * Launches Browser application asynchronously using selected iap and 
        * user defined URL
        * @since S60 5.0
        * @param aObserver Observer to be notified about launch completion
        * @param aIapId IAP id passed as a parameter
        * @param aWapId WAP id passed as a parameter
        * @param aURL User defined WWW address to be used as start-up page    
        */
        void LaunchBrowserL( MWsfBrowserLaunchObserver& aObserver, 
                             TUint aIapId,
                             TUint aWapId,
                             const TDesC& aURL ); 

        /**
        * Switches Browser application to foreground
        * @since S60 5.0
        */
        void ContinueBrowsingL();
        
        /**
        * Switches Browser application to foreground
        * @since S60 5.0
        * @param aObserver Observer to be notified about launch completion
        * @param aIapId IAP id passed as a parameter
        */
        void ContinueBrowsingL( MWsfBrowserLaunchObserver& aObserver, 
                                TUint aIapId );
        
        /**
        * Query launching state
        * @since S60 5.0
        * @return Lauching state
        */
        TWsfLaunchState Launching();
        
        
        /**
        * Returns the IAP which was last passed to the browser
        * @since S60 5.0
        * @return The IAP id passed to the browser.
        */
        TUint32 BrowserIap() const;
        

    private: // New private functions

        /**
        * Kills browser application if the one is already lauched
        * @since S60 5.0
        * @return ETrue if browser was killed, EFalse otherwise
        */
        TBool KillBrowserIfAlreadyExists();

        /**
        * Launches the browser asynchronously.
        * @since S60 5.0
        */
        void DoLaunchBrowserL();

        /**
        * Checks if Browser exists
        * @since S60 5.0
        * @return ETrue if the browser exists
        */
        TBool BrowserExists();

        /**
        * Checks what the default homepage is (URL) 
        * @since S60 5.0
        * @param aHomePageURL URL of the default homepage
        * @return KErrNone if successful, KErrNotFound if HomePage is 
        *         not defined
        */
        TInt BrowserHomepage( TDes& aHomePageURL );

        /**
        * Gets the access point selection mode
        * @since S60 5.0
        * @return 0=User defined, 1=Always ask, 2=Destination
        */
        TInt ApSelectionMode();

        /**
        * Defines the access point selection mode
        * @since S60 5.0
        * @param aEnable Selection mode: 0=User defined, 1=Always ask,  
        *                2=Destination
        */
        void SetApSelectionMode( TInt aApSelectionMode );
        
        /**
        * Restores Access Point selection mode
        * @since S60 5.0
        */   
        void RestoreApSelectionMode();

        /**
        * Configures browser access point using central repository
        * @since S60 5.0
        * @param aAccessPointId Access Point's IAP id
        */   
        void SetAccessPoint( TUint aAccessPointId );
        
        /*
        * Restores original accesspoint id if one is set
        * @since S60 5.0
        */
        void RestoreAccessPoint();
        

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


    private: // data

        /**
        * Observer to be notified about launch completion. Not owned.
        */
        MWsfBrowserLaunchObserver* iObserver;
        
        /**
        * Window session
        */
        RWsSession iWsSession;

        /**
        * IAP id to use
        */
        TUint iIapId;
        
        /**
        * WAP id for browser's default connection id
        */
        TUint iWapId; 

        /**
        * State of browser launching
        */
        TWsfLaunchState iLaunchState;  

        /**
        * Stored original selection mode
        */
        TBool iRestoreSelectionMode;

        /**
        * Indicates whether to restore access point
        */
        TBool iRestoreAccessPoint;

        /**
        * The original IAP id used by the browser
        */
        TUint iOriginalApId;

        /**
        * Indicates whether to launch bookmarks page 
        * (if homepage is not set)
        */
        TBool iLaunchBookMarks;

        /**
        * Browser app's UID
        */
        TUid iBrowserUid;

        /**
        * Key number for browser's default homepage
        */
        TUint iHomepageKey;

        /**
        * Handle to a repository. Owned.
        */
        CRepository* iRepository;

        /**
        * URL of the homepage. Owned.
        */
        HBufC* iURL;

        /**
        * Timer to wait for browser startup etc.
        */
        RTimer iTimer;     

        /**
        * Indicates which AP selection mode was originally used.
        */
        TUint iOriginalApSelectionMode;

        /**
        * Browser's cenrep key number for the homepage
        */
        TUint iBrowserHomePageKey;

        /**
        * Browser's cenrep key number for the starting page
        */
        TUint iBrowserStartingPageKey;
        
        /**
        * The id of the broser thread
        */
        TThreadId iThreadId;
        
        /**
        * The browser thread
        */
        RThread iThread;
    };


#endif // C_WSFAPPLAUNCHER_H

