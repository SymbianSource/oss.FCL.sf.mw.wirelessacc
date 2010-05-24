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
#include <HbDocumentLoader>
#include <HbStackedWidget>
#include <HbRadioButtonList>
#include <HbAction>
#include <HbLineEdit>
#include <HbLabel>
#include <QGraphicsWidget>
#include <QObjectList>
#include <QtCore>
#include <QTest>
#include <QDebug>
#include <QList>
#include <cmmanagerdefines_shim.h>

// User includes
#include "testwlanwizardui.h"
#include "testwlanwizardui_conf.h"
#include "hbautotest.h"
#include "wlanwizard.h"
#include "wlanwizard_p.h"
#include "wlanwizardpagessid.h"
#include "context_wlanqtutils.h"
#include "wlanqtutils.h"
#include "wlanqtutilsap.h"

// External function prototypes

// Local constants

class TestRadioButtonList: public HbRadioButtonList
{
public:
    void emitActivated(const QModelIndex &modelIndex)
    { HbRadioButtonList::emitActivated(modelIndex); }
};

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------
// FRAMEWORK FUNCTIONS
// ---------------------------------------------------------

ContextWlanApList::ContextWlanApList()
{

}

ContextWlanApList::~ContextWlanApList()
{
    clear();
}

void ContextWlanApList::Add(QString name, int netMode, int secMode, bool wpaPskInUse,
    bool wpsSupported, int signalStrength)
{
    QSharedPointer<WlanQtUtilsAp> temp = QSharedPointer<WlanQtUtilsAp>(new WlanQtUtilsAp());
    temp->setValue(WlanQtUtilsAp::ConfIdSsid, name);
    temp->setValue(WlanQtUtilsAp::ConfIdConnectionMode, netMode);
    temp->setValue(WlanQtUtilsAp::ConfIdSecurityMode, secMode);
    temp->setValue(WlanQtUtilsAp::ConfIdWpaPskUse, wpaPskInUse);
    temp->setValue(WlanQtUtilsAp::ConfIdWpsSupported, wpsSupported);
    temp->setValue(WlanQtUtilsAp::ConfIdSignalStrength, signalStrength);

    mList.append(temp);
}

void ContextWlanApList::clear()
{
    mList.clear();
}

/**
 * This function will be called before the first test function is executed.
 */
void TestWlanWizardUi::initTestCase()
{
    qDebug("TestWlanWizardUi::initTestCase");

    mMainWindow = new HbAutoTestMainWindow;

    mView = new TestView();

    mMainWindow->addView(mView);
    mMainWindow->setCurrentView(mView);
    mMainWindow->installEventFilter(this);
    mMainWindow->show();

    QTest::qWait(1);
    while (!mEvent) {
        QTest::qWait(WaitTimeForUi);
    }
    mEvent = false;

    mApList = new ContextWlanApList();

}

/**
 * This function will be called after the last test function was executed.
 */
void TestWlanWizardUi::cleanupTestCase()
{
    qDebug("TestWlanWizardUi::cleanupTestCase");

    qDebug("delete mApList");
    delete mApList;
    mApList = NULL;

    qDebug("delete mMainWindow");
    mMainWindow->deleteLater();

    qDebug("TestWlanWizardUi::cleanupTestCase exit");
}

/**
 * This function will be called before each test function is executed.
 */
void TestWlanWizardUi::init()
{
    qDebug("TestWlanWizardUi::init()");

    mView->createWizard();
    mWlanQtUtilsContext = new WlanQtUtilsContext(mView->mWizard->d_ptr->mWlanQtUtils.data());
}

/**
 * This function will be called after each test function is executed.
 */
void TestWlanWizardUi::cleanup()
{
    qDebug("TestWlanWizardUi::cleanup()");

    delete mWlanQtUtilsContext;
    mView->deleteWizard();
    mApList->clear();
    QTest::qWait(1);
}

// ---------------------------------------------------------
// TEST CASES
// ---------------------------------------------------------


void TestWlanWizardUi::tcStartWizard()
{
    qDebug("Start mView");
}

/*!
 * 
 */
void TestWlanWizardUi::tc01()
{
#ifdef tc01_enabled
    
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false);

    mWlanQtUtilsContext->setScanWlanDirectResult("huuhaa3421", mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);
    
    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", ""), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), false );
#endif
}

/*!
 * 
 */ 
