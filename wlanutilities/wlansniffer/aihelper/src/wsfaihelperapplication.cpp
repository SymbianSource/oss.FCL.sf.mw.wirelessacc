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
* Description:  Implementation of CWsfAiHelperApplication
*
*/


#include "wsfaihelperdocument.h"
#include "wsfaihelperapplication.h"


/**
 * UID for the application
 */
static const TUid KUidSnifferAiHelperApp = { 0x10281CEB };

// ---------------------------------------------------------------------------
// CWsfAiHelperApplication::CreateDocumentL
// ---------------------------------------------------------------------------
//
CApaDocument* CWsfAiHelperApplication::CreateDocumentL()
    {  
    // Create an Sniffer document, and return a pointer to it
    CApaDocument* document = CWsfAiHelperDocument::NewL( *this );
    return document;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperApplication::AppDllUid
// ---------------------------------------------------------------------------
//
TUid CWsfAiHelperApplication::AppDllUid() const
    {
    // Return the UID for the Sniffer application
    return KUidSnifferAiHelperApp;
    }

