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
* Description:  Implementation of CWsfDetailsViewModel.
*
*/

//  EXTERNAL INCLUDES
#include <badesca.h>
#include <StringLoader.h> 
#include <AknUtils.h>
#include <wlansniffer.rsg>
#include <coemain.h>
#include <avkon.rsg>
#include <cmpluginwlandef.h>
#include <cmmanager.h>
#include <cmconnectionmethod.h>
#include <rconnmon.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"
#include "wsfdetailsviewmodel.h"
#include "wsfwlaninfoarray.h"
#include "wsflogger.h"

//  LOCAL DEFINITIONS
using namespace CMManager;

#ifdef _DEBUG
    _LIT( KDetailsViewModelPanic, "CWsfDetailsViewModel" );
    #define _ASSERTD( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KDetailsViewModelPanic, __LINE__) )
#else
    #define _ASSERTD( cond ) {}
#endif //_DEBUG

// the size of the Details Listbox's array
const TInt KWlanInfoGranularity = 8;
// the size of the numbers in the NaviPane
const TInt KNumberLegth = 5;
// Listbox elements format string
_LIT( KListItemFormat, "%S\t%S" );
// NaviPane elements format string
_LIT( KPaneFormater, "%i/%i" ); 

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::CWsfDetailsViewModel()
// ---------------------------------------------------------------------------
//
CWsfDetailsViewModel::CWsfDetailsViewModel():iCoeEnv( CCoeEnv::Static() )
    {
    }
    
// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::ConstructL()
// ---------------------------------------------------------------------------
//
void CWsfDetailsViewModel::ConstructL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::ConstructL" );    
    iFormattedWlanInfo = new (ELeave) CDesCArrayFlat( KWlanInfoGranularity );
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CWsfDetailsViewModel* CWsfDetailsViewModel::NewL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::NewL" );    
    CWsfDetailsViewModel* self = CWsfDetailsViewModel::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CWsfDetailsViewModel* CWsfDetailsViewModel::NewLC()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::NewLC" );
    CWsfDetailsViewModel* self = new( ELeave ) CWsfDetailsViewModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::~CWsfDetailsViewModel()
// ---------------------------------------------------------------------------
//
CWsfDetailsViewModel::~CWsfDetailsViewModel()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::~CWsfDetailsViewModel" );
    delete iFormattedWlanInfo;
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatWlanInfoL()
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfDetailsViewModel::FormatWlanInfoL( 
                                            TWsfWlanInfo* aWlanInfo, 
                                            CWsfWlanInfoArray* aWlanArray )
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatWlanInfoL" );    
    _ASSERTD( aWlanArray );

    for( TInt i = 0; i < iFormattedWlanInfo->MdcaCount(); i++)
        {
        TPtrC temp = iFormattedWlanInfo->MdcaPoint( i );
        TBuf<100> tempElement = temp;
        LOG_WRITEF( "(1)iFormattedWlanInfo: %S", &tempElement );
        }
        
    iWlanArray = aWlanArray;
    iFormattedWlanInfo->Reset();
    
    TInt visibleWlans = VisibleWlans( *iWlanArray );
    if ( aWlanInfo && visibleWlans )
        {
        iWlanInfo = *aWlanInfo;
        TWsfWlanInfo* temp = iWlanArray->Match( aWlanInfo->iSsid, 
                                                visibleWlans );
        if ( temp )
            {
            iIndex = iWlanArray->GetArrayIndex( temp );
            }
        else 
            {
            iIndex = iWlanArray->GetArrayIndex( &iWlanInfo );
            }

        // create array of not-connected WLAN data 
        FormatAndAppenWlanIapNameL();           
        FormatAndAppenWlanSsidL();
        FormatAndAppenSingnalL();
        FormatAndAppenModeL();
        FormatAndAppenSecurityL();
        FormatAndAppenCoverageL();
        FormatAndAppenSpeedL();
        }
    else if ( !aWlanInfo )// Current Wlan info has no signal
        {
        iWlanInfo.iStrengthLevel = EWlanSignalUnavailable;
        FormatAndAppenWlanIapNameL();
        FormatAndAppenWlanSsidL();
        FormatAndAppenSingnalL();
        }

    for( TInt i = 0; i < iFormattedWlanInfo->MdcaCount(); i++)
        {
        TPtrC temp = iFormattedWlanInfo->MdcaPoint( i );
        TBuf<100> tempElement = temp;
        LOG_WRITEF( "(2)iFormattedWlanInfo: %S", &tempElement );
        }
                
    return iFormattedWlanInfo;
    }
   
// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatNextWlanInfoL()
// ---------------------------------------------------------------------------
//

CDesCArrayFlat* CWsfDetailsViewModel::FormatNextWlanInfoL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatNextWlanInfoL" );
    _ASSERTD( iWlanArray );
    
    TInt visibleWlans = VisibleWlans( *iWlanArray );
        
    if( iIndex < visibleWlans - 1 )
        {
        iFormattedWlanInfo->Reset();
        iIndex++;
        
        if( iIndex >= visibleWlans - 1 )
            {
            // Set the index to point to the last visible in the array.
            iIndex = visibleWlans - 1;
            }
        else if ( iWlanInfo.SignalStrength() == ENoSignal )
            {
            iIndex = 0;
            }
            
        // (Array bounds check is done in the first if sentence.)
        iWlanInfo = *(iWlanArray->At( iIndex ) );
        FormatAndAppenWlanIapNameL();
        FormatAndAppenWlanSsidL();
        FormatAndAppenSingnalL();
        FormatAndAppenModeL();
        FormatAndAppenSecurityL();
        FormatAndAppenCoverageL();
        FormatAndAppenSpeedL();
        }
        
    return iFormattedWlanInfo;      
    }

   
    
// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatPreviousWlanInfoL()
// ---------------------------------------------------------------------------
//

CDesCArrayFlat* CWsfDetailsViewModel::FormatPreviousWlanInfoL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatPreviousWlanInfoL" );
    _ASSERTD( iWlanArray );

    if( VisibleWlans( *iWlanArray )  )
        {
        iFormattedWlanInfo->Reset();
        iIndex--;
        
        if ( iIndex < 0 || iWlanInfo.SignalStrength() == ENoSignal )
            {
            iIndex = 0;
            }
        
        // Just in case the iIndex would somehow point out of iWlanArray...
        if( iIndex >= (TInt)iWlanArray->Count() )
            {
            iIndex = 0;
            }
        iWlanInfo = *(iWlanArray->At( iIndex ) );
        FormatAndAppenWlanIapNameL();
        FormatAndAppenWlanSsidL();
        FormatAndAppenSingnalL();
        FormatAndAppenModeL();
        FormatAndAppenSecurityL();
        FormatAndAppenCoverageL();
        FormatAndAppenSpeedL();
        }
    return iFormattedWlanInfo;
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatPaneTextLC()
// ---------------------------------------------------------------------------
//
HBufC* CWsfDetailsViewModel::FormatPaneTextLC()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatPaneTextLC" );

    HBufC* paneText = NULL;
    
    if ( iWlanInfo.SignalStrength() == ENoSignal )
        {
        paneText = KNullDesC().AllocLC();
        }
    else
        {
        TInt visibleWlans = VisibleWlans( *iWlanArray );
        if( visibleWlans )
            {
            paneText = HBufC::NewLC( KPaneFormater().Length() + 
                                     KNumberLegth + 
                                     KNumberLegth );
                                        
            TPtr paneTextPrt = paneText->Des();

            // Format string ( KListItemFormat = %i/%i )
            paneTextPrt.Format( KPaneFormater, 
                                iIndex + 1, 
                                visibleWlans );
            }
        else
            {
            paneText = KNullDesC().AllocLC();
            }        
        }
        
    return paneText;
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatLeftScrollButton()
// ---------------------------------------------------------------------------
//
TBool CWsfDetailsViewModel::FormatLeftScrollButton()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatLeftScrollButton" );
    TBool leftButton( EFalse );
    
    if ( iIndex == 0 )
        {
        leftButton = ETrue;
        }
    
    return leftButton;
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatRightScrollButton()
// ---------------------------------------------------------------------------
//
TBool CWsfDetailsViewModel::FormatRightScrollButton()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatRightScrollButton" );
    TBool rightButton( EFalse );
    TInt count = iWlanArray->Count();
    
    if ( iIndex + 1 >= count )
        {
        rightButton = ETrue;
        }
    
    return rightButton;
    }
    
// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::GetWlanDetails()
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfDetailsViewModel::GetWlanDetails()
    {
    return iFormattedWlanInfo;
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::WlanSsid()
// ---------------------------------------------------------------------------
//
const TDesC8& CWsfDetailsViewModel::WlanSsid() const
    {
    return iWlanInfo.iSsid;
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatListBoxItemLC
// ---------------------------------------------------------------------------
//
HBufC* CWsfDetailsViewModel::FormatListBoxItemLC( 
                                           const TUint aResourceId, 
                                           const TDesC& aValueText ) const
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatListBoxItemLC" );
  // hold the column name on list item
    HBufC* columnName = NULL;
 
    if ( aResourceId )
        {
        columnName = StringLoader::LoadLC( aResourceId );
        }
    else
        {
        columnName = KNullDesC().AllocLC();
        }


    // Holds the list box item
    HBufC* listBoxItem = HBufC::NewL( KListItemFormat().Length()+
                                      columnName->Length() + 
                                      aValueText.Length() );

    // For formating needs
    TPtr listBoxItemPrt = listBoxItem->Des();

    // Format string ( KListItemFormat = %S\t%S )
    listBoxItemPrt.Format( KListItemFormat, columnName, &aValueText );
    CleanupStack::PopAndDestroy( columnName );
    CleanupStack::PushL( listBoxItem );
    return listBoxItem;
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatAndAppenWlanIapNameL()
// ---------------------------------------------------------------------------
//
void CWsfDetailsViewModel::FormatAndAppenWlanIapNameL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatAndAppenWlanIapNameL" );
    if ( iWlanInfo.iNetworkName.Length() )
        {
        HBufC* wlanIapName = iWlanInfo.GetIapNameAsUnicodeLC();
        HBufC* listBoxItem = FormatListBoxItemLC( 
                                R_QTN_CMON_HEADING_CONN_NAME, 
                                *wlanIapName );
                                              
        iFormattedWlanInfo->AppendL( *listBoxItem );
        CleanupStack::PopAndDestroy( listBoxItem );
        CleanupStack::PopAndDestroy( wlanIapName );
        }
    }
    
// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatAndAppenWlanSsidL()
// ---------------------------------------------------------------------------
//
void CWsfDetailsViewModel::FormatAndAppenWlanSsidL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatAndAppenWlanSsidL" );
    HBufC* wlanSsid = iWlanInfo.GetSsidAsUnicodeLC();
    HBufC* listBoxItem = FormatListBoxItemLC( 
                                R_QTN_SNIFFER_HEADING_WLAN_NETWORK_NAME, 
                                *wlanSsid );
                                              
    iFormattedWlanInfo->AppendL( *listBoxItem );
    CleanupStack::PopAndDestroy( listBoxItem );
    CleanupStack::PopAndDestroy( wlanSsid );
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatAndAppenSingnalL()
// ---------------------------------------------------------------------------
//
void CWsfDetailsViewModel::FormatAndAppenSingnalL() 
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatAndAppenSingnalL" );    
    HBufC* singnalStrength = FormatSingnalStrenghtLC();
    HBufC* listBoxItem = FormatListBoxItemLC( 
                                R_QTN_SNIFFER_HEADING_WLAN_SIGNAL_STRENGTH, 
                                *singnalStrength );
                                              
    iFormattedWlanInfo->AppendL( *listBoxItem );
    CleanupStack::PopAndDestroy( listBoxItem );
    CleanupStack::PopAndDestroy( singnalStrength );
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatAndAppenModeL()
// ---------------------------------------------------------------------------
//
void CWsfDetailsViewModel::FormatAndAppenModeL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatAndAppenModeL" ); 
    HBufC* mode = FormatModeLC();
    HBufC* listBoxItem = FormatListBoxItemLC( 
                                R_QTN_SNIFFER_HEADING_WLAN_NETWORK_MODE, 
                                *mode );
                                              
    iFormattedWlanInfo->AppendL( *listBoxItem );
    CleanupStack::PopAndDestroy( listBoxItem );
    CleanupStack::PopAndDestroy( mode );
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatAndAppenSecurityL()
// ---------------------------------------------------------------------------
//
void CWsfDetailsViewModel::FormatAndAppenSecurityL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatAndAppenSecurityL" );
    HBufC* security = FormatSecurityLC();
    HBufC* listBoxItem = FormatListBoxItemLC( 
                                R_QTN_SNIFFER_HEADING_WLAN_SECURITY_MODE, 
                                *security );
                                              
    iFormattedWlanInfo->AppendL( *listBoxItem );
    CleanupStack::PopAndDestroy( listBoxItem );
    CleanupStack::PopAndDestroy( security );
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatAndAppenCoverageL()
// ---------------------------------------------------------------------------
//
void CWsfDetailsViewModel::FormatAndAppenCoverageL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatAndAppenCoverageL" );
    HBufC* coverage = FormatCoverageLC();
    HBufC* listBoxItem = FormatListBoxItemLC( 
                                R_QTN_SNIFFER_HEADING_WLAN_NW_AP_AMOUNT, 
                                *coverage );
                                              
    iFormattedWlanInfo->AppendL( *listBoxItem );
    CleanupStack::PopAndDestroy( listBoxItem );
    CleanupStack::PopAndDestroy( coverage );
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatAndAppenSpeedL()
// ---------------------------------------------------------------------------
//
void CWsfDetailsViewModel::FormatAndAppenSpeedL()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatAndAppenSpeedL" );
    HBufC* speed = FormatTechnologyLC();
    HBufC* listBoxItem = FormatListBoxItemLC( 
                                R_QTN_SNIFFER_HEADING_WLAN_NW_MAX_RATE, 
                                *speed );
                                              
    iFormattedWlanInfo->AppendL( *listBoxItem );
    CleanupStack::PopAndDestroy( listBoxItem );
    CleanupStack::PopAndDestroy( speed );
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatSingnalStrenghtLC()
// ---------------------------------------------------------------------------
//
HBufC* CWsfDetailsViewModel::FormatSingnalStrenghtLC()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatSingnalStrenghtLC" );
    // Holds the showed signal strenth string
    HBufC* singnalStrengthString = NULL;
       
    // Get string
    switch( iWlanInfo.SignalStrength() )
        {
        case ENoSignal:
            {
            singnalStrengthString = StringLoader::LoadLC( 
                R_QTN_SNIFFER_WLAN_SIGNAL_STRENGTH_NO_SIGNAL, iCoeEnv );   
            break; 
            }
        case EPoor:
            {
            singnalStrengthString = StringLoader::LoadLC ( 
                                    R_QTN_SNIFFER_WLAN_SIGNAL_STRENGTH_LOW, 
                                    iWlanInfo.SignalStrengthPercentage(), 
                                    iCoeEnv );                   
            break;
            }
        case EAverage:
            {
            singnalStrengthString = StringLoader::LoadLC ( 
                                R_QTN_SNIFFER_WLAN_SIGNAL_STRENGTH_MEDIUM, 
                                iWlanInfo.SignalStrengthPercentage(), 
                                iCoeEnv );                                 
            break;
            }
        case EExcelent:
            {
            singnalStrengthString = StringLoader::LoadLC ( 
                                R_QTN_SNIFFER_WLAN_SIGNAL_STRENGTH_STRONG, 
                                iWlanInfo.SignalStrengthPercentage(), 
                                iCoeEnv );                 
            break;
            }
        default:
            {
            return KNullDesC().AllocLC();  
            }

        }    
    return singnalStrengthString;  
    }