void TestWlanWizardUi::tc02()
{
#ifdef tc02_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false);

    mWlanQtUtilsContext->setScanWlanDirectResult("12345678901234567890123456789012", mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "1234567890123456789012345678901234567890"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyActionButtons(ButtonEnabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true );  
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
#endif
}

/*!
 *  
 */
void TestWlanWizardUi::tc03()
{
#ifdef tc03_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyActionButtons(ButtonEnabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true );  
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyActionButtons(ButtonDisabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    // Currently against the spec in http://wikis.in.nokia.com/pub/UXD/101ConnectionManager/occ_logical_flows_and_wireframes.pdf
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonHidden, ButtonEnabled), true );

    QCOMPARE( mouseClickCancel(), true );

    QTest::qWait(WaitTimeForUi);

#endif
}

/*!
 * 
 */
void TestWlanWizardUi::tc04()
{
#ifdef tc04_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyActionButtons(ButtonEnabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true );  

    // Click previous button and next again
    QCOMPARE( mouseClickPrevious(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonEnabled, ButtonHidden), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyActionButtons(ButtonDisabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    // Currently against the spec in http://wikis.in.nokia.com/pub/UXD/101ConnectionManager/occ_logical_flows_and_wireframes.pdf
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonHidden, ButtonEnabled), true );
    QCOMPARE( mouseClickFinish(), true );
#endif
}

/*!
 * 
 */
void TestWlanWizardUi::tc05()
{
#ifdef tc05_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false);

    mWlanQtUtilsContext->setScanWlanDirectResult("huuhaa3421", mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyActionButtons(ButtonEnabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true ); 
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyActionButtons(ButtonDisabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    // Currently against the spec in http://wikis.in.nokia.com/pub/UXD/101ConnectionManager/occ_logical_flows_and_wireframes.pdf
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonHidden, ButtonEnabled), true );
    QCOMPARE( mouseClickFinish(), true );
#endif
}

/*!
 * 
 */
void TestWlanWizardUi::tc06()
{
#ifdef tc06_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyActionButtons(ButtonEnabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true ); 
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyActionButtons(ButtonDisabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    // Currently against the spec in http://wikis.in.nokia.com/pub/UXD/101ConnectionManager/occ_logical_flows_and_wireframes.pdf
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonHidden, ButtonEnabled), true );
    QCOMPARE( mouseClickFinish(), true );
#endif
}

/*!
 * Stop verifying buttons for views that have been already verified.
 */
