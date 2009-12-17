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
* Description:   Declares main application class.
*
*/



#ifndef HSBROWSERAPP_H
#define HSBROWSERAPP_H

// INCLUDES
#include <aknapp.h>

// CLASS DECLARATION
/**
* CHsBrowserApp application class.
* Provides the factory to create a concrete document object.
* 
*/
class CHsBrowserApp : public CAknApplication
    {
    
    public: // Functions from base classes
        /**
        * From CApaApplication, overridden to enable INI file support.
        * @return CDictionaryStore*, A pointer to the dictionary store
        */
        CDictionaryStore* OpenIniFileLC(RFs& aFs) const;
    private:

        /**
        * From CApaApplication, creates CHsBrowserDocument document object.
        * @return CApaDocument*, A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
        
        /**
        * From CApaApplication, returns application's UID.
        * @return TUid, The value of KUidHsBrowserApp.
        */
        TUid AppDllUid() const;
    };

#endif

// End of File

