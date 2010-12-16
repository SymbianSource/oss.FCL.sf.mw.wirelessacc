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


// INCLUDE FILES

//User includes
#include "wlanpluginwlanstate.h"
#include "wlanplugindbobserver.h"
#include "wlanpluginlogger.h"
#include "wlanplugintimeshifter.h"

//System includes
#include <wsfmodel.h>
#include <wsfwlaninfoarray.h>
#include <wlanpluginrsc.rsg>
#include <StringLoader.h> 
#include <gsparentplugin.h>
#include <wlanplugin.mbg>
#include <wsfwlaninfoarrayvisitor.h>


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanPluginWlanState::CWlanPluginWlanState
// Constructor
// ---------------------------------------------------------
CWlanPluginWlanState::CWlanPluginWlanState( CGSParentPlugin *aParent ) 
: iScanning( EFalse ),
  iConnected( EFalse ),
  iExplicitScanning( 0 ),
  iCoeEnv( CCoeEnv::Static() ),
  iParent( aParent ),
  iSignalStrength(ENoSignal),
  iSupressParentUpdate( EFalse ),
  iSupressUpdateWlan( EFalse ),
  iValidSecondLine( ETrue ),   
  iSupressTimeShifter( EFalse )
    {
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::~CWlanPluginWlanState
// Descructor
// ---------------------------------------------------------
CWlanPluginWlanState::~CWlanPluginWlanState()
    {

    CLOG_ENTERFN( "CWlanPluginWlanState::~CWlanPluginWlanState()" );  
    
    if ( iEngine )
        {
        iEngine->CancelNotifyEvents();
        }
    
    if( iExplicitScanning )
        {
        TRAP_IGNORE( SetScanningL( EFalse ) );
        }
    if( iTimeShifter )
        {
        delete iTimeShifter;
        }
       
    delete iDbObserver;
    
    delete iWlanInfoBranding;
    
    //remove self from further notifications from the
    //sniffer server
    delete iEngine;
    delete iPresentString;
        
    CLOG_LEAVEFN( "CWlanPluginWlanState::~CWlanPluginWlanState()" );        
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::NewLC
// ---------------------------------------------------------
CWlanPluginWlanState* CWlanPluginWlanState::NewLC( CGSParentPlugin *aParent )
    {
    CWlanPluginWlanState* self= new ( ELeave ) 
                                   CWlanPluginWlanState( aParent );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::NewL
// ---------------------------------------------------------
CWlanPluginWlanState* CWlanPluginWlanState::NewL( CGSParentPlugin *aParent )
    {
    CWlanPluginWlanState* self = CWlanPluginWlanState::NewLC( aParent );
    CleanupStack::Pop( self );    
    return self;
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::ConstructL
// ---------------------------------------------------------
void CWlanPluginWlanState::ConstructL()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::ConstructL()" );  
              
    iSupressParentUpdate = ETrue;
        
    iEngine = CWsfModel::NewL( *this, EFalse );
    
    iWlanInfoBranding = CWsfWlanInfoArrayVisitor::NewL( ETrue );
    iWlanInfoBranding->LoadFilterDefinitionsL();
    
    if( iEngine->IsScanEnabledL() )
        {
        iScanning = ETrue;
        UpdateWlanListL();
        }
    
    if( iEngine->IsConnectedL() )
        {
        iEngine->GetConnectedWlanDetailsL( iConnectedWlanInfo );
        iConnected = iConnectedWlanInfo.Connected();
        }
            
    iDbObserver = CWlanPluginDbObserver::NewL( this );
    iDbObserver->ActivateItL();
    
    if( !iConnected && !iScanning && iExplicitScanning )
        {
        iValidSecondLine = EFalse;
        }
        
    iSupressParentUpdate = EFalse;
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::ConstructL()" );            
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::WlanListChangedL
// ---------------------------------------------------------
void CWlanPluginWlanState::WlanListChangedL()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::WlanListChangedL()" );  
	
	iValidSecondLine = ETrue;
    //if there is modification
    UpdateWlanListL();
    
    UpdateParentViewL();
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::WlanListChangedL()" );    
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::NotifyEngineError
// ---------------------------------------------------------
void CWlanPluginWlanState::NotifyEngineError( TInt /*aError*/ )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::NotifyEngineError()" );
    CLOG_LEAVEFN( "CWlanPluginWlanState::NotifyEngineError()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::ScanDisabledL
// ---------------------------------------------------------
void CWlanPluginWlanState::ScanDisabledL()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::ScanDisabledL()" ); 
    
    iScanning = EFalse;
    UpdateParentViewL();
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::ScanDisabledL()" );    
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::ScanEnabledL
// ---------------------------------------------------------
void CWlanPluginWlanState::ScanEnabledL()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::ScanEnabledL()" ); 
    
    // plugin has enabled scanning - background scan is on
    if ( iExplicitScanning )
        {
        CLOG_WRITE( "Refresh scan results" );
        iEngine->RefreshScanL();
        }
    
    iScanning = ETrue;
    UpdateWlanListL();
    UpdateParentViewL();
        
    CLOG_LEAVEFN( "CWlanPluginWlanState::ScanEnabledL()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::WlanConnectionActivatedL
// ---------------------------------------------------------    
void CWlanPluginWlanState::WlanConnectionActivatedL( TInt32 /* aIapId */ )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::WlanConnectionActivatedL()" );   
    
    iEngine->GetConnectedWlanDetailsL( iConnectedWlanInfo );
    iConnected = iConnectedWlanInfo.Connected();
    UpdateParentViewL();
                
    CLOG_LEAVEFN( "CWlanPluginWlanState::WlanConnectionActivatedL()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::WlanConnectionClosedL
// ---------------------------------------------------------    
void CWlanPluginWlanState::WlanConnectionClosedL( TInt32 /* aIapId */ )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::WlanConnectionClosedL()" );    
    
    iConnected = EFalse;
    if( iScanning )
        {
        UpdateWlanListL();
        }
        
    UpdateParentViewL();
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::WlanConnectionClosedL()" );
    }

// ---------------------------------------------------------------------------
// CWlanPluginWlanState::ConnectionCreationProcessFinishedL
// ---------------------------------------------------------------------------
//
void CWlanPluginWlanState::ConnectionCreationProcessFinishedL(
        TInt32 /* aIapId */,
        TInt /*aResult*/ )
    {
    // no implementation required
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::GetStateStringL
// ---------------------------------------------------------
void CWlanPluginWlanState::GetStateStringL( TDes& aDes )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::GetStateStringL()" ); 
    
    if( !iValidSecondLine )
    	{
        return;
    	}
    
    //make the string according to it
    HBufC* statestring = NULL;
    MakeStateStringL( iInfoArray, statestring );
    aDes.Copy( statestring->Des() );
    
    //keep it to decide later if it changed
    delete iPresentString; 
    iPresentString = statestring;
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::GetStateStringL()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::MakeStateStringL
// ---------------------------------------------------------    
void CWlanPluginWlanState::MakeStateStringL(
                            CWsfWlanInfoArray* aArray, HBufC*& aBuf )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::MakeStateStringL()" ); 
    
    if( iConnected )
        {
        CLOG_ENTERFN( "Connected format needed" );
        FormatConnectedL( aBuf );	
        }
    else if ( !iScanning )
        {
        CLOG_ENTERFN( "Offline format needed" );
        FormatStatusOffL( aBuf );        
        }
    // if there's wlans avail
    else if ( aArray && aArray->Count() )
        {
        CLOG_ENTERFN( "wlans/wlan format needed" );
        CheckWlansL( aArray, aBuf );            
        }
    else 
        {
        // no wlans available
        CLOG_ENTERFN( "No wlan format needed" );
        FormatNoWlansAvailableL( aBuf );
        }
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::MakeStateStringL()" );
    }
    
// ---------------------------------------------------------
// CWlanPluginWlanState::FormatConnectedL
// ---------------------------------------------------------    
void CWlanPluginWlanState::FormatConnectedL( HBufC*& aBuf )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::FormatConnectedL()" ); 
    
    //Ssid as primary text
    HBufC* ssid( NULL );
    
    if ( iConnectedWlanInfo.iNetworkName.Length() ) // If there is IAP
        {
        ssid = iConnectedWlanInfo.GetIapNameAsUnicodeLC();
        }
    else // If there no IAP
        {
        ssid = iConnectedWlanInfo.GetSsidAsUnicodeLC();
        }
    
    aBuf = StringLoader::LoadL( R_QTN_CP_DETAIL_WLAN_CONNECTED, 
                       *ssid );
                       
    CleanupStack::PopAndDestroy( ssid );
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::FormatConnectedL()" );
    }


// ---------------------------------------------------------
// CWlanPluginWlanState::FormatStatusOffL
// ---------------------------------------------------------    
void CWlanPluginWlanState::FormatStatusOffL( HBufC*& aBuf )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::FormatStatusOffL()" ); 
    
    aBuf = StringLoader::LoadL( R_QTN_CP_DETAIL_WLAN_SCAN_OFF );
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::FormatStatusOffL()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::FormatNoWlansAvailableL
// ---------------------------------------------------------    
void CWlanPluginWlanState::FormatNoWlansAvailableL( HBufC*& aBuf )
    { 
    CLOG_ENTERFN( "CWlanPluginWlanState::FormatNoWlansAvailableL()" ); 
      
    aBuf = StringLoader::LoadL( R_QTN_CP_DETAIL_WLAN_NO_NW_AVAILABLE );
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::FormatNoWlansAvailableL()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::CheckWlansL
// ---------------------------------------------------------    
void CWlanPluginWlanState::CheckWlansL( CWsfWlanInfoArray* aArray, 
                                    HBufC*& aBuf )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::CheckWlansL()" ); 

    if ( aArray->At( 0 )->Known() )
        {
        FormatWlanSingleLineL( *aArray->At( 0 ), aBuf );
        return;
        }
    TBool multipleUnknownWlans = ( aArray->Count() > 1 );
    FormatUnknownWlansSingleLineL( multipleUnknownWlans, aBuf );    
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::CheckWlansL()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::FormatWlanSingleLineL
// ---------------------------------------------------------     
void CWlanPluginWlanState::FormatWlanSingleLineL( 
                                     TWsfWlanInfo& aWlan, HBufC*& aBuf )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::FormatWlanSingleLineL()" ); 
    
        //Ssid as primary text
    HBufC* ssid( NULL );
    
    if ( aWlan.iNetworkName.Length() ) // If there is IAP
        {
        ssid = aWlan.GetIapNameAsUnicodeLC();
        }
    else // If there no IAP
        {
        ssid = aWlan.GetSsidAsUnicodeLC();
        }
        
    TInt formatResource = R_QTN_CP_DETAIL_WLAN_KNOWN_NW_AVAILABLE; 
    
    aBuf = StringLoader::LoadL ( formatResource, *ssid );
   
    CleanupStack::PopAndDestroy( ssid );            
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::FormatWlanSingleLineL()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::FormatUnknownWlansSingleLineL
// ---------------------------------------------------------
void CWlanPluginWlanState::FormatUnknownWlansSingleLineL( 
                        const TBool aMultipleUnknownWlans, HBufC*& aBuf )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::FormatUnknownWlansSingleLineL()" ); 
    
    TInt unknownItemResource = aMultipleUnknownWlans ? 
                        R_QTN_CP_DETAIL_WLAN_NETWORKS_AVAILABLE :
                        R_QTN_CP_DETAIL_WLAN_NETWORK_AVAILABLE;

    aBuf = StringLoader::LoadL( unknownItemResource );
                
    CLOG_LEAVEFN( "CWlanPluginWlanState::FormatUnknownWlansSingleLineL()" );   
    } 

// ---------------------------------------------------------
// CWlanPluginWlanState::GetMainIconL
// ---------------------------------------------------------    
TBool CWlanPluginWlanState::GetMainIconL( TInt& aPic, 
		                                  TInt& aPicMask,
		                                  TAknsItemID& aItemId )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::GetMainIconL()" ); 
    
    if( !iValidSecondLine )
        {
        aPic = EMbmWlanpluginQgn_prop_set_conn_wlan;
        aPicMask = EMbmWlanpluginQgn_prop_set_conn_wlan_mask;  
        aItemId = KAknsIIDQgnPropSetConnWlanConn;
        CLOG_LEAVEFN( "CWlanPluginWlanState::GetMainIconL()" );
        return ETrue;
        }
    
    if( !iSupressTimeShifter)
        {
        iSupressTimeShifter = EFalse;
        CLOG_WRITE(" Time shifter start not supressed ");
        if( !iTimeShifter )
            {
        	CLOG_WRITE(" Time shifter started ");
            iSupressTimeShifter = ETrue;
            iTimeShifter = CWlanPluginTimeShifter::NewL( this );
            iTimeShifter->ActivateIt();
            }
        }
    else
        {
        CLOG_WRITE(" Time shifter start supressed ");
        iSupressTimeShifter = EFalse;
        }
    if( iConnected )
        {
        //CONNECTED
        aPic = EMbmWlanpluginQgn_prop_set_conn_wlan_conn;
        aPicMask = EMbmWlanpluginQgn_prop_set_conn_wlan_conn_mask;
        aItemId = KAknsIIDQgnPropSetConnWlanConn;
        }
    else if( iScanning )
        {
        //SCANNING BUT NOT CONNECTED
        aPic = EMbmWlanpluginQgn_prop_set_conn_wlan;
        aPicMask = EMbmWlanpluginQgn_prop_set_conn_wlan_mask;  
        aItemId = KAknsIIDQgnPropSetConnWlanConn;
        }
    else
        {
        //SCANNING DISABLED
        aPic = EMbmWlanpluginQgn_prop_set_conn_wlan_off;
        aPicMask = EMbmWlanpluginQgn_prop_set_conn_wlan_off_mask;
        aItemId = KAknsIIDQgnPropSetConnWlanOff;
        }
              
    CLOG_LEAVEFN( "CWlanPluginWlanState::GetMainIconL()" );
    return ETrue;
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::GetStrengthIconL
// ---------------------------------------------------------        
TBool CWlanPluginWlanState::GetStrengthIconL( TInt& aPic, 
		                                      TInt& aPicMask, 
		                                      TAknsItemID& aItemId)
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::GetStrengthIconL()" ); 
    
    if( !iValidSecondLine )
    	{
        return EFalse;
    	}
    if ( iConnected )
        {
        switch( iConnectedWlanInfo.SignalStrength() )
            {
            case EPoor:
                aPic = EMbmWlanpluginQgn_indi_wlan_signal_low_add;
                aPicMask = EMbmWlanpluginQgn_indi_wlan_signal_low_add_mask;
                aItemId = KAknsIIDQgnIndiWlanSignalLowAdd;
                break;    
            case EAverage:
                aPic = EMbmWlanpluginQgn_indi_wlan_signal_med_add;
                aPicMask = EMbmWlanpluginQgn_indi_wlan_signal_med_add_mask;
                aItemId = KAknsIIDQgnIndiWlanSignalMedAdd;
                break;
            case EExcelent:
                aPic = EMbmWlanpluginQgn_indi_wlan_signal_good_add;
                aPicMask = EMbmWlanpluginQgn_indi_wlan_signal_good_add_mask;
                aItemId = KAknsIIDQgnIndiWlanSignalGoodAdd;
                break;
            default:
                aPic = EMbmWlanpluginQgn_indi_wlan_signal_low_add;
                aPicMask = EMbmWlanpluginQgn_indi_wlan_signal_low_add_mask;
                aItemId = KAknsIIDQgnIndiWlanSignalLowAdd;
            }
        iSignalStrength = iConnectedWlanInfo.SignalStrength();
        CLOG_LEAVEFN( "CWlanPluginWlanState::GetStrengthIconL()" );
        return ETrue;
        }
    iSignalStrength = ENoSignal;            
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::GetStrengthIconL()" );
    
    return EFalse;    
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::IsStateChangedL
// ---------------------------------------------------------    
TBool CWlanPluginWlanState::IsStateChangedL()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::IsStateChangedL()" ); 

    if ( !iPresentString ) 
        {
        CLOG_LEAVEFN( "CWlanPluginWlanState::IsStateChangedL()" );
        return ETrue;
        }
    //alloc a buf
    CLOG_WRITE( "CWlanPluginWlanState::IsStateChangedL - there is presentstring" );
    HBufC* buf = NULL;
       
    CLOG_WRITE( "CWlanPluginWlanState::IsStateChangedL - wlan list get" );
    //make the string that would be written out
    MakeStateStringL( iInfoArray, buf );
    
    CleanupStack::PushL( buf ); 
    
    CLOG_WRITE( "CWlanPluginWlanState::IsStateChangedL - state string made" );
    
    //decide if the string is the same as it was written before
    //and the connection state is the same
    TBool ret = 
            ( buf->Des().Compare( iPresentString->Des() ) ||
            iSignalStrength != GetSignalStrength() )
            ? ETrue : EFalse;  
    
    CLOG_WRITE( "CWlanPluginWlanState::IsStateChangedL - ret get" );
    
    CleanupStack::PopAndDestroy( buf );
        
    if( ret )
        {
        CLOG_WRITE( "State Changed" );   
        }
    else
        {
        CLOG_WRITE( "State NOT Changed" );
        }        
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::IsStateChangedL()" );
    
    //just to have ETrue in all true cases
    return ret;
    }
   
// ---------------------------------------------------------
// CWlanPluginWlanState::GetSignalStrength
// ---------------------------------------------------------    
TWsfWlanSignalStrengthLevel CWlanPluginWlanState::GetSignalStrength()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::GetSignalStrength()" ); 

    if ( iConnected )
        {
        CLOG_LEAVEFN( "CWlanPluginWlanState::GetSignalStrength()" );
        return iConnectedWlanInfo.SignalStrength();
        }
    else 
        {
        CLOG_LEAVEFN( "CWlanPluginWlanState::GetSignalStrength()" );
        return ENoSignal;
        }
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::IsScanning
// ---------------------------------------------------------    
TBool CWlanPluginWlanState::IsScanning()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::IsScanning()" ); 
    CLOG_LEAVEFN( "CWlanPluginWlanState::IsScanning()" );
    return iScanning;
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::SetScanning
// ---------------------------------------------------------    
void CWlanPluginWlanState::SetScanningL( TBool aScanning )
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::SetScanningL()" ); 
        
    if( aScanning )
        {
        iExplicitScanning++;
        if( iExplicitScanning == 1 && iEngine )
            {
            iEngine->EnableScanL(); 
            }
        }
    else 
        {
        iExplicitScanning--;
        if( iExplicitScanning == 0 && iEngine )
            {
            iEngine->DisableScanL(); 
            }
        }    
                   
    CLOG_LEAVEFN( "CWlanPluginWlanState::SetScanningL()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::UpdateParentViewL
// ---------------------------------------------------------  
void CWlanPluginWlanState::UpdateParentViewL()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::UpdateParentViewL()" );
    
    
    if( iSupressParentUpdate )
        {
        CLOG_WRITE( "Parent update suppressed" );  
        CLOG_LEAVEFN( "CWlanPluginWlanState::UpdateParentViewL()" );
        return;
        }
        
    iSupressParentUpdate = ETrue;
    
    if ( IsStateChangedL() )
        {
        iSupressTimeShifter = ETrue;
        iParent->UpdateView();
        }
    else
    	{
    	iSupressTimeShifter = EFalse;
    	}
        
    iSupressParentUpdate = EFalse;
    
    CLOG_LEAVEFN( "CWlanPluginWlanState::UpdateParentViewL()" );
    } 
    
// ---------------------------------------------------------
// CWlanPluginWlanState::UpdateWlanListL
// ---------------------------------------------------------  
void CWlanPluginWlanState::UpdateWlanListL()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::UpdateWlanListL()" );
    if( iSupressUpdateWlan )
        {
        CLOG_WRITE( "Update WLAN List supressed" );
        CLOG_LEAVEFN( "CWlanPluginWlanState::UpdateWlanListL()" );
        return;
        }
    
    iSupressUpdateWlan = ETrue; 
    
    TRAPD( error,
    iInfoArray = iEngine->GetWlanListL();
    
    if( iConnected )
        {
        iEngine->GetConnectedWlanDetailsL( iConnectedWlanInfo );
        iConnected = iConnectedWlanInfo.Connected();
        }
    
    iWlanInfoBranding->LoadFilterDefinitionsL();
    
    iInfoArray->SortArrayL( *iWlanInfoBranding );
    );
    
    if ( error != KErrNone )
        {
        CLOG_WRITE( "Fetching wlan data leaved" );
        // Reset the temporary flag so that wlan list 
        // updates are not completely suppressed 
        iSupressUpdateWlan = EFalse;
        User::Leave( error );
        }
    
#ifdef _DEBUG    
    if( iInfoArray )
        {
        if( iInfoArray->Count() <=0 )
            {
            CLOG_WRITE( "EMPTY ARRAY" );
            }
        for(TInt i = 0; i< iInfoArray->Count(); i++)
            {
            
            CLOG_WRITE( "----AP----" );
            HBufC* buf = iInfoArray->At( i )->GetSsidAsUnicodeLC();
            CLOG_WRITEF(_L("AP: %S"), ( buf ) );
            CleanupStack::PopAndDestroy( buf );
            if( iInfoArray->At( i )->Connected() )
                {
                CLOG_WRITE( "--Connected--" );
                }
            else
                {
                CLOG_WRITE( "--NOT Connected--" );
                }
            if( iInfoArray->At( i )->Known() )
                {
                CLOG_WRITE( "--Known--" );
                }
            else
                {
                CLOG_WRITE( "--NOT Known--" );
                }    
            }
        }
    else
        {
        CLOG_ENTERFN( "NO ARRAY" );
        }
#endif
    
    iSupressUpdateWlan = EFalse;
    CLOG_LEAVEFN( "CWlanPluginWlanState::UpdateWlanListL()" );
    }
    
// ---------------------------------------------------------
// CWlanPluginWlanState::DirectlyScan
// ---------------------------------------------------------      
void CWlanPluginWlanState::DirectlyScanL()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::DirectlyScan()" );
    if ( iExplicitScanning && iParent->IsForeground())
        {
        iEngine->RefreshScanL();
        }
    CLOG_LEAVEFN( "CWlanPluginWlanState::DirectlyScan()" );
    }

// ---------------------------------------------------------
// CWlanPluginWlanState::DeleteTimeShifter
// ---------------------------------------------------------          
void CWlanPluginWlanState::DeleteTimeShifter()
    {
    CLOG_ENTERFN( "CWlanPluginWlanState::DeleteTimeShifter()" );
    delete iTimeShifter;
    iTimeShifter = NULL;
    CLOG_LEAVEFN( "CWlanPluginWlanState::DeleteTimeShifter()" );
    }
