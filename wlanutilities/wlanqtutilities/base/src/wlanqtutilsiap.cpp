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

#include <QString>
#include "wlanqtutilsiap.h"

WlanQtUtilsIap::WlanQtUtilsIap() : 
    id_(0),
    netId_(0),
    name_(""),
    bearerType_(WlanQtUtilsBearerTypeNone),
    connectionStatus_(WlanQtUtilsConnectionStatusNone)
{

}

WlanQtUtilsIap::WlanQtUtilsIap(
    int id,
    int netId,
    QString name,
    WlanQtUtilsBearerType bearerType) :
    id_(id), netId_(netId), name_(name), bearerType_(bearerType), connectionStatus_(WlanQtUtilsConnectionStatusNone)
{
}

WlanQtUtilsIap::~WlanQtUtilsIap()
{
}

int WlanQtUtilsIap::id() const
{
    return id_;
}

int WlanQtUtilsIap::networkId() const
{
    return netId_;
}

QString WlanQtUtilsIap::name() const
{
    return name_;
}

WlanQtUtilsBearerType WlanQtUtilsIap::bearerType() const
{
    return bearerType_;
}

WlanQtUtilsConnectionStatus WlanQtUtilsIap::connectionStatus() const
{
    return connectionStatus_;
}

void WlanQtUtilsIap::setId(int id)
{
    id_ = id;
}

void WlanQtUtilsIap::setNetworkId(int netId)
{
    netId_ = netId;
}

void WlanQtUtilsIap::setName(QString name)
{
    name_ = name;
}

void WlanQtUtilsIap::setBearerType(WlanQtUtilsBearerType bearerType)
{
    bearerType_ = bearerType;
}

void WlanQtUtilsIap::setConnectionStatus(WlanQtUtilsConnectionStatus connectionStatus)
{
    connectionStatus_ = connectionStatus;
}
