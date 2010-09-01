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
* Description:  Class header for CWsfIct
*
*/



#ifndef C_WSFICT_H
#define C_WSFICT_H


//  EXTERNAL INCLUDES
#include <e32base.h>
#include <ictsclientinterface.h>


//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * Class to monitor and control ICT interface
 *
 * @lib wsfserver.exe
 * @since S60 5.2
 */
NONSHARABLE_CLASS( CWsfIct ): public CBase,
                              public MIctsObserver
    {
    public:     // Constructors and destructor

        /**
        * Factory function.
        * @since S60 5.2
        * @return CWsfIct instance
        */
        IMPORT_C static CWsfIct* NewL();

        /**
        * Factory function.
        * @since S60 5.2
        * @return CWsfIct instance
        */
        IMPORT_C static CWsfIct* NewLC();

        IMPORT_C ~CWsfIct();

    private:    // Constructors
        CWsfIct();

        /**
        * Factory function.
        * @since S60 5.2
        */
        void ConstructL();

    public:     // New methods
        
        /**
        * Tests the connected WLAN access point
        * @since S60 5.2
        * @param aIapId Id of the connection method
        */
        void TestConnectedAccessPointL( TUint aIapId );
        
        /**
        * Initializes ICT parameters
        * @since S60 5.2
        * @param ETrue if ICT needs to be executed
        * @param aIapId Id of the connection method
        * @param aConnectOnly ETrue if connect only case, 
        * EFalse if browser is launched after successful connection.
        */
        void InitializeIctL( TBool aTestAccessPoint,
                             TUint aIapId,
                             TBool aConnectOnly );

    public: // from MIctsObserver

        /**
        * Called by ICTS during/after connectivity test
        * @since S60 5.2
        * @param aResult Result code of the connectivity test
        * @param aString Parameter to hold auxiliary data
        */
        void ConnectivityObserver( TIctsTestResult aResult, const TDesC& aString );


    private:  // New methods
        
        /**
        * Starts WLAN Login application
        * @param aString Contains redirect URL
        */  
        void LaunchWlanLoginL( const TDesC& aString );
        
        /**
        * Moves the IAP to internet SNAP
        * @param aIapId Id of the connection method
        */  
        void MoveToInternetSnapL( const TUint32 aIapId );


    private:    // Data

        /**
        * Id of currently connected IAP (if applicable)
        */
        TUint iIapId;
        
        /**
        * Network id of currently connected IAP (if applicable)
        */
        TUint iNetId;
        
        /**
        * Indicates if "Connect" or "Start Web browsing" is selected.
        */
        TBool iConnectOnly;
        
        /**
        * ICT class. Owned.
        */
        CIctsClientInterface* iIct;

    };

#endif      //  C_WSFICT_H

// End of file
