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

#ifndef WLANSNIFFERLISTVIEW_H
#define WLANSNIFFERLISTVIEW_H

#include <HbView>
#include "wlanqtutilscommon.h"

class HbListWidget;
class HbListWidgetItem;
class HbLabel;
class WlanSniffer;
class WlanQtUtilsWlanIap;
class WlanQtUtilsWlanAp;

class WlanSnifferListView : public HbView
    {
    Q_OBJECT
    
public:
    WlanSnifferListView(WlanSniffer *appRef);
    virtual ~WlanSnifferListView();
    
    void update();
    void updateConnectionOpened(int iapId);
    void updateConnectionClosed(int iapId);

signals:
    void detailsTriggered(int);
    void completeServiceTriggered();

private slots:
    void handleListItemActivated(HbListWidgetItem *item);
    void handleListItemLongPressed(HbListWidgetItem *item, const QPointF &coords);
    void handleDisconnect();
    
private:
    QString signalStrengthIconChoose(int signalStrength, WlanQtUtilsWlanSecMode secMode);
    HbListWidgetItem *wlanListItemIapCreate(const WlanQtUtilsWlanIap *iap);
    HbListWidgetItem *wlanListItemApCreate(const WlanQtUtilsWlanAp *ap);
    
private:
    HbListWidget *mWlanList;
    HbLabel *mStatusLabel;
    WlanSniffer *mAppRef;
    int mConnectingIapId;
    bool mIapItemMenuOpen;
    };

#endif
