/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
* This is a source file for connmon library stub functions
*/

#include <ictsclientinterface.h>

#ifdef __WINS__

#ifdef WLANQTUTILS_T_WLANQTUTILS_UT_FLAG

// ----------------------------------------------
// Stub functions for ICT library, used in UT
// ----------------------------------------------

#include "wlanqtutilstestcontext.h"

extern WlanQtUtilsTestContext testContext;

// ----------------------------------------------------
// NewL is not stubbed -> parameters are not checked at all
// ----------------------------------------------------

/**
 * Empty implementation to prevent actual ICT from starting. Result callback is called directly
 * in the test cases.
 */
EXPORT_C void CIctsClientInterface::StartL()
    {
    }

#endif // WLANQTUTILS_T_WLANQTUTILS_UT_FLAG

// ----------------------------------------------------
// No stubs used in emulator
// ----------------------------------------------------
#endif // __WINS__
