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
* Description:  ECOM proxy table for AiPlugin
*
*/


#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "wsfaiplugin.h"
#include "wsfaipluginuids.hrh"

// Provide a key pair value table for ECOM.
// Used to identify the correct construction function for 
// the requested interface.
const TImplementationProxy ImplementationTable[] =
{
    IMPLEMENTATION_PROXY_ENTRY( 
            AI_UID_ECOM_IMPLEMENTATION_CONTENTPUBLISHER_WIZARDPLUGIN,
            CWsfAiPlugin::NewL )
};


// --------------------------------------------------------------------------
// ImplementationGroupProxy
// Return an instance of the proxy table.
// --------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
                                                    TInt& aTableCount )
{
    aTableCount = sizeof( ImplementationTable ) / sizeof( 
                                                    TImplementationProxy );
    return ImplementationTable;
}

// End of File
