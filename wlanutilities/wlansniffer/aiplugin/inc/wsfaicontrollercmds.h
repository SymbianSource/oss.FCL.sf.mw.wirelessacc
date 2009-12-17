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
* Description:  TWsfAiController's commands
*
*/

#ifndef WSFAICONTROLLERCMDS_H
#define WSFAICONTROLLERCMDS_H

/**
 * Scanning on, not connected
 */
enum TWsfAiOffline
    {
    EAiOfflineStartBrowsing,
    EAiOfflineConnect,
    EAiOfflineSearchWlans,
    EAiOfflineDisableScan
    };


/**
 * Scanning on, connected but browser not active or not using WLAN
 */
enum TWsfAiConnected
    {
    EAiConnectedStartBrowsing,
    EAiConnectedDisconnect
    };


/**
 * Scanning on, connecting
 */
enum TWsfAiConnecting
    {
    EAiConnectingDisconnect
    };


/**
 * Scanning on, connected and browser is using the connected WLAN
 */
enum TWsfAiBrowsing
    {
    EAiBrowsingContinueBrowsing,
    EAiBrowsingDisconnect
    };



#endif // WSFAICONTROLLERCMDS_H


// End of file
