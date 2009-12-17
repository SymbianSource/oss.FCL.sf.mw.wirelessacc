/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   The UI class for HotSpot Browser Application.
*
*/



#ifndef HSBROWSERAPPUI_H
#define HSBROWSERAPPUI_H

// INCLUDES
#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknappui.h>
#include <in_sock.h>

// FORWARD DECLARATIONS
class CHsBrowserContainer;

// CONSTANTS
const TInt KBufSize = 512;

// CLASS DECLARATION
/**
* CHsBrowserAppUi class.
* Application UI class which provides support for the following features:
* - EIKON control architecture
* 
*/
class CHsBrowserAppUi : public CAknAppUi
    {
    public:
    
        /**
        * EPOC default constructor.
        */      
        void ConstructL();

        /**
        * Destructor.
        */      
        ~CHsBrowserAppUi();
        
    public: // From CEikAppUi
        
        /**
        * Parses command line arguments.
        *
        * @param aCommandLine Reference to command line.
        */
        TBool ProcessCommandParametersL( CApaCommandLine& aCommandLine );

        /**
        * Takes care of command handling.
        *
        * @param aCommand Command to be handled
        */
        void HandleCommandL(TInt aCommand);

        /**
        * Handles key events.
        *
        * @param aKeyEvent Event to handled.
        * @param aType Type of the key event. 
        * @return Response code (EKeyWasConsumed, EKeyWasNotConsumed). 
        */
        virtual TKeyResponse HandleKeyEventL(
            const TKeyEvent& aKeyEvent,TEventCode aType);

        /**
        * Handles changes in keyboard focus when an application switches to, 
        * or from, the foreground.
        *
        * @param aForeground ETrue if the application is in the foreground,
        * otherwise EFalse.
        */
        void HandleForegroundEventL( TBool aForeground );

    private: //data

        /**
        * Pointer to container of Hotspot Browser .
        */
        CHsBrowserContainer* iAppContainer;        
    };

#endif

// End of File