void TestWlanWizardUi::tc07()
{
#ifdef tc07_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWep, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    
    // Key query short pwd
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageKeyQuery), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "password"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( verifyDialogText("labelErrorNote", hbTrId("txt_occ_dialog_key_is_of_incorrect_length_please")), true );
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);

    // Key query success
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( verifyDialogText("labelErrorNote", ""), true );
    QCOMPARE( insertTextToObject("lineEditKey", "12345"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QCOMPARE( mouseClickFinish(), true );
    
#endif
}

/*!
 * 
 */
void TestWlanWizardUi::tc08()
{
#ifdef tc08_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWep, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageKeyQuery), true );
    QCOMPARE( mouseClickPrevious(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageKeyQuery), true );

#endif
}

/*!
 * 
 */
void TestWlanWizardUi::tc09()
{
#ifdef tc09_enabled

    mWlanQtUtilsContext->setScanWlanDirectResult("huuhaa3421", mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkMode), true );
    QCOMPARE( verifyActionButtons(ButtonEnabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true ); 
    QCOMPARE( mouseClickPrevious(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( mouseClickNext(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkMode), true );
    QCOMPARE( selectRadioButton( "list", 0 ), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( verifyActionButtons(ButtonEnabled, ButtonEnabled, ButtonEnabled, ButtonHidden), true );
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkSecurity), true );
    QCOMPARE( verifyActionButtons(ButtonEnabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true ); 
    QCOMPARE( mouseClickPrevious(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkMode), true );
    QCOMPARE( mouseClickNext(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkSecurity), true );
    QCOMPARE( selectRadioButton( "list", 0 ), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( verifyActionButtons(ButtonEnabled, ButtonEnabled, ButtonEnabled, ButtonHidden), true );
    QCOMPARE( mouseClickNext(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonHidden, ButtonEnabled), true );
    QCOMPARE( mouseClickFinish(), true );
#endif    
}

/*!
 * 
 */
void TestWlanWizardUi::tc10()
{
#ifdef tc10_enabled
    mMainWindow->setOrientation(Qt::Horizontal, false);
    QTest::qWait(WaitTimeForUi);

    mWlanQtUtilsContext->setScanWlanDirectResult("huuhaa3421", mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkMode), true );
    QCOMPARE( mouseClickPrevious(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( mouseClickNext(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkMode), true ); 
    QCOMPARE( selectRadioButton( "list", 0 ), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkSecurity), true );
    QCOMPARE( mouseClickPrevious(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkMode), true );
    QCOMPARE( mouseClickNext(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageNetworkSecurity), true );
    QCOMPARE( selectRadioButton( "list", 0 ), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QCOMPARE( mouseClickFinish(), true );
#endif    
}

/*!
 * 
 */
void TestWlanWizardUi::tc11()
{
#ifdef tc11_enabled
    mMainWindow->setOrientation(Qt::Horizontal, false);
    QTest::qWait(WaitTimeForUi);

    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWep, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    
    // Key query short pwd
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageKeyQuery), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "password"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( verifyDialogText("labelErrorNote", hbTrId("txt_occ_dialog_key_is_of_incorrect_length_please")), true );
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);

    // Key query success
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "12345"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QCOMPARE( mouseClickFinish(), true );
#endif    
}

/*!
 * 
 */
void TestWlanWizardUi::tc12()
{
#ifdef tc12_enabled

    mView->mWizard->setParameters("huuhaa3421",
        CMManagerShim::Infra,
        CMManagerShim::WlanSecModeWep,
        false, false, false);

    mView->showWizard();

    mMainWindow->setOrientation(Qt::Horizontal, false);
    QTest::qWait(WaitTimeForUi);

    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    QTest::qWait(4000);

#endif    
}

/*
 * 
 */
void TestWlanWizardUi::tc13()
{
#ifdef tc13_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWep, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageKeyQuery), true );
    
    QCOMPARE( insertTextToObject("lineEditKey", "passworddd"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( verifyDialogText("labelErrorNote", hbTrId("txt_occ_dialog_illegal_characters_in_key_please_c")), true );
    QTest::qWait(WaitTimeForUi);

#endif
}

void TestWlanWizardUi::tc14()
{
#ifdef tc14_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa, true, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QTest::qWait(2000);
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageKeyQuery), true );
    
    QCOMPARE( insertTextToObject("lineEditKey", "1234"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( verifyDialogText("labelErrorNote", hbTrId("txt_occ_dialog_preshared_key_too_short_at_least")), true );
    QTest::qWait(WaitTimeForUi);
#endif
}

/*
 * wpa psk
 */
void TestWlanWizardUi::tc15()
{
#ifdef tc15_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa, true, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QTest::qWait(2000);
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageKeyQuery), true );

    QCOMPARE( insertTextToObject("lineEditKey", "1234567890"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QCOMPARE( mouseClickFinish(), true );
#endif
}

/*
 * no wpa psk
 */
void TestWlanWizardUi::tc16()
{
#ifdef tc16_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QCOMPARE( mouseClickFinish(), true );
#endif
}

/*
 * wpa2 psk
 */
void TestWlanWizardUi::tc17()
{
#ifdef tc17_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa2, true, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QTest::qWait(2000);
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageKeyQuery), true );

    QCOMPARE( insertTextToObject("lineEditKey", "1234567890"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QCOMPARE( mouseClickFinish(), true );
#endif
}

/*
 * wpa2 no psk
 */
void TestWlanWizardUi::tc18()
{
#ifdef tc18_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa2, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QCOMPARE( mouseClickFinish(), true );
#endif
}

/*
 * wpa2 no psk
 */
void TestWlanWizardUi::tc19()
{
#ifdef tc19_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa2, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(0, false);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(false);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 20, 500), false );
#endif
}

/*
 * Wlan network closed -> generic error
 */
void TestWlanWizardUi::tc20()
{
#ifdef tc20_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa2, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkClosed(100, 1);
    mWlanQtUtilsContext->setConnectionSuccessed(false);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageGenericError, 10, 500), true );
    
#endif
}

/*
 * adhoc no psk
 */
void TestWlanWizardUi::tc21()
{
#ifdef tc21_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Adhoc, CMManagerShim::WlanSecModeOpen, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    
#endif
}

/*
 * 802_1x
 */
void TestWlanWizardUi::tc22()
{
#ifdef tc22_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecMode802_1x, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
#endif
}

/*
 * Wapi
 */
void TestWlanWizardUi::tc23()
{
#ifdef tc23_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWapi, false, false);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
#endif
}

/*
 * Multiple AP scan match results
 */
void TestWlanWizardUi::tc24()
{
#ifdef tc24_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa, false, false, 100);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false, 400);
    mApList->Add("foobar", CMManagerShim::Infra, CMManagerShim::WlanSecModeWep, false, false, 300);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false, 200);

    mWlanQtUtilsContext->setScanWlanApsResult(mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
#endif
}

/*
 * Multiple Direct scan match results
 */
void TestWlanWizardUi::tc25()
{
#ifdef tc25_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa, false, false, 100);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false, 400);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWep, false, false, 300);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false, 200);

    mWlanQtUtilsContext->setScanWlanDirectResult("huuhaa3421", mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSsid), true );
    QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
#endif
}

/*!
 * Connect to open:
 * - verifies IAP settings
 * - ICT success
 * - Finish button pressed
 */
void TestWlanWizardUi::tc_connect_to_open_success()
{
#ifdef tc_connect_to_open_success_enabled
    tc_connect_success(
        "tc_connect_to_open_success",
        CMManagerShim::Infra,
        CMManagerShim::WlanSecModeOpen,
        false,
        "" );
#endif 
}

/*!
 * Connect to open
 */
void TestWlanWizardUi::tc_connect_to_open_success_hidden()
{
#ifdef tc_connect_to_open_success_hidden_enabled
    tc_connect_success(
        "tc_connect_to_open_success_hidden",
        CMManagerShim::Infra,
        CMManagerShim::WlanSecModeOpen,
        true,
        "" );
#endif 
}

/*!
 * Connect to open
 */
void TestWlanWizardUi::tc_connect_to_open_success_adhoc()
{
#ifdef tc_connect_to_open_success_adhoc_enabled
    tc_connect_success(
        "tc_connect_to_open_success_adhoc",
        CMManagerShim::Adhoc,
        CMManagerShim::WlanSecModeOpen,
        true,
        "" );
#endif 
}

/*!
 * Connect to open
 */
void TestWlanWizardUi::tc_connect_to_wep_success()
{
#ifdef tc_connect_to_wep_success_enabled
    tc_connect_success(
        "tc_connect_to_wep_success",
        CMManagerShim::Adhoc,
        CMManagerShim::WlanSecModeWep,
        true,
        "abcde" );
#endif 
}


/*!
 * Connect to open
 */
void TestWlanWizardUi::tc_connect_to_wpa_psk_success()
{
#ifdef tc_connect_to_wpa_psk_success_enabled
    tc_connect_success(
        "tc_connect_to_wpa_psk_success",
        CMManagerShim::Infra,
        CMManagerShim::WlanSecModeWpa,
        false,
        "password" );
#endif 
}
/*!
 * Connect to open
 */
void TestWlanWizardUi::tc_connect_to_wpa2_psk_success()
{
#ifdef tc_connect_to_wpa2_psk_success_enabled
    tc_connect_success(
        "tc_connect_to_wpa2_psk_success",
        CMManagerShim::Adhoc,
        CMManagerShim::WlanSecModeWpa2,
        false,
        "password" );
#endif 
}


/*!
 * Connect to open:
 * - Opens
 * - ICT success
 * - Cancel pressed in summary page
 */
void TestWlanWizardUi::tc_connect_to_open_success_cancel()
{
#ifdef tc_connect_to_open_success_cancel_enabled

    mWlanQtUtilsContext->setCreateWlanIapResult(3);
    mWlanQtUtilsContext->setConnectionSuccessed(true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(3);
    mWlanQtUtilsContext->setSignalIctResult(3, true);
    
    mView->mWizard->setParameters(
        "tc_connect_to_open_success_cancel",
        CMManagerShim::Infra,
        CMManagerShim::WlanSecModeOpen,
        false, false, false);
    
    mView->showWizard();
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QTest::qWait(WaitTimeForUi);
    
    // Ignore previous calls
    mWlanQtUtilsContext->calledMethods();
    
    QCOMPARE( mouseClickCancel(), true );
    
    QStringList calledMethods;
    calledMethods 
        << "disconnectIap"
        << "deleteIap";
    
    QCOMPARE(mWlanQtUtilsContext->calledMethods(), calledMethods);
    QCOMPARE( mView->verifyStatus(TestView::WizardStatusSignalCancelled), true);
#endif 
}

/*!
 * Helpper test case for testing success case with compinations of provided
 * parameters.
 */
void TestWlanWizardUi::tc_connect_success(
    const QString &ssid,
    int networkMode,
    int securityMode,
    bool hidden,
    QString key)
{
    
    mWlanQtUtilsContext->setCreateWlanIapResult(2);
    mWlanQtUtilsContext->setConnectionSuccessed(true);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(2);
    mWlanQtUtilsContext->setSignalIctResult(2, true);
    
    mView->mWizard->setParameters(
        ssid,
        networkMode,
        securityMode,
        true, 
        hidden,
        false);

    WlanQtUtilsAp ap;
    ap.setValue(WlanQtUtilsAp::ConfIdConnectionMode, networkMode);
    ap.setValue(WlanQtUtilsAp::ConfIdSecurityMode, securityMode);
    ap.setValue(WlanQtUtilsAp::ConfIdSsid, ssid);
    ap.setValue(WlanQtUtilsAp::ConfIdHidden, hidden);
    
    QStringList calledMethods;
    calledMethods << "WlanQtUtils";
    QCOMPARE(mWlanQtUtilsContext->calledMethods(), calledMethods);

    mView->showWizard();
    if (securityMode == CMManagerShim::WlanSecModeWep ||
        securityMode == CMManagerShim::WlanSecModeWpa ||
        securityMode == CMManagerShim::WlanSecModeWpa2) {
        // Key query short pwd
        QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageKeyQuery), true );
        QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonEnabled, ButtonHidden), true );
        QTest::qWait(WaitTimeForUi);
        
        QCOMPARE( mouseClickObject("lineEditKey"), true );
        QTest::qWait(WaitTimeForUi);
        
        if (securityMode == CMManagerShim::WlanSecModeWep) {
            ap.setValue(WlanQtUtilsAp::ConfIdWepKey1, key );
            ap.setValue(WlanQtUtilsAp::ConfIdWepKey2, key );
            ap.setValue(WlanQtUtilsAp::ConfIdWepKey3, key );
            ap.setValue(WlanQtUtilsAp::ConfIdWepKey4, key );
            ap.setValue(WlanQtUtilsAp::ConfIdWepDefaultIndex, CMManagerShim::WepKeyIndex1);
            QCOMPARE( insertTextToObject("lineEditKey", "abcde"), true );
        } 
        else {
            QCOMPARE( insertTextToObject("lineEditKey", "password"), true );
            ap.setValue(WlanQtUtilsAp::ConfIdWpaPsk, key );
            ap.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, true );
        }
        QCOMPARE( mouseClickObject("dialog"), true );
        
        QTest::qWait(WaitTimeForUi);
        QCOMPARE( mouseClickNext(), true );

        QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
        QCOMPARE( verifyActionButtons(ButtonDisabled, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
        QTest::qWait(WaitTimeForUi);
        
        QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
        QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonHidden, ButtonEnabled), true );
        QTest::qWait(WaitTimeForUi);
    }
    else {
        QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
        QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonDisabled, ButtonHidden), true );
        
        QTest::qWait(WaitTimeForUi);
        QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
        QCOMPARE( verifyActionButtons(ButtonHidden, ButtonEnabled, ButtonHidden, ButtonEnabled), true );
        QTest::qWait(WaitTimeForUi);
    }
    
    calledMethods.clear();
    calledMethods 
        << "createIap" 
        << "activeIap"
        << "connectIap";
    QCOMPARE( mWlanQtUtilsContext->calledMethods(), calledMethods);
    
    QCOMPARE( mWlanQtUtilsContext->verifyWlanIapSettings(ap), true);
    
    QTest::qWait(WaitTimeForUi);
    QCOMPARE( mouseClickFinish(), true );
    QCOMPARE( mView->verifyStatus(TestView::WizardStatusSignalFinished, 2), true);
}

