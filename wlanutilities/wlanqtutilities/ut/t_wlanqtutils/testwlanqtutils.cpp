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
* This is the source file for testing Wlan Qt Utilities library.
*/

#include <QtCore>
#include <QTest>
#include <QSignalSpy>
#include <rconnmon.h>
#include <nifvar.h>
#include "wlanqtutilscommon.h"
#include "wlanqtutilswlanap.h"
#include "wlanqtutilsiap.h"
#include "wlanqtutilswlaniap.h"
#include "wlanqtutils.h"
#include "wlanqtutils_p.h"
#include "wlanqtutilsconmonwrapper.h"
#include "wlanqtutilsconmonwrapper_s60_p.h"
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
 * This function will be called before the first test function is executed.
 */
void TestWlanQtUtils::initTestCase()
{
    wlanQtUtils_ = NULL;
    signalScanReady_ = NULL;
    signalWlanNetworkOpened_ = NULL;
    signalWlanNetworkClosed_ = NULL;

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

    // TODO: Can we get rid of this delay?
    QWARN(": \n *********** \n * PASSED! * \n *********** \n\n\n");
    QWARN(": \n *** Window will be closed in 5s... \n\n\n");    
    QTest::qSleep(5000);
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
}

// ---------------------------------------------------------
// TEST CASES
// ---------------------------------------------------------

/**
 * Test available WLAN APs when scan is triggered by client.
 * Two scan results have the same SSID and the latter one of those will be removed.
 * Two scan results have the same SSID but different security mode
 * and both are included in the results.
 */
void TestWlanQtUtils::testAvailableWlanAps()
{
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(10);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[0]->setSsid("Same SSID");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[2]->setSsid("Same SSID");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[5]->setSsid("Same SSID, different SecMode");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[5]->setSecurityMode(WlanQtUtilsWlanSecModeWpa);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[8]->setSsid("Same SSID, different SecMode");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[8]->setSecurityMode(WlanQtUtilsWlanSecModeOpen);

    // Request a scan to get result signal
    wlanQtUtils_->scanWlans();
    
    // Let active object run and verify signal.
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get and verify the list of available WLAN APs.
    QList<WlanQtUtilsWlanIap *> iaps;
    QList<WlanQtUtilsWlanAp *> aps;
    wlanQtUtils_->availableWlanAps(iaps, aps);

    // First remove AP that has duplicate SSID.
    // Then order APs alphabetically.
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.removeAt(2);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.move(4, 1);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.move(7, 1);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.move(8, 3);
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear(); // TODO: Free AP pointers too.
}

/**
 * Test available WLAN APs when there are also WLAN IAPs available.
 */
void TestWlanQtUtils::testAvailableWlanApsWithIaps()
{
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(7);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[1]->setSsid("WLAN IAP 3");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[1]->setSecurityMode(WlanQtUtilsWlanSecModeWpa);    
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[3]->setSsid("");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[5]->setSsid("WLAN IAP 1");
    // Todo: security mode not compared yet due to an issue with CMM vs ConnMon WLAN security
    // modes under investigation
    // SSID is found in Internet Snap, but security mode does not match:
    // testContext.connMon_.wlanScanResult_.wlanScanResultList_[6]->setSsid("WLAN IAP 2");
    // testContext.connMon_.wlanScanResult_.wlanScanResultList_[6]->setSecurityMode(WlanQtUtilsWlanSecModeWpa);    
    
    // Request a scan to get result signal
    wlanQtUtils_->scanWlans();
    
    // Let active object run and verify signal.
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get and verify the list of available WLAN APs.
    QList<WlanQtUtilsWlanIap *> iaps;
    QList<WlanQtUtilsWlanAp *> aps;
    wlanQtUtils_->availableWlanAps(iaps, aps);

    // Verify WLAN AP list. First, remove scan results that will not appear
    // because they are hidden WLANs or WLAN IAPs.
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.removeAt(5);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.removeAt(3);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_.removeAt(1);
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);

    // Verify WLAN IAP list
    QCOMPARE(iaps.count(), 2);
    QCOMPARE(iaps[0]->id(), 5);
    QCOMPARE(iaps[0]->name(), QString("WLAN IAP 1"));
    QCOMPARE(iaps[0]->bearerType(), WlanQtUtilsBearerTypeWlan);
    QCOMPARE(iaps[0]->ssid(), QString("WLAN IAP 1"));
    QCOMPARE(iaps[1]->id(), 7);
    QCOMPARE(iaps[1]->name(), QString("WLAN IAP 3"));
    QCOMPARE(iaps[1]->bearerType(), WlanQtUtilsBearerTypeWlan);
    QCOMPARE(iaps[1]->ssid(), QString("WLAN IAP 3"));
    iaps.clear(); // TODO: Free IAP pointers too.
    aps.clear(); // TODO: Free AP pointers too.
}