// ---------------------------------------------------------
// CWsfDetailsViewModel::FormatModeLC
// ---------------------------------------------------------
//
HBufC* CWsfDetailsViewModel::FormatModeLC()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatModeLC" );
    TInt resourceId(0);
    
    switch( iWlanInfo.iNetMode )
        {
        case EAdhoc:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_NEWTWORK_MODE_ADHOC;
            break;
            }

        case EInfra:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_NEWTWORK_MODE_INFRA;
            break;
            }
        default:
            {
            return KNullDesC().AllocLC();  
            }

        }
        
    return StringLoader::LoadLC( resourceId ); 
    }

// ---------------------------------------------------------
// CWsfDetailsViewModel::FormatSecurityLC
// ---------------------------------------------------------
//
HBufC* CWsfDetailsViewModel::FormatSecurityLC()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatSecurityLC" );
    TInt resourceId(0);
    
    switch( iWlanInfo.iSecurityMode )
        {
        
        case EWlanSecModeOpen:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_SECURITY_MODE_OPEN;
            break;
            }
        case EWlanSecModeWep:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_SECURITY_MODE_WEP;
            break;
            }
        case EWlanSecMode802_1x:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_SECURITY_MODE_802_1X;
            break;
            }
        case EWlanSecModeWpa:
        case EWlanSecModeWpa2:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_SECURITY_MODE_WPA;
            break;
            }
        case EWlanSecModeWAPI:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_SECURITY_MODE_WAPI;
            break;
            }
        default:
            {
            return KNullDesC().AllocLC();  
            }
        }

    return StringLoader::LoadLC( resourceId );        
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::FormatCoverageLC
// ---------------------------------------------------------------------------
//
HBufC* CWsfDetailsViewModel::FormatCoverageLC()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatCoverageLC" );
    if ( iWlanInfo.iCoverage == 1)
        {
        return StringLoader::LoadLC( 
                            R_QTN_SNIFFER_WLAN_AP_AMOUNT_IN_RANGE, iCoeEnv ); 
        }
    else 
        {
        return StringLoader::LoadLC( 
                            R_QTN_SNIFFER_WLAN_AP_AMOUNT_IN_RANGE_MANY, 
                            iWlanInfo.iCoverage, iCoeEnv );    
        }
    }

