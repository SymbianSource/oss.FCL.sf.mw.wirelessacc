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



#ifndef C_WSFWLANINFOARRAYVISITOR_H
#define C_WSFWLANINFOARRAYVISITOR_H


//  EXTERNAL INCLUDES
#include <e32base.h>
#include <d32dbms.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfoarrayfiltervisitor.h"
#include "wsfwlaninfo.h"


//  FORWARD DECLARATIONS
class CWsfWlanInfoSortingDefault;
class CWsfWlanInfoSortingBrand;
class CWsfWlanInfoSortingBlackList;
class CEikonEnv;
class CAknIconArray;
class CWsfWlanInfoSortingDbChangeNotifier;

/**
* This class is the main entry point for wlan list sorting services
*
* @lib wsfwlaninfosorting.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CWsfWlanInfoArrayVisitor ): public CBase, 
                                        public TKeyArrayFix, 
                                        public MWsfWlanInfoArrayFilterVisitor 
    {
    public:     // construction
        /**
        * Factory function.
        * @since S60 5.0
        * @param aReadOnly If EFalse, database-dependent filters 
        *                  won't get updated
        * @return Class instance
        */
        IMPORT_C static CWsfWlanInfoArrayVisitor* NewLC( 
                                                       const TBool aReadOnly );

        /**
        * Factory function.
        * @since S60 5.0
        * @param aReadOnly If EFalse, database-dependent filters 
        *                  won't get updated
        * @return Class instance
        */
        IMPORT_C static CWsfWlanInfoArrayVisitor* NewL( 
                                                      const TBool aReadOnly );
        
        IMPORT_C ~CWsfWlanInfoArrayVisitor();
        
    private:
        /**
        * Constructor.
        * @since S60 5.0
        * @param aReadOnly If EFalse, database-dependent filters
        *                  won't get updated
        */
        CWsfWlanInfoArrayVisitor( const TBool aReadOnly );
        
        void ConstructL();    

        
    public:     // new methods
        /**
        * Carries out operations before starting the actual sorting
        * @since S60 5.0
        * @param aArrayToBeFiltered The array we are working on.
        */
        IMPORT_C void FilterPreSortL( CWsfWlanInfoArray* aArrayToBeFiltered );
        
        /**
        * Carries out operations after having sorted the array
        * @since S60 5.0
        * @param aArrayToBeFiltered The array we are working on.
        */
        IMPORT_C void FilterPostSortL( CWsfWlanInfoArray* aArrayToBeFiltered );
        
        /**        
        * Return the sorting key handle
        * @since S60 5.0
        * @return Reference to the sorting key
        */        
        IMPORT_C TKeyArrayFix& SortingKey();        


    public:     // from TKeyArrayFix
        /**
        * Compares elements of the given indices according to the 
        * sorting criteria.
        * @since S60 5.0
        * @param aLeft Index of left element
        * @param aRight Index of right element
        * @return Negative if the left element is less than the right, 
        *         positive if the right element is less than the left, 
        *         zero if the two elements are equal.         
        */
        TInt Compare( TInt aLeft, TInt aRight ) const;


    public:     // new methods
    
        /**
        * Load definition of each filter
        * @since S60 5.0
        */        
        IMPORT_C void LoadFilterDefinitionsL();
    
        /**
        * Add the icons of the given array to the branding icon list
        * @since S60 5.0
        * @param aIconArray The icons to be appended
        */        
        IMPORT_C void AppendBrandingIconsL( CAknIconArray& aIconArray );

        /**
        * Launches the UI to modify the SSID blacklist (filter out networks)
        * @since S60 5.0
        * @param aInfoItemArray The current wlaninfo array
        */        
        IMPORT_C void EditBlackListL( CWsfWlanInfoArray& aInfoItemArray );
        
        /**
        * Returns the number of blacklisted SSIDs
        * @since S60 5.0
        * @return The count of filtered SSIDs 
        */        
        IMPORT_C TInt BlackListItemCount() const;
        

    private:    // new methods
    
        /**
        * Opens the shared filter database
        * @since S60 5.0
        */        
        void OpenDataBaseL();


    private: //data    
        
        /**
        * The wlaninfo array we are currently working on. Not owned.
        */
        CWsfWlanInfoArray* iInfoArray;
    
        /**
        * Default sorting filter. Owned.
        */
        CWsfWlanInfoSortingDefault* iDefaultSorting;

        /**
        * Branded item sorting filter. Owned.
        */
        CWsfWlanInfoSortingBrand* iBrandSorting;

        /**
        * Blacklisted item sorting filter. Owned.
        */
        CWsfWlanInfoSortingBlackList* iBlackListSorting;

        /**
        * Notifier of database changes. Owned.
        */
        CWsfWlanInfoSortingDbChangeNotifier* iDbChangeNotifier;

        /**
        * Reference to the UI environment
        */
        CEikonEnv& iCoeEnv;
    
        /**
        * Resource file offset
        */
        TInt iResourceFileOffset;
    
        /**
        * Handle to the filter database
        */
        RDbNamedDatabase iFilterDb;

        /**
        * Handle to the DBMS
        */
        RDbs iDbms;

        /**
        * Handle to the FS
        */
        RFs iFileServer;

        /**
        * If EFalse, database-dependent filters won't get updated
        */
        TBool iReadOnly;
    
    };
    



#endif // C_WSFWLANINFOARRAYVISITOR_H

