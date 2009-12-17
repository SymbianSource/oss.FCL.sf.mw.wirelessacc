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
* Description:   Implements the main application class.
*
*/



// INCLUDE FILES
#include <eikstart.h>

#include "hsbrowserapp.h"
#include "hsbrowserdocument.h"
#include "hsbrowsercommon.h"
#include "am_debug.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CHsBrowserApp::AppDllUid()
// Returns application UID
// ---------------------------------------------------------
//
TUid CHsBrowserApp::AppDllUid() const
    {
    return KUidHsBrowserApp;
    }

// ---------------------------------------------------------
// CDictionaryStore* CHsBrowserApp::OpenIniFileLC(RFs& aFs) const
// overrides CAknApplication::OpenIniFileLC to enable INI file support
// ---------------------------------------------------------
//
CDictionaryStore* CHsBrowserApp::OpenIniFileLC( RFs& aFs ) const
    {
    return CEikApplication::OpenIniFileLC( aFs );
    }
   
// ---------------------------------------------------------
// CHsBrowserApp::CreateDocumentL()
// Creates CHsBrowserAppDocument object
// ---------------------------------------------------------
//
CApaDocument* CHsBrowserApp::CreateDocumentL()
    {
    return CHsBrowserDocument::NewL( *this );
    }

// ================= OTHER EXPORTED FUNCTIONS ==============
//
// ---------------------------------------------------------
// NewApplication() 
// Constructs CHsBrowserApp
// Returns: created application object
// ---------------------------------------------------------
//
LOCAL_C CApaApplication* NewApplication( )
	{
    return new CHsBrowserApp;
    }
    
// ---------------------------------------------------------
//    E32Main
//    Main function for Symbian OS v9 EXE application
//    Returns: TInt
// ---------------------------------------------------------
GLDEF_C TInt E32Main()
    {
	DEBUG( "***** Starting Hotspot Browser Logon Application..." );
    return EikStart::RunApplication( NewApplication );
    }

// End of File  
