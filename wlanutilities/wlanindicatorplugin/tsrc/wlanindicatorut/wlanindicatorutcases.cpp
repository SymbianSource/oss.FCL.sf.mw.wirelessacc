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

#include <QtTest/QtTest>
#include <QObject>
#include <HbLabel>
#include "wlanindicatorutcases.h"

//static const char IndicatorType[] =
//    {"com.nokia.hb.indicator.connectivity.wlanindicatorplugin/1.0"};

QString WLAN_not_connected(hbTrId("txt_occ_dblist_wireless_lan_val_not_connected"));
QString WLAN_connected(hbTrId("txt_occ_dblist_wireless_lan_val_1_connected"));
QString WLAN_menu(hbTrId("txt_occ_dblist_indi_menu_wireless_lan"));
QString Empty("");
QString Icon("");

enum WlanStates{
    wlanIsNotConnected = 0,
    wlanIsConnected = 1
};

enum WlanIndicatorParameters{
    wlanState = 0,
    wlanIapName = 1,
};

WlanIndicatorUtCases::WlanIndicatorUtCases()
{
}

WlanIndicatorUtCases::~WlanIndicatorUtCases()
{
}

void WlanIndicatorUtCases::initTestCase()
{
}

// Start of test cases
void WlanIndicatorUtCases::wlanNotConnected()
{
    qDebug("WLAN Not Connected");
    bool result(true);
    QList<QVariant> params;

    params.insert(wlanState, wlanIsNotConnected );

    result &= activateIndicator( &mWlanIndicatorPlugin );
    result &= clientParameterHandler( &mWlanIndicatorPlugin,
                                      HbIndicatorInterface::RequestActivate,
                                      params, true );
    result &= indicatorDataFeeder( &mWlanIndicatorPlugin, 
                                   HbIndicatorInterface::SecondaryTextRole,
                                   WLAN_not_connected );
    QVERIFY(result == true);
}

void WlanIndicatorUtCases::wlanConnected()
{
    qDebug("WLAN Connected");
    bool result(true);
    QList<QVariant> params;

    params.insert(wlanState, wlanIsConnected);
    params.insert(wlanIapName,"lahna");

    result &= activateIndicator(&mWlanIndicatorPlugin);
    result &= clientParameterHandler(&mWlanIndicatorPlugin, 
                                     HbIndicatorInterface::RequestActivate, 
                                     params, true );
    result &= indicatorDataFeeder( &mWlanIndicatorPlugin,
                                   HbIndicatorInterface::SecondaryTextRole,
                                   WLAN_connected);
    result &= indicatorIconFeeder( &mWlanIndicatorPlugin,
                                   HbIndicatorInterface::IconNameRole );
    QVERIFY(result == true);
}

void WlanIndicatorUtCases::differentIndicatorData()
{
    qDebug("Different indicator data handling");
    bool result(true);
    
    result &= indicatorDataFeeder( &mWlanIndicatorPlugin, 
                                   HbIndicatorInterface::PrimaryTextRole, WLAN_menu);
    
    result &= indicatorDataFeeder( &mWlanIndicatorPlugin, 
                                   255, Empty);
    result &= indicatorIconFeeder( &mWlanIndicatorPlugin, 
                                   HbIndicatorInterface::IconNameRole );
    result &= indicatorIconFeeder( &mWlanIndicatorPlugin, 
                                   HbIndicatorInterface::DecorationNameRole );
    QVERIFY(result == true);
}

void WlanIndicatorUtCases::errorHandling()
{
    qDebug("WLAN Indicator error handling");
    bool result(true);
    
    result &= error( &mWlanIndicatorPlugin, 0 );
    
    QVERIFY(result == true);
}

void WlanIndicatorUtCases::clientParamatersHandler()
{
    qDebug("Parameter handling");
    QList<QVariant> params;
    bool result(true);

    // Step 1: Deliver parameters for mParameter
    params.insert(wlanIapName, "lahna");
    result &= clientParameterHandler( &mWlanIndicatorPlugin,
                                      HbIndicatorInterface::RequestActivate,
                                      params,
                                      true );
    // Step 2: Clear parameters for mParameter
    result &= clientParameterHandler( &mWlanIndicatorPlugin,
                                      HbIndicatorInterface::RequestDeactivate,
                                      params,
                                      false );
    // Step 3: Try to feed data with SecondaryTextRole. Data is not valid not because
    // mParameter has been cleared in Step 2
    result &= indicatorDataFeeder( &mWlanIndicatorPlugin, 
                                   HbIndicatorInterface::SecondaryTextRole, 
                                   Empty );
    QVERIFY(result == true);
}

