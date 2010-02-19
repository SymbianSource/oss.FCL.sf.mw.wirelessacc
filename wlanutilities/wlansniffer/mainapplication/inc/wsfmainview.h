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
* Description:  Class header for CWsfMainView
*
*/

#ifndef C_WSFMAINVIEW_H
#define C_WSFMAINVIEW_H

//  EXTERNAL INCLUDES
#include <aknview.h>
#include <aknlists.h>

//  INTERNAL INCLUDES
#include "wsfmainviewcontroller.h"

//  FORWARD DECLARATIONS
class CWsfMainViewContainer;        
class CWsfMainViewModel;



//  CLASS DEFINITION
/**
 * The view class of the Main view.
 * @since S60 5.0 
 */
NONSHARABLE_CLASS( CWsfMainView ): public CAknView, 
                                   public MEikListBoxObserver
    {
    public:

        /**
         * Two-phased constructor
         * @since S60 5.0 
         * @param aIsEmbedded check application started 
         *        as an embedded application
         * @return instance of CWsfMainView class
         */
        static CWsfMainView* NewL( TBool aIsEmbedded );

        /**
         * Two-phased constructor. Leaves the object on CleanupStack
         * @since S60 5.0 
         * @param aIsEmbedded check application started 
         *        as an embedded application
         * @return instance of CWsfMainView class
         */
        static CWsfMainView* NewLC( TBool aIsEmbedded );


        /**
        * Destructor of CWsfMainView class
        * @since S60 5.0     
        */
        ~CWsfMainView(); 
    
    private:

        /**
        * 2nd phase constructor
        * @since S60 5.0 
        */
        void ConstructL( TBool aIsEmbedded );

        /**
        * Default C++ constructor 
        * @since S60 5.0 
        */
        CWsfMainView();


    public: //From CAknView
        
        /**
        * From CAknView returns Uid of View
        * @since S60 5.0 
        * @return TUid uid of the view
        */
        TUid Id() const;

        /**
        * From MEikMenuObserver delegate commands from the menu
        * @since S60 5.0 
        * @param aCommand a command emitted by the menu 
        */
        void HandleCommandL( TInt aCommand );

        /**
        * From CAknView reaction if size change
        * @since S60 5.0 
        */
        void HandleClientRectChange();
        

    private:    //From CAknView

        /**
        * From CAknView activate the view
        * @since S60 5.0 
        * @param aPrevViewId 
        * @param aCustomMessageId 
        * @param aCustomMessage 
        */
        void DoActivateL( const TVwsViewId& aPrevViewId, 
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );

        /**
        * From CAknView deactivate the view (free resources)
        * @since S60 5.0 
        */
        void DoDeactivate();

    private: // from MEikMenuObserver
        
        /** 
        * From MEikCommandObserver
        * Control the menu pane commands
        * @since S60 5.0 
        * @param aResourceId resource id of the menu pane to be modified
        * @param aMenuPane pointer to menu pane to be modified
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

    protected: 
        /**
        * From MEikListBoxObserver.
        * Handles listbox events.
        * @since S60 5.0     
        * @param aListBox listbox pointer.
        * @param aEventType event type.
        */
        void HandleListBoxEventL( CEikListBox* aListBox, 
                                  TListBoxEvent aEventType );

    public:  //New functions
        
        /**
        * Return controller interface
        * @since S60 5.0     
        * @return controiller interface
        */
        MWsfMainViewControllerIf& Controller();
        
        /**
         * Sets the iSelectionKey True
         * @since S60 5.0 
         */
        void SelectionKeyPressed();
        
        /**
         * Checks if Browser is using the active WLAN connection
         */
        void UpdateBrowserUsageInfoL();

    private:  //New functions
        
        /**
        * Check if the Browser application is already running and 
        * using the selected connection.
        * @since S60 5.0 
        * @return EFalse if Start Web Browsing menu item should be shown
        *         ETrue if Start Web Browsing menu item should not be shown
        */
        TBool IsBrowserUsingWlanL();
        
    private:

        /**
        * Main view's controller
        */
        TWsfMainViewController iController;
        
        /**
        * Main view's container
        * Own.
        */
        CWsfMainViewContainer* iContainer;
        
        /**
        * Main view's model
        * Own.
        */
        CWsfMainViewModel* iModel;

        /**
        * Indicates FSelectionKey is pressed or not.
        */
        TBool iSelectionKey;
        
        /**
         * If Browser is using the WLAN connection or not.
         */
        TBool iIsBrowserUsingWlan;
        
    };


#endif // C_WSFMAINVIEW_H
