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
// CWlanPluginDbObserver::CWlanPluginDbObserver
// Constructor
// ---------------------------------------------------------
CWsfDbObserver::CWsfDbObserver(  ) 
: CActive( EPriorityNormal ),
iOuterScanState( EFalse )
    {
    }
    
// ---------------------------------------------------------
// CWlanPluginDbObserver::NewL
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
// CWlanPluginDbObserver::ConstructL
// ---------------------------------------------------------   
void CWsfDbObserver::ConstructL()
    {
#ifndef __WINS__ // client is not available on wins
    iWlanMgmtClient = CWlanMgmtClient::NewL();
#endif
    
    FeatureManager::InitializeLibL(); 
         
    CActiveScheduler::Add(this);

  }
    
// ---------------------------------------------------------
// CWlanPluginDbObserver::~CWlanPluginDbObserver
// Destructor
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
// CWlanPluginDbObserver::RunL
// ---------------------------------------------------------       
void CWsfDbObserver::RunL()
    {
    LOG_ENTERFN( "CWsfDbObserver::RunL" );        
    //iSession->OpenTransactionL();
    //iRecord->RefreshL( *iSession );
    iRecord->LoadL( *iSession );
    //iSession->CommitTransactionL();        
    
    //if scanning state changed start or stop the server
    TInt i = iRecord->iBgScanInterval;
    TInt j = iRecord->iSavedBgScanInterval;
    LOG_WRITEF( "CWsfDbObserver::RunL -- Bg:%d bg2:%d O:%d", i, j, iOuterScanState );
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
// CWlanPluginDbObserver::DoCancel
// ---------------------------------------------------------       
void CWsfDbObserver::DoCancel()
    {
    
    iRecord->CancelNotification( *iSession, iStatus );       
    
    }

// ---------------------------------------------------------
// CWlanPluginDbObserver::ActivateIt
// ---------------------------------------------------------       
void CWsfDbObserver::ActivateItL()
    {
    
    iSession = CMDBSession::NewL( KCDLatestVersion );
    
    TMDBElementId tableId = 0;
    
    tableId = CCDWlanDeviceSettingsRecord::TableIdL( *iSession );
        
    iRecord = new( ELeave )
            CCDWlanDeviceSettingsRecord( tableId );         
    
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
// CWlanPluginDbObserver::SetController
// ---------------------------------------------------------  
void CWsfDbObserver::SetController( TWsfAiController* aController )
	{
	iController = aController;
	}

// ---------------------------------------------------------
// CWlanPluginDbObserver::EnableScanL
// ---------------------------------------------------------  
void CWsfDbObserver::EnableScanL()
    {
	if( !iOuterScanState )
        {
        iSession->OpenTransactionL();
        iRecord->RefreshL( *iSession );
        if ( iRecord->iBgScanInterval == 0 ) // not scanning
        	{
        	TInt j;
        	j = iRecord->iSavedBgScanInterval;
        	if( j == 0 )
        		{
        		j = DefaultScanIntervalL();        		
        		}
        	iRecord->iBgScanInterval = j;
        	iRecord->ModifyL( *iSession );
        	iOuterScanState = ETrue;
        	}
        iSession->CommitTransactionL();
#ifndef __WINS__
        // Notifying WLAN Engine about changes in settings
        iWlanMgmtClient->NotifyChangedSettings();
#endif
        }
    }
        
// ---------------------------------------------------------
// CWlanPluginDbObserver::DisableScanL
// ---------------------------------------------------------     
void CWsfDbObserver::DisableScanL()
	{
	if( iOuterScanState )
		{
		iSession->OpenTransactionL();
		iRecord->RefreshL( *iSession );
        if ( iRecord->iBgScanInterval != 0 ) // scanning
        	{
        	TInt i;
        	i = iRecord->iBgScanInterval;
        	iRecord->iBgScanInterval = 0;
        	iRecord->iSavedBgScanInterval = i;
        	iRecord->ModifyL( *iSession );
        	iOuterScanState = EFalse;
        	}
        iSession->CommitTransactionL();
#ifndef __WINS__
        // Notifying WLAN Engine about changes in settings
        iWlanMgmtClient->NotifyChangedSettings();
#endif
		}
	}

// ---------------------------------------------------------
// CWlanPluginDbObserver::DefaultScanInterval
// ---------------------------------------------------------     
TInt CWsfDbObserver::DefaultScanIntervalL()
    {  
	  	TBool iPsmSupported = FeatureManager::FeatureSupported( KFeatureIdPowerSave );
			if ( iPsmSupported )
				{
    		// Read the default value from CenRep (different in PSM mode)
    		TInt defaultScanInterval( KWlanSettingsUiDefaultScanNetwork );
    
    		CRepository* repository = CRepository::NewLC( KCRUidWlanDeviceSettingsRegistryId ); 
    		if ( repository )
    			{
      	  	repository->Get( KWlanDefaultBGScanInterval, defaultScanInterval );
    			}     
    		 CleanupStack::PopAndDestroy( repository );
   			 return  defaultScanInterval; 
 				 } 
  		else
				{
   			 return KWlanSettingsUiDefaultScanNetwork;
  			}	
     }

