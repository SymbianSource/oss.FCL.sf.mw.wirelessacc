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
* This is a header file for WlanQtUtilsIap class.
*/

#ifndef WLANQTUTILSIAP_H
#define WLANQTUTILSIAP_H

#include <QObject>
#include "wlanqtutilscommon.h"

class QString;

class WLANQTUTILITIESDLL_EXPORT WlanQtUtilsIap : public QObject
{

public:

    WlanQtUtilsIap();
    WlanQtUtilsIap(
        int id,
        int netId,
        QString name,
        WlanQtUtilsBearerType bearerType);
    virtual ~WlanQtUtilsIap();

    int id() const;
    int networkId() const;
    QString name() const;
    WlanQtUtilsBearerType bearerType() const;
    WlanQtUtilsConnectionStatus connectionStatus() const;
    
    void setId(int id);
    void setNetworkId(int NetId);
    void setName(QString name);
    void setBearerType(WlanQtUtilsBearerType bearerType);
    void setConnectionStatus(WlanQtUtilsConnectionStatus connectionStatus);

private:
    int id_;
    int netId_;
    QString name_;
    WlanQtUtilsBearerType bearerType_;
    WlanQtUtilsConnectionStatus connectionStatus_;
    // TODO: SNAP ID list
};

#endif /* WLANQTUTILSIAP_H */
