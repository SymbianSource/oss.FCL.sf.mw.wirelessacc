/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the source file for testing Wlan Qt Utilities library.
*/

#include <QApplication>
#include <QtCore>
#include <QTest>
#include <QSignalSpy>
#include <QSharedPointer>
#include <rconnmon.h>
#include <nifvar.h>
#include <cmmanagerdefines_shim.h>

#include "wlanqtutilsap.h"
#include "wlanqtutilsiap.h"
#include "wlanqtutils.h"
#include "wlanqtutils_p.h"
#include "wlanqtutilsconmonwrapper.h"
#include "wlanqtutilsconmonwrapperinfo_s60_p.h"
#include "wlanqtutilsconntestwrapper.h"
#include "wlanqtutilsconntestwrapper_s60_p.h"
#include "testwlanqtutils.h"
#include "wlanqtutilstestcontext.h"

WlanQtUtilsTestContext testContext;

const QString TestWlanQtUtils::commsDatDefault_ = "default.cre";

// ---------------------------------------------------------
// FRAMEWORK FUNCTIONS
// ---------------------------------------------------------

/**
 * Test main function. Runs all test cases.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\data\\wlanqtutils_qtest_log.txt";
 
    TestWlanQtUtils tc;
    int res = QTest::qExec(&tc, 3, pass);
 
    return res;
}

/**
 * This function will be called before the first test function is executed.
 */
void TestWlanQtUtils::initTestCase()
{
    wlanQtUtils_ = NULL;
    signalScanReady_ = NULL;
    signalWlanNetworkOpened_ = NULL;
    signalWlanNetworkClosed_ = NULL;
    signalIctResult_ = NULL;

    //If Active scheduler exists then don't install a new one as it will cause panic
    if (CActiveScheduler::Current() == NULL) {
        CActiveScheduler *scheduler = new CActiveScheduler();
        CActiveScheduler::Install(scheduler);
    }
}

/**
 * This function will be called after the last test function was executed.
 */
void TestWlanQtUtils::cleanupTestCase()
{
    // CommsDat file is initialized.
    subTestLoadCommsDatFile(commsDatDefault_);
}

/**
 * This function will be called before each test function is executed.
 */
void TestWlanQtUtils::init()
{
    // CommsDat file is initialized.
    subTestLoadCommsDatFile(commsDatDefault_);

    testContext.initialize();

    QVERIFY(wlanQtUtils_ == NULL);
    QVERIFY(signalScanReady_ == NULL);
    QVERIFY(signalWlanNetworkOpened_ == NULL);
    QVERIFY(signalWlanNetworkClosed_ == NULL);
    QVERIFY(signalIctResult_ == NULL);
    subTestNewWlanQtUtils();
}

/**
 * This function will be called after each test function is executed.
 */
void TestWlanQtUtils::cleanup()
{
    delete wlanQtUtils_;
    wlanQtUtils_ = NULL;

    QCOMPARE(signalScanReady_->count(), 0);
    delete signalScanReady_;
    signalScanReady_ = NULL;

    QCOMPARE(signalWlanNetworkOpened_->count(), 0);
    delete signalWlanNetworkOpened_;
    signalWlanNetworkOpened_ = NULL;

    QCOMPARE(signalWlanNetworkClosed_->count(), 0);
    delete signalWlanNetworkClosed_;
    signalWlanNetworkClosed_ = NULL;

    QCOMPARE(signalIctResult_->count(), 0);
    delete signalIctResult_;
    signalIctResult_ = NULL;
}

// ---------------------------------------------------------
// TEST CASES
// ---------------------------------------------------------

/**
 * Test available WLAN APs when scan is triggered by client.
 */
void TestWlanQtUtils::testAvailableWlan()
{
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(4);
    
    // Request a scan to get result signal
    wlanQtUtils_->scanWlans();
    
    // Let active object run and verify signal.
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get and verify the list of available WLAN APs.
    QList< QSharedPointer<WlanQtUtilsIap> > iaps;
    QList< QSharedPointer<WlanQtUtilsAp> > aps;
    wlanQtUtils_->availableWlans(iaps, aps);

    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear();
}

/**
 * Test available WLAN APs when scan is triggered by client.
 * Two scan results have the same SSID and the latter one of those will be removed.
 * Two scan results have the same SSID but different security mode
 * Two scan results have the same SSID and security mode but different WpaPsk value
 * and both are included in the results.
 */
void TestWlanQtUtils::testAvailableWlanDuplicates()
{
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(10);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[0]->setValue(WlanQtUtilsAp::ConfIdSsid, "Same SSID");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[2]->setValue(WlanQtUtilsAp::ConfIdSsid, "Same SSID");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[5]->setValue(WlanQtUtilsAp::ConfIdSsid, "Same SSID, different SecMode");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[5]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[6]->setValue(WlanQtUtilsAp::ConfIdSsid, "Same SSID, different SecMode");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[6]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeOpen);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[7]->setValue(WlanQtUtilsAp::ConfIdSsid, "Same SSID & SecMode diff Psk");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[7]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa2);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[9]->setValue(WlanQtUtilsAp::ConfIdSsid, "Same SSID & SecMode diff Psk");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[9]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa2);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[9]->setValue(WlanQtUtilsAp::ConfIdWpaPskUse,true);

    // Request a scan to get result signal
    wlanQtUtils_->scanWlans();
    
    // Let active object run and verify signal.
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get and verify the list of available WLAN APs.
    QList< QSharedPointer<WlanQtUtilsIap> > iaps;
    QList< QSharedPointer<WlanQtUtilsAp> > aps;
    wlanQtUtils_->availableWlans(iaps, aps);

    // First remove AP that has duplicate SSID.
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.removeAt(2);
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear();
}

/**
 * Test available WLAN APs when scan is triggered by client.
 * Include all supported security modes.
 */
void TestWlanQtUtils::testAvailableWlanSecModes()
{
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(9);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[0]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeOpen);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[0]->setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Adhoc);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[1]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWep);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[2]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecMode802_1x);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[3]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[4]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[4]->setValue(WlanQtUtilsAp::ConfIdWpaPskUse,true);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[5]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa2);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[6]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa2);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[6]->setValue(WlanQtUtilsAp::ConfIdWpaPskUse,true);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[7]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWapi);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[8]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWapi);
    // Let's trick the stub to return WapiPsk by putting WpaPskUse on *
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[8]->setValue(WlanQtUtilsAp::ConfIdWpaPskUse,true);

    // Request a scan to get result signal
    wlanQtUtils_->scanWlans();
    
    // Let active object run and verify signal.
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get and verify the list of available WLAN APs.
    QList< QSharedPointer<WlanQtUtilsIap> > iaps;
    QList< QSharedPointer<WlanQtUtilsAp> > aps;
    wlanQtUtils_->availableWlans(iaps, aps);

    // Remove the trick *. WapiPsk and Wapi both map to just Wapi
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[8]->setValue(WlanQtUtilsAp::ConfIdWpaPskUse,false);
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear();
}

/**
 * Test available WLAN APs when scan is triggered by client.
 * Include some exotic ssids.
 */
void TestWlanQtUtils::testAvailableWlanSsids()
{
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(6);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[1]->setValue(WlanQtUtilsAp::ConfIdSsid, "           ");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[2]->setValue(WlanQtUtilsAp::ConfIdSsid, "    whitespace around ssid  ");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[3]->setValue(WlanQtUtilsAp::ConfIdSsid, QString(QChar()));
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[4]->setValue(WlanQtUtilsAp::ConfIdSsid, "maximum length ssid that is 32 c");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[5]->setValue(WlanQtUtilsAp::ConfIdSsid, "A");

    // Request a scan to get result signal
    wlanQtUtils_->scanWlans();
    
    // Let active object run and verify signal.
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get and verify the list of available WLAN APs.
    QList< QSharedPointer<WlanQtUtilsIap> > iaps;
    QList< QSharedPointer<WlanQtUtilsAp> > aps;
    wlanQtUtils_->availableWlans(iaps, aps);

    // Empty ssids are removed from results
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.removeAt(3);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.removeAt(1);
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear();
}

/**
 * Test available WLAN APs when there are also WLAN IAPs available.
 */
