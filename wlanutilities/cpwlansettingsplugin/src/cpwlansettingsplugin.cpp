/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QLocale>
#include <QCoreApplication>
#include <cpitemdatahelper.h>
#include <cpsettingformitemdata.h>

// User includes

#include "cpwlansettingsplugin.h"
#include "cpwlansettingsgroupitemdata.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "cpwlansettingspluginTraces.h"
#endif

/*!
    \class CpWlanSettingsPlugin
    \brief CpWlanSettingsPlugin implements the WLAN Settings Control Panel Plugin
    which will allow viewing/editing of Manage WLAN Settings.
*/

// External function prototypes

// Local constants

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
*/

CpWlanSettingsPlugin::CpWlanSettingsPlugin() :
    QObject()
{
    OstTraceFunctionEntry1(CPWLANSETTINGSPLUGIN_CPWLANSETTINGSPLUGIN_ENTRY, this);
    
    translator = new QTranslator(this);
    
    OstTraceFunctionExit1(CPWLANSETTINGSPLUGIN_CPWLANSETTINGSPLUGIN_EXIT, this);
}

/*!
    Destructor.
*/

CpWlanSettingsPlugin::~CpWlanSettingsPlugin()
{

    OstTraceFunctionEntry1(DUP1_CPWLANSETTINGSPLUGIN_CPWLANSETTINGSPLUGIN_ENTRY, this);
    OstTraceFunctionExit1(DUP1_CPWLANSETTINGSPLUGIN_CPWLANSETTINGSPLUGIN_EXIT, this);
}

/*!
    Plugin function for creating the WLAN Settings form item data.
*/

QList<CpSettingFormItemData*> CpWlanSettingsPlugin::createSettingFormItemData(
        CpItemDataHelper &itemDataHelper) const
{
    OstTraceFunctionEntry1(CPWLANSETTINGSPLUGIN_CREATESETTINGFORMITEMDATA_ENTRY, this);
    
    // Locale
    QString lang = QLocale::system().name(); 
    QString path = "Z:/resource/qt/translations/"; 
    translator->load("cpwlansettingsplugin_" + lang, path); 
    qApp->installTranslator(translator);

    QList<CpSettingFormItemData*> settingFormData;
    settingFormData.append(new CpWlanSettingsGroupItemData(itemDataHelper));
    
    OstTraceFunctionExit1(CPWLANSETTINGSPLUGIN_CREATESETTINGFORMITEMDATA_EXIT, this);
    return settingFormData;
    
}

Q_EXPORT_PLUGIN2(cpwlansettingsplugin, CpWlanSettingsPlugin);
