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
*      Inline method definitions CWlanSettingsUiModel.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_MODEL_INL
#define WLAN_SETTINGS_UI_MODEL_INL

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanSettingsUiModel::ShowWlanAvail
// ---------------------------------------------------------
//
TBool CWlanSettingsUiModel::ShowWlanAvail()
    {
    return iShowWlanAvail;
    }

// ---------------------------------------------------------
// CWlanSettingsUiModel::PowerSaving
// ---------------------------------------------------------
//        
TBool CWlanSettingsUiModel::PowerSaving()
    {
    return iPowerSaving;
    }

// ---------------------------------------------------------
// CWlanSettingsUiModel::SetShowWlanAvail
// ---------------------------------------------------------
//        
void CWlanSettingsUiModel::SetShowWlanAvail( TBool aShowWlanAvail )
    {
    iShowWlanAvail = aShowWlanAvail;
    }
 
// ---------------------------------------------------------
// CWlanSettingsUiModel::SetPowerSaving
// ---------------------------------------------------------
//       
void CWlanSettingsUiModel::SetPowerSaving( TBool aPowerSaving )
    {
    iPowerSaving = aPowerSaving;
    }
    
// ---------------------------------------------------------
// CWlanSettingsUiModel::PsmModeOn
// ---------------------------------------------------------
//    
TBool CWlanSettingsUiModel::PsmModeOn()
    {
    return iPsmMode;
    }
    
#endif