void TestWlanQtUtils::testAvailableWlanWithIaps()
{
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(7);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[1]->setValue(WlanQtUtilsAp::ConfIdSsid, "WLAN IAP 3");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[1]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa);    
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[1]->setValue(WlanQtUtilsAp::ConfIdWpaPskUse,true);    
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[5]->setValue(WlanQtUtilsAp::ConfIdSsid, "WLAN IAP 1");
    // SSID is found in Internet Snap, but security mode does not match:
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[6]->setValue(WlanQtUtilsAp::ConfIdSsid, "WLAN IAP 2");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[6]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa);
    
    // Request a scan to get result signal
    wlanQtUtils_->scanWlans();
    
    // Let active object run and verify signal.
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get and verify the list of available WLAN APs.
    QList< QSharedPointer<WlanQtUtilsIap> > iaps;
    QList< QSharedPointer<WlanQtUtilsAp> > aps;
    wlanQtUtils_->availableWlans(iaps, aps);

    // Verify WLAN AP list. First, remove scan results that will not appear
    // because they are WLAN IAPs.
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.removeAt(5);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.removeAt(1);
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);

    // Verify WLAN IAP list
    QCOMPARE(iaps.count(), 2);
    QCOMPARE(iaps[0]->value(WlanQtUtilsIap::ConfIdIapId).toInt(), 5);
    QCOMPARE(iaps[0]->value(WlanQtUtilsIap::ConfIdName).toString(), QString("WLAN IAP 1"));
    QCOMPARE(iaps[0]->value(WlanQtUtilsAp::ConfIdSsid).toString(), QString("WLAN IAP 1"));
    QCOMPARE(iaps[1]->value(WlanQtUtilsIap::ConfIdIapId).toInt(), 7);
    QCOMPARE(iaps[1]->value(WlanQtUtilsIap::ConfIdName).toString(), QString("WLAN IAP 3"));
    QCOMPARE(iaps[1]->value(WlanQtUtilsAp::ConfIdSsid).toString(), QString("WLAN IAP 3"));
    iaps.clear();
    aps.clear();
}

/**
 * Test refereshing of WLAN APs when client requests sequential scans.
 */
void TestWlanQtUtils::testAvailableWlanSequence()
{
    // **************** Before 1st scan ********************
    QList< QSharedPointer<WlanQtUtilsIap> > iaps;
    QList< QSharedPointer<WlanQtUtilsAp> > aps;
    wlanQtUtils_->availableWlans(iaps, aps);
    // Verify we have no results
    QCOMPARE(aps.count(), 0);
    QCOMPARE(iaps.count(), 0);
    
    // **************** 1st scan ********************
    // 6 APs are required for this scan
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(6);

    wlanQtUtils_->scanWlans();
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get the results for the 1st scan
    wlanQtUtils_->availableWlans(iaps, aps);
    // Verify the results for the scan
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear();

    // **************** 2nd scan ********************
    // 2 APs are required for this scan
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(2);

    wlanQtUtils_->scanWlans();
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get the results for the 2nd scan
    wlanQtUtils_->availableWlans(iaps, aps);
    // Verify the results for the scan
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear();

    // **************** 3rd scan ********************
    // 4 APs are required for the 3rd scan
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(4);

    wlanQtUtils_->scanWlans();
    subTestSignalWaitAndTake(signalScanReady_, NULL);
    
    // Get the results for the 3rd scan
    wlanQtUtils_->availableWlans(iaps, aps);
    // Verify the results for the scan
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear();
}

/**
 * This function tests creation of WLAN IAP in a succesful manner.
 * - WLAN scan is made because otherwise we cannot verify that IAP creation is successful.
 * - Check that there are no available WLAN IAPs.
 * - Fetch SNAP list.
 * - Create WLAN IAP.
 * - Check that WLAN IAP has been created and that this IAP is not in WLAN AP scan results.
 */
void TestWlanQtUtils::testCreateIapOk()
{
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(1);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[0]->setValue(WlanQtUtilsAp::ConfIdSsid, "testCreateIapOk");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[0]->setValue(WlanQtUtilsAp::ConfIdSecurityMode,CMManagerShim::WlanSecModeWpa);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[0]->setValue(WlanQtUtilsAp::ConfIdWpaPskUse,true);

    // Request a scan to get result signal
    wlanQtUtils_->scanWlans();
    
    // Let active object run and verify signal.
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Ensure there are no WLAN IAPs but there is one scan result.
    QList< QSharedPointer<WlanQtUtilsIap> > iaps;
    QList< QSharedPointer<WlanQtUtilsAp> > aps;
    wlanQtUtils_->availableWlans(iaps, aps);

    QCOMPARE(iaps.count(), 0);
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);

    // Execute createIap() function
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testCreateIapOk");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90 );
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeWpa);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, true);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPsk, "100euronlounas");
    int iapId = wlanQtUtils_->createIap(&wlanAp);
    QVERIFY(iapId != WlanQtUtils::IapIdNone);

    // Verify that created IAP is in the IAP list and AP list is empty.
    wlanQtUtils_->availableWlans(iaps, aps);
    QCOMPARE(iaps.count(), 1);
    QCOMPARE(iaps[0]->value(WlanQtUtilsIap::ConfIdIapId).toInt(), iapId);
    QCOMPARE(iaps[0]->value(WlanQtUtilsIap::ConfIdName).toString(), QString("testCreateIapOk"));
    QCOMPARE(iaps[0]->value(WlanQtUtilsAp::ConfIdSsid).toString(), QString("testCreateIapOk"));
    QCOMPARE(iaps[0]->value(WlanQtUtilsAp::ConfIdSecurityMode).toInt(), (int)(CMManagerShim::WlanSecModeWpa));
    QCOMPARE(iaps[0]->value(WlanQtUtilsAp::ConfIdWpaPskUse).toBool(), true);
    QCOMPARE(aps.count(), 0);
    iaps.clear();
}

