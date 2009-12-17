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
* Description:  Class header for MWsfMainViewControllerPartner
*
*/

#ifndef M_WSFMAINVIEWCONTROLLERPARTNER_H
#define M_WSFMAINVIEWCONTROLLERPARTNER_H

/**
 *  An interface to the Main view controller for the Main view container.
 *  @since S60 v5.0
 */
class MWsfMainViewControllerPartner
    {
    public:
    
        /**
         * Handles up key presses
         * @since S60 5.0
         */
        virtual void HandleUpKeyPressedL() = 0;
    
        /**
         * Handles down key presses
         * @since S60 5.0
         */
        virtual void HandleDownKeyPressedL() = 0;
    
        /**
         * Handles selection key presses
         * @since S60 5.0
         */
        virtual void HandleSelectionKeyPressedL() = 0;
    };

#endif // M_WSFMAINVIEWCONTROLLERPARTNER_H

