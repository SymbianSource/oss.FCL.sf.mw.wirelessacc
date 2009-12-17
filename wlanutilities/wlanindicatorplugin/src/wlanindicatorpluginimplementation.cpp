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
* Description:  wlanindicatorplugin implementation
*
*/



// INCLUDE FILES
#include <eikenv.h>
#include <avkon.rsg>
#include <avkon.hrh>
#include <apgcli.h>
#include <apgtask.h>
#include <coemain.h>
#include <bautils.h>
#include <data_caging_path_literals.hrh>
#include <StringLoader.h>
#include <rconnmon.h>
#include <cmmanager.h>
#include <cmconnectionmethod.h>
#include <wlanmgmtclient.h>
#include <wlanscaninfo.h>
#include <cdblen.h>
#include <commdb.h>
#include <WlanCdbCols.h>
#include <utf.h>
#include <wlanindicator.rsg>
#include "wlanindicatorpluginimplementation.h"


// CONSTANTS
const TUid KSnifferAppUid = { 0x10281CAA };
_LIT( KDriveZ, "z:" );
_LIT( KWlanIndicatorResourceFile, "wlanindicator.rsc" );

using namespace CMManager;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::CWlanIndicatorPluginImplementation
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CWlanIndicatorPluginImplementation::CWlanIndicatorPluginImplementation()
    : iCoeEnv( CCoeEnv::Static() )
    {
    }

// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CWlanIndicatorPluginImplementation::ConstructL()
    {    
    TFileName fileName;

    fileName.Append( KDriveZ );
    fileName.Append( KDC_RESOURCE_FILES_DIR );   
    fileName.Append( KWlanIndicatorResourceFile );

    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );
    iResource = iCoeEnv->AddResourceFileL( fileName );
    
    iConnMonitor.ConnectL();
    
    iWlanMgmtClient = CWlanMgmtClient::NewL();
    
    }

