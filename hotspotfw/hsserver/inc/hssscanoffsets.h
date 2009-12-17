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
* Description:   Definition of scan frame offsets.
*
*/



#ifndef HSSSCANOFFSETS_H
#define HSSSCANOFFSETS_H

#include "802dot11.h"

// Offsets for the static header.
const TUint32 RX_LEVEL_OFFSET         = 0;                                                // 0
const TUint32 RX_SNR_OFFSET           = RX_LEVEL_OFFSET + sizeof(TUint32);                // 4

// Offsets for scan results.
const TUint32 LENGTH_OFFSET           = RX_SNR_OFFSET + sizeof(TUint32);                  // 8

// length of the control information proceeding the DOT11 header (DOT11_BASE_OFFSET)
const TUint32 CNTRL_HEADER_LEN        = 12;

const TUint32 DOT11_BASE_OFFSET       = CNTRL_HEADER_LEN;
// start of BSSID
const TUint32 BSSID_OFFSET            = DOT11_BASE_OFFSET + 16;           
// start of first IE in beacon of the probe response
const TUint32 BODY_OFFSET             = DOT11_BASE_OFFSET + sizeof(SManagementFrameHeader)
                                        + KTimeStampFixedFieldLength
                                        + KBeaconIntervalFixedFieldLength
                                        + KCapabilityInformationFixedFieldLength;

const TUint32 BEACON_INTERVAL_OFFSET  = DOT11_BASE_OFFSET 
                                        + sizeof(SManagementFrameHeader)
                                        + KTimeStampFixedFieldLength;
const TUint32 CAPABILITY_OFFSET       = BEACON_INTERVAL_OFFSET + KBeaconIntervalFixedFieldLength;

// start of timestamp field
const TUint32 TIMESTAMP_OFFSET        = DOT11_BASE_OFFSET + sizeof(SManagementFrameHeader);

// Offsets for the static header.
const TUint16 x30RX_LEVEL_OFFSET         = 13;
const TUint16 x30RX_SNR_OFFSET           = 12;
//const TUint16 FRAME_CNTRL_OFFSET      = 12;
const TUint16 x30BSSID_OFFSET            = 0;
const TUint16 x30BEACON_INTERVAL_OFFSET  = 52;
const TUint16 x30CAPABILITY_OFFSET       = 54;
const TUint16 x30STATUS_INFO_LENGTH      = 12;
// Offsets for scan results.
const TUint16 x30LENGTH_OFFSET           = 62;
const TUint16 x30BODY_OFFSET             = 64 + x30STATUS_INFO_LENGTH;
#endif // HSSSCANOFFSETS_H
