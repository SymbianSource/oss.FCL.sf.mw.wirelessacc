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
*      Declaration of class CWlanSettingsUiStatusPane.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_STATUS_PANE_H
#define WLAN_SETTINGS_UI_STATUS_PANE_H

// INCLUDES

#include <e32base.h>

// FORWARD DECLARATION

class CEikonEnv;
class CEikStatusPane;
class CAknTitlePane;
class CAknNavigationControlContainer;
class CAknNavigationDecorator;

// CLASS DECLARATION

/**
* Status pane setter class. Replaces status pane text on construction,
* restores old text on deletion.
*/
NONSHARABLE_CLASS( CWlanSettingsUiStatusPane ): public CBase
    {
    
    public:     // Constructors and destructor

        /**
        * Two-phased constructor. Leaves on failure.
        * @param aEikEnv Eikon environment.
        * aResourceId TBUF resource id.
        * @return The constructed object.
        */
        static CWlanSettingsUiStatusPane* NewLC
            ( const CEikonEnv* aEikEnv, TInt aResourceId );

        /**
        * Destructor.
        */
        virtual ~CWlanSettingsUiStatusPane();

    protected:  // Constructors

        /**
        * Second-phase constructor.
        * @param aEikEnv Eikon environment.
        * aResourceId TBUF resource id.
        */
        void ConstructL( const CEikonEnv* aEikEnv, TInt aResourceId );

    private:    // Data

        CEikStatusPane* iStatusPane;                ///< Status pane, not own.
        CAknTitlePane* iTitlePane;                  ///< Title pane, not own.
        CAknNavigationControlContainer* iNaviPane;  ///< Navi pane, not own.
        CAknNavigationDecorator* iNaviDecorator;    ///< Navi decorator, own.
        HBufC* iOldTitleText;                       ///< Old title text, own.

    };

#endif
