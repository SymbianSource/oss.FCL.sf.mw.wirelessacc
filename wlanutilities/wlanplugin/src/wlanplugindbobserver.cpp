/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "wlanplugindbobserver.h"
#include "wlanpluginwlanstate.h"
#include "wlanpluginlogger.h"

// ---------------------------------------------------------
// CWlanPluginDbObserver::CWlanPluginDbObserver
// Constructor
// ---------------------------------------------------------
CWlanPluginDbObserver::CWlanPluginDbObserver( CWlanPluginWlanState* aState ) 
: CActive( EPriorityNormal ),
  iWlanState( aState )
    {
    }
    
// ---------------------------------------------------------
// CWlanPluginDbObserver::NewL
// ---------------------------------------------------------    
CWlanPluginDbObserver* CWlanPluginDbObserver::NewL( 
                                              CWlanPluginWlanState* aState )
    {
    CWlanPluginDbObserver* self = new ( ELeave ) 
                                             CWlanPluginDbObserver( aState );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
       
// ---------------------------------------------------------
// CWlanPluginDbObserver::ConstructL
// ---------------------------------------------------------   
void CWlanPluginDbObserver::ConstructL()
    {
    CLOG_ENTERFN( "CWlanPluginDbObserver::ConstructL()" );
     
    CActiveScheduler::Add(this);
    
    CLOG_LEAVEFN( "CWlanPluginDbObserver::ConstructL()" );
    }
    
// ---------------------------------------------------------
// CWlanPluginDbObserver::~CWlanPluginDbObserver
// Destructor
// ---------------------------------------------------------       
CWlanPluginDbObserver::~CWlanPluginDbObserver()
    {
    CLOG_ENTERFN( "CWlanPluginDbObserver::~CWlanPluginDbObserver()" );
    
    Cancel();
    
    delete iRecord;
    
    delete iSession;
            
    CLOG_LEAVEFN( "CWlanPluginDbObserver::~CWlanPluginDbObserver()" );
    }

// ---------------------------------------------------------
// CWlanPluginDbObserver::RunL
// ---------------------------------------------------------       
void CWlanPluginDbObserver::RunL()
    {
    CLOG_ENTERFN( "CWlanPluginDbObserver::RunL()" );
            
    iRecord->RefreshL( *iSession );
            
    //if scanning state changed start or stop the server
    if ( iRecord->iBgScanInterval == 0 && iOuterScanState )
        {
        CLOG_WRITE( "Scan disabled" );
        iWlanState->SetScanningL( EFalse );
        iOuterScanState = EFalse;
        }
    else if(iRecord->iBgScanInterval != 0 && !iOuterScanState )
        {
        CLOG_WRITE( "Scan enabled" );
        iWlanState->SetScanningL( ETrue );
        iOuterScanState = ETrue;
        }
    
    SetActive();
    
    iRecord->RequestNotification(*iSession,iStatus);
    
    CLOG_LEAVEFN( "CWlanPluginDbObserver::RunL()" );
    }

// ---------------------------------------------------------
// CWlanPluginDbObserver::DoCancel
// ---------------------------------------------------------       
void CWlanPluginDbObserver::DoCancel()
    {
    CLOG_ENTERFN( "CWlanPluginDbObserver::DoCancel()" );
    
    iRecord->CancelNotification( *iSession, iStatus );       
    
    CLOG_LEAVEFN( "CWlanPluginDbObserver::DoCancel()" );
    }

// ---------------------------------------------------------
// CWlanPluginDbObserver::ActivateIt
// ---------------------------------------------------------       
void CWlanPluginDbObserver::ActivateItL()
    {
    CLOG_ENTERFN( "CWlanPluginDbObserver::ActivateItL()" );
    
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
    
    
    iRecord->RequestNotification(*iSession,iStatus);
    
    SetActive();
    
    if ( iRecord->iBgScanInterval != 0 )
        {
        iWlanState->SetScanningL( ETrue );
        iOuterScanState = ETrue;
        }
    
    CLOG_LEAVEFN( "CWlanPluginDbObserver::ActivateItL()" );
    }