/**
 * Test refereshing of WLAN APs when client requests sequential scans.
 */
void TestWlanQtUtils::testAvailableWlanApsSequence()
{
    // **************** Before 1st scan ********************
    QList<WlanQtUtilsWlanIap *> iaps;
    QList<WlanQtUtilsWlanAp *> aps;
    wlanQtUtils_->availableWlanAps(iaps, aps);
    // Verify we have no results
    QCOMPARE(aps.count(), 0);
    QCOMPARE(iaps.count(), 0);
    
    // **************** 1st scan ********************
    // 6 APs are required for this scan
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(6);

    wlanQtUtils_->scanWlans();
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get the results for the 1st scan
    wlanQtUtils_->availableWlanAps(iaps, aps);
    // Verify the results for the scan
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear(); // TODO: free memory of pointers too.

    // **************** 2nd scan ********************
    // 2 APs are required for this scan
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(2);

    wlanQtUtils_->scanWlans();
    subTestSignalWaitAndTake(signalScanReady_, NULL);

    // Get the results for the 2nd scan
    wlanQtUtils_->availableWlanAps(iaps, aps);
    // Verify the results for the scan
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear(); // TODO: Free memory

    // **************** 3rd scan ********************
    // 4 APs are required for the 3rd scan
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(4);

    wlanQtUtils_->scanWlans();
    subTestSignalWaitAndTake(signalScanReady_, NULL);
    
    // Get the results for the 3rd scan
    wlanQtUtils_->availableWlanAps(iaps, aps);
    // Verify the results for the scan
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);
    QCOMPARE(iaps.count(), 0);
    aps.clear(); // TODO: Free memory    
}

/**
 * This function tests creation of WLAN IAP in a succesful manner.
 * - WLAN scan is made because otherwise we cannot verify that IAP creation is successful.
 * - Check that there are no available WLAN IAPs.
 * - Fetch SNAP list.
 * - Create WLAN IAP.
 * - Check that WLAN IAP has been created and that this IAP is not in WLAN AP scan results.
 */
void TestWlanQtUtils::testCreateWlanIapOk()
{
    testContext.connMon_.wlanScanResult_.createDefaultWlanScanResultList(1);
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[0]->setSsid("New WLAN IAP");
    testContext.connMon_.wlanScanResult_.wlanScanResultList_[0]->setSecurityMode(WlanQtUtilsWlanSecModeWpa2);

    // Create a new instance in order to test functionality triggered in constructor.
    WlanQtUtils *utils = new WlanQtUtils();
    QSignalSpy spy(utils, SIGNAL(wlanScanReady()));
    QVERIFY(spy.isValid() == true);
    subTestSignalWaitAndTake(&spy, NULL);

    // Ensure there are no WLAN IAPs but there is one scan result.
    QList<WlanQtUtilsWlanIap *> iaps;
    QList<WlanQtUtilsWlanAp *> aps;    
    utils->availableWlanAps(iaps, aps);

    QCOMPARE(iaps.count(), 0);
    testContext.connMon_.wlanScanResult_.verifyWlanScanResultList(aps);

    // Execute createWlanIap() function
    WlanQtUtilsWlanAp wlanAp("New WLAN IAP", "abcdef", 90, EConnMonInfraStructure, WlanQtUtilsWlanSecModeWpa2);
    int iapId = utils->createWlanIap(&wlanAp);

    // Verify that created IAP is in the IAP list and AP list is empty.
    utils->availableWlanAps(iaps, aps);
    QCOMPARE(iaps.count(), 1);
    QCOMPARE(iaps[0]->id(), iapId);
    QCOMPARE(iaps[0]->name(), QString("New WLAN IAP"));
    QCOMPARE(iaps[0]->bearerType(), WlanQtUtilsBearerTypeWlan);
    QCOMPARE(iaps[0]->ssid(), QString("New WLAN IAP"));
    // Todo: should be Wpa2, CMM vs ConnMon WLAN security modes under investigation
    QCOMPARE(iaps[0]->securityMode(), WlanQtUtilsWlanSecModeWpa);
    QCOMPARE(aps.count(), 0);
    iaps.clear(); // TODO: memory leak.
    delete utils;
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
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Connection status change in ConnMon interface
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    
    // Receive signal for connection opening (caused by connectIap, which completed immediately)
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), 5); 

    // Connection status change to opened in ConnMon interface. Sub test cases between test
    // cases check that no extra signals are sent
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
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
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonEventBase(
            EConnMonDeleteConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));
    
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkClosed_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), 7);
}

