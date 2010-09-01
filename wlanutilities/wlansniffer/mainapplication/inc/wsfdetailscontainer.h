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
* Description:  Class header for CWsfDetailsContainer
*
*/

#ifndef C_WSFDETAILSCONTAINER_H
#define C_WSFDETAILSCONTAINER_H

//  EXTERNAL INCLUDES
#include <aknlists.h>
#include <AknNaviDecoratorObserver.h>

//  INTERNAL INCLUDES
#include "wsfdetailscontainerif.h"

// FORWARD DECLARATIONS
class CAknNavigationDecorator;
class CAknNavigationControlContainer;
class MWsfDetailsViewControllerPartner;
class CWsfDetailsViewModel;
class CWsfActiveTimeUpdater;
class MWsfActiveTimerHandler;

// CLASS DECLARATION
/**
* CWsfDetailsContainer  container control class.
* 
* @since S60 5.0
* @lib wlansniffer.exe
*/
class CWsfDetailsContainer : public CCoeControl, 
                             public MWsfDetailsContainerIf,
                             public MAknNaviDecoratorObserver

    {
    public: // Constructors and destructor
    
        /**
         * Two-phased constructor
         * @since S60 5.0 
         * @param aRect gives the correct TRect for construction.
         * @param aController reference for the MWsfActiveTimerHandler
         * @return instance of CWsfDetailsContainer class  
         */
        static CWsfDetailsContainer* NewL( 
                                      const TRect& aRect,
                                      MWsfActiveTimerHandler& aController );
        
        /**
         * Two-phased constructor. Leaves the object on CleanupStack
         * @since S60 5.0 
         * @param aRect gives the correct TRect for construction.
         * @param aController reference for the MWsfActiveTimerHandler
         * @return instance of CWsfDetailsContainer class  
         */
        static CWsfDetailsContainer* NewLC( 
                                      const TRect& aRect,
                                      MWsfActiveTimerHandler& aController );
        
        /**
         * Destructor of CWsfDetailsContainer class
         * @since S60 5.0     
         */
        ~CWsfDetailsContainer();

    private:  // Constructors and destructor

        /**
         * Default C++ constructor 
         * @since S60 5.0 
         */
        CWsfDetailsContainer();

        /**
         * 2nd phase constructor
         * @since S60 5.0 
         * @param aRect gives the correct TRect for construction.
         * @param aController reference for the MWsfActiveTimerHandler
         */
        void ConstructL( const TRect& aRect,
                         MWsfActiveTimerHandler& aController );

    public: // New functions

        /**
        * Set partner interface to container
        * @param aPartner - partner interface
        */
        void SetPartner( MWsfDetailsViewControllerPartner& aPartner );
        
        /**
        * Set Model to container
        * @param aModel
        */
        void SetDetailsModel( CWsfDetailsViewModel* aModel );

    public: // from CCoeControl
    
        /**
        * Handles key events.
        * @since S60 5.0
        * @param aKeyEvent The key event
        * @param aType The type of key event: EEventKey, EEventKeyUp or 
        *        EEventKeyDown
        * @return Indicates whether or not the key event was used 
        *         by this control
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType );

        /**
        * Gets an indexed component of a compound control
        * @since S60 5.0
        * @param aIndex The index of the control
        * @return The component control with an index of aIndex
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * Handles a change to the control's resources.
        * @since S60 5.0
        * @param aType A message UID value
        */        
        void HandleResourceChange( TInt aType );        

        
    public:  // From MWsfDetailsContainerIf
    
        /**
        * Refresh the content of Navi pane and 
        * Push it to stack.
        * @since S60 5.0
        * @param aPaneText Text to be show on pane
        */
        void UpdateNaviPaneL( const TDesC& aPaneText );

        /**
        * Refresh the content list box
        * @since S60 5.0
        * @param aItemTextArray Items to be show on listbox
        */
        void UpdateListBoxL( MDesCArray* aItemTextArray );
    

    private: // Methods from CoeControl
    
        /**
        * Responds to changes to the size and position of 
        * the contents of this control.
        * @since S60 5.0
        */
        void SizeChanged();

        /**
        * Gets the number of controls contained in a compound control.
        * @since S60 5.0
        * @return The number of component controls contained by this control
        */
        TInt CountComponentControls() const;
        
        /**
        * Responds to a change in focus
        * @since S60 5.0
        * @param aDrawNow Contains the value that was passed to it 
        *                 by SetFocus(). 
        */        
        void FocusChanged( TDrawNow aDrawNow );
    
        
    private: // From MAknNaviDecoratorObserver
    
        /**
         * Handles events in the navipane
         * @since S60 5.0
         * @param aEventID The id of the event
         */
        void HandleNaviDecoratorEventL( TInt aEventID );        

        
    private: // New methods
    
        /**
        * Gets the title panes.
        * @since S60 5.0
        */
        void GetPanesL();

        /**
        * Pop the NaviPane from stack.
        * @since S60 5.0
        */
        void PopNaviPane();

        
    private: // data       
    
        /**
        * Listbox contains details a connection
        * Own.
        */
        CAknSingleHeadingStyleListBox* iListBox;
        
        /**
        * Partner reference
        * Ref.
        */
        MWsfDetailsViewControllerPartner* iPartner;

        /**
        * For NaviPane.
        * Own.
        */
        CAknNavigationDecorator* iNaviDecorator;

        /**
        * For NaviPane.
        * Ref.
        */
        CAknNavigationControlContainer* iNaviPane;
        
        /**
        * For CWsfDetailsViewModel.
        * Ref.
        */
        CWsfDetailsViewModel* iDetailsModel;
        
        /**
        * For CWsfActiveTimeUpdater.
        * Own.
        */
        CWsfActiveTimeUpdater* iActiveUpdater;
    };

#endif // C_WSFDETAILSCONTAINER_H

// End of File

