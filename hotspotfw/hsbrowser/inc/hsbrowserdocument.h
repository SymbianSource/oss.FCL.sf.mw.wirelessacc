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
* Description:   Declares document for application.
*
*/



#ifndef HSBROWSERDOCUMENT_H
#define HSBROWSERDOCUMENT_H

// INCLUDES
#include <AknDoc.h>
   
// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;
class CHsBrowserModel;

// CLASS DECLARATION

/**
*  CHsBrowserDocument application class.
*/
class CHsBrowserDocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CHsBrowserDocument* NewL( CEikApplication& aApp );

        /**
        * Destructor.
        */
        virtual ~CHsBrowserDocument();

    public: // access
    
        /**
        * Accessor method for iModel.
        *
        * @return CHsBrowserModel* A pointer to model of HsBrowser.
        */
        inline CHsBrowserModel* Model();
    
    private:

        /**
        * C++ default constructor.
        */
        CHsBrowserDocument( CEikApplication& aApp );
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private: // new
         
        /**
        * Initialize model.
        */ 
        //inline void ResetModel();
         
    private:

        /**
        * From CEikDocument, create CBrCtlSampleAppAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    
    private: // data
    
        /**
        * Pointer to model of HsBrowser.
        * Owned.
        */
        CHsBrowserModel* iModel;
    };

#include "hsbrowserdocument.inl"

#endif

// End of File