// TODO: testDisconnectFail cases are needed when fail branches are
// implemented into the connmon wrapper
/**
 * This function tests IAP disconnecting functionality.
 */
void TestWlanQtUtils::testDisconnectIapOk()
{
    // Create and connect an IAP we can then disconnect
    WlanQtUtilsWlanAp wlanAp("Disconnect IAP", "abcdef", 90, EConnMonInfraStructure, WlanQtUtilsWlanSecModeWpa2);
    int iapId = wlanQtUtils_->createWlanIap(&wlanAp);
    testContext.esock_.startRetValue_ = KErrNone;
    wlanQtUtils_->connectIap(iapId);
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), iapId); 
    
    // The disconnect function does not have any return values or
    // signals related to the disconnection, thus plain
    // function call is done for the test.
    wlanQtUtils_->disconnectIap(iapId);
}

/**
 * This function tests IAP getter with existing IAP ID.
 */
void TestWlanQtUtils::testIapFound()
{
    // Create the IAP we want to find with the getter
    WlanQtUtilsWlanAp wlanAp("testIapFound", "abcdef", 90, EConnMonInfraStructure, WlanQtUtilsWlanSecModeWpa2);
    int iapId = wlanQtUtils_->createWlanIap(&wlanAp);
    
    // Execute the function under test and check that we got the correct IAP
    WlanQtUtilsIap *iap = wlanQtUtils_->iap(iapId);
    QVERIFY(iap != NULL);
    QCOMPARE(iap->id(), iapId);
    QCOMPARE(iap->name(), QString("testIapFound"));
    QCOMPARE(iap->bearerType(), WlanQtUtilsBearerTypeWlan);
}

/**
 * This function tests IAP getter with non-existing IAP ID.
 */
void TestWlanQtUtils::testIapNotFound()
{
    // Execute the function under test and check that we get no IAP as result
    WlanQtUtilsIap *iap = wlanQtUtils_->iap(1000);
    QVERIFY(iap == NULL);
}

/**
 * This function tests connected WLAN IAP getter with existing connection.
 */
void TestWlanQtUtils::testConnectedWlanIdFound()
{
    // Create the IAP we want to find with the getter
    WlanQtUtilsWlanAp wlanAp("testConnectedWlanIdFound", "abcdef", 90, EConnMonInfraStructure, WlanQtUtilsWlanSecModeWpa2);
    int iapId = wlanQtUtils_->createWlanIap(&wlanAp);
    
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, iapId);

    // Send event for connection creation.
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Send events for connection status change -> opened.
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KStartingConnection));
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments);
    QCOMPARE(arguments.at(0).toInt(), iapId);
        
    // Execute the function under test and check that we get valid ID as result
    int id = wlanQtUtils_->connectedWlanId();
    QCOMPARE(id, iapId);
}

/**
 * This function tests connected WLAN IAP getter with connection existing already during dll construction.
 */
void TestWlanQtUtils::testConnectedWlanIdFoundConstructor()
{
    // IAP ID 5 exists in default commsdat file
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 5);

    // Create a new instance in order to test functionality triggered in constructor.
    WlanQtUtils *utils = new WlanQtUtils();

    // Execute the function under test and check that we get valid ID as result
    int id = utils->connectedWlanId();
    QCOMPARE(id, 5);

    delete utils;
}

/**
 * This function tests connected WLAN IAP getter without existing connection.
 */
void TestWlanQtUtils::testConnectedWlanIdNotFound()
{
    // Execute the function under test and check that we get invalid ID as result
    int id = wlanQtUtils_->connectedWlanId();
    QCOMPARE(id, WlanQtUtilsInvalidIapId);
}

