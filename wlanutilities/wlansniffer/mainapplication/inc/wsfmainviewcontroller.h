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
* Description:  Class header for TWsfMainViewController
*
*/
          
#ifndef T_WSFMAINVIEWCONTROLLER_H
#define T_WSFMAINVIEWCONTROLLER_H

//  EXTERNAL INCLUDES
#include <e32def.h>


//  INTERNAL INCLUDES
#include "wsfsession.h"
#include "wsfwlaninfo.h"
#include "wsfmainviewcontrollerif.h"
#include "wsfmainviewpartner.h"
#include "wsfmainviewcontrollerpartner.h"


//  FORWARD DECLARATIONS
class CWsfMainViewModel;
class CWsfMainViewContainer;
class CWsfWlanInfoArray;
class MWsfMainViewPartner;

//  CLASS DEFINITION
/**
 * TWsfMainViewController acts as a view level controller
 * @since S60 5.0
 */
class TWsfMainViewController : public MWsfMainViewControllerIf, 
                               public MWsfMainViewControllerPartner
    {

    public:     // New methods


        /**
        * Set view level model
        * @since S60 5.0
        * @param aModel - model pointer
        */
        void SetModel( CWsfMainViewModel* aModel );

        /**
        * Set view level view
        * @param aContainer - container pointer
        */
        void SetContainer( CWsfMainViewContainer* aContainer );

        /**
        * Updates available WLAN list to the display
        * @param aWlanList - Current Wlan Array
        */
        void UpdateHotSpotsL( CWsfWlanInfoArray* aWlanList );

        /**
         * Updates the local copy of the selected wlaninfo
         * @since S60 5.0
         */
        void UpdateSelectedItemL();

        
    public: //From MWsfMainViewControllerIf

        /**
         * Updates WLAN list with values passed as parameter
         * @param aWlanArray list of WLAN
         * @since S60 5.0
         */
        void UpdateWlanListL( CWsfWlanInfoArray* aWlanArray );

        /**
         * Request to refresh the mainview
         * @since S60 5.0
         */
        void RefreshL();
        
        /**
         * Request to refresh the mainview
         * @return Details of selected WLAN
         * @since S60 5.0
         */
        TWsfWlanInfo* SelectedItem();

        /**
         * Sets partner object
         * @aPartner The partner
         * @since S60 5.0
         */
        void SetPartner( MWsfMainViewPartner& aPartner );


    public: //MWsfMainViewControllerPartner

        /**
         * Handles up key presses
         * @since S60 5.0
         */
        void HandleUpKeyPressedL();

        /**
         * Handles down key presses
         * @since S60 5.0
         */
        void HandleDownKeyPressedL();

        /**
         * Handles selection key presses
         * @since S60 5.0
         */
        void HandleSelectionKeyPressedL();

    private:   
        
        /**
        * Updated navipane
        */
        void UpdateNaviPaneL();
        

    private:    // Data
    
        //Ref: View level model
        CWsfMainViewModel* iModel;

        //Ref: View level view
        CWsfMainViewContainer* iContainer;

        //Ref: Partner
        MWsfMainViewPartner* iPartner;
        
        // Temporary storage for the current wlaninfo
        TWsfWlanInfo iSelectedItem;
        
        // Indicates whether a real wlaninfo is selected
        TBool iValidItem;
    };

//  INLINE FUNCTIONS

#endif      // T_WSFMAINVIEWCONTROLLER_H

// End of file

