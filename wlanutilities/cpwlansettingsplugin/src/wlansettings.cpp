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

// System includes

#include <xqsettingsmanager.h>
#include <xqsettingskey.h>
#include <psmsrvdomaincrkeys.h>

// User includes

#include "wlansettings.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlansettingsTraces.h"
#endif

/*!
   \class WlanSettings
   \brief WlanSettings is a wrapper class for CWlanSettingsImpl.
*/

// External function prototypes

// Local constants
/**  Default value for Scan Interval. */
const int KDefaultScanInterval = 5;

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
*/

WlanSettings::WlanSettings() :
    QObject(), mPsmKeyValue(0)
{

    OstTraceFunctionEntry1(WLANSETTINGS_WLANSETTINGS_ENTRY, this);
    OstTraceFunctionExit1(WLANSETTINGS_WLANSETTINGS_EXIT, this);
}

/*!
    Destructor.
*/

WlanSettings::~WlanSettings()
{
    OstTraceFunctionEntry1(DUP1_WLANSETTINGS_WLANSETTINGS_ENTRY, this);

    delete mImpl;
    
    OstTraceFunctionExit1(DUP1_WLANSETTINGS_WLANSETTINGS_EXIT, this);
}

/*!
    Creates the CWlanSettingsImpl object for reading/writing settings.
    \return Error code.
*/

int WlanSettings::init()
{
    OstTraceFunctionEntry1(WLANSETTINGS_INIT_ENTRY, this);
    
    readPsmKey();
    
    TRAPD(error,(mImpl = CWlanSettingsPrivate::NewL(mPsmKeyValue)));
    
    OstTraceFunctionExit1(WLANSETTINGS_INIT_EXIT, this);
    return error;
}


void WlanSettings::readPsmKey()
{
    OstTraceFunctionEntry1(WLANSETTINGS_READPSMKEY_ENTRY, this);
    
    QScopedPointer<XQSettingsManager> settingsManager;
    settingsManager.reset(new XQSettingsManager());

    XQSettingsKey key(XQSettingsKey::TargetCentralRepository,
            KCRUidPowerSaveMode.iUid, KPsmCurrentMode);

    QVariant keyValue = settingsManager->readItemValue(key);

    mPsmKeyValue = keyValue.toInt();
    
    OstTraceFunctionExit1(WLANSETTINGS_READPSMKEY_EXIT, this);
}

/*!
    Reads the WLAN setting values.
    \return Error code.
*/

int WlanSettings::loadSettings()
{
    OstTraceFunctionEntry1(WLANSETTINGS_LOADSETTINGS_ENTRY, this);
    
    TRAPD(error, mImpl->LoadDBSettingsL());
    
    OstTraceFunctionExit1(WLANSETTINGS_LOADSETTINGS_EXIT, this);
    return error;
}

/*!
    Function to get the scan network type.
    \return ScanNetworkType which is automatic or user-defined.
*/

WlanSettings::ScanNetworkType WlanSettings::scanNetworkType()
{
    OstTraceFunctionEntry1(WLANSETTINGS_SCANNETWORKTYPE_ENTRY, this);
    
    ScanNetworkType scanType;
    uint scanInterval = mImpl->ScanInterval();
    
    if (scanInterval == KWlanSettingsScanNetworkAuto) {
        scanType = EScanNetworkAuto;
    }
    else {
        scanType = EScanNetworkUserDefined;
    }
    
    OstTraceFunctionExit1(WLANSETTINGS_SCANNETWORKTYPE_EXIT, this);
    return scanType;
}

/*!
    Function to get the scan interval.
    \return Scan interval  in minutes.
*/

uint WlanSettings::scanInterval()
{
    OstTraceFunctionEntry1(WLANSETTINGS_SCANINTERVAL_ENTRY, this);
    
    uint scanInterval = mImpl->ScanInterval();
    
    if (scanInterval == KWlanSettingsScanNetworkAuto) {
        scanInterval = KDefaultScanInterval;
    }
    
    OstTraceFunctionExit1(WLANSETTINGS_SCANINTERVAL_EXIT, this);
    return scanInterval;
}

/*!
    Function to get Power Saving Option.
    \return True if Power Saving option is enabled, otherwise False.
*/

int WlanSettings::isPowerSavingEnabled() const
{
    return mImpl->PowerSaving();
}

/*!
    Function to set Power Saving Option.
    \param powerSavingOption True to enable or false to disable power saving option. 
    \return Error code.
*/

int WlanSettings::setWlanPowerSaving(int powerSavingOption)
{
    OstTraceFunctionEntry1(WLANSETTINGS_SETWLANPOWERSAVING_ENTRY, this);
    
    mImpl->SetPowerSaving(powerSavingOption);
    TRAPD(error, mImpl->SaveDBSettingsL(CWlanSettingsPrivate::EWlanPowerSaving));

    OstTraceFunctionExit1(WLANSETTINGS_SETWLANPOWERSAVING_EXIT, this);
    return error;
}

/*!
    Function to set Scan interval.
    \param scanInterval Scan interval in minutes. 
    \return Error code.
*/

int WlanSettings::setWlanScanInterval(uint scanInterval)
{
    OstTraceFunctionEntry1(WLANSETTINGS_SETWLANSCANINTERVAL_ENTRY, this);
    
    mImpl->SetScanInterval(scanInterval);
    TRAPD(error, mImpl->SaveDBSettingsL(CWlanSettingsPrivate::EWlanScanInterval));

    OstTraceFunctionExit1(WLANSETTINGS_SETWLANSCANINTERVAL_EXIT, this);
    return error;
}

/*!
    Function to get Join WLAN networks option.
    \return 0 for Known, 1 for Manual.
*/

int WlanSettings::joinWlanMode() const
{
    return mImpl->JoinWlanMode();
}

/*!
    Function to set Join WLAN networks option.
    \param mode 0 for Known, 1 for Manual. 
    \return Error code.
*/

int WlanSettings::setJoinWlanMode(int mode)
{
    OstTraceFunctionEntry1(WLANSETTINGS_SETJOINWLANMODE_ENTRY, this);
    
    TRAPD(error, mImpl->SaveJoinWlanSettingL(mode));
    
    OstTraceFunctionExit1(WLANSETTINGS_SETJOINWLANMODE_EXIT, this);
    return error;
}

/*!
    Function to get Power Saving status of the device.
    \return True if Power Saving is enabled.
*/

int WlanSettings::isPsmEnabled() const
{
    return mImpl->IsPsmEnabled();
}
