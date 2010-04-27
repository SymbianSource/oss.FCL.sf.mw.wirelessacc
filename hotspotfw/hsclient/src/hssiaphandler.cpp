/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Implementation of iapsettings
*
*/



// INCLUDE FILES
#include <EapType.h>
#include <cdbpreftable.h>
#include "hssiaphandler.h"
#include "hotspotclientserver.h"
#include "am_debug.h"
#include <es_enum.h>

#include <cmconnectionmethodext.h>
#include <cmconnectionmethoddef.h>
#include <cmmanagerext.h>
#include <cmmanagerdef.h>
#include <cmdestinationext.h>

using namespace CMManager;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHotSpotPluginSession
// -----------------------------------------------------------------------------
//
CHssIapHandler::CHssIapHandler( )
	{
    DEBUG("CHssIapHandler::CHssIapHandler");
	}

// -----------------------------------------------------------------------------
// ConstructL
// -----------------------------------------------------------------------------
//
void CHssIapHandler::ConstructL()
    {
    DEBUG("CHssIapHandler::ConstructL()");
    }

// -----------------------------------------------------------------------------
// NewL
// -----------------------------------------------------------------------------
//
CHssIapHandler* CHssIapHandler::NewL()
    {
    CHssIapHandler* self = new( ELeave ) CHssIapHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// ~CHssIapHandler
// -----------------------------------------------------------------------------
//
CHssIapHandler::~CHssIapHandler()
	{
	DEBUG("CHssIapHandler::~CHssIapHandler()");
    
	if ( iWLANRecord != NULL )
		{
		delete iWLANRecord;
		}
	iWLANRecord = NULL;
	}

// -----------------------------------------------------------------------------
// ChangeSettingsL
// -----------------------------------------------------------------------------
//
TInt CHssIapHandler::ChangeSettingsL( const TUint aIapID, 
                                      const THssIapSettings& aSettings )
    {
    // Wlan table name -field not modified, because it is used to store UIDs
    // of clients. 
    DEBUG("CHssIapHandler::ChangeSettingsL");
    TInt ret( KErrNone );
    
    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );
    
    TUint easyWlanId = cmManager.EasyWlanIdL();
    
    // Easy WLAN can't be modified
    if ( easyWlanId == aIapID )
    	{
        CleanupStack::PopAndDestroy( &cmManager ); 
    	return KErrPermissionDenied;
    	}
    
    // Read WLAN table service id
    TUint32 serviceId(0);
	RCmConnectionMethodExt plugin = cmManager.ConnectionMethodL( aIapID );
	CleanupClosePushL( plugin );
    serviceId = plugin.GetIntAttributeL( EWlanServiceId );
    DEBUG1("CHssIapHandler::ChangeSettingsL WLAN serviceId: %d", serviceId);
    
    if  ( aSettings.Name.Length() > 0 )
        {
        plugin.SetStringAttributeL( ECmName, aSettings.Name );
        plugin.UpdateL();
        }
    CleanupStack::PopAndDestroy( &plugin ); 
    CleanupStack::PopAndDestroy( &cmManager ); 

    // CommsDat section starts
    CMDBSession* dbSession = CMDBSession::NewL(CMDBSession::LatestVersion());
    CleanupStack::PushL( dbSession );
    iWLANRecord = static_cast<CMDBGenericRecord*>
    	( CCDRecordBase::RecordFactoryL( 0));
            
    iWLANRecord->InitializeL( KGenericTable(),NULL );
    iWLANRecord->SetRecordId( serviceId );
    iWLANRecord->LoadL( *dbSession );

    // Set Network Name
    if ( aSettings.iSSID.Length() > 0 )
        {
        CMDBField<TDesC>* textField = 
            (CMDBField<TDesC>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanSSID );            
        textField->SetMaxLengthL( aSettings.iSSID.Length() );
        textField->SetL( aSettings.iSSID );
        }
    
    // Set Connection Mode 
    *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanConnMode)) = 
                                                   aSettings.iConnectionMode;
    
    // Set Channel Id.
    *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanChannelID )) = 0;
    
    //  Set Security Mode
    *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanSecMode )) = 
                                                    aSettings.iSecurityMode;
    
    // Check if THssIapSettings is created wrong (dynamically). 
    // -> Then default value is not set to security mode and it will be huge.
    // -> Destination can become unusable.
    TInt secMode = aSettings.iSecurityMode;
    if ( secMode > EHssWpa2Only )
    	{
    	secMode = EHssAllowUnsecure;
    	}
    
    if( secMode == EHssWep )
        {
        DEBUG("CHssIapHandler::ChangeSettingsL -> SecurityMode == EHssWep");
        *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL(KCDTIdWlanAuthMode)) = aSettings.iAuthenticationMode;
        *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanWepIndex )) = aSettings.iWepIndex;
                
        ret = WriteWepKeysL( aSettings );
            
        *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanFormatKey1 )) = aSettings.iWepKeyFormat[0];
        *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanFormatKey2 )) = aSettings.iWepKeyFormat[1];
        *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanFormatKey3 )) = aSettings.iWepKeyFormat[2];
        *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanFormatKey4 )) = aSettings.iWepKeyFormat[3];
        }
    else if( secMode > EHssWep )
        {
        DEBUG("CHssIapHandler::ChangeSettingsL -> SecurityMode > EHssWep");
        if ( aSettings.iEnableWpaPsk ) // Use PreSharedKey
            {
            TBuf8<KHssWlanWpaPskLength> buf;
        
            TInt len = aSettings.iWPAPreSharedKey.Length();
        
            for( TInt i=0; i<len; i++)
                {
                buf.Append( aSettings.iWPAPreSharedKey[i]);
                }
        
            CMDBField<TDesC8>* binField = (CMDBField<TDesC8>*)iWLANRecord->GetFieldByIdL(KCDTIdWlanWpaPreSharedKey);
            binField->SetMaxLengthL( len);
            binField->SetL( buf);
       
            *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL(KCDTIdWlanWpaKeyLength)) = aSettings.iWPAKeyLength;
            *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL(KCDTIdWlanEnableWpaPsk)) = aSettings.iEnableWpaPsk;
            }
        else // Use EAP
            {
            DEBUG("CHssIapHandler::ChangeSettingsL -> SecurityMode > EHssWep -> EAP");
            *((CMDBField<TUint32>*)iWLANRecord->GetFieldByIdL(KCDTIdWlanEnableWpaPsk)) = aSettings.iEnableWpaPsk;
            }
        }
      
    // access point updated EAP data can be now set if needed
    if ( ( !aSettings.iEnableWpaPsk ) && secMode > EHssWep )
        {
        DEBUG("CHssIapHandler::ChangeSettingsL -> Set EAP data");
        TBuf8<KEapListMaxLength> enabledEapList;
        for (TInt i=0;i<aSettings.iEapDataArray.Count();i++)
            {
            TBuf8< KEapExpandedIdLength > eapTypeCue;
            TChar eapCode;
            GetEapTypeDesC(aSettings.iEapDataArray[i]->iEAPType, eapTypeCue, eapCode );
        
            CEapType* eapType = NULL;
            TRAPD( error, ( eapType = CEapType::NewL( eapTypeCue, ELan, serviceId ) ) );
            DEBUG1(" CEapType::NewL error: %d", error);
            if ( ( error == KErrNone ) && eapType )
                {
                CleanupStack::PushL( eapType );
                EAPSettings* eapSettings = new (ELeave) EAPSettings;
                
                ConvertEApSettings( *aSettings.iEapDataArray[i], *eapSettings );
                
                // Set EAP tunneling
                if ( aSettings.iEapDataArray[i]->iEncapsulatedInside != EAPSettings::EEapNone )
                	{
                	DEBUG("iEncapsulatedInside != EAPSettings::EEapNone");
                	eapType->SetTunnelingType( 
                			static_cast<TInt> ( aSettings.iEapDataArray[i]->iEncapsulatedInside ) );
                	}
                else // Write to EAP list
                	{
                	DEBUG("CHssIapHandler::ChangeSettingsL -> Add to EAP list");
                	TBuf8<8> expandedForm;
    	            expandedForm.AppendFill( 0xFE, 1 );
    	            if ( THssEapSettings::EEapPlainMschapv2 == aSettings.iEapDataArray[i]->iEAPType )
    	            	{
    	            	expandedForm.AppendFill( 0xFF, 3 );
    	            	}
    	            else{
    	            	expandedForm.AppendFill( 0x00, 6 );
    	            	}
    	       
    	            expandedForm.AppendFill( eapCode, 1 );
    	               	           
    	            enabledEapList.Append( expandedForm );      
    	        	}
                
                DEBUG("CHssIapHandler::ChangeSettingsL -> eapType->SetConfigurationL");
                TRAPD( eapErr, eapType->SetConfigurationL( *eapSettings ) );
                DEBUG1(" eapType->SetConfigurationL error: %d", eapErr);
                if( eapErr != KErrNone )
                	{
                	CleanupStack::PopAndDestroy( eapType );
                	CleanupStack::PopAndDestroy( dbSession );
                    DEBUG("CHssIapHandler::ChangeSettingsL EapError DONE");
                    return eapErr;
                	}
                CleanupStack::PopAndDestroy( eapType );
                }
            if ( error != KErrNone )
                {
                DEBUG("CHssIapHandler::ChangeSettingsL Eap error");
                CleanupStack::PopAndDestroy( dbSession );
                return error;
                }
            }
       
            // Set enabled EAP list
            if ( enabledEapList.Length() > 0 )
        		{
        		DEBUG("CHssIapHandler::ChangeSettingsL -> Set EAP list");
	            CMDBField<TDesC8>* eaps = (CMDBField<TDesC8>*)iWLANRecord->GetFieldByIdL( KCDTIdWlanEnabledEaps );
	            //eaps->SetMaxLengthL( eapList16.Length() )
	            eaps->SetL( enabledEapList );
            	}
        }
    // now update the access point
    iWLANRecord->ModifyL( *dbSession );
    
    CleanupStack::PopAndDestroy( dbSession );
    DEBUG("CHssIapHandler::ChangeSettingsL DONE");
	return ret;
    }
 
