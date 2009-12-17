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
* Description:  Class header for CWsfWlanInfoArrayVisitor
*
*/



#ifndef C_WSFWLANINFOSORTINGBLACKLIST_H
#define C_WSFWLANINFOSORTINGBLACKLIST_H


//  EXTERNAL INCLUDES
#include <e32base.h>
#include <d32dbms.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfosortingfiltervisitor.h"
#include "wsfwlaninfo.h"



//  FORWARD DECLARATIONS
class CDesC8Array;
class CGulIcon;
class CAknIconArray;



/**
* Blacklisted item sorting filter
*
* @lib wsfwlaninfosorting.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CWsfWlanInfoSortingBlackList ): public CBase, 
                                        public MWsfWlanInfoSortingFilterVisitor
    {
    public:
        /**
        * Factory function.
        * @since S60 5.0
        * @param aDatabase The database to read the blacklist from.
        * @return Class instance.
        */
        static CWsfWlanInfoSortingBlackList* NewLC( 
                                                 RDbNamedDatabase& aDatabase );

        /**
        * Factory function.
        * @since S60 5.0
        * @param aDatabase The database to read the blacklist from.
        * @return Class instance.
        */
        static CWsfWlanInfoSortingBlackList* NewL(
                                                 RDbNamedDatabase& aDatabase );
        
        ~CWsfWlanInfoSortingBlackList();

        
    private:
        /**
        * Constructor.
        * @since S60 5.0
        * @param aDatabase The database to read the blacklist from.
        */
        CWsfWlanInfoSortingBlackList( RDbNamedDatabase& aDatabase );
        
        void ConstructL();    
    

    public: // from MWsfWlanInfoSortingFilterVisitor
        
        /**
        * Do comparing between these two entries
        * @since S60 5.0
        * @param aInfoLeft Left element
        * @param aInfoRight Right element
        * @return Negative if aInfoLeft is smaller tha aInfoRight,
        *         zero if the entries are equal,
        *         positive if the aInfoLeft is greater than aInfoRight
        */
        virtual TInt Compare( const TWsfWlanInfo& aInfoLeft, 
                              const TWsfWlanInfo& aInfoRight );

        /**
        * Check if the entry matches to filter rules and carry out additional
        * operations if needed.
        * @since S60 5.0
        * @param aInfoLeft The element to be checked
        * @return ETrue if the item matches the blacklist rule
        */
        virtual TBool SetupOnRuleMatch( TWsfWlanInfo& aInfoLeft );

        /**
        * Handle filter match 
        * @since S60 5.0
        * @param aFilterEntry The element in question
        */ 
        virtual void MarkMatchedFilterL( TWsfWlanInfo& aFilterEntry );

        /**
        * Edit filter definitions
        * @since S60 5.0
        * @param aInfoArray The element in question
        */
        virtual void EditFilterDefsL( CWsfWlanInfoArray& aInfoArray );
        
        /**
        * Read the filter defintions to filter
        * @since S60 5.0
        */
        virtual void ReadFilterDefsL();

        
    public:     // new methods
        /**
        * Return the number of elements in the blacklist
        * @since S60 5.0
        * @return The count of blacklisted SSIDs
        */
        TInt BlackListItemCount() const;

    
    private:    // new methods
        /**
        * Save filter definitions
        * @since S60 5.0
        */
        void SaveDefsL();
        
        /**
        * Restore filter definitions
        * @since S60 5.0
        */
        void RestoreDefsL();
        
        /**
        * Check if the blacklist table exists in the database
        * @since S60 5.0
        */
        void CheckTableL();

    
    private:    //data    
    
        /**
        * Array of blacklisted SSIDs. Owned.
        */
        CDesC8Array* iBlackListSsids;
        
        /**
        * Handle to the blacklist database. Not owned.
        */
        RDbNamedDatabase* iDefsDb;
        
        /**
        * Handle to the blacklist table
        */
        RDbTable iBlackListSsidTable;
    };
    


#endif // C_WSFWLANINFOSORTINGBLACKLIST_H