/**
 * This function tests creation of WLAN IAP in unsuccesful manner.
 */
void TestWlanQtUtils::testCreateIapErr()
{
    // Execute createIap() function with invalid parameters
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testCreateIapErr");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90 );
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);
    // Invalid security mode -> exception
    wlanAp.setValue(
        WlanQtUtilsAp::ConfIdSecurityMode,
        CMManager::EWlanSecModeWAPI + 1111);
    int iapId = wlanQtUtils_->createIap(&wlanAp);
    QVERIFY(iapId == WlanQtUtils::IapIdNone);
}

/**
 * This function tests creation of WLAN IAPs with different WEP keys.
 */
void TestWlanQtUtils::testCreateIapWepKeys()
{
    // Create the IAPs with different kind of WEP keys
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testCreateIapWepKeys");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeWep);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepDefaultIndex, 1);
    // All keys in HEX format
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey1, "1234567891");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey2, "1234567891");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey3, "1234567891");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey4, "1234567891");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);

    int iapId = wlanQtUtils_->createIap(&wlanAp);
    QVERIFY(iapId != WlanQtUtils::IapIdNone);
    
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepDefaultIndex, 2);
    // All keys in ASCII format
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey1, "12345");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey2, "12345");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey3, "12345");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey4, "12345");

    iapId = wlanQtUtils_->createIap(&wlanAp);
    QVERIFY(iapId != WlanQtUtils::IapIdNone);

    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepDefaultIndex, 3);
    // Different keys, including a missing one
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey1, "");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey2, "12345678911234567892123456");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey3, "12345");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey4, "1234567891123");

    iapId = wlanQtUtils_->createIap(&wlanAp);
    QVERIFY(iapId != WlanQtUtils::IapIdNone);
}

/**
 * This function tests updating of WLAN IAP in a succesful manner.
 */
void TestWlanQtUtils::testUpdateIapOk()
{
    // Create an IAP that can be updated
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testUpdateIapOk");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeOpen);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);
    int iapId = wlanQtUtils_->createIap(&wlanAp);

    // Update the name and verify it changed
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "UPDATED_testUpdateIapOk");

    bool success = wlanQtUtils_->updateIap(iapId, &wlanAp);
    QVERIFY(success == true);
    QString name = wlanQtUtils_->iapName(iapId);
    QCOMPARE(name, QString("UPDATED_testUpdateIapOk"));
}

/**
 * This function tests updating of WLAN IAP in unsuccesful manner.
 */
void TestWlanQtUtils::testUpdateIapErr()
{
    // Try to update a non-existing IAP
    WlanQtUtilsAp wlanAp;
    bool success = wlanQtUtils_->updateIap(200, &wlanAp);
    QVERIFY(success == false);
}

/**
 * This function tests deletion of WLAN IAP in a succesful manner.
 */
void TestWlanQtUtils::testDeleteIapOk()
{
    // Create an IAP that can be deleted
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testDeleteIapOk");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90 );
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeOpen);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);
    int iapId = wlanQtUtils_->createIap(&wlanAp);

    // Check that IAP now exists
    QString name = wlanQtUtils_->iapName(iapId);
    QCOMPARE(name, QString("testDeleteIapOk"));
    
    // Delete it and verify it does not exist anymore
    wlanQtUtils_->deleteIap(iapId);
    name = wlanQtUtils_->iapName(iapId);
    QVERIFY(name.isEmpty());
}

/**
 * This function tests deletion of WLAN IAP in unsuccesful manner.
 */
