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
* Description:   Class header for MWsfStateChangeObserver
*
*/




#ifndef M_WSFSTATECHANGEOBSERVER_H
#define M_WSFSTATECHANGEOBSERVER_H

//  EXTERNAL INCLUDES
#include <e32std.h>


//  CLASS DEFINITION
/**
 * Mixin class to handle server events
 *
 * MWsfStateChangeObserver provides an interface through which 
 * the implementing class can receive notifications of different server events
 * communicated by the client-side event handler.
 *
 * @since S60 5.0
 */
class MWsfStateChangeObserver
    {
    public:
        /**
        * The current wlan data has changed and it is ready to be retrieved
        * @since S60 5.0
        */
        virtual void WlanListChangedL() = 0;

        /**
        * Engine error occured
        * @since S60 5.0
        * @param aError System wide error code
        */
        virtual void NotifyEngineError( TInt aError ) = 0;

        /**
        * Scanning has been disabled
        * @since S60 5.0
        */
        virtual void ScanDisabledL() = 0;

        /**
        * Scanning has been enabled
        * @since S60 5.0
        */
        virtual void ScanEnabledL() = 0;
        
        /**
        * A WLAN connection has been established
        * @since S60 5.0
        */
        virtual void WlanConnectionActivatedL() = 0;

        /**
        * A WLAN connection has been closed
        * @since S60 5.0
        */
        virtual void WlanConnectionClosedL() = 0;
        
        /**
        * A WLAN connection creation finished
        * @since S60 5.2
        * @param aError System wide error code
        */
        virtual void ConnectionCreationProcessFinishedL( TInt aError ) = 0;
        
        
    };


#endif // M_WSFSTATECHANGEOBSERVER_H
