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
* Description:  Class header for CWsfDetailsViewModel
*
*/

#ifndef C_WSFDETAILSVIEWMODEL_H
#define C_WSFDETAILSVIEWMODEL_H

//EXTERNAL INCLUDES
#include <e32base.h>
#include <rconnmon.h>


//INTERNAL INCLUDES
#include "wsfwlaninfo.h"


//FORWARD DECLARATIONS
class CCoeEnv;
class CWsfWlanInfoArray;


/**
 *  Details view Model
 *
 *  Holds & formats TWlanInfo for list box showable format
 *
 *  @lib Sniffer.app
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CWsfDetailsViewModel ): public CBase
    {

    public: 
    
        // Constructors and the C++ destructor
        /**
         * Two-phased constructor
         * @since S60 5.0 
         * @return instance of CWsfDetailsViewModel class
         */
        IMPORT_C static CWsfDetailsViewModel* NewL();
        
        /**
         * Two-phased constructor. Leaves the object on CleanupStack
         * @since S60 5.0 
         * @return instance of CWsfDetailsViewModel class
         */
        IMPORT_C static CWsfDetailsViewModel* NewLC();
        
        /**
         * Destructor of CWsfDetailsViewModel class
         * @since S60 5.0     
         */
        ~CWsfDetailsViewModel();
    
    
        // New methods
        
        /**
         * Formats Wlan info for showing
         *
         * @since S60 v5.0
         * @param iWlanInfo - selected Wlan info
         * @param aWlanArray - Wlan info array
         * @return formated text array
         */
        CDesCArrayFlat* FormatWlanInfoL( TWsfWlanInfo* aWlanInfo, 
                                         CWsfWlanInfoArray* aWlanArray );
    
        /**
         * Formats next Wlan info from array
         *
         * @since S60 v5.0
         * @return formated text array
         */
        CDesCArrayFlat* FormatNextWlanInfoL();
    
        /**
         * Formats previous Wlan info from array
         *
         * @since S60 v5.0
         * @return formated text array
         */
        CDesCArrayFlat* FormatPreviousWlanInfoL();
    
        /**
         * Formats text for pane to show
         *
         * @since S60 v5.0
         * @return formated text array
         */
        HBufC* FormatPaneTextLC();
        
        /**
         * Formats Left Scroll Button for pane to show
         *
         * @return boolen (EFalse show the arrow, ETrue hide it)
         */
        TBool FormatLeftScrollButton();
        
        /**
         * Formats Right Scroll Button for pane to show
         *
         * @return boolen (EFalse show the arrow, ETrue hide it)
         */
        TBool FormatRightScrollButton();
        
        /**
         * Returns WLAN details that has already formatted
         * @since S60 v5.0
         * @return Formatted WLAN details 
         */        
        CDesCArrayFlat* GetWlanDetails();  
    
        /**
         * Returns current Wlan Ssid
         * @since S60 v5.0
         * @return Ssid
         */        
        const TDesC8& WlanSsid() const;
    
    private:
    
        // Constructors and the C++ destructor
        
        /**
         * Default C++ constructor 
         * @since S60 5.0 
         */
        CWsfDetailsViewModel();
        
        /**
         * 2nd phase constructor
         * @since S60 5.0 
         */
        void ConstructL();
        
        // New methods
        
        /**
         * Creates listbox items from header and value (formatter %S\t%S)
         * @since S60 v5.0
         * @param aResourceId - id of loaded header string
         * @param aValueText - string containing value data 
         * @return formated string 
         */        
        HBufC* FormatListBoxItemLC( const TUint aResourceId,
                                    const TDesC& aValueText ) const;
    
        /**
         * Appends Wlan IAP Name to iFormattedWlanInfo
         * @since S60 v5.0
         */        
        void FormatAndAppenWlanIapNameL();
    
        /**
         * Appends Wlan Ssid to iFormattedWlanInfo
         * @since S60 v5.0
         */        
        void FormatAndAppenWlanSsidL();
    
        /**
         * Appends singnal strength to iFormattedWlanInfo
         * @since S60 v5.0
         */        
        void FormatAndAppenSingnalL();
    
        /**
         * Appends wlan mode to iFormattedWlanInfo
         * @since S60 v5.0
         */        
        void FormatAndAppenModeL();
    
        /**
         * Appends wlan security to iFormattedWlanInfo
         * @since S60 v5.0
         */        
        void FormatAndAppenSecurityL();
    
        /**
         * Appends wlan coverage to iFormattedWlanInfo
         * @since S60 v5.0
         */        
        void FormatAndAppenCoverageL();
    
        /**
         * Appends wlan speed to iFormattedWlanInfo
         * @since S60 v5.0
         */        
        void FormatAndAppenSpeedL();
    
        /**
         * Formats signal strenth to string 
         * @since S60 v5.0
         * @return formated string
         */        
        HBufC* FormatSingnalStrenghtLC();
        
        /**
         * Formats wlan mode to string 
         * @since S60 v5.0
         * @return formated string
         */        
        HBufC* FormatModeLC();
    
        /**
         * Formats wlan security to string 
         * @since S60 v5.0
         * @return formated string
         */        
        HBufC* FormatSecurityLC();
    
        /**
         * Formats wlan coverage to string 
         * @since S60 v5.0
         * @return formated string
         */        
        HBufC* FormatCoverageLC();
    
        /**
         * Formats wlan technology to string 
         * @since S60 v5.2
         * @return formated string
         */        
        HBufC* FormatTechnologyLC();
    
        /**
        * Calculate number of visible (Not hidden) WLANs
        * @since S60 v.3.0
        * @param aArray Array of WLAN passed as reference
        * @return Number of visible WLANs
        */ 
        TInt VisibleWlans( CWsfWlanInfoArray& aArray );
    
    private: // data
    
        /**
         * Copy of showed Wlan info
         * Own.
         */
        TWsfWlanInfo iWlanInfo;
        
        /**
         * Formater Wlan info
         * Own.
         */
        CDesCArrayFlat* iFormattedWlanInfo;    
    
        /**
         * Current List index;
         * Own.
         */
        TInt iIndex;
    
        /**
         * Coe env for loading string from resource
         * Ref.
         */
        CCoeEnv* iCoeEnv; 
        
        /**
         * Reference to Wlan array
         * ref.
         */
        CWsfWlanInfoArray* iWlanArray;

    };

#endif // C_WSFDETAILSVIEWMODEL_H
