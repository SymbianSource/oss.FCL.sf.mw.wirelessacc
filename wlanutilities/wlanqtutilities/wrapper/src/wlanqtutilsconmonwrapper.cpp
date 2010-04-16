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

// INCLUDE FILES
#include <cmdestinationext.h>
#include <cmmanagerext.h>
#include "wlanqtutilsconmonwrapper.h"
#include "wlanqtutilsconmonwrapper_s60_p.h"

// ================= MEMBER FUNCTIONS =======================

ConMonWrapper::ConMonWrapper(QObject *parent)
 : QObject(parent)
{
    d_ptrScanWlans = new ConnMonScanWlanAps(this);
    d_ptrConnInfo = new ConnMonConnInfo(this);
    d_ptrConnDisconnect = new ConnMonConnDisconnect(this);
}

ConMonWrapper::~ConMonWrapper()
{
    delete d_ptrScanWlans;
    delete d_ptrConnInfo;
    delete d_ptrConnDisconnect;
}

int ConMonWrapper::scanAvailableWlanAPs()
{
    return d_ptrScanWlans->scanAvailableWlanAPs();
}

void ConMonWrapper::emitAvailableWlans(QList<WlanQtUtilsWlanAp *> &availableWlanAPs)
{
    emit availableWlanApsFromWrapper(availableWlanAPs);
}

void ConMonWrapper::emitConnCreatedEvent(uint connectionId)
{
   emit connCreatedEventFromWrapper(connectionId);
}

void ConMonWrapper::emitConnDeletedEvent(uint connectionId)
{
   emit connDeletedEventFromWrapper(connectionId);
}

void ConMonWrapper::emitConnStatusEvent(uint connectionId, WlanQtUtilsConnectionStatus connectionStatus)
{
   emit connStatusEventFromWrapper(connectionId, connectionStatus);
}

WlanQtUtilsActiveConn *ConMonWrapper::activeConnection()
{
    return d_ptrConnInfo->activeConnection();
}

WlanQtUtilsActiveConn* ConMonWrapper::connectionInfo(uint connectionId)
{
    return d_ptrConnInfo->connectionInfo(connectionId);
}

void ConMonWrapper::disconnectIap(int iapId)
{
   d_ptrConnDisconnect->disconnectConnection(iapId);
}
