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
* Description:  Implementation of CWsfMainViewModel.
*
*/

//  EXTERNAL INCLUDES
#include <badesca.h>
#include <coemain.h>
#include <StringLoader.h>
#include <wlansniffer.rsg>

//  CLASS HEADER
#include "wsfmainviewmodel.h"

//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"
#include "wsfwlaninfoarray.h"
#include "wsfmainviewinternals.h"

#include "wsflogger.h"


//  LOCAL DEFINITIONS

// Listbox item format for open networks
_LIT( KVisibleItemFormat1Icon, "%d\t%S\t%S\t%d" );

// Listbox item format for secured networks
_LIT( KVisibleItemFormat2Icons, "%d\t%S\t%S\t%d\t%d" );

// Listbox item format for "Other (unlisted)" item
_LIT( KHiddenItemFormat, "\t%S" );

// Maximal length of listbox item strings
const TInt KListBoxItemMaxLength = 128;

// Listbox granularity
const TUint KWlanListGranularity = 4;



//  CONSTRUCTION AND DESTRUCTION
// ---------------------------------------------------------------------------
// CWsfMainViewModel::NewL
// ---------------------------------------------------------------------------
//
CWsfMainViewModel* CWsfMainViewModel::NewL()
    {
    LOG_ENTERFN( "CWsfMainViewModel::NewL" );
    CWsfMainViewModel* self = CWsfMainViewModel::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::NewLC
// ---------------------------------------------------------------------------
//
CWsfMainViewModel* CWsfMainViewModel::NewLC()
    {
    LOG_ENTERFN( "CWsfMainViewModel::NewLC" );
    CWsfMainViewModel* self = new( ELeave ) CWsfMainViewModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::~CWsfMainViewModel
// ---------------------------------------------------------------------------
//
CWsfMainViewModel::~CWsfMainViewModel()
    {
    LOG_ENTERFN( "CWsfMainViewModel::~CWsfMainViewModel" );
    delete iFormattedWlanList;
    delete iSelectedWlan;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::CWsfMainViewModel
// ---------------------------------------------------------------------------
//
CWsfMainViewModel::CWsfMainViewModel(): iCoeEnv( CCoeEnv::Static() )
    {
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfMainViewModel::ConstructL()
    {
    LOG_ENTERFN( "CWsfMainViewModel::ConstructL" );
    iFormattedWlanList = new (ELeave) CDesCArrayFlat( KWlanListGranularity );
    iSelectedWlan = KNullDesC8().AllocL();
    
    // add the hidden wlan item to the list by default
    TWsfWlanInfo dummy;
    HBufC* hidden = HBufC::NewLC( KListBoxItemMaxLength );
    TPtr ptr( hidden->Des() );

    FormatHiddenWlanItemL( dummy, ptr );
    iFormattedWlanList->AppendL( *hidden );
    
    CleanupStack::PopAndDestroy( hidden );
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::GetWlanList
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfMainViewModel::GetWlanList()
    {
    return iFormattedWlanList;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::GetInfoArray
// ---------------------------------------------------------------------------
//
CWsfWlanInfoArray* CWsfMainViewModel::GetInfoArray()
    {
    return iWlanInfoArray;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::SetSelectedWlan
// ---------------------------------------------------------------------------
//
void CWsfMainViewModel::SetSelectedWlan( HBufC8* aSsid )
    {
    delete iSelectedWlan;
    iSelectedWlan = aSsid;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::SelectedWlan
// ---------------------------------------------------------------------------
//
const TDesC8& CWsfMainViewModel::SelectedWlan()
    {
    return *iSelectedWlan;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::SetSelectedIndex
// ---------------------------------------------------------------------------
//
void CWsfMainViewModel::SetSelectedIndex( TInt aIndex )
    {
    iListboxIndex = aIndex;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::SelectedIndex
// ---------------------------------------------------------------------------
//
TInt CWsfMainViewModel::SelectedIndex()
    {
    return iListboxIndex;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::FormatNaviPaneLC
// ---------------------------------------------------------------------------
//
HBufC* CWsfMainViewModel::FormatNaviPaneLC()
    {
    LOG_ENTERFN( "CWsfMainViewModel::FormatNaviPaneLC" );
    HBufC* textOfNaviPane = NULL;

    if ( !iVisibleWlans )
        {
        textOfNaviPane =  KNullDesC().AllocLC();
        }

    else if ( iVisibleWlans == 1 )
        {
        textOfNaviPane = StringLoader::LoadLC(
            R_QTN_SNIFFER_NAVI_ONE_WLAN_NW_AVAILABLE, iCoeEnv );
        }
    else
        {
        textOfNaviPane = StringLoader::LoadLC(
                                R_QTN_SNIFFER_NAVI_MANY_WLAN_NWS_AVAILABLE, 
                                iVisibleWlans, 
                                iCoeEnv );
        }
    return textOfNaviPane;
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::FormatWlanListL
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfMainViewModel::FormatWlanListL( 
                                            CWsfWlanInfoArray* aWlanList )
    {
    LOG_ENTERFN( "CWsfMainViewModel::FormatWlanListL" );
    //Function expects that items in list are already in proper order
    iVisibleWlans = 0;
    iWlanInfoArray = aWlanList;
    iFormattedWlanList->Reset();
    HBufC* item = HBufC::NewLC( KListBoxItemMaxLength );
    TPtr ptr( item->Des() );
    for (TInt i = 0; i < aWlanList->Count(); ++i )
        {
        TWsfWlanInfo* wlanInfo = aWlanList->At( i );

        // Hidden WLAN item is appended later for unknown hidden networks
        if ( wlanInfo->Hidden() && !wlanInfo->iIapId )
            {
            continue;
            }
        else
            {
            ++iVisibleWlans;
            }

        // known hidden wlans are also "visible"
        ptr.Zero();
        FormatVisibleWlanItemL( *wlanInfo, ptr );
        iFormattedWlanList->AppendL( ptr );
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

// ---------------------------------------------------------------------------
// CWsfMainViewModel::FormatHiddenWlanItemL
// ---------------------------------------------------------------------------
//
void CWsfMainViewModel::FormatHiddenWlanItemL( TWsfWlanInfo& /*aWlan*/, 
                                               TDes& aItem )
    {
    LOG_ENTERFN( "CWsfMainViewModel::FormatHiddenWlanItemL" );
    HBufC* primaryText = StringLoader::LoadLC( R_QTN_SNIFFER_HIDDEN_WLAN );

    // Only Hidden WLAN text is shown.
    // All icons are transparent
    aItem.Format( KHiddenItemFormat, primaryText );

    CleanupStack::PopAndDestroy( primaryText );
    }

// ---------------------------------------------------------------------------
// CWsfMainViewModel::FormatVisibleWlanItemL
// ---------------------------------------------------------------------------
//
void CWsfMainViewModel::FormatVisibleWlanItemL( TWsfWlanInfo& aWlan, 
                                                TDes& aItem )
    {
    LOG_ENTERFN( "CWsfMainViewModel::FormatVisibleWlanItemL" );
    // Icon in first column is transparent by default
    // Known / Connected / None
    TInt column1Icon = KTransparentIcon;   // qgn_transparent.svg

    if ( aWlan.BrandId() )
           {
           column1Icon = KTransparentIcon + aWlan.BrandId();
           }
    else if ( aWlan.Connected() )
        {
        column1Icon = KConnectedNWIcon;    // qgn_prop_cmon_wlan_conn.svg
        }
    else if ( aWlan.Known() )
        {
        column1Icon = KKnownNWIcon;        // qgn_prop_wlan_bearer.svg
        }

    //Ssid as primary text
    HBufC* primaryText( NULL );
    
    if ( aWlan.iNetworkName.Length() ) // If there is IAP
        {
        primaryText = aWlan.GetIapNameAsUnicodeLC();
        }
    else // If there no IAP
        {
        primaryText = aWlan.GetSsidAsUnicodeLC();
        }

    //Secondary Text, "Known" if IAP is already defined. Else "Unknown"
    TInt resId = R_QTN_SNIFFER_UNKNOWN;
    if ( aWlan.ConnectionStatus() == EConnected )
        {
        resId = R_QTN_SNIFFER_CONNECTED;
        }
    else if ( aWlan.ConnectionStatus() == EConnecting ) 
        {
        resId = R_QTN_SNIFFER_CONNECTING;
        }
    else if ( aWlan.Known() ) 
        {
        resId = R_QTN_SNIFFER_KNOWN;
        }

    HBufC* secondaryText = StringLoader::LoadLC( resId );


    //Column 3
    //Show secure icon if network is secure. By default show transparant icon.
    TInt column4Icon = aWlan.Secure() ? KSecureNetworkIcon : KTransparentIcon;


    // Signal strenght for column 4
    // No signal icon is set by default
    TInt column3Icon = KNoSignalIcon;    //qgn_transparent.svg

    switch ( aWlan.SignalStrength() )
        {
        case EPoor:
            {
            column3Icon = KPoorSignal;      //qgn_indi_wlan_signal_low_add.svg
            break;
            }
        case EAverage:
            {
            column3Icon = KAverageSignal;   //qgn_indi_wlan_signal_med_add.svg
            break;
            }
        case EExcelent:
            {
            column3Icon = KExcelentSignal;  //qgn_indi_wlan_signal_good_add.svg
            break;
            }

        case ENoSignal:
        default:
            {
            column3Icon = KNoSignalIcon;     //qgn_indi_wlan_signal_no_wlan.svg
            break;
            }
        }

    if ( column4Icon == KTransparentIcon )
        {
        aItem.Format( KVisibleItemFormat1Icon, column1Icon,
                                        primaryText,
                                        secondaryText,
                                        column3Icon );
        }
    else
        {
        aItem.Format( KVisibleItemFormat2Icons, column1Icon,
                                        primaryText,
                                        secondaryText,
                                        column3Icon,
                                        column4Icon );
        }

    CleanupStack::PopAndDestroy( secondaryText );
    CleanupStack::PopAndDestroy( primaryText );
    }

// End of file

