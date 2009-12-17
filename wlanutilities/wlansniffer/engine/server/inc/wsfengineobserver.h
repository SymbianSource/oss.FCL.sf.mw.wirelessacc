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
* Description:  Class header for MWsfEngineObserver
*
*/



#ifndef M_WSFENGINEOBSERVER_H
#define M_WSFENGINEOBSERVER_H

//  EXTERNAL INCLUDES
#include <e32def.h>


//  CLASS DEFINITION
/*
* Class acts as an interface for the CWsfEngine to be able to call back 
* its clients in case of different events.
*
* @since S60 5.0
*/
class MWsfEngineObserver
    {
    public:     // Abstract methods
    
        /**
        * Called when the WLAN scanning is finished and results are available
        * @since S60 5.0
        */
        virtual void WlanListChangedL() = 0;

        /**
        * Called when an error occured in the engine
        * @since S60 5.0
        * @param aError System wide error code
        */
        virtual void NotifyErrorL( TInt aError ) = 0;

        /**
        * Called when the WLAN scanning has been enabled
        * @since S60 5.0
        */
        virtual void ScanEnabledL() = 0;
        
        /**
        * Called when the WLAN scanning has been disabled
        * @since S60 5.0
        */
        virtual void ScanDisabledL() = 0;
        
        /**
        * Called when a WLAN connection has been established
        * @since S60 5.0
        */
        virtual void ConnectedL() = 0;
        
        /**
        * Called when the active WLAN connection has been terminated
        * @since S60 5.0
        */
        virtual void DisconnectedL() = 0;
        
        /**
        * Called when an error occured during the connecting process
        * @since S60 5.0
        * @param aError System wide error code
        */
        virtual void ConnectingFailedL( TInt aError ) = 0;

    };

#endif      //  M_WSFENGINEOBSERVER_H

// End of file
