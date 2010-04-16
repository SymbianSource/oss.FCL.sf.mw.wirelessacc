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

#ifndef WLANSNIFFERMAINWINDOW_H
#define WLANSNIFFERMAINWINDOW_H

#include <QObject>

#include <HbMainWindow>

class HbAction;
class WlanSnifferListView;
class WlanSniffer;
class WlanSnifferService;

class WlanSnifferMainWindow : public HbMainWindow
    {
    Q_OBJECT

public:
    WlanSnifferMainWindow(WlanSniffer *appRef);
    virtual ~WlanSnifferMainWindow();

public slots:
    void toListView();
    void toDetailsView(int iapId);
    
    void updateListView();
    void updateListViewConnectionOpened(int iapId);
    void updateListViewConnectionClosed(int iapId);

signals:
    void exitTriggered();

private slots:
    void completeService();

private:
    // Methods to add views to the main window
    void addListView();
    
private:
    WlanSniffer *mAppRef;
    WlanSnifferListView *mListView;
    WlanSnifferService* mService;
    };

#endif
