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
*        Header file for WlanPluginWlanState class.
*
*/


#ifndef __C_WLANPLUGINWLANSTATE_H__
#define __C_WLANPLUGINWLANSTATE_H__

// System includes
#include <e32base.h>
#include <wsfstatechangeobserver.h>
#include <wsfwlaninfo.h>
#include <AknsItemID.h>

//forward declarations
class CWsfModel;
class CWsfWlanInfoArray;
class CCoeEnv;
class CGSParentPlugin;
class CGulIcon;
class CWlanPluginDbObserver;
class CWsfWlanInfoArrayVisitor;
class CWlanPluginTimeShifter;

/**
* WLAN state class.
*/
class CWlanPluginWlanState : public CBase,
                                public MWsfStateChangeObserver
    {
        
    public:
    
        /*
        * Default constructor
        * @param aParent The the parent plugin of it in.
        * the Control Panel
        */
        CWlanPluginWlanState( CGSParentPlugin *aParent );
        
        /*
        * Destructor
        */
        virtual ~CWlanPluginWlanState();
        
        /*
        * First phase of 2 phase constructor.
        * @param aParent pointer to parent plugin.
        * @return An instance.
        */
        static CWlanPluginWlanState* NewLC( CGSParentPlugin *aParent );
        
        /*
        * First phase of 2 phase constructor
        * @param aParent Pointer to parent plugin.
        * @return An instance.
        */
        static CWlanPluginWlanState* NewL( CGSParentPlugin *aParent );
        
        /*
        *  Gives back the wlan state descriptor into aDes.
        *  @param aDes Reference to the destination descriptor.
        */
        void GetStateStringL( TDes& aDes );
        
        /*
        * Gives back the wlan state main icon. The result is
        * ETrue if there is valid id. 
        * @param aPic The id of the icon.
        * @param aPicMask The id of the mask.
        */
        TBool GetMainIconL( TInt& aPic, 
        		            TInt& aPicMask, 
        		            TAknsItemID& aItemID );

        /*
        * Gives back the signal strength. The result is
        * ETrue if there is valid strength, and the state
        * is connected. 
        * @param aPic The id of the icon.
        * @param aPicMask The id of the mask.
        * @return If D column icon is needed.
        */
        TBool GetStrengthIconL( TInt& aPic, 
        		                TInt& aPicMask, 
        		                TAknsItemID& aItemId );
        
        /**
        * Returns if the observed scanning is on
        * @return If the scanning is on.
        */
        TBool IsScanning();
        
        /**
        * Sets the scanning value, and manage effects, caused
        * by it. It is used by the dbobserver class to manage 
        * setting scanning value caused by the "Wlan availability".
        * @param aScanning The set value.
        */
        void SetScanningL( TBool aScanning );
        
        /**
        *
        */
        void DirectlyScanL();
        
        /**
        *
        */
        void DeleteTimeShifter();
        
    public: //from MWlanSnifferStateChangeObserver
        /*
        * The current wlan data has changed
        * and it is ready to be retrieved.
        */
        virtual void WlanListChangedL();

        /*
        * Engine error occured
        * @param aError System wide error to describe what has happened.
        */
        virtual void NotifyEngineError( TInt aError );

        /*
        *  The network scanning has been disabled.
        */
        virtual void ScanDisabledL();

        /*
        * The network scanning has been enabled.
        */
        virtual void ScanEnabledL();

        /*
        * Wlan bearer opened using accesspoint
        */
        virtual void WlanConnectionActivatedL();

        /*
        * wlan bearer has been closed
        */
        virtual void WlanConnectionClosedL();
        
        /**
        * Connection creation process finished
        * @param aError System wide error code
        */
        virtual void ConnectionCreationProcessFinishedL( TInt aError );
        
    protected:
        /*
        * Makes the wlan state descriptor according to
        * the given info array.
        * @param aArray the given info array.
        * @param aDes the result is written here.
        */
        void MakeStateStringL( CWsfWlanInfoArray* aArray, HBufC*& aBuf );


        /*
        * Gives the connected wlan status string.
        * @param aItem Ref to the result descriptor. 
        */
        void FormatConnectedL( HBufC*& aBuf );
        
        /*
        * Gives the offline wlan status string.
        * @param aItem Ref to the result descriptor. 
        */
        void FormatStatusOffL( HBufC*& aBuf );

        /*
        * Decides if there is one interesting wlan or
        * more and writes out wlan state string.
        * @param aArray input array of wlans.
        * @param aItem ref to the result descriptor.
        */
        void CheckWlansL( CWsfWlanInfoArray* aArray, HBufC*& aBuf );

        /*
        * Decides if the wlan given is connected, or known 
        * and writes out the suitable text to aItem according
        * to it.
        * @param aWlan Input wlan info.
        * @param aItem Ref to the result descriptor. 
        */
        void FormatWlanSingleLineL( TWsfWlanInfo& aWlan, HBufC*& aBuf );

        /*
        * Decides if there is one unknown or more
        * networks and writes solution according to it.
        * @param aMultipleUnknownWlans If 
        * there is more network available.
        * @param aItem Ref to the result descriptor.
        */
        void FormatUnknownWlansSingleLineL( 
                    const TBool aMultipleUnknownWlans, 
                    HBufC*& aBuf );

        /*
        * Gives the descripto meaning there are no
        * wlans found. 
        * @param aItem Ref to output descriptor.
        */
        void FormatNoWlansAvailableL( HBufC*& aBuf );

        /*
        * Checks if the wlan state is different from
        * the one iPresentString represent. The
        * result is ETrue if the state changed.
        * @return If the state is changed.
        */
        TBool IsStateChangedL();
        
        /*
        * Gets the present signal strength. It is a normal
        * strength if the wlan state is connected, otherwise it is
        * ENoSignal.
        * @return The signal strength
        */
        TWsfWlanSignalStrengthLevel GetSignalStrength();
        
        /**
        * Updates the parent plugin in Control Panel
        */
        void UpdateParentViewL();
                  
    private:

        /*
        * Second phase of two phase construction
        */
        void ConstructL();
        
        /**
        * Updates the wlan list
        */
        void UpdateWlanListL();
        
    protected:
        
        /*
        * @var pointer to a sniffer model    
        */
        CWsfModel* iEngine;

        /*
        * @var present wlan info list    
        */
        CWsfWlanInfoArray* iInfoArray;
        
        /*
        * @var if wlan is scanning    
        */
        TBool iScanning;
        
        /*
        * @var if there is wlan connection    
        */
        TInt iConnected;
        
        /*
        * @var true if the plugin explicitely started wlan scanning
        */
        TInt iExplicitScanning;
        
        /*
        * @var pointer to coeEnv
        */
        CCoeEnv* iCoeEnv;

        /*
        * @var pointer to parent plugin
        */
        CGSParentPlugin* iParent;

        /*
        * @var the present written out status string
        */
        HBufC* iPresentString;
        
        /*
        * @var the present signal strength
        */ 
        TWsfWlanSignalStrengthLevel iSignalStrength;
        
        /**
        * @var The Database observer class
        */
        CWlanPluginDbObserver* iDbObserver;
                
        /**
        * @var A bool representing if calling an update for parent
        * plugin is enabled. True if the parent do not have be updated. 
        */
        TBool iSupressParentUpdate;
        
        /**
        * @var A bool false, if wlan list update is valid to call
        */
        TBool iSupressUpdateWlan;
        
        /**
        * @var object needed filtering out wlans
        */
        CWsfWlanInfoArrayVisitor* iWlanInfoBranding;
        
        /*
        * @var the second line data reachable is valid
        */
        TBool iValidSecondLine;
        
        /*
        * @var tThe wlan info of the connected network
        */
        TWsfWlanInfo iConnectedWlanInfo;
        
        /*
        * @var Time shifter
        */
        CWlanPluginTimeShifter* iTimeShifter;
        
        /*
        * @var Time shifter supressing
        */
        TBool iSupressTimeShifter;
        
    };

#endif //__C_WLANPLUGINWLANSTATE_H__
