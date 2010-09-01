/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*      Declaration of class CWlanSettingsUiSettingItemList.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_SETTING_ITEM_LIST_H
#define WLAN_SETTINGS_UI_SETTING_ITEM_LIST_H

// INCLUDES

#include <aknsettingitemlist.h>
#include "wlansettingsuibinarypopupsettingitem.h"

// CLASS DECLARATION
class CWlanSettingsUiModel;

/**
* WLAN settings item list.
* Added functionality:
* - Creates proper type setting items, based on setting id.
*/
NONSHARABLE_CLASS( CWlanSettingsUiSettingItemList )
: public CAknSettingItemList,
  public MWlanSettingsUiBinaryPopupSettingObserver
    {

    public:     // Constructors

        /**
        * Constructor.
        * @param aModel Data model.
        */
        inline CWlanSettingsUiSettingItemList
            ( CWlanSettingsUiModel* aModel );

    public:     // New methods

        /**
        * Edit current item.
        * @param aCalledFromMenu ETrue if called from menu.
        */
        void EditCurrentItemL( TBool aCalledFromMenu );

    public: // From CAknSettingItemList & base classes

        /**
        * Activate.
        */
        void ActivateL();

        /**
        * Load settings.
        */
        void LoadSettingsL();
        
        /**
        * Handle resource change
        * @param aType message UID value
        */
        void HandleResourceChange( TInt aType );

        /**
        * Launches the setting page for the current item by calling 
        * EditItemL on it
        * @param aIndex Current item's (Visible) index in the list
        * @param aCalledFromMenu Ignored in most classes; may alter 
        * the behaviour of the setting page
        */
        void EditItemL( TInt aIndex, TBool aCalledFromMenu );

        /**
        * Create setting item.
        * @param aSettingId Setting id.
        * @return Setting item.
        */
        CAknSettingItem* CreateSettingItemL( TInt aSettingId );

        /**
        * Set view (hide / show items as appropriate).
        */
        void RefreshViewL();

    private:    // From MWlanSettingsUiBinaryPopupSettingObserver

        /**
        * Binary setting internal value has changed.
        * @param aItem Changed item.
        * @param aNewInternalVal New internal value.
        */
        void BinarySettingChangedL
            (
            CWlanSettingsUiBinaryPopupSettingItem* aItem,
            TBool aNewInternalVal
            );

    private:    // New methods

        /**
        * Sets the drawing rectangle for the listbox.
        */
        void SetListBoxRect();
       
    
    private:    // Data

        CWlanSettingsUiModel* iModel;///< Data model.

        TBool iShowWlanAvail;        ///< Show WLAN availablility on/off
        
        ///< Scan networks mode selection (automatic/user-defined)
        TInt iScanMode;
        
        ///< User selected scan interval needs to be checked if in PSM mode              
        TInt iScanInterval;      

        TInt iPowerSaving;
        
        // Old scan networks value is reverted back if 
        // user turns PSM mode on and then off in settings UI
        TBool iChangeScanNetworksValueBack;
        
        // Back up for the scan networks value
        TUint iScanNetworksBackUp;
        
        };

#include "wlansettingsuisettingitemlist.inl"

#endif
