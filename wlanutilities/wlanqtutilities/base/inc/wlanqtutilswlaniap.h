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
* This is a header file for WlanQtUtilsWlanIap class.
*/

#ifndef WLANQTUTILSWLANIAP_H
#define WLANQTUTILSWLANIAP_H

#include "wlanqtutilscommon.h"
#include "wlanqtutilsiap.h"

class WLANQTUTILITIESDLL_EXPORT WlanQtUtilsWlanIap : public WlanQtUtilsIap
{
    // Q_OBJECT macro needed for qobject_cast to work
    Q_OBJECT
    
public:
    WlanQtUtilsWlanIap();
    WlanQtUtilsWlanIap(int id, int netId, QString name, WlanQtUtilsBearerType bearerType, QString ssid, WlanQtUtilsWlanSecMode securityMode);
    virtual ~WlanQtUtilsWlanIap();

    QString ssid() const;
    int signalStrength() const;
    WlanQtUtilsWlanSecMode securityMode() const;
        
    void setSsid(QString ssid);
    void setSignalStrength(int signalStrength);
    void setSecurityMode(WlanQtUtilsWlanSecMode securityMode);

private:
    QString ssid_;
    int signalStrength_;
    WlanQtUtilsWlanSecMode securityMode_;
    // TODO: secKey_  (type unknown)
};

#endif /* WLANQTUTILSWLANIAP_H */
