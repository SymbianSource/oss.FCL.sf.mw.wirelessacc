/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Implementation of WLAN SSID discovery class
*
*/



//  EXTERNAL INCLUDES
#include <wlanmgmtclient.h>
#include <wlanmgmtcommon.h>

//  CLASS HEADER
#include "wsfwlanssiddiscovery.h"

//  INTERNAL INCLUDES
#include "wsflogger.h"


using namespace CMManager;


/**
* Management frame information element IDs.
* needed to determine SSID
* (from \S60\osext\wlan\wlanldd\wlan_common\umac_common\inc\802dot11.h)
*/
enum TWsf802Dot11InformationElementID
    {
    E802Dot11SsidIE                 = 0,
    E802Dot11SupportedRatesIE       = 1,
    E802Dot11FhParameterSetIE       = 2,
    E802Dot11DsParameterSetIE       = 3,
    E802Dot11CfParameterSetIE       = 4,
    E802Dot11TimIE                  = 5,
    E802Dot11IbssParameterSetIE     = 6,
    E802Dot11CountryIE              = 7,
    E802Dot11HoppingPatternParamIE  = 8,
    E802Dot11HoppingPatternTableIE  = 9,
    E802Dot11RequestIE              = 10,

    E802Dot11ChallengeTextIE        = 16,
    // Reserved for challenge text extension 17 - 31
    E802Dot11ErpInformationIE       = 42,
    E802Dot11ExtendedRatesIE        = 50,
    E802Dot11AironetIE              = 133,
    E802Dot11ApIpAddressIE          = 149,
    E802Dot11RsnIE                  = 221
    };


/**
* Bit masks for Capability Information field.
* (from \S60\osext\wlan\wlanldd\wlan_common\umac_common\inc\802dot11.h)
*/
enum TWsf802Dot11CapabilityBitMask
    {
    E802Dot11CapabilityEssMask          = 0x0001,
    E802Dot11CapabilityIbssMask         = 0x0002,
    E802Dot11CapabilityCfPollableMask   = 0x0004,
    E802Dot11CapabilityCfPollRequestMask= 0x0008,
    E802Dot11CapabilityPrivacyMask      = 0x0010,
    E802Dot11ShortPreambleMask          = 0x0020,
    E802Dot11PbccMask                   = 0x0040,      
    E802Dot11ChannelAgilityMask         = 0x0080
    };


// ======================= MEMBER FUNCTIONS =================================

// ----------------------------------------------------------------------------
// CWsfWlanSsidDiscovery::NewL
// ----------------------------------------------------------------------------
//
CWsfWlanSsidDiscovery* CWsfWlanSsidDiscovery::NewL()
    {
    CWsfWlanSsidDiscovery* self = new (ELeave) CWsfWlanSsidDiscovery();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfWlanSsidDiscovery::CWsfWlanSsidDiscovery
// ----------------------------------------------------------------------------
//
CWsfWlanSsidDiscovery::CWsfWlanSsidDiscovery(): CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }


