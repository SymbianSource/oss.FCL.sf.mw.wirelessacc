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
*      Declaration of class CWlanSettingsUiMainViewDlg.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_MAIN_VIEW_DLG_H
#define WLAN_SETTINGS_UI_MAIN_VIEW_DLG_H

// INCLUDES

#include "wlansettingsuidlg.h"
#include <wlansettingsui.rsg>

// CLASS DECLARATION

/**
* WLAN settings main view dialog.
*/
NONSHARABLE_CLASS( CWlanSettingsUiMainViewDlg ): public CWlanSettingsUiDlg
    {

    public:     // Constructors

        /**
        * Constructor.
        * @param aModel Data model.
        */
        inline CWlanSettingsUiMainViewDlg( CWlanSettingsUiModel* aModel );

    private:    // From CWlanSettingsUiDlg

        /**
        * Get menu bar resource id.
        * @return Menu bar resource id.
        */
        inline TInt MenuBarResourceId() const;

        /**
        * Get dialog resource id.
        * @return Dialog resource id.
        */
        inline TInt DialogResourceId() const;
        
        void FocusChanged(TDrawNow aDrawNow); 
        


    private:    // From CAknDialog & its base classes

        /**
        * Get help context.
        * @param aContext Help context is returned here.
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;

        /**
        * Initialize menu pane.
        * @param aResourceId Menu pane resource id.
        * @param CEikMenuPane Menu pane.
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

        /**
        * Process command.
        * @param aCommandId Command id.
        */
        void ProcessCommandL( TInt aCommandId );

        };

#include "wlansettingsuimainviewdlg.inl"

#endif
