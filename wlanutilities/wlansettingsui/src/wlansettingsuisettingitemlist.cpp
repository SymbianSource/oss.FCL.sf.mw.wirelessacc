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
*      Implementation of class CWlanSettingsUiSettingItemList.   
*      
*
*/


// INCLUDE FILES

#include "wlansettingsuisettingitemlist.h"
#include "wlansettingsuibinarypopupsettingitem.h"
#include "wlansettingsuimodel.h"
#include "wlansettingsuipanic.h"
#include "wlansettingsuidefs.h"
#include "wlansettingsui.hrh"
#include "wlansettingsuiscanintervalsettingitem.h"

#include <wlansettingsui.rsg>


// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CWlanSettingsUiSettingItemList::EditItemL
// ---------------------------------------------------------
//
void CWlanSettingsUiSettingItemList::EditItemL( TInt aIndex, 
                                                TBool aCalledFromMenu )
    {
    __ASSERT_ALWAYS ( aIndex < SettingItemArray()->Count(), \
                                        User::Leave( KErrArgument ) );
        
    CAknSettingItem* item = SettingItemArray()->At ( aIndex );

    CAknSettingItemList::EditItemL( aIndex, aCalledFromMenu);                        
    CAknSettingItemList::StoreSettingsL();
    
    switch ( item->Identifier() )
        {
        case EWlanSettingsUiSettingScanNetworks:
            {
            if ( iScanMode == EWlanSettingsUiScanNetworkAuto )
                {
                iModel->SetScanNetworks( KWlanSettingsUiScanNetworkAuto );
                }
            else
                {
                iModel->SetScanNetworks( iScanInterval );
                }
            break;                        
            }
            
        case EWlanSettingsUiSettingPowerSaving:
            {
            iModel->SetPowerSaving( iPowerSaving );
            break;
            }

        default:
            {
            break;
            }
        }
    iModel->SaveSettingsL();
    
    DrawNow();
    }

// ---------------------------------------------------------
// CWlanSettingsUiSettingItemList::EditCurrentItemL
// ---------------------------------------------------------
//
void CWlanSettingsUiSettingItemList::EditCurrentItemL( TBool aCalledFromMenu )
    {
    TInt index = SettingItemArray()->ItemIndexFromVisibleIndex
        ( ListBox()->CurrentItemIndex() );
    EditItemL( index, aCalledFromMenu );
    }

// ---------------------------------------------------------
// CWlanSettingsUiSettingItemList::LoadSettingsL
// ---------------------------------------------------------
//
void CWlanSettingsUiSettingItemList::LoadSettingsL()
    {
    CAknSettingItemList::LoadSettingsL();
    iShowWlanAvail = iModel->ShowWlanAvail();
    iScanInterval = iModel->ScanNetworks();
    if ( static_cast<TUint>( iScanInterval ) ==
        KWlanSettingsUiScanNetworkAuto )
        {
        iScanMode = EWlanSettingsUiScanNetworkAuto;
        iScanInterval = KWlanSettingsUiDefaultUserDefScanInterval;
        }
    else
        {
        iScanMode = EWlanSettingsUiScanNetworkUserDef;
        }
    iPowerSaving = iModel->PowerSaving();
    iChangeScanNetworksValueBack = EFalse;
    RefreshViewL();
    }


// ---------------------------------------------------------
// CWlanSettingsUiSettingItemList::CreateSettingItemL
// ---------------------------------------------------------
//
CAknSettingItem* CWlanSettingsUiSettingItemList::CreateSettingItemL
( TInt aSettingId )
    {
    CAknSettingItem* item;
    switch ( aSettingId )
        {
        case EWlanSettingsUiSettingShowWlanAvail:
            {
            item = new (ELeave) CWlanSettingsUiBinaryPopupSettingItem
                ( *this, aSettingId, iShowWlanAvail );
            break;
            }

        case EWlanSettingsUiSettingScanNetworks:
            {
            item = new (ELeave)CWlanSettingsUiScanIntervalSettingItem
                    ( aSettingId, iScanMode, iScanInterval, iModel );
            break;            
            }

        case EWlanSettingsUiSettingPowerSaving:
            {
            item = new (ELeave) CAknBinaryPopupSettingItem
                ( aSettingId, iPowerSaving );
            break;
            }
            
        default:
            {
            __ASSERT_DEBUG( EFalse, \
                WlanSettingsUiPanic( EWlanSettingsUiBadSetting ) );
            item = new (ELeave) CAknSettingItem( aSettingId );
            break;
            }
        }
    return item;
    }

