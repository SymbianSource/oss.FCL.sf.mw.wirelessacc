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
* Description:  Class header for MWsfBrowserLaunchAdapter
*
*/





#ifndef M_WSFBROWSERLAUNCHADAPTER_H
#define M_WSFBROWSERLAUNCHADAPTER_H


//  CLASS DEFINITION
/**
 * Interface class to manage browser launching
 *
 * @since S60 5.0
 */
class MWsfBrowserLaunchAdapter
    {

    public:
        /**
        * Implementing class should launch the browser with the give IAP id
        * @since S60 5.0
        * @param aIapId The IAP id to connect with
        */
        virtual void LaunchBrowserL( TUint32 aIapId ) = 0;

 
    };


#endif // M_WSFBROWSERLAUNCHADAPTER_H
