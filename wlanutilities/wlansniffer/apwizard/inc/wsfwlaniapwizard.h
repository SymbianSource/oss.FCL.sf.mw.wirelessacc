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



#ifndef C_WSFWLANIAPWIZARD_H
#define C_WSFWLANIAPWIZARD_H


//  EXTERNAL INCLUDES
#include <e32base.h>
#include <AknWaitDialog.h>

//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"


//  FORWARD DECLARATIONS
class CWsfWlanIapCreator;
class CWsfWlanSsidDiscovery;


//  CLASS DEFINITION
/**
* Class to control the user interaction during access point creation
*
* @lib wsfapwizard.lib
* @since S60 5.0
*/  
NONSHARABLE_CLASS( CWsfWlanIapWizard ): public CBase,
                                        public MProgressDialogCallback
    {
    public:  
        /**
        * Factory function.
        * @since S60 5.0
        * @return Class instance
        */
        IMPORT_C static CWsfWlanIapWizard* NewL();

        /**
        * Factory function.
        * @since S60 5.0
        * @return Class instance
        */
        IMPORT_C static CWsfWlanIapWizard* NewLC();

        /**
        * Destructor
        * @since S60 5.0
        */
        IMPORT_C ~CWsfWlanIapWizard();


    private:
        /**
        * Constructor
        * @since S60 5.0
        */
        CWsfWlanIapWizard();    

        /**
        * Second-phase constructor
        * @since S60 5.0
        */
        void ConstructL();


    private:    // from MProgressDialogCallback
        
        /**
        * Callback method. Gets called when a dialog is dismissed. 
        * @since S60 5.0
        * @param aButtonId The button which caused the dismiss
        */        
        void DialogDismissedL( TInt aButtonId ); 


    public:     // new methods
        /** 
        * Starts the IAP creation wizard using the given wlaninfo item
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo with the already known network details
        * @param aDefineAccessPoint If ETrue, 'Define access point' was 
        *                   selected from 'Options' menu
        * @return ETrue on success, otherwise EFalse 
        */
        IMPORT_C TBool LaunchWizardL( TWsfWlanInfo& aWlanInfo, 
                                      TBool aDefineAccessPoint );

        /**
        * Creates the accesspoint using the entered data
        * @since S60 5.0
        * @return ETrue on success, EFalse if IAP creation was cancelled
        */
        IMPORT_C TBool CreateAccessPointL();    
        

    private:
        /**
        * Makes necessary questions when creating the IAP
        * @since S60 5.0
        */
        void AskNetworkDetailsL();
        
        /**
        * Prompts for the WLAN SSID in case of hidden/unlisted networks
        * @since S60 5.0
        * @return Buffer containing the entered SSID
        */        
        HBufC* AskSsidL();

        /**
        * General purpose text query
        * @since S60 5.0
        * @param aQueryStringResource Resource id of the caption string
        * @param aVariable The buffer to put the entered data into
        * @param aMaxLength Max length of the buffer
        * @return Id of the button which dismissed the query
        */
        TInt QueryTextL( TInt aQueryStringResource, 
                         TDes& aVariable, 
                         const TInt& aMaxLength );

        /**
        * Queries the secure key 
        * @since S60 5.0
        * @return ETrue if query was accepted, EFalse on cancel
        */
        TBool QuerySecureKeyL();
        
        /**
        * Cleanup function for the searching for wlan waitnote
        * @since S60 5.0
        * @param aPtr Pointer to this class
        */
        static void DismissWaitNote( TAny* aPtr );

        
        /**
        * Initializes the resource file in the given UI context
        * @since S60 5.0
        * @param aEnv The current UI environment
        * @return Resource file offset
        */
        static TInt InitResourcesL( CEikonEnv& aEnv );

        /**
        * Frees the given resource in the current UI context
        * @since S60 5.0
        * @param aEnv The current UI environment
        * @param aResOffset The resource to release
        */
        static void ReleaseResources( CEikonEnv& aEnv, TInt aResOffset );
        
        
    private:    // data
        /**
        * Reference to the Eikon environment (not owned)
        */        
        CEikonEnv& iEnv;
        
        /**
        * Password for the current network (owned)
        */        
        HBufC* iUsedPassword;
        
        /**
        * In case of WEP networks it indicates whether the password is given
        * in hexadecimal format
        */
        TBool iIsHexWep;
        
        /**
        * IAP creator object (owned)
        */
        CWsfWlanIapCreator* iCreator;

        /**
        * Resource file offset
        */
        TInt iResourceFileOffset;
        
        /**
        * Pointer to the wlaninfo of the current network (not owned)
        */        
        TWsfWlanInfo* iWlanInfo;
        
        /**
        * If ETrue, 'Define access point' was selected
        */
        TBool iDefineNewAccessPoint;

        /**
        * "Searching WLAN networks" wait dialog (owned)
        */
        CAknWaitDialog* iWaitDialog;
        
        /**
        * WLAN discovery object (owned)
        */
        CWsfWlanSsidDiscovery* iSsidDiscovery;
       
    };


#endif // C_WSFWLANIAPWIZARD_H
