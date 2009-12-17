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
* Description:  Implementation of CWsfActiveTimeUpdater.
*
*/


//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfactivetimeupdater.h"
#include "wsfactivetimerhandler.h"

#include "wsflogger.h"

// CONSTANTS

//  CONSTRUCTION AND DESTRUCTION
// ---------------------------------------------------------------------------
// CWsfActiveTimeUpdater::NewL
// ---------------------------------------------------------------------------
//   
CWsfActiveTimeUpdater* CWsfActiveTimeUpdater::NewL( 
                                MWsfActiveTimerHandler* aActiveTimerHandler )
    {
    LOG_ENTERFN( "CWsfActiveTimeUpdater::NewL" );
    CWsfActiveTimeUpdater* thisPtr = NewLC( aActiveTimerHandler );
    CleanupStack::Pop( thisPtr );
    return thisPtr;
    }
    
// ---------------------------------------------------------------------------
// CWsfActiveTimeUpdater::NewLC
// ---------------------------------------------------------------------------
//   
CWsfActiveTimeUpdater* CWsfActiveTimeUpdater::NewLC( 
                                MWsfActiveTimerHandler* aActiveTimerHandler )
    {
    LOG_ENTERFN( "CWsfActiveTimeUpdater::NewLC" );
    CWsfActiveTimeUpdater* thisPtr = 
                new ( ELeave ) CWsfActiveTimeUpdater( aActiveTimerHandler );
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    return thisPtr;
    }
    
// ---------------------------------------------------------------------------
// CWsfActiveTimeUpdater::~CWsfActiveTimeUpdater
// ---------------------------------------------------------------------------
//   
CWsfActiveTimeUpdater::~CWsfActiveTimeUpdater()
    {
    LOG_ENTERFN( "CWsfActiveTimeUpdater::~CWsfActiveTimeUpdater" );
    if( iPeriodic )
        {
        iPeriodic->Cancel();
        }
    delete iPeriodic;
    }
    
// ---------------------------------------------------------------------------
// CWsfActiveTimeUpdater::CWsfActiveTimeUpdater
// ---------------------------------------------------------------------------
//
CWsfActiveTimeUpdater::CWsfActiveTimeUpdater( 
                                MWsfActiveTimerHandler* aActiveTimerHandler )
    : iActiveTimerHandler( aActiveTimerHandler )
    {
    }
    
// ---------------------------------------------------------------------------
// CWsfActiveTimeUpdater::ConstructL
// ---------------------------------------------------------------------------
//       
void CWsfActiveTimeUpdater::ConstructL()
    {
    LOG_ENTERFN( "CWsfActiveTimeUpdater::ConstructL" );
    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    }
    
//  METHODS
// ---------------------------------------------------------------------------
// CWsfActiveTimeUpdater::Start
// ---------------------------------------------------------------------------
//   
void CWsfActiveTimeUpdater::Start( TInt aUpdateInterval )
    {
    LOG_ENTERFN( "CWsfActiveTimeUpdater::Start" );
    // Start periodic timer
    iPeriodic->Start( TTimeIntervalMicroSeconds32( 0 ), 
                      TTimeIntervalMicroSeconds32( aUpdateInterval ), 
                      TCallBack( UpdateTimeL, this ) );
    }

// ---------------------------------------------------------------------------
// CWsfActiveTimeUpdater::Stop
// ---------------------------------------------------------------------------
//       
void CWsfActiveTimeUpdater::Stop()
    {
    LOG_ENTERFN( "CWsfActiveTimeUpdater::Stop" );
    // Cancel the timer request
    iPeriodic->Cancel();
    }

// ---------------------------------------------------------------------------
// CWsfActiveTimeUpdater::DoUpdateTimeL
// ---------------------------------------------------------------------------
//   
void CWsfActiveTimeUpdater::DoUpdateTimeL()
    {
    LOG_ENTERFN( "CWsfActiveTimeUpdater::DoUpdateTimeL" );
    iActiveTimerHandler->UpdateActiveTimeL();
    }

// ---------------------------------------------------------------------------
// CWsfActiveTimeUpdater::UpdateTimeL
// ---------------------------------------------------------------------------
//    
TInt CWsfActiveTimeUpdater::UpdateTimeL( TAny* aUpdater )
    {
    LOG_ENTERFN( "CWsfActiveTimeUpdater::UpdateTimeL" );
    static_cast<CWsfActiveTimeUpdater*>( aUpdater )->DoUpdateTimeL();
    return 0;
    } 

// End of file
