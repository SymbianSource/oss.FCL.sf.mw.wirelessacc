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

#include <QGraphicsWidget>

#include <HbLabel>
#include <HbMenu>
#include <HbAction>
#include <HbListWidget>
#include <HbListWidgetItem>
#include <HbDocumentLoader>
#include <HbInstance>

#include <xqserviceutil.h>

#include "wlanqtutils.h"
#include "wlanqtutilswlanap.h"
#include "wlanqtutilswlaniap.h"
#include "wlanqtutilscommon.h"

#include "wlansniffer.h"
#include "wlansnifferlistview.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlansnifferlistviewTraces.h"
#endif

WlanSnifferListView::WlanSnifferListView(WlanSniffer *appRef) : 
    mWlanList(NULL),
    mStatusLabel(NULL),
    mAppRef(appRef),
    mConnectingIapId(0),
    mIapItemMenuOpen(false)
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_WLANSNIFFERLISTVIEW_ENTRY );
    
    bool ok = false;
    HbDocumentLoader loader;
    loader.load(":/docml/wlansnifferlistview.docml", &ok);
    Q_ASSERT_X(ok, "Wlan Sniffer", "Invalid docml file");
    
    // Load the view by name from the xml file
    QGraphicsWidget *widget = loader.findWidget("occ_list");
    Q_ASSERT_X(widget != 0, "Wlan Sniffer", "View not found");
    // Set the WlanListView view to be the widget that was loaded from the xml
    setWidget(widget);

    HbMenu *viewMenu = qobject_cast<HbMenu *>(loader.findWidget("viewMenu"));
    Q_ASSERT_X(viewMenu != 0, "Wlan Sniffer", "Menu not found");
    setMenu(viewMenu);
    
    mWlanList = qobject_cast<HbListWidget *>(loader.findWidget("listWidget"));
    Q_ASSERT_X(mWlanList != 0, "Wlan Sniffer", "List not found");
    
    mStatusLabel = qobject_cast<HbLabel *>(loader.findWidget("statusLabel"));
    Q_ASSERT_X(mStatusLabel != 0, "Wlan Sniffer", "Label not found");

    connect(
        mWlanList,
        SIGNAL(activated(HbListWidgetItem *)),
        this,
        SLOT(handleListItemActivated(HbListWidgetItem *)));
    connect(
        mWlanList,
        SIGNAL(longPressed(HbListWidgetItem *, const QPointF &)),
        this,
        SLOT(handleListItemLongPressed(HbListWidgetItem *, const QPointF &)));
    
    // Set correct orientation when drawing view first time. Orientation change
    // on the fly is not yet implemented
    if (HbInstance::instance()->allMainWindows().value(0)->orientation() == Qt::Horizontal) {
        bool loaded = false;
        loader.load( ":/docml/wlansnifferlistview.docml", "landscape", &loaded );
    } // else: portrait is ok by default

    // We need to create a custom navigation action in case the application was
    // launched as embedded via QtHighway
    if (XQServiceUtil::isEmbedded()) {
        HbAction *completeAction = new HbAction(Hb::BackNaviAction, this);
        setNavigationAction(completeAction);
        connect(
            completeAction,
            SIGNAL(triggered(bool)),
            this,
            SIGNAL(completeServiceTriggered()));
        // TODO: What to do when exit action is ran? Should we close whole control panel?
    }
    
    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_WLANSNIFFERLISTVIEW_EXIT );
}
    
WlanSnifferListView::~WlanSnifferListView()
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_WLANSNIFFERLISTVIEWDESTR_ENTRY );    
    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_WLANSNIFFERLISTVIEWDESTR_EXIT );
}

