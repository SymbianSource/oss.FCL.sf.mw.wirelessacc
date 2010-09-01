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
*      Implementation of class CWlanSettingsUi.   
*      
*
*/


// INCLUDE FILES

#include "wlansettingsui.h"
#include "wlansettingsuiimpl.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanSettingsUi::NewLC
// ---------------------------------------------------------
//
EXPORT_C CWlanSettingsUi* CWlanSettingsUi::NewL( CEikonEnv& aEikEnv )
    {
    CWlanSettingsUi* ui = new ( ELeave ) CWlanSettingsUi();
    CleanupStack::PushL( ui );
    ui->iImpl = CWlanSettingsUiImpl::NewL( aEikEnv );
    CleanupStack::Pop( ui ); 
    return ui;    
    }

// ---------------------------------------------------------
// CWlanSettingsUi::~CWlanSettingsUi
// ---------------------------------------------------------
//
EXPORT_C CWlanSettingsUi::~CWlanSettingsUi()
    {
    delete iImpl;
    }

// ---------------------------------------------------------
// CWlanSettingsUi::RunSettingsL
// ---------------------------------------------------------
//
EXPORT_C void CWlanSettingsUi::RunSettingsL( CMDBSession* aSession )
    {
    iImpl->RunSettingsL( aSession );
    }

// ---------------------------------------------------------
// CWlanSettingsUi::RunSettingsL
// ---------------------------------------------------------
//  
EXPORT_C void CWlanSettingsUi::RunSettingsL()
    {
    CommsDat::CMDBSession* session = CMDBSession::NewLC( KCDLatestVersion );
    RunSettingsL( session );
    CleanupStack::PopAndDestroy( session );
    }
