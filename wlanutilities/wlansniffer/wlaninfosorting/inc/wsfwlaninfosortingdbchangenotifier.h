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
* Description:  Class header for CWsfWlanInfoSortingDbChangeNotifier
*
*/



#ifndef C_WSFWLANINFOSORTINGDBCHANGENOTIFIER_H
#define C_WSFWLANINFOSORTINGDBCHANGENOTIFIER_H


//  EXTERNAL INCLUDES
#include <e32base.h>
#include <d32dbms.h>


//  FORWARD DECLARATIONS
class MWsfWlanInfoSortingFilterVisitor;


/**
 * Active object to monitor changes in a given database
 *
 * @lib wsfwlaninfosorting.lib
 * @since S60 5.0
 */

NONSHARABLE_CLASS( CWsfWlanInfoSortingDbChangeNotifier ): public CActive
    {
    public:     // construction
        /**
         * Factory function.
         * @since S60 5.0
         * @param aDatabase The database to be monitored. Ownership not passed.
         * @return Class instance
         */
        static CWsfWlanInfoSortingDbChangeNotifier* NewLC( 
                                                      RDbDatabase& aDatabase );

        /**
         * Factory function.
         * @since S60 5.0
         * @param aDatabase The database to be monitored. Ownership not passed.
         * @return Class instance
         */
        static CWsfWlanInfoSortingDbChangeNotifier* NewL( 
                                                      RDbDatabase& aDatabase );
        
        ~CWsfWlanInfoSortingDbChangeNotifier();


    private:    // constructors
        /**
         * Constructor
         * @since S60 5.0
         * @param aDatabase The database to be monitored. Ownership not passed.
         */
        CWsfWlanInfoSortingDbChangeNotifier( RDbDatabase& aDatabase );
        
        void ConstructL();    
    
        
    public:     // new methods

        /**
        * Starts monitoring the database
        * @since S60 5.0
        */
         void StartDbObservation();
         
         /**
         * Append filter to the list of clients to be notified. 
        * @since S60 5.0
         * @param aRelatedFilter A filter which is interested in db events.
         *                       Ownership not passed.
         * @return - void
         */
        void AppendDbRelatedFilterL( 
                            MWsfWlanInfoSortingFilterVisitor* aRelatedFilter );
        
        /**
        * Removes the filter from the list of clients. 
        * @since S60 5.0
         * @param aRelatedFilter A filter which is interested in db events.
         *                       Ownership not passed.
         * @return - void
         */
        void RemoveDbRelatedFilter( 
                            MWsfWlanInfoSortingFilterVisitor* aRelatedFilter );
        
        
    private: // from CActive
        
        void DoCancel(); 
        void RunL();


    private: //data    
        /**
        * The database to be monitored. Not owned.
        */
        RDbDatabase* iDatabase;
        
        /**
        * Database notifier
        */
        RDbNotifier iNotifier;
        
        /**
        * Array of DB-related filters to be notified
        */
        RPointerArray<MWsfWlanInfoSortingFilterVisitor> iDbAwareFilters;
    };

#endif // C_WSFWLANINFOSORTINGDBCHANGENOTIFIER_H
