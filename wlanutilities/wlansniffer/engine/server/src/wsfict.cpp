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
* Description:  Implementation of CWsfIct
*
*/


//  EXTERNAL INCLUDES
#include <apgcli.h>
#include <apgtask.h>
#include <ictsclientinterface.h>
#include <cmmanagerext.h>
#include <cmdestinationext.h>
#include <cmpluginwlandef.h>

//  CLASS HEADER
#include "wsfict.h"
#include "wsflogger.h"

//  INTERNAL INCLUDES

using namespace CMManager;

/**
* UID of Wlan Login application (hsbrowser)
* used when launching WLAN Login application
*/
static const TInt KBrowserUid = { 0x2000AFCC };

/**
* Length of a needed separators
* used when launching WLAN Login application
*/
const TInt KSeparatorsLength = 4;


// ----------------------------------------------------------------------------
// CWsfIct::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfIct* CWsfIct::NewL()
    {
    CWsfIct* self = CWsfIct::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfIct::NewLC
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfIct* CWsfIct::NewLC()
    {
    CWsfIct* self = new( ELeave ) CWsfIct;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfIct::~CWsfIct
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfIct::~CWsfIct()
    {
    LOG_ENTERFN( "CWsfIct::~CWsfIct" );
    if ( iIct )
        {
        LOG_WRITE( "ict cancel" );
        TRAP_IGNORE( iIct->CancelStartL() );
        delete iIct;
        iIct = NULL;
        }
    }


// ----------------------------------------------------------------------------
// CWsfIct::CWsfIct
// ----------------------------------------------------------------------------
//
CWsfIct::CWsfIct(): iConnectOnly( EFalse )
    {
    }


// ----------------------------------------------------------------------------
// CWsfIct::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfIct::ConstructL()
    {
    }


// ----------------------------------------------------------------------------
// CWsfIct::ConnectivityObserver
// ----------------------------------------------------------------------------
//
void CWsfIct::ConnectivityObserver( TIctsTestResult aResult, 
                                    const TDesC& aString )
    {
    LOG_ENTERFN( "CWsfIct::ConnectivityObserver" );
    LOG_WRITEF( "ICTS result: %d", aResult );
    
    // check the result
    switch ( aResult )
        {
        case EConnectionOk:
            {
            // test succeeded
            TRAP_IGNORE( MoveToInternetSnapL( iIapId ) );
            LOG_WRITE( "ICT: EConnectionOk" );
            break;            
            }
            
        case EConnectionNotOk:
            {
            // test was run but it failed
            LOG_WRITE( "ICT: EConnectionNotOk" );
            break;
            }
        case EHttpAuthenticationNeeded:
            {
            // test was run but HTTP authentication is required
            LOG_WRITE( "ICT: EHttpAuthenticationNeeded" );
            if ( iConnectOnly )
                {
                // Connect selected. WLAN Login needed.
                TRAP_IGNORE( LaunchWlanLoginL(aString) );
                }    
            break;
            }    
        case ETimeout:
            {
            LOG_WRITE( "ICT: ETimeout" );
            break;
            }
            
        default:
            {
            _LIT( KIctPanic, "ICT result" );
            User::Panic( KIctPanic, aResult );
            }
        }
    }


// -----------------------------------------------------------------------------
// CWsfIct::LaunchWlanLoginL()
// -----------------------------------------------------------------------------
//    
void CWsfIct::LaunchWlanLoginL( const TDesC& aString )
    {   
    LOG_ENTERFN( "CWsfIct::LaunchWlanLoginL" );
      
    // Count IAP Id length
    TInt iapIdLength ( 1 );
    TInt iapId = iIapId;
    while ( iapId >= 10 )
        {
        iapId = iapId/10;
        iapIdLength++;
        }
  
    // Count Network Id length
    TInt netIdLength ( 1 );
    TInt netId = iNetId;
    while ( netId >= 10 )
        {
        netId = netId/10;
        netIdLength++;
        }
    
    TInt length = aString.Length() + 
                  iapIdLength + 
                  netIdLength + 
                  KSeparatorsLength;
    HBufC* param = HBufC::NewLC( length );
    _LIT(tmpString, "%d, %d, %S");
    param->Des().Format( tmpString, 
                         iIapId, 
                         iNetId, 
                         &aString );
    TUid uid( TUid::Uid( KBrowserUid ) );
    TThreadId id;
    
    RApaLsSession appArcSession;
    User::LeaveIfError( appArcSession.Connect() ); 
    CleanupClosePushL( appArcSession );
        
    TInt err = appArcSession.StartDocument( *param, TUid::Uid( KBrowserUid ), id );
    if ( err != KErrNone )
        {
        LOG_ENTERFN( "CWsfIct::LaunchWlanLoginL failed" );
        }
    CleanupStack::PopAndDestroy( &appArcSession );
    CleanupStack::PopAndDestroy( param );
    }


// ----------------------------------------------------------------------------
// CWsfIct::MoveToInternetSnapL
// ----------------------------------------------------------------------------
//
void CWsfIct::MoveToInternetSnapL( const TUint32 aIapId )
    {
	LOG_ENTERFN( "CWsfIct::MoveToInternetSnapL" );
    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );
    
    // Check that is IAP uncategorized
    RArray<TUint32> cmIds;
    CleanupClosePushL( cmIds );
    cmManager.ConnectionMethodL( cmIds, ETrue, ETrue, EFalse );
    TBool foundFromUncategorized = EFalse;
    
    for ( TInt k = 0; k < cmIds.Count(); k++ )
        {
        RCmConnectionMethodExt cm;
        TRAPD( error, cm = cmManager.ConnectionMethodL( cmIds[k] ) );
        if ( KErrNone == error )
            {
            CleanupClosePushL( cm );

            TUint iapId = cm.GetIntAttributeL( CMManager::ECmIapId );
            if ( iapId == aIapId )
                {
                LOG_WRITE( "IAP is uncategorized" );
                foundFromUncategorized = ETrue;
                }
            CleanupStack::PopAndDestroy( &cm );
            }
        }
    
    CleanupStack::PopAndDestroy( &cmIds );
    
    if ( !foundFromUncategorized )
        {
        LOG_WRITE( "Not moving IAP since it is in SNAP" );
        CleanupStack::PopAndDestroy( &cmManager );
        return;
        }
    
    // Read all destination(SNAP) settings into an array
    RArray<TUint32> destinations;
    CleanupClosePushL( destinations );
    
    cmManager.AllDestinationsL( destinations );
    RCmDestinationExt destination;
    // Loop through each destination
    for( TInt i = 0; i < destinations.Count(); i++ )
        {
        destination = cmManager.DestinationL( destinations[i] );
        CleanupClosePushL( destination ); 
        // Internet destination will always exist in the system.
        // Internet destination will have ESnapPurposeInternet 
        // set in its metadata.
        if ( destination.MetadataL( CMManager::ESnapMetadataPurpose ) == 
                                    CMManager::ESnapPurposeInternet )
            {
            RCmConnectionMethodExt iap = cmManager.ConnectionMethodL( aIapId );
            CleanupClosePushL( iap );     
            LOG_WRITE( "Move Iap to internet destination" );
            destination.AddConnectionMethodL( iap );
            destination.UpdateL();
            CleanupStack::PopAndDestroy( &iap ); 
            }
        CleanupStack::PopAndDestroy( &destination ); 
        }
    CleanupStack::PopAndDestroy( &destinations ); 
    CleanupStack::PopAndDestroy( &cmManager ); 
    }


// ----------------------------------------------------------------------------
// CWsfIct::TestConnectedAccessPointL
// ----------------------------------------------------------------------------
//
void CWsfIct::TestConnectedAccessPointL( TUint aIapId )
    {
    LOG_ENTERFN( "CWsfIct::TestConnectedAccessPointL" );
    if ( !aIapId || aIapId != iIapId )
        {
        // the wlaninfo must already contain a valid IAP id
        LOG_WRITE( "invalid IAP id" );
        return;
        }

    if ( iIct )
        {
        iIct->CancelStartL();
        delete iIct;
        iIct = NULL;
        }

    LOG_WRITE( "starting ICT test..." );

    iIct = CIctsClientInterface::NewL( iIapId, iNetId, *this );
    LOG_WRITE( "ICT created" );
    iIct->StartL();
    LOG_WRITE( "ICT: started" );
    }


// ----------------------------------------------------------------------------
// CWsfIct::InitializeIctL
// ----------------------------------------------------------------------------
//
void CWsfIct::InitializeIctL( TBool aTestAccessPoint,
                              TUint aIapId,
                              TBool aConnectOnly )
    {
    LOG_ENTERFN( "CWsfIct::InitializeIct" );

    LOG_WRITEF( "IAP id = %d aTestAccessPoint = %d aConnectOnly = %d", 
                aIapId, aTestAccessPoint, aConnectOnly );
    
    if ( !aIapId || !aTestAccessPoint )
        {
        // the wlaninfo must already contain a valid IAP id 
        // and check that IAP needs testing
        LOG_WRITE( "ICT not initialized" );
        iConnectOnly = EFalse;
        iIapId = 0;
        iNetId = 0;
        return;
        }
    
    // set variables
    iConnectOnly = aConnectOnly;
    iIapId = aIapId;

    if ( iIct )
        {
        iIct->CancelStartL();
        delete iIct;
        iIct = NULL;
        }

    // get network Id
    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL(cmManager);

    RCmConnectionMethodExt cm = cmManager.ConnectionMethodL(iIapId);
    CleanupClosePushL(cm);

    iNetId = cm.GetIntAttributeL(CMManager::ECmNetworkId);
    
    LOG_WRITEF( "Network Id=%d ", iNetId );

    CleanupStack::PopAndDestroy(&cm);
    CleanupStack::PopAndDestroy(&cmManager);
    }
    
// End of file

