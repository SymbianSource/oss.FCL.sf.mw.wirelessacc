/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*      Implementation of class TWlanSettingsUiModel.   
*      
*
*/


// INCLUDE FILES
#include <centralrepository.h>
#include <wlanmgmtclient.h>
#include <internetconnectivitycrkeys.h>
#include <wlancontainer.h>
#include "wlansettingsuimodel.h"
#include "wlansettingsuipanic.h"

#include <psmtypes.h>
#include <psmsrvdomaincrkeys.h>
#include <wlandevicesettingsinternalcrkeys.h>
#include <featmgr.h>
#include <mpmconnectscreenid.h>

// CONSTANTS

const TInt KWlanSettingsUiSecondsInMinute = 60;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanSettingsUiModel::NewL
// ---------------------------------------------------------
//
CWlanSettingsUiModel* CWlanSettingsUiModel::NewL
    (
    CMDBSession* aSession,
    CWlanMgmtClient* aWlanMgmtClient,
    CRepository* aRepository
    )
    {
    CWlanSettingsUiModel* self = new( ELeave ) 
         CWlanSettingsUiModel(aSession, 
                              aWlanMgmtClient,
                              aRepository);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------
// CWlanSettingsUiModel::CWlanSettingsUiModel
// ---------------------------------------------------------
//
CWlanSettingsUiModel::CWlanSettingsUiModel
    (
    CMDBSession* aSession,
    CWlanMgmtClient* aWlanMgmtClient,
    CRepository* aRepository
    )
: iShowWlanAvail( KWlanSettingsUiDefaultShowWlanAvail ),
  iScanNetworks( KWlanSettingsUiDefaultScanNetwork ),
  iPowerSaving( KWlanSettingsUiDefaultPowerSaving ),
  iPsmMode( EFalse ),
  iSession( aSession ),
  iWlanMgmtClient( aWlanMgmtClient ),
  iRepository( aRepository ),
  iPsmSupported( EFalse )
    {
    }

// ---------------------------------------------------------
// CWlanSettingsUiModel::ConstructL
// ---------------------------------------------------------
//
void CWlanSettingsUiModel::ConstructL()
    {
    FeatureManager::InitializeLibL();
    // Set the member variable to indicate if full/partial PSM is on.
    // This value stays the same for the whole WLAN Settings session,
    // i.e. we ignore any mode changes during the session.
    CheckPsmModeL();
    }


// ---------------------------------------------------------
// CWlanSettingsUiModel::~CWlanSettingsUiModel()
// ---------------------------------------------------------
//
CWlanSettingsUiModel::~CWlanSettingsUiModel()
    {
     FeatureManager::UnInitializeLib();  
    }

// ---------------------------------------------------------
// CWlanSettingsUiModel::LoadSettingsL
// ---------------------------------------------------------
//
void CWlanSettingsUiModel::LoadSettingsL()
    {
    TBool ownTransaction( EFalse );
    if( !iSession->IsInTransaction() )
        {
        ownTransaction = ETrue;
        iSession->OpenTransactionL();
        }
    TMDBElementId tableId = 0;
    
    tableId = CCDWlanDeviceSettingsRecord::TableIdL( *iSession );
        
    CCDWlanDeviceSettingsRecord* record = new( ELeave )
            CCDWlanDeviceSettingsRecord( tableId );         
    CleanupStack::PushL( record );
    
    record->iWlanDeviceSettingsType = KWlanUserSettings; 
    
    if( record->FindL( *iSession ) )
        {
        record->LoadL( *iSession );
        }

    // Read scan interval

    iScanNetworks = record->iBgScanInterval;
    FixScanNetworks( record->iSavedBgScanInterval );
 
    iPowerSaving = record->iWlanPowerMode;
    CleanupStack::PopAndDestroy( record ); 
    
    if ( ownTransaction )
        {
        iSession->CommitTransactionL();
          // Rollback operation.
        }
    }

// ---------------------------------------------------------
// CWlanSettingsUiModel::SaveSettingsL
// ---------------------------------------------------------
//
void CWlanSettingsUiModel::SaveSettingsL()
    {
    TBool ownTransaction( EFalse );
    if( !iSession->IsInTransaction() )
        {
        ownTransaction = ETrue;
        iSession->OpenTransactionL();
        }
    TMDBElementId tableId = 0;
    tableId = CCDWlanDeviceSettingsRecord::TableIdL( *iSession );
        
    CCDWlanDeviceSettingsRecord* record = new( ELeave )
            CCDWlanDeviceSettingsRecord( tableId );         
    
    CleanupStack::PushL( record );
    
    record->iWlanDeviceSettingsType = KWlanUserSettings; 
    
    if( record->FindL( *iSession ) )
        {
        record->LoadL( *iSession );
        }
    
    if ( iShowWlanAvail )
        {
        record->iBgScanInterval = iScanNetworks;
        }
    else
        {
        record->iBgScanInterval = KWlanSettingsUiScanNetworkNever;
        
        // Do not change the saved interval value if in full or partial PSM Mode.
        // We want to preserve the previous user-selected value in memory, not the
        // forced PSM mode value (automatic).
        if ( !PsmModeOn() )
            {
            record->iSavedBgScanInterval = iScanNetworks;
            }
        }

    record->iWlanPowerMode = iPowerSaving;
    
    // Whenever settings are modified, iUseDefaultSettings must be set to false.
    record->iUseDefaultSettings = EFalse;

    record->ModifyL( *iSession );
        
    CleanupStack::PopAndDestroy( record );        
    if ( ownTransaction )
        {
        iSession->CommitTransactionL();
        }

#ifndef __WINS__
    // Notifying WLAN Engine about changes in settings
    iWlanMgmtClient->NotifyChangedSettings();
#endif
    }

// ---------------------------------------------------------
// CWlanSettingsUiModel::FixScanNetworks
// ---------------------------------------------------------
//
void CWlanSettingsUiModel::FixScanNetworks( TUint aSavedScanInterval )
    {    
    if ( iScanNetworks == KWlanSettingsUiScanNetworkNever )
        {
        iShowWlanAvail = EFalse;
  
        if( aSavedScanInterval == KWlanSettingsUiScanNetworkNever
            || iPsmMode )
            {
            iScanNetworks = KWlanSettingsUiDefaultScanNetwork;
            }
        else
            {
            iScanNetworks = FixScanInterval( aSavedScanInterval );
            }
        }   
             
    else
        {
        iShowWlanAvail = ETrue;
        iScanNetworks = FixScanInterval( iScanNetworks );
        }
    }


// ---------------------------------------------------------
// CWlanSettingsUiModel::FixScanInterval
// ---------------------------------------------------------
//
TUint CWlanSettingsUiModel::FixScanInterval( TUint aScanInterval )
    {
    if( aScanInterval <=
        ( EWlanSettingsUiScanNetworkMax * KWlanSettingsUiSecondsInMinute ) )
        {
        // Return only full minutes
        return aScanInterval -
            ( aScanInterval % KWlanSettingsUiSecondsInMinute );
        }
    else
        {
        return KWlanSettingsUiScanNetworkAuto;
        }
    }

// ---------------------------------------------------------
// CWlanSettingsUiModel::CheckPsmModeL
// ---------------------------------------------------------
//
void CWlanSettingsUiModel::CheckPsmModeL()
    {		
    iPsmSupported = FeatureManager::FeatureSupported( KFeatureIdPowerSave ); 
    if( iPsmSupported )   
    {       	   
    TPsmsrvMode mode( EPsmsrvModeNormal );

    CRepository* cenrep = NULL;

    TRAP_IGNORE( cenrep = CRepository::NewL( KCRUidPowerSaveMode ) );
    
  	  if ( cenrep )
    	    {
      	  TInt crValue(0);
        	cenrep->Get( KPsmCurrentMode, crValue );
        	mode = static_cast<TPsmsrvMode>( crValue );
        	if ( mode == EPsmsrvModePowerSave || mode == EPsmsrvPartialMode )
                {
                iPsmMode = ETrue;
                }
        	else 
        	    {
        	    iPsmMode = EFalse;
        	    }
        	
         delete cenrep;
       	 cenrep = NULL;                    
        }
      }  
    }

// ---------------------------------------------------------
// CWlanSettingsUiModel::ScanNetworks
// ---------------------------------------------------------
//              
TUint CWlanSettingsUiModel::ScanNetworks()
    {
    if ( iScanNetworks == KWlanSettingsUiScanNetworkAuto )
        {
        return iScanNetworks;   
        }
    else
        {
        // Return scan time in minutes
        return ( iScanNetworks / KWlanSettingsUiSecondsInMinute );
        }
    }

// ---------------------------------------------------------
// CWlanSettingsUiModel::SetScanNetworks
// ---------------------------------------------------------
//        
void CWlanSettingsUiModel::SetScanNetworks( TUint aScanNetworks )
    {
    if ( aScanNetworks == KWlanSettingsUiScanNetworkAuto )
        {
        iScanNetworks = aScanNetworks;
        }
    else
        {
        // Scan time stored in seconds
        iScanNetworks = aScanNetworks * KWlanSettingsUiSecondsInMinute;
        }
    } 
