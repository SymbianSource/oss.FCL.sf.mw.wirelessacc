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
* This is a source file for EsockWrapper class.
*/

#include "wlanqtutilsesockwrapper.h"
#include "wlanqtutilsesockwrapper_s60_p.h"

EsockWrapper::EsockWrapper(QObject *parent)
 : QObject(parent)
{
    d_ptr = new EsockWrapperPrivate(this);
}

EsockWrapper::~EsockWrapper()
{
    delete d_ptr;
}

void EsockWrapper::updateConnection(bool isOpened)
{
	emit connectionStatusFromWrapper(isOpened);
}

void EsockWrapper::connectIap(int iapId)
{
    d_ptr->connectIap(iapId);
}

void EsockWrapper::disconnectIap()
{
    d_ptr->disconnectIap();
}
