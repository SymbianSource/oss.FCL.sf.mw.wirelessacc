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
* Description:  Class header for CWsfMainViewContainer
*
*/

#ifndef C_WSFMAINVIEWCONTAINER_H
#define C_WSFMAINVIEWCONTAINER_H

//  EXTERNAL INCLUDES
#include <coecntrl.h>
#include <AknIconArray.h>
#include <AknsConstants.h>

//  FORWARD DECLARATIONS
class CAknDoubleGraphicStyleListBox;
class MWsfMainViewControllerPartner;
class CAknNavigationDecorator;
class CAknNavigationControlContainer;
class CWsfMainView;

//  CLASS DEFINITION
/**
 *  The container of the Main view.
 *  @since S60 v5.0
 */
class CWsfMainViewContainer : public CCoeControl
    {
    public:     // Constructors and destructor

        /**
         * Two-phased constructor
         * @since S60 5.0 
         * @param aRect gives the correct TRect for construction.
         * @return instance of CWsfMainViewContainer class  
         */
        static CWsfMainViewContainer* NewL( const TRect& aRect,
                CWsfMainView* aParentView );

        /**
         * Two-phased constructor. Leaves the object on CleanupStack
         * @since S60 5.0 
         * @param aRect gives the correct TRect for construction.
         * @return instance of CWsfMainViewContainer class  
         */
        static CWsfMainViewContainer* NewLC( const TRect& aRect,
                CWsfMainView* aParentView );

        /**
         * Destructor of CWsfMainViewContainer class
         * @since S60 5.0     
         */
        ~CWsfMainViewContainer();

    private:    // Constructors

        /**
         * Default C++ constructor 
         * @since S60 5.0 
         */
        CWsfMainViewContainer();

        /**
         * 2nd phase constructor
         * @since S60 5.0 
         * @param aRect gives the correct TRect for construction.
         */
        void ConstructL( const TRect& aRect, CWsfMainView* aParentView );

    public:     // New methods

        /**
        * Update the contents of control
        * Set the focus on aCurrentImte
        * @param aItemTextArray text of the current item
        * @param aCurrentItem index of the current item
        */ 
        void UpdateHotSpotsL( MDesCArray* aItemTextArray, 
                              TInt aCurrentItem ); 
        
        /**
        * Gives back the index of the selected item
        * @return the index of the selected item
        */ 
        TInt SelectedItem();

        /**
        * Set partner interface to container
        * @param aPartner - partner interface
        */
        void SetPartner( MWsfMainViewControllerPartner& aPartner );   

        /**
        * Refresh the content of Navi pane and 
        * Push it to stack.
        * @param aPaneText - text to be show on pane
        */
        void UpdateNaviPaneL( const TDesC& aPaneText );
        
        /**
        * Returns pointer to the listbox.
        * @return pointer to the listbox.
        */
        CAknDoubleGraphicStyleListBox* ListBox() const;

    protected: // Functions from CoeControl

        /**
        * From CCoeControl.
        * Responds to changes to the size and position of the contents 
        * of this control.
        */
        void SizeChanged();

        /**
        * From CCoeControl.
        * Gets the number of controls contained in a compound control.
        * @return the number of controls this component contains.
        */
        TInt CountComponentControls() const;

        /**
        * From CCoeControl.
        * Gets an indexed component of a compound control.
        * @return The required component control.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CCoeControl.
        * Handles key events.
        * @param aKeyEvent key event.
        * @param aType type of key event.
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType );

        /**
        * From CCoeControl, 
        * used for sclable ui and landscape support.
        * Handles a change to the control's resources.
        * @param aType type of change.
        */    
        void HandleResourceChange( TInt aType );        
        
        /**
        * From CCoeControl.
        * Returns the help context
        * @param aContext the returned help context
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;


    private:    // New methods

        /**
        * Create the iconarray. Derived classes please don't forget that the
        * 0th icon must be the markable listbox's mark icon.
        * @return The created iconarray. Owner is the caller.
        */
        CAknIconArray* LoadGraphicsL();

        /**
         * Creates a new icon of desired type.
         * @param aIconId UID Icon type UID of the icon to be created.
         * @return Pointer of the icon.
         */
        CGulIcon* LoadIconLC( TInt aIconId );
        
        /**
         * Creates a new icon of desired type.
         * @param aIconId UID Icon type UID of the icon to be created.
         * @return Pointer of the icon.
         */
        CGulIcon* LoadBlackIconLC( TInt aIconId );
        
        /**
         * Creates a new icon of desired type.
         * @param aAknItemId An Akvon skin item ID of the graphical item 
         *        (e.g. an icon) to load.
         * @param aIconId UID Icon type UID of the icon to be created.
         * @return Pointer of the icon.
         */
        CGulIcon* LoadSkinnableIconLC( const TAknsItemID aAknItemId, 
                const TInt aIconId );

        /**
         * Creates a new icon of desired type.
         * @param aAknItemId An Akvon skin item ID of the graphical item 
         *        (e.g. an icon) to load.
         * @param aIconId UID Icon type UID of the icon to be created.
         * @return Pointer of the icon.
         */
        CGulIcon* LoadSkinnableBlackIconLC( const TAknsItemID aAknItemId, 
                const TInt aIconId );
        
        /**
         * CleanUp for NaviPane
         */
        void PopNaviPane();
        
        /**
         * Get the Navipane from the framework 
         */
        void GetPanesL();        
        
        /**
         * Set the icons for the ListBox 
         */
        void SetListboxIconsL();
        
        /**
         * Cleanup function for a flat pointer array
         * @since S60 5.0
         * @param aArray The array whose elements are to be cleaned up
         */
        static void CleanUpResetAndDestroy( TAny* aArray );
        
        /*
         * Parses the string into ui component data
         * @param aString - a string into ui
         * @return - TBool - ETrue if connecting is found
         */
        TBool ParseStringForAnimationL( TDes& aString );
        
        /**
         * Set connecting status on.
         */
        void StartConnectingAnimationL();

        /**
         * Set connecting status off.
         */
        void StopConnectingAnimation();

        /**
         * Steps the connecting animation
         * @param ptr Pointer to an instance of this class
         */
        static TInt DoConnectingAnimationStepL( TAny* ptr );
        
        /**
         * Steps the connecting animation
         */                
        void DoConnectingAnimationStepL();

        /**
         *  Initialize the connecting animation
         */
        void InitializeConnectingAnimation();

        /*
         * Step the animation
         */
        void AnimateConnecting();

    private:    // Data

        /*
         * Pointer to the listbox
         */
        CAknDoubleGraphicStyleListBox* iListBox;

        /**
         * Partner reference
         * Ref.
         */
        MWsfMainViewControllerPartner* iPartner;

        /**
         * NaviDecorator pointer for NaviPane.
         * Own.
         */
        CAknNavigationDecorator* iNaviDecorator;

        /**
         * NaviDecorator's default control pointer For NaviPane.
         * Ref.
         */
        CAknNavigationControlContainer* iNaviPane;

        /**
         * List of animation states of connecting icon
         */
        TFixedArray<TInt, 4> iConnectingIcons;

        /**
         * The connecting animation frame index
         */
        TInt iConnectingIndex;

        /**
         * The periodic that steps the animation
         */
        CPeriodic* iAnimationPeriodic;

        /**
         * Constants for 'Connecting' text
         * Own.
         */
        HBufC* iStatusConnecting;
        
        /**
         * Reference to the parent view
         */
        CWsfMainView* iParentView;

    };



#endif      //  C_WSFMAINVIEWCONTAINER_H

// End of file
