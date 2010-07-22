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
* WLAN Management API stubbing for WLAN Qt Utilities.
*/

#ifndef WLANMGMTCLIENT_H
#define WLANMGMTCLIENT_H

#include "wlanmgmtcommon.h"
#include "wlanscaninfo.h"

// CLASS DECLARATION

class CWlanMgmtClient : public CBase
    {
    public:  // Methods

        // Constructors and destructor

        /**
         * Static constructor.
         * @return Pointer to the constructed object.
         */
        static CWlanMgmtClient* NewL();

        /**
         * Destructor.
         */
        ~CWlanMgmtClient();

        /**
         * Perform a broadcast scan and return the detected WLAN networks.
         *
         * @param aStatus Status of the calling active object. On successful
         *                completion contains KErrNone, otherwise one of the
         *                system-wide error codes.
         * @param aResults Results of the scan.
         */
        void GetScanResults(
            TRequestStatus& aStatus,
            CWlanScanInfo& aResults );

        /**
         * Perform a direct scan for an SSID and return the detected WLAN networks.
         * If the SSID has zero length, a broadcast scan will be done.
         *
         * @param aSsid name of the WLAN network
         * @param aStatus Status of the calling active object. On successful
         *                completion contains KErrNone, otherwise one of the
         *                system-wide error codes.
         * @param aResults Results of the scan.
         */
        void GetScanResults(
            TWlanSsid& aSsid,
            TRequestStatus& aStatus,
            CWlanScanInfo& aResults );
        
        /**
         * Cancel an outstanding scan request.
         */
        void CancelGetScanResults();
        
    private: // Data
        
        CWlanMgmtClient();
    };

#endif // WLANMGMTCLIENT_H
