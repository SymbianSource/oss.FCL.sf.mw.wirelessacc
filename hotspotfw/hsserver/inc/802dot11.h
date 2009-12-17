/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Header file mainly for 802.11 specific declarations and 
*                definitions.
*
*/



#ifndef WLAN802DOT11_H
#define WLAN802DOT11_H

// for copy, equal and operator != support
#include "algorithm.h"
#include "pack.h"

// returns aDest
extern 
TAny* os_memcpy( TAny* aDest, const TAny* aSrc, TUint32 aLengthinBytes );
// returns 0 if equal
extern TInt 
os_memcmp( const TAny* aSrc1, const TAny* aSrc2, TUint aLengthinBytes );

/**
 * Returns a TUint16 host byte order value in WLAN MAC layer byte order 
 * (LSB first)
 *
 * @since S60 3.1
 * @param aNw value in network byte order
 * @return value in host byte order
 */
extern TUint16 os_Hton( TUint16 aHost );

/**
 * Returns a TUint16 WLAN MAC layer byte order (LSB first) value in host byte 
 * order
 *
 * @since S60 3.1
 * @param aNw value in network byte order
 * @return value in host byte order
 */
extern TUint16 os_Ntoh( TUint16 aNw );

/**
 * Allows also unaligned reading of a TUint16 WLAN MAC layer byte order 
 * (LSB first) value and returns the result in the byte order which
 * is in use in the host
 *
 * @since S60 3.1
 * @param aNwSource where to read the WLAN MAC layer byte order value from
 * @return the value which was read, in host byte order
 */
inline TUint16 ReadUint16Toh( const TUint16* aNwSource )
    {
    return ( os_Ntoh( 
        ( static_cast<const TUint16>( 
            ( reinterpret_cast<const TUint8*>(aNwSource) )[0] ) | 
         ( static_cast<const TUint16>(
            ( reinterpret_cast<const TUint8*>(aNwSource) )[1] ) << 8 ) ) ) );
    }

/**
 * Allows also unaligned writing of a TUint16 host byte order value
 * into WLAN MAC layer byte order
 *
 * @since S60 3.1
 * @param aNwTarget where to write the WLAN MAC layer byte order value to
 * @param aHostValue the host byte order value to be written
 */
inline void WriteHtoUint16( TUint16* aNwTarget, TUint16 aHostValue )
    {
    TUint16 nwValue = os_Hton( aHostValue );
    reinterpret_cast<TUint8*>(aNwTarget)[0] = 
        ( reinterpret_cast<TUint8*>(&nwValue) )[0];
    reinterpret_cast<TUint8*>(aNwTarget)[1] = 
        ( reinterpret_cast<TUint8*>(&nwValue) )[1];
    }

/**
 * Reverses the byte order of a TUint16 value
 *
 * @since S60 3.1
 * @param aOriginal value whose byte order is to be reversed
 * @return the input parameter in reversed byte order
 */
inline TUint16 ReverseUint16( TUint16 aOriginal )
    {
    return ( ( aOriginal >> 8 ) | ( aOriginal << 8 ) );
    }


/**
* A measurement of time equal to 1024 �s.
*/
const TUint16 KTU = 1024;

/**
* Length of the MAC address
*/
const TUint8 KMacAddressLength = 6;

/**
* Length of MAC header
*/
const TUint8 KMacHeaderLength = 24;

/**
* Maximum number of 802.11b supported rates 
* 1, 2, 5.5, 11 MBit/s
*/
const TUint8 KMaxNumberOfDot11bRates = 4; 

/**
* Maximum number of actual supported rate elements in 
* supported rates information element
*/
const TUint8 KMaxNumberOfRates = 8;

/**
* Maximum number of actual 802.11 supported rate elements in 
* extended supported rates information element
*/
const TUint8 KMaxNumberOfExtendedRates = 255;

/**
* Maximum number of 802.11b plus 802.11g supported rates 
*/
const TUint8 KMaxNumberOfDot11bAndgRates = 14; 

/**
* Bit mask for checking is supported rate element 
* part of BSS Basic Rate Set (MSB bit is set)
*/
const TUint8 KBasicRateMask = 0x80;

/**
* Maximum length for a WEP key in BYTES
*/
const TUint32 KMaxWEPKeyLength = 29;    // 232 bits

/**
* Length of TKIP key;
*/
const TUint8 KTKIPKeyLength = 16;

/**
* Length of WEP Init.Vector in BYTES
*/
const TUint8 KWepIVLength = 4;

/**
* Length of WEP ICV in BYTES
*/
const TUint8 KWEPICVLength = 4;

/**
* Length of Extended IV field in bytes.
*/
const TUint8 KWepExtendedIVLength = 4;

/**
* Mask to determine is Ext IV bit up in WEP IV
*/
const TUint32 KWepExtendedIvMask = 0x20000000;

/**
* Mask for Use Protection bit in ERP Information IE
*/
const TUint8 KUseProtectionMask = 0x02;

/**
* Length of MIC in BYTEs
*/
const TUint8 KMicLength = 8;

/** 
* Length of the MIC key in BYTEs
*/ 
const TUint8 KMicKeyLength = 8;

/**
* Maximum length of a SSID in BYTES
*/
const TUint8 KMaxSSIDLength = 32;

/**
* Maximum length of an outbound RSN IE length in BYTEs
*/
const TUint8 KMaxTxRsnIELength = 38;

/**
* Length of ID and Length fields of an information element.
*/
const TUint8 KInfoElementHeaderLength = 2;

/**
* Length of the challenge text in BYTES used in shared key authentication
*/
const TUint8 KChallengeTextLength = 128;

/**
* Ethernet header type field identifier for Bounce type packet
*/
const TUint16 KBounceType = 0x8013;

/**
* Ethernet header type field identifier for EAPOL type packet (IEEE 802.1X)
*/
const TUint16 KEapolType = 0x888E;

/**
* Ethernet header type field identifier for IP type packet
*/
const TUint16 KIpType = 0x0800;

/**
* Ethernet header type field identifier for IPv6 type packet
*/
const TUint16 KIpv6Type = 0x86DD;

/**
* Ethernet header type field identifier for ARP type packet
*/
const TUint16 KArpType = 0x0806;

/**
* Maks for determining does PLCP header service field have PBCC bit set
*/
const TUint8 KPlcpServicePbccMask = 0x08;

/**
* Length of OUI field in SNAP header
*/
const TUint8 KOIULength = 3;

/**
* Maximum length of 802.11 Frame Body field
*/
const TUint16 KMaxDot11FrameBodyLength = 2312;

/**
* Length of trailing FCS in bytes
*/
const TUint32 KFcsLength = 4;

/**
* Maximum length of 802.11 frame we will transmit or receive.
* This excludes the tailing FCS ( HW strips it )
* and Address4 field in the MPDU header 
* ( AP-to-AP mode frames are filtered out )
*/
const TUint16 KMaxDot11FrameLength = KMacHeaderLength 
    + KMaxDot11FrameBodyLength; // 24 + 2312 = 2336

/**
* Maximum length of 802.11 frame as stated in the standard
*/
const TUint16 KDot11MPDUMaxLength = 2346;

/**
* Maximum length of ethernet frame
*/
const TUint16 KMaxEthernetFrameLength = 1514;

/**
* Maximum value in ethernet length field
*/
const TUint16 KMaxEthernetLengthFieldvalue = 1500;

/**
* Length of the TKIP key in BYTEs
*/
const TUint8 KTkIpKeyLength = 16;

/**
* Length of IPv4 address field in BYTEs
*/
const TUint8 KIpv4AddrLength = 4;

/**
* Length of the priority filed used in WPA MIC calculation
*/
const TUint32 KWpaPriorityVecLen = 4;

/**
* WPA priority field used in MIC calculation
*/
const TUint8 KWpaPriorityVec[KWpaPriorityVecLen] = {0,0,0,0};

/**
* Mask for frame type for frame control field.
*/
const TUint32 K802Dot11FrameTypeMask = 0xFFFF;

/**
* Mask to determine if U-APSD bit is up in QoS info field of WMM IE
*/
const TUint8 KUapsdQosInfoMask = 0x80;

/**
* Mask for parameter set count in QoS info field of WMM IE
*/
const TUint8 KParamSetCountQosInfoMask = 0x0F;

/**
* Length of WMM Information Element
*/
const TUint8 KWmmInfoElemLen = 7;

/**
* Length of OUI field in Information Elements
*/
const TUint8 KIeOuiLength = 3;

typedef TUint8 TIeOui[KIeOuiLength];

/**
* OUI value of WMM Information Element and
* WMM Parameter Element
*/
const TIeOui KWmmElemOui = { 0x00, 0x50, 0xF2 };

/**
* OUI Type value of WMM Information Element and
* WMM Parameter Element
*/
const TUint8 KWmmElemOuiType = 2;

/**
* OUI Subtype value of WMM Information Element
*/
const TUint8 KWmmInfoElemOuiSubType = 0;

/**
* OUI Subtype value of WMM Parameter Element
*/
const TUint8 KWmmParamElemOuiSubtype = 1;

/**
* Version of WMM Information Element
*/
const TUint8 KWmmInfoElemVersion = 1;

/**
* Version of WMM Parameter Element
*/
const TUint8 KWmmParamElemVersion = 1;

/**
* The TXOP Limit in the AC Parameters Record of WMM Parameter Element
* needs to be multiplied by this value to get TXOP Limit in microseconds
*/
const TUint8 KWmmTxOpLimitMultiplier = 32;

/**
* AC flags in QoS info field of WMM IE
* When a flag is set the corresponding AC is both trigger and delivery enabled
*/
enum TQosInfoUapsdFlag
    {
    EAcVoUapsdFlag = 0x01, // Voice 
    EAcViUapsdFlag = 0x02, // Video 
    EAcBkUapsdFlag = 0x04, // Background 
    EAcBeUapsdFlag = 0x08  // Best Effort 
    };


/**
* Maximum U-APSD Service Period length. Indicates the max number of MSDUs and 
* MMPDUs the WMM AP may deliver to a WMM STA during any service period 
* triggered by the WMM STA.
*/
enum TQosInfoUapsdMaxSpLen
    {
    EMaxSpLenAllFrames  = 0x00,
    EMaxSpLenTwoFrames  = 0x20,
    EMaxSpLenFourFrames = 0x40,
    EMaxSpLenSixFrames  = 0x60
    };

const TUint8 K802Dot11AccessCategoryMask = 0x60;

/**
* WMM Access Categories
*/
enum TWmmAccessCategory
    {
    EAcBestEffort   = 0x00, 
    EAcBackground   = 0x20,
    EAcVideo        = 0x40,
    EAcVoice        = 0x60
    };

/**
* Number of WMM Access Categories
*/
const TUint8 KNumOfWmmACs = 4;

/**
* Mask for the Admission Control Mandatory flag
* in ACI/AIFSN field
* of AC parameter Record
* of WMM Parameter Element
*/
const TUint8 KWmmAdmissionCtrlMandatoryMask = 0x10;

/**
* Mask for the AIFSN subfiled
* in ACI/AIFSN field
* of AC parameter Record
* of WMM Parameter Element
*/
const TUint8 KWmmAifsnMask = 0x0F;

/**
* Mask for the ECWmin field
* of AC parameter Record
* of WMM Parameter Element
*/
const TUint8 KWmmEcwMinMask = 0x0F;

/**
* Mask for the ECWmax field
* of AC parameter Record
* of WMM Parameter Element
*/
const TUint8 KWmmEcwMaxMask = 0xF0;

/**
* 802.11 Authentication sequence numbers
*/
enum T802Dot11AuthenticationSeqNmbr
    {
    E802Dot11AuthenticationSeqNmbr1   = 1,
    E802Dot11AuthenticationSeqNmbr2   = 2,
    E802Dot11AuthenticationSeqNmbr3   = 3,
    E802Dot11AuthenticationSeqNmbr4   = 4
    };

