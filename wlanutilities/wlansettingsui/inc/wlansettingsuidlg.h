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
*      Declaration of class CWlanSettingsUiDlg.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_DLG_H
#define WLAN_SETTINGS_UI_DLG_H

// INCLUDES

#include <AknDialog.h>

// FORWARD DECLARATIONS

class CWlanSettingsUiSettingItemList;
class CWlanSettingsUiModel;

// CLASS DECLARATION

/**
* WLAN settings dialog base.
*/
NONSHARABLE_CLASS( CWlanSettingsUiDlg ): public CAknDialog
    {

    protected:  // Constructors

        /**
        * Constructor.
        * @param aModel Data model.
        */
        inline CWlanSettingsUiDlg( CWlanSettingsUiModel* aModel );

    public:     // New methods

        /**
        * Construct and execute the dialog.
        * @return ETrue if dialog was accepted.
        */
        virtual TInt ExecuteLD();

    protected:  // From CAknDialog & its base classes

        /**
        * Create custom control.
        * @param aControlType Control type.
        * @return Control info.
        */
        SEikControlInfo CreateCustomControlL( TInt aControlType );

        /**
        * Process command.
        * @param aCommandId Command id.
        */
        void ProcessCommandL( TInt aCommandId );
        
        /**
         * From @c CEikDialog.
         *
         * Handles a dialog button press for the specified button. 
         *
         * This function is invoked when the user presses a button in the
         * button panel. It is not called if the Cancel button is activated 
         * unless the @c EEikDialogFlagNotifyEsc flag has been set.
         *
         *
         * @param aButtonId The ID of the button that was activated.
         * @return @c ETrue to validate and exit the dialog, 
         *         @c EFalse to keep the dialog active. If @c ETrue, the 
         *         dialog will be destroyed automatically by @c ExecuteLD(). 
         *         The default implementation returns @c ETrue no matter which
         *         button is pressed.
         */
        TBool OkToExitL(TInt aButtonId);
        
    protected:  // New methods

        /**
        * Get the setting item list inside.
        */
        CWlanSettingsUiSettingItemList& SettingItemList() const;

        /**
        * Get menu bar resource id.
        * @return Menu bar resource id.
        */
        virtual TInt MenuBarResourceId() const = 0;

        /**
        * Get dialog resource id.
        * @return Dialog resource id.
        */
        virtual TInt DialogResourceId() const = 0;
        
        /**
        * Check if Help is supported on the phone or not, querying the 
        * KFeatureIdHelp feature flag.
        * 
        * @return True  If Help is supported, 
        *         False If Help is not supported.
        */
        static TBool IsHelpSupported();

    protected:  // Data

        CWlanSettingsUiModel* iModel;   ///< Data model.

        };

#include "wlansettingsuidlg.inl"

#endif
