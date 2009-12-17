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
* Description: 
*      CWlanSettingsUiBinaryPopupSettingItem implementation.   
*      
*
*/


#include "wlansettingsuibinarypopupsettingitem.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanSettingsUiBinaryPopupSettingItem::CWlanSettingsUiBinaryPopupSettingItem
// ---------------------------------------------------------
//
CWlanSettingsUiBinaryPopupSettingItem::CWlanSettingsUiBinaryPopupSettingItem(
                        MWlanSettingsUiBinaryPopupSettingObserver& aObserver,
                        TInt aResourceId,
                        TBool& aValue )
: CAknBinaryPopupSettingItem( aResourceId, aValue ), 
  iObserver( aObserver )
    {
    }

// ---------------------------------------------------------
// CWlanSettingsUiBinaryPopupSettingItem::EditItemL
// ---------------------------------------------------------
//
void CWlanSettingsUiBinaryPopupSettingItem::EditItemL( TBool aCalledFromMenu )
    {
    // Since this is a binary setting, edit always means change.
    // (There is no popup to Cancel, the setting changes in place.)
    CAknBinaryPopupSettingItem::EditItemL( aCalledFromMenu );
    iObserver.BinarySettingChangedL( this, InternalValue() );
    }

