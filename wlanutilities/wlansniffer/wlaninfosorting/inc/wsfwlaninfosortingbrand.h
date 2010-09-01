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
* Description:  Class header for CWsfWlanInfoSortingBrand
*
*/



#ifndef C_WSFWLANINFOSORTINGBRAND_H
#define C_WSFWLANINFOSORTINGBRAND_H


//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfosortingfiltervisitor.h"
#include "wsfwlaninfo.h"


//  FORWARD DECLARATIONS
class CAknIconArray;
class CGulIcon;


/**
* Branded item sorting filter
*
* @lib wsfwlaninfosorting.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CWsfWlanInfoSortingBrand ): public CBase, 
                                        public MWsfWlanInfoSortingFilterVisitor
    {
    protected:  // Type definitions

        /**
        * Structure to store and compare branded items
        *
        * @lib wsfwlaninfosorting.lib
        * @since S60 5.0
        */
        class TBrandableItem
            {
            public:     // constructors
                TBrandableItem();
                
                /**
                * Constructor.
                * @since S60 5.0
                * @param aSsid SSID of the branded network
                * @param aBrandId ID of the branded network
                */
                TBrandableItem( const TDesC8& aSsid, const TInt8 aBrandId );
                
            public:     // new methods
                /**
                * Compares two branded items.
                * @since S60 5.0
                * @param aItemLeft First item
                * @param aItemRight Second item
                * @return ETrue if the two items are equal
                */
                static TBool Match( const TBrandableItem& aItemLeft, 
                                    const TBrandableItem& aItemRight );

                /**
                * Defines a sorting relation between two branded items based on
                * their brand id.
                * @since S60 5.0
                * @param aItemLeft Left item
                * @param aItemRight Right item
                * @return Negative if left < right, zero if left == right, 
                *         positive if left > right.
                */
                static TInt SortByBrandId( const TBrandableItem& aItemLeft, 
                                           const TBrandableItem& aItemRight );

                /**
                * Defines a sorting relation between two branded items based on
                * their SSID.
                * @since S60 5.0
                * @param aItemLeft Left item
                * @param aItemRight Right item
                * @return Negative if left < right, zero if left == right, 
                *         positive if left > right.
                */
                static TInt SortBySsid( const TBrandableItem& aItemLeft, 
                                        const TBrandableItem& aItemRight );
                                        
            public:     // data
                /**
                * SSID of the branded item
                */
                TBuf8<KWlanMaxAccessPointNameLength> iSsid;
                
                /**
                * Brand identifier number
                */
                TInt8 iBrandId;
                
                /**
                * Icon id for this branded item
                */
                TInt iIconId;

                /**
                * Icon mask id for this branded item
                */
                TInt iMaskId;
            };

    public:     // construction
        static CWsfWlanInfoSortingBrand* NewLC();
        static CWsfWlanInfoSortingBrand* NewL();
        
        ~CWsfWlanInfoSortingBrand();
        
        
    private:    // constructors
        CWsfWlanInfoSortingBrand();
        void ConstructL();    

        
      public:     // new methods

        /**
        * Append the icons from the given array to the available icons list
        * @since S60 5.0
        * @param aIconArray The array to take the icons from
        */
        void AppendBrandIconsL( CAknIconArray& aIconArray );
        
        
    public:     // from MWsfWlanInfoSortingFilterVisitor
        
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

    
    private:    // new methods
    
        /**
        * Creates an icon of the given icon and mask id
        * @since S60 5.0
        * @param aIconId Icon ID
        * @param aMaskId        
        * @return The created icon. Ownership passed.
        */
        CGulIcon* MakeIconLC( const TInt aIconId, const TInt aMaskId );
                
        /**
        * Read the branded networks' data from file/resource
        * @since S60 5.0
        */
        void ReadBrandedSsidsL();

        
    private: //data    

        /**
        * Array of branded items
        */        
        RPointerArray<TBrandableItem> iBrandItemArray;     

        /**
        * Name of the bitmap file for the brand icons. Owned.
        */
        HBufC* iBitmapFileName;
        
        /**
        * Indicates whether the read data is ok
        */
        TBool iBrandingDataOk;
    };
    


#endif // C_WSFWLANINFOSORTINGBRAND_H

