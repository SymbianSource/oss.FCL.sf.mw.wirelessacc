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
* Description:  Details view Model
*
*/


//  EXTERNAL INCLUDES
#include <badesca.h>
#include <StringLoader.h> 
#include <wlansniffer.rsg>
#include <coemain.h>
#include <AknUtils.h>
#include <avkon.rsg>
#include <nifvar.h>
#include <rconnmon.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"
#include "wsfconnecteddetailsmodel.h"
#include "wsfwlaninfoarray.h"
#include "wsfactivewaiter.h"
#include "wsflogger.h"


//  LOCAL DEFINITIONS
#ifdef _DEBUG
    _LIT( KDetailsViewModelPanic, "CWsfConnectedDetailsModel" );
    #define _ASSERTD( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KDetailsViewModelPanic, __LINE__) )
#else
    #define _ASSERTD( cond ) {}
#endif //_DEBUG

// the size of the Details Listbox's array
const TInt KWlanInfoGranularity = 3;

// Listbox element format string
_LIT( KListItemFormat, "%S\t%S" ); 

// invalid connection id
const TInt KInvalidConnectionId = 0;


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::CWsfConnectedDetailsModel
// ---------------------------------------------------------------------------
//
CWsfConnectedDetailsModel::CWsfConnectedDetailsModel():
    iIsConnActive( EFalse ),
    iConnectionId( KInvalidConnectionId ),
    iCoeEnv( CCoeEnv::Static() )
    {
    }

// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfConnectedDetailsModel::ConstructL()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::ConstructL" );
    iFormattedWlanInfo = new (ELeave) CDesCArrayFlat( KWlanInfoGranularity );
    iConnectionMonitor.ConnectL();
    FindWlanConnectionL();
    }

// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::NewL
// ---------------------------------------------------------------------------
//
CWsfConnectedDetailsModel* CWsfConnectedDetailsModel::NewL()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::NewL" );
    CWsfConnectedDetailsModel* self = CWsfConnectedDetailsModel::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::NewLC
// ---------------------------------------------------------------------------
//
CWsfConnectedDetailsModel* CWsfConnectedDetailsModel::NewLC()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::NewLC" );
    CWsfConnectedDetailsModel* self = new( ELeave ) CWsfConnectedDetailsModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::~CWsfConnectedDetailsModel
// ---------------------------------------------------------------------------
//
CWsfConnectedDetailsModel::~CWsfConnectedDetailsModel()
    {
    LOG_ENTERFN( 
        "CWsfConnectedDetailsModel::~CWsfConnectedDetailsModel" );
    iConnectionMonitor.CancelAsyncRequest(EConnMonGetUintAttribute);
    iConnectionMonitor.CancelAsyncRequest(EConnMonGetIntAttribute);
    iConnectionMonitor.Close();
    delete iFormattedWlanInfo;
    }


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::FindWlanConnectionL
// ---------------------------------------------------------------------------
//
void CWsfConnectedDetailsModel::FindWlanConnectionL()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::FindWlanConnectionL" );

