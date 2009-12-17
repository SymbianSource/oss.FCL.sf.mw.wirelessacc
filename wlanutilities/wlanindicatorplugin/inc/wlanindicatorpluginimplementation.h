/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Defines the CWlanIndicatorPluginImplementation class
*
*/



#ifndef CWLANINDICATORPLUGINIMPLEMENTATION_H
#define CWLANINDICATORPLUGINIMPLEMENTATION_H

//  INCLUDES
#include <AknIndicatorPlugin.h> // CAknIndicatorPlugin
#include <wlanmgmtcommon.h>
#include <wlanmgmtclient.h>
#include <rconnmon.h>

//  CONSTANTS
const TInt KWlanMaxAccessPointNameLength = 50;

//  FORWARD DECLARATIONS
class CCoeEnv;

//  CLASS DECLARATIONS
struct TWliWlanInfo
    {
    // Network name (unknown: SSID, known: IAP name), TBuf8
    TWlanSsid iSsid;

    //Internet Access Point Id. 0 if the IAP has not been defined. 
    TUint32 iIapId;

    // The name of the WLAN IAP, TBuf16
    TBuf<KWlanMaxAccessPointNameLength> iNetworkName;
};


/**
*  WLAN Indicator Plug-in implementation
*
*  @since 5.0
*/
class CWlanIndicatorPluginImplementation : public CAknIndicatorPlugin
                                         
    {
        
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CWlanIndicatorPluginImplementation* NewL();

        /**
        * Destructor.
        */
        ~CWlanIndicatorPluginImplementation();
        

    private: // From CAknIndicatorPlugin
        
        /**
         * @see CAknIndicatorPlugin
         */
        void HandleIndicatorTapL( const TInt aUid );

        /**
         * @see CAknIndicatorPlugin
         */
        HBufC* TextL( const TInt aUid, TInt& aTextType );        
    
    
    private:

        /**
        * C++ default constructor.
        */
        CWlanIndicatorPluginImplementation();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        

    private: // new functions

        /**
        * Creates text for the cases
        * - "WLAN network found"
        * - "WLAN networks found"
        * - "'myNetwork' found"
        * @return Descriptor for the current "wlan available" case
        */
        HBufC* CreateWlanNetworksFoundTextL();
        
        /**
        * Creates text for the "connected" case (open or secure).
        * @return Descriptor containing the "'myNetwork' connected" text
        */
        HBufC* CreateWlanConnectedTextL();
        
        /**
         * Finds the required info for the "networks available" case:
         * - name of the known network (null if all unknown)
         * - number of available networks
         * @param aAvailableCount Number of available networks
         */
        HBufC* AvailableNetworksInfoL( TInt& aAvailableCount );
         
        /**
         * Finds the network name based on the IAP id
         * @param Struct for storing the IAP information
         */
        void GetWlanInfoFromIapL( TWliWlanInfo& aWlanInfo );
        
        /**
         * Finds the WLAN connection name.
         * @return Descriptor containing the name of the connected WLAN nw.
         */
        HBufC* ConnectionNameL();
        
        /** Finds the WLAN connection id
         * @return WLAN connection id
         */
        TUint FindWlanBearerConnectedL(); 
        
        /** Checks if the given connection is WLAN
         * @param Id of the active connection
         */
        TBool CheckIfWlan( TUint aConnectionId ); 
        
        /**
         * Launches WlanSniffer application.
         */
        void LaunchWlanSnifferL();        
        

    private: // data
    
        /**
         * Ref
         */
        CCoeEnv* iCoeEnv;
        
        /**
         * Resource file offset in CoeEnv's resource file list
         */
        TInt iResource;
        
        /**
         * Owned. Used for getting connection info
         */
        RConnectionMonitor iConnMonitor;
        
        /**
         * Owned. Used for getting WLAN available networks info
         */
        CWlanMgmtClient* iWlanMgmtClient;
        

    };

#endif      // CWLANINDICATORPLUGINIMPLEMENTATION_H

// End of File
