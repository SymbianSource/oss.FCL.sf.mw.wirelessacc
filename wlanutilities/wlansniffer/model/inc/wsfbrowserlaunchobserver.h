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
* Description:  Class header for MWsfBrowserLaunchObserver
*
*/




#ifndef M_WSFBROWSERLAUNCHOBSERVER_H
#define M_WSFBROWSERLAUNCHOBSERVER_H



//  CLASS DEFINITION
/**
 * Interface class to get notified about browser launch results
 *
 * @since S60 5.0
 */
class MWsfBrowserLaunchObserver
    {

    public:
        /**
        * Called if the browser launch failed for some reason
        * @since S60 5.0
        * @param aError System-wide error code
        */
        virtual void BrowserLaunchFailed( TInt aError ) = 0;

        /**
        * Called if the launch has been completed successfully
        * @since S60 5.0
        */
        virtual void BrowserLaunchCompleteL() = 0;

        /**
        * Called when the previously launched browser exits
        * @since S60 5.0
        */
        virtual void BrowserExitL() = 0;
        
    };


#endif // M_WSFBROWSERLAUNCHOBSERVER_H
