/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Handle load progress events
*
*/



#ifndef HSBROWSERLOADEVENTOBSERVER_H
#define HSBROWSERLOADEVENTOBSERVER_H

// INCLUDES
#include <brctlinterface.h>

// FORWARD DECLARATIONS
class CHsBrowserContainer;

// CONSTANTS
_LIT8( KHttp, "http" );
const TInt KTotalPollingTime = 600000000;       // 10 minutes
const TInt KExecutionTimeInterval = 10000000;   // 10 seconds
const TInt KBufSize = 4;

// CLASS DECLARATION
/**
* This class is used to receive load progress events.
*/
class CHsBrowserLoadEventObserver : public CBase, public MBrCtlLoadEventObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CHsBrowserLoadEventObserver* NewL( CHsBrowserContainer* aContainer );

        /**
        * Destructor.
        */
        ~CHsBrowserLoadEventObserver();
    
    public: // from MBrCtlLoadEventObserver
    
        /**
        * From MBrCtlLoadEventObserver, load events notification.
        */
        void HandleBrowserLoadEventL( TBrCtlDefs::TBrCtlLoadEvent aLoadEvent,
                                      TUint aSize,
                                      TUint16 aTransactionId) ;
    private:

		/**
		* Constructor.
		*/
        CHsBrowserLoadEventObserver( CHsBrowserContainer* aContainer );

	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private: // new

        /**
        * Update label in the navigation pane.
        *
        * @param aName Label for navi pane.
        */
        void UpdateNaviLabel( const TDesC& aName );
        
        /**
        * Update name in fast swapper.
        *
        * @param aName Name in fast swapper.
        */
        void UpdateFastSwapNameL( const TDesC& aName );

        /**
        * Get page title, udpate navi label and name in fast swapper.
        */
        void UpdateNameL();        
                
        /**
        * Start polling internet connection.
        */
        void TestInternetConnectivityL();
        
        /**
        * Update boolean flag for internet connectivity test.
        */
        void UpdateDoIctFlagL();

    private:   // data
    
        /**
        * Pointer to the container class associated with this observer.
        */
        CHsBrowserContainer* iContainer; // not owner
        
        /**
        * Pointer to internet connectivity test object.
        */
        CIctsClientInterface* iIcts; // owner
        
        /**
        * Boolean flag for Internet Connectivity Test.
        */
        TBool iDoIct;

        /**
        * Used to differentiate the navibar content: no title shown yet when
        * we are just loading the logon page
        */        
        TBool iIsFirstLoad;
    };

#endif // HSBROWSERLOADEVENTOBSERVER_H
            
// End of File
