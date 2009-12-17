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
* Description:  Implementation of E32Main.
*
*/

#include <eikstart.h>
#include "wsfapplication.h"


// ---------------------------------------------------------------------------
// NewApplication
// Create an application, and return a pointer to it
// ---------------------------------------------------------------------------
//
CApaApplication* NewApplication()
    {
    return new CWsfApplication;
    }

// ---------------------------------------------------------------------------
// E32Main
// ---------------------------------------------------------------------------
//
TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }



