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
* Description:  Implementation of CWsfWlanScanner
*
*/



//  EXTERNAL INCLUDES
#include <wlanmgmtclient.h>
#include <centralrepository.h>
#include <utf.h>
#include <wlancontainer.h>
#include <commsdattypeinfov1_1.h>
#include <wlanscaninfo.h>
#include <cmdestinationext.h>
#include <commdb.h>

#ifdef __WINS__
#include <e32math.h>
#endif

//  CLASS HEADER
#include "wsfwlanscanner.h"

//  INTERNAL INCLUDES
#include "wsfwlaninfoarray.h"
#include "wsfwlanscaninfodefines.h"
#include "wsfwlanconnectiondetailsprovider.h"
#include "wsfwlansettingsaccessor.h"

#include "wsflogger.h"

//  LOCAL DEFINITIONS
using namespace CMManager;


/**
* Microseconds in a second 
*/
static const TInt KMicrosecPerSecond = 1000 * 1000;


//  CONSTRUCTION AND DESTRUCTION

// ---------------------------------------------------------------------------
// CWsfWlanScanner::NewL
// ---------------------------------------------------------------------------
//    
CWsfWlanScanner* CWsfWlanScanner::NewL( CommsDat::CMDBSession& aDbSession )
    {
    CWsfWlanScanner* self = CWsfWlanScanner::NewLC( aDbSession );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::NewLC
// ---------------------------------------------------------------------------
//    
CWsfWlanScanner* CWsfWlanScanner::NewLC( CommsDat::CMDBSession& aDbSession )
    {
    CWsfWlanScanner* self = new( ELeave ) CWsfWlanScanner( aDbSession );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::~CWsfWlanScanner
// ---------------------------------------------------------------------------
//    
CWsfWlanScanner::~CWsfWlanScanner()
    {
    LOG_ENTERFN( "CWsfWlanScanner::~CWsfWlanScanner" );
    StopScanning();
    Cancel();
    iTimer.Close();
    iCmManagerExt.Close();
    delete iActiveConnectionName;
    delete iScanResults;
    delete iWlanMgmtClient;
    delete iWlanSettingsAccessor;
    delete iScanInfo;
    delete iScanArray;
    iDirectScanSsids.Close();
    iDirectScanIapIDs.Close();
    iAvailableIaps.Close();
    iDbSession = NULL; // not owning
    iConnectionDetailsProvider = NULL;
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::CWsfWlanScanner
// ---------------------------------------------------------------------------
//    
CWsfWlanScanner::CWsfWlanScanner( CommsDat::CMDBSession& aDbSession ): 
    CActive( EPriorityStandard ),
    iDbSession( &aDbSession ),
    iScanState( EIdle )
    {
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::ConstructL
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::ConstructL()
    {
    LOG_ENTERFN( "CWsfWlanScanner::ConstructL" );
    
#ifndef __WINS__ // client is not available on wins
    iWlanMgmtClient = CWlanMgmtClient::NewL();

    iScanInfo = CWlanScanInfo::NewL();
#endif

    iCmManagerExt.OpenL();
    
    iScanArray = CWsfWlanInfoArray::NewL();
    
    User::LeaveIfError( iTimer.CreateLocal() );
    iScanResults = KNullDesC8().AllocL();

    iWlanSettingsAccessor = CWsfWlanSettingsAccessor::NewL( *iDbSession );
    iScanningInterval = iWlanSettingsAccessor->ScanInterval() * 
                                                          KMicrosecPerSecond;
    iShowAvailability = iWlanSettingsAccessor->ShowAvailability();
    
    LOG_WRITEF( "initial bgScanInterval = %d sec", 
                iWlanSettingsAccessor->ScanInterval() );
    LOG_WRITEF( "initial showAvailability = %d", iShowAvailability );
    
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::DoCancel
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::DoCancel()
    {
    LOG_ENTERFN( "CWsfWlanScanner::DoCancel" );
    iTimer.Cancel();
#ifndef __WINS__
    iWlanMgmtClient->CancelGetScanResults();
#endif    
    iScanState = EIdle;    
    }
 

// ---------------------------------------------------------------------------
// CWsfWlanScanner::RunL
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::RunL()
    {
    LOG_ENTERFN( "CWsfWlanScanner::RunL" );
    
    /*
     * Scan logic
     * 1. Get available IAPs - state = EIdle
     * 2. Do broadcast scan - state = EIdle
     * 3. Process broadcast scan results - state = EBroadcastScan
     * 4. Do direct scans for remaining known networks
     *    from step 2. Get available IAPs - state = EDirectScan
     * 5. Add connected network - state = EFinished
     * 6. Set names and priorities for known networks - state = EFinished
     */

    if ( iScanState == EIdle )
        {
        LOG_WRITE( "broadcast scan phase" );

        // prepare things for direct scans
        PrepareDirectScan();
          
        // notify clients
        if ( iObserver )
            {
            iObserver->WlanScanStarted();
            }

        
#ifndef __WINS__
        // get available iaps
        // (this only shows iaps with security mode matching to scan results
        // and  also finds hidden wlans for which an iap has been configured)
        iAvailableIaps.Reset();
        iWlanMgmtClient->GetAvailableIaps( iAvailableIaps );
        
		// do broadcast scan
        iWlanMgmtClient->GetScanResults( iStatus, *iScanInfo );
        SetActive();
#else
        // for testing
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
#endif     
        
        iScanState = EBroadcastScan;
        }
        
    else if ( iScanState == EBroadcastScan )
        {
        // process broadcast scan results
        DoScanForNetworksL();
        
        // now it's time to initiate direct scan 
        // for remaining known networks
        if ( iDirectScanSsids.Count() )
            {
#ifdef _DEBUG
            HBufC* ssid = TWsfWlanInfo::GetSsidAsUnicodeLC( 
                                                        iDirectScanSsids[0] );
            LOG_WRITEF( "requesting direct scan for [%S]", ssid );
            CleanupStack::PopAndDestroy( ssid );
#endif
            
#ifndef __WINS__
            // run direct scan
            iWlanMgmtClient->GetScanResults( iDirectScanSsids[0], 
                                             iStatus, *iScanInfo );
            SetActive();
#else
            // for testing
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, KErrNone );
#endif                                             

            iScanState = EDirectScan;
            }
        else
            {
            LOG_WRITE( "direct scan skipped" );
            iScanState = EFinished;
            }
        }
        
    else if ( iScanState == EDirectScan )
        {
        LOG_WRITE( "direct scan results" );

#ifndef __WINS__
        ProcessDirectScanResultL();
#endif
 
        // delete the processed item (first)
        iDirectScanSsids.Remove( 0 );
        iDirectScanIapIDs.Remove( 0 );

        if ( iDirectScanSsids.Count() )
            {
            // repeated direct scans for known networks
#ifdef _DEBUG
            HBufC* ssid = TWsfWlanInfo::GetSsidAsUnicodeLC( 
                                                        iDirectScanSsids[0] );
            LOG_WRITEF( "requesting direct scan for [%S]", ssid );
            CleanupStack::PopAndDestroy( ssid );
#endif
            
#ifndef __WINS__
            // run direct scan
            iWlanMgmtClient->GetScanResults( iDirectScanSsids[0], 
                                             iStatus, *iScanInfo );        
            SetActive();
#else
            // for testing
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, KErrNone );
#endif                                             
            }
        else
            {
            // there is nothing more to do
            iScanState = EFinished;
            }
        }
    
    if ( iScanState == EFinished )
        {
        LOG_WRITE( "scanning finished" );
#ifndef __WINS__
        AddConnectedWLANInfoL();
        
        TWsfWlanInfo* info(NULL);
        TInt scanArrayCount( iScanArray->Count() );
        
        for ( TInt i(0) ; i < scanArrayCount ; i++ )
            {  
            info = (*iScanArray)[ i ];
            
            if( info->iIapId )
                {
                TRAPD( error, UpdatePriorityL( *info ) );
                if ( error )
                    {
                    // Ignore error and just continue
                    LOG_WRITE( "Prio update failed" );
                    }
                }
            
            if( info->iIapId && info->iConnectionState != EConnected )
                {
                ReplaceSsidsWithIapName( *info );
                }
            }
#endif //_WINS_
        
        // sort the array
        iScanArray->SortArrayL();
        
        HBufC8* results = iScanArray->SerializeContentLC();
        CleanupStack::Pop( results );
        delete iScanResults;
        iScanResults = results;

#ifdef _DEBUG
        DumpScanResultsL( iScanArray );
#endif
        
        // we may let go the scan array contents...
        iScanArray->Reset();
    
        // notify clients that data is ready
        if ( iObserver )
            {
            iObserver->WlanScanCompleteL();
            }

        if ( !iShowAvailability )
            {
            // reset the timer if we are responsible for scheduling scans
            iTimer.Cancel();
            iTimer.After( iStatus, 
                          TTimeIntervalMicroSeconds32( iScanningInterval ) );
            Cancel();
            SetActive();
            }
        
        iScanState = EIdle;
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::AddConnectedWLANInfo
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::AddConnectedWLANInfoL()
    {
    LOG_ENTERFN( "CWsfWlanScanner::AddConnectedWLANInfoL" );
    // Get the possible connection
    if ( iConnectionDetailsProvider )
        {
        TWlanSsid connectedSsid;
        TInt error = iWlanMgmtClient->GetConnectionSsid( connectedSsid );
        
        LOG_WRITEF( "GetConnectionSsid returned=%d", error );
        
        TBool connected = ( error == KErrNone && connectedSsid.Length() );
    
        if ( !connected && !iConnectionDetailsProvider->IsConnected() )
            {
            LOG_WRITEF( "Not connected [connected=%d]", connected );
            return;
            }
        
        TWsfWlanInfo* connectedInfo = new (ELeave) TWsfWlanInfo();
        CleanupStack::PushL( connectedInfo );
        
        if ( iConnectionDetailsProvider->ConnectedWlanConnectionDetailsL(
                connectedInfo) )
            {
            TBuf8<KWlanMaxAccessPointNameLength> connectedSsidOrIap;
            connectedSsidOrIap.Copy( connectedInfo->iSsid );
            // ConnectedWlanConnectionDetailsL() may have returned an IAP name instead of SSID.
            // make sure that we really have SSID in connectedInfo->iSSID at this phase.
            
            connectedInfo->iRawSsid.Copy( connectedSsid );
            connectedInfo->iSsid.Copy( connectedSsid );
            connectedInfo->iCoverage = 0;
            connectedInfo->iVisibility = 1;

            TBool connectedInfoAppended = EFalse;

            RPointerArray<TWsfWlanInfo> matchArray;
            CleanupClosePushL( matchArray );

            if ( connectedInfo->iIapId )
                {
                LOG_WRITEF( "Connected iap [%d]", connectedInfo->iIapId );
                iScanArray->MatchWithIapIDL( connectedInfo->iIapId,
                        iScanArray->Count(), matchArray );
                
                if ( !matchArray.Count() )
                    {
                    LOG_WRITE( "Not found with Iap id" );
                    iScanArray->MatchL( connectedInfo->iSsid,
                                        connectedInfo->iSecurityMode,
                                        connectedInfo->iNetMode, 
                                        connectedInfo->UsesPreSharedKey(),
                                        iScanArray->Count(),
                                        matchArray );
                    }
                
                }
            else
                {
                LOG_WRITE( "Easy WLAN connection" );
                iScanArray->MatchL( connectedInfo->iSsid,
                        connectedInfo->iSecurityMode,
                        connectedInfo->iNetMode, 
                        connectedInfo->UsesPreSharedKey(),
                        iScanArray->Count(),
                        matchArray );
                }

            if ( matchArray.Count() )
                {
                LOG_WRITE( "Info found" );
                TWsfWlanInfo* temp = matchArray[0];
                temp->iConnectionState = EConnected;
                temp->iSsid.Copy( connectedSsidOrIap );
                temp->iNetworkName.Zero();
                }
            else
                {
                LOG_WRITE( "Info not found - append" );
                connectedInfo->iSsid.Copy( connectedSsidOrIap );
                iScanArray->AppendL( connectedInfo );
                connectedInfoAppended = ETrue;
                }

            CleanupStack::PopAndDestroy( &matchArray );
            if ( connectedInfoAppended )
                {
                CleanupStack::Pop( connectedInfo );
                }
            else
                {
                CleanupStack::PopAndDestroy( connectedInfo );
                }
            }
        else
            {
            CleanupStack::PopAndDestroy( connectedInfo );
            }
        }
    else
        {
        LOG_WRITE( "No connection details provider" );
        }
    }


#ifdef _DEBUG
// ---------------------------------------------------------------------------
// CWsfWlanScanner::DumpScanResultsL
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::DumpScanResultsL( CWsfWlanInfoArray* aArray )
    {
    LOG_ENTERFN( "CWsfWlanScanner::DumpScanResultsL" );
    _LIT( Kopen, "open" );
    _LIT( Kwep, "wep" );
    _LIT( Kwpa, "wpa" );
    _LIT( Kwpa2, "wpa2" );        
    _LIT( K802, "802.1x" );
    _LIT( KWAPI, "WAPI" );
    _LIT( KUnknown, "Unknown" );

    _LIT( Kpsk, "psk" );
    _LIT( Keap, "eap" );
    _LIT( Khidden, "<hidden>" );

    for ( TInt i( 0 ); i < aArray->Count(); ++i )
        {
        TWsfWlanInfo* wi( (*aArray)[i] );
        const TDesC* sm( 0 );

        switch ( wi->iSecurityMode )
            {
            case EWlanSecModeOpen:
                sm = &Kopen;
                break;
            case EWlanSecModeWep:
                sm = &Kwep;
                break;
            case EWlanSecModeWpa:
                sm = &Kwpa;
                break;
            case EWlanSecModeWpa2:
                sm = &Kwpa2;
                break;
            case EWlanSecMode802_1x:
                sm = &K802;
                break;
            case EWlanSecModeWAPI:
                sm = &KWAPI;
                break;
            default:
                sm = &KUnknown;
                break;
            }
        
        const TDesC* psk = wi->UsesPreSharedKey()? &Kpsk:             
                               ( ( wi->iSecurityMode == EWlanSecMode802_1x || 
                                   wi->iSecurityMode == EWlanSecModeWpa ||
                                   wi->iSecurityMode == EWlanSecModeWpa2 )? 
                                      &Keap: &KNullDesC );
        HBufC16 *ssid = TWsfWlanInfo::GetSsidAsUnicodeLC( wi->iSsid );
        
        LOG_WRITEF( "[%S] %S %S %S", ssid, sm, psk, 
                    wi->iVisibility? &KNullDesC: &Khidden );
        CleanupStack::PopAndDestroy( ssid );
        }
    
    }
#endif // _DEBUG


// ---------------------------------------------------------------------------
// CWsfWlanScanner::RunError
// ---------------------------------------------------------------------------
//    
TInt CWsfWlanScanner::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfWlanScanner::RunError" );
    LOG_WRITEF( "error = %d", aError );
    
    iScanArray->Reset();
    
    if ( iObserver && aError != KErrNotReady )
        {
        // KErrNotReady is excluded as it indicates that the WLAN engine
        // has not yet set up itself, which we cannot help
        iObserver->NotifyError( aError );    
        }

    iScanState = EIdle;
    
    if ( !iShowAvailability )
        {
        // the scanning has failed, re-issue the scan timer
        iTimer.Cancel();
        iTimer.After( iStatus, 
                      TTimeIntervalMicroSeconds32( iScanningInterval ) );
        SetActive();    
        }
        
    return KErrNone;    
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::SetObserver
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::SetObserver( MWsfWlanScannerObserver& aObserver )
    {
    iObserver = &aObserver;
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::StartScanningL
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::StartScanningL()
    {
    LOG_ENTERFN( "CWsfWlanScanner::StartScanningL" );

    if ( iScanState == EIdle && !IsActive() )
        {
        LOG_WRITE( "request notifications" );
#ifndef __WINS__
        iWlanMgmtClient->CancelNotifications();
        iWlanMgmtClient->ActivateNotificationsL( *this );
#endif
        iWlanSettingsAccessor->RequestNotificationL( *this );
        
        iScanState = EIdle;

        if ( !iShowAvailability )
            {
            // in case show wlan availability is off, carry out a scan now
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, KErrNone );
            }
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::StartScanningL
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::StopScanning()
    {
    LOG_ENTERFN( "CWsfWlanScanner::StopScanning" );

    Cancel();

#ifndef __WINS__       
    if ( iWlanMgmtClient )
        {
        iWlanMgmtClient->CancelNotifications();
        }
#endif
  
    if ( iWlanSettingsAccessor )
        {
        iWlanSettingsAccessor->CancelNotifications();
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::RestartScanning
// ---------------------------------------------------------------------------
//    
TBool CWsfWlanScanner::RestartScanning()
    {
    LOG_ENTERFN( "CWsfWlanScanner::RestartScanning" );

    TBool restarted( EFalse );
    LOG_WRITEF( "iScanState = %d isActive = %d iShowAvailability = %d",
               iScanState, IsActive(), iShowAvailability );
    
    if ( iScanState == EIdle && ( IsActive() || iShowAvailability ) )
        {
        // we have been waiting for the timer to complete
        // cancel it manually
        Cancel();
        
        // then complete ourselves
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        restarted = ETrue;
        }
    
    LOG_WRITEF( "restarted = %d", restarted );
    return restarted;
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::AbortScanning
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::AbortScanning()
    {
    LOG_ENTERFN( "CWsfWlanScanner::AbortScanning" );
    
    if ( iScanState != EIdle )
        {
        // cancel the current scanning
        Cancel();

        if ( !iShowAvailability )
            {
            // still, life goes on
            iTimer.After( iStatus, TTimeIntervalMicroSeconds32( 
                                                        iScanningInterval ) );
            SetActive();        
            }
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::ConnectionStateChanged
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::ConnectionStateChanged( 
                                            TWlanConnectionMode /*aNewState*/ )
    {
    LOG_ENTERFN( "CWsfWlanScanner::ConnectionStateChanged" );
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanScanner::BssidChanged
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::BssidChanged( TWlanBssid& /*aNewBSsid*/ )
    {
    LOG_ENTERFN( "CWsfWlanScanner::BssidChanged" );
    if ( iScanState == EIdle && !IsActive() )
        {
        // complete ourselves
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::BssLost
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::BssLost()
    {
    LOG_ENTERFN( "CWsfWlanScanner::BssLost" );
    if ( iScanState == EIdle && !IsActive() )
        {
        // complete ourselves
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::BssRegained
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::BssRegained()
    {
    LOG_ENTERFN( "CWsfWlanScanner::BssRegained" );
    if ( iScanState == EIdle && !IsActive() )
        {
        // complete ourselves
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::NewNetworksDetected
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::NewNetworksDetected()
    {
    LOG_ENTERFN( "CWsfWlanScanner::NewNetworksDetected" );
    if ( iScanState == EIdle && !IsActive() )
        {
        // complete ourselves
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::OldNetworksLost
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::OldNetworksLost()
    {
    LOG_ENTERFN( "CWsfWlanScanner::OldNetworksLost" );
    if ( iScanState == EIdle && !IsActive() )
        {
        // complete ourselves
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::TransmitPowerChanged
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::TransmitPowerChanged( TUint /*aPower*/ )
    {
    LOG_ENTERFN( "CWsfWlanScanner::TransmitPowerChanged" );
    if ( iScanState == EIdle && !IsActive() )
        {
        // complete ourselves
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }
        

// ---------------------------------------------------------------------------
// CWsfWlanScanner::RssChanged
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::RssChanged( TWlanRssClass /*aRssClass*/, TUint /*aRss*/ )
    {
    LOG_ENTERFN( "CWsfWlanScanner::RssChanged" );
    if ( iScanState == EIdle && !IsActive() )
        {
        // complete ourselves
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::PrepareDirectScan
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::PrepareDirectScan()
    {
    LOG_ENTERFN( "CWsfWlanScanner::PrepareDirectScanL" );

    // flush the arrays
    iDirectScanSsids.Reset();
    iDirectScanIapIDs.Reset();
    }
    
    
// ---------------------------------------------------------------------------
// CWsfWlanScanner::ProcessDirectScanResultL
// ---------------------------------------------------------------------------
//   
void CWsfWlanScanner::ProcessDirectScanResultL()
    {
    LOG_ENTERFN( "CWsfWlanScanner::ProcessDirectScanResultL" );
    
    for ( iScanInfo->First(); !iScanInfo->IsDone(); iScanInfo->Next() )
        {
        RPointerArray<TWsfWlanInfo> matchArray;
        CleanupClosePushL(matchArray);
        iScanArray->MatchWithIapIDL( iDirectScanIapIDs[0], iScanArray->Count(), matchArray );
  
        TInt matchcount = matchArray.Count();

        if ( matchcount == 0 )
            {
            // not found yet
#ifdef _DEBUG
            HBufC* ssid = TWsfWlanInfo::GetSsidAsUnicodeLC( iDirectScanSsids[0] );
            LOG_WRITEF( "[%S] found in direct scan", ssid ); 
            CleanupStack::PopAndDestroy( ssid );
#endif       
            }
        else
            {
            LOG_WRITEF( "[%d] iap id is in scanarray", iDirectScanIapIDs[0] ); 
            for( TInt i(0); i < matchcount; i++ )
            	{
            	//if already found increase coverage
                TWsfWlanInfo* temp = matchArray[i];
                ++temp->iCoverage;
                RefreshSignalStrength( *temp );
                RefreshTechnology( *temp );
             	}
            }
        // Close() for matchArray
        CleanupStack::PopAndDestroy( &matchArray );  
        } // for iScanInfo              
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::ReplaceSsidsWithIapName
// ---------------------------------------------------------------------------
//   
void CWsfWlanScanner::ReplaceSsidsWithIapName(TWsfWlanInfo& aWlanInfo)
    {	
    LOG_ENTERFN( "CWsfWlanScanner::ReplaceSsidsWithIapName" );
    if ( aWlanInfo.iNetworkName.Length() )
        {
        LOG_WRITE( "Replace ssid" );
        aWlanInfo.iSsid.Copy( aWlanInfo.iNetworkName );
        aWlanInfo.iNetworkName.Zero();
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::UpdatePriorityL
// ---------------------------------------------------------------------------
//    
void CWsfWlanScanner::UpdatePriorityL( TWsfWlanInfo& aWlanInfo )
    {
    LOG_WRITE( "CWsfWlanScanner::updatePriority" );

    TUint32 priority( 0 );
    TInt exists( KErrNotFound );
    TInt count( 0 );   

    // search for the destination of it                                                        
    RArray<TUint32> destinations;                                                              
    CleanupClosePushL( destinations );

    LOG_WRITE( "CWsfWlanScanner::updatePriority get all destinations" );
    iCmManagerExt.AllDestinationsL(destinations);                                              

    count = destinations.Count();                                                         
    LOG_WRITEF( "destination count %d", count);

    for( TInt i = 0; i < count && exists != KErrNone; i++ )                                    
        {
        RCmDestinationExt destination;                                                              
        destination = iCmManagerExt.DestinationL( destinations[ i ] );                         
        CleanupClosePushL( destination );  

        LOG_WRITE( "check if connection method belongs to destination" );

        RCmConnectionMethodExt connectionMethod;
        TRAP( exists,                                                                      
                connectionMethod = destination.ConnectionMethodByIDL(                            
                        aWlanInfo.iIapId ) ); 

        LOG_WRITEF( "exists %d", exists );
        if( exists == KErrNone )                                                           
            {   
            CleanupClosePushL( connectionMethod ); 
            // correct destination found                                                   
            priority = destination.PriorityL( connectionMethod );
            aWlanInfo.SetPriority( priority );

            LOG_WRITEF( "priority %d", priority );
            CleanupStack::PopAndDestroy( &connectionMethod ); 
            }
        CleanupStack::PopAndDestroy( &destination );                                           
        }                                                                                      
    CleanupStack::PopAndDestroy( &destinations );
    }


#ifndef __WINS__
// ---------------------------------------------------------------------------
// CWsfWlanScanner::DoScanForNetworksL
// ---------------------------------------------------------------------------
//   
void CWsfWlanScanner::DoScanForNetworksL()
    {
    LOG_ENTERFN( "CWsfWlanScanner::DoScanForNetworksL" );
        
    // start by making sure the scan array is empty
    iScanArray->Reset();    

    LOG_WRITEF( "GetScanResults returned %d", iStatus.Int() );
    
    if ( iStatus.Int() )
        {
        // if the status is not KErrNone, we cannot be sure that iScanInfo
        // doesn't cause a crash, so it's better to leave
        User::Leave( iStatus.Int() );
        }
   
    TInt nElem = 0;
    TBool isHidden( EFalse );

    TInt avIapCount = iAvailableIaps.Count();

    LOG_WRITEF( "Available iap count %d", avIapCount ); 

    for( TInt i(0); i < avIapCount; i++ )
        {
        TBool addToArray( ETrue ); 
        TWsfWlanInfo* availableInfo = new ( ELeave ) TWsfWlanInfo();       
        CleanupStack::PushL( availableInfo );
        availableInfo->iIapId = iAvailableIaps[i];
        TRAPD( error, GetWlanInfoFromIapL( *availableInfo ) );

        if ( error == KErrNotFound )
            {
            LOG_WRITEF( "Iap id = %d does not exist", availableInfo->iIapId );
            addToArray = EFalse;
            }
        else if ( error )
            {
            LOG_WRITEF( "GetWlanInfoFromIapL failed err = %d", error );
            User::Leave( error );
            }

        if( addToArray )
            { 
            LOG_WRITE( "Add to array" );
            availableInfo->iCoverage = 0;
            availableInfo->iVisibility = 1;
            availableInfo->iStrengthLevel = EWlanSignalUnavailable;
            availableInfo->iTransferRate = 0;
            availableInfo->iConnectionState = ENotConnected;
            iScanArray->AppendL(availableInfo);
            nElem++;

            if ( availableInfo->iIapId )
                {
                LOG_WRITEF( "Append available iap [%d] for direct scan", availableInfo->iIapId );
                iDirectScanIapIDs.Append( availableInfo->iIapId );
                iDirectScanSsids.Append( availableInfo->iSsid );
                }

            CleanupStack::Pop( availableInfo );
            }
        else
            {
            LOG_WRITE( "Info not added" );
            CleanupStack::PopAndDestroy( availableInfo );
            }
        }
      
    // Process the scanned results
    for( iScanInfo->First(); !iScanInfo->IsDone(); iScanInfo->Next() )
        {
        TWsfWlanInfo* wlanInfo = iScanArray->At( nElem );
        if ( !wlanInfo )
            {
            wlanInfo = new ( ELeave ) TWsfWlanInfo();
            CleanupStack::PushL( wlanInfo );
            iScanArray->AppendL( wlanInfo );
            CleanupStack::Pop( wlanInfo );
            }
        isHidden = RefreshNetworkNameL( *wlanInfo );
        wlanInfo->iVisibility = !isHidden;
        wlanInfo->iStrengthLevel = EWlanSignalUnavailable;
        wlanInfo->iTransferRate = 0;
        wlanInfo->iConnectionState = ENotConnected;

        if( !isHidden )
            {
            // not hidden
            RefreshNetworkMode( *wlanInfo );
            RefreshSecurityMode( *wlanInfo );
            RefreshTechnology( *wlanInfo );

            // check if we already have an entry/entries corresponding to a scan result
            // (multiple entries for one scan result possible if GetAvailableIaps()
            // found several iaps configured for same wlan)
            RPointerArray<TWsfWlanInfo> matchArray;
            CleanupClosePushL(matchArray);
            iScanArray->MatchL( wlanInfo->iSsid, 
                                wlanInfo->iSecurityMode, 
                                wlanInfo->iNetMode, 
                                wlanInfo->UsesPreSharedKey(), 
                                nElem, 
                                matchArray );

            TInt matchcount = matchArray.Count();

            // if not found
            if( matchcount == 0 )
                {
                wlanInfo->iCoverage = 1;
                RefreshSignalStrength( *wlanInfo );
                RefreshTechnology( *wlanInfo );
                ++nElem; // new entry, inc index in array
                }
            else // if found inc coverage and refresh signal strength and rate
                {
                for( TInt i(0); i < matchcount; i++ )
                    {
                    TWsfWlanInfo* temp = matchArray[i];
                    ++temp->iCoverage;
                    RefreshSignalStrength( *temp );
                    RefreshTechnology( *temp );

                    if ( temp->iIapId )
                        {
                        TInt index( KErrNone );
                        do {
                            LOG_WRITE( "Not hidden - Searching from direct scan list.." );
                            // remove this item from the direct scan list, if found 
                            index = iDirectScanIapIDs.Find( temp->iIapId  );
                            if ( index != KErrNotFound )
                                {
                                LOG_WRITEF( "Found - removing iap id [%d]", iDirectScanIapIDs[index] );
                                iDirectScanSsids.Remove( index );
                                iDirectScanIapIDs.Remove( index );
                                }
                        } while ( index != KErrNotFound );
                        }
                    }
                }
            CleanupStack::PopAndDestroy(); // results in Close() being called on matchArray
            }
        }
    //get rid of excess items
    iScanArray->DeleteFromTail(iScanArray->Count() - nElem);
    }

#else // __WINS__

// ---------------------------------------------------------------------------
// CWsfWlanScanner::DoScanForNetworksL
// ---------------------------------------------------------------------------
//   
void CWsfWlanScanner::DoScanForNetworksL()
    {
    LOG_ENTERFN( "CWsfWlanScanner::DoScanForNetworksL" );
        
    // start by making sure the scan array is empty
    iScanArray->Reset();    

    TWsfWlanInfo* wlan0 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan0 );    
    wlan0->iConnectionState = EConnected;
    wlan0->iIapId = 666;
    wlan0->iNetMode = EInfra;
    wlan0->iSecurityMode = EWlanSecModeWep;
    wlan0->iSsid = _L8("[C]Known WEP");
    wlan0->iStrengthLevel = EWlanSignalStrengthMax;  
    wlan0->iVisibility = ETrue;
    wlan0->iCoverage = 1;
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan0 );
        }
    else 
        {
        delete wlan0;
        }
        
    CleanupStack::Pop( wlan0 );

    TWsfWlanInfo* wlan1 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan1 );    
    wlan1->iConnectionState = ENotConnected;
    wlan1->iIapId = 666;
    wlan1->iNetMode = EInfra;
    wlan1->iSecurityMode = EWlanSecModeOpen;
    wlan1->iSsid = _L8("Known open");
    wlan1->iStrengthLevel = EWlanSignalStrengthMin;  
    wlan1->iVisibility = ETrue;
    wlan1->iCoverage = 3;
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan1 );
        }
    else
        {
        delete wlan1;
        }

    CleanupStack::Pop( wlan1 );

    TWsfWlanInfo* wlan2 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan2 );    
    wlan2->iConnectionState = ENotConnected;
    wlan2->iIapId = 0;
    wlan2->iNetMode = EInfra;
    wlan2->iSecurityMode = EWlanSecModeWpa2;
    wlan2->iSsid = _L8("Known WPA2 PSK");
    wlan2->iStrengthLevel = EWlanSignalStrengthLow-7;  
    wlan2->iVisibility = ETrue;
    wlan2->iCoverage = 1;
    wlan2->SetUsesPreSharedKey( ETrue );
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan2 );
        }
    else
        {
        delete wlan2;
        }
    CleanupStack::Pop( wlan2 );

    TWsfWlanInfo* wlan3 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan3 );    
    wlan3->iConnectionState = ENotConnected;
    wlan3->iIapId = 0;
    wlan3->iNetMode = EInfra;
    wlan3->iSecurityMode = EWlanSecModeOpen;
    wlan3->iSsid = _L8("Unknown open");
    wlan3->iStrengthLevel = EWlanSignalStrengthMax;  
    wlan3->iVisibility = 1;
    wlan3->iCoverage = 1;    
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan3 );
        }
    else
        {
        delete wlan3;
        }
    CleanupStack::Pop( wlan3 );
    
    TWsfWlanInfo* wlan4 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan4 );    
    wlan4->iConnectionState = ENotConnected;
    wlan4->iIapId = 0;
    wlan4->iNetMode = EAdhoc;
    wlan4->iSecurityMode = EWlanSecModeWpa;
    wlan4->iSsid = _L8("Unknown WPA");
    wlan4->iStrengthLevel = EWlanSignalStrengthMin;  
    wlan4->iVisibility = 1;
    wlan4->iCoverage = 1;
    wlan2->SetUsesPreSharedKey( ETrue );

    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan4 );    
        }
    else
        {
        delete wlan4;
        }
    CleanupStack::Pop( wlan4 );

    TWsfWlanInfo* wlan5 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan5 );    
    wlan5->iConnectionState = ENotConnected;
    wlan5->iIapId = 12;
    wlan5->iNetMode = EInfra;
    wlan5->iSecurityMode = EWlanSecModeOpen;
    wlan5->iSsid = _L8("SES");
    wlan5->iStrengthLevel = EWlanSignalStrengthLow-5;  
    wlan5->iVisibility = 0;
    wlan5->iCoverage = 1;    
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan5 );
        }
    else
        {
        delete wlan5;
        }
    
    CleanupStack::Pop( wlan5 );
    
    TWsfWlanInfo* wlan6 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan6 );    
    wlan6->iConnectionState = ENotConnected;
    wlan6->iIapId = 666;
    wlan6->iNetMode = EInfra;
    wlan6->iSecurityMode = EWlanSecModeWpa;
    wlan6->iSsid = _L8("Sunny 22");
    wlan6->iStrengthLevel = EWlanSignalStrengthMin;  
    wlan6->iVisibility = 1;
    wlan6->iCoverage = 2;    
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan6 );
        }
    else
        {
        delete wlan6;
        }
    CleanupStack::Pop( wlan6 );


    wlan5 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan5 );    
    wlan5->iConnectionState = ENotConnected;
    wlan5->iIapId = 0;
    wlan5->iNetMode = EInfra;
    wlan5->iSecurityMode = EWlanSecModeOpen;
    wlan5->iSsid = _L8("FON_accesspoint");
    wlan5->iStrengthLevel = EWlanSignalStrengthLow-8;  
    wlan5->iVisibility = 1;
    wlan5->iCoverage = 1;    
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan5 );
        }
    else
        {
        delete wlan5;
        }
    CleanupStack::Pop( wlan5 );
    
    
    TWsfWlanInfo* wlan7 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan7 );    
    wlan7->iConnectionState = ENotConnected;
    wlan7->iIapId = 667;
    wlan7->iNetMode = EAdhoc;
    wlan7->iSecurityMode = EWlanSecModeWpa;
    wlan7->iSsid = _L8("Ad-hoc WPA");
    wlan7->iStrengthLevel = EWlanSignalStrengthMax;  
    wlan7->iVisibility = ETrue;
    wlan7->iCoverage = 1;    
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan7 );
        }
    else
        {
        delete wlan7;
        }
    CleanupStack::Pop( wlan7 );
    
    TWsfWlanInfo* wlan8 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan8 );    
    wlan8->iConnectionState = ENotConnected;
    wlan8->iIapId = 667;
    wlan8->iNetMode = EInfra;
    wlan8->iSecurityMode = EWlanSecModeOpen;
    wlan8->iSsid = _L8("Known pri 1");
    wlan8->iStrengthLevel = EWlanSignalStrengthMax;  
    wlan8->iVisibility = ETrue;
    wlan8->iCoverage = 1;   
    wlan8->iPriority = 1;
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan8 );
        }
    else
        {
        delete wlan8;
        }
    CleanupStack::Pop( wlan8 );
    
    TWsfWlanInfo* wlan9 = new (ELeave) TWsfWlanInfo();
    CleanupStack::PushL( wlan9 );    
    wlan9->iConnectionState = ENotConnected;
    wlan9->iIapId = 668;
    wlan9->iNetMode = EInfra;
    wlan9->iSecurityMode = EWlanSecModeOpen;
    wlan9->iSsid = _L8("Known pri 2");
    wlan9->iStrengthLevel = EWlanSignalStrengthMax;  
    wlan9->iVisibility = ETrue;
    wlan9->iCoverage = 1;  
    wlan9->iPriority = 2;
    if ( Math::Random() % 2 == 0 )
        {
        iScanArray->AppendL( wlan9 );
        }
    else
        {
        delete wlan9;
        }
    CleanupStack::Pop( wlan9 );
    

    }

