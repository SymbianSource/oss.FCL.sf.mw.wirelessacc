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

#include <xqserviceutil.h>

#include "wlansniffermainwindow.h"
#include "wlansnifferlistview.h"
#include "wlansnifferservice.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlansniffermainwindowTraces.h"
#endif


WlanSnifferMainWindow::WlanSnifferMainWindow(WlanSniffer *appRef) :
    mAppRef(appRef),
    mService(0)
{
    OstTraceFunctionEntry0( WLANSNIFFERMAINWINDOW_WLANSNIFFERMAINWINDOW_ENTRY );
    
    mService = new WlanSnifferService(this);
    connect(mService,SIGNAL(toListView()),this,SLOT(toListView()));
    connect(mService,SIGNAL(returnValueDelivered()),this,SIGNAL(exitTriggered()));

    // Add the views to the main window
    addListView();
    
    // Check if sniffer is used via a service
    if (!XQServiceUtil::isEmbedded()) {
        // Show the main window (which will display the first view that was added)
        show();
    }

    OstTraceFunctionExit0( WLANSNIFFERMAINWINDOW_WLANSNIFFERMAINWINDOW_EXIT );
}

WlanSnifferMainWindow::~WlanSnifferMainWindow()
{
    OstTraceFunctionEntry0( WLANSNIFFERMAINWINDOW_WLANSNIFFERMAINWINDOWDESTR_ENTRY );
    OstTraceFunctionExit0( WLANSNIFFERMAINWINDOW_WLANSNIFFERMAINWINDOWDESTR_EXIT );
}

void WlanSnifferMainWindow::toListView()
{
    OstTraceFunctionEntry0( WLANSNIFFERMAINWINDOW_TOLISTVIEW_ENTRY );
    
    setCurrentView(mListView);
    show();

    OstTraceFunctionExit0( WLANSNIFFERMAINWINDOW_TOLISTVIEW_EXIT );
}

void WlanSnifferMainWindow::toDetailsView(int iapId)
{
    OstTraceFunctionEntry0( WLANSNIFFERMAINWINDOW_TODETAILSVIEW_ENTRY );
    
    // TODO: Launch details view via QtHighway??
    (void)iapId;

    OstTraceFunctionExit0( WLANSNIFFERMAINWINDOW_TODETAILSVIEW_EXIT );
}

void WlanSnifferMainWindow::updateListView()
{
    OstTraceFunctionEntry0( WLANSNIFFERMAINWINDOW_UPDATELISTVIEW_ENTRY );
    
    mListView->update();

    OstTraceFunctionExit0( WLANSNIFFERMAINWINDOW_UPDATELISTVIEW_EXIT );
}

void WlanSnifferMainWindow::updateListViewConnectionOpened(int iapId)
{
    OstTraceFunctionEntry0( WLANSNIFFERMAINWINDOW_UPDATELISTVIEWCONNECTIONOPENED_ENTRY );
    
    mListView->updateConnectionOpened(iapId);

    OstTraceFunctionExit0( WLANSNIFFERMAINWINDOW_UPDATELISTVIEWCONNECTIONOPENED_EXIT );
}

void WlanSnifferMainWindow::updateListViewConnectionClosed(int iapId)
{
    OstTraceFunctionEntry0( WLANSNIFFERMAINWINDOW_UPDATELISTVIEWCONNECTIONCLOSED_ENTRY );
    
    mListView->updateConnectionClosed(iapId);

    OstTraceFunctionExit0( WLANSNIFFERMAINWINDOW_UPDATELISTVIEWCONNECTIONCLOSED_EXIT );
}

void WlanSnifferMainWindow::completeService()
{
    OstTraceFunctionEntry0( WLANSNIFFERMAINWINDOW_COMPLETESERVICE_ENTRY );
    
    mService->complete();

    OstTraceFunctionExit0( WLANSNIFFERMAINWINDOW_COMPLETESERVICE_EXIT );
}

void WlanSnifferMainWindow::addListView()
{
    OstTraceFunctionEntry0( WLANSNIFFERMAINWINDOW_ADDLISTVIEW_ENTRY );
    
    mListView = new WlanSnifferListView(mAppRef);
    addView(mListView);
    
    bool connectStatus = connect(
        mListView,
        SIGNAL(detailsTriggered(int)),
        this,
        SLOT(toDetailsView(int)));
    Q_ASSERT(connectStatus == true);
    connectStatus = connect(
        mListView,
        SIGNAL(completeServiceTriggered()),
        this,
        SLOT(completeService()));
    Q_ASSERT(connectStatus == true);

    OstTraceFunctionExit0( WLANSNIFFERMAINWINDOW_ADDLISTVIEW_EXIT );
}