/**
* 802.11 WEP key ID values
*/
enum T802Dot11WepKeyId
    {
    E802Dot11WepKeyId0 = 0,
    E802Dot11WepKeyId1 = 1,
    E802Dot11WepKeyId2 = 2,
    E802Dot11WepKeyId3 = 3,
    // defined as an upper bound
    E802Dot11WepKeyIdMax = 4
    };

/**
* 802.11 supported rate bytes. Used e.g. in Beacon frames.
*/
enum T802Dot11SupportedRate
    {
    E802Dot11Rate1MBit              = 2,
    E802Dot11Rate2MBit              = 4,
    E802Dot11Rate5p5MBit            = 11,
    E802Dot11Rate6MBit              = 12,
    E802Dot11Rate9MBit              = 18,
    E802Dot11Rate11MBit             = 22,
    E802Dot11Rate12MBit             = 24,
    E802Dot11Rate18MBit             = 36,
    E802Dot11Rate22MBit             = 44,
    E802Dot11Rate24MBit             = 48,
    E802Dot11Rate33MBit             = 66,
    E802Dot11Rate36MBit             = 72,
    E802Dot11Rate48MBit             = 96,
    E802Dot11Rate54MBit             = 108
    };

/**
* Management frame information element IDs.
*/
enum T802Dot11InformationElementID
    {
    E802Dot11SsidIE                 = 0,
    E802Dot11SupportedRatesIE       = 1,
    E802Doi11FhParameterSetIE       = 2,
    E802Dot11DsParameterSetIE       = 3,
    E802Dot11CfParameterSetIE       = 4,
    E802Dot11TimIE                  = 5,
    E802Dot11IbssParameterSetIE     = 6,
    E802Dot11CountryIE              = 7,
    E802Dot11HoppingPatternParamIE  = 8,
    E802Dot11HoppingPatternTableIE  = 9,
    E802Dot11RequestIE              = 10,

    E802Dot11ChallengeTextIE        = 16,
    // Reserved for challenge text extension 17 - 31
    E802Dot11ErpInformationIE       = 42,
    E802Dot11ExtendedRatesIE        = 50,
    E802Dot11RsnIE                  = 221,
    E802Dot11VendorSpecificIE       = 221    
    };

/**
* Bit masks for bit fields �n Frame Control field.
*/
enum T802Dot11FrameControlBitMask
    {
    E802Dot11FrameControlProtVersionMask= 0x0003,
    E802Dot11FrameControlTypeMask       = 0x000C,
    E802Dot11FrameControlSubtypeMask    = 0x00F0,
    E802Dot11FrameControlToDsMask       = 0x0100,
    E802Dot11FrameControlFromDsMask     = 0x0200,
    E802Dot11FrameControlMoreFragMask   = 0x0400,
    E802Dot11FrameControlRetryMask      = 0x0800,
    E802Dot11FrameControlPowerMgmtMask  = 0x1000,
    E802Dot11FrameControlMoreDataMask   = 0x2000,
    E802Dot11FrameControlWepMask        = 0x4000,
    E802Dot11FrameControlOrderMask      = 0x8000
    };

/**
* Frame Control field type masks.
*/
enum T802Dot11FrameControlBasicTypeMask
{
    E802Dot11FrameBasicTypeManagement       = 0x00,
    E802Dot11FrameBasicTypeControl          = 0x04,
    E802Dot11FrameBasicTypeData             = 0x08
};

/**
* Frame Control field subtype masks.
*/
enum T802Dot11FrameControlTypeMask
    {
    E802Dot11FrameTypeAssociationReq        = 0x00,
    E802Dot11FrameTypeAssociationResp       = 0x10,
    E802Dot11FrameTypeReassociationReq      = 0x20,
    E802Dot11FrameTypeReassociationResp     = 0x30,
    E802Dot11FrameTypeProbeReq              = 0x40,
    E802Dot11FrameTypeProbeResp             = 0x50,
    E802Dot11FrameTypeBeacon                = 0x80,
    E802Dot11FrameTypeAtim                  = 0x90,
    E802Dot11FrameTypeDisassociation        = 0xA0,
    E802Dot11FrameTypeAuthentication        = 0xB0,
    E802Dot11FrameTypeDeauthentication      = 0xC0,
    E802Dot11FrameTypePowerSavePoll         = 0xA4,
    E802Dot11FrameTypeReqToSend             = 0xB4,
    E802Dot11FrameTypeClearToSend           = 0xC4,
    E802Dot11FrameTypeAcknowledgement       = 0xD4,
    E802Dot11FrameTypeCfEnd                 = 0xE4,
    E802Dot11FrameTypeCfEndCfAck            = 0xF4,
    E802Dot11FrameTypeData                  = 0x08,
    E802Dot11FrameTypeDataCfAck             = 0x18,
    E802Dot11FrameTypeDataCfPoll            = 0x28,
    E802Dot11FrameTypeDataCfAckCfPoll       = 0x38,
    E802Dot11FrameTypeDataNull              = 0x48,
    E802Dot11FrameTypeCfAckNoData           = 0x58,
    E802Dot11FrameTypeCfPollNoData          = 0x68,
    E802Dot11FrameTypeCfAckCfPollNoData     = 0x78,
    E802Dot11FrameTypeQosData               = 0x88,
    E802Dot11FrameTypeQosDataCfAck          = 0x98,
    E802Dot11FrameTypeQosDataCfPoll         = 0xA8,
    E802Dot11FrameTypeQosDataCfAckCfPoll    = 0xB8,
    E802Dot11FrameTypeQosDataNull           = 0xC8,

    // not valid 802.11 types. 
    // just used in TxCompleted function to distinct separate cases
    E802Dot11FrameTypeAuthSeqNmbr1          = 0xFD,
    E802Dot11FrameTypeAuthSeqNmbr3          = 0xFE,
    E802Dot11FrameTypeDataEapol             = 0xFF,

    // not valid 802.11 types. 
    // just for test purposes
    E802Dot11FrameTypeTestFrame             = 0xFFFF
    };

/**
* Bit masks for Capability Information field.
*/
enum T802Dot11CapabilityBitMask
    {
    E802Dot11CapabilityEssMask          = 0x0001,
    E802Dot11CapabilityIbssMask         = 0x0002,
    E802Dot11CapabilityCfPollableMask   = 0x0004,
    E802Dot11CapabilityCfPollRequestMask= 0x0008,
    E802Dot11CapabilityPrivacyMask      = 0x0010,
    // these little critters are from 802.11b spec
    E802Dot11ShortPreambleMask          = 0x0020,
    E802Dot11PbccMask                   = 0x0040,      
    E802Dot11ChannelAgilityMask         = 0x0080,
    E802Dot11ShortSlotTimeMask          = 0x0400
    };

/**
* Supported authentication modes
*/
const TUint16 K802Dot11AuthModeOpen   = 0;
const TUint16 K802Dot11AuthModeShared = 1;

/**
* 802.11 status codes
*/
enum T802Dot11ManagementStatusCode
    {
    E802Dot11StatusSuccess                          = 0,
    E802Dot11StatusUnspecifiedFailure               = 1,
    // 2 -9 reserved    
    E802Dot11StatusUnsupportedCapabilities          = 10,
    E802Dot11StatusReAssociationDenied              = 11,
    E802Dot11StatusAssocDenied                      = 12,
    E802Dot11StatusAuthAlgorithmNotSupported        = 13,
    E802Dot11StatusAuthFrameOutOfSequence           = 14,
    E802Dot11StatusChallengeFailure                 = 15,
    E802Dot11StatusAuthRejectedTimeout              = 16,
    E802Dot11StatusAssocDeniedApFull                = 17,
    E802Dot11StatusAssocDeniedBasicRatesUnSupp      = 18,
    // following three little critters are from 802.11b spec
    E802Dot11StatusAssocDeniedShortPreambleUnSupp   = 19,
    E802Dot11StatusAssocDeniedPbccUnSupp            = 20,
    E802Dot11StatusAssocDeniedChannelAgilityUnSupp  = 21    
    // 22 - 65,535 reserved
    };

/**
* 802.11 reason codes
*/
enum T802Dot11ManagementReasonCode
    {
    // 0 reserved
    E802Dot11ReasonUnspecified                = 1,
    E802Dot11ReasonAuthNoLongerValid          = 2,
    E802Dot11ReasonDeauthStationLeft          = 3,
    E802Dot11ReasonDisAssocInactivity         = 4,
    E802Dot11ReasonDisAssocAPFull             = 5,
    E802Dot11ReasonClass2FrameWhenNotAuth     = 6,
    E802Dot11ReasonClass3FrameWhenNotAssoc    = 7,
    E802Dot11ReasonDisAssocStationLeft        = 8,
    E802Dot11ReasonAssocFailNotAuth           = 9,
    // 10 - 12 reserved    
    // WPA stuff
    E802Dot11ReasonInvalidIe                  = 13,
    E802Dot11ReasonMicFailure                 = 14,
    E802Dot11ReasonHandshakeTimeout           = 15,
    E802Dot11ReasonGroupKeyUpdateTimeout      = 16,
    E802Dot11ReasonIeMismatch                 = 17,
    E802Dot11ReasonMulticastChipherNotValid   = 18,
    E802Dot11ReasonUnicastChipherNotValid     = 19,
    E802Dot11ReasonAkmpNotValid               = 20,
    E802Dot11ReasonUnsupportedRsneVersion     = 21,
    E802Dot11ReasonInvalidRsneCapabilities    = 22,
    E802Dot11Reason1xAuthenticationFailed     = 23,
    // 24 - 65,535 reserved    
    };

/**
* 802.11 management frame fixed field and IE lengths in BYTEs
*/
const TUint32 KTimeStampFixedFieldLength                = 8;
const TUint32 KBeaconIntervalFixedFieldLength           = 2;
const TUint32 KCapabilityInformationFixedFieldLength    = 2;
const TUint32 KDsParameterSetIeLength                   = 3;
const TUint32 KIbssParameterSetIeLength                 = 4;

/**
* The one and only MAC address struct
*/
#pragma pack( 1 )
struct TMacAddress
    {
    /** the MAC address */
    TUint8 iMacAddress[KMacAddressLength];
    } __PACKED; // 6 bytes

/**
* Broadcast MAC Address.
*/
const TMacAddress KBroadcastMacAddr = {{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }};