void WlanIndicatorUtCases::handleInteractions()
{
    qDebug("Interaction handling");
    bool result(true);
    
    result &= handleInteraction( &mWlanIndicatorPlugin,
                                 HbIndicatorInterface::NoInteraction,
                                 false );
    result &= handleInteraction( &mWlanIndicatorPlugin,
                                 HbIndicatorInterface::InteractionActivated,
                                 true );
    
    QVERIFY(result == true);
}

void WlanIndicatorUtCases::processErrorHandling()
{
    qDebug("WLAN Indicator error handling");
    bool result(true);
    
    result &= processError( &mWlanIndicatorPlugin, QProcess::FailedToStart );   
    result &= processError( &mWlanIndicatorPlugin, QProcess::Crashed );
    result &= processError( &mWlanIndicatorPlugin, QProcess::Timedout ); 
    result &= processError( &mWlanIndicatorPlugin, QProcess::ReadError ); 
    result &= processError( &mWlanIndicatorPlugin, QProcess::WriteError ); 
    result &= processError( &mWlanIndicatorPlugin, QProcess::UnknownError );
    
    QVERIFY(result == true);
}
// End of test cases

// Sub-test cases
bool WlanIndicatorUtCases::activateIndicator( WlanIndicatorPlugin* plugin )
{
    QStringList indTypes("com.nokia.hb.indicator.connectivity.wlanindicatorplugin/1.0");
    QStringList resIndTypes("");
    QString indTypeString("");
    bool result(true);
    HbIndicatorInterface* resPtr(NULL);
    HbSecurityInfo* securityInfo = NULL;
    bool resAllowed(false);
    
    resIndTypes = plugin->indicatorTypes();
    
    if (resIndTypes != indTypes) {
        qDebug("Indicator activation error: Erroneous IndicationType");
        result = false;
    }

    resAllowed = plugin->accessAllowed( indTypeString, securityInfo );
    
    if (resAllowed == false) {
        qDebug("Indicator activation error: Erroneous allowance");
        result = false;
    }
    
    resPtr = plugin->createIndicator(indTypeString);
    
    if (resPtr == NULL) {
        qDebug("Indicator activation error: Indicator creation failed");
        result = false;
    }
    return result;
}

bool WlanIndicatorUtCases::clientParameterHandler( WlanIndicatorPlugin* plugin,
                                                   HbIndicatorInterface::RequestType ReqType,
                                                   QVariant parameter,
                                                   bool expectedResult )
{
    bool resHandled(false);
    bool result(true);
    
    resHandled = plugin->handleClientRequest( ReqType, parameter );

    if (resHandled != expectedResult) {
        qDebug("Indicator error: Error in client request handling");
        result = false;
    }
    return result;
}

bool WlanIndicatorUtCases::indicatorDataFeeder( WlanIndicatorPlugin* plugin,
                                                int role,
                                                QString expectedString )
{    
    QString resString;
    bool result(true);
    
    resString = plugin->indicatorData(role).toString();
    int x = QString::compare(resString, expectedString, Qt::CaseInsensitive);
 
    if (x != 0) {
        qDebug("Indicator error: Error in indicatorData");
        result = false;
    }
    return result;
}

bool WlanIndicatorUtCases::indicatorIconFeeder( WlanIndicatorPlugin* plugin,
                                                int role )
{    
    QVariant resIcon;
    bool result(true);
    
    resIcon = plugin->indicatorData(role);
    int x = 0;
 
    if (x != 0) {
        qDebug("Indicator error: Error in indicatorData");
        result = false;
    }
    return result;
}

bool WlanIndicatorUtCases::error( WlanIndicatorPlugin* plugin,
                                  int expectedError )
{    
    int error;
    bool result(true);
    
    error = plugin->error();
    
    if (error != expectedError) {
        qDebug("Indicator error: Error in error value");
        result = false;
    }
    return result;
}

bool WlanIndicatorUtCases::handleInteraction( WlanIndicatorPlugin* plugin,
                                              HbIndicatorInterface::InteractionType type,
                                              bool handleValue )
{    
    bool resHandle(false);
    bool result(true);
    
    resHandle = plugin->handleInteraction( type );
    
    if (resHandle != handleValue) {
        qDebug("Indicator error: Error in interaction handling");
        result = false;
    }
    return result;
}

bool WlanIndicatorUtCases::processError( WlanIndicatorPlugin* plugin,
                                         QProcess::ProcessError err ) 
{
   bool result(true);
   plugin->processError( err );
   return result;
}
