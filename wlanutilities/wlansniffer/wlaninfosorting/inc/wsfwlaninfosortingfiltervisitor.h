/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class header for MWsfWlanInfoSortingFilterVisitor
*
*/



#ifndef M_WSFWLANINFOSORTINGFILTERVISITOR_H
#define M_WSFWLANINFOSORTINGFILTERVISITOR_H


//  EXTERNAL INCLUDES
#include <e32base.h>


//  FORWARD DECLARATIONS
class TWsfWlanInfo;
class CWsfWlanInfoArray;


//  CLASS DEFINITION
/**
 * Interface class for common filtering actions
 *
 * @since S60 5.0
 */
class MWsfWlanInfoSortingFilterVisitor 
    {
    public:
    
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
                              const TWsfWlanInfo& aInfoRight ) = 0;

        /**
        * Check if the entry matches to filter rules and carry out additional
        * operations if needed.
        * @since S60 5.0
        * @param aInfoLeft The element to be checked
        * @return ETrue if the item matches the rules
        */
        virtual TBool SetupOnRuleMatch( TWsfWlanInfo& aInfoLeft ) = 0;
    

        /**
        * Handle filter match 
        * @since S60 5.0
        * @param aFilterEntry The element in question
        */ 
        virtual void MarkMatchedFilterL( TWsfWlanInfo& aFilterEntry ) = 0;

        /**
        * Edit filter definitions
        * @since S60 5.0
        * @param aInfoArray The element in question
        */
        virtual void EditFilterDefsL( CWsfWlanInfoArray& aInfoArray ) = 0;
        
        /**
        * Read the filter defintions to filter
        * @since S60 5.0
        */
        virtual void ReadFilterDefsL() = 0;
    };
    



#endif // M_WSFWLANINFOSORTINGFILTERVISITOR_H