// -----------------------------------------------------------------------------
// WriteWepKeysL
// Writes WEP keys from aSettings to current WLANServiceTable record
// The record which the data is written to must be fetched from CommsDat
// before calling WriteWepKeysL()
// -----------------------------------------------------------------------------
//
TInt CHssIapHandler::WriteWepKeysL( const THssIapSettings& aSettings )
    {
    DEBUG("CHssIapHandler::WriteWepKeysL");
    // 1
    CMDBField<TDesC8>* wepKey = static_cast<CMDBField<TDesC8>*>(iWLANRecord->GetFieldByIdL(KCDTIdWlanWepKey1));
    if ( aSettings.iWepKey1.KeyLength <= KHssMaxWEPKeyLength )
        {
        PackWepKeyCommsDatL( aSettings.iWepKey1, aSettings.iWepKeyFormat[0], wepKey );
        }
    else 
        {
        return KErrTooBig;
        }
    
    // 2
    wepKey = static_cast<CMDBField<TDesC8>*>(iWLANRecord->GetFieldByIdL(KCDTIdWlanWepKey2));
    if ( aSettings.iWepKey2.KeyLength <= KHssMaxWEPKeyLength )
        {
        PackWepKeyCommsDatL( aSettings.iWepKey2, aSettings.iWepKeyFormat[1], wepKey );   
        }
    else 
        {
        return KErrTooBig;
        }
    
    // 3
    wepKey = static_cast<CMDBField<TDesC8>*>(iWLANRecord->GetFieldByIdL(KCDTIdWlanWepKey3));
    if ( aSettings.iWepKey3.KeyLength <= KHssMaxWEPKeyLength )
        {
        PackWepKeyCommsDatL( aSettings.iWepKey3, aSettings.iWepKeyFormat[2], wepKey );    
        }
    else 
        {
        return KErrTooBig;
        }
    
    // 4
    wepKey = static_cast<CMDBField<TDesC8>*>(iWLANRecord->GetFieldByIdL(KCDTIdWlanWepKey4));
    if ( aSettings.iWepKey4.KeyLength <= KHssMaxWEPKeyLength )
        {
         PackWepKeyCommsDatL( aSettings.iWepKey4,aSettings.iWepKeyFormat[3],  wepKey );   
        }
    else 
        {
        return KErrTooBig;
        }
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// PackWepKeyCommsDatL
// Copies wepkey string from wepkey structs TUint8 buffer to a WLANServiceTable
// record's WEP key field descriptor
// -----------------------------------------------------------------------------
//
void CHssIapHandler::PackWepKeyCommsDatL( const SHssWep& aInputKey,
                                          const EHssWEPKeyFormat&  aFormat,  
                                                  CMDBField<TDesC8>* aPackedKey )
    {
    DEBUG("CHssIapHandler:::PackWepKeyL");
    if( aInputKey.KeyLength > KHssMaxWEPKeyLength)
        {
        User::Leave( KErrArgument);
        }
    
    TBuf8<KHssMaxWEPKeyLength> wepBuf;
    
    for (TUint i = 0 ; i<aInputKey.KeyLength; i++)
        {
        wepBuf.Append(aInputKey.KeyMaterial[i]);
        }
        
    // Ascii to hexa convertion, if not already hexa
    if( EAscii == aFormat )
        {
        HBufC8* buf8Conv = HBufC8::NewLC( wepBuf.Length()*2 );
        ConvertAsciiToHex( wepBuf, buf8Conv );
        wepBuf.Copy( buf8Conv->Des() );
        CleanupStack::PopAndDestroy( buf8Conv );
        }
    
    aPackedKey->SetL( wepBuf);
    }

// -----------------------------------------------------------------------------
// ConvertEApSettings
// -----------------------------------------------------------------------------
//
void CHssIapHandler::ConvertEApSettings( THssEapSettings& aHssEap, EAPSettings& aEap )
    {
    DEBUG("CHssIapHandler::ConvertEApSettings");
    aEap.iEAPType = static_cast<EAPSettings::TEapType>(aHssEap.iEAPType);
    aEap.iUsernamePresent = aHssEap.iUsernamePresent;
    aEap.iUsername = aHssEap.iUsername;
    aEap.iPasswordPresent = aHssEap.iPasswordPresent;
    aEap.iPassword = aHssEap.iPassword;
    aEap.iRealmPresent = aHssEap.iRealmPresent;
    aEap.iRealm = aHssEap.iRealm;
    aEap.iUsePseudonymsPresent = aHssEap.iUsePseudonymsPresent;
    aEap.iUsePseudonyms = aHssEap.iUsePseudonyms;
    aEap.iVerifyServerRealmPresent = aHssEap.iVerifyServerRealmPresent;
    aEap.iVerifyServerRealm = aHssEap.iVerifyServerRealm;
    aEap.iRequireClientAuthenticationPresent = aHssEap.iRequireClientAuthenticationPresent;
    aEap.iRequireClientAuthentication = aHssEap.iRequireClientAuthentication;
    aEap.iSessionValidityTimePresent = aHssEap.iSessionValidityTimePresent;
    aEap.iSessionValidityTime = aHssEap.iSessionValidityTime;
    aEap.iCipherSuitesPresent = aHssEap.iCipherSuitesPresent;
    
    for( TInt i = 0; i < aHssEap.iCipherSuitesCount; i++ )
        {
        aEap.iCipherSuites.Append( aHssEap.iCipherSuites[i] );
        }
    aEap.iPEAPVersionsPresent = aHssEap.iPEAPVersionsPresent;
    aEap.iPEAPv0Allowed = aHssEap.iPEAPv0Allowed;
    aEap.iPEAPv1Allowed = aHssEap.iPEAPv1Allowed;
    aEap.iPEAPv2Allowed = aHssEap.iPEAPv2Allowed;
    aEap.iCertificatesPresent = aHssEap.iCertificatesPresent;
    for( TInt i = 0; i < aHssEap.iCertificatesCount; i++ )
        {
        DEBUG("CHssIapHandler::ConvertEApSettings Certificates present");
        CertificateEntry entry;
        
        entry.iCertType = static_cast<CertificateEntry::TCertType>( aHssEap.iCertificates[i]->iCertType );
        entry.iSubjectNamePresent = aHssEap.iCertificates[i]->iSubjectNamePresent;
        entry.iSubjectName = aHssEap.iCertificates[i]->iSubjectName;
        entry.iIssuerNamePresent = aHssEap.iCertificates[i]->iIssuerNamePresent;
        entry.iIssuerName = aHssEap.iCertificates[i]->iIssuerName;
        entry.iSerialNumberPresent= aHssEap.iCertificates[i]->iSerialNumberPresent;
        entry.iSerialNumber = aHssEap.iCertificates[i]->iSerialNumber;
        entry.iSubjectKeyIDPresent = aHssEap.iCertificates[i]->iSubjectKeyIDPresent;
        entry.iSubjectKeyID = aHssEap.iCertificates[i]->iSubjectKeyID;
        
        entry.iThumbprintPresent = aHssEap.iCertificates[i]->iThumbprintPresent;
        entry.iThumbprint = aHssEap.iCertificates[i]->iThumbprint;
        TRAP_IGNORE( aEap.iCertificates.AppendL( entry ) ); // Memory problem, ignored...
        }
    aEap.iEncapsulatedEAPTypesPresent = aHssEap.iEncapsulatedEAPTypesPresent;
    for( TInt i = 0; i < aHssEap.iEncapsulatedEAPTypesCount; i++ )
        {
        aEap.iEncapsulatedEAPTypes.Append( aHssEap.iEncapsulatedEAPTypes[i] );
        }
    }
    
// -----------------------------------------------------------------------------
// ConvertAsciiToHex
// -----------------------------------------------------------------------------
//
void CHssIapHandler::ConvertAsciiToHex( const TDes8& aSource, 
                                                HBufC8*& aDest )
    {
    DEBUG("CHssIapHandler::ConvertAsciiToHex");
    _LIT( hex, "0123456789ABCDEF" );
    TInt size = aSource.Size();
    TPtr8 ptr = aDest->Des();
    for ( TInt ii = 0; ii < size; ii++ )
        {
        TText8 ch = aSource[ii];
        ptr.Append( hex()[(ch/16)&0x0f] );
        ptr.Append( hex()[ch&0x0f] );
        }
    }

// -----------------------------------------------------------------------------
// GetEapTypeDesC
// -----------------------------------------------------------------------------
//
void CHssIapHandler::GetEapTypeDesC( TUint aEapType, TDes8& aEapString, TChar& aEapCode )
    {
    DEBUG1( "CHssIapHandler::GetEapTypeDesC aEapType: %d", aEapType );
    switch ( aEapType )
        {
            // EapNone
            case 0:
                aEapString.Copy( KEapNoneId, KEapExpandedIdLength );
                aEapCode = KEapNoneId[7];
                break;
            // KEapGtc
            case 6:
            	aEapString.Copy( KEapGtcId, KEapExpandedIdLength );
            	aEapCode = KEapGtcId[7];
                break;
            // KEapTls
            case 13:
            	aEapString.Copy( KEapTlsId, KEapExpandedIdLength );
            	aEapCode = KEapTlsId[7];
                break;
            // KEapLeap
            case 17:
            	aEapString.Copy( KEapLeapId, KEapExpandedIdLength );
            	aEapCode = KEapLeapId[7];
                break;
            // KEapSim
            case 18:
            	aEapString.Copy( KEapSimId, KEapExpandedIdLength );
            	aEapCode = KEapSimId[7];
                break;
            // KEapTtls
            case 21:
            	aEapString.Copy( KEapTtlsId, KEapExpandedIdLength );
            	aEapCode = KEapTtlsId[7];
                break;
            // KEapAka
            case 23:
            	aEapString.Copy( KEapAkaId, KEapExpandedIdLength );
            	aEapCode = KEapAkaId[7];
                break;
            // KEapPeap
            case 25:
            	aEapString.Copy( KEapPeapId, KEapExpandedIdLength );
            	aEapCode = KEapPeapId[7];
                break;
            // KEapMschapv2
            case 26:
            	aEapString.Copy( KEapMschapv2Id, KEapExpandedIdLength );
            	aEapCode = KEapMschapv2Id[7];
                break;
            // KEapSecurid
            case 32:
            	aEapString.Copy( KEapSecuridId, KEapExpandedIdLength );
            	aEapCode = KEapSecuridId[7];
                break;
            // KEapPlainMschapv2
            case 99:
            	aEapString.Copy( KEapPlainMschapv2Id, KEapExpandedIdLength );
            	aEapCode = KEapPlainMschapv2Id[7];
            	break;
            default:
            	aEapString.Copy( KEapNoneId, KEapExpandedIdLength );
            	aEapCode = KEapNoneId[7];
                break;
        }
    DEBUG( "CHssIapHandler::GetEapTypeDesC DONE" );
    }

// -----------------------------------------------------------------------------
// GetNetworkIdL
// -----------------------------------------------------------------------------
//
void CHssIapHandler::GetNetworkIdL( const TUint aIapId, TUint32& aNetId )
    {
    DEBUG( "CHssIapHandler::GetNetworkIdL()" );
    
    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );

    RCmConnectionMethodExt plugin = cmManager.ConnectionMethodL( aIapId );
    CleanupClosePushL( plugin );
    
    aNetId = plugin.GetIntAttributeL( ECmNetworkId );
    
    CleanupStack::PopAndDestroy( &plugin ); // Close() called on "plugin"
    CleanupStack::PopAndDestroy( &cmManager ); // Close() called on "cmManager"

    DEBUG1( "CHssIapHandler::GetNetworkIdL netId: %d", aNetId );
    }