/**
* MAC address that all zeros
*/
const TMacAddress KZeroMacAddr = {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

/**
* operator== for TMacAddress 
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== ( 
    const TMacAddress& aLhs, const TMacAddress& aRhs )
    {
    return ( equal( (aLhs.iMacAddress),
                    (aLhs.iMacAddress) + KMacAddressLength, 
                    aRhs.iMacAddress) );
    }

/**
* Sets or clears the group bit of the MAC address 
* @param aMac the address
* @param aSet set or clear the bit
*/
inline void GroupBit( TMacAddress& aMac, TBool aSet = ETrue )
    {
    if ( aSet )
        {
        aMac.iMacAddress[0] |= KBit0;
        }
    else
        {
        aMac.iMacAddress[0] &= ~KBit0;
        }
    }

/**
* Evaluates if the group bit of the MAC address is set
* @param aMac the address
* @return ETrue if group bit is set EFalse in other case
*/
inline TBool IsGroupBitSet( const TMacAddress& aMac )
    {
    return (aMac.iMacAddress[0] & KBit0);
    }

/**
* Sets or clears the local bit of the MAC address 
* @param aMac the address
* @param aSet set or clear the bit
*/
inline void LocalBit( TMacAddress& aMac, TBool aSet = ETrue )
    {
    if ( aSet )
        {
        aMac.iMacAddress[0] |= KBit1;
        }
    else
        {
        aMac.iMacAddress[0] &= ~KBit1;
        }
    }

/**
* Evaluates if the local bit of the MAC address is set
* @param aMac the address
* @return ETrue if group bit is set EFalse in other case
*/
inline TBool IsLocalBitSet( const TMacAddress& aMac )
    {
    return (aMac.iMacAddress[0] & KBit1);
    }

/**
* 802.11 information element header
*/
#pragma pack( 1 )
struct SInformationElementHeader
    {
    /** the element ID */
    const TUint8    iElementID;
    /** length of the following IE */
    TUint8          iLength;      

    /**
    * Ctor
    * @param aElementID element ID used
    */
    explicit SInformationElementHeader( 
        T802Dot11InformationElementID aElementID ) 
        : iElementID( static_cast<TUint8>(aElementID) ), iLength( 0 ) {};
    /**
    * Ctor
    * @param aElementID element ID used
    * @param aLength length of the information element following this header
    */
    SInformationElementHeader( T802Dot11InformationElementID aElementID, 
        const TUint8 aLength ) 
        : iElementID( static_cast<TUint8>(aElementID) ), iLength( aLength ) {};

private:

    /** Prohibit assignment operator */
    SInformationElementHeader& operator= ( const SInformationElementHeader& );
    /** Prohibit copy constructor */
    SInformationElementHeader( const SInformationElementHeader& );
    } __PACKED; // 2 bytes

/**
* operator== for SInformationElementHeader
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== ( 
    const SInformationElementHeader& aLhs, 
    const SInformationElementHeader& aRhs )
    {
    return static_cast<TBool>(( aLhs.iElementID == aRhs.iElementID
        && aLhs.iLength == aRhs.iLength ));
    }

/**
* 802.11 SSID information element
*/
#pragma pack( 1 )
struct  SSsIdIE
    {
    /** information element header */
    SInformationElementHeader   iHeader;                    // 2 bytes
    /** SSID information element */
    TUint8                      iSsIdIe[KMaxSSIDLength];    // 32 bytes 

    /**
    * Ctor
    */
    inline SSsIdIE() : iHeader( E802Dot11SsidIE ) {};

    /**
    * Ctor
    * @param aData pointer to SSID
    * @param aLength length of aData
    */
    inline SSsIdIE( const TUint8* aData, TUint8 aLength );

    /**
    * Returns information elements length ( the element + its header )
    * @return see above
    */
    TUint8 GetIeLength() const { return static_cast<TUint8>( 
        (iHeader.iLength + sizeof( SInformationElementHeader ) ) ); }        

private:

    /** Prohibit assignment operator */
    SSsIdIE& operator= ( const SSsIdIE& );
    /** Prohibit copy constructor */
    SSsIdIE( const SSsIdIE& );
    } __PACKED; // 34 bytes

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline SSsIdIE::SSsIdIE( const TUint8* aData, TUint8 aLength ) 
    : iHeader( E802Dot11SsidIE )
    {
    iHeader.iLength =  aLength;
    os_memcpy( &(iSsIdIe[0]), aData, aLength );   
    }       

/**
* operator== for SSsIdIE
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal EFalse not equal
*/
inline TBool operator== (
    const SSsIdIE& aLhs,
    const SSsIdIE& aRhs)
    {
    return static_cast<TBool>(( ( aLhs.iHeader == aRhs.iHeader )
        && !(os_memcmp( aLhs.iSsIdIe, aRhs.iSsIdIe, aLhs.iHeader.iLength )) ));
    }

/**
* 802.11 supported rates information element
*/
#pragma pack( 1 )
struct SSupportedRatesIE
    {
    /** information element header */
    SInformationElementHeader iHeader;                      // 2
    /** 
    * supported rates information element
    * NOTE! The 802.11 standard specifies that the max length of the 
    * information part of this IE is eight rates (eight bytes).
    * However at least some APs seem to put all their supported rates
    * into this element. In order to be able to associate with those
    * APs we allocate enough space to incorporate all 802.11b/g rates
    * in this IE. We ourselves will still always advertise max eight 
    * rates using this IE (and the rest using the Extended Supported
    * Rates IE).
    */
    TUint8 iSupportedRatesIE[KMaxNumberOfDot11bAndgRates];  // 14

    /**
    * Ctor
    */
    SSupportedRatesIE() 
        : iHeader( E802Dot11SupportedRatesIE, KMaxNumberOfRates ) {};

    /**
    * operator[] to get supported rates element at given index
    * @param aIdx index to be get
    * @return value in given index
    */
    inline TUint8 operator[] ( TUint8 aIdx ) const;

    /**
    * sets IE data also sets the IE headers length field
    * @param aIeData actual IE data
    * @param aLength length of aIeData
    */
    inline void SetIeData( const TUint8* aIeData, const TUint8 aLength);

    /**
    * Returns information element's actual length 
    * ( element's size + its header )
    * @return see above
    */
    inline TUint8 GetIeLength() const; 

    /**
    * Returns only the information element's actual length ( header excluded )
    * @return see above
    */
    inline TUint8 GetElementLength() const; 

    /** Marks IE zero length size */
    inline void Clear();

    /**
    * Appends a single rate element to IE
    * @param aRate rate element to be appended
    */
    inline void Append( TUint8 aRate );

private:

    /** Prohibit assignment operator */
    SSupportedRatesIE& operator= ( const SSupportedRatesIE& );
    /** Prohibit copy constructor */
    SSupportedRatesIE( const SSupportedRatesIE& );
    } __PACKED; // 16 bytes

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SSupportedRatesIE::SetIeData( 
    const TUint8* aIeData, 
    const TUint8 aLength)
    {
    iHeader.iLength = aLength;
    os_memcpy( iSupportedRatesIE, aIeData, aLength );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SSupportedRatesIE::operator[] ( TUint8 aIdx ) const
    {
    return iSupportedRatesIE[aIdx];
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SSupportedRatesIE::GetIeLength() const 
    { 
    return static_cast<TUint8>( 
        ( iHeader.iLength + sizeof( SInformationElementHeader ) ) ); 
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SSupportedRatesIE::GetElementLength() const
    {
    return iHeader.iLength;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SSupportedRatesIE::Clear()
    {
    iHeader.iLength = 0;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SSupportedRatesIE::Append( TUint8 aRate )
    {
    iSupportedRatesIE[iHeader.iLength] = aRate;
    ++(iHeader.iLength);
    }

/**
* operator== for SSupportedRatesIE
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SSupportedRatesIE& aLhs,
    const SSupportedRatesIE& aRhs )
    {
    return static_cast<TBool>(( ( aLhs.iHeader == aRhs.iHeader )
        && !(os_memcmp( aLhs.iSupportedRatesIE, 
        aRhs.iSupportedRatesIE, aLhs.iHeader.iLength )) ));
    }

/**
* 802.11 extended supported rates information element
*/
#pragma pack( 1 )
struct SExtendedSupportedRatesIE
    {
	enum { KPad = 1 };
    /** information element header */
    SInformationElementHeader   iHeader;                                        // 2
    /** supported rates information element */
    TUint8                      iSupportedRatesIE[KMaxNumberOfExtendedRates];   // 255
	/** padding */
	TUint8						iPad[KPad];				                        // 1

    /**
    * Ctor
    */
    SExtendedSupportedRatesIE() 
        : iHeader( E802Dot11ExtendedRatesIE, KMaxNumberOfExtendedRates ) {};

    /**
    * operator[] to get supported rates element at given index
    * @param aIdx index to be get
    * @return value in given index
    */
    inline TUint8 operator[] ( TUint8 aIdx ) const;

    /**
    * sets IE data also sets the IE headers length field
    * @param aIeData actual IE data
    * @param aLength length of aIeData
    */
    inline void SetIeData( const TUint8* aIeData, const TUint8 aLength);

    /**
    * Returns information element's actual length 
    * ( element's size + its header )
    * @return see above
    */
    inline TUint8 GetIeLength() const; 

    /**
    * Returns only the information element's actual length ( header excluded )
    * @return see above
    */
    inline TUint8 GetElementLength() const; 

    /** Marks IE zero length size */
    inline void Clear();

    /**
    * Appends a single rate element to IE
    * @param aRate rate element to be appended
    */
    inline void Append( TUint8 aRate );

private:

    /** Prohibit assignment operator */
    SExtendedSupportedRatesIE& operator= ( const SExtendedSupportedRatesIE& );
    /** Prohibit copy constructor */
    SExtendedSupportedRatesIE( const SExtendedSupportedRatesIE& );
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SExtendedSupportedRatesIE::SetIeData( 
    const TUint8* aIeData, 
    const TUint8 aLength)
    {
    iHeader.iLength = aLength;
    os_memcpy( iSupportedRatesIE, aIeData, aLength );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SExtendedSupportedRatesIE::operator[] ( TUint8 aIdx ) const
    {
    return iSupportedRatesIE[aIdx];
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SExtendedSupportedRatesIE::GetIeLength() const 
    { 
    return static_cast<TUint8>( 
        ( iHeader.iLength + sizeof( SInformationElementHeader ) ) ); 
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SExtendedSupportedRatesIE::GetElementLength() const
    {
    return iHeader.iLength;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SExtendedSupportedRatesIE::Clear()
    {
    iHeader.iLength = 0;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SExtendedSupportedRatesIE::Append( TUint8 aRate )
    {
    iSupportedRatesIE[iHeader.iLength] = aRate;
    ++(iHeader.iLength);
    }

/**
* operator== for SExtendedSupportedRatesIE
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SExtendedSupportedRatesIE& aLhs,
    const SExtendedSupportedRatesIE& aRhs )
    {
    return static_cast<TBool>(( ( aLhs.iHeader == aRhs.iHeader )
        && !(os_memcmp( aLhs.iSupportedRatesIE, 
        aRhs.iSupportedRatesIE, aLhs.iHeader.iLength )) ));
    }

#pragma pack( 1 )
struct SDsParameterSetIE
    {
    SInformationElementHeader   iHeader;                // 2
    TUint8                      iValue;                 // 1                         

    inline SDsParameterSetIE( const TUint32 aValue );

private:

    /** Prohibit assignment operator */
    SDsParameterSetIE& operator= ( const SDsParameterSetIE& aObj );
    /** Prohibit copy constructor */
    SDsParameterSetIE( const SDsParameterSetIE& );
    } __PACKED;     // 3 bytes 

inline SDsParameterSetIE::SDsParameterSetIE( 
    const TUint32 aValue )         
    : iHeader( E802Dot11DsParameterSetIE, sizeof(iValue) )
    {
    iValue = aValue;
    }

#pragma pack( 1 )
struct SIbssParameterSetIE
    {
    SInformationElementHeader   iHeader;                // 2
    TUint16                     iValue;                 // 2                         

    inline SIbssParameterSetIE( const TUint16 aValue );
    
    inline TUint16 AtimWindow() const;

private:

    /** Prohibit assignment operator */
    SIbssParameterSetIE& operator= ( const SIbssParameterSetIE& aObj );
    /** Prohibit copy constructor */
    SIbssParameterSetIE( const SIbssParameterSetIE& );
    } __PACKED;     // 4 bytes 

inline SIbssParameterSetIE::SIbssParameterSetIE( 
    const TUint16 aValue )         
    : iHeader( E802Dot11IbssParameterSetIE, sizeof(iValue) )
    {
    WriteHtoUint16( &iValue, aValue );
    }

inline TUint16 SIbssParameterSetIE::AtimWindow() const
    {
    return ( ReadUint16Toh( &iValue ) );
    }

/**
* 802.11 WiFi WPA outbound information element
*/
#pragma pack( 1 )
struct STxRsnIE
    {
    /** information element header */
    SInformationElementHeader   iHeader;                       // 2
    /** RSN information element */
    TUint8                      iTxRsnIe[KMaxTxRsnIELength];   // 38

    /** Ctor */
    STxRsnIE() 
        : iHeader( E802Dot11VendorSpecificIE , KMaxTxRsnIELength ) {};

    /**
    * Returns information element's actual length 
    * ( element's size + its header )
    * @return see above
    */
    inline TUint8 GetIeLength() const; 

    /**
    * Sets iTxRsnIe field and IE length field
    * @param aData data to be set
    * @param aLength length of the data
    */
    inline void SetRsn( const TUint8* aData, TUint32 aLength );

private:

    /** Prohibit assignment operator */
    STxRsnIE& operator= ( const STxRsnIE& aObj );
    /** Prohibit copy constructor */
    STxRsnIE( const STxRsnIE& );
    } __PACKED; // 40 bytes

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 STxRsnIE::GetIeLength() const
    {
    return static_cast<TUint8>( 
        ( iHeader.iLength + sizeof( SInformationElementHeader ) ) ); 
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void STxRsnIE::SetRsn( const TUint8* aData, TUint32 aLength )
    {
    iHeader.iLength = static_cast<TUint8>(aLength);
    os_memcpy( iTxRsnIe, aData, aLength );
    }

/**
* Inbound WMM information element without IE header
*/
#pragma pack( 1 )
struct SRxWmmIeData
    {
    TIeOui                      iOui;           // 3
    TUint8                      iOuiType;       // 1
    TUint8                      iOuiSubType;    // 1
    TUint8                      iVersion;       // 1
    /** information element */
    TUint8                      iQosInfo;       // 1

    /**
    * Evaluates if U-APSD bit is up
    * @return ETrue if bit is up, otherwise EFalse
    */
    inline TBool IsUapsdBitSet() const;

    /**
    * Gets the the parameter set count
    * @return parameter set count
    */
    inline TUint8 ParameterSetCount() const;

private:

    /** Prohibit assignment operator */
    SRxWmmIeData& operator= ( const SRxWmmIeData& aObj );
    /** Prohibit copy constructor */
    SRxWmmIeData( const SRxWmmIeData& );
    } __PACKED;     // 7 bytes 

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SRxWmmIeData::IsUapsdBitSet() const
    {
    return ( (iQosInfo & KUapsdQosInfoMask )? ETrue : EFalse );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SRxWmmIeData::ParameterSetCount() const
    {
    return ( iQosInfo & KParamSetCountQosInfoMask );
    }


/**
* Outbound WMM information element
*/
#pragma pack( 1 )
struct STxWmmIE
    {
    /** information element header */
    SInformationElementHeader   iHeader;        // 2
    TIeOui                      iOui;           // 3
    TUint8                      iOuiType;       // 1
    TUint8                      iOuiSubType;    // 1
    TUint8                      iVersion;       // 1
    /** information element */
    TUint8                      iQosInfo;       // 1


    /**
    * Ctor
    */
    inline STxWmmIE() : iHeader( E802Dot11VendorSpecificIE, KWmmInfoElemLen ),
        iOuiType( KWmmElemOuiType ), 
        iOuiSubType( KWmmInfoElemOuiSubType ), iVersion( KWmmInfoElemVersion ),
        iQosInfo( 0 ) 
        { 
        os_memcpy( iOui, KWmmElemOui, KIeOuiLength );
        }

    /**
    * Sets the U-APSD flags for different ACs
    * @param aFlags flag(s) to be set
    */
    inline void SetUapsdFlags( TQosInfoUapsdFlag aFlags );

    /**
    * Sets the maximum service period length
    * @param aMaxSpLen length of the service period 
    */
    inline void SetMaxSpLen( TQosInfoUapsdMaxSpLen aMaxSpLen );
    
    /**
    * Returns information elements total length, i.e.
    * element's length + header length
    * @return see above
    */
    inline TUint8 GetIeLength() const; 

    /** Clears the IE content, i.e. the iQosInfo field */
    inline void Clear();    

private:

    /** Prohibit assignment operator */
    STxWmmIE& operator= ( const STxWmmIE& aObj );
    /** Prohibit copy constructor */
    STxWmmIE( const STxWmmIE& );
    } __PACKED;     // 9 bytes 


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void STxWmmIE::SetUapsdFlags( TQosInfoUapsdFlag aFlags )
    {
    iQosInfo |= aFlags;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void STxWmmIE::SetMaxSpLen( TQosInfoUapsdMaxSpLen aMaxSpLen )
    {
    iQosInfo |= aMaxSpLen;    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 STxWmmIE::GetIeLength() const
    {
    return static_cast<TUint8>( 
        ( iHeader.iLength + sizeof( SInformationElementHeader ) ) ); 
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void STxWmmIE::Clear()
    {
    iQosInfo = 0;    
    }

/**
* AC parameters record of WMM parameter element
*/
#pragma pack( 1 )
struct SAcParamsRecord
    {
    
    TUint8                      iAciAifsn;      // 1
    TUint8                      iEcwMinMax;     // 1
    TUint16                     iTxOpLimit;     // 2
    
    inline TWmmAccessCategory AccessCategory() const;

    inline TBool AdmissionControlMandatory() const;
    
    inline TUint8 Aifsn() const;

    inline TUint16 CwMin() const;
    
    inline TUint16 CwMax() const;

    inline TUint16 TxOpLimit() const;        
    
private:

    /** Prohibit assignment operator */
    SAcParamsRecord& operator= ( const SAcParamsRecord& aObj );
    /** Prohibit copy constructor */
    SAcParamsRecord( const SAcParamsRecord& );
    } __PACKED;     // 4 bytes 

inline TWmmAccessCategory SAcParamsRecord::AccessCategory() const
    {
    return ( static_cast<TWmmAccessCategory>(iAciAifsn & K802Dot11AccessCategoryMask) );
    }

inline TBool SAcParamsRecord::AdmissionControlMandatory() const
    {
    return ( iAciAifsn & KWmmAdmissionCtrlMandatoryMask );
    }

inline TUint8 SAcParamsRecord::Aifsn() const
    {
    return ( iAciAifsn & KWmmAifsnMask );
    }

inline TUint16 SAcParamsRecord::CwMin() const
    {
    return ( ( static_cast<TUint16>( 1 ) << 
             ( iEcwMinMax & KWmmEcwMinMask ) ) - 1 );
    }

inline TUint16 SAcParamsRecord::CwMax() const
    {
    return ( ( static_cast<TUint16>( 1 ) << 
             ( ( iEcwMinMax & KWmmEcwMaxMask ) >> 4 ) ) - 1 );
    }

inline TUint16 SAcParamsRecord::TxOpLimit() const
    {
    return ( ReadUint16Toh( &iTxOpLimit ) );
    }

/**
* WMM Parameter Element without element header
*/
#pragma pack( 1 )
struct SWmmParamElemData
    {
    /** information element header */
    TIeOui                      iOui;                    // 3
    TUint8                      iOuiType;                // 1
    TUint8                      iOuiSubType;             // 1
    TUint8                      iVersion;                // 1
    /** information element fields: */
    TUint8                      iQosInfo;                // 1
    TUint8                      iReserved;               // 1
    SAcParamsRecord             iAcParams[KNumOfWmmACs]; // 16

    /**
    * Evaluates if U-APSD bit is up
    * @return ETrue if bit is up, otherwise EFalse
    */
    inline TBool IsUapsdBitSet() const;

    /**
    * Gets the the parameter set count
    * @return parameter set count
    */
    inline TUint8 ParameterSetCount() const;

private:

    /** Prohibit assignment operator */
    SWmmParamElemData& operator= ( const SWmmParamElemData& aObj );
    /** Prohibit copy constructor */
    SWmmParamElemData( const SWmmParamElemData& );
    } __PACKED;     // 24

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SWmmParamElemData::IsUapsdBitSet() const
    {
    return ( (iQosInfo & KUapsdQosInfoMask )? ETrue : EFalse );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SWmmParamElemData::ParameterSetCount() const
    {
    return ( iQosInfo & KParamSetCountQosInfoMask );
    }

// capability information fixed-field 
//
// bit 14 - 15     13     11 - 12    10        8-9       7         6                         
//   ----------+-------+-----------+-------+---------+----------+-------
//    reserved | DSSS- | reserved  | short | reserved|  Channel |   PBCC   
//             | OFDM  |           | slot  |         |  Agility |          
//   ----------+-------+-----------+-------+---------+----------+--------
// bit        5           4         3          2          1          0  
//        ----------+----------+----------+----------+----------+----------+
//           Short  |  Privacy |  CF-Poll |   CF     |   IBSS   |    ESS   |
//          Preamble|   (WEP)  |  Request | Pollable |          |          |        
//        ----------+----------+----------+----------+----------+----------+

/**
* 802.11 management frame body capability information fixed-field
*/
#pragma pack( 1 )
struct SCapabilityInformationField
    {
    enum { KReservedFieldsMask = 0xEB00 };

    /** capability information fixed field */
    TUint16 iCapabilityInformationField;

    /**
    * Ctor
    */
    SCapabilityInformationField() 
        : iCapabilityInformationField( 0 ) {};

    /**
    * Ctor
    * @param aParam value used in iCapabilityInformationField field
    */
    explicit inline SCapabilityInformationField( const TUint16 aParam ); 

    /**
    * Assignment operator for TUint16 type
    * @param aParam value used as iCapabilityInformationField
    */
    inline SCapabilityInformationField& operator= ( const TUint16 aParam );
        
    /**
    * Returns the value of the Capability Information Field
    * @return 
    */
    inline TUint16 CapabilityInformationField() const;
    /**
    * Evaluates if ESS bit is up
    * @return ETrue if bit is up, otherwise EFalse
    */
    inline TBool IsEssBitSet() const;
    /**
    * Evaluates if IBSS bit is up
    * @return ETrue if bit is up, otherwise EFalse
    */
    inline TBool IsIbssBitSet() const;
    /**
    * Evaluates if Privaecy bit bit is up
    * @return ETrue if bit is up, otherwise EFalse
    */
    inline TBool IsPrivacyBitSet() const;
    /**
    * Evaluates if Short Preamble bit is up
    * @return ETrue if bit is up, otherwise EFalse
    */
    inline TBool IsShortPreambleBitSet() const;
    /**
    * Evaluates if PBCC bit is up
    * @return ETrue if bit is up, otherwise EFalse
    */
    inline TBool IsPbccBitSet() const;
    /**
    * Evaluates if Channel Agility bit is up
    * @return ETrue if bit is up, otherwise EFalse
    */
    inline TBool IsChannelAgilityBitSet() const;
    /**
    * Evaluates if short slot time bit is up
    * @return ETrue if bit is up, otherwise EFalse
    */
    inline TBool IsShortSlotTimeBitSet() const;
    /**
    * Clears both CF bits
    */
    inline void ClearCfFields();    
    /**
    * Clear CF pollable field
    */
    inline void ClearCfPollable();    
    /**
    * Clears both CF poll request field
    */
    inline void ClearCfPollRequest();    
    /**
    * Sets the short preamble bit
    */
    inline void SetShortPreamble();
    /**
    * Clears the short preamble bit
    */
    inline void ClearShortPreamble();
    /**
    * sets the pbcc bit
    */
    inline void SetPbcc();
    /**
    * sets the IBSS bit
    */
    inline void SetIbss();
    /**
    * Clear PBCC bit
    */
    inline void ClearPbcc();
    /** Clear reserved fields */
    inline void ClearReservedFields();
    /** Set WEP bit */
    inline void SetWepBit();
    /** Clear WEP bit */
    inline void ClearWepBit();

private:

    /** Prohibit copy constructor */
    SCapabilityInformationField( const SCapabilityInformationField& );
    } __PACKED; // 2 bytes


inline SCapabilityInformationField::SCapabilityInformationField( const TUint16 aParam )
    {
    WriteHtoUint16( &iCapabilityInformationField, aParam );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline SCapabilityInformationField& SCapabilityInformationField::operator= (
    const TUint16 aParam )
    {
    WriteHtoUint16( &iCapabilityInformationField, aParam );
    return (*this);
    }

// ---------------------------------------------------------
// 
// ---------------------------------------------------------
//
inline TUint16 SCapabilityInformationField::CapabilityInformationField() const
    {
    return ( ReadUint16Toh( &iCapabilityInformationField ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SCapabilityInformationField::IsEssBitSet() const
    {
    return ( ( ReadUint16Toh( &iCapabilityInformationField ) & 
        E802Dot11CapabilityEssMask )
        ? ETrue : EFalse );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SCapabilityInformationField::IsIbssBitSet() const
    {
    return ( ( ReadUint16Toh( &iCapabilityInformationField ) & 
        E802Dot11CapabilityIbssMask )
        ? ETrue : EFalse );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SCapabilityInformationField::IsPrivacyBitSet() const
    {
    return ( ( ReadUint16Toh( &iCapabilityInformationField ) & 
        E802Dot11CapabilityPrivacyMask )
        ? ETrue : EFalse );    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SCapabilityInformationField::IsShortPreambleBitSet() const
    {
    return ( ( ReadUint16Toh( &iCapabilityInformationField ) & 
        E802Dot11ShortPreambleMask )
        ? ETrue : EFalse );    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SCapabilityInformationField::IsPbccBitSet() const
    {
    return ( ( ReadUint16Toh( &iCapabilityInformationField ) & 
        E802Dot11PbccMask )
        ? ETrue : EFalse );    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SCapabilityInformationField::IsChannelAgilityBitSet() const
    {
    return ( ( ReadUint16Toh( &iCapabilityInformationField ) & 
        E802Dot11ChannelAgilityMask )
        ? ETrue : EFalse );    
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SCapabilityInformationField::IsShortSlotTimeBitSet() const
    {
    return ( ( ReadUint16Toh( &iCapabilityInformationField ) & 
        E802Dot11ShortSlotTimeMask )
        ? ETrue : EFalse );    
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::ClearCfFields()
    {
    ClearCfPollable();
    ClearCfPollRequest();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::ClearCfPollable()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) & 
        ( ~E802Dot11CapabilityCfPollableMask ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::ClearCfPollRequest()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) & 
        ( ~E802Dot11CapabilityCfPollRequestMask ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::SetShortPreamble()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) | 
        E802Dot11ShortPreambleMask );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::SetPbcc()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) | 
        E802Dot11PbccMask );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::SetIbss()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) | 
        E802Dot11CapabilityIbssMask );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::ClearShortPreamble()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) & 
        ( ~E802Dot11ShortPreambleMask ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::ClearPbcc()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) & 
        ( ~E802Dot11PbccMask ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::ClearReservedFields()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) & 
        ( ~KReservedFieldsMask ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::SetWepBit()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) | 
        E802Dot11CapabilityPrivacyMask );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SCapabilityInformationField::ClearWepBit()
    {
    WriteHtoUint16( &iCapabilityInformationField, 
        ReadUint16Toh( &iCapabilityInformationField ) & 
        ( ~E802Dot11CapabilityPrivacyMask ) );
    }

/**
* operator== for SCapabilityInformationField
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== ( 
    const SCapabilityInformationField& aLhs,
    const SCapabilityInformationField& aRhs)
    {
    return static_cast<TBool>( aLhs.CapabilityInformationField() 
        == aRhs.CapabilityInformationField() );
    }

/**
* 802.11 management frame body listen interval fixed-field
*/
#pragma pack( 1 )
struct SListenIntervalField
    {
    /** listen interval fixed field */
    TUint16 iListenInterval;
    
    /**
    * Ctor
    */
    SListenIntervalField() : iListenInterval( 0 ) {};

    /**
    * Ctor
    * @param aParam value used in iListenInterval
    */
    explicit inline SListenIntervalField( const TUint16 aParam );

    /**
    * Returns the value of the Listen Interval
    * @return Listen Interval
    */
    inline TUint16 ListenInterval() const;

    /**
    * assignment operator for TUint16 type
    * @param aInterval listen interval fixed field
    */
    inline SListenIntervalField& operator= ( const TUint16 aInterval );           
    
private:

    /** Prohibit assignment operator */
    SListenIntervalField& operator= ( const SListenIntervalField& );
    /** Prohibit copy constructor */
    SListenIntervalField( const SListenIntervalField& );
    } __PACKED; // 2 bytes

inline SListenIntervalField::SListenIntervalField( const TUint16 aParam )
    {
    WriteHtoUint16( &iListenInterval, aParam );
    }

inline TUint16 SListenIntervalField::ListenInterval() const
    {
    return ( ReadUint16Toh( &iListenInterval ) );
    }

inline SListenIntervalField& SListenIntervalField::operator= (
    const TUint16 aInterval )
    {
    WriteHtoUint16( &iListenInterval, aInterval );
    return (*this);
    }

/**
* operator== for SListenIntervalField
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SListenIntervalField& aLhs,
    const SListenIntervalField& aRhs)
    {
    return static_cast<TBool>( aLhs.ListenInterval() == aRhs.ListenInterval() );
    }


/*
        802.11 DATA Frame
       +----------------+
       |                |
       |  Frame Control |
       |    2 bytes     | 
       +----------------+
       |                |
       |  Duration ID   |
       |    2 bytes     |        
       +----------------+
       |                |
       |   Address 1    |
       |    6 bytes     |
       +----------------+
       |                |
       |   Address 2    |
       |    6 bytes     |
       +----------------+
       |                |
       |   Address 3    |
       |    6 bytes     |
       +----------------+
       |                |
       | Sequence Cntrl |
       |    2 bytes     |
       +----------------+
       |                |
       |   Address 4    |
       |    6 bytes     |
       +----------------+
       |  DSAP - 1 byte | = 0xAA ( SNAP )
       +----------------+
       |  SSAP - 1 byte | = 0xAA ( SNAP )
       +----------------+
       |Control - 1 byte| = 0x03 
       +----------------+ 
       | OUI - 3 bytes  | = 0x0
       |                | 
       +----------------+
       | Type - 2 bytes |  = Ethernet type (IP=0x0800)
       +----------------+ 
       |                | 
       |      Data      |
       |                |
       ~                ~
       ~                ~
       |   46 to 1500   |
       |     bytes      |
       |                |
       +----------------+
       |      FCS       |
       |    4 bytes     |
       +----------------+

*/

// FrameControl field of the 802.11 header
//
// |--------------------- control -----------------------|
//
// bit 15    14     13     12     11     10      9     8  
// +-------+-----+------+-----+-------+------+------+----+
// | Order | WEP | More | Pwr | Retry | More | From | To |
// |       |     | Data | Mgmt|       | Frag | DS   | DS |
// +-------+-----+------+-----+-------+------+------+----+
//    1      1      1     1      1       1      1     1   
//--------- type ------------|
//
//   7-4     3-2      1-0
//---------+------+----------+
// Subtype | Type | Protocol |
//         |      | Version  |
//---------+------+----------+
//    4       2        2

/**
* 802.11 Frame Control field 
*/
#pragma pack( 1 )
struct SFrameControl
    {
    /** type field */
    TUint8 iType;
    /** control filed */
    TUint8 iControl;

    /**
    * Ctor
    * @param aType type field
    * @param aControl control field
    */
    SFrameControl( 
        T802Dot11FrameControlTypeMask aType, 
        T802Dot11FrameControlBitMask aControl ) 
        : iType( static_cast<TUint8>(aType) ), 
        iControl( static_cast<TUint8>(aControl) ) {};

    /**
    * Returns type and control fields combined as a single TUint16 value
    * @return see above
    */
    inline TUint16 operator()() const;

private:

    /** Prohibit assignment operator */
    SFrameControl& operator= ( const SFrameControl& aObj );    
    /** Prohibit copy constructor */
    SFrameControl( const SFrameControl& );
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint16 SFrameControl::operator()() const
    {
    TUint16 value( iType );
    return ( static_cast<TUint16>(( value << 8 ) + iControl ));
    }

/**
* operator== for SFrameControl
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SFrameControl& aLhs,
    const SFrameControl& aRhs)
    {
    return static_cast<TBool>( aLhs.iType == aRhs.iType
        && aLhs.iControl == aRhs.iControl );
    }


// SequenceControl field of the 802.11 header
//
// bit      15 - 4         3 - 0
// +-------------------+-----------+
// |  Sequence Number  | Fragment  |
// |                   |  Number   |
// +-------------------+-----------+
//          12              4

#pragma pack( 1 )
struct SPsPoll
    {
    const SFrameControl   iFrameControl;  // 2 bytes
    TUint16               iAid;           // 2 bytes
    const TMacAddress     iBssId;         // 6 bytes
    const TMacAddress     iTa;            // 6 bytes

    inline SPsPoll( 
        TUint16 aAid, 
        const TMacAddress& aBssId, 
        const TMacAddress& aTa );
    } __PACKED; // 16 bytes


inline SPsPoll::SPsPoll( 
    TUint16 aAid, 
    const TMacAddress& aBssId, 
    const TMacAddress& aTa )
    : iFrameControl( E802Dot11FrameTypePowerSavePoll, 
    static_cast<T802Dot11FrameControlBitMask>(0) ),
    iAid( 0 ),
    iBssId( aBssId ), iTa( aTa )
    {
    // AID always has the 2 most significant bits set to 1
    WriteHtoUint16( &iAid, ( aAid | 0xC000  ) );
    }

/**
* 802.11 data frame MAC header
*/
#pragma pack( 1 )
struct SDataFrameHeader
    {
    /** frame control field */
    SFrameControl   iFrameControl;  // 2 bytes
    /** duration field */
    const TUint16   iDuration;      // 2 bytes
    /** address1 field */
    TMacAddress     iAddress1;      // 6 bytes    
    /** address2 field */
    TMacAddress     iAddress2;      // 6 bytes
    /** address3 field */
    TMacAddress     iAddress3;      // 6 bytes
    /** sequence control field */
    const TUint16   iSeqCtl;        // 2 bytes
    // this littly piggy is only used in AP-AP mode 
    // which we don't do so it is omitted
    // const TMacAddress iAddress4;      // 6 bytes   
    
    /**
    * Ctor
    */ 
    SDataFrameHeader() 
        : iFrameControl( E802Dot11FrameTypeData, 
        static_cast<T802Dot11FrameControlBitMask>(0) )
        ,iDuration( 0 ), iSeqCtl( 0 ) {};

    /**
    * Sets WEP bit from Frame Control field
    */
    inline void SetWepBit();
    /**
    * Clears WEP bit from Frame Control field
    */
    inline void ClearWepBit();
    /**
    * Sets ToDS bit from Frame Control field
    */
    inline void SetToDsBit();
    /**
    * Clears ToDS bit from Frame Control field
    */
    inline void ClearToDsBit();
    /**
    * Clears FromDS bit from Frame Control field
    */
    inline void ClearFromDsBit();
    /**
    * Evaluates is FromDS bit set from Frame Control field
    */
    inline TBool IsFromDsBitSet() const;
    inline TBool IsToDsBitSet() const;
    /**
    * Evaluates is WEP bit set from Frame Control field
    */
    inline TBool IsWepBitSet() const;
    /**
    * Gets Frame Control field
    * @return reference to the frame control field
    */
    inline const SFrameControl& GetFrameControl() const;

    /**
    * Returns Sequence Number from iSeqCtl field
    * @return Sequence Number
    */
    inline TUint16 SequenceNumber() const;

private:

    /** Prohibit assignment operator */
    SDataFrameHeader& operator= ( const SDataFrameHeader& aObj );    
    /** Prohibit copy constructor */
    SDataFrameHeader( const SDataFrameHeader& );
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SDataFrameHeader::SetWepBit()
    {
    iFrameControl.iControl |= ( E802Dot11FrameControlWepMask >> 8 );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SDataFrameHeader::ClearWepBit()
    {
    iFrameControl.iControl &= ~( E802Dot11FrameControlWepMask >> 8 );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SDataFrameHeader::SetToDsBit()
    {
    iFrameControl.iControl |= ( E802Dot11FrameControlToDsMask >> 8 );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SDataFrameHeader::ClearToDsBit()
    {
    iFrameControl.iControl &= ~( E802Dot11FrameControlToDsMask >> 8 );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SDataFrameHeader::ClearFromDsBit()
    {
    iFrameControl.iControl &= ~( E802Dot11FrameControlFromDsMask >> 8 );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SDataFrameHeader::IsFromDsBitSet() const
    {
    return static_cast<TBool>( iFrameControl.iControl & ( 
        E802Dot11FrameControlFromDsMask >> 8 ));
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SDataFrameHeader::IsToDsBitSet() const
    {
    return static_cast<TBool>( iFrameControl.iControl & ( 
        E802Dot11FrameControlToDsMask >> 8 ));
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TBool SDataFrameHeader::IsWepBitSet() const
    {
    return static_cast<TBool>( iFrameControl.iControl & ( 
        E802Dot11FrameControlWepMask >> 8 ));
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline const SFrameControl& SDataFrameHeader::GetFrameControl() const
    {
    return iFrameControl;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
inline TUint16 SDataFrameHeader::SequenceNumber() const
    {
    return ( ReadUint16Toh( &iSeqCtl ) >> 4 );    
    }


typedef SDataFrameHeader SNullDataFrame;
typedef SDataFrameHeader Sdot11MacHeader;

typedef TUint16 T802Dot11QosControl;

/**
* According to the WiFi-WMM Test Specification v1.2 bits 3-5 of
* the DSCP bits 0-5 of the DS field determine the user priority
* This is a mask for those bits for IPv4 header
*/
const T802Dot11QosControl KWmmUserPriorityMask = 0x0007;

/**
* 802.11 QoS data frame MAC header
*/
#pragma pack( 1 )
struct SQosDataFrameHeader
    {
    /** frame control field */
    SDataFrameHeader iHdr;              // 24 bytes
    /** QoS control field */
    T802Dot11QosControl iQosControl;    // 2 bytes
    
    /**
    * Resets the QoS Control field to zero
    */
    inline void ResetQosControl();

    /**
    * Sets the WMM user priority (3 lowest bits) of the QoS Control field
    */
    inline void SetUserPriority( TUint8 aPriority );

    /**
    * Returns the WMM user priority (3 lowest bits) of the QoS Control field
    */
    inline TUint8 UserPriority() const;
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SQosDataFrameHeader::ResetQosControl()
    {
    iQosControl = 0;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SQosDataFrameHeader::SetUserPriority( TUint8 aPriority )
    {
    // clear old priority
    WriteHtoUint16( &iQosControl, 
        ReadUint16Toh( &iQosControl ) & 
        ( ~KWmmUserPriorityMask ) );
    // set new priority
    WriteHtoUint16( &iQosControl, 
        ReadUint16Toh( &iQosControl ) | 
        aPriority );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SQosDataFrameHeader::UserPriority() const
    {
    return ( ReadUint16Toh( &iQosControl ) & KWmmUserPriorityMask );
    }

typedef SQosDataFrameHeader SQosNullDataFrame;

/**
* 802.11 management frame MAC header
*/
#pragma pack( 1 )
struct SManagementFrameHeader
    {
    /** frame control field */
    SFrameControl   iFrameControl;  // 2 bytes 
    /** duration field */
    const TUint16   iDuration;      // 2 bytes
    /** DA address field */
    TMacAddress     iDA;            // 6 bytes frames destination = AP   
    /** SA address field */
    TMacAddress     iSA;            // 6 bytes source address
    /** BSSID address field */
    TMacAddress     iBSSID;         // 6 bytes BSS identifier = iDA
    /** sequence control field */
    const TUint16   iSeqCtl;        // 2 bytes
        
    /**
    * Ctor 
    * @param aTypeMask frame control type mask
    * @param aControlMask frame control control mask
    */
    SManagementFrameHeader( 
        T802Dot11FrameControlTypeMask aTypeMask, 
        T802Dot11FrameControlBitMask aControlMask 
        = static_cast<T802Dot11FrameControlBitMask>( 0 ) ) 
        : iFrameControl( aTypeMask, aControlMask ), 
        iDuration( 0 ), iSeqCtl( 0 ) {};

    /**
    * Set WEP bit from Frame Control Field
    */
    void SetWepBit();
    /**
    * Clear WEP bit from Frame Control Field
    */
    void ClearWepBit();

private:

    /** Prohibit assignment operator */
    SManagementFrameHeader& operator= ( SManagementFrameHeader& aObj );
    /** Prohibit copy constructor */
    SManagementFrameHeader( const SManagementFrameHeader& );
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SManagementFrameHeader::SetWepBit()
    {
    iFrameControl.iControl |= ( E802Dot11FrameControlWepMask >> 8 );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SManagementFrameHeader::ClearWepBit()
    {
    iFrameControl.iControl &= ~( E802Dot11FrameControlWepMask >> 8 );
    }

/**
* 802.11 fixed length components required in 
* authentication request management frame
*/
#pragma pack( 1 )
struct SAuthenticationFixedFields
    {
    /** algorithm number */
    TUint16   iAlgorithmNumber;     // 2 bytes
    /** sequence number */
    TUint16   iSequenceNmbr;        // 2 bytes
    /** status code */
    TUint16   iStatusCode;          // 2 bytes
    
    /**
    * Ctor
    * @param aAlgorithm authentication mode used
    * @param aSeqNmbr sequence number used
    * @param aStatusCode status code used
    */
    inline SAuthenticationFixedFields( 
        const  TUint16 aAlgorithm = K802Dot11AuthModeOpen, 
        const TUint16 aSeqNmbr = E802Dot11AuthenticationSeqNmbr1, 
        const T802Dot11ManagementStatusCode aStatusCode 
        = E802Dot11StatusSuccess );
    
    /**
    * Returns the authentication transaction algorithm number
    * @return sequence number
    */
    inline TUint16 AlgorithmNumber() const;

    /**
    * Sets the authentication transaction algorithm number
    * @param aSequenceNumber value to be set
    */
    inline void SetAlgorithmNumber( 
        TUint16 aAlgorithmNumber );

    /**
    * Returns the authentication transaction sequence number
    * @return sequence number
    */
    inline TUint16 SequenceNumber() const;

    /**
    * Sets the authentication transaction sequence number
    * @param aSequenceNumber value to be set
    */
    inline void SetSequenceNumber( TUint16 aSequenceNumber );

    /**
    * Returns the authentication transaction status code
    * @return status code
    */
    inline TUint16 StatusCode() const;

    } __PACKED;
    
inline SAuthenticationFixedFields::SAuthenticationFixedFields( 
    const  TUint16 aAlgorithm, 
    const TUint16 aSeqNmbr, 
    const T802Dot11ManagementStatusCode aStatusCode )
    {
    WriteHtoUint16( &iAlgorithmNumber, static_cast<TUint16>( aAlgorithm ) );
    WriteHtoUint16( &iSequenceNmbr, static_cast<TUint16>( aSeqNmbr ) );
    WriteHtoUint16( &iStatusCode, static_cast<TUint16>( aStatusCode ) );    
    }
    
inline TUint16 SAuthenticationFixedFields::AlgorithmNumber() const
    {
    return ( ReadUint16Toh( &iAlgorithmNumber ) );    
    }

inline void SAuthenticationFixedFields::SetAlgorithmNumber( TUint16 aAlgorithmNumber )
    {
    WriteHtoUint16( &iAlgorithmNumber, aAlgorithmNumber );    
    }

inline TUint16 SAuthenticationFixedFields::SequenceNumber() const
    {
    return ( ReadUint16Toh( &iSequenceNmbr ) );
    }
    
inline void SAuthenticationFixedFields::SetSequenceNumber( 
    TUint16 aSequenceNumber )
    {
    WriteHtoUint16( &iSequenceNmbr, aSequenceNumber );
    }

inline TUint16 SAuthenticationFixedFields::StatusCode() const
    {
    return ( ReadUint16Toh( &iStatusCode ) );
    }    
    

/**
* 802.11 authentication management frame
*/
#pragma pack( 1 )
struct SAuthenticationFrame
    {
    /** management frame header */
    SManagementFrameHeader      iHeader;                // 24 bytes
    /** authentication frame fixed fields */
    SAuthenticationFixedFields  iAuthenticationFields;  // 6 bytes

    /** Ctor */
    SAuthenticationFrame() 
        : iHeader( E802Dot11FrameTypeAuthentication ) {};
    
    /** Increments sequnece number to next number we shall send */
    inline void IncrementSeqNmbr();

    /** Resets sequnece number to initial value */
    inline void ResetSeqNmbr();

    /**
    * Gets the frames sequence number
    * @return frames sequence number
    */
    inline TUint16 GetSeqNmbr() const;

    /**
    * Gets the frames status code field
    * @return frames status code field
    */
    inline TUint16 GetStatusCode() const;

    /**
    * Gets the algorithm number 
    * @return algorithm number
    */
    inline TUint16 GetAlgorithmNumber() const;

    /** Sets the WEP bit from frame control field */
    inline void SetWepBit();

    /** Clears the WEP bit from frame control field */
    inline void ClearWepBit();

    /**
    * Sets the the algorithm number field
    * @param aAlgorithm algorithm to be used
    */
    inline void SetAlgorithmNmbr( TUint16 aAlgorithm );

private:

    /** Prohibit assignment operator */
    SAuthenticationFrame& operator= ( const SAuthenticationFrame& );
    /** Prohibit copy constructor */
    SAuthenticationFrame( const SAuthenticationFrame& );
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAuthenticationFrame::IncrementSeqNmbr()
    {
    iAuthenticationFields.SetSequenceNumber( 
        iAuthenticationFields.SequenceNumber() + 2 );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAuthenticationFrame::ResetSeqNmbr()
    {
    iAuthenticationFields.SetSequenceNumber( E802Dot11AuthenticationSeqNmbr1 );        
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint16 SAuthenticationFrame::GetSeqNmbr() const
    {
    return ( iAuthenticationFields.SequenceNumber() );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint16 SAuthenticationFrame::GetStatusCode() const
    {
    return ( iAuthenticationFields.StatusCode() );    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAuthenticationFrame::SetAlgorithmNmbr( 
    TUint16 aAlgorithm )
    {
    iAuthenticationFields.SetAlgorithmNumber( aAlgorithm );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint16 SAuthenticationFrame::GetAlgorithmNumber() const
    {
    return ( iAuthenticationFields.AlgorithmNumber() );        
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAuthenticationFrame::SetWepBit()
    {
    iHeader.SetWepBit();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAuthenticationFrame::ClearWepBit()
    {
    iHeader.ClearWepBit();
    }

/**
* 802.11 fixed length components required 
* in association request management frame
*/
#pragma pack( 1 )
struct SAssociationRequestFixedFields
    {
    /** capability info fixed field */
    SCapabilityInformationField iCapabilityInfo;    // 2 bytes
    /** listeninterval fixed field */
    SListenIntervalField iListenInterval;           // 2 bytes

    /** Ctor */
    SAssociationRequestFixedFields() {};

private:

    /** Prohibit assignment operator */
    SAssociationRequestFixedFields& operator= ( 
        const SAssociationRequestFixedFields& );
    /** Prohibit copy constructor */
    SAssociationRequestFixedFields( 
        const SAssociationRequestFixedFields& );
    } __PACKED;

/**
* operator== for SAssociationRequestFixedFields
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SAssociationRequestFixedFields& aLhs,
    const SAssociationRequestFixedFields& aRhs)
    {
    return static_cast<TBool>( aLhs.iCapabilityInfo == aRhs.iCapabilityInfo
        && aLhs.iListenInterval == aRhs.iListenInterval );
    }


/**
* 802.11 association request management frame
* excluding variable length information elements
* - SSID
* - supported rates
*/
#pragma pack( 1 )
struct SAssociationRequestFrame
    {
    /** management frame header */
    SManagementFrameHeader          iHeader;        // 24 bytes
    /** association request fixed fields */
    SAssociationRequestFixedFields  iFixedFields;   // 4 bytes

    /** Ctor */
    SAssociationRequestFrame() 
        : iHeader( E802Dot11FrameTypeAssociationReq ),
        iFixedFields() {}; 

    /**
    * Helper function to set short preamble bit in capability info
    */
    inline void SetCapabilityShortPreamble();

    /**
    * Helper function to clear short preamble bit in capability info
    */
    inline void ClearCapabilityShortPreamble();

    /**
    * Helper function to set PBCC bit in capability info
    */
    inline void SetCapabilityPbcc();

    /**
    * Helper function to clear PBCC bit in capability info
    */
    inline void ClearCapabilityPbcc();

    /**
    * Helper function to clear CF fields from capability info
    */
    inline void ClearCFfields();

    /** Helper function to clear reserved fields */
    inline void ClearReservedFields();

    /** Helper function to set WEP bit from the capability info fixed field */
    inline void SetWepBit();

    /** 
    * Helper function to clear WEP bit from 
    * the capability info fixed field 
    */
    inline void ClearWepBit();

private:

    /** Prohibit assignment operator */
    SAssociationRequestFrame& operator= ( 
        const SAssociationRequestFrame& );
    /** Prohibit copy constructor */
    SAssociationRequestFrame( 
        const SAssociationRequestFrame& );
    } __PACKED; // 28 bytes

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAssociationRequestFrame::SetCapabilityShortPreamble()
    {
    iFixedFields.iCapabilityInfo.SetShortPreamble();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAssociationRequestFrame::ClearCapabilityShortPreamble()
    {
    iFixedFields.iCapabilityInfo.ClearShortPreamble();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAssociationRequestFrame::SetCapabilityPbcc()
    {
    iFixedFields.iCapabilityInfo.SetPbcc();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAssociationRequestFrame::ClearCapabilityPbcc()
    {
    iFixedFields.iCapabilityInfo.ClearPbcc();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAssociationRequestFrame::ClearCFfields()
    {
    iFixedFields.iCapabilityInfo.ClearCfFields();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAssociationRequestFrame::ClearReservedFields()
    {
    iFixedFields.iCapabilityInfo.ClearReservedFields();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAssociationRequestFrame::SetWepBit()
    {
    iFixedFields.iCapabilityInfo.SetWepBit();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SAssociationRequestFrame::ClearWepBit()
    {
    iFixedFields.iCapabilityInfo.ClearWepBit();
    }

/**
* 802.11 association response frame fixed fields
*/
#pragma pack( 1 )
struct SAssociationResponseFixedFields
    {
    /** capability info fixed field */
    SCapabilityInformationField iCapabilityInfo;    // 2 bytes
    /** status code fixed field */
    TUint16                     iStatusCode;        // 2 bytes
    /** AID fixed field */
    TUint16                     iAID;               // 2 bytes
    
    /**
    * Returns the association response status code
    * @return status code
    */
    inline TUint16 StatusCode() const;

    /**
    * Returns the Association ID (AID)
    * @return AID
    */
    inline TUint16 Aid() const;

private:

    /** Prohibit assignment operator */
    SAssociationResponseFixedFields& operator= ( 
        const SAssociationResponseFixedFields& );    
    /** Prohibit copy constructor */
    SAssociationResponseFixedFields( const SAssociationResponseFixedFields& );
    } __PACKED;

inline TUint16 SAssociationResponseFixedFields::StatusCode() const
    {
    return ( ReadUint16Toh( &iStatusCode ) );
    }

inline TUint16 SAssociationResponseFixedFields::Aid() const
    {
    return ( ReadUint16Toh( &iAID ) );
    }


/**
* 802.11 fixed length components required in deauthenticate frame
*/
#pragma pack( 1 )
struct SDeauthenticateFixedFields
    {
    /** reason code fixed field */
    TUint16 iReasonCode;

    /** Ctor */
    SDeauthenticateFixedFields()
        : iReasonCode( E802Dot11ReasonDeauthStationLeft ) {};

    /*
    * Returns the reason code 
    * @return Reason code
    */
    inline TUint16 ReasonCode() const;

    /** Setter for the reason code */
    inline void SetReasonCode( TUint16 aReasonCode );
    
private:

    /** Prohibit assignment operator */
    SDeauthenticateFixedFields& operator= ( 
        T802Dot11ManagementReasonCode aReason );
    /** Prohibit copy constructor */
    SDeauthenticateFixedFields( const SDeauthenticateFixedFields& );
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint16 SDeauthenticateFixedFields::ReasonCode() const
    {
    return ( ReadUint16Toh( &iReasonCode ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SDeauthenticateFixedFields::SetReasonCode( TUint16 aReasonCode )
    {
    WriteHtoUint16( &iReasonCode, aReasonCode );
    }

/**
* operator== for SDeauthenticateFixedFields
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SDeauthenticateFixedFields& aLhs,
    const SDeauthenticateFixedFields& aRhs)
    {
    return static_cast<TBool>( 
        aLhs.ReasonCode() == aRhs.ReasonCode() );
    }

/**
* 802.11 deauthenticate management frame
*/
#pragma pack( 1 )
struct SDeauthenticateFrame
    {
    /** management frame header */
    SManagementFrameHeader      iHeader;        // 24 bytes     
    /** reason code fixed field  */
    SDeauthenticateFixedFields  iReasonCode;    // 2 bytes

    /** Ctor */
    SDeauthenticateFrame() 
        : iHeader( E802Dot11FrameTypeDeauthentication ),
        iReasonCode() {};

private:

    /** Prohibit assignment operator */
    SDeauthenticateFrame& operator= ( const SDeauthenticateFrame& );
    /** Prohibit copy constructor */
    SDeauthenticateFrame( const SDeauthenticateFrame& );
    } __PACKED;

/**
* operator== for SDeauthenticateFrame
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SDeauthenticateFrame& aLhs,
    const SDeauthenticateFrame& aRhs)
    {
    return ( aLhs == aRhs );
    }

/**
* 802.11 fixed length components required in disassociate frame
*/
#pragma pack( 1 )
struct SDisAssociateFixedFields
    {
    /** reason code fixed field */
    TUint16 iReasonCode;

    /** Ctor */
    inline SDisAssociateFixedFields();

    /*
    * Returns the reason code 
    * @return Reason code
    */
    inline TUint16 ReasonCode() const;

    /** Setter for the reason code */
    inline void SetReasonCode( TUint16 aReasonCode );
    
private:
    // Prohibit assignment operator
    SDisAssociateFixedFields& operator= ( const SDisAssociateFixedFields& );
    // Prohibit copy constructor    
    SDisAssociateFixedFields( const SDisAssociateFixedFields& );
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline SDisAssociateFixedFields::SDisAssociateFixedFields()
    {
    WriteHtoUint16( &iReasonCode, E802Dot11ReasonClass3FrameWhenNotAssoc );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint16 SDisAssociateFixedFields::ReasonCode() const
    {
    return ( ReadUint16Toh( &iReasonCode ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SDisAssociateFixedFields::SetReasonCode( TUint16 aReasonCode )
    {
    WriteHtoUint16( &iReasonCode, aReasonCode );
    }

/**
* operator== for SDisAssociateFixedFields
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SDisAssociateFixedFields& aLhs,
    const SDisAssociateFixedFields& aRhs)
    {
    return static_cast<TBool>( 
        aLhs.ReasonCode() == aRhs.ReasonCode() );
    }

/**
* 802.11 disassociate management frame
*/
#pragma pack( 1 )
struct SDisassociateFrame
    {
    /** management frame header */
    SManagementFrameHeader      iHeader;        // 24 bytes 
    /** reason code fixed field  */
    SDisAssociateFixedFields    iReasonCode;    // 2 bytes

    /** Ctor */
    SDisassociateFrame()
        : iHeader( E802Dot11FrameTypeDisassociation ),
        iReasonCode() {};

private:

    /** Prohibit assignment operator */
    SDisassociateFrame& operator= ( const SDisassociateFrame& );
    /** Prohibit copy constructor */
    SDisassociateFrame( const SDisassociateFrame& );
    } __PACKED; // 26 bytes

/**
* operator== for SDisassociateFrame
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SDisassociateFrame& aLhs,
    const SDisassociateFrame& aRhs)
    {
    return ( aLhs == aRhs );
    }

/**
* 802.11 fixed length components required 
* in reassociation request management frame
*/
#pragma pack( 1 )
struct SReassociationRequestFixedFields
    {
    /** capability info fixed field */
    SCapabilityInformationField iCapabilityInfo;    // 2 bytes
    /** listeninterval fixed field */
    SListenIntervalField iListenInterval;           // 2 bytes
    /** current AP address fixed field */
    TMacAddress iCurrentApAddress;                  // 6 bytes

    /** Ctor */
    SReassociationRequestFixedFields() {};

private:

    /** Prohibit assignment operator */
    SReassociationRequestFixedFields& operator= ( 
        const SReassociationRequestFixedFields& );
    /** Prohibit copy constructor */
    SReassociationRequestFixedFields( 
        const SReassociationRequestFixedFields& );
    } __PACKED;

/**
* operator== for SReassociationRequestFixedFields
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SReassociationRequestFixedFields& aLhs,
    const SReassociationRequestFixedFields& aRhs)
    {
    return static_cast<TBool>( aLhs.iCapabilityInfo == aRhs.iCapabilityInfo
                               && aLhs.iListenInterval == aRhs.iListenInterval
                               && aLhs.iCurrentApAddress == aRhs.iCurrentApAddress );
    }

/**
* 802.11 reassociation request management frame
* excluding variable length information elements
*/
#pragma pack( 1 )
struct SReassociationRequestFrame
    {
    /** management frame header */
    SManagementFrameHeader          iHeader;        // 24 bytes
    /** association request fixed fields */
    SReassociationRequestFixedFields  iFixedFields; // 10 bytes

    /** Ctor */
    SReassociationRequestFrame() 
        : iHeader( E802Dot11FrameTypeReassociationReq ),
        iFixedFields() {}; 

    /**
    * Helper function to set short preamble bit in capability info
    */
    inline void SetCapabilityShortPreamble();

    /**
    * Helper function to clear short preamble bit in capability info
    */
    inline void ClearCapabilityShortPreamble();

    /**
    * Helper function to set PBCC bit in capability info
    */
    inline void SetCapabilityPbcc();

    /**
    * Helper function to clear PBCC bit in capability info
    */
    inline void ClearCapabilityPbcc();

    /**
    * Helper function to clear CF fields from capability info
    */
    inline void ClearCFfields();

    /** Helper function to clear reserved fields */
    inline void ClearReservedFields();

    /** Helper function to set WEP bit from the capability info fixed field */
    inline void SetWepBit();

    /** 
    * Helper function to clear WEP bit from 
    * the capability info fixed field 
    */
    inline void ClearWepBit();

private:

    /** Prohibit assignment operator */
    SReassociationRequestFrame& operator= ( 
        const SReassociationRequestFrame& );
    /** Prohibit copy constructor */
    SReassociationRequestFrame( 
        const SReassociationRequestFrame& );
    } __PACKED; // 34 bytes

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SReassociationRequestFrame::SetCapabilityShortPreamble()
    {
    iFixedFields.iCapabilityInfo.SetShortPreamble();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SReassociationRequestFrame::ClearCapabilityShortPreamble()
    {
    iFixedFields.iCapabilityInfo.ClearShortPreamble();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SReassociationRequestFrame::SetCapabilityPbcc()
    {
    iFixedFields.iCapabilityInfo.SetPbcc();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SReassociationRequestFrame::ClearCapabilityPbcc()
    {
    iFixedFields.iCapabilityInfo.ClearPbcc();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SReassociationRequestFrame::ClearCFfields()
    {
    iFixedFields.iCapabilityInfo.ClearCfFields();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SReassociationRequestFrame::ClearReservedFields()
    {
    iFixedFields.iCapabilityInfo.ClearReservedFields();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SReassociationRequestFrame::SetWepBit()
    {
    iFixedFields.iCapabilityInfo.SetWepBit();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline void SReassociationRequestFrame::ClearWepBit()
    {
    iFixedFields.iCapabilityInfo.ClearWepBit();
    }

/**
* 802.11 reassociation response frame fixed fields
*/
#pragma pack( 1 )
struct SReassociationResponseFixedFields
    {
    /** capability info fixed field */
    SCapabilityInformationField iCapabilityInfo;    // 2 bytes
    /** status code fixed field */
    TUint16                     iStatusCode;        // 2 bytes
    /** AID fixed field */
    TUint16                     iAID;               // 2 bytes

    /*
    * Returns the reassociation response status code
    * @return status code
    */
    inline TUint16 StatusCode() const;

    /*
    * Returns the Asociation ID (AID)
    * @return AID
    */
    inline TUint16 Aid() const;

private:

    /** Prohibit assignment operator */
    SReassociationResponseFixedFields& operator= ( 
        const SReassociationResponseFixedFields& );    
    /** Prohibit copy constructor */
    SReassociationResponseFixedFields( const SReassociationResponseFixedFields& );
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint16 SReassociationResponseFixedFields::StatusCode() const
    {
    return ( ReadUint16Toh( &iStatusCode ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint16 SReassociationResponseFixedFields::Aid() const
    {
    return ( ReadUint16Toh( &iAID ) );
    }


/**
* SNAP header
*/
#pragma pack( 1 )
struct SSnapHeader
    {
    /** destination service access point */
    TUint8 iDSAP;
    /** source service access point */
    TUint8 iSSAP;
    /** control field */
    TUint8 iControl;
    /** organizationally unique identifier */
    TUint8 iOUI[KOIULength];    // 3
    } __PACKED; // 6 bytes

/**
* operator== for SSnapHeader
* @param aLhs left hand side
* @param aRhs right hand side
* @return ETrue equal, EFalse not equal
*/
inline TBool operator== (
    const SSnapHeader& aLhs,
    const SSnapHeader& aRhs)
    {    
    return ( equal( reinterpret_cast<const TUint8*>(&aLhs),
                    reinterpret_cast<const TUint8*>(&aLhs) + sizeof(SSnapHeader), 
                    reinterpret_cast<const TUint8*>(&aRhs) ));    
    }

/**
* RFC 1042 encapsulation SNAP header
*/
const SSnapHeader KEncapsulatingRfc1042SnapHeader 
    = { 0xAA, 0xAA, 0x03, { 0x00, 0x00, 0x00 } };

/**
* 802.1 h SNAP header
*/
const SSnapHeader KEncapsulating802_1hSnapHeader 
    = { 0xAA, 0xAA, 0x03, { 0x00, 0x00, 0xF8 } };

/**
* 802.11 data frame encapsulation header
*/ 
#pragma pack( 1 )
struct SDataFrameEncapsulationHeader
    {
    /** SNAP header */
    SSnapHeader     iSnapHeader;    // 6    
    } __PACKED;

/**
* 802.11 data frame and its encapsulation header combined
*/ 
#pragma pack( 1 )
struct SDataMpduHeader
    {
    /** Ctor */
    SDataMpduHeader()
        {
        iEncHdr.iSnapHeader = KEncapsulatingRfc1042SnapHeader;
        }

    /** 802.11 data frame header */
    SDataFrameHeader iHdr;                  // 24 
    /** 802.11 data frame encapsulation header */
    SDataFrameEncapsulationHeader iEncHdr;  // 6
    } __PACKED; // 30

/*
        802.2 SNAP DATA Frame 
       +----------------+
       |                |
       |  Destination   |
       |    6 bytes     |
       +----------------+
       |                |
       |     Source     |
       |    6 bytes     |
       +----------------+
       |  Frame Length  | Must be <= 1500 Dec.
       |    2 bytes     |
       +----------------+
       |  DSAP - 1 byte | = 0xAA ( SNAP )
       +----------------+
       |  SSAP - 1 byte | = 0xAA ( SNAP )
       +----------------+
       |Control - 1 byte| = 0x03 
       +----------------+ 
       | OUI - 3 bytes  | = 0x0
       |                | 
       +----------------+
       | Type - 2 bytes |  = Ethernet type (IP=0x0800)
       +----------------+ 
       |                | 
       |      Data      |
       |                |
       ~                ~
       ~                ~
       |   46 to 1500   |
       |     bytes      |
       |                |
       +----------------+
       |      FCS       |
       |    4 bytes     |
       +----------------+
*/

/*
This is an Ethernet Version 2 frame:

       +--------------+
       |              | The destination address is a six byte Media Access
       | Destination  | Control (MAC) address, usually burned into the
       |   6 bytes    | ROM of the Ethernet card.
       +--------------+
       |              | The source address is a six byte MAC address, and
       |   Source     | can signify a physical station or a broadcast.
       |   6 bytes    |
       +--------------+
       |     Type     | The Type field it must be grater then 1500 dec.
       |    2 bytes   |
       +--------------+
       |              |  Any higher layer information is placed in the
       |    Data      |  data field, which could contain protocol
       |              |  information or user data.
       ~              ~
       ~              ~
       |  46 to 1500  |
       |    bytes     |
       |              |
       +--------------+
       |     FCS      |  
       |   4 bytes    |  
       +--------------+  
                         
*/

/**
* 802.11 QoS data frame and its encapsulation header combined
*/ 
#pragma pack( 1 )
struct SQosDataMpduHeader
    {
    /** Ctor */
    SQosDataMpduHeader()
        {
        iEncHdr.iSnapHeader = KEncapsulatingRfc1042SnapHeader;
        }

    /** 802.11 QoS data frame header */
    SQosDataFrameHeader iHdr;               // 26 
    /** 802.11 data frame encapsulation header */
    SDataFrameEncapsulationHeader iEncHdr;  // 6
    } __PACKED; // 33

const TUint16 KUint16HiByteMask = 0xFF00;

/**
* Ethernet 14-byte Header (RFC 894)
*/
#pragma pack( 1 )
struct SEthernetHeader 
    {
    /** destination MAC address */
    TMacAddress iDA;    // 6
    /** source MAC address */
    TMacAddress iSA;    // 6
    /** ethernet type field */
    TUint16     iType;  // 2

    /**
    * Returns the Ethernet Type
    * @return ethernet type
    */
    inline TUint16 Type() const;
    } __PACKED;

// ---------------------------------------------------------------------------
// If the type is other than EAPOL (e.g. IP or ARP), we need to reverse the 
// byte order as the IP world uses different byter order (MSB first) as WLAN 
// MAC layer (LSB first)
// ---------------------------------------------------------------------------
//
inline TUint16 SEthernetHeader::Type() const
    {
    return ReverseUint16( ReadUint16Toh( &iType ) );
    }

/**
* Ethernet 802.2/802.3 Header (RFC 1042)
*/
#pragma pack( 1 )
struct SEthernet802_3Header
    {
    /** destination MAC address */
    TMacAddress iDA;        // 6
    /** source MAC address */
    TMacAddress iSA;        // 6
    /** length of frame */
    TUint16     iLength;    // 2
    /** SNAP header */
    SSnapHeader iSnapHeader;// 6
    /** type field */    
    TUint16     iType;      // 2
    } __PACKED;

/**
* Ethernet type field
*/
#pragma pack( 1 )
struct SEthernetType
    {
    /** type field */    
    TUint16     iType;      // 2

    /**
    * Returns the Ethernet Type
    * @return ethernet type
    */
    inline TUint16 Type() const;    
    } __PACKED;

// ---------------------------------------------------------------------------
// We need to reverse the byte order as the IP world uses different byter 
// order (MSB first) as WLAN MAC layer (LSB first)
// ---------------------------------------------------------------------------
//
inline TUint16 SEthernetType::Type() const
    {
    return ReverseUint16( ReadUint16Toh( &iType ) );
    }

/**
* IP version mask
*/
const TUint8 KIpVersionMask = 0xF0;

/**
* The DS (differentiated services) field according to RFC 2474
* Bits 0-5: DSCP (differentiated services codepoint)
* Bits 6-7: CU (currently unused)
*/
typedef TUint8 TDsField;

/**
* According to the WiFi-WMM Test Specification v1.2 the three highest 
* bits of the DS field determine the user priority
* This is a mask for those bits in IPv4 header
*/
const TUint8 KDscpIpv4Mask = 0xE0;

/**
* Version number for IPv4
*/
const TUint8 KIpv4 = 4;

/**
* Version number for IPv6
*/
const TUint8 KIpv6 = 6;

/**
* IPv4 header (RFC 791) prefix with the Type of Service field redefined
* as the DS field according to RFC 2474
*/
#pragma pack( 1 )
struct SIpv4HeaderPrefix
    {
    /** Version (4 highest bits) + IHL (Internet Header Length, 4 lowest 
    bits) */
    TUint8 iVersionIhl;    // 1
    /** DS field */ 
    TDsField iDs;          // 1

    /**
    * Returns the IP protocol version
    */
    inline TUint8 Version() const;

    /**
    * Returns the 802.1d priority value (range 0-7)
    */
    inline TUint8 The802Dot1dPriority() const;
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SIpv4HeaderPrefix::Version() const
    {
    return ( iVersionIhl & KIpVersionMask ) >> 4;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SIpv4HeaderPrefix::The802Dot1dPriority() const
    {
    return ( ( iDs & KDscpIpv4Mask ) >> 5 );
    }

/**
* According to the WiFi-WMM Test Specification v1.2 the three highest 
* bits of the DS field determine the user priority
* This is a mask for those bits in IPv6 header
*/
const TUint8 KDscpIpv6Mask = 0x0E;

/**
* IPv6 header (RFC 2460) prefix with the Traffic Class field redefined
* as the DS field according to RFC 2474
*/
#pragma pack( 1 )
struct SIpv6HeaderPrefix
    {
    /** 
    * Version (highest 4 bits) + highest 4 bits of the DS (in lowest 4 bits
    * of this byte)
    */
    TUint8 iVersionDs;    // 1

    /**
    * Returns the 802.1d priority value (range 0-7)
    */
    inline TUint8 The802Dot1dPriority() const;
    } __PACKED;

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
inline TUint8 SIpv6HeaderPrefix::The802Dot1dPriority() const
    {
    return ( ( iVersionDs & KDscpIpv6Mask ) >> 1 );
    }

// pop the pragma pack stack to return to normal alignment of structures
#pragma pack( ) 

/**
* typedef for WEP key
*/
typedef TUint8 TWepKey[KMaxWEPKeyLength];

/**
* Struct for mapped key: key + mac address
*/
struct SMappedWepKey
    {
    TMacAddress MacAddress;
    TWepKey WepKey;
    };

/**
* Struct for tkip key
*/
struct STkipKey
    {
    TMacAddress MacAddress;
    TWepKey WepKey;
    TUint8 KeyId;
    TUint8 KeyIndicator;
    TUint8 KeyIndex;
    };

#endif      // WLAN802DOT11_H   
