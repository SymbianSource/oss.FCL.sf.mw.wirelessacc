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
*      Implementation of class CWlanSettingsUiMainViewDlg.   
*      
*
*/


// INCLUDE FILES

#include <AknQueryDialog.h>
#include <eikmenup.h>
#include "wlansettingsuimainviewdlg.h"
#include "wlansettingsui.hrh"
#include "wlansettingsuisettingitemlist.h"

#include "wlansettingsuidefs.h"
#include <csxhelp/cp.hlp.hrh>


//CONSTANTS

// ================= MEMBER FUNCTIONS =======================
        void FocusChanged(TDrawNow aDrawNow); 
// ---------------------------------------------------------
// CWlanSettingsUiMainViewDlg::GetHelpContext
// ---------------------------------------------------------
//
void CWlanSettingsUiMainViewDlg::FocusChanged(TDrawNow /* aDrawNow */)
    {
    TRAP_IGNORE( SettingItemList().RefreshViewL() );
    }

// ---------------------------------------------------------
// CWlanSettingsUiMainViewDlg::GetHelpContext
// ---------------------------------------------------------
//
void CWlanSettingsUiMainViewDlg::GetHelpContext
( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KWlanSettingsUiHelpMajor;
    aContext.iContext = KSET_HLP_WLAN_SETTINGS; 
    }

// ---------------------------------------------------------
// CWlanSettingsUiMainViewDlg::DynInitMenuPaneL
// ---------------------------------------------------------
//
void CWlanSettingsUiMainViewDlg::DynInitMenuPaneL
( TInt aResourceId, CEikMenuPane* aMenuPane )
    {
    CWlanSettingsUiDlg::DynInitMenuPaneL( aResourceId, aMenuPane );
    if ( aResourceId == R_WLAN_SETTINGS_UI_MAIN_VIEW_MENU )
        {
        if ( !IsHelpSupported() )
            {
            aMenuPane->DeleteMenuItem( EAknCmdHelp );
            }
        }
    }

// ---------------------------------------------------------
// CWlanSettingsUiMainViewDlg::ProcessCommandL
// ---------------------------------------------------------
//
void CWlanSettingsUiMainViewDlg::ProcessCommandL( TInt aCommandId )
    {
    if ( MenuShowing() )
        {
        HideMenu();
        }

    switch( aCommandId )
        {
        default:
            {
            CWlanSettingsUiDlg::ProcessCommandL( aCommandId );
            }
        }
    }
