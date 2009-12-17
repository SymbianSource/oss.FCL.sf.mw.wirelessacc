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
*      Implementation of class CWlanSettingsUiDlg.   
*      
*
*/


// INCLUDE FILES

#include "wlansettingsuidlg.h"
#include "wlansettingsuisettingitemlist.h"
#include "wlansettingsuipanic.h"
#include "wlansettingsui.hrh"

#include <aknappui.h>
#include <featmgr.h>

#include <hlplch.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanSettingsUiDlg::ExecuteLD
// ---------------------------------------------------------
//
TInt CWlanSettingsUiDlg::ExecuteLD()
    {
    CleanupStack::PushL( this );
    ConstructL( MenuBarResourceId() );
    CleanupStack::Pop( this );
    return CAknDialog::ExecuteLD( DialogResourceId() );
    }

// ---------------------------------------------------------
// CWlanSettingsUiDlg::CreateCustomControlL
// ---------------------------------------------------------
//
SEikControlInfo CWlanSettingsUiDlg::CreateCustomControlL( TInt aControlType )
    {
    SEikControlInfo controlInfo;
    controlInfo.iControl = NULL;
    controlInfo.iTrailerTextId = 0;
    controlInfo.iFlags = 0;
    switch ( aControlType )
        {
        case EWlanSettingsUiSettingItemList:
            {
            controlInfo.iControl = new (ELeave) CWlanSettingsUiSettingItemList(
                    iModel );
            break;
            }

        default:
            {
            __ASSERT_DEBUG( EFalse, \
                WlanSettingsUiPanic( EWlanSettingsUiBadControl ) );
            break;
            }
        }
    return controlInfo;
    }

// ---------------------------------------------------------
// CWlanSettingsUiDlg::ProcessCommandL
// ---------------------------------------------------------
//
void CWlanSettingsUiDlg::ProcessCommandL( TInt aCommandId )
    {
    if ( MenuShowing() ) 
        {
        HideMenu();
        }

    switch( aCommandId )
        {
        case EWlanSettingsUiCmdChange: 
            {
            SettingItemList().EditCurrentItemL( ETrue );
            break;
            }
            
        case EAknSoftkeyChange:
            {
            SettingItemList().EditCurrentItemL( EFalse );
            break;
            }

        case EAknSoftkeyBack:
            {
            TryExitL( aCommandId );
            break;
            }
            
        case EAknCmdHelp:
            {
            if( IsHelpSupported() )
                {
                HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(),
                    iEikonEnv->EikAppUi()->AppHelpContextL() );
                }
            break;
            }

        case EAknCmdExit:
        case EEikCmdExit:
            {
            ( ( CAknAppUi* )iEikonEnv->EikAppUi() )->
                                           ProcessCommandL( aCommandId );
            break;
            }

        default:
            {
            CAknDialog::ProcessCommandL( aCommandId );
            }
        }
    }
  
// ---------------------------------------------------------
// CWlanSettingsUiDlg::OkToExitL
// ---------------------------------------------------------
//  
TBool CWlanSettingsUiDlg::OkToExitL( TInt aButtonId )
    {
    if ( aButtonId == EAknSoftkeyChange )
        {
        ProcessCommandL( aButtonId );
        return EFalse;
        }
        
    return CAknDialog::OkToExitL( aButtonId );  
    }
    
// ---------------------------------------------------------
// CWlanSettingsUiDlg::SettingItemList
// ---------------------------------------------------------
//
CWlanSettingsUiSettingItemList& CWlanSettingsUiDlg::SettingItemList() const
    {
    return *( STATIC_CAST( CWlanSettingsUiSettingItemList*, \
        Control( EWlanSettingsUiDlgSettingList ) ) );
    }

// ---------------------------------------------------------
// CWlanSettingsUiDlg::IsHelpSupported
// ---------------------------------------------------------
//
TBool CWlanSettingsUiDlg::IsHelpSupported()
    {
    TBool isHelpSupported = EFalse;
    
    TRAPD( error, FeatureManager::InitializeLibL() );
    
    if( !error )
        {
        isHelpSupported = 
            FeatureManager::FeatureSupported( KFeatureIdHelp );
        
        FeatureManager::UnInitializeLib();
        }
    
    return isHelpSupported;
    }  
