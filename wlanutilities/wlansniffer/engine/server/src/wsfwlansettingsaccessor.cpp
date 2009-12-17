/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CWsfWlanSettingsAccessor
*
*/


//  EXTERNAL INCLUDES
#include <d32dbms.h> 
#include <WlanCdbCols.h>
#include <wlancontainer.h>

#include <centralrepository.h>
#include <wlandevicesettingsinternalcrkeys.h>
#include <featmgr.h>

//  CLASS HEADER
#include "wsfwlansettingsaccessor.h"

// INTERNAL INCLUDES
#include "wsfwlanscanintervalchangeobserver.h"
#include "wsflogger.h"


using namespace CommsDat;

// background scan disabled value
static const TUint KWlanBgScanIntervalNever = 0;

// background scan automatic scanning value
static const TUint KWlanBgScanIntervalAuto = 0xffffffff;

// default background scan interval in seconds
static const TUint KWlanBgScanIntervalDefault = 300;

// hold-up time after first DB notification to prevent bursts (in microseconds)
static const TUint KDbNotificationHoldupTime = 1000 * 1000;



// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::NewL
// ----------------------------------------------------------------------------
//
CWsfWlanSettingsAccessor* CWsfWlanSettingsAccessor::NewL( 
                                                     CMDBSession& aDbSession )
    {
    CWsfWlanSettingsAccessor *thisPtr = NewLC( aDbSession );
    CleanupStack::Pop( thisPtr );
    return thisPtr;
    }


// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::NewLC
// ----------------------------------------------------------------------------
//
CWsfWlanSettingsAccessor* CWsfWlanSettingsAccessor::NewLC( 
                                                     CMDBSession& aDbSession )
    {
    CWsfWlanSettingsAccessor *thisPtr = 
                          new (ELeave) CWsfWlanSettingsAccessor( aDbSession );
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    return thisPtr; 
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::CWsfWlanSettingsAccessor
// ----------------------------------------------------------------------------
//
CWsfWlanSettingsAccessor::CWsfWlanSettingsAccessor( CMDBSession& aDbSession ): 
    CActive( CActive::EPriorityStandard ), 
    iDbSession( &aDbSession ),
    iBeingHeldUp( EFalse )
    {
    }

    
// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfWlanSettingsAccessor::ConstructL()
    {
    CActiveScheduler::Add( this );
    
    // get WLAN table id
    TRAP_IGNORE( iTableId = CCDWlanDeviceSettingsRecord::TableIdL( 
                                                              *iDbSession ) );

    if ( !iTableId )
        {
        iTableId = CCDWlanDeviceSettingsRecord::CreateTableL( *iDbSession );
        }

    iWlanSettingsRecord = new (ELeave) CCDWlanDeviceSettingsRecord( iTableId );
    iWlanSettingsRecord->iWlanDeviceSettingsType = KWlanUserSettings;
    
    if ( !iWlanSettingsRecord->FindL( *iDbSession ) )
        {
        User::Leave( KErrNotFound );
        }
    
    User::LeaveIfError( iTimer.CreateLocal() );
    
    DoCheckSettingL( iScanInterval, iShowAvailability );
    }


// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::~CWsfWlanSettingsAccessor
// ----------------------------------------------------------------------------
//
CWsfWlanSettingsAccessor::~CWsfWlanSettingsAccessor()
    {
    Cancel();
    iTimer.Close();
    iDbSession = NULL; // not owning
    iChangeObserver = NULL; // not owning 
    delete iWlanSettingsRecord; // own
    iWlanSettingsRecord = NULL;
    }


// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::ScanInterval
// ----------------------------------------------------------------------------
//
TUint CWsfWlanSettingsAccessor::ScanInterval() const
    {
    LOG_ENTERFN( "CWsfWlanSettingsAccessor::ScanInterval" );
    return iScanInterval;
    }


// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::ShowAvailability
// ----------------------------------------------------------------------------
//
TBool CWsfWlanSettingsAccessor::ShowAvailability() const
    {
    LOG_ENTERFN( "CWsfWlanSettingsAccessor::ShowAvailability" );
    return iShowAvailability;
    }


// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::RequestNotificationL
// ----------------------------------------------------------------------------
//
void CWsfWlanSettingsAccessor::RequestNotificationL( 
                                MWsfWlanScanIntervalChangeObserver& aObserver )
    {
    LOG_ENTERFN( "CWsfWlanSettingsAccessor::RequestNotificationL" );
    iChangeObserver = &aObserver;
    IssueNotificationRequestL();
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::IssueNotificationRequestL
// ----------------------------------------------------------------------------
//
void CWsfWlanSettingsAccessor::IssueNotificationRequestL()
    {
    LOG_ENTERFN( "CWsfWlanSettingsAccessor::IssueNotificationRequestL" );
    if ( iChangeObserver )
        {
        if ( !IsActive() )
            {
            LOG_WRITE( "issuing..." );
            User::LeaveIfError( iWlanSettingsRecord->RequestNotification( 
                                                     *iDbSession, iStatus ) );
            SetActive();    
            }
        }
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::CancelNotifications
// ----------------------------------------------------------------------------
//
void CWsfWlanSettingsAccessor::CancelNotifications()
    {
    LOG_ENTERFN( "CWsfWlanSettingsAccessor::CancelNotifications" );
    Cancel();
    iChangeObserver = NULL;
    }


// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::DoCheckSettingL
// ----------------------------------------------------------------------------
//
void CWsfWlanSettingsAccessor::DoCheckSettingL( TUint& aBgScanInterval, 
                                                TBool& aShowAvailability )
    {
    LOG_ENTERFN( "CWsfWlanSettingsAccessor::DoCheckSettingL" );
    // open the wlan settings table
    
    iWlanSettingsRecord->RefreshL( *iDbSession );
    FeatureManager::InitializeLibL();
    
    aShowAvailability = ( iWlanSettingsRecord->iBgScanInterval != 
                                                    KWlanBgScanIntervalNever );

    // read the common value
    if ( iWlanSettingsRecord->iBgScanInterval == KWlanBgScanIntervalNever )
        {
        if ( iWlanSettingsRecord->iSavedBgScanInterval == 
                                                     KWlanBgScanIntervalNever )
            {
            TInt defaultScanInterval( KWlanBgScanIntervalDefault );

							if (FeatureManager::FeatureSupported( KFeatureIdPowerSave ))
							{
         	   // Read the default value from CenRep (different in PSM mode)
          	  CRepository* cenrep = CRepository::NewL( 
                                      KCRUidWlanDeviceSettingsRegistryId );
          	  cenrep->Get( KWlanDefaultBGScanInterval, defaultScanInterval );
         		  delete cenrep;
         			 } 

            aBgScanInterval = TUint( defaultScanInterval );
            }
        else
            {
            aBgScanInterval = iWlanSettingsRecord->iSavedBgScanInterval;
            }
        }   
    else
        {
        aBgScanInterval = iWlanSettingsRecord->iBgScanInterval;
        }
    
    // Set scan interval to default value if adaptive scanning value is
    // found from db
    if (aBgScanInterval == KWlanBgScanIntervalAuto )
        {
        TInt defaultScanInterval( KWlanBgScanIntervalDefault );
        aBgScanInterval = TUint( defaultScanInterval );
        }
    
  	FeatureManager::UnInitializeLib();  
    LOG_WRITEF( "current bgScanInterval = %d sec", aBgScanInterval );
    LOG_WRITEF( "current showAvailability = %d", aShowAvailability );
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::CheckIfSettingChangedL
// ----------------------------------------------------------------------------
//
TBool CWsfWlanSettingsAccessor::CheckIfSettingChangedL()
    {
    LOG_ENTERFN( "CWsfWlanSettingsAccessor::CheckIfSettingChangedL" );
    LOG_WRITEF( "previous bgScanInterval = %d", iScanInterval );
    LOG_WRITEF( "previous showAvailability = %d", iShowAvailability );
    TUint newBgsi( 0 );
    TBool newSaf( EFalse );
    
    DoCheckSettingL( newBgsi, newSaf );
    
    TBool retval( newBgsi != iScanInterval || newSaf != iShowAvailability );
    iScanInterval = newBgsi;
    iShowAvailability = newSaf;
    
    return retval;    
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::RunL
// ----------------------------------------------------------------------------
//
void CWsfWlanSettingsAccessor::RunL()
    {
    LOG_ENTERFN( "CWsfWlanSettingsAccessor::RunL" );
    LOG_WRITEF( "iStatus.Int() = %d", iStatus.Int() );

    // Symbian DB notifiers are triggered by everything that may happen in 
    // commsdat, so it would be very resource-consuming to check the value on 
    // each database event
    // Workaround: 1-sec delay when first callback is received

    if ( !iBeingHeldUp )
        {
        LOG_WRITE( "starting anti-burst delay" );
        iBeingHeldUp = ETrue;
        iTimer.After( iStatus, TTimeIntervalMicroSeconds32( 
                                                 KDbNotificationHoldupTime ) );
        SetActive();
        }
    else
        {
        LOG_WRITE( "checking changes" );
        iBeingHeldUp = EFalse;
        if ( CheckIfSettingChangedL() )
            {
            LOG_WRITE( "setting changed, notifying observer" );
            iChangeObserver->WlanScanIntervalChangedL( iScanInterval, 
                                                       iShowAvailability );
            }
            
        IssueNotificationRequestL();
        }

    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::DoCancel
// ----------------------------------------------------------------------------
//
void CWsfWlanSettingsAccessor::DoCancel()
    {
    iTimer.Cancel();
    iBeingHeldUp = EFalse;
    
    if ( iWlanSettingsRecord )
        {
        iWlanSettingsRecord->CancelNotification( *iDbSession, iStatus );
        }
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSettingsAccessor::RunError
// ----------------------------------------------------------------------------
//
TInt CWsfWlanSettingsAccessor::RunError( TInt /*aError*/ )
    {
    TRAP_IGNORE( IssueNotificationRequestL() );
    return KErrNone;
    }

