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
CWsfActiveWrappers::~CWsfActiveWrappers()
    {
    delete iWLANListActiveWrapper;
    delete iRefreshScanActiveWrapper;
    delete iDisconnectActiveWrapper;
    delete iConnectActiveWrapper;
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::NewLC
// ----------------------------------------------------------------------------
//
CWsfActiveWrappers* CWsfActiveWrappers::NewLC( CWsfModel* aModel,
        TWsfAiController &aController )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::NewLC" );
    CWsfActiveWrappers* self = new (ELeave) CWsfActiveWrappers();
    CleanupStack::PushL( self );
    self->ConstructL( aModel,aController );
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::NewL
// ----------------------------------------------------------------------------
//
CWsfActiveWrappers* CWsfActiveWrappers::NewL( CWsfModel* aModel,
        TWsfAiController &aController )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::NewL" );
    CWsfActiveWrappers* self = CWsfActiveWrappers::NewLC( aModel, aController );
    CleanupStack::Pop(); // self;
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfActiveWrappers::ConstructL( CWsfModel* aModel,
        TWsfAiController &aController )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::ConstructL" );
    iWLANListActiveWrapper = CWsfWLANListActiveWrapper::NewL( aModel, 
                                                              aController );
    
    iRefreshScanActiveWrapper = CWsfRefreshScanActiveWrapper::NewL( aModel );
    
    iDisconnectActiveWrapper = CWsfDisconnectActiveWrapper::NewL( aModel );
    
    iConnectActiveWrapper = CWsfConnectActiveWrapper::NewL( aModel );
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::Disconnect
// ----------------------------------------------------------------------------
//
void CWsfActiveWrappers::Disconnect()
    {
    LOG_ENTERFN( "CWsfActiveWrappers::Disconnect" );
    iDisconnectActiveWrapper->Start();
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::Connect
// ----------------------------------------------------------------------------
//
void CWsfActiveWrappers::Connect( TUint aIapID, TWsfIapPersistence aPersistence )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::Connect" );
    iConnectActiveWrapper->Start( aIapID, aPersistence );
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::RefreshScan
// ----------------------------------------------------------------------------
//
void CWsfActiveWrappers::RefreshScan()
    {
    LOG_ENTERFN( "CWsfActiveWrappers::RefreshScan" );
    iRefreshScanActiveWrapper->Start();
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::RefreshWLANList
// ----------------------------------------------------------------------------
//
void CWsfActiveWrappers::RefreshWLANList( TBool aStarUp )
    {
    LOG_ENTERFN( "CWsfActiveWrappers::RefreshWLANList" );
    iWLANListActiveWrapper->Start( aStarUp );
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::GetWLANList
// ----------------------------------------------------------------------------
//
CWsfWlanInfoArray* CWsfActiveWrappers::GetWLANList()
    {
    LOG_ENTERFN( "CWsfActiveWrappers::GetWLANList" );
    return iWLANListActiveWrapper->GetWlanList();
    }


// ----------------------------------------------------------------------------
// CWsfActiveWrappers::GetConnectedWLANNetwork
// ----------------------------------------------------------------------------
//
TWsfWlanInfo CWsfActiveWrappers::GetConnectedWLANNetwork()
    {
    LOG_ENTERFN( "CWsfActiveWrappers::GetConnectedWLANNetwork" );
    return iWLANListActiveWrapper->GetConnectedWLANNetwork();
    }


