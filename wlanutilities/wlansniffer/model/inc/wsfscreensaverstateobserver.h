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
* Description:  Class header for MWsfScreenSaverStateObserver
*
*/




#ifndef M_WSFSCREENSAVERSTATEOBSERVER_H
#define M_WSFSCREENSAVERSTATEOBSERVER_H


#include <e32std.h>


/**
 * Interface class for notifications about screen saver status change
 *
 * @since S60 5.0
 */
class MWsfScreenSaverStateObserver
    {
    public:
        /**
        * Called when screen saver status has changed
        * @since S60 5.0
        * @param aScreenSaverActive ETrue if the screen saver was activated,
        *                           EFalse on deactivation
        */
        virtual void ScreenSaverStatusChangedL( 
                                          const TBool aScreenSaverActive ) = 0;
    };


#endif // M_WSFSCREENSAVERSTATEOBSERVER_H
