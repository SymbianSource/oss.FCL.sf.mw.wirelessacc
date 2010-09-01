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
* Description:   Class header for CWsfWlanSsidDiscovery
*
*/



#ifndef C_WSFWLANSSIDDISCOVERY_H
#define C_WSFWLANSSIDDISCOVERY_H


//  EXTERNAL INCLUDES
#include <e32base.h>
#include <cmpluginwlandef.h>


//  FORWARD DECLARATIONS
class CWlanMgmtClient;
class CWlanScanInfo;


//  CLASS DEFINITION
/**
* Class to carry out WLAN SSID discovery
*
* @since S60 5.0
* @lib wsfapwizard.lib
*/
NONSHARABLE_CLASS( CWsfWlanSsidDiscovery ): public CActive
    {
    /**
    * States of the WLAN discovery
    */
    enum TWsfWlanDiscoveryState
        {
        /**
        * Scanning has been started
        */
        EScanning,
        
        /**
        * Scanning finished
        */
        EDone,
        
        /**
        * Scanning was aborted
        */
        EAborted
        };
        
        
    public:
        /**
        * Factory function.
        * @since S60 5.0
        * @return Class instance
        */
        static CWsfWlanSsidDiscovery* NewL();

        /**
        * Destructor
        * @since S60 5.0
        */
        ~CWsfWlanSsidDiscovery();
        
        
    private:
        /**
        * Constructor
        * @since S60 5.0
        */
        CWsfWlanSsidDiscovery();

        /**
        * Second-phase constructor
        * @since S60 5.0
        */
        void ConstructL();

    
    public:     // new methods
        /**
        * Executes a direct/broadcast scan
        * @since S60 5.0
        * @param aSsid The SSID to look for in case of direct scan
        * @param aBroadcast If set to ETrue, does a broadcast scan, 
        *                   otherwise direct
        * @param aNetworkMode Network mode for the found network
        * @param aSecurityMode Security mode for the found network
        * @param aUsesPsk ETrue if the found network uses PSK in WPA mode
        * @return ETrue if the scan was successful, otherwise EFalse
        */
        TBool ScanForSsidL( TWlanSsid& aSsid, 
                            TBool aBroadcast,
                            CMManager::TWlanNetMode& aNetworkMode,
                            CMManager::TWlanSecMode& aSecurityMode,
                            TBool& aUsesPsk );

        /**
        * Aborts the scanning process
        * @since S60 5.0
        */
        void AbortScanning();


    private:  // from CActive
    
        /**
        * Implements cancellation of an outstanding request.
        * @since S60 5.0
        */
        void DoCancel();

        /**
        * Handles an active object's request completion event.
        * @since S60 5.0
        */
        void RunL();
        
        
    private:    // data
        /**
        * Progress state
        */
        TUint32 iProgState;
        
        /**
        * Nested waitloop
        */
        CActiveSchedulerWait iWait;
        
        /**
        * Wlan management client interface (owned)
        */
        CWlanMgmtClient* iWlanMgmt;
        
        /**
        * Scan info object (owned)
        */
        CWlanScanInfo* iScanInfo;
    };


#endif // C_WSFWLANSSIDDISCOVERY_H