void TestWlanQtUtils::testDeleteIapErr()
{
    // Try to delete a non-existing IAP
    wlanQtUtils_->deleteIap(200);

    // No signals or return values to verify
}

/**
 * This function tests connecting to IAP in a succesful manner.
 */
void TestWlanQtUtils::testConnectIapOk()
{
    testContext.esock_.startRetValue_ = KErrNone;

    // Esock stub completes connection creation immediately
    wlanQtUtils_->connectIap(5);

    // Connection creation in ConnMon interface
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 5);
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
        EConnMonCreateConnection,
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Connection status change in ConnMon interface
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    
    // Receive signal for connection opening (caused by connectIap, which completed immediately)
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), 5); 

    // Connection status change to opened in ConnMon interface. Sub test cases between test
    // cases check that no extra signals are sent
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));
}

/**
 * This function tests connecting to IAP in unsuccesful manner.
 */
void TestWlanQtUtils::testConnectIapErr()
{
    testContext.esock_.startRetValue_ = KErrGeneral;

    wlanQtUtils_->connectIap(7);

    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 7);
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonDeleteConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));
    
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkClosed_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), 7);
    QCOMPARE(arguments.at(1).toInt(), KErrGeneral);
}

// TODO: testDisconnectFail cases are needed when fail branches are
// implemented into the connmon wrapper
/**
 * This function tests IAP disconnecting functionality.
 */
void TestWlanQtUtils::testDisconnectIapOk()
{
    // Create and connect an IAP we can then disconnect
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testDisconnectIapOk");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90 );
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeWpa);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);

    int iapId = wlanQtUtils_->createIap(&wlanAp);
    testContext.esock_.startRetValue_ = KErrNone;
    wlanQtUtils_->connectIap(iapId);
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), iapId); 
    
    // Make sure there is the newly created connection active and also
    // another one to gain coverage
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(2, iapId - 1);
    // The disconnect function does not have any return values or
    // signals related to the disconnection, thus plain
    // function call is done for the test.
    wlanQtUtils_->disconnectIap(iapId);
}

/**
 * This function tests IAP disconnecting functionality when there is no
 * IAP to disconnect.
 */
void TestWlanQtUtils::testDisconnectIapIgnored()
{
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 150);
    wlanQtUtils_->disconnectIap(200);
    wlanQtUtils_->disconnectIap(WlanQtUtils::IapIdNone);
}

/**
 * This function tests connection status getter.
 */
void TestWlanQtUtils::testConnectionStatus()
{
    // Request status when there is no connection
    WlanQtUtils::ConnStatus status = wlanQtUtils_->connectionStatus();
    QCOMPARE(status, WlanQtUtils::ConnStatusDisconnected);
    
    // Make a connection and request its status
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 5);
    // Send event for connection creation.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));
    // Send events for connection status change -> connecting
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KStartingConnection));

    // Request status when there is connection in connecting state
    status = wlanQtUtils_->connectionStatus();
    QCOMPARE(status, WlanQtUtils::ConnStatusConnecting);

    // Send events for connection status change -> connected
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments);
    QCOMPARE(arguments.at(0).toInt(), 5);

    // Request status when there is connection in connected state
    status = wlanQtUtils_->connectionStatus();
    QCOMPARE(status, WlanQtUtils::ConnStatusConnected);
}

/**
 * This function tests IAP name getter with existing IAP ID.
 */
void TestWlanQtUtils::testIapNameFound()
{
    // Create the IAP we want to find with the getter
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testIapFound");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90 );
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeWpa2);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);

    int iapId = wlanQtUtils_->createIap(&wlanAp);
    
    // Execute the function under test and check that we got the correct name
    QString name = wlanQtUtils_->iapName(iapId);
    QCOMPARE(name, QString("testIapFound"));
}

/**
 * This function tests IAP name getter with non-existing IAP ID.
 */
void TestWlanQtUtils::testIapNameNotFound()
{
    // Execute the function under test and check that we get no name as result
    QString name = wlanQtUtils_->iapName(200); // id in valid range, but not found -> KErrNotFound
    QVERIFY(name.isEmpty());

    name = wlanQtUtils_->iapName(1000); // id not in valid range -> KErrArgument
    QVERIFY(name.isEmpty());

    name = wlanQtUtils_->iapName(3); // id of cellular IAP -> discarded
    QVERIFY(name.isEmpty());
}

/**
 * This function tests active WLAN IAP getter with existing connection.
 */
