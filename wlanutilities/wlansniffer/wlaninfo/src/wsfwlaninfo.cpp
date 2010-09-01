/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of TWsfWlanInfo
*
*/



//  EXTERNAL INCLUDES
#include <utf.h>
#include <s32strm.h>

//  CLASS HEADER
#include "wsfwlaninfo.h"


using namespace CMManager;


// Percentage max value
static const TInt KMaxPercentage = 100;



// ---------------------------------------------------------------------------
// TWsfWlanInfo::TWsfWlanInfo
// ---------------------------------------------------------------------------
//    
EXPORT_C TWsfWlanInfo::TWsfWlanInfo():
    iSsid( KNullDesC8 ),
	iStrengthLevel( EWlanSignalUnavailable ),
    iSecurityMode( EWlanSecModeOpen ),
    iVisibility( EFalse ),
    iNetMode( EAdhoc ),
    iIapId( 0 ),
    iCoverage( 0 ),
    iTransferRate( 0 ),
    iConnectionState( ENotConnected ),
    iFilterFlags( 0 ),
    iBrandId( 0 ),
    iNetworkName( KNullDesC8 ),
    iPriority ( 0 ),
    iRawSsid ( KNullDesC8 )
    {
    }


// ---------------------------------------------------------------------------
// TWsfWlanInfo::TWsfWlanInfo
// ---------------------------------------------------------------------------
//    
EXPORT_C TWsfWlanInfo::TWsfWlanInfo( TDesC8& aSsid,
                        TUint16 aStrengthLevel,
                        TWlanSecMode aSecurityMode,
                        TInt aVisibility,
                        TWlanNetMode aNetMode,
                        TUint32 aIapId,
                        TUint8 aCoverage,
                        TUint32 aTransferRate ):
    iSsid( aSsid ),
    iStrengthLevel( aStrengthLevel ),
    iSecurityMode( aSecurityMode ),
    iVisibility( aVisibility ),
    iNetMode( aNetMode ),
    iIapId( aIapId ),
    iCoverage( aCoverage ),
    iTransferRate( aTransferRate ),
    iConnectionState( ENotConnected ),
    iFilterFlags( 0 ),
    iBrandId( 0 ),
    iPriority( 0 ),
    iRawSsid ( KNullDesC8 )
    {
    }

// ---------------------------------------------------------------------------
// TWsfWlanInfo::GetSsidAsUnicodeLC
// ---------------------------------------------------------------------------
//    
EXPORT_C HBufC* TWsfWlanInfo::GetSsidAsUnicodeLC()
    {
    return GetSsidAsUnicodeLC( iSsid );
    }


// ---------------------------------------------------------------------------
// TWsfWlanInfo::GetSsidAsUnicodeLC
// ---------------------------------------------------------------------------
//    
EXPORT_C HBufC* TWsfWlanInfo::GetSsidAsUnicodeLC( const TWlanSsid& aSsid )
    {
    HBufC* ssid = HBufC::NewLC( aSsid.Length() );
    TPtr ptr = ssid->Des();
    TInt err = CnvUtfConverter::ConvertToUnicodeFromUtf8( ptr, aSsid );
    if ( err != KErrNone )
        {
        // If ssid wasn't utf-8 encoded, assume it unicode already
        // it's not specified how WLAN SSIDs should be encoded.
        ptr.Copy( aSsid );
        }
    return ssid;
    }


// ---------------------------------------------------------------------------
// TWsfWlanInfo::GetSsidAsUtf8LC
// ---------------------------------------------------------------------------
//    
EXPORT_C HBufC8* TWsfWlanInfo::GetSsidAsUtf8LC()
    {
    HBufC8* ssid = iSsid.AllocLC();
    return ssid;
    }


