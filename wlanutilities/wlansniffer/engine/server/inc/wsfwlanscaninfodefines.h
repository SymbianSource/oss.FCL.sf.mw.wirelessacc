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
* Description:  Header for WLAN scaninfo related constants and enums
*
*/



#ifndef WSFWLANSCANINFODEFINES_H
#define WSFWLANSCANINFODEFINES_H

/**
* Maximal number of data rate types
*/
const TUint8 KMaxNumberOfRates = 8;

/**
* Bit masks for Capability Information field.
* (from \S60\osext\wlan\wlanldd\wlan_common\umac_common\inc\802dot11.h)
*/
enum TWsf802Dot11CapabilityBitMask
    {
    E802Dot11CapabilityEssMask          = 0x0001,
    E802Dot11CapabilityIbssMask         = 0x0002,
    E802Dot11CapabilityCfPollableMask   = 0x0004,
    E802Dot11CapabilityCfPollRequestMask= 0x0008,
    E802Dot11CapabilityPrivacyMask      = 0x0010,
    // these little critters are from 802.11b spec
    E802Dot11ShortPreambleMask          = 0x0020,
    E802Dot11PbccMask                   = 0x0040,
    E802Dot11ChannelAgilityMask         = 0x0080
    };

/**
* Management frame information element IDs.
* (from \S60\osext\wlan\wlanldd\wlan_common\umac_common\inc\802dot11.h)
*/
enum TWsf802Dot11InformationElementID
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
    E802Dot11AironetIE              = 133,
    E802Dot11ApIpAddressIE          = 149,
    E802Dot11RsnIE                  = 221
    };


#endif // WSFWLANSCANINFODEFINES_H

