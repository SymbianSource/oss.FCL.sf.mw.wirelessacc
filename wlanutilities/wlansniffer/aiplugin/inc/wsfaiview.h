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
* Description:  Class header for CWsfAiView
*
*/


#ifndef C_WSFAIVIEW_H
#define C_WSFAIVIEW_H

//  EXTERNAL INCLUDES
#include <e32def.h>

//  INTERNAL INCLUDES
#include "wsfaiuiobserver.h"
#include "wsfaiplugincommon.h"


//  CONSTANTS
//  MACROS
//  DATA TYPES
//  FORWARD DECLARATIONS
class CEikLabel;
class MWsfAiPublishObserver;

class CWsfAiView : public CBase,
                   public MWsfAiUiObserver
    {
    public:     // Constructors
        /**
        * Two-phased constructor
        * @since S60 5.0 
        * @param aPublishObserver - reference to the plugin's 
        *                           abstract interface
        * @return instance of CWsfAiView class
        */
        static CWsfAiView* NewL( MWsfAiPublishObserver& aPublishObserver );
        
        /**
        * Two-phased constructor. Leaves the object on CleanupStack
        * @since S60 5.0 
        * @param aPublishObserver - reference to the plugin's 
        *                           abstract interface
        * @return instance of CWsfAiView class
        */
        static CWsfAiView* NewLC( MWsfAiPublishObserver& aPublishObserver );
        
        /**
        * Destructor of CWsfAiView class
        * @since S60 5.0     
        */
        ~CWsfAiView();
        
    private:
        
        /**
        * Default C++ constructor 
        * @since S60 5.0 
        * @param aPublishObserver - reference to the plugin's 
        *                           abstract interface
        */
        CWsfAiView( MWsfAiPublishObserver& aPublishObserver );
        
        /**
        * 2nd phase constructor
        * @since S60 5.0 
        */
        void ConstructL();    

    public: // from MWsfAiUiObserver
        
        /**
        * Update the contents of control
        * Set the focus on aCurrentImte
        * @since S60 5.0 
        * @param aItemTextArray text of the current item
        * @param aCurrentItem index of the current item
        */ 
        void UpdateHotSpotsL( MDesCArray* aItemTextArray, 
                              TInt aCurrentItem );

        /**
        * Gives back the index of the selected item
        * @since S60 5.0 
        * @return the index of the selected item
        */ 
        TInt SelectedItem();
        
        /*
        * Check if the control is multiline capable control 
        * @since S60 5.0 
        * @return Etrue if control is multiline capable control
        */
        TBool MultilineControl();

        /*
        * Display engine off text in UI
        * @since S60 5.0 
        */
        void DisplayEngineOffL();
        
        /*
        * Set the refressing UI note visible
        * @since S60 5.0 
        */
        void ForceRefreshingL();
        
        /*
        * Set the refressing UI note visible
        * @since S60 5.0 
        */
        void StartConnectingAnimationL();
        
        /**
        * Update the contents of control
        * @since S60 5.0 
        * @param aItemTextArray text of the current item
        */ 
        void UpdateViewL( MDesCArray* aItemTextArray );
        
    private:
        
        /*
        * Parses the string into ui component data
        * @since S60 5.0 
        * @param aString - a string into ui
        * @return - TBool - ETrue if deferred update is executed
        */
         TBool ParseStringL( const TDesC& aString );
             
        /*
        * Complete the change for new data - this is 
        * a callback for deferred update
        * @since S60 5.0 
        * @param aPtr pointer to this
        * @return ETrue if update is completed
        */
        static TInt DoCompleteUpdateL( TAny* aPtr );

        /* 
        * Commit the update change
        * @since S60 5.0 
        */
        void DoCompleteUpdateL();

        /** 
        * Activates refresh timer  
        * @since S60 5.0 
        */
        void ActivateRefreshTimer();
        
        /**
        * 'MakePublishIconsL' and 'MakeTransparentPublishIconsL'
        * create arrays consisting of icon ids for the icons to publish
        * @since S60 5.0 
        */
        void MakePublishIconsL();
        
        /** 
        * 'MakePublishIconsL' and 'MakeTransparentPublishIconsL'
        * create arrays consisting of icon ids for the icons to publish
        * @since S60 5.0 
        */
        void MakeTransparentPublishIconsL();

    private:    // Data

        /**
        * Coe env for loading string from resource
        * Not Own.
        */
        CCoeEnv* iEnv;

        /**
        * Publish images in array
        * Own.
        */
        CArrayFix< TInt >* iPublishIconArray;

        /**
         * Timer for update
         * Own.
         */
        CPeriodic*     iDeferredUpdater;
        
        /**
        * Times callback
        * Own. 
        */
        TCallBack*     iDeferredUpdateCallBack;
        
        /**
        * Title text label
        * Own.
        */
        CEikLabel* iTextLabel;
        
        /**
        * The label text
        */ 
        TName        iCurrentText;
        
        /**
        * ID constants for Secure Icon 
        */
        TWsfPluginIcons iUpdateSecureIconDeferred;

        /**
        * ID constants for Signal Strength Icon 
        */
        TWsfPluginIcons iUpdateSignalStrengthIconDeferred;

        /**
        * ID constants for Status Icon 
        */
        TWsfPluginIcons iUpdateStatusIconDeferred;
        
        /**
        * ID constants for Status Icon saved to remember after refreshing
        */
        TWsfPluginIcons iSavedUpdateStatusIconDeferred;
        
        /**
        * Pointer to Publish Observer interface
        * Not Own. 
        */
        MWsfAiPublishObserver* iPublishObserver;
        
        /**
        * Constants for 'WLAN scanning off' text
        * Own.
        */
        HBufC* iStatusScanningOff;
        
        /**
        * Constants for 'Refreshing' text
        * Own.
        */
        HBufC* iStatusRefreshing;

        /**
        * Constants for 'Connecting' text
        * Own.
        */
        HBufC* iStatusConnecting;
    };



#endif      //  C_WSFAIVIEW_H

// End of file