#endif // __WINS__

// ---------------------------------------------------------------------------
// CWsfWlanScanner::GetWlanInfoFromIapL()
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::GetWlanInfoFromIapL( TWsfWlanInfo& aWlanInfo )
    {
    LOG_ENTERFN( "CWsfWlanScanner::GetWlanInfoFromIapL" );

    CCommsDatabase* commsDb = CCommsDatabase::NewL();    
    CleanupStack::PushL( commsDb );
    
    CCommsDbTableView* commsDbIapTableView = commsDb->OpenViewMatchingUintLC(
        TPtrC( IAP ), TPtrC( COMMDB_ID ), aWlanInfo.iIapId );
    User::LeaveIfError( commsDbIapTableView->GotoFirstRecord() );
    
    // network name
    TBuf<KCommsDbSvrMaxFieldLength> iapName;
    commsDbIapTableView->ReadTextL( TPtrC( COMMDB_NAME ), iapName);

    TInt error = CnvUtfConverter::ConvertFromUnicodeToUtf8( 
                                                        aWlanInfo.iNetworkName, 
                                                        iapName  );
    if ( error )
        {
        LOG_WRITE( "ConvertFromUnicodeToUtf8 failed");
        aWlanInfo.iNetworkName.Copy( iapName );
        }
    
	// service Id
    TUint32 serviceId(0);
    commsDbIapTableView->ReadUintL(TPtrC( IAP_SERVICE), serviceId);
    CCommsDbTableView* wlanTableView = commsDb->OpenViewMatchingUintLC(
             TPtrC( WLAN_SERVICE), TPtrC( WLAN_SERVICE_ID), serviceId);
    User::LeaveIfError(wlanTableView->GotoFirstRecord() );

    // ssid
    wlanTableView->ReadTextL( TPtrC( NU_WLAN_SSID ), aWlanInfo.iSsid );
    aWlanInfo.iRawSsid.Copy( aWlanInfo.iSsid );
    
    // security mode
    TUint32 secMode(0);
    wlanTableView->ReadUintL(TPtrC( WLAN_SECURITY_MODE), secMode);
    // Map Wpa2 to Wpa
    secMode = ( secMode == EWlanSecModeWpa2 )? EWlanSecModeWpa : secMode;
    aWlanInfo.iSecurityMode = static_cast<TWlanSecMode>(secMode);

    // net mode
    TUint32 netMode(0);
    wlanTableView->ReadUintL(TPtrC( WLAN_CONNECTION_MODE), netMode);
    aWlanInfo.iNetMode = static_cast<TWlanNetMode>(netMode);
    
    CleanupStack::PopAndDestroy(wlanTableView);
    CleanupStack::PopAndDestroy(commsDbIapTableView);
    CleanupStack::PopAndDestroy(commsDb); 
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::SsidIdentity
// ---------------------------------------------------------------------------
//
TBool CWsfWlanScanner::SsidIdentity( const TWlanSsid& aSsid1, 
                                     const TWlanSsid& aSsid2 )
    {
    return !aSsid1.Compare( aSsid2 );
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::RefreshNetworkNameL
// ---------------------------------------------------------------------------
//
TBool CWsfWlanScanner::RefreshNetworkNameL( TWsfWlanInfo& aWlanInfo )
    {
    LOG_ENTERFN( "CWsfWlanScanner::RefreshNetworkNameL" );

    TBool isHidden( EFalse );

    TUint8 ieLen( 0 );
    const TUint8* ieData;
    TBuf8<KWlanMaxSsidLength> ssid8;

    TInt ret = iScanInfo->InformationElement( E802Dot11SsidIE, ieLen,
                                              &ieData );

    if ( ret == KErrNone )
        {
        isHidden = IsHiddenSsid( ieLen, ieData );

        if ( ieLen )
            {
            ssid8.Copy( ieData, ieLen );
            aWlanInfo.iSsid.Copy( ssid8 );
            aWlanInfo.iRawSsid.Copy( ssid8 );
            TBuf<KWlanMaxSsidLength> ssid16;
            ssid16.Copy( ssid8 );
            LOG_WRITEF( "SSID: [%S]", &ssid16 );
            }
        else
            {
            LOG_WRITE( "SSID: <hidden>" );
            }
        }
    else
        {
        User::Leave( ret );
        }

    return isHidden;
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::IsHiddenSsid
// ---------------------------------------------------------------------------
//
TBool CWsfWlanScanner::IsHiddenSsid( TUint aSsidLength, const TUint8* aSsid )
    {
    LOG_ENTERFN( "CWsfWlanScanner::IsHiddenSsid" );


    if ( !aSsidLength )
        {
        LOG_WRITEF( "result: %d", ETrue );
        return ETrue;
        }

    TInt count( 0 );
    for ( TUint i( 0 ); i < aSsidLength; ++i )
        {
        count |= aSsid[i]; // in hidden networks characters are: 0x00
        }

    LOG_WRITEF( "result: %d", !count );

    return !count;
    }
    
    
// ---------------------------------------------------------------------------
// CWsfWlanScanner::RefreshSignalStrength
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::RefreshSignalStrength( TWsfWlanInfo& aWlanInfo  )
    {
    LOG_ENTERFN( "CWsfWlanScanner::RefreshSignalStrength" );

    TInt rxLevel = iScanInfo->RXLevel();
    
    LOG_WRITEF( "rxLevel = %d", rxLevel );
    
    // yes, it is < and not >, because smaller value means stronger signal

    if ( rxLevel < aWlanInfo.iStrengthLevel )
        {
        LOG_WRITEF( "updating %d to %d", aWlanInfo.iStrengthLevel, rxLevel );
        aWlanInfo.iStrengthLevel = rxLevel;
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::RefreshNetworkMode
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::RefreshNetworkMode( TWsfWlanInfo& aWlanInfo )
    {
    LOG_ENTERFN( "CWsfWlanScanner::RefreshNetworkMode" );

    aWlanInfo.iNetMode = ( iScanInfo->Capability() & 
                               E802Dot11CapabilityEssMask ) ?
                                   EInfra : EAdhoc ;

    LOG_WRITEF( "netmode = %d", TInt( aWlanInfo.iNetMode ) );
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::RefreshSecurityMode
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::RefreshSecurityMode( TWsfWlanInfo& aWlanInfo )
    {
    LOG_ENTERFN( "CWsfWlanScanner::RefreshSecurityMode" );
    
  	TWlanConnectionExtentedSecurityMode extSecMode = iScanInfo->ExtendedSecurityMode(); 
        
    switch ( extSecMode )
        {
        case EWlanConnectionExtentedSecurityModeWepOpen:
        case EWlanConnectionExtentedSecurityModeWepShared:
            {
            aWlanInfo.iSecurityMode = EWlanSecModeWep;
            break;
            }

        case EWlanConnectionExtentedSecurityMode802d1x:
            {
            aWlanInfo.iSecurityMode = EWlanSecMode802_1x;
            break;
            }

        case EWlanConnectionExtentedSecurityModeWpa:
        case EWlanConnectionExtentedSecurityModeWpa2:
            {
            aWlanInfo.iSecurityMode = EWlanSecModeWpa;
            break;
            }

        case EWlanConnectionExtentedSecurityModeWpaPsk:
        case EWlanConnectionExtentedSecurityModeWpa2Psk:
            {
            aWlanInfo.iSecurityMode = EWlanSecModeWpa;
            break;
            }
            
        case EWlanConnectionExtentedSecurityModeWapi:
        case EWlanConnectionExtentedSecurityModeWapiPsk:
            {
            aWlanInfo.iSecurityMode = EWlanSecModeWAPI;
            break;
            }

        case EWlanConnectionExtentedSecurityModeOpen:
        default:
            {
            aWlanInfo.iSecurityMode = EWlanSecModeOpen;
            }
        }

    aWlanInfo.SetUsesPreSharedKey( 
            extSecMode == EWlanConnectionExtentedSecurityModeWpa2Psk || 
            extSecMode == EWlanConnectionExtentedSecurityModeWpaPsk );

    LOG_WRITEF( "security mode %d (PSK: %d)", 
                (TInt)aWlanInfo.iSecurityMode,
                (TInt)aWlanInfo.UsesPreSharedKey() );
    }


// -----------------------------------------------------------------------------
// CWsfWlanScanner::ConvertTxRateToTxRateEnum
// -----------------------------------------------------------------------------
//
core_tx_rate_e CWsfWlanScanner::ConvertTxRateToTxRateEnum( TUint8 aRate )
    {
    LOG_ENTERFN( "CWsfWlanScanner::ConvertTxRateToTxRateEnum" );

    switch ( aRate )
        {
        case core_tx_rate_value_1mbit:
            return core_tx_rate_1mbit;
        case core_tx_rate_value_2mbit:
            return core_tx_rate_2mbit;
        case core_tx_rate_value_5p5mbit:
            return core_tx_rate_5p5mbit;
        case core_tx_rate_value_6mbit:
            return core_tx_rate_6mbit;
        case core_tx_rate_value_9mbit:
            return core_tx_rate_9mbit;
        case core_tx_rate_value_11mbit:
            return core_tx_rate_11mbit;
        case core_tx_rate_value_12mbit:
            return core_tx_rate_12mbit;
        case core_tx_rate_value_18mbit:
            return core_tx_rate_18mbit;
        case core_tx_rate_value_22mbit:
            return core_tx_rate_22mbit;
        case core_tx_rate_value_24mbit:
            return core_tx_rate_24mbit;
        case core_tx_rate_value_33mbit:
            return core_tx_rate_33mbit;
        case core_tx_rate_value_36mbit:
            return core_tx_rate_36mbit;
        case core_tx_rate_value_48mbit:
            return core_tx_rate_48mbit;
        case core_tx_rate_value_54mbit:
            return core_tx_rate_54mbit;
        default:
            return core_tx_rate_none;
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::RefreshTechnology
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::RefreshTechnology( TWsfWlanInfo& aWlanInfo )
    {
    LOG_ENTERFN( "CWsfWlanScanner::RefreshTechnology" );

    TUint8 ieLen(0);
    const TUint8* ieData;
    TUint8 dataRates[KMaxNumberOfRates];

    if ( iScanInfo->InformationElement( E802Dot11HtCapabilitiesIE, ieLen,
            &ieData ) == 0 )
        {
        // 802.11n supported
        aWlanInfo.iTransferRate = 0x8;
        }
    else
        {
        Mem::FillZ( &dataRates[0], sizeof( dataRates ) );
        core_tx_rate_e rate( core_tx_rate_none );
        TUint32 basic_rates( 0 );
        TUint32 supported_rates( 0 );

        // Supported Rates
        iScanInfo->InformationElement( E802Dot11SupportedRatesIE, ieLen,
                &ieData );

        Mem::Copy( dataRates, ieData, ieLen );

        TUint32 temp_basic_rates( 0 );
        TUint32 temp_supported_rates( 0 );

        for ( TInt a = 0; a < ieLen; a++ )
            {
            rate = ConvertTxRateToTxRateEnum( dataRates[a]
                    & ~TX_RATE_BASIC_MASK );

            temp_supported_rates |= rate;

            if ( dataRates[a] & TX_RATE_BASIC_MASK )
                {
                /**
                 * The highest bit is enabled, the rate is both a basic rate
                 * and a supported rate.
                 */
                temp_basic_rates |= rate;
                }
            }

        basic_rates |= temp_basic_rates;
        supported_rates |= temp_supported_rates;

        // Extended Supported Rates
        Mem::FillZ( &dataRates[0], sizeof( dataRates ) );

        iScanInfo->InformationElement( E802Dot11ExtendedRatesIE, ieLen,
                &ieData );

        Mem::Copy( dataRates, ieData, ieLen );

        if ( ieData )
            {
            temp_basic_rates = 0;
            temp_supported_rates = 0;

            for ( TInt a = 0; a < ieLen; a++ )
                {
                rate = ConvertTxRateToTxRateEnum( dataRates[a]
                        & ~TX_RATE_BASIC_MASK );

                temp_supported_rates |= rate;

                if ( dataRates[a] & TX_RATE_BASIC_MASK )
                    {
                    /**
                     * The highest bit is enabled, the rate is both a basic rate
                     * and a supported rate.
                     */
                    temp_basic_rates |= rate;
                    }
                }

            basic_rates |= temp_basic_rates;
            supported_rates |= temp_supported_rates;
            }

        aWlanInfo.iTransferRate = 0x4; // 802.11bg

        // AP is 802.11b only if only 802.11b rates 
        // are advertised as supported rates.
        if ( !( supported_rates & ~CORE_TX_RATES_802P11B ) )
            {
            aWlanInfo.iTransferRate = 0x1; // 802.11b
            }
        // AP is 802.11g only if any of the 802.11g rates is a basic rate.
        else if ( basic_rates & CORE_TX_RATES_802P11G )
            {
            aWlanInfo.iTransferRate = 0x2; // 802.11g
            }
        }

    LOG_WRITEF( "technology = %d", aWlanInfo.iTransferRate );
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::ConnectionEstablishedL
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::ConnectionEstablishedL( const TDesC& aConnectionName )
    {
    LOG_ENTERFN( "CWsfWlanScanner::ConnectionEstablishedL" );
    LOG_WRITEF( "aConnectionName: [%S]", &aConnectionName );
    
    HBufC* temp = aConnectionName.AllocL();
    if ( iActiveConnectionName )
        {
        delete iActiveConnectionName;
        iActiveConnectionName = NULL;
        }
    iActiveConnectionName = temp;
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::ConnectionLostL
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::ConnectionLostL()
    {
    LOG_ENTERFN( "CWsfWlanScanner::ConnectionLostL" );
    if ( iActiveConnectionName )
        {
        delete iActiveConnectionName;
        iActiveConnectionName = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::ConnectingFailedL
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::ConnectingFailedL( TInt /*aError*/ )
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::ConnectedIapReleasedL
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::ConnectedIapReleasedL()
    {
    // no implementation required
    }
    

// ---------------------------------------------------------------------------
// CWsfWlanScanner::ScanResults
// ---------------------------------------------------------------------------
//
HBufC8* CWsfWlanScanner::ScanResults()
    {
    return iScanResults;
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::SetConnectionDetailProvider
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::SetConnectionDetailProvider( 
                                MWsfWlanConnectionDetailsProvider& aProvider )
    {
    iConnectionDetailsProvider = &aProvider;
    }


// ---------------------------------------------------------------------------
// CWsfWlanScanner::WlanScanIntervalChangedL
// ---------------------------------------------------------------------------
//
void CWsfWlanScanner::WlanScanIntervalChangedL( TUint aNewScanInterval,
                                                TBool aShowAvailability )
    {
    LOG_ENTERFN( "CWsfWlanScanner::WlanScanIntervalChangedL" );
    LOG_WRITEF( "bgScanInterval = %d sec", aNewScanInterval );
    LOG_WRITEF( "showAvailability = %d", aShowAvailability );
    iScanningInterval = aNewScanInterval * KMicrosecPerSecond;
    
    if ( iShowAvailability != aShowAvailability )
        {
        // background WLAN scanning status changed
        iShowAvailability = aShowAvailability;        

        if ( iShowAvailability )
            {
            // bgscan is now enabled
            // no reissuing, we rely on the WLAN engine callbacks 
            // from now on
            LOG_WRITE( "background scan enabled" );

            if ( iScanState == EIdle && IsActive() )
                {
                // reset the timer only if we are not in the middle 
                // of another scanning
                iTimer.Cancel();
                }
            }
        else
            {
            // bgscan is now disabled
            // reset the timer with the new interval
            LOG_WRITE( "background scan disabled" );

            if ( iScanState == EIdle )
                {
                LOG_WRITE( "reissuing timer request" );
                // reset the timer only if we are not in the middle 
                // of another scanning 
                // otherwise RunL will take care of the timer
                // doCancel resets timer
                if ( IsActive() )
                    {
                    Cancel();
                    }
                iTimer.After( iStatus, TTimeIntervalMicroSeconds32( 
                        iScanningInterval ) );
                SetActive();
                } 
            }
        }
    else if ( !iShowAvailability )
        {
        // only the scan interval has changed
        LOG_WRITE( "scan interval changed" );
        if ( iScanState == EIdle && IsActive() )
            {
            // reset the timer only if we are not in the middle of 
            // another scanning 
            // doCancel resets timer
            LOG_WRITE( "reissuing timer request" );
            Cancel();
            iTimer.After( iStatus, TTimeIntervalMicroSeconds32( 
                    iScanningInterval ) );
            SetActive();
            }        
        }
    }

// End of file

