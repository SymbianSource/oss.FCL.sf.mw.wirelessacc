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
*      Panic function and codes.   
*      
*
*/


#ifndef WLAN_SETTINGS_UI_PANIC_H
#define WLAN_SETTINGS_UI_PANIC_H

// TYPES

/**
* Panic reasons for WLAN Settings UI.
*/
enum TWlanSettingsUiPanicReason
    {
    EWlanSettingsUiBadSetting,          ///< Bad setting.
    EWlanSettingsUiBadControl,          ///< Bad control.
    EWlanSettingsUiTransactionUnderflow ///< Transaction begin/commit mismatch.
    };

// FUNCTION DECLARATIONS

/**
* Panic the thread.
* @param aReason Reason for the panic.
*/
void WlanSettingsUiPanic( TWlanSettingsUiPanicReason aReason );

#endif
