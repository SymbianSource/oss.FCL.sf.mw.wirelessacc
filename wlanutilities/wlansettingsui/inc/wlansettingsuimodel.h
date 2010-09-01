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
*      Declaration of class CWlanSettingsUiModel.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_MODEL_H
#define WLAN_SETTINGS_UI_MODEL_H

// INCLUDES

#include <e32base.h>
#include "wlansettingsui.hrh"
#include <wlancontainer.h>


// CONSTANTS

/**
 * Default "Show WLAN Availability" setting.
 */
LOCAL_D const TBool KWlanSettingsUiDefaultShowWlanAvail = EFalse;

/**
 * Value for scanning setting off.
 */
LOCAL_D const TUint KWlanSettingsUiScanNetworkNever = 0;

/**
 * Value for automatic scanning.
 */
LOCAL_D const TUint KWlanSettingsUiScanNetworkAuto = 0xFFFFFFFF;

/**
 * Default user-defined scan interval (5 minutes).
 */
LOCAL_D const TUint KWlanSettingsUiDefaultUserDefScanInterval = 5;

/**
 * Default "Scan Networks" setting.
 */
LOCAL_D const TUint KWlanSettingsUiDefaultScanNetwork =
    KWlanSettingsUiScanNetworkAuto;

/**
 * Default "Power saving" setting.
 */
LOCAL_D const TBool KWlanSettingsUiDefaultPowerSaving = ETrue;

// FORWARD DECLARATIONS

class CWlanMgmtClient;
class CRepository;

// CLASS DECLARATION

/**
* Data model for WLAN Settings UI.
*/
NONSHARABLE_CLASS( CWlanSettingsUiModel ) : public CBase
    {
    
    public:     // Constructors and destructor
                
        /**
        * Destructor
        */
        ~CWlanSettingsUiModel();

    public:     // Types

        enum TSettingType       ///< Setting type.
            {
            EDefaultSettings,   ///< Default settings.
            EUserSettings       ///< User settings.
            };

    public:     // New methods
        
        
        /**
        * First phase of two phase construction
        * @param -
        * @return returns an instance
        */
        static CWlanSettingsUiModel* NewL
            (
            CMDBSession* aSession,
            CWlanMgmtClient* aWlanMgmtClient,
            CRepository* aRepository
            );
        
        /**
        * Load settings from comms database.
        */
        void LoadSettingsL();

        /**
        * Save settings to comms database.
        */
        void SaveSettingsL();
        
        /**
        * Checks from CenRep if the PowerSave Mode is on (full or partial)
        */
        void CheckPsmModeL();
        
        
    public:
        //getters and setters
        
        /**
        * Gets if Wlan availablity is shown.
        * @return Returns wlan availablity.
        */
        inline TBool ShowWlanAvail();
        
        /**
        * Gets scan networks mode. Returns 0 for no scan, 0xFFFFFFFF for
        * automatic scan or user-defined scan time in minutes.
        * @return Scan networks mode.
        */
        TUint ScanNetworks();

        /**
        * Gets if power saving mode is enabled.
        * @return Returns if power saving mode is enabled.
        */
        inline TBool PowerSaving();
        
        /**
        * Sets the value of iShowWlanAvail.
        * @param aShowWlanAvail The set value.
        */
        inline void SetShowWlanAvail( TBool aShowWlanAvail );
        
        /**
        * Sets the value of iScanNetworks. 0 for no scan, 0xFFFFFFFF for
        * automatic scan or user-defined scan time in minutes.
        * @param aScanNetworks The set value.
        */
        void SetScanNetworks( TUint aScanNetworks );

        /**
        * Sets the value of iPowerSaving.
        * @param aPowerSaving The set value. 
        */
        inline void SetPowerSaving( TBool aPowerSaving );
        
        /**
        * Gets if user selectable power save mode is enabled.
        * @return Returns ETrue/EFalse according to if PSM mode is enabled.
        */
        inline TBool PsmModeOn();
        
        
    private:    // New methods
        
        /**
        * Constructor.
        * @param aSession CommsDat session.
        * @param aWlanMgmtClient Wlan management client.
        * @param aRepository Centeral repository.
        */
        CWlanSettingsUiModel
            (
            CMDBSession* aSession, 
            CWlanMgmtClient* aWlanMgmtClient,
            CRepository* aRepository
            );
        
        /**
        * Second phase of two phase construction
        */
        void ConstructL();
         
        /**
        * Fix "Scan networks" value: make sure that the numeric value is
        * valid. Ensures fault tolerant behaviour if commsdb contains
        * invalid value.
        * @param TUint aSavedScanInternal The saved value of scan interval.
        */
        void FixScanNetworks( TUint aSavedScanInternal );
        
        /**
        * Scan interval is stored in seconds in the database but the UI 
        * handles only 1-30 minutes. Method ensures that scan interval is
        * 0, 1-30 minutes or automatic (0xFFFFFFFF).
        * Ensures fault tolerant behaviour if commsdb contains invalid value.
        * @param TUint aScanInterval this value will be converted.
        * @return TUint the converted value.
        */
        TUint FixScanInterval(TUint aScanInterval);

    private:     

        /// Show WLAN availability.
        TBool iShowWlanAvail;
        /// Scan networks interval in seconds. 0 for no scan, 0xFFFFFFFF for automatic. 
        TUint iScanNetworks;
        /// Power saving.
        TBool iPowerSaving;
        /// Full or partial PSM mode.
        TBool iPsmMode;                      
        
        /// Comms database.
        CMDBSession* iSession;

        // WLAN Management client
        CWlanMgmtClient* iWlanMgmtClient; // not owned
        
        /**
        * Central repository member
        */
        CRepository* iRepository;  // not owned
        
        // Power Save Mode Feature
        TBool iPsmSupported;

    };

#include "wlansettingsuimodel.inl"

#endif
