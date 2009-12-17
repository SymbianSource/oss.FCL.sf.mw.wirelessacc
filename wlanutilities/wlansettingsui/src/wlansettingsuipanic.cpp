/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*      Implementation of panic function.   
*      
*
*/


// INCLUDE FILES

#include <e32std.h>
#include "wlansettingsuipanic.h"

// ================= LOCAL FUNCTIONS =======================

// ---------------------------------------------------------
// WlanSettingsUiPanic()
// ---------------------------------------------------------
//
void WlanSettingsUiPanic( TWlanSettingsUiPanicReason aReason )
    {
    _LIT( KWlanSettingsUi, "WLAN Settings UI" );
    User::Panic( KWlanSettingsUi, aReason );
    }