/**
 * Test WLAN scan triggering interface.
 */
void TestWlanQtUtils::testScanWlans()
{
    // Execute function under test
    wlanQtUtils_->scanWlans();

    // No need to verify scan results here, testAvailableWlanAps* test cases
    // are for that. Just make sure the result signal is received.
    subTestSignalWaitAndTake(signalScanReady_, NULL);
}

/**
 * This function tests Wlan network opening signal when network is not opened by the dll.
 */
void TestWlanQtUtils::testWlanNetworkOpened()
{
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 5);

    // Send event for connection creation.
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Send events for connection status change -> opened.
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments);
    QCOMPARE(arguments.at(0).toInt(), 5);
}

/**
 * This function tests Wlan network closing signal when network is not closed by the dll.
 */
void TestWlanQtUtils::testWlanNetworkClosed()
{
    // First create a connection
    testContext.connMon_.activeConnections_.createDefaultActiveConnList(1, 5);

    // Send event for connection creation.
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonEventBase(
            EConnMonCreateConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    // Send events for connection status change -> opened.
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KConnectionOpen));
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerOpen));

    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments);
    QCOMPARE(arguments.at(0).toInt(), 5);

    // Send event for connection status change -> closed.
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonConnectionStatusChange(
        testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId(),
        0,
        KLinkLayerClosed));

    // Send event for connection deletion.
    wlanQtUtils_->d_ptr->conMonWrapper_->d_ptrConnInfo->EventL(CConnMonEventBase(
            EConnMonDeleteConnection,
            testContext.connMon_.activeConnections_.activeConnList_[0]->connectionId()));

    subTestSignalWaitAndTake(signalWlanNetworkClosed_, &arguments);
    QCOMPARE(arguments.at(0).toInt(), 5);
}

/**
 * This function tests ICT when connection test passes.
 */
void TestWlanQtUtils::testConnectionTestPass()
{
    // Create and connect an IAP we can then test
    WlanQtUtilsWlanAp wlanAp("Connection test IAP", "abcdef", 90, EConnMonInfraStructure, WlanQtUtilsWlanSecModeWpa2);
    int iapId = wlanQtUtils_->createWlanIap(&wlanAp);
    testContext.esock_.startRetValue_ = KErrNone;
    wlanQtUtils_->connectIap(iapId);
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), iapId); 

    // Connection test automatically started at this point. Call result function explicitly
    // No interface exists that we could check that the IAP is moved to Internet SNAP correctly
    TBuf<5> string;
    wlanQtUtils_->d_ptr->connTestWrapper_->d_ptr_->ConnectivityObserver(EConnectionOk, string);
}

/**
 * This function tests ICT when connection test fails.
 */
void TestWlanQtUtils::testConnectionTestFail()
{
    // Create and connect an IAP we can then test
    WlanQtUtilsWlanAp wlanAp("Connection test IAP", "abcdef", 90, EConnMonInfraStructure, WlanQtUtilsWlanSecModeWpa2);
    int iapId = wlanQtUtils_->createWlanIap(&wlanAp);
    testContext.esock_.startRetValue_ = KErrNone;
    wlanQtUtils_->connectIap(iapId);
    QList<QVariant> arguments;
    subTestSignalWaitAndTake(signalWlanNetworkOpened_, &arguments); 
    QCOMPARE(arguments.at(0).toInt(), iapId); 

    // Connection test automatically started at this point. Call result function explicitly
    // No interface exists that we could check that IAP is not moved to another SNAP
    // Todo: Repeat for all possible error codes to gain coverage
    TBuf<5> string;
    wlanQtUtils_->d_ptr->connTestWrapper_->d_ptr_->ConnectivityObserver(EHttpAuthenticationNeeded, string);
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
    signalWlanNetworkClosed_ = new QSignalSpy(wlanQtUtils_, SIGNAL(wlanNetworkClosed(int)));
    QVERIFY(signalWlanNetworkClosed_->isValid() == true);
    
    // Let first active objects to run. First WLAN scan is done in engine contructor.
    subTestSignalWaitAndTake(signalScanReady_, NULL);
}

// Create main function using QTest marco.
QTEST_MAIN(TestWlanQtUtils)
