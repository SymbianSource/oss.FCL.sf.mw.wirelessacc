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
 * 
 */

#include "wlansnifferservice.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlansnifferserviceTraces.h"
#endif


WlanSnifferService::WlanSnifferService(QObject* parent)
    : XQServiceProvider("com.nokia.services.wlansniffer.list", parent)
{
    OstTraceFunctionEntry0( WLANSNIFFERSERVICE_WLANSNIFFERSERVICE_ENTRY );
    
    publishAll();

    OstTraceFunctionExit0( WLANSNIFFERSERVICE_WLANSNIFFERSERVICE_EXIT );
}

WlanSnifferService::~WlanSnifferService()
{
    OstTraceFunctionEntry0( WLANSNIFFERSERVICE_WLANSNIFFERSERVICEDESTR_ENTRY );
    OstTraceFunctionExit0( WLANSNIFFERSERVICE_WLANSNIFFERSERVICEDESTR_EXIT );
}

void WlanSnifferService::complete()
{
    OstTraceFunctionEntry0( WLANSNIFFERSERVICE_COMPLETE_ENTRY );
    
    QVariant returnValue(true);
    bool status = completeRequest(mAsyncRequestIndex, returnValue);
    Q_ASSERT(status);

    OstTraceFunctionExit0( WLANSNIFFERSERVICE_COMPLETE_EXIT );
}

void WlanSnifferService::listView()
{
    OstTraceFunctionEntry0( WLANSNIFFERSERVICE_LISTVIEW_ENTRY );
    
    mAsyncRequestIndex = setCurrentRequestAsync();
    emit toListView();

    OstTraceFunctionExit0( WLANSNIFFERSERVICE_LISTVIEW_EXIT );
}
