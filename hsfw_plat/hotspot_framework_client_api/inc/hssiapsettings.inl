/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   EAP and WLAN authentication protocols.
*
*/



inline THssCertificateEntry::THssCertificateEntry()
: iSubjectNamePresent(EFalse)
, iIssuerNamePresent(EFalse)
, iSerialNumberPresent(EFalse)
, iSubjectKeyIDPresent(EFalse)
, iThumbprintPresent(EFalse)
    {
    }

inline THssEapSettings::THssEapSettings()
: iUsernamePresent(EFalse)
, iPasswordPresent(EFalse)
, iRealmPresent(EFalse)
, iVerifyServerRealmPresent(EFalse)
, iRequireClientAuthenticationPresent(EFalse)
, iSessionValidityTimePresent(EFalse)
, iCipherSuitesPresent(EFalse)
, iCipherSuites(1)
, iPEAPVersionsPresent(EFalse)
, iCertificatesPresent(EFalse)
, iCertificates(1)
, iEncapsulatedEAPTypesPresent(EFalse)
, iEncapsulatedEAPTypes(1)
, iEncapsulatedInside( EEapNone )
    {
    }

inline void THssEapSettings::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iEAPType );
    aStream.WriteUint32L( iUsernamePresent );
    aStream << iUsername;
    aStream.WriteUint32L( iPasswordPresent );
    aStream << iPassword;
    aStream.WriteUint32L( iRealmPresent );
    aStream << iRealm;
    aStream.WriteUint32L( iUsePseudonymsPresent );
    aStream.WriteUint32L( iUsePseudonyms );
    aStream.WriteUint32L( iVerifyServerRealmPresent );
    aStream.WriteUint32L( iVerifyServerRealm );
    aStream.WriteUint32L( iRequireClientAuthenticationPresent );
    aStream.WriteUint32L( iRequireClientAuthentication );
    aStream.WriteUint32L( iSessionValidityTimePresent );
    aStream.WriteUint32L( iSessionValidityTime );
    aStream.WriteUint32L( iCipherSuitesPresent );
    aStream.WriteUint32L( iCipherSuitesCount);
    for( TInt i = 0; i < iCipherSuitesCount; i++ )
        {
        aStream.WriteUint32L(iCipherSuites[i]); 
        }
    aStream.WriteUint32L( iPEAPVersionsPresent );
    aStream.WriteUint32L( iPEAPv0Allowed );
    aStream.WriteUint32L( iPEAPv1Allowed );
    aStream.WriteUint32L( iPEAPv2Allowed );
    aStream.WriteUint32L( iCertificatesPresent );
    aStream.WriteUint32L( iCertificatesCount);
    for( TInt i = 0; i < iCertificatesCount; i++ )
        {
        aStream.WriteInt32L( iCertificates[i]->iCertType );
        aStream.WriteUint32L( iCertificates[i]->iSubjectNamePresent ); 
        aStream << iCertificates[i]->iSubjectName;   
        aStream.WriteUint32L( iCertificates[i]->iIssuerNamePresent );
        aStream << iCertificates[i]->iIssuerName;   
        aStream.WriteUint32L( iCertificates[i]->iSerialNumberPresent );
        aStream << iCertificates[i]->iSerialNumber;   
        aStream.WriteUint32L( iCertificates[i]->iSubjectKeyIDPresent );
        aStream << iCertificates[i]->iSubjectKeyID;   
        aStream.WriteUint32L( iCertificates[i]->iThumbprintPresent );
        aStream << iCertificates[i]->iThumbprint;   
        }
    aStream.WriteUint32L( iEncapsulatedEAPTypesPresent );     
    aStream.WriteUint32L( iEncapsulatedEAPTypesCount );
    for( TInt i = 0; i < iEncapsulatedEAPTypesCount; i++ )
        {
        aStream.WriteUint32L( iEncapsulatedEAPTypes[i] ); 
        }
    }

inline void THssEapSettings::InternalizeL( RReadStream& aStream )
    {   
    iEAPType = static_cast<THssEapSettings::THssEapType>(aStream.ReadInt32L());
    iUsernamePresent = aStream.ReadUint32L();
    aStream >>iUsername;
    iPasswordPresent = aStream.ReadUint32L();
    aStream >>iPassword;
    iRealmPresent = aStream.ReadUint32L();
    aStream >>iRealm;
    iUsePseudonymsPresent = aStream.ReadUint32L();
    iUsePseudonyms = aStream.ReadUint32L();
    iVerifyServerRealmPresent = aStream.ReadUint32L();
    iVerifyServerRealm = aStream.ReadUint32L();
    iRequireClientAuthenticationPresent = aStream.ReadUint32L();
    iRequireClientAuthentication = aStream.ReadUint32L();
    iSessionValidityTimePresent = aStream.ReadUint32L();
    iSessionValidityTime = aStream.ReadUint32L();
    iCipherSuitesPresent = aStream.ReadUint32L();
    iCipherSuitesCount = aStream.ReadUint32L();
    for( TInt i = 0; i < iCipherSuitesCount; i++ )
        {
        iCipherSuites.Append(aStream.ReadUint32L());
        }
    iPEAPVersionsPresent = aStream.ReadUint32L();
    iPEAPv0Allowed = aStream.ReadUint32L();
    iPEAPv1Allowed = aStream.ReadUint32L();
    iPEAPv2Allowed = aStream.ReadUint32L();
    iCertificatesPresent = aStream.ReadUint32L();
    iCertificatesCount = aStream.ReadUint32L();
    for( TInt i = 0; i < iCertificatesCount; i++ )
        {
        THssCertificateEntry* entry = new (ELeave) THssCertificateEntry();
        
        entry->iCertType = static_cast<THssCertificateEntry::THssCertType>(aStream.ReadInt32L());
        entry->iSubjectNamePresent = aStream.ReadUint32L();
        aStream >> entry->iSubjectName;
        entry->iIssuerNamePresent = aStream.ReadUint32L();
        aStream >> entry->iIssuerName;
        entry->iSerialNumberPresent= aStream.ReadUint32L();
        aStream >> entry->iSerialNumber;
        entry->iSubjectKeyIDPresent = aStream.ReadUint32L();
        aStream >> entry->iSubjectKeyID;
        entry->iThumbprintPresent = aStream.ReadUint32L();
        aStream >> entry->iThumbprint;
        iCertificates.Insert(entry, i);
        }
    iEncapsulatedEAPTypesPresent = aStream.ReadUint32L();
    iEncapsulatedEAPTypesCount = aStream.ReadUint32L();
    for( TInt i = 0; i < iEncapsulatedEAPTypesCount; i++ )
        {
        iEncapsulatedEAPTypes.Append(aStream.ReadUint32L());
        }
    
}

inline THssIapSettings::THssIapSettings()
: iConnectionMode(EHssInfrastructure)
, iSecurityMode( EHssAllowUnsecure )
, iAuthenticationMode(EHssOpen)
, iEapDataArray(1)  
{
}

// end of file
