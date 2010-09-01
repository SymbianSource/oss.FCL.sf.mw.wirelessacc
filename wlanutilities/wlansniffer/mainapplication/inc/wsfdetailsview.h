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
* Description:  Class header for CWsfDetailsView
*
*/

#ifndef C_WSFDETAILSVIEW_H
#define C_WSFDETAILSVIEW_H

// EXTERNAL INCLUDES
#include <aknview.h>
#include <aknlists.h>

// INTERNAL INCLUDES
#include "wsfdetailsviewcontrollerif.h"
#include "wsfdetailsviewcontroller.h"

// FORWARD DECLARATIONS
class CWsfDetailsContainer;
class MDetailsViewPartner;
class CWsfDetailsViewModel;
class TWsfDetailsViewController;

// CLASS DECLARATION
/**
*  CWsfDetailsView view class.
*/
class CWsfDetailsView : public CAknView, 
                        public MEikListBoxObserver
    {
    
    public: 
    
            // Constructors and destructor
        /**
        * Two-phased constructor
        * @since S60 5.0 
        * @return instance of CWsfDetailsView class
        */
        static CWsfDetailsView* NewL();
        
        /**
        * Two-phased constructor. Leaves the object on CleanupStack
        * @since S60 5.0 
        * @return instance of CWsfDetailsView class
        */
        static CWsfDetailsView* NewLC();
        
        /**
        * Destructor of CWsfDetailsView class
        * @since S60 5.0     
        */
        ~CWsfDetailsView();
            
        // Methods from CAknView       
            
        /**
        * Returns views id.
        * @since S60 5.0     
        * @return id for this view.
        */
        TUid Id() const;
    
        /** 
        * Command handling function. 
        * @since S60 5.0     
        * @param aCommand ID of the command to respond to. 
        */
        void HandleCommandL( TInt aCommand );
            
    
        // Methods from MEikListBoxObserver       
            
        /**
        * From MEikListBoxObserver.
        * Handles listbox events.
        * @since S60 5.0     
        * @param aListBox listbox pointer.
        * @param aEventType event type.
        */
        void HandleListBoxEventL( CEikListBox* aListBox, 
                                  TListBoxEvent aEventType );
            
        // New methods
           
        /**
        * Return controller interface
        * @since S60 5.0     
        * @return controiller interface
        */
        MWsfDetailsViewControllerIf& Controller();
            
    
    protected: 
    
        // Functions from CAknView 
                
        /**
        * From CAknView.
        * Called when view is activated.
        * @since S60 5.0     
        * @param aPrevViewId previous view id.
        * @param aCustomMessageId custom message id.
        * @param aCustomMessage custom message content.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId, 
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
                              
        /**
        * From CAknView.
        * Called when view is deactivated.
        */
        void DoDeactivate();
    
    private: // Constructors and destructor
        
        /**
         * Default C++ constructor 
         * @since S60 5.0 
         */
        CWsfDetailsView();
        
        /**
        * 2nd phase constructor
        * @since S60 5.0 
        */
        void ConstructL();
    
    private: // Data
    
        /**
        * Details view container
        * Own.
        */
        CWsfDetailsContainer* iContainer;
    
        /**
        * Controller of Details view
        * Own.
        */
        TWsfDetailsViewController iController;
            
        /**
        * Details view Model
        * Own.
        */
        CWsfDetailsViewModel* iModel;

    };

#endif // C_WSFDETAILSVIEW_H

// End of File
