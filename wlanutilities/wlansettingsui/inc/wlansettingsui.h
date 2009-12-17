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
*      Declaration of class CWlanSettingsUi.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_H
#define WLAN_SETTINGS_UI_H

// INCLUDES

#include <e32base.h>
#include <wlancontainer.h>

// FORWARD DECLARATIONS

class CEikonEnv;
class CWlanSettingsUiImpl;

// CLASS DECLARATION

/**
* Public interface of WLAN Settings UI.
* Proxy around the real implementation in CWlanSettingsUiImpl.
*/
NONSHARABLE_CLASS( CWlanSettingsUi ): public CBase
    {

    public:     // Constructors and destructor

        /**
        * Two-phased constructor. Leaves on failure.
        * @param aEikEnv Eikon environment.
        * @return The constructed CWlanSettingsUi object.
        */
        IMPORT_C static CWlanSettingsUi* NewL( CEikonEnv& aEikEnv );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CWlanSettingsUi();

    public:     // New methods

        /**
        * Launch WLAN Settings view.
        * @param aSession Commsdat session.
        */
        IMPORT_C void RunSettingsL( CMDBSession* aSession );

        /**
        * Launch WLAN Settings view.
        * with own session.
        */       
        IMPORT_C void RunSettingsL();

    private:    // Data 

        CWlanSettingsUiImpl* iImpl; ///< Implementation. Owned.

    };

#endif 
