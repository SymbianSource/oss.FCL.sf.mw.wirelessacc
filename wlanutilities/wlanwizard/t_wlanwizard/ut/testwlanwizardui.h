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

#ifndef TESTWLANWIZARDRNDUI_H_
#define TESTWLANWIZARDRNDUI_H_

#include <QObject>
#include <HbView>
#include <QSharedPointer>

class QGraphicsItem;
class HbAutoTestMainWindow;
class WlanWizard;
class WlanQtUtilsContext;
class WlanQtUtils;
class WlanQtUtilsAp;
class QString;

class ContextWlanApList
{
public:
    ContextWlanApList();
    ~ContextWlanApList();
    
    void Add(QString name,
        int netMode,
        int secMode,
        bool wpaPskInUse,
        bool wpsSupported,
        int signalStrength = 0);
    
    void clear();
    
    const QList<QSharedPointer<WlanQtUtilsAp> > &List() { return mList; }
private:
    QList<QSharedPointer<WlanQtUtilsAp> > mList;
};

class TestView: public HbView
{
Q_OBJECT

public: 
    enum WizardStatusSignal{
        WizardStatusSignalNone,
        WizardStatusSignalFinished,
        WizardStatusSignalCancelled,
        WizardStatusSignalUndefined,
    };

public:
    TestView();
    virtual ~TestView();
       
    void createWizard();
    void showWizard();
    
    bool verifyStatus(WizardStatusSignal status, int iapId = -100);
    
private slots:
    void finished(int iapId, bool connected);
    void cancelled();

public:
    void deleteWizard();
    
public:
    WlanWizard *mWizard;

private:
    WizardStatusSignal mWizardStatus;
    int mConnectedIapId;
};


class TestWlanWizardUi: public QObject
{
Q_OBJECT

public slots:
    // Test framework functions
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    // Test cases
    void tcStartWizard();
    void tc01();
    void tc02();
    void tc03();
    void tc04();
    void tc05();
    void tc06();
    void tc07();
    void tc08();
    void tc09();
    void tc10();
    void tc11();
    void tc12();
    void tc13();
    void tc14();
    void tc15();
    void tc16();
    void tc17();
    void tc18();
    void tc19();
    void tc20();
    void tc21();
    void tc22();
    void tc23();
    void tc24();
    void tc25();
    void tc26();
    void tc27();
    void tc_connect_to_open_success();
    void tc_connect_to_open_success_hidden();
    void tc_connect_to_open_success_adhoc();
    void tc_connect_to_wep_success();
    void tc_connect_to_wpa_psk_success();
    void tc_connect_to_wpa2_psk_success();
    void tc_connect_to_open_success_cancel();
    
protected:
    
    enum ButtonStatus {
        ButtonHidden,
        ButtonDisabled,
        ButtonEnabled
    };
    /*!
     * Reimplements QObject function that is invoked on any event. This
     * function handles only a focus event to mSsid edit field.
     * @param obj contains the object, where the event originates from.
     * @param event contains the event id.
     */
    bool eventFilter(QObject *obj, QEvent *event);
    
    QGraphicsWidget* findChildItem(const QString &itemName, QGraphicsWidget *widget);
    
    bool verifyCurrentPage(int pageId, int retries = 10, int wait_ms = 200);
    bool verifyDialogText(const QString objName, const QString text);
    bool verifyActionButtonVisibility(bool prevVisible, bool cancelVisible,
        bool nextVisible, bool finishVisible);
    bool verifyActionButtons(ButtonStatus prevStatus, ButtonStatus cancelStatus,
        ButtonStatus nextStatus, ButtonStatus finishStatus);
    bool mouseClickObject(const QString objName);
    bool insertTextToObject(const QString objName, const QString text);
    bool selectRadioButton(const QString objName, int index);
    
    bool mouseClickNext();
    bool mouseClickPrevious();
    bool mouseClickCancel();
    bool mouseClickFinish();
    
private:
    
private:
    void tc_connect_success(
        const QString &ssid,
        int networkMode,
        int securityMode,
        bool hidden,
        QString key);
    
    bool mEvent;

    // Test data
    TestView *mView;
    HbAutoTestMainWindow *mMainWindow;
    
    WlanQtUtilsContext *mWlanQtUtilsContext;
    
    ContextWlanApList *mApList;   
    
    bool mNoCleanup;
    
};

#endif /* TESTWLANWIZARDRNDUI_H_ */
