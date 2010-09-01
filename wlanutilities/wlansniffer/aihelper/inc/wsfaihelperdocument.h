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
* Description:  Class header for CWsfAiHelperDocument
*
*/


#ifndef C_WSFAIHELPERDOCUMENT_H
#define C_WSFAIHELPERDOCUMENT_H


//  EXTERNAL INCLUDES
#include <AknDoc.h>


//  INTERNAL INCLUDES
#include "wsfwlaninfo.h"


//  FORWARD DECLARATIONS
class CWsfAiHelperAppUi;
class CEikApplication;
class CWsfModel;


//  CLASS DEFINITION
/**
 * An instance of class CWsfAiHelperDocument is the Document part of the AVKON
 * application framework
 * 
 * @since S60 5.0
 * @lib wsfaihelper.exe
 */
NONSHARABLE_CLASS( CWsfAiHelperDocument ): public CAknDocument
    {
    public:
    
        /**
         * Factory function
         * @since S60 5.0
         * @param aApp Reference to the Application object 
         *             (ownership not passed)
         * @return Class instance
         */
        static CWsfAiHelperDocument* NewL( CEikApplication& aApp );
    
        /**
         * Factory function
         * @since S60 5.0
         * @param aApp Reference to the Application object 
         *             (ownership not passed)
         * @return Class instance
         */
        static CWsfAiHelperDocument* NewLC( CEikApplication& aApp );

        /**
         * Destructor
         * @since S60 5.0
         */
        ~CWsfAiHelperDocument();

        
    private:

        /**
         * Second-phase constructor
         * @since S60 5.0
         */
        void ConstructL();

        /**
         * Constructor
         * @since S60 5.0
         * @param aApp Reference to the application creating this document
         *             (ownership not passed)
         */
        CWsfAiHelperDocument( CEikApplication& aApp );

        
    public: // from CEikDocument
        /**
         * Constructs the application UI.
         * @since S60 5.0
         * @return AppUi instance
         */
        CEikAppUi* CreateAppUiL();
    

        /**
         * Restores the document's state from the specified file, 
         * or creates a new default document.
         * @since S60 5.0
         * @param aDoOpen True to open an existing file, 
         *                false to create a new default file
         * @param aFilename The path and name of the file to open or create.
         * @param aFs File server session to use. 
         * @return The file store that stores the main document. 
         */
        CFileStore* OpenFileL( TBool aDoOpen, 
                               const TDesC& aFilename, 
                               RFs& aFs );
    
        
    public:     // new methods
        
        /**
         * Returns reference to the model
         * @since S60 5.0
         * @return Reference to the model (ownership not passed)
         */
        inline CWsfModel& Model();
        
        /** 
         * Returns reference to the current wlaninfo
         * @since S60 5.0
         * @return Reference to the wlaninfo used (ownership not passed)
         */
        inline TWsfWlanInfo& WlanInfo();
        
    
    private:    // data
    
        /**
         * Model object (owned)
         */
        CWsfModel* iModel;
        
        /**
         * Data of the currently used WLAN network
         */
        TWsfWlanInfo iWlanInfo;
        
    };


#include "wsfaihelperdocument.inl"

#endif // C_WSFAIHELPERDOCUMENT_H
