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
* Description:  Class header for CWsfWlanInfoArray
*
*/


#ifndef C_WSFWLANINFOARRAY_H
#define C_WSFWLANINFOARRAY_H


//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"


// FORWARD DECLARATIONS
class MWsfWlanInfoArrayFilterVisitor;


//  CLASS DEFINITION
/**
 * Pointerarray container for TWsfWlanInfo items.
 * @lib wsfwlaninfo.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CWsfWlanInfoArray ): public CBase
	{
    public:     // Constructors and destructor

        /**
        * Factory function.
        * @since S60 5.0
        * @return Class instance.
        */
        IMPORT_C static CWsfWlanInfoArray* NewL();

        /**
        * Factory function.
        * @since S60 5.0
        * @return Class instance.
        */
        IMPORT_C static CWsfWlanInfoArray* NewLC();

        ~CWsfWlanInfoArray();

    private:    // Constructors
        CWsfWlanInfoArray();
        void ConstructL();

    public:     // New methods
		
        /**
        * Returns the index of the given wlaninfo
        * @since S60 5.0
        * @param aWlanInfo The element being searched for. Ownership not passed.
        * @return Index of info in the array, or -1 if not found 
        */
        IMPORT_C TInt GetArrayIndex( TWsfWlanInfo* aWlanInfo ) const;

        /**
        * Add a wlaninfo to the array
        * @since S60 5.0
        * @param aWlanInfo The info element to be added to the array. 
        *                  Ownership passed.
        */
        IMPORT_C void AppendL( TWsfWlanInfo* aWlanInfo );
    
        /**
        * Delete the given element from the array
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo to be removed from the array. 
        *                  Ownership not passed.
        */
        IMPORT_C void Delete( TWsfWlanInfo* aWlanInfo );

        /**
        * Wipes all the elements from the array
        * @since S60 5.0
        */
        IMPORT_C void Reset();

        /**
        * Returns the wlaninfo of the given index value in the array
        * @since S60 5.0
        * @param aIndex Index of the element in the array
        * @return Pointer to the wlan info object. Ownership not passed.
        */
        IMPORT_C TWsfWlanInfo* operator[]( TUint aIndex ) const;
        
        /**
        * Returns the wlaninfo of the given index value in the array with
        * boundary check.
        * @since S60 5.0
        * @param aIndex Index of the element in the array
        * @return Pointer to the wlan info object if the given index is valid
        *         (ownership not passed), NULL otherwise
        */
        IMPORT_C TWsfWlanInfo* At( TInt aIndex ) const;
        
        /**
        * Return the number of info items stored in the array 
        * @since S60 5.0
        * @return Number of items in the array.
        */
        IMPORT_C TUint Count();
        
        /**
        * Returns the info array element matching the SSID prior the given index
        * @since S60 5.0
        * @param aSsid The SSID to match
        * @param aPriorThis The index after which results are not reported
      	* @return Pointer to the wlan info object (ownership not passed), or 
      	*         NULL if ssid is not found
      	*/
		IMPORT_C TWsfWlanInfo* Match( const TDesC8& aSsid, 
		                              const TInt aPriorThis ); 
		
		/**
        * Returns the info array element matching the IapID prior the given index
        * @since S60 5.2
        * @param aIapID The IapID to match
        * @param aPriorThis The index after which results are not reported
        * @return Pointer to the wlan info object (ownership not passed), or 
        *         NULL if IapID is not found
        */
        IMPORT_C TWsfWlanInfo* Match( const TUint32 aIapID, 
                                      const TInt aPriorThis );
		
		/**
        * Returns the info array element matching the iap id prior the given index
        * @since S60 5.0
        * @param aIapId The Iap Id to match
        * @param aPriorThis The index after which results are not reported
        * @return Pointer to the wlan info object (ownership not passed), or 
        * NULL if ssid is not found
        */
        IMPORT_C void MatchWithIapIDL( const TUint aIapID, 
                                        const TInt aPriorThis, 
                                        RPointerArray<TWsfWlanInfo>& aMatchArray );
        
        
        /**
         * Returns the info array element matching the SSID, security mode and net mode
         * prior the given index
         * @since S60 5.0
         * @param aSsid The SSID to match
         * @param aSecMode The security mode to match
         * @param aNetMode The network mode to match
         * @param aUsesPreSharedKey pre shared key usage to match
         * @param aPriorThis The index after which results are not reported
         * @return Pointer to the wlan info object (ownership not passed), or 
         *         NULL if ssid is not found
         */		 
        IMPORT_C void MatchL( const TDesC8& aSsid, 
                              CMManager::TWlanSecMode aSecMode, 
                              CMManager::TWlanNetMode aNetMode, 
                              TBool aUsesPreSharedKey,
                              const TInt aPriorThis, 
                              RPointerArray<TWsfWlanInfo>& aMatchArray ); 
        
        /**
        * Serialize the array content to a buffer and pushes it on the 
        * cleanup stack
        * @since S60 5.0
        * @return The buffer created
        */
        IMPORT_C HBufC8* SerializeContentLC();
        
		/**
		* Read wlan info objects from buffer and append them to the array
        * @since S60 5.0
        * @param aStreamBuffer The buffer to read from.
		* @return The new array size
		*/
		IMPORT_C TInt AppendFromStreamBufferL( const TDesC8& aStreamBuffer );

        /**
        * Returns the hidden info array prior the given index
        * @since S60 5.0
        * @param aPriorThis The index after which results are not reported
      	* @return Pointer to the wlan info object (ownership not passed), 
      	*         or NULL if not found
      	*/
		IMPORT_C TWsfWlanInfo* FindHiddenEntry( const TInt aPriorThis );
		
		/**
		* Delete a given number of items starting from the last item in array
        * @since S60 5.0
		* @param aCount The number of items to be deleted
		* @return The count of remaining items in array 
		*/ 
		IMPORT_C TInt DeleteFromTail( const TInt aCount );
		
		/**
		* Sort the array contents in order of
		* configured access points in alphabetical order ( Ssid )
		* unknow networks in aplhabetical order ( Ssid )
		* and the hidden network container
        * @since S60 5.0
		*/
		IMPORT_C void SortArrayL();
		
		/**
        * Set UI priority Sort for array
        * @since S60 5.1
        * @param aUIPrioritySort TBool value.
        */ 
        IMPORT_C void SetUIPrioritySort( const TBool aUIPrioritySort );
		
        /**
        * Get UI Priority Sort value
        * @since S60 5.1
        * @return TBool value representing iUIPrioritySort
        */
        IMPORT_C TBool GetUIPrioritySort();
		
		/*
		* Sort the array using a visitor SortKey
        * @since S60 5.0
        * @param aSortKey The sort key to be used.
		*/
		IMPORT_C void SortArrayL( MWsfWlanInfoArrayFilterVisitor& aSortKey );		
		
 
    private:    // Data
 
        CArrayPtrFlat<TWsfWlanInfo>* iInfoArray;   ///< Owned.
        
        /**
        * If EFalse, UI priority short not in use
        */
        TBool iUIPrioritySort;

	};

#endif // C_WSFWLANINFOARRAY_H