void WlanSnifferListView::update()
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_UPDATE_ENTRY );
    
    if (mIapItemMenuOpen == false) {
        QList<WlanQtUtilsWlanAp*> aps;
        QList<WlanQtUtilsWlanIap*> iaps;
        
        mAppRef->wlanQtUtils()->availableWlanAps(iaps, aps);
        
        mWlanList->clear();
    
        // Loop the IAPs and APs and create the list items (lists are ordered already by Wlan Qt Utilities)
        foreach (WlanQtUtilsWlanIap *iap, iaps) {
            mWlanList->addItem(wlanListItemIapCreate(iap));
        }
        // Loop the AP's and create the list items
        foreach (WlanQtUtilsWlanAp *ap, aps) {
            mWlanList->addItem(wlanListItemApCreate(ap));
        }
    }

    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_UPDATE_EXIT );
}

void WlanSnifferListView::updateConnectionOpened(int iapId)
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_UPDATECONNECTIONOPENED_ENTRY );
    
    mStatusLabel->setPlainText(
        hbTrId("txt_occ_grid_connected_to_1").arg(mAppRef->wlanQtUtils()->iap(iapId)->name()));
    // Redraw the list so that the connected network moves on top
    update();
    mConnectingIapId = 0;

    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_UPDATECONNECTIONOPENED_EXIT );
}

void WlanSnifferListView::updateConnectionClosed(int iapId)
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_UPDATECONNECTIONCLOSED_ENTRY );
    
    (void)iapId;
    mStatusLabel->setPlainText(hbTrId("txt_occ_grid_not_connected"));
    mConnectingIapId = 0;

    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_UPDATECONNECTIONCLOSED_EXIT );
}

void WlanSnifferListView::handleListItemActivated(HbListWidgetItem *item)
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_HANDLELISTITEMACTIVATED_ENTRY );
    
    QVariant data = item->data();
    if (data.canConvert<int>()) {
        // Item was an IAP. Connect the IAP or show details view depending on connection state
        WlanQtUtilsIap *iap = mAppRef->wlanQtUtils()->iap(data.toInt());
        if (iap->connectionStatus() == WlanQtUtilsConnectionStatusConnected) {
            // Switch to details view
            emit detailsTriggered(data.toInt());
        } else {
            // Connect the IAP
            mConnectingIapId = iap->id();
            mStatusLabel->setPlainText(hbTrId("txt_occ_grid_connecting_to_1").arg(iap->name()));
            mAppRef->wlanQtUtils()->connectIap(mConnectingIapId);
        }
    } else {
        // Item has to be a WLAN AP, which can't already be connected. Create an IAP and connect it.
        Q_ASSERT(data.canConvert<WlanQtUtilsWlanAp>());
        WlanQtUtilsWlanAp ap = data.value<WlanQtUtilsWlanAp>();
        mConnectingIapId = mAppRef->wlanQtUtils()->createWlanIap(&ap);
        WlanQtUtilsIap *iap = mAppRef->wlanQtUtils()->iap(mConnectingIapId);
        Q_ASSERT(iap);
        mStatusLabel->setPlainText(hbTrId("txt_occ_grid_connecting_to_1").arg(iap->name()));
        mAppRef->wlanQtUtils()->connectIap(mConnectingIapId);
    }

    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_HANDLELISTITEMACTIVATED_EXIT );
}

void WlanSnifferListView::handleListItemLongPressed(HbListWidgetItem *item, const QPointF &coords)
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_HANDLELISTITEMLONGPRESSED_ENTRY );
    
    // Disable list refreshing while context menu is shown
    // Todo: Propably a more elegant way to do this is needed..
    mIapItemMenuOpen = true;
    
    (void)item;
    HbMenu *contextMenu = new HbMenu();
    HbAction *actionDetails = contextMenu->addAction(hbTrId("txt_occ_menu_details"));
    HbAction *actionSettings = contextMenu->addAction(hbTrId("txt_occ_menu_network_settings"));
    HbAction *actionDisable = contextMenu->addAction(hbTrId("txt_occ_menu_disable_network"));
    
    // Add disconnect option if the IAP is connected
    HbAction *actionDisconnect = 0;
    WlanQtUtilsIap *iap = 0;
    QVariant data = item->data();
    if (data.canConvert<int>()) {
        // Item was an IAP.
        iap = mAppRef->wlanQtUtils()->iap(data.toInt());
        if (iap->connectionStatus() == WlanQtUtilsConnectionStatusConnected) {
            actionDisconnect = contextMenu->addAction(hbTrId("txt_common_menu_disconnect"));
        }
    }
    
    // Show the menu
    HbAction *selectedAction = contextMenu->exec(coords);
    // Re-enable list refreshing
    mIapItemMenuOpen = false;

    if (selectedAction) {
        // Handle the "Disconnect" menu selection
        if (selectedAction == actionDisconnect) {
            mStatusLabel->setPlainText(hbTrId("txt_occ_grid_not_connected"));
            mAppRef->wlanQtUtils()->disconnectIap(iap->id());
            // Redraw the list so that disconnected network is not shown anymore
            update();
        }
    }        

    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_HANDLELISTITEMLONGPRESSED_EXIT );
}

