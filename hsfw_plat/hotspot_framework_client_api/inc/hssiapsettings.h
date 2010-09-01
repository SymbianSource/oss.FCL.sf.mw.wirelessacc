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
* Description:   IAP Settings for HotSpot FW 
*
*/




#ifndef HSSIAPSETTINGS_H
#define HSSIAPSETTINGS_H

//  INCLUDES
#include <e32base.h>
#include <s32strm.h>

//  CONSTANTS
const TInt KHssWlanWpaPskLength = 64;
const TInt KHssMaxMedTextLength	= 255;
const TUint8 KHssMaxSSIDLength = 32;
const TUint32 KHssMaxWEPKeyLength = 29;
const TUint KHssGeneralStringMaxLength = 255;
const TUint KHssKeyIdentifierLength = 20;
const TUint KHssThumbprintMaxLength = 64;
const TInt KHssMaxNumberOfKeys = 4;

//  DATA TYPES

enum EHssWepAuthenticationMode
    {
    EHssOpen = 0,
    EHssShared = 1    
    };

enum EHssConnectionMode
    {
    EHssAdhoc = 0,
    EHssInfrastructure = 1
    };

enum EHssWlanSecurityMode
    {
    EHssAllowUnsecure = 1,
    EHssWep = 2,
    EHssWlan8021x = 4,
    EHssWpa = 8,
    EHssWpa2Only = 16
    };

enum EHssWEPKeyFormat
    {
    EAscii,                 // Ascii format
    EHexadecimal            // Hex format
    };

/**
* Data structure for storing a WEP key.
*/
struct SHssWep
    {
    /** Specifies 64/128/152/256-bit WEP. Range: 0-3 */
    TUint32 KeyIndex;
    /** The length of KeyMaterial in bytes. Range: 0-29 */
    TUint32 KeyLength;
    /** Array that stores the WEP key. */
    TUint8 KeyMaterial[KHssMaxWEPKeyLength];
    };



	
/**
* class containing Certificate info for EAP settings
*/
class THssCertificateEntry 
{
public:

    enum THssCertType 
	{
		EUser,
		ECA
	};
	
	THssCertificateEntry();

	// Specifies whether this entry describes user or CA certificate (mandatory)
	THssCertType iCertType;
	
	// Subject name in distinguished name ASCII form.
	// For example "/C=US/O=Some organization/CN=Some common name".	
	TBool iSubjectNamePresent;
	TBuf<KHssGeneralStringMaxLength> iSubjectName;
		
	// Issuer name in distinguished name ASCII form.
	// For example "/C=US/O=Some organization/CN=Some common name".
	TBool iIssuerNamePresent;
	TBuf<KHssGeneralStringMaxLength> iIssuerName;
	
	// Serial number in ASCII form.
	TBool iSerialNumberPresent;
	TBuf<KHssGeneralStringMaxLength> iSerialNumber;
	
	// Subject key in binary form
	TBool iSubjectKeyIDPresent;
	TBuf8<KHssKeyIdentifierLength> iSubjectKeyID;
	
	// Thumbprint in binary form
	TBool iThumbprintPresent;
	TBuf<KHssThumbprintMaxLength> iThumbprint;
};


/**
* class containing EAP settings
*/
class THssEapSettings 
{
public:	

    enum THssEapType
	{
		EEapNone		= 0,
		EEapGtc			= 6,
		EEapTls			= 13,
		EEapLeap		= 17,
		EEapSim			= 18,
		EEapTtls		= 21,
		EEapAka			= 23,
		EEapPeap		= 25,
		EEapMschapv2	= 26,
		EEapSecurid		    = 32,
		EEapPlainMschapv2   = 99
	};
	
	THssEapSettings();
		
    void ExternalizeL( RWriteStream& aStream ) const;
    
    void InternalizeL( RReadStream& aStream );

	// Specifies the EAP type these settings are for. 
	// Is not really needed but is here so just some sanity checks can be made
	THssEapType iEAPType; 
	
	// Username in ASCII format
	TBool iUsernamePresent;
	TBuf<KHssGeneralStringMaxLength> iUsername; 
		
	// Password in ASCII format
	TBool iPasswordPresent;
	TBuf<KHssGeneralStringMaxLength> iPassword;
		
	// Realm in ASCII format
	TBool iRealmPresent;
	TBuf<KHssGeneralStringMaxLength> iRealm; 
	
	// Use pseudonym identities in EAP-SIM/AKA
	TBool iUsePseudonymsPresent;
	TBool iUsePseudonyms;		
	
	// Whether EAP-TLS/TTLS/PEAP should verify server realm
	TBool iVerifyServerRealmPresent;
	TBool iVerifyServerRealm;
	
	// Whether EAP-TLS/TTLS/PEAP should require client authentication
	TBool iRequireClientAuthenticationPresent;
	TBool iRequireClientAuthentication;
	
	// General session validity time (in minutes)
	TBool iSessionValidityTimePresent;
	TUint iSessionValidityTime;
	
	// An array of allowed cipher suites for EAP-TLS/TTLS/PEAP. 
	// Refer to RFC2246 chapter A.5 for the values.
	TBool iCipherSuitesPresent;
	TUint iCipherSuitesCount;
	RArray<TUint> iCipherSuites;

	// In EAP-PEAP is version 0 allowed
	TBool iPEAPVersionsPresent;
	TBool iPEAPv0Allowed;
	TBool iPEAPv1Allowed;
	TBool iPEAPv2Allowed;
  	  
  	// Array listing the allowed certificates for EAP-TLS/TTLS/PEAP
  	TBool iCertificatesPresent;
  	TUint iCertificatesCount;
	RPointerArray<THssCertificateEntry> iCertificates;
	
	// Array listing the encapsulated EAP types (in priority order)
	TBool iEncapsulatedEAPTypesPresent;
	TUint iEncapsulatedEAPTypesCount;
	RArray<TUint> iEncapsulatedEAPTypes;
	
	// EAP type that this THssEapSettings type is encapsulated to
	// Example: For EEapMschapv2 type this value could be EEapPeap
	THssEapType iEncapsulatedInside;
};
    
/**
* struct containing all wlan settings data
*/
class THssIapSettings 

    {
    public:
    THssIapSettings();
    TBuf<KHssMaxMedTextLength>      Name;
    
    EHssConnectionMode              iConnectionMode;        
    TBuf<KHssMaxSSIDLength>         iSSID;                

    EHssWlanSecurityMode            iSecurityMode;         
    
    SHssWep                         iWepKey1; 
    SHssWep                         iWepKey2;
    SHssWep                         iWepKey3;
    SHssWep                         iWepKey4;
    EHssWEPKeyFormat                iWepKeyFormat[KHssMaxNumberOfKeys];
    // Defines which WEP is in use. Range 0-3.
    TUint32                         iWepIndex;  

    EHssWepAuthenticationMode       iAuthenticationMode; 
    
    TBuf8<KHssWlanWpaPskLength>     iWPAPreSharedKey;
    TUint32                         iWPAKeyLength;
    TBool                           iEnableWpaPsk;
        
    RPointerArray<THssEapSettings>  iEapDataArray;
    };


#include "hssiapsettings.inl"

#endif // HSSIAPSETTINGS_H

// end of file

