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
*      Implementation of class CWlanSettingsUiStatusPane.   
*      
*
*/


// INCLUDE FILES

#include <eikenv.h>
#include <eikspane.h>
#include <akntitle.h>
#include <aknnavi.h>
#include <aknnavide.h>
#include <avkon.hrh>
#include <StringLoader.h>

#include "wlansettingsuistatuspane.h"
#include <wlansettingsui.rsg>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanSettingsUi::NewLC
// ---------------------------------------------------------
//
CWlanSettingsUiStatusPane* CWlanSettingsUiStatusPane::NewLC
( const CEikonEnv* aEikEnv, TInt aResourceId )
    {
    CWlanSettingsUiStatusPane* sp = new (ELeave) CWlanSettingsUiStatusPane();
    CleanupStack::PushL( sp );
    sp->ConstructL( aEikEnv, aResourceId );
    return sp;    
    }

// ---------------------------------------------------------
// CWlanSettingsUiStatusPane::~CWlanSettingsUiStatusPane
// ---------------------------------------------------------
//
CWlanSettingsUiStatusPane::~CWlanSettingsUiStatusPane()
    {
    delete iNaviDecorator;
    if ( iOldTitleText )
        {
        if ( iTitlePane )
            {
            TRAP_IGNORE( iTitlePane->SetTextL( *iOldTitleText ) )
            }
        delete iOldTitleText;
        }
    }

// ---------------------------------------------------------
// CWlanSettingsUiStatusPane::ConstructL
// ---------------------------------------------------------
//
void CWlanSettingsUiStatusPane::ConstructL
( const CEikonEnv* aEikEnv, TInt aResourceId )
    {
    iStatusPane = aEikEnv->AppUiFactory()->StatusPane();
    iTitlePane = (CAknTitlePane*)iStatusPane->ControlL
        ( TUid::Uid( EEikStatusPaneUidTitle ) );
    iOldTitleText = iTitlePane->Text()->AllocL();
    
    HBufC* newTitle = StringLoader::LoadLC( aResourceId );
    
    iTitlePane->SetTextL( *newTitle );
    CleanupStack::PopAndDestroy( newTitle );
    iNaviPane = (CAknNavigationControlContainer*)iStatusPane->ControlL
        ( TUid::Uid( EEikStatusPaneUidNavi ) );
    iNaviDecorator = iNaviPane->CreateNavigationLabelL( KNullDesC );
    iNaviPane->PushL( *iNaviDecorator );
    }
