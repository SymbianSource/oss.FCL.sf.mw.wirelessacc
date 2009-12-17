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
* Description:   Implements callback interface for Internet Connectivity Test Service
*                notifications.
*
*/



#ifndef HSBROWSERICTSOBSERVER_H
#define HSBROWSERICTSOBSERVER_H

//  INCLUDES
#include <e32base.h>
#include <ictscommon.h>

// FORWARD DECLARATION
class CHsBrowserContainer;

// CLASS DECLARATION
/**
* This class implements callback interface for Internet Connectivity Test Service
* notifications.
*/
class CHsBrowserIctsObserver : public CBase, public MIctsObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aContainer Pointer to container of browser application
        */
        static CHsBrowserIctsObserver* NewL( CHsBrowserContainer* aContainer );

        /**
        * Destructor.
        */
        ~CHsBrowserIctsObserver();

    public: // from MIctsObserver
    
        /**
        * From MIctsObserver, ConnectivityObserver 
        */
        void ConnectivityObserver( TIctsTestResult aResult, const TDesC& aString );
        
    private: // new
    
        /**
        * Process connection ok event.
        */
        void ProcessConnectionOk();
                
    private:
    
        /**
        * C++ default constructor.
        *
        * @param aContainer Pointer to container of browser application.
        */
        CHsBrowserIctsObserver( CHsBrowserContainer* aContainer );
        
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:
    
        /**
        * Container of hotspot browser application.
        * Not owned.
        */
        CHsBrowserContainer* iContainer;
    };

#endif // HSBROWSERICTSOBSERVER_H
            
// End of File

