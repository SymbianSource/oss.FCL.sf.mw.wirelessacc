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
* Description:   Class header for CWsfWlanIapCreator
*
*/




#ifndef C_WSFWLANIAPCREATOR_H
#define C_WSFWLANIAPCREATOR_H


//  EXTERNAL INCLUDES
#include <e32base.h>
#include <cmmanagerext.h>
#include <cmpluginwlandef.h>


//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"



//  CLASS DEFINITION
/**
* Class collects various IAP creation related functions.
*
* @lib wsfapwizard.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CWsfWlanIapCreator ): public CBase
    {
    public:
        /**
        * Factory function.
        * @since S60 5.0
        * @return Class instance
        */
        static CWsfWlanIapCreator* NewL();

        /**
        * Factory function.
        * @since S60 5.0
        * @return Class instance
        */
        static CWsfWlanIapCreator* NewLC();

        /**
        * Destructor
        * @since S60 5.0
        */
        ~CWsfWlanIapCreator();


    private:    // constructors
        /**
        * Constructor
        * @since S60 5.0
        */
        CWsfWlanIapCreator();    
        
        /**
        * Second-phase constructor
        * @since S60 5.0
        */
        void ConstructL();


    public:     // new methods
        /**
        * Sets the default destination for subsequent access point 
        * creation calls
        * @since S60 5.0
        * @param aDestinationId The id of the new default destination
        */
        void SetDefaultDestination( const TUint32 aDestinationId );

        /**
        * Creates an access point to wlan.
        * @since S60 5.0
        * @param aWlanInfo Contains all available data from wlan scanner. 
        *                  Note: the network name field may change if 
        *                  IAP already exists, e.g. MyWlan -> MyWlan(01). 
        *                  Also the IAP id is changed on successful creation
        * @param aPresharedKey The used key to access network
        * @param aIsHex ETrue if the key is given in hexadecimal string format.
        *               Applicable only in case of WEP authentication.
        */    
        void CreateAccessPointL( TWsfWlanInfo& aWlanInfo, 
                                 const TDesC& aPresharedKey, 
                                 const TBool aIsHex );    


    private:    // new methods
        /**
        * Creates connection method using the given WLAN parameters
        * @since S60 5.0
        * @param aWlanInfo WLAN parameters.
        *                  Note: the network name field may change if 
        *                  IAP already exists, e.g. MyWlan -> MyWlan(01)
        * @return The id of the created IAP
        */
        TUint32 CreateConnectionMethodL( TWsfWlanInfo& aWlanInfo );
        
        /**
        * Stores WPA data
        * @since S60 5.0
        * @param aIapId The IAP for which WPA data are to be stored
        * @param aPresharedKey WPA key string. MUST be correct as the function
        *                  DOES NOT verify its correctness!
        * @param aSecMode Security mode (to tell apart WPA-PSK/EAP)
        */    
        void StoreWPADataL( const TInt aIapId, 
                            const TDesC& aPresharedKey, 
                            const CMManager::TWlanSecMode aSecMode );
        
        /**
        * Stores WEP key data
        * @param S60 5.0
        * @param aIapId The IAP for which WEP data are to be stored
        * @param aWepKey WEP key string. MUST be correct as the function 
        *                  DOES NOT verify its correctness!
        * @param aIsHexWep To indicate whether the passed key is in hex format
        */
        void StoreWEPDataL( const TInt aIapId, 
                            const TDesC& aWepKey, 
                            const TBool aIsHexWep );

        /**
        * Converts an ascii sequence to hexadecimal character string
        * @since S60 5.0
        * @param aSource The ASCII string
        * @param aDest The destination for the hex string
        */
        void ConvertAsciiToHex( const TDesC8& aSource, TDes8& aDest );

        /**
        * Cleanup function for commsdat session
        * @since S60 5.0
        * @param aDbSession The commsdat session to clean up
        */
        static void RollbackCommsDat( TAny* aDbSession );
        

    private:    // data
        /**
        * Commsdat session object (owned)
        */
        CommsDat::CMDBSession *iDbSession;
        
        /**
        * Handle to the CmManager
        */
        RCmManagerExt iCmManagerExt;
        
        /**
        * Default destination id
        */
        TUint32 iDestinationId;
    };

#endif // C_WSFWLANIAPCREATOR_H
