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
* Description:  Class header for MWsfMainViewPartner
 *
*/

#ifndef M_WSFMAINVIEWPARTNER_H
#define M_WSFMAINVIEWPARTNER_H

/**
 * An interface to the Main view controller for the Main view container
 * @since S60 5.0
 */
class MWsfMainViewPartner
    {
    public:

        /**
         * Mainview request to show manubar
         * @since S60 5.0
         */
        virtual void ShowMenuBarL() = 0;
    };

#endif // M_WSFMAINVIEWPARTNER_H
