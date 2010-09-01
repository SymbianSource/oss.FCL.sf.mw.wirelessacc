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
* Description:  Header for common client-server constants and enums
*
*/



#ifndef WSFCOMMON_H
#define WSFCOMMON_H

/**
* Possible values for the persistence property of monitored IAPs 
*/
enum TWsfIapPersistence
    {
    /**
    * Default persistence value
    */
    EIapDeleted,
    
    /**
    * The IAP won't be deleted at all.
    */
    EIapPersistent,
    
    /**
    * There is an owned active connection whose IAP will be invalidated 
    * when a disconnect event occurs.
    */
    EIapExpireOnDisconnect,
    
    /**
    * There is an owned active connection being used by the browser. 
    * The IAP will be invalidated when a disconnect event occurs.
    */
    EIapNestedExpireOnDisconnect,

    /**
    * The IAP is invalidated only if the server is forced to terminate.
    */
    EIapExpireOnShutdown,

    /**
    * The IAP is invalidated when the browser exits or a disconnect occurs.
    */
    EIapExpireOnBrowserExit,
    
    /**
    * Transient state to mark the IAP as immediately invalidated
    */
    EIapForcedExpiry

    };


/**
* Possible values for the auto-disconnect timer control commands.
* Start/stop and Reset can be combined with usual bitfield operations.
*/
enum TWsfAutoDisconnectCommand
    {
    /**
    * Stops the timer but does not change its current value
    */
    EAdcStopTimer           =   0x0000,
    
    /**
    * Starts the timer from its previous value
    */
    EAdcStartTimer          =   0x0001,
    
    /**
    * Resets the value
    */
    EAdcTimerReset          =   0x0002
    };



#endif // WSFCOMMON_H
