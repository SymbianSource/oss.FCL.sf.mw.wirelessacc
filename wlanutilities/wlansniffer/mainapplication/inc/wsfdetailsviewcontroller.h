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
* Description:  Class header for TWsfDetailsViewController
*
*/

#ifndef T_WSFDETAILSVIEWCONTROLLER_H
#define T_WSFDETAILSVIEWCONTROLLER_H

//  EXTERNAL INCLUDES
#include <e32def.h>

//  INTERNAL INCLUDES
#include "wsfsession.h"
#include "wsfwlaninfo.h"
#include "wsfdetailsviewcontrollerif.h"
#include "wsfdetailsviewcontrollerpartner.h"
#include "wsfactivetimerhandler.h"

//  FORWARD DECLARATIONS
class CWsfDetailsViewModel;
class MWsfDetailsContainerIf;
class MWsfDetailsViewPartner;
class MWsfActiveTimerHandler;

//  CLASS DEFINITION
/**
 * Details view controller
 * 
 * @since S60 5.0
 * @lib wlansniffer.exe
 */
class TWsfDetailsViewController : public MWsfDetailsViewControllerIf, 
                                  public MWsfDetailsViewControllerPartner,
                                  public MWsfActiveTimerHandler
    {
    public:     // New methods
    
        /**
         * Model setter
         * @since S60 5.0
         * @param aModel Model pointer
         */
        void SetModel( CWsfDetailsViewModel* aModel );

        /**
         * container setter
         * @since S60 5.0
         * @param aContainer Container pointer
         */
        void SetContainer( MWsfDetailsContainerIf* aContainer );

        
    public: //From MWsfDetailsViewControllerIf
        /**
         * Set the Wlan array & selected item from main view to details view
         * @since S60 5.0
         * @param aWlanArray Current Wlan Array
         * @param aSelectedWlanSsid Selected Wlan from Main view
         */
        void SetWlanListL( CWsfWlanInfoArray* aWlanArray, 
                           const TDesC8& aSelectedWlanSsid );

        /**
         * Wlan array has been chaged eg. some wlan dropped or new found
         * @since S60 5.0
         * @param aWlanArray Current Wlan Array
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

        
    public: //Method from MWsfDetailsViewControllerPartner
    
        /**
         * Handles left key presses
         * @since S60 5.0
         */
        void HandleLeftKeyPressedL();
        
        /**
         * Handles right key presses
         * @since S60 5.0
         */
        void HandleRightKeyPressedL();

        /**
         * Handles selection key presses
         * @since S60 5.0
         */
        void HandleSelectionKeyPressedL();
        

    public:     // From MWsfActiveTimerHandler
    
        /**
         * Updates the details view active time
         * @since S60 5.0
         */
        void UpdateActiveTimeL();
        
        
    private:    // New methods
    
        /**
         * Handles pane text update on container
         * @since S60 5.0
         */
        void HandlePaneTextUpdateL();
    
        
    private:    // Data

        //Ref: details view model
        CWsfDetailsViewModel* iModel;
        
        //Ref: details view container
        MWsfDetailsContainerIf* iContainer;
        
        //Ref: Details view partner
        MWsfDetailsViewPartner* iPartner;
        
    };

#endif      //  T_WSFDETAILSVIEWCONTROLLER_H

// End of file

