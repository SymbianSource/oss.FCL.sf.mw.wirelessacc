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
*      Implementation of class CWlanSettingsUiImpl.   
*      
*
*/


// INCLUDE FILES

#include <eikenv.h>
#include <bautils.h>
#include <centralrepository.h>
#include <data_caging_path_literals.hrh>
#include <wlanmgmtclient.h>
#include <internetconnectivitycrkeys.h>
#include <f32file.h>
#include <mpmconnectscreenid.h>
#include <wlandevicesettingsinternalcrkeys.h>

#include "wlansettingsuiimpl.h"
#include "wlansettingsuimainviewdlg.h"
#include "wlansettingsuimodel.h"
#include "wlansettingsuistatuspane.h"

// CONSTANTS

/// WLAN Settings UI resource file path.
_LIT( KWlanSettingsUiResourceFile, "z:wlansettingsui.rsc" );

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanSettingsUiImpl::NewLC
// ---------------------------------------------------------
//
CWlanSettingsUiImpl* CWlanSettingsUiImpl::NewL( CEikonEnv& aEikEnv )
    {
    CWlanSettingsUiImpl* ui = new ( ELeave ) CWlanSettingsUiImpl( aEikEnv );
    CleanupStack::PushL( ui );
    ui->ConstructL();
    CleanupStack::Pop( ui ); 
    return ui;    
    }

// ---------------------------------------------------------
// CWlanSettingsUiImpl::~CWlanSettingsUiImpl
// ---------------------------------------------------------
//
CWlanSettingsUiImpl::~CWlanSettingsUiImpl()
    {    
    iResources.Close();
    
#ifndef __WINS__
    delete iWlanMgmtClient;
#endif
    delete iRepository;
    }

// ---------------------------------------------------------
// CWlanSettingsUiImpl::CWlanSettingsUiImpl
// ---------------------------------------------------------
//
CWlanSettingsUiImpl::CWlanSettingsUiImpl( CEikonEnv& aEikEnv )
: iEikEnv( &aEikEnv ), 
  iResources( *CCoeEnv::Static() ) 
    {
    }

// ---------------------------------------------------------
// CWlanSettingsUiImpl::ConstructL
// ---------------------------------------------------------
//
void CWlanSettingsUiImpl::ConstructL()
    {
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );

    // Find the resource file:
    TParse parse;
    parse.Set( KWlanSettingsUiResourceFile, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file:
    BaflUtils::NearestLanguageFile( fsSession, fileName );

    // Open resource file:
    iResources.OpenL( fileName );

    // If leave occurs before this, close is called automatically when the
    // thread exits.
    fsSession.Close();


#ifndef __WINS__
    iWlanMgmtClient = CWlanMgmtClient::NewL();
#endif
    iRepository = CRepository::NewL( KCRUidInternetConnectivitySettings );
    }

// ---------------------------------------------------------
// CWlanSettingsUiImpl::RunSettingsL
// ---------------------------------------------------------
//
void CWlanSettingsUiImpl::RunSettingsL( CMDBSession* aSession )
    {
    CWlanSettingsUiModel* model = CWlanSettingsUiModel::NewL( 
                                aSession, 
                                iWlanMgmtClient,
                                iRepository);
    
    CleanupStack::PushL( model );
    
    model->LoadSettingsL();
    CWlanSettingsUiStatusPane* statusPane =
        CWlanSettingsUiStatusPane::NewLC( iEikEnv, R_WLAN_SETTINGS_UI_TITLE );
    CWlanSettingsUiMainViewDlg* dlg =
        new ( ELeave ) CWlanSettingsUiMainViewDlg( model );
    dlg->ExecuteLD();
    
    CleanupStack::PopAndDestroy( statusPane );
    CleanupStack::PopAndDestroy( model );
    }