void TestWlanQtUtils::testActiveIapFound()
{
    // Create the IAP we want to find with the getter
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testConnectedWlanIdFound");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90 );
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeWpa);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);

    int iapId = wlanQtUtils_->createIap(&wlanAp);
    
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, iapId);

    // Send event for connection creation.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Send events for connection status change -> opened.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KStartingConnection));
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments);
    QCOMPARE(arguments.at(0).toInt(), iapId);
        
    // Execute the function under test and check that we get valid ID as result
    int id = wlanQtUtils_->activeIap();
    QCOMPARE(id, iapId);
}

/**
 * This function tests active WLAN IAP getter with connection existing already during dll construction.
 */
void TestWlanQtUtils::testActiveIapFoundConstructor()
{
    // IAP IDs 4 and 5 exist in default commsdat file, 4 is GPRS, 5 is WLAN
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(2, 4);
    testContext.connMon_.activeConnections_.activeConnList_[0]->connMonBearerType_ = EBearerGPRS;

    // Create a new instance in order to test functionality triggered in constructor.
    WlanQtUtils *utils = new WlanQtUtils();

    // Execute the function under test and check that we get valid ID as result
    int id = utils->activeIap();
    QCOMPARE(id, 5);

    delete utils;
}

/**
 * This function tests active WLAN IAP getter without existing connection.
 */
void TestWlanQtUtils::testActiveIapNotFound()
{
    // Execute the function under test and check that we get invalid ID as result
    int id = wlanQtUtils_->activeIap();
    QCOMPARE(id, WlanQtUtils::IapIdNone);
}

/**
 * Test WLAN scan triggering interface.
 */
void TestWlanQtUtils::testScanWlans()
{
    // Execute function under test
    wlanQtUtils_->scanWlans();

    // No need to verify scan results here, testAvailableWlan* test cases
    // are for that. Just make sure the result signal is received.
    subTestSignalWaitAndTake(signalScanReady_, NULL);
}

/**
 * Test WLAN scan triggering interface with two consecutive requests.
 */
void TestWlanQtUtils::testScanWlansIgnored()
{
    // Do not complete scan request immediately
    testContext.connMon_.wlanScanResult_.completeWlanScan_ = false;
    
    // Execute function under test
    wlanQtUtils_->scanWlans();

    // Execute function under test again -> this one is ignored
    wlanQtUtils_->scanWlans();
    
    // Ongoing scan is cancelled in destructors
}

/**
 * Test WLAN scan stopping interface when scan is active.
 */
void TestWlanQtUtils::testStopWlanScanOk()
{
    // Do not complete scan request immediately
    testContext.connMon_.wlanScanResult_.completeWlanScan_ = false;
    // Start a scan
    wlanQtUtils_->scanWlans();
    // Execute function under test
    wlanQtUtils_->stopWlanScan();
    
    // No return values or signals to verify
}

/**
 * Test WLAN scan stopping interface when no scan is ongoing.
 */
void TestWlanQtUtils::testStopWlanScanIgnored()
{
    // Execute function under test
    wlanQtUtils_->stopWlanScan();
    
    // No return values or signals to verify
}

/**
 * This function tests Wlan network opening signal when network is not opened by the dll.
 */
void TestWlanQtUtils::testWlanNetworkOpened()
{
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 5);

    // Send event for connection creation.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Send events for connection status change -> opened.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConfigDaemonFinishedRegistration));
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments);
    QCOMPARE(arguments.at(0).toInt(), 5);

    // Send uninteresting event to gain coverage
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
        EConnMonNetworkStatusChange,
        10));
}

/**
 * This function tests Wlan network closing signal when network is not closed by the dll.
 */
void TestWlanQtUtils::testWlanNetworkClosed()
{
    // First create a connection
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 5);

    // Send event for connection creation.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Send events for connection status change -> opened.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments);
    QCOMPARE(arguments.at(0).toInt(), 5);

    // Send event for connection status change -> closed.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerClosed));

    // Send event for connection deletion.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonDeleteConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    subTestSignalWaitAndTake(signalWlanNetworkClosed_, &arguments);
    QCOMPARE(arguments.at(0).toInt(), 5);
    QCOMPARE(arguments.at(1).toInt(), KErrNone);
}

/**
 * This function tests WlanQtUtilsAp copy constructor.
 */
