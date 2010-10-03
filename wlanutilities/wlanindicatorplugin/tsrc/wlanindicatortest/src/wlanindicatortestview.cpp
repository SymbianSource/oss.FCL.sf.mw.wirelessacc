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

#include <qgraphicswidget>
#include <hbmenu.h>
#include <hbaction.h>
#include <hbpushbutton.h>
#include <hbdocumentloader.h>
#include <hbinstance.h>
#include <hbinputdialog.h>

#include <hbindicator.h>
#include <hbindicatorplugininterface.h>
#include <hbindicatorinterface.h>

#include "wlanindicatortest.h"
#include "wlanindicatortestview.h"

WlanIndicatorTestView::WlanIndicatorTestView(WlanIndicatorTest *appRef) : 
    mAppRef(appRef)
{
    bool ok = false;
    mWlanIap = "Access Point";
    HbDocumentLoader loader;
    if (HbInstance::instance()->allMainWindows().value(0)->orientation() == Qt::Horizontal) {
        loader.load(":/docml/wlanindicatortestview.docml", &ok);
    }
    else {
        loader.load(":/docml/wlanindicatortestview_portrait.docml", &ok);
    }       
    Q_ASSERT_X(ok, "Wlan Indicator Test", "Invalid docml file");
    
    // Load the view by name from the xml file
    QGraphicsWidget *widget = loader.findWidget("view");
    Q_ASSERT_X(widget != 0, "Wlan Indicator Test", "View not found");
    
    setWidget(widget);

    HbMenu *viewMenu = qobject_cast<HbMenu *>(loader.findWidget("viewMenu"));
    Q_ASSERT_X(viewMenu != 0, "Wlan Indicator Test", "Menu not found");
    setMenu(viewMenu);
   
    mTestButton2 = qobject_cast<HbPushButton *>(loader.findWidget("pushButton_2"));
    Q_ASSERT_X(mTestButton2 != 0, "Wlan Indicator Test", "Button2 not found");
    
    mTestButton3 = qobject_cast<HbPushButton *>(loader.findWidget("pushButton_3"));
    Q_ASSERT_X(mTestButton3 != 0, "Wlan Indicator Test", "Button3 not found");
    
    mTestButton4 = qobject_cast<HbPushButton *>(loader.findWidget("pushButton_4"));
    Q_ASSERT_X(mTestButton4 != 0, "Wlan Indicator Test", "Button4 not found");
    
    mTestButton5 = qobject_cast<HbPushButton *>(loader.findWidget("pushButton_5"));
    Q_ASSERT_X(mTestButton5 != 0, "Wlan Indicator Test", "Button5 not found");
    
    mTestButton6 = qobject_cast<HbPushButton *>(loader.findWidget("pushButton_6"));
    Q_ASSERT_X(mTestButton6 != 0, "Wlan Indicator Test", "Button6 not found");
        
    mTestButton7 = qobject_cast<HbPushButton *>(loader.findWidget("pushButton_7"));
    Q_ASSERT_X(mTestButton7 != 0, "Wlan Indicator Test", "Button7 not found");

    mTestButton8 = qobject_cast<HbPushButton *>(loader.findWidget("pushButton_8"));
    Q_ASSERT_X(mTestButton8 != 0, "Wlan Indicator Test", "Button8 not found");
    
    HbAction *exitAction = qobject_cast<HbAction *>(loader.findObject("exitAction"));
    Q_ASSERT_X(exitAction != 0, "Wlan Indicator Test", "Exit action not found");
    
    
    bool connectStatus = connect(
        mTestButton2,
        SIGNAL(clicked()),
        this,
        SLOT(handleWlanPushButtonPressed()));
    Q_ASSERT(connectStatus == true);
    connectStatus = connect(
        mTestButton3,
        SIGNAL(clicked()),
        this,
        SLOT(handleWlanPushButtonPressed()));
    Q_ASSERT(connectStatus == true);
    connectStatus = connect(
        mTestButton4,
        SIGNAL(clicked()),
        this,
        SLOT(handleWlanPushButtonPressed()));
    Q_ASSERT(connectStatus == true);
    connectStatus = connect(
        mTestButton5,
        SIGNAL(clicked()),
        this,
        SLOT(handleWlanPushButtonPressed()));
    Q_ASSERT(connectStatus == true);
    connectStatus = connect(
        mTestButton6,
        SIGNAL(clicked()),
        this,
        SLOT(handleGprsPushButtonPressed()));
    Q_ASSERT(connectStatus == true);
    connectStatus = connect(
        mTestButton7,
        SIGNAL(clicked()),
        this,
        SLOT(handleGprsPushButtonPressed()));
    Q_ASSERT(connectStatus == true);
    connectStatus = connect(
        mTestButton8,
        SIGNAL(clicked()),
        this,
        SLOT(handleWlanPushButtonPressed()));
    Q_ASSERT(connectStatus == true);
    connectStatus = connect(
        exitAction,
        SIGNAL(triggered(bool)),
        this,
        SIGNAL(exitTriggered()));
    Q_ASSERT(connectStatus == true);

    
    // Set correct orientation when drawing view first time. Orientation change
    // on the fly is not yet implemented
    if (HbInstance::instance()->allMainWindows().value(0)->orientation() == Qt::Horizontal) {
        bool loaded = false;
        loader.load( ":/docml/wlanindicatortestview.docml", "landscape", &loaded );
    } 
    else {
        bool loaded = false;
        loader.load( ":/docml/wlanindicatortestview_portrait.docml", "portrait", &loaded );
    }
}
    
WlanIndicatorTestView::~WlanIndicatorTestView()
{
}



void WlanIndicatorTestView::handleWlanPushButtonPressed()
{
    HbPushButton *item = (HbPushButton *)sender();
    
    QList<QVariant> list;
    QString test = item->text();
    
    if (test == "No Wlan Networks") {
        list.insert(0,0);

    } else if (test == "Wlan Connected") {
        list.insert(0,1);
        list.insert(1,"lahna");
        // list.insert(2,"lahna");
/*
    } else if (test == "Unknown Wlan Found") {
        list.insert(0,4);
        list.insert(1,12);            
    } else if (test == "WLAN connected") {
        list.insert(0,5);
        list.insert(1,0); //no. of networks not used but must be there
        QString label("Connected to");
        bool ok = false;
        mWlanIap = HbInputDialog::getText(label, mWlanIap, &ok, scene(), this);
        list.insert(2,mWlanIap);
*/       
    }

    HbIndicator indicator;
    if (test == "WLAN disconnected") {
        indicator.deactivate("com.nokia.hb.indicator.connectivity.wlanindicatorplugin/1.0");
    }
    else {    
        indicator.activate("com.nokia.hb.indicator.connectivity.wlanindicatorplugin/1.0", list);
    }
    
    int error = indicator.error();  
        
}

void WlanIndicatorTestView::handleGprsPushButtonPressed()
{
    HbPushButton *item = (HbPushButton *)sender();
    
    QList<QVariant> list;
    QString test = item->text();

    HbIndicator indicator;

    if (test == "GPRS connected") {

        list.insert(0,"Sonera"); //dummy IAP name
        list.insert(1,"12345678"); //dummy connection id
        
        indicator.activate("com.nokia.hb.indicator.connectivity.cellularindicatorplugin/1.0", list);
        
    } else if (test == "GPRS disconnected") {

        indicator.deactivate("com.nokia.hb.indicator.connectivity.cellularindicatorplugin/1.0");    
        
    }
    
    int error = indicator.error();
    
}

