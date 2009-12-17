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
* Description:  Class header for MWsfActiveTimerHandler
*
*/

#ifndef M_WSFACTIVETIMERHANDLER_H
#define M_WSFACTIVETIMERHANDLER_H

/**
 *  Interface between details view controller and timer for active time update
 */
class MWsfActiveTimerHandler
    {

    public:

        /**
         * Updates the details view active time
         */
        virtual void UpdateActiveTimeL() = 0;
    };

#endif // M_WSFACTIVETIMERHANDLER_H

