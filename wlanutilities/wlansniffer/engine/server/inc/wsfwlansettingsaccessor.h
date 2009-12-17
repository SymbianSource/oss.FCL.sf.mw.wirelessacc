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


#ifndef C_WSFWLANSETTINGSACCESSOR_H
#define C_WSFWLANSETTINGSACCESSOR_H

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <commsdattypesv1_1.h>

//  INTERNAL INCLUDES
#include "wlanmgmtcommon.h"
#include "wlanscaninfo.h"


//  FORWARD DECLARATIONS
class CCDWlanDeviceSettingsRecord;
class MWsfWlanScanIntervalChangeObserver;


//  CLASS DEFINITION
/**
 * Class to monitor WLAN background scan interval and Show WLAN availability 
 * flag value
 *
 * @since S60 5.0
 * @lib wsfserver.exe
 */
NONSHARABLE_CLASS( CWsfWlanSettingsAccessor ): public CActive
    {
    public:     // Constructors and destructor
        /**
        * Factory function.
        * @since S60 5.0
        * @param aDbSession Reference to the database session to use
        * @return Class instance
        */
        static CWsfWlanSettingsAccessor* NewL( 
                                          CommsDat::CMDBSession& aDbSession );

        /**
        * Factory function.
        * @since S60 5.0
        * @param aDbSession Reference to the database session to use
        * @return Class instance
        */
        static CWsfWlanSettingsAccessor* NewLC( 
                                          CommsDat::CMDBSession& aDbSession );

        /**
        * Destructor.
        */
        ~CWsfWlanSettingsAccessor();
        

    private:    // Constructors
        /**
        * Constructor
        * @since S60 5.0
        * @param aDbSession Reference to the database session to use
        * @return Class instance
        */
        CWsfWlanSettingsAccessor( CommsDat::CMDBSession& aDbSession );

        /**
        * Second-phase constructor.
        * @since S60 5.0
        */
        void ConstructL();


    public: // new methods
        /**
        * Returns the value of the scan interval
        * @since S60 5.0
        * @return The value of the scan interval in seconds
        */ 
        TUint ScanInterval() const;

        /**
        * Returns the value of the show WLAN availability flag
        * @since S60 5.0
        * @return The value of the flag
        */ 
        TBool ShowAvailability() const;

        /**
        * Requests notification for when the scan interval changes
        * @since S60 5.0
        * @param aObserver The observer object to be notified
        */
        void RequestNotificationL( 
                              MWsfWlanScanIntervalChangeObserver& aObserver );
            
        /**
        * Cancels the change notifications
        * @since S60 5.0
        */
        void CancelNotifications();


    private:    // New methods
        /**
        * Does the actual setting check
        * @since S60 5.0
        * @param aBgScanInterval Variable to store the latest background scan 
        *                        interval value
        * @param aShowAvailability Variable to store the latest Show WLAN 
        *                        availability flag value
        */
        void DoCheckSettingL( TUint& aBgScanInterval, 
                              TBool& aShowAvailability);

        /**
        * Checks if the current bgscaninterval/availability flag is different 
        * from the previously retrieved value
        * @since S60 5.0
        * @return ETrue if the settings has been changed
        */
        TBool CheckIfSettingChangedL();

        /**
        * Reissues the request for database notification
        * @since S60 5.0
        */
        void IssueNotificationRequestL();


    private:  // from CActive
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
        
    
    private:    // Data
        /**
        * Handle to the database session (not owned)
        */
        CommsDat::CMDBSession* iDbSession;
        
        /**
        * Reference to the observer object (not owned)
        */
        MWsfWlanScanIntervalChangeObserver* iChangeObserver;
        
        /**
        * Instance of the WLAN settings record (owned)
        */
        CCDWlanDeviceSettingsRecord* iWlanSettingsRecord;
        
        /**
        * The last retrieved bgscaninterval value (in seconds)
        */
        TUint iScanInterval;
        
        /**
        * Table ID for the WLAN settings table
        */
        CommsDat::TMDBElementId iTableId;
        
        /**
        * Hold-up timer to avoid notification bursts
        */
        RTimer iTimer;
        
        /**
        * Indicates whether the processing has been activated but got held up
        */
        TBool iBeingHeldUp;
        
        /**
        * Indicates whether show wlan availability flag is on
        */
        TBool iShowAvailability;
    };

#endif // C_WSFWLANSETTINGSACCESSOR_H

