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
* Description:  Class header for CWsfAiHelperContainer
*
*/


#ifndef C_WSFAIHELPERCONTAINER_H
#define C_WSFAIHELPERCONTAINER_H

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <coecntrl.h>


//  FORWARD DECLARATIONS
class CWsfAiHelperAppUi;
class MAknsControlContext;


//  CLASS DEFINITION
/**
 * The container class for the helper application UI
 * 
 * @since S60 5.0
 * @lib wsfaihelper.exe
 */
NONSHARABLE_CLASS( CWsfAiHelperContainer ): public CCoeControl
    {
    public:     // Constructors and destructor
      
        /**
         * Second-phase constructor
         * @since S60 5.0
         * @param aRect Dimensions of this container
         * @param aAppUi Pointer to the AppUi instance (ownership not passed)
         */
        void ConstructL( TRect aRect, CWsfAiHelperAppUi* aAppUi );

        /**
         * Destructor
         * @since S60 5.0
         */
        ~CWsfAiHelperContainer();

        
    public:     // from CCoeControl
           
        /**
        * Responds to changes to the size and position of the contents 
        * of this control.
        * @since S60 5.0
        */
        void SizeChanged();

        /**
        * Used for scalable ui and landscape support. Handles a change to the 
        * control's resources.
        * @since S60 5.0
        * @param aType The type of change.
        */    
        void HandleResourceChange( TInt aType );


    private:  // from CCoeControl

        /**
         * Retrieves an object of the same type as that encapsulated in aId
         * @since S60 5.0
         * @param aId The id of the requested object type
         * @return Pointer to the requested object
         */
        TTypeUid::Ptr MopSupplyObject( TTypeUid aId );

        /**
         * Draws the control.
         * @since S60 5.0
         * @param aRect The region of the control to be redrawn.
         */
        void Draw( const TRect& aRect ) const;
        

    private:    // Data
                  
        /**
         * Pointer to the AppUi object (not owned)
         */
        CWsfAiHelperAppUi* iAppUi;
           
        /**
         * Pointer to the control context (owned)
         */
        MAknsControlContext* iBackGround;
    
    };

#endif      //  C_WSFAIHELPERCONTAINER_H

// End of file
