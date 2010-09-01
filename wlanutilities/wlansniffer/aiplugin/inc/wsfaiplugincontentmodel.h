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
* Description:  Content enums
*
*/


#ifndef WSFAIPLUGINCONTENTMODEL_H
#define WSFAIPLUGINCONTENTMODEL_H


#include <aicontentmodel.h>

#include "wsfaipluginuids.hrh"

// Plug-in Implementation UID.
const TInt KImplUidWlanWizardPlugin = 
                AI_UID_ECOM_IMPLEMENTATION_CONTENTPUBLISHER_WIZARDPLUGIN;
const TUid KUidWlanWizardPlugin = { KImplUidWlanWizardPlugin };


// ================================= Content ================================

/**
 * Content Ids
 */
enum TWsfAiWizardContentIds
    {
     EAiWizardContentStatusIcon
    ,EAiWizardContentStrengthIcon   
    ,EAiWizardContentSecureIcon
    ,EAiWizardContentNetworkStatus
    ,EAiWizardContentNetworkName
    };

/**
 * Content that the plugin will publish
 */
const TAiContentItem KAiWizardContent[] =
    {
     { EAiWizardContentStatusIcon,    L"StatusIcon",   KAiContentTypeBitmap }
    ,{ EAiWizardContentStrengthIcon,  L"StrengthIcon", KAiContentTypeBitmap }
    ,{ EAiWizardContentSecureIcon,    L"SecureIcon",   KAiContentTypeBitmap }
    ,{ EAiWizardContentNetworkStatus, L"NetworkStatus",KAiContentTypeText }
    ,{ EAiWizardContentNetworkName,   L"NetworkName",  KAiContentTypeText }
    };

// ============================ Events ==============================
/**
 * Event Ids.
 */
enum TWsfAiWizardEventIds
    {
     EAiWizardEventLaunchApplication,
     EAiWizardGainFocusInfoArea,
     EAiWizardLostFocusInfoArea,     
     EAiWizardOpenMenu
    };

/**
 * Services that the plugin can perform
 */
const TAiContentItem KAiWizardEvents[] =
    {
     { EAiWizardEventLaunchApplication,         L"LaunchApplication", "" }
    ,{ EAiWizardGainFocusInfoArea,              L"GainFocusInfoArea", "" }
    ,{ EAiWizardLostFocusInfoArea,              L"LostFocusInfoArea", "" }
    ,{ EAiWizardOpenMenu,                       L"OpenMenu", "" }
    };

#endif // WSFAIPLUGINCONTENTMODEL_H