#ifndef __WINS__    
    // first call, need to get the connection parameters
    iConnectionId = KInvalidConnectionId;
    TUint connectionCount( 0 );
    
    CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewL();
    iConnectionMonitor.GetConnectionCount( connectionCount, waiter->iStatus );
    waiter->WaitForRequest();

    if ( waiter->iStatus.Int() == KErrNone )
        {
        LOG_WRITEF( "connectionCount = %d", connectionCount );
        // find the connection with WLAN bearer 
        // that will be the selected WLAN
        for ( TUint i = 1; i <= connectionCount; ++i )
            {
            TUint connectionId( KInvalidConnectionId );
            TUint subConnectionCount( 0 );
            TConnMonBearerType bearerType( EBearerUnknown );
    
            iConnectionMonitor.GetConnectionInfo( i, 
                                                  connectionId, 
                                                  subConnectionCount );
            iConnectionMonitor.GetIntAttribute( connectionId, 
                                                0, 
                                                KBearer, 
                                                ( TInt& )bearerType, 
                                                waiter->iStatus );
            waiter->WaitForRequest();
            
            if ( waiter->iStatus.Int() == KErrNone && 
                 bearerType == EBearerWLAN )
                {
                LOG_WRITEF( "found connection id = %d", connectionId );
                iConnectionId = connectionId;
                break;
                }
            }
        }
    
    if ( iConnectionId != KInvalidConnectionId )
        {
        iIsConnActive = ETrue;
        
        // now we have the connectiond ID, ask the start time
        LOG_WRITE( "querying start time..." );
        
        TConnMonTimeBuf timeBuf;
        iConnectionMonitor.GetPckgAttribute( iConnectionId, 0, KStartTime,
                                             timeBuf, waiter->iStatus );
        waiter->WaitForRequest();

        if ( waiter->iStatus.Int() == KErrNone )
            {
            iStartTime = timeBuf();
            }
        }
    
    delete waiter;

#else   // __WINS__

    // for testing
    iStartTime.UniversalTime();
    iIsConnActive = ETrue;
    
#endif  // __WINS__    

    }


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::FormatWlanInfoL
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfConnectedDetailsModel::FormatWlanInfoL( 
                                              TWsfWlanInfo* aWlanInfo, 
                                              CWsfWlanInfoArray* aWlanArray )
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::FormatWlanInfoL" );     
    _ASSERTD( aWlanArray );

    // While the system loads the dialog and the connection lost that could
    // cause problems eg.: "No data" displayed
    if ( !aWlanInfo || !VisibleWlans( *aWlanArray ) )
        {
        // bad wlan info or no visible wlans
        return iFormattedWlanInfo;
        }

    iWlanInfo = *aWlanInfo;
    iWlanArray = aWlanArray;

    RefreshCurrentWlanInfoL();
    
    
    return GetWlanDetails();
    }
    
    
// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::RefreshCurrentWlanInfoL
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfConnectedDetailsModel::RefreshCurrentWlanInfoL()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::RefreshCurrentWlanInfoL" ); 
    LOG_WRITEF( "iIsConnActive: %d", iIsConnActive );
    _ASSERTD( iWlanArray );
    if ( iRefreshing )
        {
        return iFormattedWlanInfo;
        }

    // mutual exclusion
    iRefreshing = ETrue;
        
#ifndef __WINS__
    if ( iIsConnActive )
        {
        LOG_WRITEF( "RefreshCurrentWlanInfoL - iConnectionId: %d", 
                    iConnectionId );
        
        CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewL();
        TInt connStat( 0 );
        iConnectionMonitor.GetIntAttribute( iConnectionId, 
                                            0,
                                            KConnectionStatus,
                                            connStat,
                                            waiter->iStatus );
        waiter->WaitForRequest();
        
        LOG_WRITEF( "connStat: %d",  connStat );
        LOG_WRITEF( "status.Int(): %d",  waiter->iStatus.Int() );
        
        if ( waiter->iStatus.Int() != KErrNone )
            {
            delete waiter;
            iRefreshing = EFalse;
            return iFormattedWlanInfo;
            }
        
        delete waiter;
        
        // set the connection status
        LOG_WRITEF( "connStat: %d",  connStat );
        if ( ( connStat == KLinkLayerOpen ) || 
             ( connStat == KConnectionOpen ) )
            {
            iIsConnActive = ETrue;
            }
        else 
            {
            iIsConnActive = EFalse;
            }
        }

