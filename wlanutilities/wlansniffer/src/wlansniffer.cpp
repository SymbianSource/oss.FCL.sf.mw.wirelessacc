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

#include <QTranslator>
#include <QLocale>

#include "wlanqtutils.h"

#include "wlansniffer.h"
#include "wlansniffermainwindow.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlansnifferTraces.h"
#endif

// Scan interval is 10 seconds
const int scanTimerInterval = 10000;

WlanSniffer::WlanSniffer(int argc, char* argv[]) :
    HbApplication(argc, argv),
    mWlanQtUtils(new WlanQtUtils())    
{
    OstTraceFunctionEntry0( WLANSNIFFER_WLANSNIFFER_ENTRY );
    
    // Start the first scan immediately so that results are available as
    // soon as possible. Start also the timer for periodic scanning.
    mWlanQtUtils->scanWlans();
    mScanTimerId = startTimer(scanTimerInterval);

    // Install localization
    mTranslator = new QTranslator(this);
    QString lang = QLocale::system().name(); 
    QString path = "Z:/resource/qt/translations/"; 
    mTranslator->load("wlansniffer_" + lang, path); 
    qApp->installTranslator(mTranslator);

    mMainWindow = new WlanSnifferMainWindow(this);
    
    connect(
        mWlanQtUtils,
        SIGNAL(wlanScanReady()),
        mMainWindow,
        SLOT(updateListView()));

    connect(
        mWlanQtUtils,
        SIGNAL(wlanNetworkOpened(int)),
        mMainWindow,
        SLOT(updateListViewConnectionOpened(int)));
    
    connect(
        mWlanQtUtils,
        SIGNAL(wlanNetworkClosed(int)),
        mMainWindow,
        SLOT(updateListViewConnectionClosed(int)));
    
    connect(
        mMainWindow,
        SIGNAL(exitTriggered()),
        this,
        SLOT(exitApplication()));

    OstTraceFunctionExit0( WLANSNIFFER_WLANSNIFFER_EXIT );
}

WlanSniffer::~WlanSniffer()
{
    OstTraceFunctionEntry0( WLANSNIFFER_WLANSNIFFERDESTR_ENTRY );

    killTimer(mScanTimerId);
    
    delete mMainWindow;
    delete mWlanQtUtils;
    
    OstTraceFunctionExit0( WLANSNIFFER_WLANSNIFFERDESTR_EXIT );
}

void WlanSniffer::timerEvent(QTimerEvent *event)
{
    OstTraceFunctionEntry0( WLANSNIFFER_TIMEREVENT_ENTRY );
    
    Q_UNUSED(event);
    // Request a new scan. Timer events come periodically.
    mWlanQtUtils->scanWlans();

    OstTraceFunctionExit0( WLANSNIFFER_TIMEREVENT_EXIT );
}

WlanQtUtils *WlanSniffer::wlanQtUtils() const
{
    OstTraceFunctionEntry0( WLANSNIFFER_WLANQTUTILS_ENTRY );
    OstTraceFunctionExit0( WLANSNIFFER_WLANQTUTILS_EXIT );
    return mWlanQtUtils;
}

void WlanSniffer::exitApplication()
{
    OstTraceFunctionEntry0( WLANSNIFFER_EXITAPPLICATION_ENTRY );
    
    exit();

    OstTraceFunctionExit0( WLANSNIFFER_EXITAPPLICATION_EXIT );
}
