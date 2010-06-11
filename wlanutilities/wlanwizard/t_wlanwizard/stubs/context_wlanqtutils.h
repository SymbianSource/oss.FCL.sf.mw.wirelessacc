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
*/

#ifndef CONTROL_WLANQTUTILS_H
#define CONTROL_WLANQTUTILS_H

#include <QObject>
#include <QSharedPointer>

#include "wlanqtutilscommon.h"

class WlanQtUtils;
class WlanQtUtilsWlanIap;
class WlanQtUtilsAp;

class Q_DECL_EXPORT WlanQtUtilsContext: public QObject
{
    Q_OBJECT

public:
    WlanQtUtilsContext(WlanQtUtils *utils );
    ~WlanQtUtilsContext();
  
public:
    void setScanWlanApsResult( QList<QSharedPointer<WlanQtUtilsAp> > result );
    
    void setScanWlanDirectResult( QString ssid, 
        QList<QSharedPointer<WlanQtUtilsAp> > result );
    
    void setCreateWlanIapResult(int iapId);
    
    void setUpdateWlanIapResult(bool returnValue);
    
    void setConnectionSuccessed(bool value);
    
    void setSignalWlanNetworkOpened(int iapId);
    
    void setSignalWlanNetworkClosed(int iapId, int reason);
    
#ifdef ICT_RESULT_ENUM
    void setSignalIctResult(int iapId, WlanLoginIctsResultType result);
#else
    void setSignalIctResult(int iapId, bool result);
#endif
    
    QStringList calledMethods();
    
    bool verifyWlanIapSettings(WlanQtUtilsAp &ref);
    
private:
    WlanQtUtils* mUtils;
};

#endif /* CONTROL_WLANQTUTILS_H */

// End of File
