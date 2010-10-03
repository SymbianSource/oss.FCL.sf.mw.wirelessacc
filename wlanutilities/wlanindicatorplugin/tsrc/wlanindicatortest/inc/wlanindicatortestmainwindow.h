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

#ifndef WLANINDICATORMAINWINDOW_H
#define WLANINDICATORMAINWINDOW_H

#include <qobject>
#include <hbmainwindow.h>

class HbAction;
class WlanIndicatorTestView;
class WlanIndicatorTest;

class WlanIndicatorTestMainWindow : public HbMainWindow
    {
    Q_OBJECT

public:
    WlanIndicatorTestMainWindow(WlanIndicatorTest *appRef);
    virtual ~WlanIndicatorTestMainWindow();
    
private:
    void addTestView();

signals:
    void exitTriggered();
  
private:
    WlanIndicatorTest *mAppRef;
    HbAction *mExitAction;
    WlanIndicatorTestView *mTestView;
    };

#endif
