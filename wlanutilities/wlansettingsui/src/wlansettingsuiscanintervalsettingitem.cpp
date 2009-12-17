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
 
// INCLUDE FILES

#include "wlansettingsui.hrh"
#include "wlansettingsuimodel.h"
#include "wlansettingsuiscanintervalsettingitem.h"

#include <wlansettingsui.rsg>
#include <AknQueryDialog.h>
#include <StringLoader.h>
#include <aknnotewrappers.h>

// CONSTANTS

/**
* User defined scan interval minimum
*/
const TInt KScanIntervalMin = EWlanSettingsUiScanNetworkMin;

/**
* User defined scan interval maximum
*/
const TInt KScanIntervalMax = EWlanSettingsUiScanNetworkMax;


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CWlanSettingsUiScanIntervalSettingItem::
//                                      CWlanSettingsUiScanIntervalSettingItem
// ---------------------------------------------------------------------------
//
CWlanSettingsUiScanIntervalSettingItem::
    CWlanSettingsUiScanIntervalSettingItem( 
        TInt aResourceId, TInt& aListValue, TInt& aValue,
        CWlanSettingsUiModel* aModel ) : 
    CAknEnumeratedTextPopupSettingItem( aResourceId, aListValue ),
    iModel( aModel ),
    iValue( aValue ),
    iEnv( CEikonEnv::Static() ),
    iRevertBackToAutomaticScan( EFalse ) 
    {        
    };

// ---------------------------------------------------------------------------
// CWlanSettingsUiScanIntervalSettingItem::
//                                     ~CWlanSettingsUiScanIntervalSettingItem
// Destructor
// ---------------------------------------------------------------------------
//
CWlanSettingsUiScanIntervalSettingItem::
    ~CWlanSettingsUiScanIntervalSettingItem()
    {
    iSettingText.Close();
    }

// ---------------------------------------------------------------------------
// CWlanSettingsUiScanIntervalSettingItem::SettingTextL
// ---------------------------------------------------------------------------
//
const TDesC& CWlanSettingsUiScanIntervalSettingItem::SettingTextL()
    {
    HBufC* tempText;
    
    iSettingText.Close();
    
    if ( InternalValue() == EWlanSettingsUiScanNetworkAuto )
        {
        tempText = iEnv->AllocReadResourceL( R_WLAN_SETT_BG_SCAN_AUTO );
        }
    else
        {
        if ( iValue == 1 )
            {
            tempText = iEnv->AllocReadResourceL( R_WLAN_SETT_BG_SCAN_1_MIN );
            }
        else
            {
            tempText = StringLoader::LoadL( R_WLAN_SETT_BG_SCAN_X_MIN, iValue, iEnv );
            }
        }
    
    iSettingText.Assign( tempText );
    
    AknTextUtils::LanguageSpecificNumberConversion( iSettingText );
    
    return iSettingText;
    }

// ---------------------------------------------------------------------------
// CWlanSettingsUiScanIntervalSettingItem::EditItemL
// ---------------------------------------------------------------------------
//
void CWlanSettingsUiScanIntervalSettingItem::EditItemL( TBool aCalledFromMenu )
    {
    CAknEnumeratedTextPopupSettingItem::EditItemL( aCalledFromMenu );
    
    if ( iRevertBackToAutomaticScan )
        {
        // Scan mode needs to be reverted back to automatic because we are
        // either in power saving mode or user cancelled the user-defined
        // scan interval query
        SetInternalValue( EWlanSettingsUiScanNetworkAuto );
        SetSelectedIndex( IndexFromValue( EWlanSettingsUiScanNetworkAuto ) );
        UpdateListBoxTextL();
        iRevertBackToAutomaticScan = EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CWlanSettingsUiScanIntervalSettingItem::HandleSettingPageEventL
// ---------------------------------------------------------------------------
//
void CWlanSettingsUiScanIntervalSettingItem::HandleSettingPageEventL(
        CAknSettingPage* aSettingPage,
        TAknSettingPageEvent aEventType )
    {
    if ( aEventType != EEventSettingCancelled )
        {
        CEikListBox* listBox = (CEikListBox*)aSettingPage->EditorControl();
        TInt index = listBox->CurrentItemIndex();
        
        if ( index == IndexFromValue( EWlanSettingsUiScanNetworkUserDef ) )
            {
            // User-defined scan interval selected
            if ( !iModel->PsmModeOn() )
                {
                if ( iValue == 0 )
                    {
                    // Set default scan interval for query dialog
                    iValue = KWlanSettingsUiDefaultUserDefScanInterval;
                    }
                if ( ShowScanIntervalQueryL() == FALSE )
                    {
                    // Dialog was cancelled, revert back to automatic scan?
                    if ( InternalValue() == EWlanSettingsUiScanNetworkAuto )
                        {
                        // Previous selection was automatic, revert back to
                        // automatic
                        iRevertBackToAutomaticScan = ETrue;
                        }
                    }
                }
            else
                {
                // We are in power saving mode, user-defined interval not allowed
                ShowPowerSavingInfoNoteL();
                iRevertBackToAutomaticScan = ETrue;
                }
            }
        }

    CAknSettingItem::HandleSettingPageEventL(aSettingPage, aEventType);
    }

// ---------------------------------------------------------------------------
// CWlanSettingsUiScanIntervalSettingItem::ShowScanIntervalQueryL
// ---------------------------------------------------------------------------
//
TBool CWlanSettingsUiScanIntervalSettingItem::ShowScanIntervalQueryL()
    {
    HBufC* msg = StringLoader::LoadLC( R_WLAN_PRMPT_BG_SCAN_INTERVAL );

    CAknNumberQueryDialog* dialog = CAknNumberQueryDialog::NewL( iValue );
    dialog->PrepareLC( R_WLAN_SETTINGS_UI_NUMBER_EDITOR_2 );
    dialog->SetPromptL( *msg );
    dialog->SetMinimumAndMaximum( KScanIntervalMin, KScanIntervalMax );
    TBool result = ( dialog->RunLD() == EAknSoftkeyOk );
    CleanupStack::PopAndDestroy( msg );
    return result; 
    }

// ---------------------------------------------------------------------------
// CWlanSettingsUiScanIntervalSettingItem::ShowPowerSavingInfoNoteL
// ---------------------------------------------------------------------------
//
void CWlanSettingsUiScanIntervalSettingItem::ShowPowerSavingInfoNoteL()
    {
    HBufC* prompt = StringLoader::LoadLC(
        R_WLAN_SETTINGS_UI_POWER_SAVING_INFONOTE );
    CAknInformationNote* note = new ( ELeave ) CAknInformationNote( ETrue );
    note->ExecuteLD( *prompt );
    CleanupStack::PopAndDestroy( prompt );                    
    }
