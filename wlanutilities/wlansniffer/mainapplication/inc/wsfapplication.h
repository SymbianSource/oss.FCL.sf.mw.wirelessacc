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
* Description:  Class header for CWsfApplication
*
*/


#ifndef C_WSFAPPLICATION_H
#define C_WSFAPPLICATION_H

#include <aknapp.h>


//  CLASS DEFINITION
/** 
* An instance of CWsfApplication is the application part of the
* AVKON application framework for the Sniffer example application
* 
* @since S60 5.0
* @lib wlansniffer.exe
*/
NONSHARABLE_CLASS( CWsfApplication ): public CAknApplication
    {
    public:  // from CAknApplication
    
        /** 
        * Returns the application DLL UID value
        * @since S60 5.0
        * @return The UID of this Application/Dll
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

#endif // C_WSFAPPLICATION_H
