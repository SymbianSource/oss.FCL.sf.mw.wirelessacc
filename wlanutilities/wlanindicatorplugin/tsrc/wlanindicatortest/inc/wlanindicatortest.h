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

#ifndef WLANINDICATORTEST_H
#define WLANINDICATORTEST_H

#include <HbApplication.h>

class WlanIndicatorTestMainWindow;


class WlanIndicatorTest : public HbApplication
{
    Q_OBJECT

public:
    WlanIndicatorTest(int argc, char* argv[]);
    virtual ~WlanIndicatorTest();

private slots:
    void exitApplication();
    
private:
    WlanIndicatorTestMainWindow *mMainWindow;
};

#endif // WLANINDICATORTEST_H

