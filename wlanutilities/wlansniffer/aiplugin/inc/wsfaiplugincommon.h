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
* Description:  PluginIcons
*
*/


#ifndef WSFAIPLUGINCOMMON_H
#define WSFAIPLUGINCOMMON_H

#include <data_caging_path_literals.hrh>
#include <e32base.h>

// the drive letter
_LIT( KResourceDrive, "z:" );
// resource file
_LIT( KResourceFile, "wsfaipluginrsc.r*" );
// icons
_LIT( KBitmapFile, "wsficons.mif" );

/** Icon ID constants */
enum TWsfPluginIcons
    {
    //Constants
    ESecureNetworkIcon,
    ENoSignalIcon,
    EPoorSignal,
    EAverageSignal,
    EExcelentSignal,
    EConnectedNWIcon,
    EKnownNWIcon,
    ETransparentIcon,
    EWlanOnIcon,
    EWlanOnIcon_0,
    EWlanOnIcon_1,
    EWlanOffIcon,
    EWlanNaviBarIcon
    };
    
#endif // WSFAIPLUGINCOMMON_H
