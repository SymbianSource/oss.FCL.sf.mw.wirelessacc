/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*        Header file for CWlanPluginDbObserver class.
*
*/


#ifndef C_WLAN_PLUGIN_DB_OBSERVER
#define C_WLAN_PLUGIN_DB_OBSERVER

// System includes
#include <e32base.h>
#include <wlancontainer.h>
#include <d32dbms.h> 
#include <WlanCdbCols.h>
#include <commsdat.h>
#include <wlanmgmtclient.h>

//forward declaration
class TWsfAiController;

/**
* Database observer class.
*/
NONSHARABLE_CLASS( CWsfDbObserver ) : public CActive
    {
    public:
        /**
        * The Constructor.
        * @param aState Pointer to the WLAN State object.
        */
    	CWsfDbObserver(  );
        
        /**
        * First phase of two phased construction.
        * @param aState Pointer to the WLAN State object.
        */
        static CWsfDbObserver* NewL(  );
        
        /**
        * This function activates this object as an active object.
        */
        void ActivateItL();
        
        /**
        * Destructor.
        */
        virtual ~CWsfDbObserver();
        
        /**
         * Set the controller
         */
        void SetController( TWsfAiController* aController );
        	
        /**
         * Set WLAN availablity to scanning
         */
        void EnableScanL();
        
        /*
         * Set WLAN availablity to not scanning
         */
        void DisableScanL();
          
    protected:
        /**
        * When the WLAN device settings table is accessed this function
        * is called. Handle of the active object.
        */
        void RunL();
        
        /**
        * Whan waiting for a database access is stopped this function
        * is called.
        */
        void DoCancel();
 
    private:
        /**
        * Second phase of two phase construction.
        */
        void ConstructL();
        
        /*
         * Get default scanning interval 
         */
        TInt DefaultScanIntervalL();

    private:  
        /**
        * @var Pointer to the WLAN state object to ask and set scanning
        * state.
        */ 
        //CWlanPluginWlanState * iWlanState;
        
        /**
        * A CommsDat session fod database access.
        */
        CMDBSession* iSession;
        
        /**
        * A class representing a recor of WLAN Devive Settings table.
        */
        CCDWlanDeviceSettingsRecord* iRecord;
        
        /**
        * The value of it shows the scan state of the WLAN availablity
        */
        TBool iOuterScanState;
        
        /**
         * Reference to the controller class of the module.
         */
        TWsfAiController* iController;
        
        /**
        * Handle to the WLAN management client (owned)
        */
        CWlanMgmtClient* iWlanMgmtClient;
    
    };
    
    
#endif //C_WLAN_PLUGIN_DB_OBSERVER
