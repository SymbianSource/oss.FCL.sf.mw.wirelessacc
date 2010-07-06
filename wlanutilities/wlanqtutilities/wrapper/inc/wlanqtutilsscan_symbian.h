/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Symbian platform specific implementation of WLAN scanning.
*/

#ifndef WLANQTUTILSSCAN_SYMBIAN_H
#define WLANQTUTILSSCAN_SYMBIAN_H

// System includes

#include <wlanmgmtclient.h>
#include <QString>
#include <QSharedPointer>

// User includes

#include "wlanqtutilsap.h"

// Forward declarations

class CWlanMgmtClient;
class CWlanScanInfo;
class WlanQtUtilsScan;

// External data types

// Constants

// Class declaration

class WlanQtUtilsScanPrivate : public CActive
{

public:

    // Data types

    static WlanQtUtilsScanPrivate *NewL(WlanQtUtilsScan *q_ptr);
    
    ~WlanQtUtilsScanPrivate();

    void Scan();
    
    void Scan(const QString &ssid);
    
    void StopScan();
    
protected:

    void RunL();

    void DoCancel();

private:

    explicit WlanQtUtilsScanPrivate(WlanQtUtilsScan *q_ptr);

    void ConstructL();

    void ExtractScanResults(
        QList< QSharedPointer<WlanQtUtilsAp> > &scanResults);
    
    QString ExtractSsid();
    
    QByteArray ExtractBssid();
    
    void StoreSecMode(
        QSharedPointer<WlanQtUtilsAp> ap,
        TUint wlanSecMode);

private: // data
    
    //! Public implementation reference
    WlanQtUtilsScan *q_ptr;
    
    // Owned data

    //! WLAN Management Client
    CWlanMgmtClient *mWlanMgmtClient;

    //! Scanned SSID
    TWlanSsid mWlanSsid;
    
    //! Scan results
    CWlanScanInfo *mResults;

    // Not owned data
    
    // Friend classes
};

#endif // WLANQTUTILSSCAN_SYMBIAN_H
