/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0""
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

// System includes

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include "qsysteminfo.h"

// User includes

#include "cpwlanentryitemdata.h"
#include "cpwlanentryplugin.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "cpwlanentrypluginTraces.h"
#endif

using namespace QtMobility;

/*!
    \class CpWlanEntryPlugin
    \brief CpWlanEntryPlugin implements the WLAN Status Control Panel Plugin
    that shows the current WLAN status.
*/

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
*/

CpWlanEntryPlugin::CpWlanEntryPlugin() : QObject()
{
    OstTraceFunctionEntry1(CPWLANENTRYPLUGIN_CPWLANENTRYPLUGIN_ENTRY, this);
    
    // Check & store WLAN dynamic configuration
    QtMobility::QSystemInfo sysinfo;
    mWlanSupported = sysinfo.hasFeatureSupported(QSystemInfo::WlanFeature);
    
    if (mWlanSupported) {
        // Install localization
        mTranslator = new QTranslator(this);
        QString lang = QLocale::system().name(); 
        QString path = "Z:/resource/qt/translations/"; 
        mTranslator->load("wlanentryplugin_" + lang, path); 
        qApp->installTranslator(mTranslator);
    }
    
    OstTraceFunctionExit1(CPWLANENTRYPLUGIN_CPWLANENTRYPLUGIN_EXIT, this);
}

/*!
    Destructor.
*/

CpWlanEntryPlugin::~CpWlanEntryPlugin()
{
    OstTraceFunctionEntry1(DUP1_CPWLANENTRYPLUGIN_CPWLANENTRYPLUGIN_ENTRY, this);
    OstTraceFunctionExit1(DUP1_CPWLANENTRYPLUGIN_CPWLANENTRYPLUGIN_EXIT, this);
}

/*!
    Plugin function for creating the entry plugin form item data.
*/

CpSettingFormItemData *CpWlanEntryPlugin::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    OstTraceFunctionEntry1(CPWLANENTRYPLUGIN_CREATESETTINGFORMITEMDATA_ENTRY, this);
    
    CpWlanEntryItemData *entryItemData = 0;
    
    // The plugin is disabled if WLAN is not supported by the product.
    if (mWlanSupported) {
        entryItemData = new CpWlanEntryItemData(itemDataHelper);
    }

    OstTraceFunctionExit1(CPWLANENTRYPLUGIN_CREATESETTINGFORMITEMDATA_EXIT, this);
    return entryItemData;
}

Q_EXPORT_PLUGIN2(cpwlanentryplugin, CpWlanEntryPlugin);