// ---------------------------------------------------------------------------
// TWsfWlanInfo::InternalizeL
// ---------------------------------------------------------------------------
//    
EXPORT_C void TWsfWlanInfo::InternalizeL( RReadStream& aStream )
	{
	aStream >> iSsid;
	iStrengthLevel = aStream.ReadInt16L();
	iSecurityMode = TWlanSecMode( aStream.ReadInt32L() );
	iVisibility = aStream.ReadInt8L();
	iNetMode = TWlanNetMode( aStream.ReadInt32L() );
	iIapId = aStream.ReadInt32L();
	iCoverage = aStream.ReadInt8L();
	iTransferRate = aStream.ReadInt32L();
	iConnectionState = TWsfWlanConnectionState( aStream.ReadInt8L() );
	iFilterFlags = aStream.ReadInt8L();
	iBrandId = aStream.ReadInt8L();
	iPriority = aStream.ReadInt8L();
	aStream >> iNetworkName;
	aStream >> iRawSsid;
	}


// ---------------------------------------------------------------------------
// TWsfWlanInfo::ExternalizeL
// ---------------------------------------------------------------------------
//    
EXPORT_C void TWsfWlanInfo::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iSsid;
	aStream.WriteInt16L( iStrengthLevel );
	aStream.WriteInt32L( iSecurityMode );
	aStream.WriteInt8L( iVisibility );
	aStream.WriteInt32L( iNetMode );
	aStream.WriteInt32L( iIapId );
	aStream.WriteInt8L( iCoverage );
	aStream.WriteInt32L( iTransferRate );
	aStream.WriteInt8L( iConnectionState );
	aStream.WriteInt8L( iFilterFlags );
	aStream.WriteInt8L( iBrandId );
	aStream.WriteInt8L( iPriority );
	aStream << iNetworkName;
	aStream << iRawSsid;
	}


// ---------------------------------------------------------------------------
// TWsfWlanInfo::SignalStrength
// ---------------------------------------------------------------------------
//    
EXPORT_C TWsfWlanSignalStrengthLevel TWsfWlanInfo::SignalStrength()
	{
	// check the absolute signal level and return symbolic representation
	// smaller value means stronger signal
	TWsfWlanSignalStrengthLevel level( EAverage );
	
    if ( iStrengthLevel < EWlanSignalStrengthGood )
        {
        level = EExcelent;
        }
    else if ( iStrengthLevel > EWlanSignalStrengthLow )
        {
        if ( iStrengthLevel == EWlanSignalUnavailable )
            {
            level = ENoSignal;
            }
        else
            {
            level = EPoor;
            }
        }

    return level;
	}


// ---------------------------------------------------------------------------
// TWsfWlanInfo::SignalStrengthPercentage
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt TWsfWlanInfo::SignalStrengthPercentage()
	{
	// smaller value means stronger signal
	TInt signalStrength( 0 );
	
    if ( iStrengthLevel <= EWlanSignalStrengthMax )
        {
        signalStrength = KMaxPercentage;
        }
    else if ( iStrengthLevel < EWlanSignalStrengthMin )
        {
        TInt diff = EWlanSignalStrengthMin - iStrengthLevel;
        signalStrength = diff * KMaxPercentage / 
                           ( EWlanSignalStrengthMin - EWlanSignalStrengthMax );
        }
        
    return signalStrength;
	}


// ---------------------------------------------------------------------------
// TWsfWlanInfo::GetSsidAsUnicodeLC
// ---------------------------------------------------------------------------
//    
EXPORT_C HBufC* TWsfWlanInfo::GetIapNameAsUnicodeLC()
    {
    // This is essentially the same as ssid conversion
    return GetSsidAsUnicodeLC( iNetworkName );
    }


// ---------------------------------------------------------------------------
// TWsfWlanInfo::GetSsidAsUtf8LC
// ---------------------------------------------------------------------------
//    
EXPORT_C HBufC8* TWsfWlanInfo::GetIapNameAsUtf8LC()
    {
    HBufC8* networkName = iNetworkName.AllocLC();
    return networkName;
    }


// End of file