// Todo: This is a temporary solution - real WLAN status indication & listening
// is implemented later on.
void WlanSnifferListView::handleDisconnect()
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_HANDLEDISCONNECT_ENTRY );
    
    mStatusLabel->setPlainText(hbTrId("txt_occ_grid_not_connected"));
    // Redraw the list so that disconnected network is not shown anymore
    update();    

    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_HANDLEDISCONNECT_EXIT );
}

// Todo: no place for separate secure WLAN icon in the widget -> custom layout needed?
// Todo: check the signal strength values
QString WlanSnifferListView::signalStrengthIconChoose(int signalStrength, WlanQtUtilsWlanSecMode secMode)
{
    (void)secMode;
    QString iconName;
    if (signalStrength <= WlanQtUtilsWlanSignalStrengthGood) {
        iconName = "qtg_small_signal_good";
    } else if (signalStrength <= WlanQtUtilsWlanSignalStrengthLow) {
        iconName = "qtg_small_signal_medium";
    } else {
        iconName = "qtg_small_signal_low";
    }
    
    return iconName;
}

HbListWidgetItem *WlanSnifferListView::wlanListItemIapCreate(const WlanQtUtilsWlanIap *iap)
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_WLANLISTITEMIAPCREATE_ENTRY );
    
    HbListWidgetItem* item = new HbListWidgetItem();
    item->setText(iap->name());
#if 1                                       // TODO: How to show secure/open network info??
    if (iap->securityMode() == WlanQtUtilsWlanSecModeOpen) {
        item->setSecondaryText("Open");
    } else {
        item->setSecondaryText("Secure");            
    }
#endif
    
    if (iap->connectionStatus() == WlanQtUtilsConnectionStatusConnected) {
        item->setIcon(HbIcon("qtg_small_online"));
    } else {
        item->setIcon(HbIcon("qtg_small_wlan"));
    }
    item->setSecondaryIcon(HbIcon(signalStrengthIconChoose(iap->signalStrength(), iap->securityMode())));
    QVariant data(iap->id());
    item->setData(data);

    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_WLANLISTITEMIAPCREATE_EXIT );
    return item;
}

HbListWidgetItem *WlanSnifferListView::wlanListItemApCreate(const WlanQtUtilsWlanAp *ap)
{
    OstTraceFunctionEntry0( WLANSNIFFERLISTVIEW_WLANLISTITEMAPCREATE_ENTRY );
    
    HbListWidgetItem* item = new HbListWidgetItem();
    item->setText(ap->ssid());
#if 1                                       // TODO: How to show secure/open network info??
    if (ap->securityMode() == WlanQtUtilsWlanSecModeOpen) {
        item->setSecondaryText("Open");
    } else {
        item->setSecondaryText("Secure");            
    }
#endif    
    item->setSecondaryIcon(HbIcon(signalStrengthIconChoose(ap->signalStrength(), ap->securityMode())));
    QVariant data;
    data.setValue(*ap);
    item->setData(data);
    
    OstTraceFunctionExit0( WLANSNIFFERLISTVIEW_WLANLISTITEMAPCREATE_EXIT );
    return item;
}

// End of File
