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
* Description:  Class header for CWsfActiveTimeUpdater
*
*/

#ifndef C_WSFACTIVETIMEUPDATER_H
#define C_WSFACTIVETIMEUPDATER_H

//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class MWsfActiveTimerHandler;


/**
 *  Responsible for time updating by using a customizable time interval.
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CWsfActiveTimeUpdater ) : public CBase
    {
    public:     // Constructors and destructor
        
        /**
         * Two-phased constructor
         * @since S60 5.0 
         * @param aActiveTimerHandler interface details views controller
         * @return instance of CWsfActiveTimeUpdater class
         */
        static CWsfActiveTimeUpdater* NewL(
                MWsfActiveTimerHandler* aActiveTimerHandler );


        /**
         * Two-phased constructor. Leaves the object on CleanupStack
         * @since S60 5.0 
         * @param aActiveTimerHandler interface details views controller
         * @return instance of CWsfActiveTimeUpdater class
         */
        static CWsfActiveTimeUpdater* NewLC( 
                MWsfActiveTimerHandler* aActiveTimerHandler );
        
        /**
         * Destructor of CWsfActiveTimeUpdater class
         * @since S60 5.0     
         */
        ~CWsfActiveTimeUpdater();
        
    private:    // Constructors
        
        /**
         * Default C++ constructor 
         * @since S60 5.0 
         * @param aActiveTimerHandler interface details views controller
         */
        CWsfActiveTimeUpdater( MWsfActiveTimerHandler* aActiveTimerHandler );
        
        /**
         * 2nd phase constructor
         * @since S60 5.0 
         */
        void ConstructL();
    
    public:     // New methods
    
        /**
         * Start update timer
         * @since S60 5.0 
         * @param aUpdateInterval refresh time interval
         */
        void Start( TInt aUpdateInterval );
        
        /**
         * Stop update timer
         * @since S60 5.0 
         */
        void Stop();
    
        /**
         * Called from static call back of CPeriodic.
         * The actual update process starts here
         * @since S60 5.0 
         */
        void DoUpdateTimeL();
        
    private:    // New methods
        /**
         * Function to called when periodic timer releases
         * @since S60 5.0 
         * @param aUpdater pointer to this
         */
        static TInt UpdateTimeL( TAny* aUpdater );
    
    private: 
    
        /**
         * Pointer to the Active Timer handler interface 
         * Own.
         */
        MWsfActiveTimerHandler* iActiveTimerHandler;

        /**
         * Timer, to refresh the details views
         * Own.
         */
        CPeriodic* iPeriodic;
    };

#endif // C_WSFACTIVETIMEUPDATER_H
