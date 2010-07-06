/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
* WLAN Management API stubbing for WLAN Qt Utilities use in emulator.
*/

#include <e32std.h>
#include <e32base.h>
#include "wlanmgmtclient.h"

// ---------------------------------------------------------
// CWlanScanRequest::NewL
// ---------------------------------------------------------
//
CWlanMgmtClient* CWlanMgmtClient::NewL()
    {
    CWlanMgmtClient* self = new CWlanMgmtClient;
    return self;
    }
    
// ---------------------------------------------------------
// CWlanScanRequest::~CWlanMgmtClient
// ---------------------------------------------------------
//
CWlanMgmtClient::~CWlanMgmtClient()
    {
    }

// ---------------------------------------------------------
// CWlanScanRequest::CWlanMgmtClient
// ---------------------------------------------------------
//
CWlanMgmtClient::CWlanMgmtClient()
    {
    }

// ---------------------------------------------------------
// CWlanScanRequest::GetScanResults
// ---------------------------------------------------------
//
void CWlanMgmtClient::GetScanResults(
    TRequestStatus& aStatus,
    CWlanScanInfo& aResults )
    {
    // Complete the request immediately
    TRequestStatus *status = &aStatus;
    User::RequestComplete(status, KErrNone);
    
    // Results are returned when asked per AP
    (void)aResults;
    }

// ---------------------------------------------------------
// CWlanScanRequest::GetScanResults
// ---------------------------------------------------------
//
void CWlanMgmtClient::GetScanResults(
    TWlanSsid& aSsid,
    TRequestStatus& aStatus,
    CWlanScanInfo& aResults )
    {
    // Complete the request immediately
    TRequestStatus *status = &aStatus;
    User::RequestComplete(status, KErrNone);
    
    // Results are returned when asked per AP
    (void)aSsid;
    (void)aResults;
    }

// ---------------------------------------------------------
// CWlanScanRequest::CancelGetScanResults
// ---------------------------------------------------------
//
void CWlanMgmtClient::CancelGetScanResults()
    {
    }
