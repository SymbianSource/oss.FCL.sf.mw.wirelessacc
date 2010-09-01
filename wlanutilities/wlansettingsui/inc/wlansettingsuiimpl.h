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
*      Declaration of class CWlanSettingsUiImpl.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_IMPL_H
#define WLAN_SETTINGS_UI_IMPL_H

// INCLUDES

#include <e32base.h>
#include <wlancontainer.h>
#include <ConeResLoader.h>


// FORWARD DECLARATIONS

class CEikonEnv;
class CWlanSettingsUiImplImpl;
class CWlanMgmtClient;
class CRepository;

// CLASS DECLARATION

/**
* WLAN Settings UI implementation (behind proxy class CWlanSettingsUi):
* entry points for WLAN Settign UI functionality.
*/
NONSHARABLE_CLASS( CWlanSettingsUiImpl ): public CBase
    {

    public:     // Constructors and destructor

        /**
        * Two-phased constructor. Leaves on failure.
        * @param aEikEnv Eikon environment.
        * @return The constructed CWlanSettingsUiImpl object.
        */
        static CWlanSettingsUiImpl* NewL( CEikonEnv& aEikEnv );

        /**
        * Destructor.
        */
        virtual ~CWlanSettingsUiImpl();

    protected:  // Constructors

        /**
        * Constructor.
        * @param aEikEnv Eikon environment.
        */
        CWlanSettingsUiImpl( CEikonEnv& aEikEnv );

        /**
        * Second-phase constructor.
        */
        void ConstructL();

    public:     // New methods

        /**
        * Launch General WLAN Settings view.
        * @param aCommsDb Comms database.
        */
        void RunSettingsL( CMDBSession* aSession );

    private:    // Data 

        /*
        * Eikon environment. Not owned.
        */
        CEikonEnv* iEikEnv;     
        
        /*
        * @var resource loader
        */
        RConeResourceLoader iResources;
        
        /*
        * Wlan management
        */
        CWlanMgmtClient* iWlanMgmtClient;
        
        /*
        * Centeral Repository
        */
        CRepository* iRepository;
        
    };

#endif 
