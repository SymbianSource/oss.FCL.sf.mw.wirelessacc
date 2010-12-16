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
* Description:  Class header for MWsfWlanMonitorObserver
*
*/



#ifndef M_WSFWLANMONITOROBSERVER_H
#define M_WSFWLANMONITOROBSERVER_H


//  EXTERNAL INCLUDES
#include <e32def.h>


//  CLASS DEFINITION
/**
 * Abstact interface definition class for wlan connection detection
 * @since S60 5.0
 */
class MWsfWlanMonitorObserver
    {
    public:     // Abstract methods
    
        /**
        * Called when a wlan connection is established
        * @since S60 5.0
        * @param aIapId Access point id
        */
        virtual void ConnectionEstablishedL( TInt32 aIapId ) = 0;

        /**
        * Called when wlan connection has been lost
        * @since S60 5.0
        * @param aIapId Access point id
        */
        virtual void ConnectionLostL( TInt32 aIapId ) = 0;
        
        /**
        * Called when the connection process failed for some reason
        * @since S60 5.0
        * @param aIapId Access point id
        * @param aError System wide error code
        */
        virtual void ConnectingFailedL( TInt32 aIapId, TInt aError ) = 0;
        
        /**
        * Called when the connection no longer needs the IAP it was using
        * @since S60 5.0
        * @param aIapId Access point id
        */
        virtual void ConnectedIapReleasedL( TInt32 aIapId ) = 0;

    };


#endif // M_WSFWLANMONITOROBSERVER_H

