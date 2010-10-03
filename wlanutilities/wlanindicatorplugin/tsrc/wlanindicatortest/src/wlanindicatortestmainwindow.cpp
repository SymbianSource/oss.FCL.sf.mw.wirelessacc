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

#include <hbaction.h>
#include <qgraphicswidget>
#include <hblabel.h>
#include <hbmenu.h>
#include <hbpushbutton.h>
#include <hbdocumentloader.h>
#include <hbinstance.h>

#include <hbindicator.h>
#include <hbindicatorplugininterface.h>
#include <hbindicatorinterface.h>

#include "wlanindicatortest.h"
#include "wlanindicatortestview.h"
#include "wlanindicatortestmainwindow.h"


WlanIndicatorTestMainWindow::WlanIndicatorTestMainWindow(WlanIndicatorTest *appRef) :
    mAppRef(appRef),
    mExitAction(0)
{
    addTestView();

    mExitAction = softKeyAction(Hb::SecondarySoftKey);

    show();
}

WlanIndicatorTestMainWindow::~WlanIndicatorTestMainWindow()
{
}

void WlanIndicatorTestMainWindow::addTestView()
{
    mTestView = new WlanIndicatorTestView(mAppRef);
    addView(mTestView);
    
    bool connectStatus = connect(
         mTestView,
         SIGNAL(exitTriggered()),
         this,
         SIGNAL(exitTriggered()));
     Q_ASSERT(connectStatus == true);
     
}

