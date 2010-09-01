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
*      Inline method definitions CWlanSettingsUiMainViewDlg.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_MAIN_VIEW_DLG_INL
#define WLAN_SETTINGS_UI_MAIN_VIEW_DLG_INL

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanSettingsUiMainViewDlg::CWlanSettingsUiMainViewDlg
// ---------------------------------------------------------
//
CWlanSettingsUiMainViewDlg::CWlanSettingsUiMainViewDlg
( CWlanSettingsUiModel* aModel )
: CWlanSettingsUiDlg( aModel )
    {
    }

// ---------------------------------------------------------
// CWlanSettingsUiMainViewDlg::MenuBarResourceId
// ---------------------------------------------------------
//
TInt CWlanSettingsUiMainViewDlg::MenuBarResourceId() const
    {
    return R_WLAN_SETTINGS_UI_MAIN_VIEW_MENUBAR;
    }

// ---------------------------------------------------------
// CWlanSettingsUiMainViewDlg::DialogResourceId
// ---------------------------------------------------------
//
TInt CWlanSettingsUiMainViewDlg::DialogResourceId() const
    {
    return R_WLAN_SETTINGS_UI_MAIN_VIEW_DIALOG;
    }

#endif
