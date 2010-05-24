/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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

// INCLUDE FILES
#include "wlanqtutilsconntestwrapper.h"
#include "wlanqtutilsconntestwrapper_s60_p.h"

ConnTestWrapper::ConnTestWrapper(QObject *parent) :
    QObject(parent),
    d_ptr_(new ConnTestWrapperPrivate(this))
{
}

ConnTestWrapper::~ConnTestWrapper()
{
}

void ConnTestWrapper::connectivityTestDone(bool result)
{
    emit connectivityTestResult(result);
}

void ConnTestWrapper::startConnectivityTest(int iapId, int netId)
{
    d_ptr_->startConnectivityTest(iapId, netId);
}

//end of file
