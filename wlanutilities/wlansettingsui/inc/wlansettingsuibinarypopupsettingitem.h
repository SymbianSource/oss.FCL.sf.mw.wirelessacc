/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*      Declaration of class CWlanSettingsUiBinaryPopupSettingItem.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_BINARY_POPUP_SETTING_ITEM_H
#define WLAN_SETTINGS_UI_BINARY_POPUP_SETTING_ITEM_H

// INCLUDES

#include <aknsettingitemlist.h>

// FORWARD DECLARATIONS

class CWlanSettingsUiBinaryPopupSettingItem;

// CLASS DECLARATION

/**
* Interface to observe change in internal value of an
* CAknBinaryPopupSettingItem.
*/
NONSHARABLE_CLASS( MWlanSettingsUiBinaryPopupSettingObserver )
    {

    public:     // New methods

        /**
        * Binary setting internal value has changed.
        * @param aItem Changed item.
        * @param aNewInternalVal New internal value.
        */
        virtual void BinarySettingChangedL
            (
            CWlanSettingsUiBinaryPopupSettingItem* aItem,
            TBool aNewInternalVal
            ) = 0;

    };

/**
* Binary setting item.
* Added functionality: notifies observer about setting change.
*/
NONSHARABLE_CLASS( CWlanSettingsUiBinaryPopupSettingItem )
: public CAknBinaryPopupSettingItem
    {

    public:     // Constructors

        /**
        * Constructor.
        * @param aObserver observer reference
        * @param aResourceId Resource id.
        * @param aValue Value.
        */
        CWlanSettingsUiBinaryPopupSettingItem
            (
            MWlanSettingsUiBinaryPopupSettingObserver& aObserver,
            TInt aResourceId,
            TBool& aValue
            );

    private:    // From CAknSettingItem & base classes

        /**
        * Edit item.
        * @param aCalledFromMenu ETrue if called from menu.
        */
        void EditItemL( TBool aCalledFromMenu );

    private:    // Data

        MWlanSettingsUiBinaryPopupSettingObserver& iObserver;   ///< Observer.

        };

#endif
