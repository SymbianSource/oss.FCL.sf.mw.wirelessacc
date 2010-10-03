/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QtCore/QVariant>
#include <hbindicator.h>
#include <hbindicatorplugininterface.h>
#include <hbindicatorinterface.h>
#include "wlanindicatortest.h"
#include "wlanindicatortestmainwindow.h"



WlanIndicatorTest::WlanIndicatorTest(int argc, char* argv[])
    :  HbApplication(argc, argv),
    mMainWindow (new WlanIndicatorTestMainWindow(this))
{
	
    connect(
            mMainWindow,
            SIGNAL(exitTriggered()),
            this,
            SLOT(exitApplication()));
}


void WlanIndicatorTest::exitApplication()
{
exit();
}

WlanIndicatorTest::~WlanIndicatorTest()
{

}
