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

#include <internetconnectivitycrkeys.h>
#include <psmtypes.h>
#include <cmmanager.h>

// User includes

#include "wlansettings_s60_p.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlansettings_s60Traces.h"
#endif

/*!
    \class CWlanSettingsPrivate
    \brief CWlanSettingsPrivate is the actual implementation for 
    reading/writing WLAN Settings from/to database.
*/

// External function prototypes

// Local constants
/**  Seconds per minute. */
const TInt KWlanSettingsSecondsInMinute = 60;
/**  Maximum value for scan interval in minutes. */
const TInt KWlanSettingsScanNetworkMax = 30;

// ======== MEMBER FUNCTIONS ========

/*!
    Static NewL function for creating CWlanSettingsPrivate object.
    \return Pointer to CWlanSettingsPrivate object.
*/

CWlanSettingsPrivate* CWlanSettingsPrivate::NewL(int psmKeyValue)
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_NEWL_ENTRY, this);
    
    CWlanSettingsPrivate* impl = new ( ELeave ) CWlanSettingsPrivate(psmKeyValue);
    CleanupStack::PushL( impl );
    impl->ConstructL();
    CleanupStack::Pop( impl ); 
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_NEWL_EXIT, this);
    return impl;    
}

/*!
    Second phase Constructor.
*/

void CWlanSettingsPrivate::ConstructL()
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_CONSTRUCTL_ENTRY, this);
    
    mSession = CMDBSession::NewL( KCDLatestVersion );
    
#ifndef __WINS__
    mWlanMgmtClient = CWlanMgmtClient::NewL();
#endif
    
    CheckPsmModeL();
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_CONSTRUCTL_EXIT, this);
}

/*!
    Constructor.
*/

CWlanSettingsPrivate::CWlanSettingsPrivate(int psmKeyValue) :
    mPsmMode( EFalse ),
    mPowerSaving(KWlanSettingsDefaultPowerSaving),
    mScanInterval(KWlanSettingsDefaultScanNetwork),
    mPsmKeyMode(psmKeyValue)
{
    
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_CWLANSETINGPRIVATE_ENTRY, this);
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_CWLANSETINGPRIVATE_EXIT, this);
    
}

/*!
    Destructor.
*/

CWlanSettingsPrivate::~CWlanSettingsPrivate()
{
    OstTraceFunctionEntry1(DUP1_CWLANSETINGPRIVATE_CWLANSETINGPRIVATE_ENTRY, this);
    
    delete mSession;
#ifndef __WINS__
    delete mWlanMgmtClient;
#endif
    
    OstTraceFunctionExit1(DUP1_CWLANSETINGPRIVATE_CWLANSETINGPRIVATE_EXIT, this);
}

/*!
    Function to get the Power Saving mode of the device.
*/

void CWlanSettingsPrivate::CheckPsmModeL()
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_CHECKPSMMODEL_ENTRY, this);
    
    TPsmsrvMode mode( EPsmsrvModeNormal );
    
    mode = static_cast<TPsmsrvMode>( mPsmKeyMode );
    if ( mode == EPsmsrvModePowerSave || mode == EPsmsrvPartialMode ) {
        mPsmMode = ETrue;
    }
    else {
        mPsmMode = EFalse;
    }
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_CHECKPSMMODEL_EXIT, this);
}

/*!
    Function to load the WLAN Settings from CommsDB.
*/

void CWlanSettingsPrivate::LoadDBSettingsL()
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_LOADDBSETTINGSL_ENTRY, this);
    
    TBool ownTransaction( EFalse );
    if ( !mSession->IsInTransaction() ) {
        ownTransaction = ETrue;
        mSession->OpenTransactionL();
    }
    TMDBElementId tableId = 0;
    
    tableId = CCDWlanDeviceSettingsRecord::TableIdL( *mSession );
        
    CCDWlanDeviceSettingsRecord* record = new( ELeave )
            CCDWlanDeviceSettingsRecord( tableId );         
    CleanupStack::PushL( record );
    
    record->iWlanDeviceSettingsType = KWlanUserSettings; 
    
    if ( record->FindL( *mSession ) ) {
        record->LoadL( *mSession );
    }

    // Read scan interval

    mScanInterval = record->iBgScanInterval;
    //In Case of PSM mode Scan Interval should be set to Auto
    if (mPsmMode) {
        mScanInterval = KWlanSettingsScanNetworkAuto;
    }
    else {
        mScanInterval = record->iSavedBgScanInterval;
        //Safe check for scan interval, control should not come here.
        if (mScanInterval > (KWlanSettingsScanNetworkMax
                * KWlanSettingsSecondsInMinute)) {
            mScanInterval = KWlanSettingsScanNetworkAuto;
        }
    }
    
    mPowerSaving = record->iWlanPowerMode;
    CleanupStack::PopAndDestroy( record ); 
    
    if ( ownTransaction ) {
        mSession->CommitTransactionL();
          // Rollback operation.
    }
    
    //To Load CM Settings.
    LoadJoinWlanSettingL();
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_LOADDBSETTINGSL_EXIT, this);
}

/*!
    Function to load the Join WLAN networks Setting from CMManager.
*/