// -----------------------------------------------------------------------------
// GetClientIapsL
// -----------------------------------------------------------------------------
//
void CHssIapHandler::GetClientIapsL( const TUid aUId, RArray<TUint>& aIapIdArray )
    {
    DEBUG("CHssIapSettingsHandler::GetClientsIapsL");
    TBuf<32> buffer;                    // Temporary buffer for found UID from destination.
    TUidName uidClient = aUId.Name();   // UID of the client.
    TUint32 iapId = 0;                  // IAP Identifiier.

    RArray<TUint32> destArray = RArray<TUint32>( 10 );  // KCmArrayGranularity instead of 10
    CleanupClosePushL( destArray );

    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );    

    cmManager.AllDestinationsL( destArray );

    for (TInt i = 0; i < destArray.Count(); i++)
        {
        RCmDestinationExt dest = cmManager.DestinationL( destArray[i] );
        CleanupClosePushL( dest );

        for (TInt j = 0; j < dest.ConnectionMethodCount(); j++)
            {
            TInt bearerType = dest.ConnectionMethodL(j).
                GetIntAttributeL( CMManager::ECmBearerType );

            if (bearerType == KUidWlanBearerType)
                {
                HBufC* uid = dest.ConnectionMethodL(j).
                        GetStringAttributeL( EWlanServiceExtensionTableName );
                CleanupStack::PushL( uid );

                // Copy found uid to temporary buffer
                buffer.Copy( *uid );    

                // If uids match, store corresponding IapId to aIapIdArray.    
                if (buffer.Compare( uidClient ) == 0)
                    {
                    iapId = dest.ConnectionMethodL(j).GetIntAttributeL( ECmIapId );
                    aIapIdArray.Append( iapId );
                    DEBUG("CHssIapSettingsHandler::GetClientsIapsL: UIDs matched");
                    DEBUG1("CHssIapSettingsHandler::Found IapId: %d", iapId);
                    }
                else
                    {
                    DEBUG("CHssIapSettingsHandler::GetClientsIapsL: NO match");
                    }
                CleanupStack::PopAndDestroy( uid );
                }
            }
        CleanupStack::PopAndDestroy( &dest );
        }

    CleanupStack::PopAndDestroy( &cmManager );
    CleanupStack::PopAndDestroy( &destArray );
    }

// End of File
