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
 */

// System includes
#include <HbApplication>
#include <QtCore>
#include <QTest>
#include <QDebug>
#include <QList>

// User includes
#include "testwlanwizard.h"
#include "testwlanwizardeap.h"
#include "testwlanwizardconnect.h"
#include "testwlanwizardmanual.h"
#include "testwlanwizardwps.h"

// ======== LOCAL FUNCTIONS ========
int main(int argc, char *argv[])
{
    qDebug("TestWlanWizard main start");
    HbApplication app(argc, argv);
    int res = 0;
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o";
    
    // log folder: \epoc32\winscw\c\data
    
    app.setApplicationName("TestWlanWizardUtilities");
    pass[2] = "c:\\data\\TestWlanWizardUtilities.txt";
    TestWlanWizard *tcUtilities = new TestWlanWizard();
    res |= QTest::qExec(tcUtilities, 3, pass);
    delete tcUtilities;
    tcUtilities = NULL;

    app.setApplicationName("TestWlanWizardConnect");
    pass[2] = "c:\\data\\TestWlanWizardConnect.txt";
    TestWlanWizardConnect *tcConnect = new TestWlanWizardConnect();
    res |= QTest::qExec(tcConnect, 3, pass);
    delete tcConnect;
    tcConnect = NULL;
    
    app.setApplicationName("TestWlanWizardManual");
    pass[2] = "c:\\data\\TestWlanWizardManual.txt";
    TestWlanWizardManual *tcManual = new TestWlanWizardManual();
    res |= QTest::qExec(tcManual, 3, pass);
    delete tcManual;
    tcManual = NULL;

    app.setApplicationName("TestWlanWizardEap");
    pass[2] = "c:\\data\\TestWlanWizardEap.txt";
    TestWlanWizardEap *tcEap = new TestWlanWizardEap();
    res |= QTest::qExec(tcEap, 3, pass);
    delete tcEap;
    tcEap = NULL;

    app.setApplicationName("TestWlanWizardWps");
    pass[2] = "c:\\data\\TestWlanWizardWps.txt";
    TestWlanWizardWps *tcWps = new TestWlanWizardWps();
    res |= QTest::qExec(tcWps, 3, pass);
    delete tcWps;
    tcWps = NULL;
    
    qDebug("TestWlanWizard main exit");
    return res;
}
