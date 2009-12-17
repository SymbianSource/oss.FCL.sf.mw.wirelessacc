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
*      Implementation of time shifting   
*      
*
*/


//User includes
#include "wlanplugintimeshifter.h"
#include "wlanpluginwlanstate.h"
#include "wlanpluginlogger.h"


// ---------------------------------------------------------
// CWlanPluginTimeShifter::NewLC
// ---------------------------------------------------------  
CWlanPluginTimeShifter* CWlanPluginTimeShifter::NewLC( 
                                      CWlanPluginWlanState* aWlanState )
    {
    CWlanPluginTimeShifter* self = new( ELeave )
                                   CWlanPluginTimeShifter( 
                                   aWlanState );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }       

// ---------------------------------------------------------
// CWlanPluginTimeShifter::NewL
// --------------------------------------------------------- 
CWlanPluginTimeShifter* CWlanPluginTimeShifter::NewL( 
                                      CWlanPluginWlanState* aWlanState )
    {
    CWlanPluginTimeShifter* self = CWlanPluginTimeShifter::NewLC( 
                                      aWlanState );
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------
// CWlanPluginTimeShifter::~CWlanPluginTimeShifter
// --------------------------------------------------------- 
CWlanPluginTimeShifter::~CWlanPluginTimeShifter()
    {
    Cancel();
    }
    
// ---------------------------------------------------------
// CWlanPluginTimeShifter::CWlanPluginTimeShifter
// --------------------------------------------------------- 
CWlanPluginTimeShifter::CWlanPluginTimeShifter( 
                                    CWlanPluginWlanState* aWlanState )
    : CActive( EPriorityNormal ),
    iWlanState( aWlanState )
    {
    
    }
    
// ---------------------------------------------------------
// CWlanPluginTimeShifter::ConstructL
// --------------------------------------------------------- 
void CWlanPluginTimeShifter::ConstructL()
    {
    CLOG_ENTERFN( "CWlanPluginTimeShifter::ConstructL" ); 
    
    CActiveScheduler::Add(this);
    CLOG_LEAVEFN( "CWlanPluginTimeShifter::ConstructL" ); 
    }
    
// ---------------------------------------------------------
// CWlanPluginTimeShifter::ConstructL
// --------------------------------------------------------- 
void CWlanPluginTimeShifter::RunL()
    {
    CLOG_ENTERFN( "CWlanPluginTimeShifter::RunL" ); 
    
    iWlanState->DirectlyScanL();
    iWlanState->DeleteTimeShifter();
	
	CLOG_LEAVEFN( "CWlanPluginTimeShifter::RunL" ); 
    }
        
// ---------------------------------------------------------
// CWlanPluginTimeShifter::ConstructL
// --------------------------------------------------------- 
void CWlanPluginTimeShifter::DoCancel()
    {
    CLOG_ENTERFN( "CWlanPluginTimeShifter::DoCancel" ); 
    CLOG_LEAVEFN( "CWlanPluginTimeShifter::DoCancel" ); 
    }

// ---------------------------------------------------------
// CWlanPluginTimeShifter::ConstructL
// ---------------------------------------------------------     
void CWlanPluginTimeShifter::ActivateIt()
    {
    CLOG_ENTERFN( "CWlanPluginTimeShifter::ActivateIt" ); 
    
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive();
   
    CLOG_LEAVEFN( "CWlanPluginTimeShifter::ActivateIt" );
    }
