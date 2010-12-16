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




#ifndef WSFSERVERCOMMON_H
#define WSFSERVERCOMMON_H


#include <e32base.h>

/**
* Published name of the sniffer server
*/
_LIT( KWlanSnifferServerName, "WlanSnifferServer" );

/**
* Filename of the server binary
*/
_LIT( KWlanSnifferServerExe, "wsfserver.exe" );

/**
* Version numbers for the sniffer server
*/
const TUint KWlanSnifferServerMajor = 1;
const TUint KWlanSnifferServerMinor = 0;
const TUint KWlanSnifferServerBuild = 0;


/**
* IPC commands recognised by the sniffer server
*/
enum TWsfWlanSnifferCmds
    {
    /**
    * Asks for the size of the scan results in bytes
    */
    ESnifferCmdWlanDataSize,

    /**
    * Asks to be notified of server events
    */
    ESnifferCmdNotifyEvents,

    /**
    * Asks to cancel notifications
    */
    ESnifferCmdCancelNotifyEvents,

    /**
    * Asks for the wlan scan results
    */
    ESnifferCmdGetWlanInfo,

    /**
    * Asks to create a connection
    */
    ESnifferCmdConnect,

    /**
    * Asks to disconnect the current connection
    */
    ESnifferCmdDisconnect,

    /**
    * Asks to start scanning for WLAN networks
    */
    ESnifferCmdActivateScan,

    /**
    * Asks to disable WLAN scanning
    */
    ESnifferCmdDisableScan,

    /**
    * Asks for the name of the active WLAN connection
    */
    ESnifferCmdConnectionName,

    /**
    * Asks to cancel every operation
    */
    ESnifferCmdCancelAll,

    /**
    * Asks explicitly for an immediate scan
    */
    ESnifferCmdRequestScan,

    /**
    * Asks to monitor the given IAP
    */
    ESnifferCmdMonitorAp,

    /**
    * Queries the scanning status
    */
    ESnifferCmdIsScanEnabled,

    /**
    * Queries the connection status
    */
    ESnifferCmdIsConnected,

    /**
    * Queries the details of the active WLAN connection
    */
    ESnifferCmdConnectedDetails,

    /**
    * Aborts the ongoing scanning
    */
    ESnifferCmdAbortScanning,

    /**
    * Aborts the ongoing connection process
    */
    ESnifferCmdAbortConnecting,

    /**
    * Sets the persistence property of the currently monitored IAP
    */
    ESnifferCmdSetIapPersistence,

    /**
    * Sets the behaviour of the auto-disconnect timer
    */
    ESnifferCmdControlDisconnectTimer
    };
    


/**
* Structure for passing back the active connection name
*/    
class TWsfQueryConnectedAccount
    {
    public:
        /**
         * Maximal length of IAP name that the class should be able to store
         */    
        static const TInt KWlanNameMaxLength = 50;
        
    public:
        /**
        * Indicates whether there was a connection
        */
        TBool iConnected;
        
        /**
        * Name of the active WLAN connection
        */
        TBuf<KWlanNameMaxLength> iConnectedAccountName;
    
    };    


/**
* Events in the server that clients are notified of
*/
enum TWsfEngineEvent
    {
    /**
    * There is a new scan result available.
    */
    EEngineWlanDataChanged,

    /**
    * Notifies that scanning has been disabled
    */
    EEngineScanDisabled,

    /**
    * Notifies that scanning has been enabled
    */
    EEngineScanEnabled,

    /**
    * Notifies that a WLAN connection has been established
    */
    EEngineConnected,

    /**
    * Notifies that the active connection was disconnected
    */
    EEngineDisconnected,

    /**
    * Notifies that an error occured in the server
    */
    EEngineError,

    /**
    * Notifies that the connecting process failed for some reason
    */
    EEngineConnectingFailed
    };


/**
* Data structure for client notification
*/
class TWsfNotifyEventContainer
    {
    public:
        /**
        * Server event
        */
         TWsfEngineEvent iEvent;
         
         /**
         * Error code
         */
         TInt iError;    
         
         /**
          * IAP ID assiciated with the event if any
          */
         TInt32 iIapId;
    };
    


    
#endif 
