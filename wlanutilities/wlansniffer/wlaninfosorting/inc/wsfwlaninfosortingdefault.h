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
* Description:  Class header for CWsfWlanInfoSortingDefault
*
*/



#ifndef C_WSFWLANINFOSORTINGDEFAULT_H
#define C_WSFWLANINFOSORTINGDEFAULT_H


//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfosortingfiltervisitor.h"


//  CLASS DEFINITION
/**
* Default sorting filter for WLAN items
*
* @lib wsfwlaninfosorting.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CWsfWlanInfoSortingDefault ): public CBase, 
                                        public MWsfWlanInfoSortingFilterVisitor
    {
    public:     
        static CWsfWlanInfoSortingDefault* NewLC();
        static CWsfWlanInfoSortingDefault* NewL();
        
        ~CWsfWlanInfoSortingDefault();
        
        
    private:    // constructors
        void ConstructL();
        CWsfWlanInfoSortingDefault();    
        
            
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
        * @return ETrue if the item matches the rule
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

    };


#endif // C_WSFWLANINFOSORTINGDEFAULT_H