// ---------------------------------------------------------
// CWsfDetailsViewModel::FormatTechnologyLC
// ---------------------------------------------------------
//
HBufC* CWsfDetailsViewModel::FormatTechnologyLC()
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::FormatTechnologyLC" );
    TInt resourceId( 0 );
    
    switch( iWlanInfo.iTransferRate )
        {
        
        case 1:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_TECHNOLOGY_802_11B;
            break;
            }
        case 2:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_TECHNOLOGY_802_11G;
            break;
            }
        case 4:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_TECHNOLOGY_802_11BG;
            break;
            }
        case 8:
            {
            resourceId = R_QTN_SNIFFER_WLAN_SETT_TECHNOLOGY_802_11N;
            break;
            }
        default:
            {
            return KNullDesC().AllocLC();
            }
        }

    return StringLoader::LoadLC( resourceId );
    }

// ---------------------------------------------------------------------------
// CWsfDetailsViewModel::VisibleWlans
// ---------------------------------------------------------------------------
//
TInt CWsfDetailsViewModel::VisibleWlans( CWsfWlanInfoArray& aArray )
    {
    LOG_ENTERFN( "CWsfDetailsViewModel::VisibleWlans" );
    TUint i = 0;
    TWsfWlanInfo* wlanInfo;
    LOG_WRITEF( "Number of Wlans: %d", aArray.Count() );
    // The hidden WLAN should be in the end of the aArray.
    while ( i < aArray.Count() )
        {
        wlanInfo = aArray[i];
        if ( wlanInfo->Hidden() && !wlanInfo->Known() )
            {
            break; // Hidden was found. Break to exit the while-loop.
            }
        ++i;                
        }
    
    LOG_WRITEF( "VisibleWlans: %d", i );
    return i;
    }      

// END OF FILE

