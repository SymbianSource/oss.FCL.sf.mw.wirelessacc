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
* Description:  Class header for MWsfMainUiObserver
*
*/

#ifndef M_WSFMAINUIOBSERVER_H
#define M_WSFMAINUIOBSERVER_H

//  EXTERNAL INCLUDES
#include <e32def.h>


//  CLASS DEFINITION
/**
 *  An interface to the main controller for the AppUI.
 * 
 *  @since S60 v5.0
 */
class MWsfMainUiObserver
    {
    public:     // Abstract methods

        /**
         * Called when the main view has been created.
         * @since S60 5.0
         */
        virtual void MainViewCreatedL() = 0;
        
        /**
         * Brings the main wlan list to the foreground
         * @since S60 5.0
         */
        virtual void ShowWlanListL() = 0;
        
        /**
         * Issues a refresh on the wlan list
         * @since S60 5.0
         */
        virtual void RefreshL() = 0;
        
        /**
         * Invokes the details view
         * @since S60 5.0
         */
        virtual void ShowDetailsL() = 0;        
        
        /**
         * Starts the browser with the currently selected wlan
         * @since S60 5.0
         */
        virtual void StartBrowsingL() = 0;
        
        /**
         * Continues browsing with the currently selected wlan
         * @since S60 5.0
         */
        virtual void ContinueBrowsingL() = 0;

        /**
         * Connects to the currently selected wlan
         * @since S60 5.0
         */
        virtual void ConnectL() = 0;
        
        /**
         * Disconnects the currently selected wlan
         * @since S60 5.0
         */
        virtual void DisconnectL() = 0;

        /**
         * Called when the application has completely started
         * @since S60 5.0
         */
        virtual void AppLaunchCompleteL() = 0;

        /**
         * Called when the application is brought to the foreground
         * @since S60 5.0
         */
        virtual void AppSwitchedForegroundL() = 0;

        /**
         * Called when the application is sent to the background
         * @since S60 5.0
         */
        virtual void AppSwitchedBackgroundL() = 0;
        
        /**
         * Invokes the wlan list filtering dialog
         * @since S60 5.0
         */
        virtual void EditBlackListItemsL() = 0;
        
        /**
         * Queries whether the engine is refreshing
         * @since S60 5.0
         * @return ETrue if the engine is refreshing, EFalse otherwise
         */
        virtual TBool EngineRefreshing() = 0;

    };

#endif      //  M_WSFMAINUIOBSERVER_H

// End of file
