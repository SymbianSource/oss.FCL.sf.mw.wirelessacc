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
* Description:   Implementation of CWsfWlanIapCreator
*
*/




//  EXTERNAL INCLUDES
#include <wlancontainer.h>
#include <EapType.h>
#include <cmdestinationext.h>
#include <utf.h>

//  CLASS HEADER
#include "wsfwlaniapcreator.h"

//  INTERNAL INCLUDES
#include "wsfactivewaiter.h"
#include "wsflogger.h"


using namespace CMManager;


/**
* Order of first WEP key
*/
static const TInt KFirstWepKey = 0;

/**
* WEP authentication modes
*/
static const TInt KWepAuthOpen = 0;        // Open authentication

/**
* Max length of WPA key
*/
static const TUint KWpaKeyMaxLength = 64;
    
/**
* Max length of WEP key
*/
static const TUint KMaxWepKeyLen = 26; 



  

// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::NewL
// ----------------------------------------------------------------------------
//
CWsfWlanIapCreator* CWsfWlanIapCreator::NewL()
    {
    CWsfWlanIapCreator* thisPtr = NewLC();
    CleanupStack::Pop( thisPtr );
    return thisPtr;         
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::NewLC
// ----------------------------------------------------------------------------
//
CWsfWlanIapCreator* CWsfWlanIapCreator::NewLC()
    {
    CWsfWlanIapCreator* thisPtr = new (ELeave) CWsfWlanIapCreator();
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    return thisPtr;    
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::~CWsfWlanIapCreator
// ----------------------------------------------------------------------------
//
CWsfWlanIapCreator::~CWsfWlanIapCreator()
    {
    delete iDbSession;
    iCmManagerExt.Close();
    }
    

// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::CWsfWlanIapCreator
// ----------------------------------------------------------------------------
//
CWsfWlanIapCreator::CWsfWlanIapCreator()     
    {
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfWlanIapCreator::ConstructL()
    {
    iDbSession = CommsDat::CMDBSession::NewL( CMDBSession::LatestVersion() );
    iCmManagerExt.OpenL();
    iDestinationId = 0;
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::StoreWPADataL
// ----------------------------------------------------------------------------
//
void CWsfWlanIapCreator::StoreWPADataL( const TInt aIapId, 
                                     const TDesC& aPresharedKey, 
                                     const CMManager::TWlanSecMode aSecMode )
    {
    LOG_ENTERFN( "CWsfWlanIapCreator::StoreWPADataL" );
    
    CCDIAPRecord *iapRec = static_cast<CCDIAPRecord*>
                          ( CCDRecordBase::RecordFactoryL( KCDTIdIAPRecord ) );
    
    CleanupStack::PushL( iapRec );
    
    iapRec->SetRecordId( aIapId );
    iapRec->LoadL( *iDbSession );
        
    TUint32 serviceId = TUint32( iapRec->iService );

    LOG_WRITEF("IAP(%d).serviceID = %d", aIapId, serviceId );
    
    // we have the service id, now we can flush the iap record
    CleanupStack::PopAndDestroy( iapRec );

    // search for the record    
    CMDBGenericRecord* generic = static_cast<CMDBGenericRecord*>( 
                                          CCDRecordBase::RecordFactoryL( 0 ) );
    CleanupStack::PushL( generic );    
    generic->InitializeL( TPtrC( WLAN_SERVICE ), NULL );
    generic->LoadL( *iDbSession );
    
    CMDBField<TUint32>* sidField = static_cast<CMDBField<TUint32>*>
                             ( generic->GetFieldByIdL( KCDTIdWlanServiceId ) );
    
    // prime with service id                
    *sidField = serviceId;
    
    TBool found = generic->FindL( *iDbSession );

    LOG_WRITE( "saving common WPA settings..." );
    TBool usesPsk( aPresharedKey.Length() > 0 );

    // Save PreShared Key as 8 bit string
    TBuf8<KWpaKeyMaxLength> keyWPA;
    keyWPA.Copy( aPresharedKey );
    TInt len( keyWPA.Length() );
    LOG_WRITEF( "PSK:[%S] len = %d", &aPresharedKey, len );


    CMDBField<TUint>* enableWpaPskField = static_cast<CMDBField<TUint>*>
                          ( generic->GetFieldByIdL( KCDTIdWlanEnableWpaPsk ) );
    enableWpaPskField->SetL( usesPsk );

    CMDBField<TUint>* secModeField = static_cast<CMDBField<TUint>*>
                        ( generic->GetFieldByIdL( KCDTIdWlanSecMode ) );
    secModeField->SetL( aSecMode );

    CMDBField<TDesC8>* wpaPskField = static_cast<CMDBField<TDesC8>*>
                       ( generic->GetFieldByIdL( KCDTIdWlanWpaPreSharedKey ) );
    wpaPskField->SetL( keyWPA );
        
    // Save PreShared Key length
    CMDBField<TUint>* keyLengthField = static_cast<CMDBField<TUint>*>
                        ( generic->GetFieldByIdL( KCDTIdWlanWpaKeyLength ) );
    keyLengthField->SetL( keyWPA.Length() );


    if ( !usesPsk )
        {
        // not PSK -> must be EAP
        LOG_WRITE( "saving EAP info..." );
        RImplInfoPtrArray eapArray;
        REComSession::ListImplementationsL( KEapTypeInterfaceUid, eapArray );
        CleanupClosePushL( eapArray );
        
        for ( TInt i = 0; i < eapArray.Count(); ++i )
            {
            // IsDisallowedOutsidePEAP actually means IsDisallowedOutsideTunnel
            if ( !CEapType::IsDisallowedOutsidePEAP( *eapArray[i] ) )
                {
                CEapType* eapType = CEapType::NewL( eapArray[i]->DataType(), 
                                                    ELan, 
                                                    serviceId );
                CleanupStack::PushL( eapType );
                
                eapType->SetIndexL( ELan, serviceId );
                CleanupStack::PopAndDestroy( eapType );
                }
            }

        eapArray.ResetAndDestroy();
        CleanupStack::PopAndDestroy( &eapArray );
        }

    if ( !found )
        {
        // there wasn't any wlan service record, we have to create it now
        LOG_WRITE("new service record");
        generic->SetRecordId( KCDNewRecordRequest );
        generic->StoreL( *iDbSession );
        }
    else
        {
        // modify existing record
        LOG_WRITE("existing service record");
        generic->ModifyL( *iDbSession );
        }
    
    CleanupStack::PopAndDestroy( generic );
    }
    

// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::StoreWEPDataL
// ----------------------------------------------------------------------------
//
void CWsfWlanIapCreator::StoreWEPDataL( const TInt aIapId, 
                                        const TDesC& aWepKey, 
                                        const TBool aIsHexWep )
    {
    LOG_ENTERFN( "CWsfWlanIapCreator::StoreWEPDataL" );
    
    CCDIAPRecord *iapRec = static_cast<CCDIAPRecord*>
                          ( CCDRecordBase::RecordFactoryL( KCDTIdIAPRecord ) );
    
    CleanupStack::PushL( iapRec );
    
    iapRec->SetRecordId( aIapId );
    iapRec->LoadL( *iDbSession );

    TUint32 serviceId = TUint32( iapRec->iService );
    LOG_WRITEF("IAP(%d).serviceID = %d", aIapId, serviceId );
    
    CleanupStack::PopAndDestroy( iapRec );


    CMDBGenericRecord* generic = static_cast<CMDBGenericRecord*>
        ( CCDRecordBase::RecordFactoryL( 0 ) );
    CleanupStack::PushL( generic );    
    generic->InitializeL( TPtrC( WLAN_SERVICE ), NULL );
    generic->LoadL( *iDbSession );
    
    CMDBField<TUint32>* sidField = static_cast<CMDBField<TUint32>*>
                             ( generic->GetFieldByIdL( KCDTIdWlanServiceId ) );
    
    // prime with service id                
    *sidField = serviceId;
    
    TBool found = generic->FindL( *iDbSession );
    
    LOG_WRITE( "saving WEP settings..." );


    CMDBField<TUint>* indexField = static_cast<CMDBField<TUint>*>
                            ( generic->GetFieldByIdL( KCDTIdWlanWepIndex ) );
    indexField->SetL( KFirstWepKey );

    // Save authentication mode    
    CMDBField<TUint>* authenticationField = static_cast<CMDBField<TUint>*>
                            ( generic->GetFieldByIdL( KCDTIdWlanAuthMode ) );
    authenticationField->SetL( KWepAuthOpen );
    
    

    // now we need to convert the key to 8bit and to hex
    // and again detect the required bits
    TBuf8<KMaxWepKeyLen> key;
    
    // convert to 8 bit
    key.Copy( aWepKey );

       
    if ( !aIsHexWep )
        {
        // Must be converted to hexa and stored as a hexa
        // Ascii key is half the length of Hex
        HBufC8* buf8Conv = HBufC8::NewLC( key.Length()*2 );
        TPtr8 bufptr( buf8Conv->Des() );
        ConvertAsciiToHex( key, bufptr );
        
        CMDBField<TDesC8>* wepKey1Field = static_cast<CMDBField<TDesC8>*>
                            ( generic->GetFieldByIdL( KCDTIdWlanWepKey1 ) );
        wepKey1Field->SetL( buf8Conv->Des() );

        CleanupStack::PopAndDestroy( buf8Conv );
        }
    else
        {
        // already in hexa format
        CMDBField<TDesC8>* wepKey1Field = static_cast<CMDBField<TDesC8>*>
                            ( generic->GetFieldByIdL( KCDTIdWlanWepKey1 ) );
        wepKey1Field->SetL( key );
        }
        
    
     // write default values to the rest of the columns
    key.Zero();
    
    CMDBField<TDesC8>* wepKey2Field = static_cast<CMDBField<TDesC8>*>
                        ( generic->GetFieldByIdL( KCDTIdWlanWepKey2 ) );
    wepKey2Field->SetL( key );
    CMDBField<TDesC8>* wepKey3Field = static_cast<CMDBField<TDesC8>*>
                        ( generic->GetFieldByIdL( KCDTIdWlanWepKey3 ) );
    wepKey3Field->SetL( key );
    CMDBField<TDesC8>* wepKey4Field = static_cast<CMDBField<TDesC8>*>
                        ( generic->GetFieldByIdL( KCDTIdWlanWepKey4 ) );
    wepKey4Field->SetL( key );


    // write format data
    TUint fmt( aIsHexWep );
    
    CMDBField<TUint>* formatKey1Field = static_cast<CMDBField<TUint>*>
                            ( generic->GetFieldByIdL( KCDTIdWlanFormatKey1 ) );
    formatKey1Field->SetL( fmt );
    CMDBField<TUint>* formatKey2Field = static_cast<CMDBField<TUint>*>
                            ( generic->GetFieldByIdL( KCDTIdWlanFormatKey2 ) );
    formatKey2Field->SetL( fmt );
    CMDBField<TUint>* formatKey3Field = static_cast<CMDBField<TUint>*>
                            ( generic->GetFieldByIdL( KCDTIdWlanFormatKey3 ) );
    formatKey3Field->SetL( fmt );
    CMDBField<TUint>* formatKey4Field = static_cast<CMDBField<TUint>*>
                            ( generic->GetFieldByIdL( KCDTIdWlanFormatKey4 ) );
    formatKey4Field->SetL( fmt );
    
    
    if ( !found )
        {
        // there wasn't any wlan service record, we have to create it now
        LOG_WRITE("new service record");
        generic->SetRecordId( KCDNewRecordRequest );
        generic->StoreL( *iDbSession );
        }
    else
        {
        // modify existing record
        LOG_WRITE("existing service record");
        generic->ModifyL( *iDbSession );
        }
        
    CleanupStack::PopAndDestroy( generic );    
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::ConvertAsciiToHex
// ----------------------------------------------------------------------------
//
void CWsfWlanIapCreator::ConvertAsciiToHex( const TDesC8& aSource, 
                                            TDes8& aDest )
    {
    LOG_ENTERFN( "CWsfWlanIapCreator::ConvertAsciiToHex" );
    _LIT8( hex, "0123456789ABCDEF" );
    TInt size = aSource.Size();
    aDest.Zero();
    
    for ( TInt ii = 0; ii < size; ii++ )
        {
        TText8 ch = aSource[ii];
        aDest.Append( hex()[(ch/16)&0x0f] );
        aDest.Append( hex()[ch&0x0f] );
        }
    }
    


// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::SetDefaultDestination
// ----------------------------------------------------------------------------
//
void CWsfWlanIapCreator::SetDefaultDestination( const TUint32 aDestinationId )
    {
    LOG_ENTERFN( "CWsfWlanIapCreator::SetDefaultDestination" );
    LOG_WRITEF( "destId = %d", aDestinationId );
    iDestinationId = aDestinationId;
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::CreateAccessPointL
// ----------------------------------------------------------------------------
//
void CWsfWlanIapCreator::CreateAccessPointL( TWsfWlanInfo& aWlanInfo, 
                                                const TDesC& aPresharedKey, 
                                                const TBool aIsHex )
    {
    LOG_ENTERFN( "CWsfWlanIapCreator::CreateAccessPointL" );
    TUint32 iapId( 0 );

    // create connection method
    iapId = CreateConnectionMethodL( aWlanInfo );
    

    // open a transaction
    const TUint KMaxOpenTransAttempts = 10;
    const TUint KRetryAfter = 100000;

    TInt err( KErrNone );
    TUint attempts( KMaxOpenTransAttempts );
    
    CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewLC();
    RTimer timer;
    timer.CreateLocal();
    CleanupClosePushL( timer );
    
    do
        {
        TRAP( err, iDbSession->OpenTransactionL() );
        if ( err )
            {
            timer.After( waiter->iStatus, 
                         TTimeIntervalMicroSeconds32( KRetryAfter ) );
            waiter->WaitForRequest();
            }
        }
    while ( err && attempts-- );

    User::LeaveIfError( err );

    CleanupStack::PopAndDestroy( &timer );
    CleanupStack::PopAndDestroy( waiter );
    
    CleanupStack::PushL( TCleanupItem( RollbackCommsDat, iDbSession ) );

    
    // now we have a valid transaction, save security settings
    
    switch ( aWlanInfo.iSecurityMode )
        {
        case EWlanSecModeWep:
            {
            StoreWEPDataL( iapId, aPresharedKey, aIsHex );
            break;
            }
        case EWlanSecMode802_1x:
        case EWlanSecModeWpa:
        case EWlanSecModeWpa2:
            {
            StoreWPADataL( iapId, aPresharedKey, aWlanInfo.iSecurityMode );
            break;
            }
        case EWlanSecModeOpen: // fall through on purpose
        default:
            {
            }
        }

    CleanupStack::Pop( 1 ); // transaction rollback popped
    
    // commit changes
    if ( iDbSession->IsInTransaction() )
        {
        iDbSession->CommitTransactionL();
        }
        
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::RollbackCommsDat
// ----------------------------------------------------------------------------
void CWsfWlanIapCreator::RollbackCommsDat( TAny* aDbSession )
    {
    CommsDat::CMDBSession* session = static_cast<CommsDat::CMDBSession*>( 
                                                                 aDbSession );
    TRAP_IGNORE( session->RollbackTransactionL() );
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapCreator::CreateConnectionMethodL
// ----------------------------------------------------------------------------
//
TUint32 CWsfWlanIapCreator::CreateConnectionMethodL( TWsfWlanInfo& aWlanInfo )
    {
    LOG_ENTERFN( "CWsfWlanIapCreator::CreateConnectionMethodL" );
    
    HBufC* name16 = NULL;
    HBufC* ssid16 = NULL;
    
    if ( aWlanInfo.iRawSsid.Length() )
        {
        // ssid is from scan result store it as it is 
        // since the encoding of ssid is unknown
        name16 = HBufC::NewLC( aWlanInfo.iRawSsid.Length() );
        name16->Des().Copy( aWlanInfo.iRawSsid );
        ssid16 = HBufC::NewLC( aWlanInfo.iRawSsid.Length() );
        ssid16->Des().Copy( aWlanInfo.iRawSsid );
        }
    else
        {
        // user has inputted ssid store it to name as unicode since it is utf-8 
        name16 = aWlanInfo.GetSsidAsUnicodeLC();
        ssid16 = HBufC::NewLC( aWlanInfo.iSsid.Length() );
        ssid16->Des().Copy( aWlanInfo.iSsid );
        }
        
    TUint32 iapId( 0 );
    
    if ( !iDestinationId )
        {
        // possibly temporary iap to uncategorized
        LOG_WRITE( "destId not set -> temporary into Uncategorised" );
        
        RCmConnectionMethodExt cm = iCmManagerExt.CreateConnectionMethodL( 
                                                         KUidWlanBearerType );
        CleanupClosePushL( cm );
        
        cm.SetStringAttributeL( ECmName, *name16 );
        cm.SetStringAttributeL( EWlanSSID, *ssid16 );
        cm.SetIntAttributeL( EWlanSecurityMode, aWlanInfo.iSecurityMode );
        cm.SetIntAttributeL( EWlanConnectionMode, aWlanInfo.iNetMode );
        cm.SetBoolAttributeL( EWlanScanSSID, !aWlanInfo.iVisibility );
        
        // commit changes
        cm.UpdateL();

        // get the iap id
        iapId = cm.GetIntAttributeL( ECmId );
        
        // now retrieve the name again to see if it has been changed 
        // (ExistingCmName -> ExistingCmName(01) and alike)
        HBufC* cmName = cm.GetStringAttributeL( ECmName );
        
        TInt error = CnvUtfConverter::ConvertFromUnicodeToUtf8( 
                                                        aWlanInfo.iNetworkName, 
                                                        *cmName  );
        if ( error )
            {
            LOG_WRITE( "ConvertFromUnicodeToUtf8 failed");
            aWlanInfo.iNetworkName.Copy( *cmName );
            }

        delete cmName;
        
        CleanupStack::PopAndDestroy( &cm );
        }
    else
        {
        // final iap to the default destination
        LOG_WRITEF( "destId = %d", iDestinationId );
        
        RCmDestinationExt destination = iCmManagerExt.DestinationL( 
                                                            iDestinationId );
        CleanupClosePushL( destination );
        
        RCmConnectionMethodExt cm = destination.CreateConnectionMethodL( 
                                                         KUidWlanBearerType );
        CleanupClosePushL( cm );

        cm.SetStringAttributeL( ECmName, *name16 );
        cm.SetStringAttributeL( EWlanSSID, *ssid16 );
        cm.SetIntAttributeL( EWlanSecurityMode, aWlanInfo.iSecurityMode );
        cm.SetIntAttributeL( EWlanConnectionMode, aWlanInfo.iNetMode );
        cm.SetBoolAttributeL( EWlanScanSSID, !aWlanInfo.iVisibility );
        
        // commit changes
        destination.UpdateL();

        // get the iap id
        iapId = cm.GetIntAttributeL( ECmId );
        
        // now retrieve the name again to see if it has been changed 
        // (ExistingCmName -> ExistingCmName(01) and alike)
        HBufC* cmName = cm.GetStringAttributeL( ECmName );
        TInt error = CnvUtfConverter::ConvertFromUnicodeToUtf8( 
                                                         aWlanInfo.iNetworkName, 
                                                         *cmName  );
        if ( error )
            {
            LOG_WRITE( "ConvertFromUnicodeToUtf8 failed");
            aWlanInfo.iNetworkName.Copy( *cmName );
            }
        
        delete cmName;
        
        CleanupStack::PopAndDestroy( &cm );
        CleanupStack::PopAndDestroy( &destination );
        }

    CleanupStack::PopAndDestroy( ssid16 );
    CleanupStack::PopAndDestroy( name16 );

    aWlanInfo.iIapId = iapId;
    LOG_WRITEF( "new iapId = %d", iapId );

    return iapId;
    }
        

