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
* Description: 
*      Implementation of utilities functions.   
*      
*
*/



//User includes
#include "wsfdbobserver.h"
#include "wsfaicontroller.h"
#include "wsflogger.h"

#include <featmgr.h>

#include <wlandevicesettingsinternalcrkeys.h> 
#include <centralrepository.h>


const TInt KWlanSettingsUiDefaultScanNetwork = 300;


// ---------------------------------------------------------
// CWsfDbObserver::CWsfDbObserver
// ---------------------------------------------------------
CWsfDbObserver::CWsfDbObserver(  ) 
: CActive( EPriorityNormal ),
iOuterScanState( EFalse )
    {
    }
    

// ---------------------------------------------------------
// CWsfDbObserver::NewL
// ---------------------------------------------------------    
CWsfDbObserver* CWsfDbObserver::NewL(  )
    {
    CWsfDbObserver* self = new ( ELeave ) CWsfDbObserver( );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------
// CWsfDbObserver::ConstructL
// ---------------------------------------------------------   
void CWsfDbObserver::ConstructL()
    {
#ifndef __WINS__ // client is not available on wins
    iWlanMgmtClient = CWlanMgmtClient::NewL();
#endif
    
    FeatureManager::InitializeLibL(); 
         
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------
// CWsfDbObserver::~CWsfDbObserver
// ---------------------------------------------------------       
CWsfDbObserver::~CWsfDbObserver()
    {
    Cancel();
    
    delete iRecord;
            
    delete iSession;
            
    delete iWlanMgmtClient;
    
    FeatureManager::UnInitializeLib();  
    }


// ---------------------------------------------------------
// CWsfDbObserver::RunL
// ---------------------------------------------------------       
void CWsfDbObserver::RunL()
    {
    LOG_ENTERFN( "CWsfDbObserver::RunL" );        
    iRecord->LoadL( *iSession );
    
    //if scanning state changed start or stop the server
    TInt i = iRecord->iBgScanInterval;
    TInt j = iRecord->iSavedBgScanInterval;
    LOG_WRITEF( "CWsfDbObserver::RunL -- Bg:%d bg2:%d O:%d", i, j, 
                                                              iOuterScanState );
    if ( iRecord->iBgScanInterval == 0 && iOuterScanState )
        {
        LOG_WRITE( "CWsfDbObserver::RunL -- Scan disabled" );
        iController->DisableScanL();
        iOuterScanState = EFalse;
        }
    else if(iRecord->iBgScanInterval != 0 && !iOuterScanState )
        {
        LOG_WRITE( "CWsfDbObserver::RunL -- Scan enabled" );
        iController->EnableScanL();
        iOuterScanState = ETrue;
        }
    
    SetActive();
    
    iRecord->RequestNotification(*iSession,iStatus);
    }


// ---------------------------------------------------------
// CWsfDbObserver::DoCancel
// ---------------------------------------------------------       
void CWsfDbObserver::DoCancel()
    {
    iRecord->CancelNotification( *iSession, iStatus );       
    }


// ---------------------------------------------------------
// CWsfDbObserver::ActivateIt
// ---------------------------------------------------------       
void CWsfDbObserver::ActivateItL()
    {
    LOG_ENTERFN( "CWsfDbObserver::ActivateItL" );
    
    iSession = CMDBSession::NewL( KCDLatestVersion );

    TMDBElementId tableId = 0;

    tableId = CCDWlanDeviceSettingsRecord::TableIdL( *iSession );

    iRecord = new (ELeave) CCDWlanDeviceSettingsRecord( tableId );

    iRecord->iWlanDeviceSettingsType = KWlanUserSettings;
    
    
    if(iRecord->FindL( *iSession ))
        {
        iRecord->LoadL( *iSession );
        }          
       
    if ( iRecord->iBgScanInterval != 0 )
        {
        iController->EnableScanL();
        iOuterScanState = ETrue;
        }
    SetActive();
    
    iRecord->RequestNotification(*iSession,iStatus);
    
    }


// ---------------------------------------------------------
// CWsfDbObserver::SetController
// ---------------------------------------------------------  
void CWsfDbObserver::SetController( TWsfAiController* aController )
	{
	iController = aController;
	}


// ---------------------------------------------------------
// CWsfDbObserver::EnableScanL
// ---------------------------------------------------------  
void CWsfDbObserver::EnableScanL()
    {
    LOG_ENTERFN( "CWsfDbObserver::EnableScanL" );
    if ( !iOuterScanState )
        {
        iRecord->RefreshL( *iSession );
        if ( iRecord->iBgScanInterval == 0 ) // not scanning
            {
            TInt j;
            j = iRecord->iSavedBgScanInterval;
            if ( j == 0 )
                {
                j = DefaultScanIntervalL();
                }
            iRecord->iBgScanInterval = j;
            iRecord->ModifyL( *iSession );
            iOuterScanState = ETrue;
            }
#ifndef __WINS__
        // Notifying WLAN Engine about changes in settings
        iWlanMgmtClient->NotifyChangedSettings();
#endif
        }
    }
       

// ---------------------------------------------------------
// CWsfDbObserver::DisableScanL
// ---------------------------------------------------------     
void CWsfDbObserver::DisableScanL()
    {
    LOG_ENTERFN( "CWsfDbObserver::DisableScanL" );
    if ( iOuterScanState )
        {
        iRecord->RefreshL(*iSession);
        if ( iRecord->iBgScanInterval != 0 ) // scanning
            {
            TInt i;
            i = iRecord->iBgScanInterval;
            iRecord->iBgScanInterval = 0;
            iRecord->iSavedBgScanInterval = i;
            iRecord->ModifyL( *iSession );
            iOuterScanState = EFalse;
            }
#ifndef __WINS__
        // Notifying WLAN Engine about changes in settings
        iWlanMgmtClient->NotifyChangedSettings();
#endif
        }
    }


// ---------------------------------------------------------
// CWsfDbObserver::DefaultScanInterval
// ---------------------------------------------------------     
TInt CWsfDbObserver::DefaultScanIntervalL()
    {
    LOG_ENTERFN( "CWsfDbObserver::DefaultScanIntervalL" );
    TBool iPsmSupported = FeatureManager::FeatureSupported(
                                                        KFeatureIdPowerSave );
    if ( iPsmSupported )
        {
        // Read the default value from CenRep (different in PSM mode)
        TInt defaultScanInterval( KWlanSettingsUiDefaultScanNetwork );

        CRepository* repository = CRepository::NewLC( 
                KCRUidWlanDeviceSettingsRegistryId );
        if ( repository )
            {
            repository->Get( KWlanDefaultBGScanInterval, defaultScanInterval );
            }
        CleanupStack::PopAndDestroy( repository );
        return defaultScanInterval;
        }
    else
        {
        return KWlanSettingsUiDefaultScanNetwork;
        }
    }


// ----------------------------------------------------------------------------
// CWsfDbObserver::RunError
// ----------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CWsfDbObserver::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfDbObserver::RunError" );
    LOG_WRITEF( "aError = %d", aError );
#else
TInt CWsfDbObserver::RunError( TInt /*aError*/ )
    {
#endif
    return KErrNone;
    }


