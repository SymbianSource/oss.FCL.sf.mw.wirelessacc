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
* Description:  Implementation of CWsfAiModel.
*
*/

//  EXTERNAL INCLUDES
#include <badesca.h>
#include <coemain.h>
#include <StringLoader.h>
#include <wsfaipluginrsc.rsg>

//  INTERNAL INCLUDES
#include "wsfaimodel.h"
#include "wsfwlaninfo.h"
#include "wsfwlaninfoarray.h"
#include "wsfaiplugincommon.h"
#include "wsfwlaninfoarrayvisitor.h"
#include "wsflogger.h"

//  MACROS
#ifdef _DEBUG
    _LIT( KModelPanic, "CWsfAiModel" );
    #define _ASS_D( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KModelPanic, __LINE__) )
#else
        #define _ASS_D( cond ) {}
#endif //_DEBUG
    
//  LOCAL DEFINITIONS
const TInt KListBoxItemMaxLength = 128;
const TUint KWLanListGranularity = 4;

// List item format
_LIT( KListItemFormat, "%d\t%S\t%d\t%d" ); 

// List item format for "Other (unlisted)" item
_LIT( KHiddenItemFormat, "\t%S" );


//  CONSTRUCTION AND DESTRUCTION
// --------------------------------------------------------------------------
// CWsfAiModel::NewL
// --------------------------------------------------------------------------
//
CWsfAiModel* CWsfAiModel::NewL()
    {
    CWsfAiModel* self = CWsfAiModel::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::NewLC
// --------------------------------------------------------------------------
//
CWsfAiModel* CWsfAiModel::NewLC()
    {
    CWsfAiModel* self = new( ELeave ) CWsfAiModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::~CWsfAiModel
// --------------------------------------------------------------------------
//
CWsfAiModel::~CWsfAiModel()
    {  
    delete iWlanInfoBranding;
    delete iFormattedWlanList;
    if ( iStatusRefreshing )
        {
        delete iStatusRefreshing;
        }
    if ( iStatusConnecting )
        {
        delete iStatusConnecting;
        }
    if ( iStatusScanningOff )
        {
        delete iStatusScanningOff;
        }
    if ( iManualSSIDInputting )
        {
        delete iManualSSIDInputting;
        }
    }


// --------------------------------------------------------------------------
// CWsfAiModel::CWsfAiModel
// --------------------------------------------------------------------------
//
CWsfAiModel::CWsfAiModel(): 
    iCoeEnv( CCoeEnv::Static() ),
    iScanning( EFalse ),
    iConnected( EFalse ),
    iRefreshIndex( iRefreshIcons.Count() - 1 ),
    iConnectingIndex( iConnectingIcons.Count() - 1 )
    {
    }


// --------------------------------------------------------------------------
// CWsfAiModel::ConstructL
// --------------------------------------------------------------------------
//
void CWsfAiModel::ConstructL()
    {
    iFormattedWlanList = new (ELeave) CDesCArrayFlat( KWLanListGranularity );
    iWlanInfoBranding = CWsfWlanInfoArrayVisitor::NewL( ETrue );
    iWlanInfoBranding->LoadFilterDefinitionsL();  
    
    iRefreshIcons[0] = EWlanOnIcon_0;
    iRefreshIcons[1] = EWlanOnIcon_1;
    iRefreshIcons[2] = EWlanOnIcon;
    iRefreshIcons[3] = EWlanOnIcon_1;
    
    iConnectingIcons[0] = EWlanOnIcon_0;
    iConnectingIcons[1] = EWlanOnIcon_1;
    iConnectingIcons[2] = EWlanOnIcon;
    iConnectingIcons[3] = EWlanOnIcon_1;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatWlanListL
// --------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfAiModel::FormatWlanListL( CWsfWlanInfoArray* aWlanList, const TBool aUIPrioritySort )
    {
    LOG_ENTERFN( "CWsfAiModel::FormatWlanListL" );
    HBufC* item = HBufC::NewLC( KListBoxItemMaxLength );
    TPtr ptr( item->Des() ); 
    iFormattedWlanList->Reset();
    
    // Function expects that items in list are already in proper order
    iWlanInfoArray = aWlanList;
    
    if ( iWlanInfoArray )
        {
        if ( aUIPrioritySort )
            {
            iWlanInfoArray->SetUIPrioritySort( ETrue );
            iWlanInfoArray->SortArrayL( *iWlanInfoBranding );
            // Sort Array with default sort
            // this sets the highest UI priority networks on top
            iWlanInfoArray->SortArrayL( );
            iWlanInfoArray->SetUIPrioritySort( EFalse );
            }
        else
            {
            iWlanInfoArray->SortArrayL( *iWlanInfoBranding );
            }
               
        for (TInt i = 0; i < iWlanInfoArray->Count(); ++i )
            {
            TWsfWlanInfo* wlanInfo = iWlanInfoArray->At(i);
        
            // Hidden WLAN item is appended later for unknown hidden networks
            if ( !wlanInfo || ( wlanInfo->Hidden() && !wlanInfo->iIapId ) )
                {
                continue;
                }
            
            // known hidden wlans are also "visible"
            ptr.Zero();
            FormatWlanSingleLineL( *wlanInfo, ptr, EFalse );
            iFormattedWlanList->AppendL( ptr );
            }
        }

    // now add "Other (unlisted)..." for hidden networks (always visible)
    // make up a dummy wlaninfo
    TWsfWlanInfo dummy;
    
    ptr.Zero();
    FormatHiddenWlanItemL( dummy, ptr );
    iFormattedWlanList->AppendL( ptr );

    CleanupStack::PopAndDestroy ( item );
    return iFormattedWlanList;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::GetWlanList
// --------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfAiModel::GetWlanList()
    {
    LOG_ENTERFN( "CWsfAiModel::GetWlanList" );
    return iFormattedWlanList;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::GetInfoArray
// --------------------------------------------------------------------------
//
CWsfWlanInfoArray* CWsfAiModel::GetInfoArray()
    {
    LOG_ENTERFN( "CWsfAiModel::GetInfoArray" );
    return iWlanInfoArray;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatSingleLineWlanListL
// --------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfAiModel::FormatSingleLineWlanListL( 
                                        CWsfWlanInfoArray* aWlanList )
    {
    LOG_ENTERFN( "CWsfAiModel::FormatSingleLineWlanListL" );
    iWlanInfoArray = aWlanList;
    
    if ( iWlanInfoArray )
        {
        iWlanInfoArray->SetUIPrioritySort( ETrue );
        iWlanInfoArray->SortArrayL( *iWlanInfoBranding );
        
        // Sort Array with default sort
        // this sets the highest UI priority networks on top
        iWlanInfoArray->SortArrayL();
        iWlanInfoArray->SetUIPrioritySort( EFalse );
        }
      
    iFormattedWlanList->Reset();
            
    HBufC* item = HBufC::NewLC( KListBoxItemMaxLength );
    TPtr ptr( item->Des() );
        
    if ( iConnected )
        {
        FormatConnectedL( ptr );
        iFormattedWlanList->AppendL( ptr );
        }
    else if ( iConnecting )
        {
        FormatConnectingL();
        }
    else if ( !iScanning )
        {
        FormatStatusOffL( ptr );
        iFormattedWlanList->AppendL( ptr );       
        }
    // if there's wlans avail
    else if ( iWlanInfoArray && iWlanInfoArray->Count() )
        {
        CheckWlansL( iWlanInfoArray, ptr );
        iFormattedWlanList->AppendL( ptr );
        }
    else 
        {
        // no wlans available
        FormatNoWlansAvailableL( ptr );
        iFormattedWlanList->AppendL( ptr );
        }
        
    CleanupStack::PopAndDestroy( item );
          
    return iFormattedWlanList;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatRefreshingL
// --------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfAiModel::FormatRefreshingL()
    {
    LOG_ENTERFN( "CWsfAiModel::FormatRefreshingL" );
    iFormattedWlanList->Reset();
    HBufC* item = HBufC::NewLC( KListBoxItemMaxLength );
    TPtr ptr( item->Des() );           
    
    if ( !iStatusRefreshing )
        {
        iStatusRefreshing = StringLoader::LoadL( 
                R_QTN_SNIFFER_PLUG_IN_REFRESHING );
        }
   
    ptr.Format( KListItemFormat, ScanningOn() ? 
    		                     iRefreshIcons[ iRefreshIndex ] 
                                  : EWlanOffIcon, 
                                  iStatusRefreshing,
                                 ETransparentIcon, 
                                 ETransparentIcon );
        
    iFormattedWlanList->AppendL( ptr );   
    CleanupStack::PopAndDestroy ( item );

    return iFormattedWlanList;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatConnecting
// --------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfAiModel::FormatConnectingL()
    {
    LOG_ENTERFN( "CWsfAiModel::FormatConnectingL" );
    iFormattedWlanList->Reset();
    HBufC* item = HBufC::NewLC( KListBoxItemMaxLength );
    TPtr ptr( item->Des() );           
    
    if ( !iStatusConnecting )
        {
        iStatusConnecting = StringLoader::LoadL( 
                R_QTN_SNIFFER_PLUG_IN_CONNECTING );
        }
   
    ptr.Format( KListItemFormat, iConnectingIcons[ iConnectingIndex ], 
                                 iStatusConnecting,
                                 ETransparentIcon, 
                                 ETransparentIcon );
        
    iFormattedWlanList->AppendL( ptr );   
    CleanupStack::PopAndDestroy ( item );
    return iFormattedWlanList;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::AppendBrandIconsL
// --------------------------------------------------------------------------
//
void CWsfAiModel::AppendBrandIconsL( CAknIconArray& aIconArray )
    {
    LOG_ENTERFN( "CWsfAiModel::AppendBrandIconsL" );
    iWlanInfoBranding->AppendBrandingIconsL( aIconArray );
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatHiddenWlanItemL
// --------------------------------------------------------------------------
//
void CWsfAiModel::FormatHiddenWlanItemL( TWsfWlanInfo& /*aWlan*/, 
                                         TPtr& aItem )
    {
    LOG_ENTERFN( "CWsfAiModel::FormatHiddenWlanItemL" );
    if ( !iManualSSIDInputting )
        {
        iManualSSIDInputting = StringLoader::LoadL( 
                R_QTN_WLAN_MANUAL_SSID_INPUT );
        }

    // Only Hidden WLAN text is shown.
    // No icons for this item
    aItem.Format( KHiddenItemFormat, iManualSSIDInputting );      
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatStatusOffL
// --------------------------------------------------------------------------
//
void CWsfAiModel::FormatStatusOffL( TPtr& aItem )
    {
    LOG_ENTERFN( "CWsfAiModel::FormatStatusOffL" );
    if ( !iStatusScanningOff )
        {
        iStatusScanningOff = StringLoader::LoadL( 
                R_QTN_SNIFFER_PLUG_IN_SCANNING_OFF );
        }

        aItem.Format( KListItemFormat, 
                      EWlanOffIcon, 
                      iStatusScanningOff,
                      ETransparentIcon, 
                      ETransparentIcon );         
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatNoWlansAvailableL
// --------------------------------------------------------------------------
//
void CWsfAiModel::FormatNoWlansAvailableL( TPtr& aItem )
    {
    LOG_ENTERFN( "CWsfAiModel::FormatNoWlansAvailableL" );

    if ( !iNoWLANsAvailable )
        {
        iNoWLANsAvailable = StringLoader::LoadL( 
                R_QTN_STATUS_NO_WLANS_AVAILABLE );
        }
   
        aItem.Format( KListItemFormat, 
                      ScanningOn()? EWlanOnIcon: EWlanOffIcon, 
                      iNoWLANsAvailable,
                      ETransparentIcon, 
                      ETransparentIcon );  
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatWlanSingleLineL
// --------------------------------------------------------------------------
//
void CWsfAiModel::FormatWlanSingleLineL( TWsfWlanInfo& aWlan, 
                                         TPtr& aItem, 
                                         TBool aSingleLine )
    {
    LOG_ENTERFN( "CWsfAiModel::FormatWlanSingleLineL" );
    //Ssid as primary text
    HBufC* ssid( NULL );
    HBufC* formatData( NULL );
    TInt column1Icon( 0 );
    
    if ( aWlan.iNetworkName.Length() ) // If there is IAP
        {
        ssid = aWlan.GetIapNameAsUnicodeLC();
        }
    else // If there no IAP
        {
        ssid = aWlan.GetSsidAsUnicodeLC();
        }


    column1Icon = ETransparentIcon;
    
    if ( aSingleLine )
        {
        // AI single line mode
        if ( aWlan.Connected() )
            {
            column1Icon = EConnectedNWIcon; 
            }
        else if ( aWlan.Known() )
            {
            column1Icon = EWlanOnIcon;     
            }
        
        // format ssid into string '<ssid>'
        formatData = StringLoader::LoadLC( R_SSID, *ssid, iCoeEnv ); 
        }
    else
        {
        // Search wlans dialog multiline mode
        
        if ( aWlan.BrandId() )
            {
            column1Icon = EWlanOffIcon + aWlan.BrandId();
            }
        else if ( aWlan.Connected() )
            {
            column1Icon = EConnectedNWIcon; // qgn_prop_wlan_bearer.svg
            }
        else if ( aWlan.Known() )
            {
            column1Icon = EKnownNWIcon;     // qgn_prop_wlan_bearer.svg
            }       
        }
        

    // Signal strenght for column 4
    // No signal icon is set by default in case is SingleLine mode off
    TInt column3Icon = aSingleLine ? ETransparentIcon: 
                                    ENoSignalIcon;    //qgn_transparent.svg 
    TInt column4Icon = ETransparentIcon;
    
    if ( aWlan.Connected() || !aSingleLine )
        {
        // Column 3
        // Show secure icon if network is secure. 
        // By default show transparant icon.
        column4Icon = aWlan.Secure()? ESecureNetworkIcon: ETransparentIcon;
              
        switch ( aWlan.SignalStrength() )
            {      
            case EPoor:
                {
                //qgn_indi_wlan_signal_low_add.svg
                column3Icon = EPoorSignal;
                break;
                }
            case EAverage:
                {
                //qgn_indi_wlan_signal_med_add.svg
                column3Icon = EAverageSignal;                
                break;
                }
            case EExcelent:
                {
                //qgn_indi_wlan_signal_good_add.svg
                column3Icon = EExcelentSignal;                 
                break;
                }
                    
            case ENoSignal:
            default:
                {
                //qgn_indi_wlan_signal_no_wlan.svg
                column3Icon = ENoSignalIcon;                
                break;
                }        
            }
        }
    
    if ( aSingleLine )
        {
        aItem.Format( KListItemFormat, 
                      column1Icon, 
                      formatData,
                      column3Icon, 
                      column4Icon );
        CleanupStack::PopAndDestroy( formatData );
        }
    else
        {
        aItem.Format( KListItemFormat, 
                      column1Icon, 
                      ssid,
                      column3Icon, 
                      column4Icon );
        }
    CleanupStack::PopAndDestroy( ssid );            
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatUnknownWlansSingleLineL
// --------------------------------------------------------------------------
//
void CWsfAiModel::FormatUnknownWlansSingleLineL( 
                                           const TBool aMultipleUnknownWlans,
                                           TPtr& aItem )
    {
    LOG_ENTERFN( "CWsfAiModel::FormatUnknownWlansSingleLineL" );
    TInt unknownItemResource = aMultipleUnknownWlans ? 
                        R_QTN_SNIFFER_PLUG_IN_UNKNOWN_SSIDS_FOUND :
                        R_QTN_SNIFFER_PLUG_IN_UNKNOWN_SSID_FOUND;

    HBufC* unknownWlansStatusText = iCoeEnv->AllocReadResourceLC( 
                                                       unknownItemResource );

    aItem.Format( KListItemFormat, ScanningOn()? EWlanOnIcon: EWlanOffIcon, 
                                   unknownWlansStatusText,
                                   ETransparentIcon, 
                                   ETransparentIcon );         
                        
    CleanupStack::PopAndDestroy( unknownWlansStatusText );
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FindPreferredKnownWlan
// --------------------------------------------------------------------------
//
TInt CWsfAiModel::FindPreferredKnownWlan( CWsfWlanInfoArray* aWlanList )
    {
    LOG_ENTERFN( "CWsfAiModel::FindPreferredKnownWlan" );
    _ASS_D( aWlanList  );
    _ASS_D( aWlanList->Count() ); 

    TWsfWlanInfo *preferred = NULL;
    TWsfWlanInfo *temp = NULL;
    TInt loopIndex( 0 );
    TInt preferredIndex( 0 );
    //loop through the list and find Max signal strength amont know wlans...
     
    preferred = aWlanList->At( loopIndex );
    loopIndex++;
 
    for ( ; loopIndex < aWlanList->Count(); ++loopIndex )
        {
        temp = aWlanList->At( loopIndex );
        if ( !temp->Known() )
            {
            // not known net any more...
            break;
            }
        // check the signal strength
        if ( temp->SignalStrength() > preferred->SignalStrength() )
            {
            preferredIndex = loopIndex;
            preferred = temp;
            }
        }
    return preferredIndex;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::CheckWlansL
// --------------------------------------------------------------------------
//
void CWsfAiModel::CheckWlansL( CWsfWlanInfoArray* aWlanList, TPtr& aItem )
    {
    LOG_ENTERFN( "CWsfAiModel::CheckWlansL" );
    _ASS_D( aWlanList );
    _ASS_D( aWlanList->Count() ); 
    // first check the connected
    // if there's connection... it is the first item in array

    if ( aWlanList->At( 0 )->Known() )
        {
        FormatWlanSingleLineL( *aWlanList->At( 0 ), aItem, ETrue );
        }
    else
        {
        TBool multipleUnknownWlans = ( aWlanList->Count() > 1 );
        FormatUnknownWlansSingleLineL( multipleUnknownWlans, aItem ); 
        }          
    }


// --------------------------------------------------------------------------
// CWsfAiModel::FormatConnectedL
// --------------------------------------------------------------------------
//
void CWsfAiModel::FormatConnectedL( TPtr& aItem )
    {
    LOG_ENTERFN( "CWsfAiModel::FormatConnectedL" );
    // Ssid as primary text
    FormatWlanSingleLineL( iConnectedWlanInfo, aItem, ETrue );
    }

// --------------------------------------------------------------------------
// CWsfAiModel::InitializeRefreshAnimation
// --------------------------------------------------------------------------
//
void CWsfAiModel::InitializeRefreshAnimation()
	{
	LOG_ENTERFN( "CWsfAiModel::InitializeRefreshAnimation" );
	iRefreshIndex = 0;
	}

// --------------------------------------------------------------------------
// CWsfAiModel::InitializeRefreshAnimation
// --------------------------------------------------------------------------
//
void CWsfAiModel::InitializeConnectingAnimation()
    {
    LOG_ENTERFN( "CWsfAiModel::InitializeConnectingAnimation" );
    iConnectingIndex = 0;
    }

// --------------------------------------------------------------------------
// CWsfAiModel::AnimateRefresh
// --------------------------------------------------------------------------
//
void CWsfAiModel::AnimateRefresh()
	{
	LOG_ENTERFN( "CWsfAiModel::AnimateRefresh" );
	++iRefreshIndex %= iRefreshIcons.Count();
	}

// --------------------------------------------------------------------------
// CWsfAiModel::AnimateRefresh
// --------------------------------------------------------------------------
//
void CWsfAiModel::AnimateConnecting()
    {
    LOG_ENTERFN( "CWsfAiModel::AnimateConnecting" );
    ++iConnectingIndex %= iConnectingIcons.Count();
    }



// End of file