#endif // __WINS__

    iFormattedWlanInfo->Reset();

    // create array of connected WLAN data
    FormatAndAppendConnectedWlanSsidL();
    FormatAndAppendDurationL();
    FormatAndAppendTransferredL();

    iRefreshing = EFalse;
    
    return GetWlanDetails();      
    }    


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::GetWlanDetails
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CWsfConnectedDetailsModel::GetWlanDetails()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::GetWlanDetails" );
#ifdef _DEBUG    
    const TInt KThatLongBufferWillMakeDo = 100;
    for( TInt i = 0; i < iFormattedWlanInfo->MdcaCount(); i++)
        {
        TPtrC temp = iFormattedWlanInfo->MdcaPoint( i );
        TBuf<KThatLongBufferWillMakeDo> tempElement = temp;
        LOG_WRITEF( "iFormattedWlanInfo: %S", &tempElement );
        } 
#endif
        
    return iFormattedWlanInfo;
    }


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::WlanSsid
// ---------------------------------------------------------------------------
//
const TDesC8& CWsfConnectedDetailsModel::WlanSsid() const
    {
    return iWlanInfo.iSsid;
    }


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::FormatListBoxItemLC
// ---------------------------------------------------------------------------
//
HBufC* CWsfConnectedDetailsModel::FormatListBoxItemLC( 
                                            const TUint aResourceId, 
                                            const TDesC& aValueText ) const
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::FormatListBoxItemLC" );
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
// CWsfConnectedDetailsModel::FormatAndAppendConnectedWlanSsidL
// ---------------------------------------------------------------------------
//
void CWsfConnectedDetailsModel::FormatAndAppendConnectedWlanSsidL()
    {
    LOG_ENTERFN( 
        "CWsfConnectedDetailsModel::FormatAndAppendConnectedWlanSsidL" );
    HBufC* wlanName( NULL );
    
    if ( iWlanInfo.iNetworkName.Length() ) // If there is IAP
        {
        wlanName = iWlanInfo.GetIapNameAsUnicodeLC();
        }
    else // If there no IAP
        {
        wlanName = iWlanInfo.GetSsidAsUnicodeLC();
        }
    
    HBufC* listBoxItem = FormatListBoxItemLC( 
                                        R_QTN_WLAN_ITEM_HEADING_TEXT_WLAN, 
                                        *wlanName );
                                              
    iFormattedWlanInfo->AppendL( *listBoxItem );
    CleanupStack::PopAndDestroy( listBoxItem );
    CleanupStack::PopAndDestroy( wlanName );    
    }

    
// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::FormatAndAppendDurationL
// ---------------------------------------------------------------------------
//
void CWsfConnectedDetailsModel::FormatAndAppendDurationL()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::FormatAndAppendDurationL" );
    HBufC* duration = FormatDurationLC();
    HBufC* listBoxItem = FormatListBoxItemLC( 
                                        R_QTN_WLAN_ITEM_HEADING_DURATION, 
                                        *duration );
                                              
    iFormattedWlanInfo->AppendL( *listBoxItem );
    CleanupStack::PopAndDestroy( listBoxItem );
    CleanupStack::PopAndDestroy( duration );    
    }
    

// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::FormatAndAppendTransferredL
// ---------------------------------------------------------------------------
//
void CWsfConnectedDetailsModel::FormatAndAppendTransferredL()
    {
    LOG_ENTERFN( 
        "CWsfConnectedDetailsModel::FormatAndAppendTransferredL" );
    HBufC* transferredData = FormatTransferredLC();
    HBufC* listBoxItem = FormatListBoxItemLC( R_QTN_WLAN_ITEM_HEADING_TRANSF,
                                              *transferredData );
                                              
    iFormattedWlanInfo->AppendL( *listBoxItem );
    CleanupStack::PopAndDestroy( listBoxItem );
    CleanupStack::PopAndDestroy( transferredData );    
    }
    

// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::FormatDurationLC
// ---------------------------------------------------------------------------
//
HBufC* CWsfConnectedDetailsModel::FormatDurationLC()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::FormatDurationLC" );
    const TUint KTimeStringLength = 64;

    if ( iIsConnActive )
        {
        // if connection is still active, update the duration
        TTime now;
        now.UniversalTime();
        iDuration = now.MicroSecondsFrom( iStartTime );
        }
    
    HBufC* timeString = HBufC::NewLC( KTimeStringLength );
    TPtr16 tempBuf( timeString->Des() );

    HBufC* temp = StringLoader::LoadLC( R_QTN_TIME_DURAT_LONG_WITH_ZERO );
    
    TTime activeTime( iDuration.Int64() );
    activeTime.FormatL( tempBuf, *temp );
    CleanupStack::PopAndDestroy( temp );

    TPtr timePtr = timeString->Des();
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( timePtr );

    return timeString;    
    }


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::FormatTransferredLC
// ---------------------------------------------------------------------------
//
HBufC* CWsfConnectedDetailsModel::FormatTransferredLC()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::FormatTransferredLC" );
        
#ifndef __WINS__
    CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewL();

    TUint uplink( 0 );  
    TUint downlink( 0 );        

    if ( iIsConnActive && iConnectionId != KInvalidConnectionId )
        {
        // if connection is still active, update the transmitted bytes
        iConnectionMonitor.GetUintAttribute( iConnectionId, 0, KUplinkData,
                                              uplink, waiter->iStatus );
        waiter->WaitForRequest();
        }
    
    if ( waiter->iStatus.Int() == KErrNone
            && iIsConnActive
            && iConnectionId != KInvalidConnectionId )
        {
        iConnectionMonitor.GetUintAttribute( iConnectionId, 0, KDownlinkData,
                                              downlink, waiter->iStatus );     
        waiter->WaitForRequest();

        }

    iBytesTransmitted = uplink + downlink;
    delete waiter;
    
#endif
               
    const TUint KTransferredBytesFormatBuf = 16;
    const TUint KTransferredBytesWidth = 10;
    const TUint KTransferredBytesDecimalPlaces = 2;
    const TUint KKiloByte = 1024;
    TUint resId;

    TBuf<KTransferredBytesFormatBuf> buf;

    TReal denom( KKiloByte );

    if ( iBytesTransmitted < KKiloByte*KKiloByte )
        {
        denom = KKiloByte;
        resId = R_QTN_WLAN_ITEM_KB;
        }
    else
        {
        denom = KKiloByte*KKiloByte;
        resId = R_QTN_WLAN_ITEM_MB;
        }

    TReal value = TReal( iBytesTransmitted ) / denom;

    TRealFormat realFormat( KTransferredBytesWidth, 
                            KTransferredBytesDecimalPlaces );

    HBufC* number = HBufC::NewLC( KTransferredBytesWidth );
    number->Des().Num( value, realFormat );

    _LIT( KUdFormatLarge, "%S" );
    buf.Format( KUdFormatLarge, number );
    CleanupStack::PopAndDestroy( number );

    HBufC* loadedTxt = StringLoader::LoadLC( resId, buf, iCoeEnv );
    
           
    // Convert digits
    TPtr loadTextPtr( loadedTxt->Des() );
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( loadTextPtr );
    
    return loadedTxt;                 
    }
    
    
// ---------------------------------------------------------------------------
// CWsfConnectedDetailsModel::VisibleWlans
// ---------------------------------------------------------------------------
//
TInt CWsfConnectedDetailsModel::VisibleWlans( CWsfWlanInfoArray& aArray )
    {
    LOG_ENTERFN( "CWsfConnectedDetailsModel::VisibleWlans" );    
    TInt i = 0;
    TBool hiddenFound = EFalse;
    TWsfWlanInfo* wlanInfo;
    while ( i < aArray.Count() && !hiddenFound )
        {
        wlanInfo = aArray[i];
        if ( wlanInfo->Hidden() && !wlanInfo->Known() )
            {
            hiddenFound = ETrue;
            }
        else
            {
            ++i;                
            }
        }
    return i;
    }      

// END OF FILE