/*
 * Ict Result enumerator used. Hotspot.
 */
void TestWlanWizardUi::tc26()
{
#ifdef tc26_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa, false, false, 100);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false, 400);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWep, false, false, 300);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false, 200);

    mWlanQtUtilsContext->setScanWlanDirectResult("huuhaa3421", mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, IctsHotspotPassed);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(1000);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(1000);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(1000);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageSummary, 10, 500), true );
    QTest::qWait(5000);
#endif
}

/*
 * Ict Result enumerator used. Ict canceled.
 */
void TestWlanWizardUi::tc27()
{
#ifdef tc27_enabled
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWpa, false, false, 100);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false, 400);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeWep, false, false, 300);
    mApList->Add("huuhaa3421", CMManagerShim::Infra, CMManagerShim::WlanSecModeOpen, false, false, 200);

    mWlanQtUtilsContext->setScanWlanDirectResult("huuhaa3421", mApList->List());
    mWlanQtUtilsContext->setCreateWlanIapResult(100);
    mWlanQtUtilsContext->setSignalIctResult(100, IctsCanceled);
    mWlanQtUtilsContext->setSignalWlanNetworkOpened(100);
    mWlanQtUtilsContext->setConnectionSuccessed(true);

    QCOMPARE( mouseClickObject("lineEditKey"), true );
    QTest::qWait(1000);
    QCOMPARE( insertTextToObject("lineEditKey", "huuhaa3421"), true );
    QTest::qWait(1000);
    QCOMPARE( mouseClickObject("dialog"), true );
    QTest::qWait(1000);
    QCOMPARE( mouseClickNext(), true );

    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageScanning), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageProcessSettings), true );
    QCOMPARE( verifyCurrentPage(WlanWizardPageInternal::PageGenericError, 10, 500), true );
    QTest::qWait(5000);
