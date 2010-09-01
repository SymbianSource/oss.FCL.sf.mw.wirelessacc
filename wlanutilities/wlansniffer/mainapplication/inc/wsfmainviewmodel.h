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
* Description:  Class header for CWsfMainViewModel
*
*/

#ifndef C_WSFMAINVIEWMODEL_H
#define C_WSFMAINVIEWMODEL_H

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <gulicon.h>
#include <AknIconArray.h>
#include "wsfwlaninfo.h"



//  FORWARD DECLARATIONS
class CWsfWlanInfoArray;
class CCoeEnv;

//  CLASS DEFINITION
/**
 * View (Main view) level model.
 *  
 * @since S60 5.0
 * @lib wlansniffer.exe
 */
NONSHARABLE_CLASS( CWsfMainViewModel ): public CBase
    {
    public:     // Constructors and destructor
        
        /**
         * Two-phased constructor
         * @since S60 5.0 
         * @return instance of CWsfMainViewModel class
         */
        static CWsfMainViewModel* NewL();
        
        /**
         * Two-phased constructor. Leaves the object on CleanupStack
         * @since S60 5.0 
         * @return instance of CWsfMainViewModel class
         */
        static CWsfMainViewModel* NewLC();
        
        /**
         * Destructor of CWsfMainViewModel class
         * @since S60 5.0     
         */
        ~CWsfMainViewModel();

    private:    // Constructors
        
        /**
         * Default C++ constructor 
         * @since S60 5.0 
         */
        CWsfMainViewModel();
        
        /**
         * 2nd phase constructor
         * @since S60 5.0 
         */
        void ConstructL();

    public:     // New methods

        /**
        * Creates array for the listbox.
        * @param Array of WLAN that will be formatted. 
        */
        void FormatWlanListL( CWsfWlanInfoArray* aWlanList );
        
        /*
        * Returns WLAN list that has already formatted
        * @return Formatted WLAN list 
        */        
        CDesCArrayFlat* GetWlanList();        
        
        /**
        * 
        * @return non-formatted WLAN list
        */
        CWsfWlanInfoArray* GetInfoArray();
        
        /**
        * Store Ssid of selected WLAN
        * Get ownership of parameter
        * @param aSsid WLAN name
        */
        void SetSelectedWlan( HBufC8* aSsid );

        /**
        * Return Ssid of selected WLAN
        * @return WLAN name
        */
        const TDesC8& SelectedWlan();

        /**
        * Stored current listbox index
        * @param aIndex Listbox iden
        */        
        void SetSelectedIndex( TInt aIndex );

        /**
        * Return stored index listbox
        * @return Previously stored listbox index
        */        
        TInt SelectedIndex();

        /**
        * Returns navipane text
        * @return Navipane text
        */        
        HBufC* FormatNaviPaneLC();  

    private:    // New methods        
        
        /**
         * Format the Hidden WLANs for the listbox.(text and icon)
         * @since S60 v5.0
         * @param aWlan - Reference to wlaninfo
         * @param aItem - Listbox item
         */     
        void FormatHiddenWlanItemL( TWsfWlanInfo& aWlan, TDes& aItem );

        /**
         * Format the WlanInfo (visible networks) items for the 
         * listbox.(text and icon)
         * @since S60 v5.0
         * @param aWlan - reference to wlaninfo
         * @param aItem - listbox item
         */  
        void FormatVisibleWlanItemL( TWsfWlanInfo& aWlan, TDes& aItem );
        
    private:    // Data
       
        /**
        * The wlaninfo array we are currently working on. 
        * Not Own. (Ref.)
        */
        CWsfWlanInfoArray* iWlanInfoArray;
        
        /**
         * Coe env for loading string from resource
         * Not Own. (Ref.)
         */
        CCoeEnv* iCoeEnv;

        /**
        * List that contains the WLAN items 
        * Own.
        */
        CDesCArrayFlat* iFormattedWlanList;  
        
        /**
        * Store name of focused WLAN 
        * Own.
        */
        HBufC8* iSelectedWlan;
        
        /**
        * Index of the selected listbox item.
        */
        TInt iListboxIndex;
        
        /**
         * It contains the number of the Visible WLANs
         */
        TUint iVisibleWlans;
    };



#endif      //  C_WSFMAINVIEWMODEL_H

// End of file
