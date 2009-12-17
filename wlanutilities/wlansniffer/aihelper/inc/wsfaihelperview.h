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
* Description:  Class header for CWsfAiHelperView
*
*/


#ifndef C_WSFAIHELPERVIEW_H
#define C_WSFAIHELPERVIEW_H


// EXTERNAL INCLUDES
#include <aknview.h>


// FORWARD DECLARATIONS
class CWsfAiHelperAppUi;
class CWsfAiHelperContainer;


// CLASS DECLARATION
/**
*  Main view class for the AiHelper application
* 
* @since S60 5.0
* @lib wsfaihelper.exe
*/
NONSHARABLE_CLASS( CWsfAiHelperView ): public CAknView
    {
    public:     // Constructors and destructor
        /**
         * Factory function
         * @since S60 5.0
         * @param aAppUi Pointer to the appui instance (ownership not passed)
         * @return Class instance
         */
        static CWsfAiHelperView* NewL( CWsfAiHelperAppUi* aAppUi );

        /**
         * Factory function
         * @since S60 5.0
         * @param aAppUi Pointer to the appui instance (ownership not passed)
         * @return Class instance
         */
        static CWsfAiHelperView* NewLC( CWsfAiHelperAppUi* aAppUi );

        /**
         * Destructor
         * @since S60 5.0
         */
        ~CWsfAiHelperView();

    private:    // Constructors
        
        /**
         * Constructor
         * @since S60 5.0
         * @param aAppUi Pointer to the appui instance (ownership not passed)
         */
        CWsfAiHelperView( CWsfAiHelperAppUi* aAppUi );
        
        /**
         * Second-phase constructor
         * @since S60 5.0
         */
        void ConstructL();

        
    private:  // From CAknView
        
        /**
         * Returns views id
         * @since S60 5.0
         * @return Id of this view
         */
        TUid Id() const;
        
        /**
         * Handles a view activation and passes the message of 
         * type aCustomMessageId
         * @since S60 5.0
         * @param aPrevViewId Specifies the view previously active.
         * @param aCustomMessageId Specifies the message type.
         * @param aCustomMessage The activation message
         */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        
        /**
         * View deactivation function
         * @since S60 5.0
         */
        void DoDeactivate();
        
        /**
         * Handle user menu selections
         * @since S60 5.0
         * @param aCommand The enumerated code for the option selected
         */
        void HandleCommandL( TInt aCommand );

        
    private: // Data
        /**
         * Pointer to the appui object (not owned)
         */
        CWsfAiHelperAppUi* iAppUi;
        
        /**
         * Container object (owned)
         */
        CWsfAiHelperContainer* iContainer;
    };

#endif // C_WSFAIHELPERVIEW_H

// End of File
