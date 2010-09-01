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
* Description: 
*        Header file for CWlanPluginTimeShifter class.
*
*/


#ifndef C_WLAN_PLUGIN_TIME_SHIFTER
#define C_WLAN_PLUGIN_TIME_SHIFTER

#include <e32base.h>


class CWlanPluginWlanState;


NONSHARABLE_CLASS( CWlanPluginTimeShifter ) : public CActive
    {
    
    public:
        /*
        * First phase of two phase construction
        */
        static CWlanPluginTimeShifter* NewLC(
                                  CWlanPluginWlanState* aWlanState );
        
        /*
        * First phase of two phase construction
        */
        static CWlanPluginTimeShifter* NewL( 
                                  CWlanPluginWlanState* aState );
        
        /*
        * Destructor
        */
        virtual ~CWlanPluginTimeShifter();
        
        /*
        *
        */
        void ActivateIt();
    
    
    protected:
    
        /**
        * 
        * 
        */
        void RunL();
        
        /**
        * 
        * 
        */
        void DoCancel();
        
        /**
        * Constructor
        */
        CWlanPluginTimeShifter( CWlanPluginWlanState* aWlanState );
        
        
    private:
                
        /*
        * Second phase of 2 phase construction
        */
        void ConstructL();
    
    private:
        
        /**
        * @var Pointer to the WLAN state object to ask and set scanning
        * state.
        */ 
        CWlanPluginWlanState * iWlanState;
    
       
    
    
    };

#endif  //C_WLAN_PLUGIN_TIME_SHIFTER