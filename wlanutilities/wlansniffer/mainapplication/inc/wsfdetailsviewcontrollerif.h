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
* Description:  Class header for MWsfDetailsViewControllerIf
*
*/

#ifndef M_WSFDETAILSVIEWCONTROLLERIF_H
#define M_WSFDETAILSVIEWCONTROLLERIF_H

// INCLUDES
#include <e32base.h>


//FORWARD DECLARATIONS
class CWsfWlanInfoArray;
class MWsfDetailsViewPartner;

/**
 *  Interface between app level controller and details view controller
 */
class MWsfDetailsViewControllerIf
    {

    public:

        /**
         * Set the Wlan array & selected item from main view to details view
         * @since S60 5.0
         * @param aWlanArray Current Wlan Array
         * @param aSelectedWlanSsid Selected Wlan from Main view
         */
        virtual void SetWlanListL( CWsfWlanInfoArray* aWlanArray, 
                                   const TDesC8& aSelectedWlanSsid ) = 0;

        /**
         * Wlan array has been chaged eg. some wlan dropped or new found
         * @since S60 5.0
         * @param aWlanArray Current Wlan Array
         */
        virtual void WlanListChangedL( CWsfWlanInfoArray* aWlanArray ) = 0;

        /**
         * Refresh -> draw current model to view
         * @since S60 5.0
         */
        virtual void RefreshL() = 0;
        
        /**
         * Sets partner object
         * @since S60 5.0
         * @param aPartner The current partner object
         */
        virtual void SetPartner( MWsfDetailsViewPartner& aPartner ) = 0;

    };


#endif // M_WSFDETAILSVIEWCONTROLLERIF_H