void CWlanSettingsPrivate::LoadJoinWlanSettingL()
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_LOADJOINWLANSETTINGL_ENTRY, this);
    
    RCmManager CmManager;
    CmManager.CreateTablesAndOpenL();
    CleanupClosePushL(CmManager); 
    CmManager.ReadGenConnSettingsL( mCmSettings );
    CleanupStack::PopAndDestroy( 1 );     //CmManager
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_LOADJOINWLANSETTINGL_EXIT, this);
}

/*!
    Function to write the Join WLAN networks Setting using CMManager.
*/

void CWlanSettingsPrivate::SaveJoinWlanSettingL(TInt mode)
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_SAVEJOINWLANSETTINGL_ENTRY, this);
    
    switch ( mode ) {        
        case EJoinWlanKnown:
            mCmSettings.iUsageOfWlan = ECmUsageOfWlanKnown;
            break;
            
        case EJoinWlanManual:
            mCmSettings.iUsageOfWlan = ECmUsageOfWlanManual;
            break;
                        
        default:
            break;
    }
    
    RCmManager CmManager;
    CmManager.CreateTablesAndOpenL();
    CleanupClosePushL(CmManager); 
    CmManager.WriteGenConnSettingsL( mCmSettings );
    CleanupStack::PopAndDestroy( 1 );
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_SAVEJOINWLANSETTINGL_EXIT, this);
}

/*!
    Function to get the Join WLAN network setting.
    \return 0 for Known, 1 for Manual.
*/

TInt CWlanSettingsPrivate::JoinWlanMode()
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_JOINWLANMODE_ENTRY, this);
    
    TInt mode = 0;
    
    switch ( mCmSettings.iUsageOfWlan ) {
        case ECmUsageOfWlanKnown: 
            mode = EJoinWlanKnown;
            break;
            
        case ECmUsageOfWlanManual:
            mode = EJoinWlanManual;
            break;
                        
        default:
            break;
    }
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_JOINWLANMODE_EXIT, this);
    return mode;
}

/*!
    Function to write the WLAN Settings to CommsDb.
*/

void CWlanSettingsPrivate::SaveDBSettingsL(TInt option)
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_SAVEDBSETTINGSL_ENTRY, this);
    
    TBool ownTransaction( EFalse );
    if ( !mSession->IsInTransaction() ) {
        ownTransaction = ETrue;
        mSession->OpenTransactionL();
    }
    TMDBElementId tableId = 0;
    tableId = CCDWlanDeviceSettingsRecord::TableIdL( *mSession );
        
    CCDWlanDeviceSettingsRecord* record = new( ELeave )
            CCDWlanDeviceSettingsRecord( tableId );         
    
    CleanupStack::PushL( record );
    
    record->iWlanDeviceSettingsType = KWlanUserSettings; 
    
    if ( record->FindL( *mSession ) ) {
        record->LoadL( *mSession );
    }
    
    switch (option) {
        case EWlanScanInterval:
            record->iBgScanInterval = mScanInterval;
            if ( !mPsmMode ) {
                record->iSavedBgScanInterval = mScanInterval;
            }
            break;
        case EWlanPowerSaving:
            record->iWlanPowerMode = mPowerSaving;
            break;
        default:
            break;
    }
    
    // Whenever settings are modified, iUseDefaultSettings must be set to false.
    record->iUseDefaultSettings = EFalse;

    record->ModifyL( *mSession );
        
    CleanupStack::PopAndDestroy( record );        
    if ( ownTransaction ) {
        mSession->CommitTransactionL();
    }

#ifndef __WINS__
    // Notifying WLAN Engine about changes in settings
    mWlanMgmtClient->NotifyChangedSettings();
#endif
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_SAVEDBSETTINGSL_EXIT, this);
}

/*!
    Function to get the scan interval.
    \return Scan interval  in minutes.
*/

TUint CWlanSettingsPrivate::ScanInterval()
{
    if ( mScanInterval == KWlanSettingsScanNetworkAuto ) {
        return mScanInterval;   
    }
    else {
        // Return scan time in minutes
        return ( mScanInterval / KWlanSettingsSecondsInMinute );
    }
}

/*!
    Function to get Power Saving Option.
    \return True if Power Saving option is enabled, otherwise False.
*/

TBool CWlanSettingsPrivate::PowerSaving()
{
    return mPowerSaving;
}

/*!
    Function to set Scan interval.
    \param interval Scan interval in minutes. 
*/

void CWlanSettingsPrivate::SetScanInterval(TUint interval)
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_SETSCANINTERVAL_ENTRY, this);
    
    if ( interval == KWlanSettingsScanNetworkAuto ) {
        mScanInterval = interval;
    }
    else {
        // Scan time stored in seconds
        mScanInterval = interval * KWlanSettingsSecondsInMinute;
    }
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_SETSCANINTERVAL_EXIT, this);
}

/*!
    Function to set Power Saving Option.
    \param powerSavingOption True to enable or false to disable power saving option. 
*/

void CWlanSettingsPrivate::SetPowerSaving(TBool powerSavingOption)
{
    OstTraceFunctionEntry1(CWLANSETINGPRIVATE_SETPOWERSAVING_ENTRY, this);
    
    mPowerSaving = powerSavingOption;
    
    OstTraceFunctionExit1(CWLANSETINGPRIVATE_SETPOWERSAVING_EXIT, this);
}

/*!
    Function to get Power Saving status of the device.
    \return True if Power Saving is enabled.
*/

TBool CWlanSettingsPrivate::IsPsmEnabled()
{
    return mPsmMode;
}
