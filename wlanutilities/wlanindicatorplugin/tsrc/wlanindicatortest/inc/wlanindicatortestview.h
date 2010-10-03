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

#ifndef WLANINDICATORTESTVIEW_H
#define WLANINDICATORTESTVIEW_H

#include <HbView.h>

class HbPushButton;
class WlanIndicatorTest;

class WlanIndicatorTestView : public HbView
    {
    Q_OBJECT
    
public:
    WlanIndicatorTestView(WlanIndicatorTest *appRef);
    virtual ~WlanIndicatorTestView();
    
 
signals:
    void exitTriggered();

private slots:
    void handleWlanPushButtonPressed();
    void handleGprsPushButtonPressed();
   
private:
    HbPushButton *mTestButton2;
    HbPushButton *mTestButton3;
    HbPushButton *mTestButton4;
    HbPushButton *mTestButton5;
    HbPushButton *mTestButton6;
    HbPushButton *mTestButton7;
    HbPushButton *mTestButton8;
    WlanIndicatorTest *mAppRef;
    QString mWlanIap;
    };

#endif
