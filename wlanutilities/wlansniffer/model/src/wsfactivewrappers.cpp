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
 * Description:  Implementation of CWsfActiveWrappers.
 *
 */



// INCLUDE FILES
#include "wsflogger.h"
#include "wsfactivewrappers.h"
#include "wsfwlanlistactivewrapper.h"
#include "wsfrefreshscanactivewrapper.h"
#include "wsfdisconnectactivewrapper.h"
#include "wsfconnectactivewrapper.h"
#include "wsflaunchaihelperactivewrapper.h"
#include "wsfconnmonactivewrapper.h"
#include "wsfmodelobserver.h"


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::CWsfActiveWrappers
// ----------------------------------------------------------------------------
//
CWsfActiveWrappers::CWsfActiveWrappers()
    {
    // No implementation required
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::~CWsfActiveWrappers
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfActiveWrappers::~CWsfActiveWrappers()
    {
    delete iWLANListActiveWrapper;
    delete iRefreshScanActiveWrapper;
    delete iDisconnectActiveWrapper;
    delete iConnectActiveWrapper;
    delete iLaunchAiHelperActiveWrapper;
    delete iConnMonActiveWrapper;
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::NewLC
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfActiveWrappers* CWsfActiveWrappers::NewLC( CWsfModel* aModel,
        MWsfModelObserver &aModelObserver )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::NewLC" );
    CWsfActiveWrappers* self = new ( ELeave ) CWsfActiveWrappers();
    CleanupStack::PushL( self );
    self->ConstructL( aModel, aModelObserver );
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfActiveWrappers* CWsfActiveWrappers::NewL( CWsfModel* aModel,
        MWsfModelObserver &aModelObserver )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::NewL" );
    CWsfActiveWrappers* self = CWsfActiveWrappers::NewLC( aModel, aModelObserver );
    CleanupStack::Pop(); // self;
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfActiveWrappers::ConstructL( CWsfModel* aModel,
        MWsfModelObserver &aModelObserver )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::ConstructL" );
    iWLANListActiveWrapper = CWsfWLANListActiveWrapper::NewL( 
            aModel, 
            aModelObserver );
    
    iRefreshScanActiveWrapper = CWsfRefreshScanActiveWrapper::NewL( aModel );
    
    iDisconnectActiveWrapper = CWsfDisconnectActiveWrapper::NewL( aModel );
    
    iConnectActiveWrapper = CWsfConnectActiveWrapper::NewL( aModel );
    
    iLaunchAiHelperActiveWrapper = 
        CWsfLaunchAiHelperActiveWrapper::NewL(
            aModel, 
            *this );
    
    iConnMonActiveWrapper = CWsfConnMonActiveWrapper::NewL( aModelObserver );
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::Disconnect
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfActiveWrappers::Disconnect()
    {
    LOG_ENTERFN( "CWsfActiveWrappers::Disconnect" );
    iDisconnectActiveWrapper->Start();
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::Connect
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfActiveWrappers::Connect( TUint aIapID, TBool aConnectOnly, 
                                  TWsfIapPersistence aPersistence )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::Connect" );
    iConnectActiveWrapper->Start( aIapID, aConnectOnly, aPersistence );
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::RefreshScan
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfActiveWrappers::RefreshScan()
    {
    LOG_ENTERFN( "CWsfActiveWrappers::RefreshScan" );
    iRefreshScanActiveWrapper->Start();
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::RefreshWLANList
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfActiveWrappers::RefreshWLANList( TBool aStarUp )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::RefreshWLANList" );
    iWLANListActiveWrapper->Start( aStarUp );
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::LaunchHelperApplicationL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfActiveWrappers::LaunchHelperApplicationL( TWsfWlanInfo& aInfo, 
                                                   TBool aConnectOnly,
                                                   TBool aTestAccessPoint )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::LaunchHelperApplicationL" );
    iLaunchAiHelperActiveWrapper->Start( aInfo, aConnectOnly, aTestAccessPoint );
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::GetWLANList
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfWlanInfoArray* CWsfActiveWrappers::GetWLANList()
    {
    LOG_ENTERFN( "CWsfActiveWrappers::GetWLANList" );
    return iWLANListActiveWrapper->GetWlanList();
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::GetConnectedWLANNetwork
// ----------------------------------------------------------------------------
//
EXPORT_C TWsfWlanInfo CWsfActiveWrappers::GetConnectedWLANNetwork()
    {
    LOG_ENTERFN( "CWsfActiveWrappers::GetConnectedWLANNetwork" );
    return iWLANListActiveWrapper->GetConnectedWLANNetwork();
    }
	
// ----------------------------------------------------------------------------
// CWsfActiveWrappers::CheckIsWlanUsedByBrowserL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfActiveWrappers::CheckIsWlanUsedByBrowserL()
    {
    LOG_ENTERFN( "CWsfActiveWrappers::CheckIsWlanUsedByBrowserL" );
    return iConnMonActiveWrapper->Start();
	}

