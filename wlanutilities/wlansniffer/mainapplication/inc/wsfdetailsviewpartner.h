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
* Description:  Class header for MWsfDetailsViewPartner
*
*/

#ifndef M_WSFDETAILSVIEWPARTNER_H
#define M_WSFDETAILSVIEWPARTNER_H

//  EXTERNAL INCLUDES
#include <e32def.h>

//  CLASS DEFINITION
/**
 * Details view's controller notifies application level controller 
 * throught this interface
 * @since S60 5.0
 */
class MWsfDetailsViewPartner
    {
    public:     // Abstract methods

        /**
        * Detailsview requests to close view
        * @since S60 5.0
        */        
        virtual void CloseDetailsViewL() = 0;

        /**
        * Refresh scanning
        */
        virtual void DetailsScanL() = 0;
    };

#endif      //  M_WSFDETAILSVIEWPARTNER_H

// End of file
