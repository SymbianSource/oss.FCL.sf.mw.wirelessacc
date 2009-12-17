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
* Description:  Implementation of CWsfApplication.
*
*/


#include "wsfdocument.h"
#include "wsfapplication.h"

// UID for the application, this should correspond 
// to the uid defined in the mmp file
static const TUid KUidSnifferApp = {0x10281CAA};

// ---------------------------------------------------------
// CWsfApplication::CreateDocumentL()
// ---------------------------------------------------------
//
CApaDocument* CWsfApplication::CreateDocumentL()
    {  
    // Create an Sniffer document, and return a pointer to it
    CApaDocument* document = CWsfDocument::NewL( *this );
    return document;
    }

// ---------------------------------------------------------
// CWsfApplication::AppDllUid()
// ---------------------------------------------------------
//
TUid CWsfApplication::AppDllUid() const
    {
    // Return the UID for the Sniffer application
    return KUidSnifferApp;
    }