#endif
}

/*!
 * Filter to catch focus event to the text editor widget.
 */
bool TestWlanWizardUi::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == mMainWindow && event->type() == QEvent::Show) {
        mMainWindow->removeEventFilter(this);
        mEvent = true;
    }
    return false;
}

QGraphicsWidget* TestWlanWizardUi::findChildItem(const QString &itemName, QGraphicsWidget *widget)
{
    QList<QGraphicsItem*> list = widget->childItems();
    for (int i = 0; i < list.size(); i++) {
        QGraphicsWidget* item = (QGraphicsWidget*) list[i];
        if (item->objectName() == itemName) {
            return item;
        }
        else if ((item = findChildItem(itemName, item))) {
            return item;
        }
    }
    return 0;
}

bool TestWlanWizardUi::verifyCurrentPage(int pageId, int retries, int wait_ms)
{
    for (int i = 0; i < retries; i++) {
        QTest::qWait(wait_ms);

        WlanWizardPrivate *pPrivate = mView->mWizard->d_ptr;
        HbWidget* current = qobject_cast<HbWidget*> (pPrivate->mStackedWidget->currentWidget());
        WlanWizardPage *page = pPrivate->mPageMapper[current];

        // TODO: verify title in this method
        
        if (page == pPrivate->mPages[pageId]) {
            switch(pageId) {
            case WlanWizardPageInternal::PageEapStart:
                return true;
            case WlanWizardPageInternal::PageGenericError:
                return true;
            case WlanWizardPageInternal::PageKeyQuery:
                // TODO: add parameter SSID, FAILS without it
                return true; // verifyDialogText("dialog", hbTrId("txt_occ_dialog_enter_key_for_1"));
            case WlanWizardPageInternal::PageNetworkMode:
                return verifyDialogText("dialog_6", hbTrId("txt_occ_dialog_select_network_mode_and_status"));
            case WlanWizardPageInternal::PageNetworkSecurity:
                return verifyDialogText("dialog_6", hbTrId("txt_occ_dialog_select_network_security_mode"));
            case WlanWizardPageInternal::PageProcessSettings:
                // TODO: add parameter SSID, FAILS without it
                return true; // verifyDialogText("dialog", hbTrId("txt_occ_dialog_checking_connection_to_1"));
            case WlanWizardPageInternal::PageScanning:
                return true; // verifyDialogText("dialog", hbTrId("TODO_1"));
            case WlanWizardPageInternal::PageSsid:
                return verifyDialogText("dialog", hbTrId("txt_occ_dialog_insert_the_name_of_the_new_wlan_net"));
            case WlanWizardPageInternal::PageSummary:
                if (pPrivate->mTitle->plainText() != hbTrId("txt_occ_title_wlan_setup_wizard_summary")){
                    qWarning("TestWlanWizardUi::verifyCurrentPage: Invalid title");
                    return false;
                }
                return true;
            case WlanWizardPageInternal::PageWpsStart:
                return true;
            default:
                return true;
            }
        }
    }
    qWarning("verifyCurrentPage: expected: %d", pageId);
    return false;
}

