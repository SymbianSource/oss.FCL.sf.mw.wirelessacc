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
* This is the header file for testing Wlan Qt Utilities library.
*/

#ifndef TESTWLANQTUTILS_H_
#define TESTWLANQTUTILS_H_

#include <QObject>

class QSignalSpy;
class WlanQtUtils;

class TestWlanQtUtils: public QObject
{
    Q_OBJECT
    
private slots:

    // Functions from QTest framework.
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test functions for public interface.
    void testAvailableWlanAps();
    void testAvailableWlanApsWithIaps();
    void testAvailableWlanApsSequence();

    void testCreateWlanIapOk();
    
    void testConnectIapOk();
    void testConnectIapErr();
    
    void testDisconnectIapOk();

    void testIapFound();
    void testIapNotFound();
    
    void testConnectedWlanIdFound();
    void testConnectedWlanIdFoundConstructor();
    void testConnectedWlanIdNotFound();

    void testScanWlans();
    
    void testWlanNetworkOpened();
    void testWlanNetworkClosed();
    
    // Test functions for private implementation (tested via public interface)
    void testConnectionTestPass();
    void testConnectionTestFail();
    
private:
    
    // Helper functions, that is, sub test cases.
    void subTestSignalWaitAndTake(QSignalSpy *spy, QList<QVariant> *arguments);
    void subTestLoadCommsDatFile(QString newCommsdatFilename);
    void subTestNewWlanQtUtils();

    // Member variables.
    WlanQtUtils *wlanQtUtils_;
    QSignalSpy *signalScanReady_;
    QSignalSpy *signalWlanNetworkOpened_;
    QSignalSpy *signalWlanNetworkClosed_;
    
    /* Default CommsDat file name
    SNAP ID=3: Internet:
        IAP ID = 1: packet data 1
        IAP ID = 3: packet data 2
        IAP ID = 4: packet data 3
        IAP ID = 5: WLAN IAP 1, EWlanSecModeOpen
        IAP ID = 6: WLAN IAP 2, EWlanSecModeWep, w2key
        IAP ID = 7: WLAN IAP 3, EWlanSecModeWpa, wlan3key
    SNAP ID = 4: Multimedia msg.
    SNAP ID = 5: WAP services
    SNAP ID = 6: My Snap:
        IAP ID = 8: Home WLAN, EWlanSecModeOpen
        IAP ID = 9: Streaming
     */
    static const QString commsDatDefault_;
};

#endif /* TESTWLANQTUTILS_H_ */

