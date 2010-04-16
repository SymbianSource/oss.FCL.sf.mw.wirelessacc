/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/


#ifndef WLANSETTINGS_S60_H_
#define WLANSETTINGS_S60_H_

// System includes

#include <e32base.h>
#include <wlancontainer.h>
#include <centralrepository.h>
#include <wlanmgmtclient.h>
#include <cmgenconnsettings.h>

// User includes

// Forward declarations

// External data types

// Constants
/**  Value for automatic scanning. */
LOCAL_D const TUint KWlanSettingsScanNetworkAuto = 0xFFFFFFFF;

/**  Default "Scan Networks" setting. */
LOCAL_D const TUint KWlanSettingsDefaultScanNetwork =
    KWlanSettingsScanNetworkAuto;

/**  Default "Power saving" setting. */
LOCAL_D const TBool KWlanSettingsDefaultPowerSaving = ETrue;

// Class declaration

class CWlanSettingsPrivate : public CBase
{
public:
    
    enum JoinWlanType {
        EJoinWlanKnown = 0,
        EJoinWlanManual        
    };
    
    enum WlanOption {
        EWlanPowerSaving = 0,
        EWlanScanInterval        
    };
    
    static CWlanSettingsPrivate* NewL(int psmKeyValue);
    
    ~CWlanSettingsPrivate();
    
    void LoadDBSettingsL();
    
    void SaveDBSettingsL(TInt option);
    
    TUint ScanInterval();
    
    TBool PowerSaving();
    
    TInt JoinWlanMode();
    
    void SaveJoinWlanSettingL(TInt mode);
    
    void SetScanInterval(TUint interval);
    
    void SetPowerSaving(TBool powerSavingOption);
    
    TBool IsPsmEnabled();
    
private:
    
    CWlanSettingsPrivate(int psmKeyValue);
    
    void ConstructL();
    
    void CheckPsmModeL();
    
    void LoadJoinWlanSettingL();
    
private:    //data
    
    CommsDat::CMDBSession *mSession;

    //Wlan management
    CWlanMgmtClient *mWlanMgmtClient;

    // Full or partial PSM mode.
    TBool mPsmMode;   

    // Power saving.
    TBool mPowerSaving;

    // Scan networks interval in seconds. 0 for no scan, 0xFFFFFFFF for automatic. 
    TUint mScanInterval;
    
    JoinWlanType mJoinWlanType;
    
    //Struct to hold current settings
    TCmGenConnSettings mCmSettings;
    
    int mPsmKeyMode;

};

#endif /* WLANSETTINGS_S60_H_ */
