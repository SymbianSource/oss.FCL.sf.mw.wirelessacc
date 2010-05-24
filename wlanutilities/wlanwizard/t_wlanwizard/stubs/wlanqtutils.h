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

#ifndef STUB_WLANQTUTILS_H
#define STUB_WLANQTUTILS_H

#include <QObject>
#include <QSharedPointer>
#include <QStringList>
#include <QTest>

#include "wlanqtutilscommon.h"
#include "wlanqtutilsap.h"

class WlanQtUtilsIap;
class WlanQtUtilsWlanIap;

class WLANQTUTILITIESDLL_EXPORT WlanQtUtils : public QObject
{
    Q_OBJECT
    friend class WlanQtUtilsContext;
public:
    //! "None" IAP ID value (e.g. for "not found" cases)
    static const int IapIdNone = -1;
    
public:
    WlanQtUtils();
    
    ~WlanQtUtils();
   
    void availableWlanAps( QList<QSharedPointer<WlanQtUtilsAp> > &wlanApList);   
    
    int createIap(const WlanQtUtilsAp *wlanAp);

    bool updateIap(int iapId, const WlanQtUtilsAp *wlanAp);

    void deleteIap(int iapId);

    void connectIap(int iapId, bool runIct = false);
    
    int connectedWlanId();

    void disconnectIap(int iapId);
    
    int activeIap();
    
    void scanWlanAps();
   
    void scanWlanDirect(const QString &ssid);

    void stopWlanScan();

signals:
    
    void wlanScanApReady();
   
    void wlanScanDirectReady();
    
    void wlanNetworkOpened(int iapId);

    void wlanNetworkClosed(int iapId, int reason);

#ifdef ICT_RESULT_ENUM
    void ictResult(int iapId, WlanLoginIctsResultType result);
#else
    void ictResult(int iapId, bool result);
#endif

private: // Return values for all methods.
    QList<QSharedPointer<WlanQtUtilsAp> > mScanWlanAps;
    QList<QSharedPointer<WlanQtUtilsAp> > mScanWlanDirect;
    QList<QSharedPointer<WlanQtUtilsAp> > *mScanResult;
    QString mScanWlanDirectSsid;
    int mCreateWlanIapReturn;
    bool mUpdateWlanIapReturn;
    
    int mWlanNetworkOpenedIapId;
    int mWlanNetworkActiveIapId;
    int mWlanNetworkClosedIapId;
    bool mWlanNetworkClosedReason;
    
    int mIctResultIapId;
#ifdef ICT_RESULT_ENUM
    WlanLoginIctsResultType mIctResultResults;
#else
    bool mIctResultResults;
#endif
    bool mConnectionSuccess;
    
    QStringList mCalledMethods;
    
    WlanQtUtilsAp *mWlanAp;
};

#endif /* WLANQTUTILS_H */

// End of File
