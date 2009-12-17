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
* Description:  Class header for MWsfDetailsViewControllerPartner
*
*/

#ifndef M_WSFDETAILSVIEWCONTROLLERPARTNER_H
#define M_WSFDETAILSVIEWCONTROLLERPARTNER_H

/**
 * An interface to the main controller for the Details view controller.
 *
 * @since S60 v5.0
 */
class MWsfDetailsViewControllerPartner
    {
    public:
    
        /**
         * Handles left key presses
         * @since S60 5.0
         */
        virtual void HandleLeftKeyPressedL() = 0;
        
        /**
         * Handles right key presses
         * @since S60 5.0
         */
        virtual void HandleRightKeyPressedL() = 0;
    
        /**
         * Handles selection key presses
         * @since S60 5.0
         */
        virtual void HandleSelectionKeyPressedL() = 0;

    };

#endif // M_WSFDETAILSVIEWCONTROLLERPARTNER_H