void TestWlanQtUtils::testApCopyConstructor()
{
    WlanQtUtilsAp firstAp;
    firstAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testApCopyConstructor");
    firstAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90 );
    firstAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    firstAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeWpa);
    firstAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, true);    
    firstAp.setValue(WlanQtUtilsAp::ConfIdWpaPsk, "100euronlounas");
    
    // Create the copy AP
    WlanQtUtilsAp secondAp(firstAp);
    QCOMPARE(secondAp.value(WlanQtUtilsAp::ConfIdSsid).toString(), QString("testApCopyConstructor"));
    QCOMPARE(secondAp.value(WlanQtUtilsAp::ConfIdSecurityMode).toInt(), (int)(CMManagerShim::WlanSecModeWpa));
    QCOMPARE(secondAp.value(WlanQtUtilsAp::ConfIdWpaPskUse).toBool(), true);
    QCOMPARE(secondAp.value(WlanQtUtilsAp::ConfIdSignalStrength).toInt(), 90);
    QCOMPARE(secondAp.value(WlanQtUtilsAp::ConfIdConnectionMode).toInt(), (int)(CMManagerShim::Infra));
    QCOMPARE(secondAp.value(WlanQtUtilsAp::ConfIdWpaPsk).toString(), QString("100euronlounas"));
}

/**
 * This function tests ICT when connection test passes.
 */
void TestWlanQtUtils::testConnectionTestOk()
{
    // Create new IAP to test
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testConnectionTestOk");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90 );
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeOpen);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);
    int iapId = wlanQtUtils_->createIap(&wlanAp);
    QVERIFY(iapId != WlanQtUtils::IapIdNone);

    testContext.esock_.startRetValue_ = KErrNone;

    // Esock stub completes connection creation immediately
    wlanQtUtils_->connectIap(iapId, true);

    // Connection creation in ConnMon interface
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, iapId);
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Connection status change in ConnMon interface
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    
    // Receive signal for connection opening (caused by connectIap, which completed immediately)
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), iapId); 

    // Connection status change to opened in ConnMon interface. Sub test cases between test
    // cases check that no extra signals are sent
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    // Connection test automatically started at this point. Call result function explicitly
    // No interface exists that we could check that the IAP is moved to Internet SNAP correctly
    TBuf<5> string;
    wlanQtUtils_->d_ptr->mConnTestWrapper->d_ptr_->ConnectivityObserver(EConnectionOk, string);

    subTestSignalWaitAndTake(signalIctResult_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), iapId); 
    QCOMPARE(arguments.at(1).toBool(), true);
}

/**
 * This function tests ICT when connection test fails.
 */
void TestWlanQtUtils::testConnectionTestErr()
{
    // Create and connect an IAP and request ICT to be run
    WlanQtUtilsAp wlanAp;
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, "testConnectionTestErr");
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSignalStrength, 90 );
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, CMManagerShim::Infra);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, CMManagerShim::WlanSecModeWpa);
    wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);
    
    int iapId = wlanQtUtils_->createIap(&wlanAp);
    wlanQtUtils_->connectIap(iapId, true);

    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, iapId);
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), iapId); 

    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    // Connection test automatically started at this point. Call result function explicitly
    // No interface exists that we could check that IAP is not moved to another SNAP
    TBuf<5> string;
    wlanQtUtils_->d_ptr->mConnTestWrapper->d_ptr_->ConnectivityObserver(EHttpAuthenticationNeeded, string);

    subTestSignalWaitAndTake(signalIctResult_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), iapId); 
    QCOMPARE(arguments.at(1).toBool(), false);
}

/**
 * This function tests that Gprs connection events are ignored.
 */
void TestWlanQtUtils::testConnMonEventGprs()
{
    // First create a connection
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 4);
    testContext.connMon_.activeConnections_.activeConnList_[0]->connMonBearerType_ = EBearerGPRS;
    
    // Send event for connection creation.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Send events for connection status change -> opened.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    // Send event for connection status change -> closed.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerClosed));

    // Send event for connection deletion.
    wlanQtUtils_->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonDeleteConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));
}

/**
 * This function tests ConnMon event handling with connection existing already during dll construction.
 */
