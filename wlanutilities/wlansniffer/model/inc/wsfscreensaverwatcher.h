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
* Description:  Class header for CWsfScreenSaverWatcher
*
*/



#ifndef C_WSFSCREENSAVERWACTHER_H
#define C_WSFSCREENSAVERWACTHER_H

#include <e32base.h>
#include <e32property.h>


//  FORWARD DECLARATIONS
class MWsfScreenSaverStateObserver;

/**
* Active object class which monitors screen saver activity and notifies 
* the observer when changes occur.
*
* @lib wsfmodel.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CWsfScreenSaverWatcher ): public CActive
    {
    public:     // Constructors
        /**
        * Factory function.
        * @since S60 5.0
        * @param aObserver The object to notify when changes occur.
        * @return Class instance.
        */
        static CWsfScreenSaverWatcher* NewL( 
                                    MWsfScreenSaverStateObserver& aObserver );

        /**
        * Factory function.
        * @since S60 5.0
        * @param aObserver The object to notify when changes occur.
        * @return Class instance.
        */
        static CWsfScreenSaverWatcher* NewLC( 
                                    MWsfScreenSaverStateObserver& aObserver );
        
        ~CWsfScreenSaverWatcher();
        
    private: // Constructors

        /**
        * Constructor.
        * @since S60 5.0
        * @param aObserver The object to notify when changes occur.
        */
        CWsfScreenSaverWatcher( MWsfScreenSaverStateObserver& aObserver );

        void ConstructL();    
    
    public: 
        
        /**
        * Start receiving events from screensaver
        * @since S60 5.0
        */
        void StartStatusScanning();

    protected:  // from CActive
        void RunL();
        void DoCancel();
        TInt RunError( TInt aError );

    private:    // Data

        /**
        * Handle to the screen saver property value
        */
        RProperty iProperty;
        
        /**
        * The object to notify when changes occur. Not owned.
        */
        MWsfScreenSaverStateObserver* iObserver;

    };
    
#endif // C_WSFSCREENSAVERWACTHER_H