// -----------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CWlanIndicatorPluginImplementation* CWlanIndicatorPluginImplementation::NewL()
    {
    CWlanIndicatorPluginImplementation* self = 
        new( ELeave ) CWlanIndicatorPluginImplementation;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::~CWlanIndicatorPluginImplementation
// Destructor.
// -----------------------------------------------------------------------------
//
CWlanIndicatorPluginImplementation::~CWlanIndicatorPluginImplementation()
    {
    iCoeEnv->DeleteResourceFile( iResource );
 
    iConnMonitor.Close();
               
    delete iWlanMgmtClient;
    
    }


// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::HandleIndicatorTapL
// ---------------------------------------------------------------------------
//
void CWlanIndicatorPluginImplementation::HandleIndicatorTapL( const TInt aUid )
    {
    
    switch ( aUid )
        {
        case EAknIndicatorWlanAvailable:
        case EAknIndicatorWlanActive:
        case EAknIndicatorWlanActiveSecure:
            {

            LaunchWlanSnifferL();
            
            break;
            }   

        default:
            
            break;

        }


    }



// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::TextL( const TInt aUid, TInt& aTextType )
// ---------------------------------------------------------------------------
//    
HBufC* CWlanIndicatorPluginImplementation::TextL( const TInt aUid,
                                                 TInt& aTextType )
    {   
    HBufC* textBuf = NULL;
    
    switch ( aUid )
        {
        case EAknIndicatorWlanAvailable:
            {
            textBuf = CreateWlanNetworksFoundTextL();           
            aTextType = EAknIndicatorPluginLinkText;
                        
            break;
            }
            
        case EAknIndicatorWlanActive:
        case EAknIndicatorWlanActiveSecure:
            {                        
            textBuf = CreateWlanConnectedTextL();
            aTextType = EAknIndicatorPluginLinkText;
            
            break;
            }

        default:
            
            break;

        }
    
    
    return textBuf;
    
    }
    
// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::CreateWlanNetworksFoundTextL()
// ---------------------------------------------------------------------------
//
HBufC* CWlanIndicatorPluginImplementation::CreateWlanNetworksFoundTextL()
    {
    
    HBufC* dynText = NULL;
    HBufC* knownNetworkName = NULL;   
    TInt availableCount( 0 );

    knownNetworkName = AvailableNetworksInfoL( availableCount );
    
    CleanupStack::PushL( knownNetworkName ); 
    
    if ( !knownNetworkName )
        {
        if ( availableCount == 1 )
            {
            // dynText: "qtn_uni_ind_wlan_avail_unknown        WLAN network found"
            dynText = StringLoader::LoadL (
                    R_QTN_UNI_IND_WLAN_AVAIL_UNKNOWN, iCoeEnv );
            }
        else
            {
            // dynText: "qtn_uni_ind_wlan_avail_unknown_many    WLAN networks found"
            dynText = StringLoader::LoadL (
                    R_QTN_UNI_IND_WLAN_AVAIL_UNKNOWN_MANY, iCoeEnv );            
            }
        }
    else
        {
        // dynText: "qtn_uni_ind_wlan_avail_known        '%U' found"
        dynText = StringLoader::LoadL ( 
                R_QTN_UNI_IND_WLAN_AVAIL_KNOWN, *knownNetworkName, iCoeEnv );
                       
        }
    
    CleanupStack::PopAndDestroy( knownNetworkName );

    return dynText;
    }


// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::CreateWlanConnectedTextL()
// ---------------------------------------------------------------------------
//
HBufC* CWlanIndicatorPluginImplementation::CreateWlanConnectedTextL()
    {
    
    HBufC* dynText = NULL;
    HBufC* connectedNetworkName = NULL;
                  
    connectedNetworkName = ConnectionNameL( );
    
    CleanupStack::PushL( connectedNetworkName );
    
    if ( connectedNetworkName )
        {
        // "qtn_uni_ind_wlan_conn            '%U' connected"
        dynText = StringLoader::LoadL ( 
                R_QTN_UNI_IND_WLAN_CONN, *connectedNetworkName, iCoeEnv );
        }
    
    CleanupStack::PopAndDestroy( connectedNetworkName );
            
    return dynText;
    }       


// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::AvailableNetworksInfoL()
// ---------------------------------------------------------------------------
//
HBufC* CWlanIndicatorPluginImplementation::AvailableNetworksInfoL(
    TInt& aAvailableCount )
    {
    
    HBufC* knownNetworkName = NULL; 
 
    // get available iaps
    RArray<TUint> availableIaps;
    TInt iapError( 0 );
    iapError = iWlanMgmtClient->GetAvailableIaps( availableIaps );
        
    if ( iapError == KErrNone )
        {
        // if there are known networks, get the name of the first one
        // in an alphabetical order
        if ( availableIaps.Count() )
            {
            // get all network names and put them into a sorted array
        
            // use one TWliWlanInfo struct to point to each IAP in a loop
            TWliWlanInfo* availableInfo = new ( ELeave ) TWliWlanInfo();
            CleanupStack::PushL( availableInfo );
        
            // descriptor array with granularity 2
            CDesCArray* networkNames = new (ELeave)CDesCArrayFlat( 2 );
            CleanupStack::PushL( networkNames );
      
            for ( TInt i = 0; i < availableIaps.Count() ; i++ )
                {
                availableInfo->iIapId = availableIaps[i];
                GetWlanInfoFromIapL( *availableInfo );
            
                // insert the name into the proper place
                // in alphabetical order
                networkNames->InsertIsqL(
                    ( availableInfo->iNetworkName ), ECmpFolded );
                }
        
            knownNetworkName = ( networkNames->MdcaPoint(0) ).AllocL();
        
            CleanupStack::PopAndDestroy( networkNames );
            CleanupStack::PopAndDestroy( availableInfo );
               
            }
            
        else // unknown networks only        
            {
            CWlanScanInfo* scanInfo = CWlanScanInfo::NewL();                    

            TInt scanError( 0 );
            scanError = iWlanMgmtClient->GetScanResults( *scanInfo );
            if ( scanError == KErrNone )
                {
                // Find if we have 1 or more networks available,
                // we don't have to know the exact amount
                scanInfo->Next() ? aAvailableCount = 2 : aAvailableCount = 1;
                }
            
            delete scanInfo;
        
            }
        }    
    
    availableIaps.Close();
    
    return knownNetworkName;

    }

// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::GetWlanInfoFromIapL()
// ---------------------------------------------------------------------------
//
void CWlanIndicatorPluginImplementation::GetWlanInfoFromIapL( TWliWlanInfo& aWlanInfo )
    {
    CCommsDatabase* commsDb = CCommsDatabase::NewL();    
    CleanupStack::PushL( commsDb );
    
    CCommsDbTableView* commsDbIapTableView = commsDb->OpenViewMatchingUintLC(
        TPtrC( IAP ), TPtrC( COMMDB_ID ), aWlanInfo.iIapId );

    User::LeaveIfError( commsDbIapTableView->GotoFirstRecord() );

    commsDbIapTableView->ReadTextL( TPtrC( COMMDB_NAME ), aWlanInfo.iNetworkName );
    
    CleanupStack::PopAndDestroy( commsDbIapTableView );
    CleanupStack::PopAndDestroy( commsDb );
    }

// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::ConnectionNameL()
// ---------------------------------------------------------------------------
//
HBufC* CWlanIndicatorPluginImplementation::ConnectionNameL( )
    {
    HBufC* connName = NULL;
    
    // get the connection ID
    TUint connectionId = FindWlanBearerConnectedL();
    
    TUint connectedIapId(0);
    TRequestStatus status;
 
    // get the IAP ID
    iConnMonitor.GetUintAttribute( connectionId, 0, KIAPId,
        connectedIapId, status );
    User::WaitForRequest( status );
 
    // get the connection method from cmmanager
    RCmManager cmManager;
    cmManager.OpenLC();
    
    RCmConnectionMethod connMethod = cmManager.ConnectionMethodL( connectedIapId );  
    CleanupClosePushL( connMethod );
    
    connName = connMethod.GetStringAttributeL( ECmName );
        
    CleanupStack::PopAndDestroy( 2, &cmManager ); //cmManager and connMethod
    
    return connName;
    }

// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::FindWlanBearerConnectedL()
// ---------------------------------------------------------------------------
//
TUint CWlanIndicatorPluginImplementation::FindWlanBearerConnectedL( )
    {
    TUint connectionId( 0 );
    
    TUint connCount( 0 );
    TUint subConnectionCount( 0 );
    TRequestStatus status;
    
    iConnMonitor.GetConnectionCount( connCount, status );
    User::WaitForRequest( status );
    
    if ( status.Int() == KErrNone )
        {    
        for ( TUint i = 1; i <= connCount; ++i )
            {
            User::LeaveIfError( iConnMonitor.GetConnectionInfo( i, connectionId, 
                                                        subConnectionCount ) );
            if ( CheckIfWlan( connectionId ) )
                {
                break;
                }
            }
        }
    
    return connectionId;
    }

// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::CheckIfWlan()
// ---------------------------------------------------------------------------
//
TBool CWlanIndicatorPluginImplementation::CheckIfWlan( TUint aConnectionId )
    {
    TConnMonBearerType bearerType( EBearerUnknown );

    TBool foundWlanBearer( EFalse );
    
    TRequestStatus status;

    // Add only connections with valid id
    if ( aConnectionId > 0 )
        {
        iConnMonitor.GetIntAttribute( aConnectionId, 0, KBearer, (TInt&)bearerType, 
            status );
        User::WaitForRequest( status );        

        if ( status.Int() == KErrNone )
            {
            if ( bearerType == EBearerWLAN )
                {
                foundWlanBearer = ETrue;
                }
            }

        }

    return foundWlanBearer;
    
    }

// ---------------------------------------------------------------------------
// CWlanIndicatorPluginImplementation::LaunchWlanSnifferL()
// ---------------------------------------------------------------------------
//
void CWlanIndicatorPluginImplementation::LaunchWlanSnifferL()
    {

    RApaLsSession appArcSession;

    User::LeaveIfError( appArcSession.Connect() ); // connect to AppArc server
    CleanupClosePushL( appArcSession );

    // check if the app is already running
    TApaTaskList taskList( CEikonEnv::Static()->WsSession() );
    TApaTask task = taskList.FindApp( KSnifferAppUid );
    
    if ( task.Exists() )
        {
        task.BringToForeground();
        }
    else
        {
        TThreadId threadId;
        User::LeaveIfError( appArcSession.StartDocument( KNullDesC, 
                                KSnifferAppUid, threadId ) );
                                
        }
        
    CleanupStack::PopAndDestroy(); // appArcSession
    
    }

//  End of File
