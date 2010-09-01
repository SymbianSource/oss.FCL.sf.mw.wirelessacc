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
* Description:  Class header for AiHelper
*
*/


#ifndef C_WSFAIHELPERAPPLICATION_H
#define C_WSFAIHELPERAPPLICATION_H

//  EXTERNAL INCLUDES
#include <aknapp.h>


//  CLASS DEFINITION
/** 
 * An instance of CWsfAiHelperApplication is the application part of 
 * the AVKON application framework.
 * 
 * @since S60 5.0
 * @lib wsfaihelper.exe
 */
NONSHARABLE_CLASS( CWsfAiHelperApplication ): public CAknApplication
    {
    public:  // from CAknApplication
    
        /**
         * Returns the application UID value
         * @since S60 5.0
         * @return The UID of this application
         */
        TUid AppDllUid() const;
    
    protected: // from CAknApplication
        /** 
        * Creates a CApaDocument object and return a pointer to it
        * @since S60 5.0
        * @return A pointer to the created document
        */
        CApaDocument* CreateDocumentL();
    };

#endif // C_WSFAIHELPERAPPLICATION_H
