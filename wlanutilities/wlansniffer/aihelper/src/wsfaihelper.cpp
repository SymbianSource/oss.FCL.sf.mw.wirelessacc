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
* Description:  Implementation of AiHelper entry point
*
*/


//  EXTERNAL INCLUDES
#include <eikstart.h>

//  INTERNAL INCLUDES
#include "wsfaihelperapplication.h"


// ---------------------------------------------------------------------------
// ::NewApplication
// ---------------------------------------------------------------------------
//
CApaApplication* NewApplication()
    {
    return new CWsfAiHelperApplication;
    }


// ---------------------------------------------------------------------------
// ::E32Main
// ---------------------------------------------------------------------------
//
TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }



