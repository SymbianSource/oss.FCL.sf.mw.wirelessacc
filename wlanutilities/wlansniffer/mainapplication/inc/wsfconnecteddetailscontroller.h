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
* Description:  Class header for TWsfConnectedDetailsController
*
*/

#ifndef T_WSFCONNECTEDDETAILSCONTROLLER_H
#define T_WSFCONNECTEDDETAILSCONTROLLER_H

//  EXTERNAL INCLUDES
#include <e32def.h>

//  INTERNAL INCLUDES
#include "wsfsession.h"
#include "wsfwlaninfo.h"
#include "wsfdetailsviewcontrollerif.h"
#include "wsfactivetimerhandler.h"

//  FORWARD DECLARATIONS
class CWsfConnectedDetailsModel;
class MWsfActiveTimerHandler;
class CWsfConnectedDetailsDialog;

//  CLASS DEFINITION
/**
 * Details view controller
 * 
 * @since S60 5.0
 * @lib wlansniffer.exe
 */
class TWsfConnectedDetailsController: public MWsfDetailsViewControllerIf, 
                                      public MWsfActiveTimerHandler
    {
    public:     // New methods

        /**
         * Dialog setter
         * @since S60 5.0
         * @param aDialog Dialog pointer
         */        
        inline void SetDialog( CWsfConnectedDetailsDialog* aDialog );
        
        /**
         * Model setter
         * @since S60 5.0
         * @param aModel Model pointer
         */
        inline void SetModel( CWsfConnectedDetailsModel* aModel );

        
    public: //From MWsfDetailsViewControllerIf

        /**
         * Set the Wlan array & selected item from main view to details view
         * @since S60 5.0
         * @param aWlanArray - Current Wlan Array
         * @param aSelectedWlanSsid - Selected Wlan from Main view
         */
        void SetWlanListL( CWsfWlanInfoArray* aWlanArray, 
                           const TDesC8& aSelectedWlanSsid );

        /**
         * Wlan array has been chaged eg. some wlan dropped or new found
         * @since S60 5.0
         * @param aWlanArray - Current Wlan Array
         */
        void WlanListChangedL( CWsfWlanInfoArray* aWlanArray );

        /**
         * Refresh -> draw current model to view
         * @since S60 5.0
         */
        void RefreshL();
        
        /**
        * Sets partner object
        * @since S60 5.0
        * @param aPartner The new partner object
        */
        void SetPartner( MWsfDetailsViewPartner& aPartner );

        
    public:     // From MWsfActiveTimerHandler
    
        /**
         * Updates the details view active time
         * @since S60 5.0
         */
        void UpdateActiveTimeL();
   
        
    private:    // Data

        /**
         * Details dialog model (not owned)
         */
        CWsfConnectedDetailsModel* iModel;
        
        /**
         * Details dialog (not owned)
         */
        CWsfConnectedDetailsDialog* iDialog;
    };



#include "wsfconnecteddetailscontroller.inl"



#endif  //  T_WSFCONNECTEDDETAILSCONTROLLER_H

// End of file
