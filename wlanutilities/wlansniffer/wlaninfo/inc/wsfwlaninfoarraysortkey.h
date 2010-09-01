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
* Description:  Class header for CWsfWlanInfoArraySortKey
*
*/



#ifndef C_WSFWLANINFOARRAYSORTKEY_H
#define C_WSFWLANINFOARRAYSORTKEY_H

//  EXTERNAL INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class CWsfWlanInfoArray;


//  CLASS DEFINITION
/**
 * Sort key for wlan info array
 * @lib wsfwlaninfo.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CWsfWlanInfoArraySortKey ): public TKeyArrayFix 
	{
    public:     // Constructors
        /**
        * Factory function.
        * @since S60 5.0
        * @param aArray The array to work with.
        */
		static CWsfWlanInfoArraySortKey* NewLC( CWsfWlanInfoArray& aArray );
		
        /**
        * Destructor.
        * @since S60 5.0
        */
		~CWsfWlanInfoArraySortKey();
  
    private:  // Constructors
        /**
        * Constructor.
        * @since S60 5.0
        * @param aArray The array to work with.
        */        
		CWsfWlanInfoArraySortKey( CWsfWlanInfoArray& aArray );

    public:   // from TKey

       /**
        * Compare function
        * @since S60 5.0
        * @param aLeft Left element to be compared
        * @param aRight Right element to be compared
        * @return -1 if aLeft<aRight, 0 if equal, 1 otherwise
        */
        virtual TInt Compare( TInt aLeft, TInt aRight ) const;

    public:    // Data
        // The array to work with. Not owned.
		CWsfWlanInfoArray* iArray;
	};


#endif // C_WSFWLANINFOARRAYSORTKEY_H