bool TestWlanWizardUi::verifyDialogText(const QString objName, const QString text)
{
    HbWidget* current = qobject_cast<HbWidget*> (
        mView->mWizard->d_ptr->mStackedWidget->currentWidget());
    HbLabel* widget = (HbLabel*) findChildItem(objName, current);

    if (widget) {
        if(widget->plainText() == text) {
            return true;
        }
        else {
            qWarning("verifyDialogText: not match");
            qDebug() << "expect: " << text;
            qDebug() << "actual: " << widget->plainText();
        }
    } else {
        qWarning("verifyDialogText: object not found");
    }
    
    return false;
}

bool TestWlanWizardUi::verifyActionButtons(ButtonStatus prevStatus,
    ButtonStatus cancelStatus,
    ButtonStatus nextStatus,
    ButtonStatus finishStatus)
{
    WlanWizardPrivate *pPrivate = mView->mWizard->d_ptr;
    ButtonStatus prevReally = ButtonHidden;
    ButtonStatus cancelReally = ButtonHidden;
    ButtonStatus nextReally = ButtonHidden;
    ButtonStatus finishReally = ButtonHidden;

    if(pPrivate->mActionPrevious->isVisible()) {
        if(pPrivate->mActionPrevious->isEnabled()) {
            prevReally = ButtonEnabled;
        } else {
            prevReally = ButtonDisabled;
        }
    }

    if(pPrivate->mActionCancel->isVisible()) {
        if(pPrivate->mActionCancel->isEnabled()) {
            cancelReally = ButtonEnabled;
        } else {
            cancelReally = ButtonDisabled;
        }
    }

    if(pPrivate->mActionNext->isVisible()) {
        if(pPrivate->mActionNext->isEnabled()) {
            nextReally = ButtonEnabled;
        } else {
            nextReally = ButtonDisabled;
        }
    }

    if(pPrivate->mActionFinish->isVisible()) {
        if(pPrivate->mActionFinish->isEnabled()) {
            finishReally = ButtonEnabled;
        } else {
            finishReally = ButtonDisabled;
        }
    }
    bool ret = true;
    if (prevReally != prevStatus){
        qWarning("Previous Button: really: %d, status: %d", prevReally, prevStatus);
        ret = false;
    }
    if (cancelReally != cancelStatus){
        qWarning("Cancel Button: really: %d, status: %d", cancelReally, cancelStatus);
        ret = false;
    }
    if (nextReally != nextStatus){
        qWarning("Next Button: really: %d, status: %d", nextReally, nextStatus);
        ret = false;
    }
    if (finishReally != finishStatus){
        qWarning("Finish Button: really: %d, status: %d", finishReally, finishStatus);
        ret = false;
    }
    
    return ret;
}

