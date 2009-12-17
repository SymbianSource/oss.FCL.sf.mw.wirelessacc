/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of class CWlanSettingsUiScanIntervalSettingItem.
*
*/


#ifndef __WLAN_SETTINGS_UI_SCAN_INTERVAL_SETTING_ITEM_H__
#define __WLAN_SETTINGS_UI_SCAN_INTERVAL_SETTING_ITEM_H__

// System includes
#include <aknsettingitemlist.h>


/**
 *  Specialized version of CAknEnumeratedTextPopupSettingItem class.
 *
 *  @lib wlansettingsui.lib
 *  @since S60 v5.2
 */
NONSHARABLE_CLASS(CWlanSettingsUiScanIntervalSettingItem) : 
    public CAknEnumeratedTextPopupSettingItem
    {
    public:

        /**
        * Constructor
        * @param aResourceId Resource id for the parent class
        * @param aValue Value for the parent class
        */
        CWlanSettingsUiScanIntervalSettingItem( 
                        TInt aResourceId, TInt &aListValue, TInt& aValue,
                        CWlanSettingsUiModel* aModel );
    
        /**
        * Destructor.
        */       
        virtual ~CWlanSettingsUiScanIntervalSettingItem();

        /**
        * From CAknEnumeratedTextPopupSettingItem
        * @return Setting item text (current value).
        */
        const TDesC& SettingTextL();
        
        /**
        * From CAknEnumeratedTextPopupSettingItem
        * @param aCalledFromMenu Ignored in this class.
        */
        void EditItemL( TBool aCalledFromMenu );

        /**
        * From CAknSettingItem
        * @param aSettingPage Pointer to setting page which has caused this callback.
        * @param aEventType Occurred setting page event type.
        */
        void HandleSettingPageEventL( CAknSettingPage* aSettingPage,
                                      TAknSettingPageEvent aEventType );
        
    private:    

        /**
        * Displays scan interval query
        */
        TBool ShowScanIntervalQueryL();

        /**
        * Shows an information note if user tries to select a non-allowed
        * value during full or partial PowerSave Mode.
        */
        void ShowPowerSavingInfoNoteL();   
        
    private: // Data
        
        CWlanSettingsUiModel* iModel;
        RBuf iSettingText;
        TInt& iValue;
        CEikonEnv* iEnv;
        TBool iRevertBackToAutomaticScan;
    };

#endif // __WLAN_SETTINGS_UI_SCAN_INTERVAL_SETTING_ITEM_H__
//End of file