// ---------------------------------------------------------
// CWlanSettingsUiSettingItemList::BinarySettingChangedL
// ---------------------------------------------------------
//
void CWlanSettingsUiSettingItemList::BinarySettingChangedL
( CWlanSettingsUiBinaryPopupSettingItem* aItem, TBool aNewInternalVal )
    {
    switch ( aItem->Identifier() )
        {
        case EWlanSettingsUiSettingShowWlanAvail:
            {
            iShowWlanAvail = aNewInternalVal;
            iModel->SetShowWlanAvail( aNewInternalVal );
            break;
            }
			
        default:
            {
            __ASSERT_DEBUG( EFalse, \
                WlanSettingsUiPanic( EWlanSettingsUiBadSetting ) );
            break;
            }
        }
    RefreshViewL();
    }

// ---------------------------------------------------------
// CWlanSettingsUiSettingItemList::RefreshViewL
// ---------------------------------------------------------
//
void CWlanSettingsUiSettingItemList::RefreshViewL()
    {
    TInt i;
    CAknSettingItem* item;
    for ( i = 0; i < SettingItemArray()->Count(); i++ )
        {
        item = SettingItemArray()->At( i );
        switch ( item->Identifier() )
            {
            case EWlanSettingsUiSettingShowWlanAvail:
                {
                // Always shown.
                break;
                }

            case EWlanSettingsUiSettingScanNetworks:
                {
                // Hidden if Show WLAN Availability is off.
                item->SetHidden( !iShowWlanAvail );
                
                if ( iShowWlanAvail )
                    {
                    iModel->CheckPsmModeL();
                    // check that the user selected value is allowed
                    // if we are in the PowerSave Mode       
                    if ( iModel->PsmModeOn() )
                        {
                        if ( iModel->ScanNetworks() != KWlanSettingsUiScanNetworkAuto )
                            {
                            iScanMode = EWlanSettingsUiScanNetworkAuto;
                            iScanNetworksBackUp = iModel->ScanNetworks();
                            iModel->SetScanNetworks( KWlanSettingsUiScanNetworkAuto );  
                            iChangeScanNetworksValueBack = ETrue;
                            CAknSettingItemList::LoadSettingsL();
                            }                   
                        }
                    else
                        {
                        if ( iChangeScanNetworksValueBack )
                            {
                            iScanMode = EWlanSettingsUiScanNetworkUserDef;
                            iModel->SetScanNetworks( iScanNetworksBackUp );  
                            iChangeScanNetworksValueBack = EFalse;
                            CAknSettingItemList::LoadSettingsL();
                            }
                        }
                    }
                
                break;
                }

            case EWlanSettingsUiSettingPowerSaving:
                {
                // Always shown.
                break;
                }
                
            default:
                {
                __ASSERT_DEBUG( EFalse, \
                    WlanSettingsUiPanic( EWlanSettingsUiBadSetting ) );
                break;
                }
            }
        }
    HandleChangeInItemArrayOrVisibilityL();
    }
    
// ---------------------------------------------------------
// CWlanSettingsUiSettingItemList::SetListBoxRect
// ---------------------------------------------------------
//    
void CWlanSettingsUiSettingItemList::SetListBoxRect()
{
   TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect(
            AknLayoutUtils::EMainPane, mainPaneRect );        
        
        TAknLayoutRect layoutRect;
        layoutRect.LayoutRect(
            TRect( TPoint( 0, 0 ), mainPaneRect.Size() ),
            AKN_LAYOUT_WINDOW_list_gen_pane( 0 ) );
                
        ListBox()->SetRect( layoutRect.Rect() ); 
}

// ---------------------------------------------------------
// CWlanSettingsUiSettingItemList::HandleResourceChange
// ---------------------------------------------------------
//
void CWlanSettingsUiSettingItemList::HandleResourceChange( TInt aType ) 
    {
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        SetListBoxRect();
        }
        
    // Base call
    CAknSettingItemList::HandleResourceChange( aType );
    }

// ---------------------------------------------------------
// CWlanSettingsUiSettingItemList::ActivateL
// ---------------------------------------------------------
//
void CWlanSettingsUiSettingItemList::ActivateL()
    {
    LoadSettingsL();
    SetListBoxRect();
    CAknSettingItemList::ActivateL();
    }    
