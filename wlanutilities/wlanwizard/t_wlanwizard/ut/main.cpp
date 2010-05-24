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
#include "testwlanwizardui.h"
#include "testwlanwizard.h"
#include "hbautotest.h"

// ======== LOCAL FUNCTIONS ========
int main(int argc, char *argv[])
{
    qDebug("TestWlanWizardUI main start");
    HbApplication app(argc, argv);
    app.setApplicationName("TestWlanWizardUI");

    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o";
    pass[2] = "c:\\data\\TestWlanWizard.txt";
    TestWlanWizard tcUtilities;
    int res = QTest::qExec(&tcUtilities, 3, pass);

    pass[2] = "c:\\data\\TestWlanWizardUi.txt";
    TestWlanWizardUi tc;
    res = QTest::qExec(&tc, 3, pass);
    
    qDebug("TestWlanWizardUI main exit");
    return res;
}