bool TestWlanWizardUi::mouseClickObject(const QString objName)
{
    HbWidget* current = qobject_cast<HbWidget*> (
        mView->mWizard->d_ptr->mStackedWidget->currentWidget());
    HbWidget* widget = (HbWidget*) findChildItem(objName, current);
    
    if (widget) {
        HbAutoTest::mouseClick(mMainWindow, widget);
        widget->clearFocus();
        widget->setFocus();
        return true;
    }
    qWarning("mouseClickObject: object not found");
    
    return false;
}

bool TestWlanWizardUi::insertTextToObject(const QString objName, const QString text)
{
    HbWidget* current = qobject_cast<HbWidget*> (
        mView->mWizard->d_ptr->mStackedWidget->currentWidget());
    HbWidget* widget = (HbWidget*) findChildItem(objName, current);

    if (widget) {
        for (int i = 0; i < text.size(); i++) {
            HbAutoTest::keyPress(mMainWindow, text.at(i).toAscii(), 0, 10);
            QTest::qWait(20);
        }
        return true;
    }
    qWarning("insertTextToObject: object not found");
    return false;
}

bool TestWlanWizardUi::selectRadioButton(const QString objName, int index)
{
    HbWidget* current = qobject_cast<HbWidget*> (
        mView->mWizard->d_ptr->mStackedWidget->currentWidget());
    TestRadioButtonList* widget = (TestRadioButtonList*) findChildItem(objName, current);

    if (widget) {
        widget->setSelected(index);
        widget->emitActivated(widget->currentIndex());
        return true;
    }
    return false;
}

