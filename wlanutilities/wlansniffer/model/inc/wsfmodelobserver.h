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
* Description:  Class header for MWsfModelObserver
*
*/




#ifndef M_WSFMODELOBSERVER_H
#define M_WSFMODELOBSERVER_H

//  EXTERNAL INCLUDES
#include <e32def.h>
#include <e32cmn.h>

#include "wsfbrowserlaunchobserver.h"

 

//  CLASS DEFINITION
/**
* Class acts as an interface for the model to be able to call back its clients
* in case of different model events.
*
* @since S60 5.0
*/
class MWsfModelObserver: public MWsfBrowserLaunchObserver
    {
    public:     // Abstract methods
        /**
         * Notification that a connection is about to be created
         * @since S60 5.0
         * @param aIapId The IAP id of the connection being created
         */
        virtual void ConnectingL( TUint32 aIapId ) = 0;
        
        /**
         * Notification that the connection creation process ended
         * @since S60 5.0
         * @param aResult The result of the connection creation
         */
        virtual void ConnectingFinishedL( TInt aResult ) = 0;
        
        /**
         * Propagates the network data from the model
         * @since Symbian^3
         */
        virtual void WlanListDataReadyL() {};
    
        /**
         * Propagates the network data from the model
         * @since Symbian^3
         */
        virtual void StartupRefreshDataReadyL() {};
        
        /**
         * Propagates the network data from the model
         * @since Symbian^3
         */
        virtual void HandleMskIfBrowsingL() = 0;
        
        /**
         * Propagates the network data from the model
         * @since Symbian^3
         */
        virtual void HandleMskIfConnectedL() = 0;
    };


#endif      //  M_WSFMODELOBSERVER_H

// End of file

