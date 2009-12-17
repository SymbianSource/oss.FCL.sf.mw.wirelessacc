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
* Description:  Class header for MWsfWlanInfoArrayFilterVisitor
*
*/



#ifndef M_WSFWLANINFOARRAYFILTERVISITOR_H
#define M_WSFWLANINFOARRAYFILTERVISITOR_H

//  EXTERNAL INCLUDES
#include <e32def.h>
#include <e32base.h>


//  FORWARD DECLARATIONS
class CWsfWlanInfoArray;


//  CLASS DEFINITION
/**
 * Abstract interface for CWsfWlanInfoArray to allow visitor-like overloading 
 * for array sorting
 * @lib wsfwlaninfo.lib
 * @since S60 5.0
 */
class MWsfWlanInfoArrayFilterVisitor 
    {
    public:     // Destructor
		/**
		* Destructor
		*/
        virtual ~MWsfWlanInfoArrayFilterVisitor() {}

	public:	
		/**
		* Execute presort before starting the actual sorting
		* @since S60 5.0
		* @param aArray The array to work on
		*/
		virtual void FilterPreSortL( CWsfWlanInfoArray* aArray ) = 0;
		
		/**
		* Run postsort after he actual sorting is executed
		* @since S60 5.0
		* @param aArray The array to work on
		*/
		virtual void FilterPostSortL( CWsfWlanInfoArray* aArray ) = 0;
                
        /**        
        * Return the handle for sorting key
		* @since S60 5.0
		* @return The handle for the sorting key
        */        
        virtual TKeyArrayFix& SortingKey() = 0; 
               
    };

#endif      //  M_WSFWLANINFOARRAYFILTERVISITOR_H

// End of file