bool TestWlanWizardUi::mouseClickNext()
{
    if (mView->mWizard->d_ptr->mActionNext->isEnabled()) {
        mView->mWizard->d_ptr->mActionNext->activate(QAction::Trigger);
        return true;
    }
    qWarning("Next button not enabled");
    return false;
}

bool TestWlanWizardUi::mouseClickPrevious()
{
    if (mView->mWizard->d_ptr->mActionPrevious->isEnabled()) {
        mView->mWizard->d_ptr->mActionPrevious->activate(QAction::Trigger);
        return true;
    }
    qWarning("Previous button not enabled");
    return false;
}

bool TestWlanWizardUi::mouseClickCancel()
{
    if (mView->mWizard->d_ptr->mActionCancel->isEnabled()) {
        mView->mWizard->d_ptr->mActionCancel->activate(QAction::Trigger);
        return true;
    }
    qWarning("Cancel button not enabled");
    return false;
}

bool TestWlanWizardUi::mouseClickFinish()
{
    if (mView->mWizard->d_ptr->mActionFinish->isEnabled()) {
        mView->mWizard->d_ptr->mActionFinish->activate(QAction::Trigger);
        return true;
    }
    qWarning("Finish button not enabled");
    return false;
}

TestView::TestView() : mWizard(NULL)
{
    qDebug("TestView::TestView()");
}

TestView::~TestView()
{
    qDebug("TestView::~TestView()");
}

void TestView::createWizard()
{
    qDebug("TestView::createWizard");
    Q_ASSERT(mWizard == NULL);
    mWizard = new WlanWizard(mainWindow());
    bool ok;
    ok = connect(
        mWizard, SIGNAL(finished(int, bool)), 
        this, SLOT(finished(int, bool)), 
        Qt::QueuedConnection);
    Q_ASSERT(ok);

    ok = connect(
        mWizard, SIGNAL(cancelled()), 
        this, SLOT(cancelled()),
        Qt::QueuedConnection);
    Q_ASSERT(ok);
    
    mWizardStatus = WizardStatusSignalNone;
    mConnectedIapId = -100;
}

void TestView::showWizard()
{
    qDebug("TestView::showWizard()");
    Q_ASSERT(mWizard);
    mWizard->show();
}

void TestView::deleteWizard()
{
    qDebug("TestView::deleteWizard");

    Q_ASSERT(mWizard != NULL);
    QTest::qWait(10);
    QTest::qWait(10); 
    QMetaObject::invokeMethod(mWizard, "deleteLater", Qt::QueuedConnection); 
    QTest::qWait(10); 
    mWizard = NULL; 
}

void TestView::finished(int iapId, bool connected)
{
    qDebug("TestView::complete(), iap id: %d, connected: %d", iapId, connected);
    
    if (mWizardStatus != WizardStatusSignalNone) {
        qWarning("TestView::finished: multiple signals received");
        mWizardStatus = WizardStatusSignalUndefined;
    } else {
        mWizardStatus = WizardStatusSignalFinished;
        mConnectedIapId = iapId;
    }
}

void TestView::cancelled()
{
    qDebug("TestView::cancelled()");
    if (mWizardStatus != WizardStatusSignalNone) {
        qWarning("TestView::cancelled: multiple signals received");
        mWizardStatus = WizardStatusSignalUndefined;
    } else {
        mWizardStatus = WizardStatusSignalCancelled;
    }
}

bool TestView::verifyStatus(WizardStatusSignal status, int iapId )
{
    // Since connections to cancelled and finished signals are queued
    // we need to use qWait() here.
    QTest::qWait(100);
    bool ret = true;
    if (status != mWizardStatus){
        qWarning("TestView::verifyStatus, status: expected: %d, actual: %d", status, mWizardStatus);
        ret = false;
    }
    if (status == WizardStatusSignalFinished) {
        if (iapId != mConnectedIapId) {
            qWarning("TestView::verifyStatus, iapid: expected: %d, actual: %d", iapId, mConnectedIapId);
            ret = false;
        }
    }
    return ret;
}
