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
* Description:  Class header for CWsfDocument
*
*/

#ifndef C_WSFDOCUMENT_H
#define C_WSFDOCUMENT_H

//External includes
#include <AknDoc.h>

//Internal includes
#include "wsfmaincontroller.h"


// Forward references
class CWsfAppUi;
class CEikApplication;
class CWsfModel;
class CWsfWlanInfoArrayVisitor;


/**
 * The document class of the Symbian OS application’s architecture.
 * 
 * @since S60 v5.0
 * @lib wlansniffer.exe
 */
NONSHARABLE_CLASS( CWsfDocument ) : public CAknDocument
    {
    public:
    
        /**
         * Two-phased constructor
         * @since S60 5.0 
         * @param aApp application creating this document
         * @return a pointer to the created instance of CWsfDocument
         */
        static CWsfDocument* NewL( CEikApplication& aApp );
    
        /**
         * Two-phased constructor. Leaves the object on CleanupStack
         * @since S60 5.0 
         * @param aApp application creating this document
         * @return a pointer to the created instance of CWsfDocument
         */
        static CWsfDocument* NewLC( CEikApplication& aApp );
    
        /**
         * Destructor of CWsfDocument class
         * @since S60 5.0     
         */
        ~CWsfDocument();
        
    public:

        /**
         * The handle for iWlanInfoBranding 
         * (reference for CWsfWlanInfoArrayVisitor)
         * @since S60 5.0    
         * @return reference for CWsfWlanInfoArrayVisitor
         */
        inline CWsfWlanInfoArrayVisitor& WlanInfoBranding() const;
        
        /**
         * Returns a reference for the model (CWsfModel)
         * @since S60 5.0    
         * @return reference to CWsfModel
         */
        inline CWsfModel& Model() const;
    
    public: // from CAknDocument
        
        /**
         * From CAknDocument.
         * Function to create the AppUi.
         *
         * @since S60 v5.0
         * @return the created AppUi instance
         */
        CEikAppUi* CreateAppUiL();
        
    
    private:
    
        /**
         * 2nd phase constructor
         * @since S60 5.0 
         */
        void ConstructL();
    
        /**
         * Default C++ constructor 
         * @since S60 5.0 
         * @param aApp application creating this document
         */
        CWsfDocument( CEikApplication& aApp );
    
    private: 
    
        /**
         * Main Controller
         */
        TWsfMainController iMainController;
        
        /**
         * Model
         * Own.
         */
        CWsfModel* iModel;
        
        /**
         * WlanInfoArrayVisitor
         * Own.
         */
        CWsfWlanInfoArrayVisitor *iWlanInfoBranding;
    };


#include "wsfdocument.inl"


#endif // C_WSFDOCUMENT_H