void TestWlanQtUtils::testConnMonEventCreatedBeforeConstructor()
{
    // IAP ID 5 exists in default commsdat file
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 5);

    // Create a new instance in order to test functionality triggered in constructor.
    WlanQtUtils *utils = new WlanQtUtils();

    QSignalSpy *signalWlanNetworkOpened = new QSignalSpy(utils, SIGNAL(wlanNetworkOpened(int)));
    QVERIFY(signalWlanNetworkOpened->isValid() == true);
    QSignalSpy *signalWlanNetworkClosed = new QSignalSpy(utils, SIGNAL(wlanNetworkClosed(int, int)));
    QVERIFY(signalWlanNetworkClosed->isValid() == true);
    
    // Send events for connection status change -> opened.
    utils->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    utils->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened, &arguments);
    QCOMPARE(arguments.at(0).toInt(), 5);

    // Send event for connection status change -> closed.
    utils->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerClosed));

    // Send event for connection deletion.
    utils->d_ptr->mConMonWrapper->d_ptrInfo->EventL(CConnMonEventBase(
            EConnMonDeleteConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    subTestSignalWaitAndTake(signalWlanNetworkClosed, &arguments);
    QCOMPARE(arguments.at(0).toInt(), 5);
    QCOMPARE(arguments.at(1).toInt(), KErrNone);
    
    delete signalWlanNetworkOpened;
    delete signalWlanNetworkClosed;
    delete utils;
}

// ---------------------------------------------------------
// SUB TEST CASES
// ---------------------------------------------------------

/**
 * This function waits for active objects to get time to run, takes the arguments of the
 * first signal and returns them from the given signal spy.
 * This function also verifies that the number of signals is 1.
 * 
 * @param[in] spy Signal spy.
 * @param[out] arguments Arguments of the first signal in the given signal spy. NULL if arguments not needed.
 */
void TestWlanQtUtils::subTestSignalWaitAndTake(QSignalSpy* spy, QList<QVariant>* arguments)
{
    QTest::qWait(1);
    QCOMPARE(spy->count(), 1);
    QList<QVariant> arguments_tmp = spy->takeFirst();
    if (arguments != NULL) {
        *arguments = arguments_tmp;
    }
}

/**
 * This function loads given CommsDat file, replacing current one.
 * If CommsDat file doesn't exist, it can be re-created by commanding WST script.
 * Using help switch is a quick way:
 *     run_wst HELP
 * 
 * @param newCommsdatFilename Filename of the new CommsDat to be loaded. 
 */
void TestWlanQtUtils::subTestLoadCommsDatFile(QString newCommsdatFilename)
{
    // EPOC's CommsDat filename   
    const QString epocCommsdatFilename("cccccc00.cre");
    // EPOC's directory for CommsDat file
    const QString commsdatDir("c:\\private\\10202be9\\persists\\");
    // Created backup directory under EPOC for CommsDat files
    const QString storeDir("c:\\private\\10202be9\\persists\\backup\\");
    QString nameOld = commsdatDir + epocCommsdatFilename;
    QString nameDefault = storeDir + newCommsdatFilename;

    // First remove the old CommsDat file.
    Q_ASSERT(QFile::remove(nameOld) == TRUE);
    
    // Copy the stored default CommsDat file.
    Q_ASSERT(QFile::copy(nameDefault, nameOld) == TRUE);
}

/**
 * 
 */
void TestWlanQtUtils::subTestNewWlanQtUtils()
{
    if (wlanQtUtils_ != NULL) {
        delete wlanQtUtils_;
    }
    wlanQtUtils_ = new WlanQtUtils();

    if (signalScanReady_ != NULL) {
        delete signalScanReady_;
    }
    signalScanReady_ = new QSignalSpy(wlanQtUtils_, SIGNAL(wlanScanReady()));
    QVERIFY(signalScanReady_->isValid() == true);

    if (signalWlanNetworkOpened_ != NULL) {
        delete signalWlanNetworkOpened_;
    }
    signalWlanNetworkOpened_ = new QSignalSpy(wlanQtUtils_, SIGNAL(wlanNetworkOpened(int)));
    QVERIFY(signalWlanNetworkOpened_->isValid() == true);
    
    if (signalWlanNetworkClosed_ != NULL) {
        delete signalWlanNetworkClosed_;
    }
    signalWlanNetworkClosed_ = new QSignalSpy(wlanQtUtils_, SIGNAL(wlanNetworkClosed(int, int)));
    QVERIFY(signalWlanNetworkClosed_->isValid() == true);
    
    if (signalIctResult_ != NULL) {
        delete signalIctResult_;
    }
    signalIctResult_ = new QSignalSpy(wlanQtUtils_, SIGNAL(ictResult(int, bool)));
    QVERIFY(signalIctResult_->isValid() == true);
}