// ----------------------------------------------------------------------------
// CWsfWlanSsidDiscovery::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfWlanSsidDiscovery::ConstructL()
    {
    LOG_ENTERFN( "CWsfWlanSsidDiscovery::ConstructL" );
#ifndef __WINS__
    iWlanMgmt = CWlanMgmtClient::NewL();
    iScanInfo = CWlanScanInfo::NewL();
#endif    
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSsidDiscovery::~CWsfWlanSsidDiscovery()
// ----------------------------------------------------------------------------
//
CWsfWlanSsidDiscovery::~CWsfWlanSsidDiscovery()
    {
    LOG_ENTERFN( "CWsfWlanSsidDiscovery::~CWsfWlanSsidDiscovery" );
    Cancel();
    delete iWlanMgmt;
    delete iScanInfo;    
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSsidDiscovery::DoCancel
// ----------------------------------------------------------------------------
//
void CWsfWlanSsidDiscovery::DoCancel()
    {
    LOG_ENTERFN( "CWsfWlanSsidDiscovery::DoCancel" );
#ifndef __WINS__    
    iWlanMgmt->CancelGetScanResults();
#endif    
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSsidDiscovery::AbortScanning
// ----------------------------------------------------------------------------
//
void CWsfWlanSsidDiscovery::AbortScanning()
    {
    LOG_ENTERFN( "CWsfWlanSsidDiscovery::AbortScanning" );
    if ( iWait.IsStarted() )
        {
        LOG_WRITE( "aborting..." );
        Cancel();

        iProgState = EAborted;
        iWait.AsyncStop();
        }    
    }
    
    
// ----------------------------------------------------------------------------
// CWsfWlanSsidDiscovery::RunL
// ----------------------------------------------------------------------------
//
void CWsfWlanSsidDiscovery::RunL()
    {
    LOG_ENTERFN( "CWsfWlanSsidDiscovery::RunL" );

    if ( iProgState == EScanning )
        {
        if ( !iStatus.Int() )
            // The list is ready 
            {
            LOG_WRITE( "No error" );
            iProgState = EDone;
            }
        else
            // something went wrong -> no coverage.
            {
            LOG_WRITE( "error" );
            }
        }
        
    // stop blocking
    if ( iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }
    
    }


// ----------------------------------------------------------------------------
// CWsfWlanSsidDiscovery::ScanForSsidL
// ----------------------------------------------------------------------------
//
TBool CWsfWlanSsidDiscovery::ScanForSsidL( TWlanSsid& aEnteredSsid,
                                TBool aBroadcast,
                                CMManager::TWlanNetMode& aNetworkMode, 
                                CMManager::TWlanSecMode& aSecurityMode,
                                TBool& aUsesPsk )
    {
    LOG_ENTERFN( "CWsfWlanSsidDiscovery::ScanForSsidL" );
    
    if ( IsActive() )
        {
        return EFalse;
        }
    
    TBool found = EFalse;
#ifndef __WINS__
    iProgState = EScanning;

    if ( aBroadcast )
        {
        LOG_WRITE( "performing broadcast scan" );
        iWlanMgmt->GetScanResults( iStatus, *iScanInfo );
        }
    else
        {
        LOG_WRITE( "performing direct scan" );        
        iWlanMgmt->GetScanResults( aEnteredSsid, iStatus, *iScanInfo );
        }

    SetActive();
    iWait.Start();
    
    // now we have the results and might start to work on them...
    if ( iProgState == EDone )
        {
        LOG_WRITE( "iProgState = EDone" );
        //finished without error, work on the result...
        for ( iScanInfo->First(); !found && !iScanInfo->IsDone(); 
                                                          iScanInfo->Next() )
            {
            TUint8 ieLen( 0 );
            const TUint8* ieData;
            
            TInt ret = iScanInfo->InformationElement( E802Dot11SsidIE, ieLen, 
                                                     &ieData );
            User::LeaveIfError( ret );
            
            // get the ssid
            TWlanSsid ssid;
            ssid.Copy( ieData, ieLen );
            
            if ( !aEnteredSsid.Compare( ssid ) )
                {
                // SSID matched
                LOG_WRITE( "SSID matched, processing security settings" );
                found = ETrue;

                // adhoc or infrastructure
                aNetworkMode = ( iScanInfo->Capability() & 
                                 E802Dot11CapabilityEssMask ) ?
                                        EInfra : EAdhoc;
                
                // security mode
                TWlanConnectionSecurityMode connSec = 
                                                   iScanInfo->SecurityMode();
                
                // convert TWlanConnectionSecurityMode to 
                // CMManager::TWlanSecMode
                switch ( connSec )
                    {
                    case EWlanConnectionSecurityOpen:
                        {
                        aSecurityMode = EWlanSecModeOpen;
                        break;
                        }
                    case EWlanConnectionSecurityWep:
                        {
                        aSecurityMode = EWlanSecModeWep;
                        break;
                        }
                    case EWlanConnectionSecurity802d1x:
                        {
                        aSecurityMode = EWlanSecMode802_1x;
                        break;
                        }
                    case EWlanConnectionSecurityWpa:
                    case EWlanConnectionSecurityWpaPsk:
                        {
                        aSecurityMode = EWlanSecModeWpa;
                        break;
                        }
                    default:
                        {
                        }
                    }

                aUsesPsk = ( connSec == EWlanConnectionSecurityWpaPsk );

                } // if aEnteredSsid
        
            } // for
        
        }
    else if ( iProgState == EAborted )
        {
        LOG_WRITE( "iProgState = EAborted" );
        User::Leave( KErrCancel );
        }
    else
        {
        LOG_WRITEF( "iProgState = ???, iStatus = %d", iStatus.Int() );
        User::LeaveIfError( iStatus.Int() );
        }
        
#else // __WINS__

    // we don't like warnings
    aEnteredSsid, aBroadcast, aNetworkMode, aSecurityMode, aUsesPsk;
    
#endif // __WINS__

    return found;
    }


