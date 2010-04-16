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
* This is a source file for WlanQtUtilsActiveConn class.
*/

#include "wlanqtutilsactiveconn.h"

WlanQtUtilsActiveConn::WlanQtUtilsActiveConn() :
    connectionId_(0),
    iapId_(0),
    connectionStatus_(WlanQtUtilsConnectionStatusNone),
    bearerType_(WlanQtUtilsBearerTypeNone)
{
}

WlanQtUtilsActiveConn::~WlanQtUtilsActiveConn()
{
}

uint WlanQtUtilsActiveConn::connectionId() const
{
    return connectionId_;
}

void WlanQtUtilsActiveConn::setConnectionId(uint connectionId)
{
    connectionId_ = connectionId;
}

uint WlanQtUtilsActiveConn::iapId() const
{
    return iapId_;
}

void WlanQtUtilsActiveConn::setIapId(uint iapId)
{
    iapId_ = iapId;
}

WlanQtUtilsConnectionStatus WlanQtUtilsActiveConn::connectionStatus() const
{
    return connectionStatus_;
}

void WlanQtUtilsActiveConn::setConnectionStatus(WlanQtUtilsConnectionStatus connectionStatus)
{
    connectionStatus_ = connectionStatus;
}

WlanQtUtilsBearerType WlanQtUtilsActiveConn::bearerType() const
{
    return bearerType_;
}

void WlanQtUtilsActiveConn::setBearerType(WlanQtUtilsBearerType bearerType)
{
    bearerType_ = bearerType;
}
