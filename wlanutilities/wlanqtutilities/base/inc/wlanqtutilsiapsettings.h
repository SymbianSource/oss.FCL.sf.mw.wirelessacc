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
* WLAN Qt Utilities WLAN IAP settings handling.
*/

#ifndef WLANQTUTILSIAPSETTINGS_H
#define WLANQTUTILSIAPSETTINGS_H

// System includes

#include <QObject>
#include <QList>
#include <QSharedPointer>

// User includes

// Forward declarations

class CmManagerShim;
class CmConnectionMethodShim;
class WlanQtUtilsIap;
class WlanQtUtilsAp;

// External data types

// Constants

// Class declaration

class WlanQtUtilsIapSettings : public QObject
{
    Q_OBJECT
    
public:

    // Data types

    explicit WlanQtUtilsIapSettings(QObject *parent = 0);
    
    virtual ~WlanQtUtilsIapSettings();

    void fetchIaps(QList< QSharedPointer<WlanQtUtilsIap> > &iapList) const;

    QSharedPointer<WlanQtUtilsIap> fetchIap(int iapId) const;
    
    int createIap(const WlanQtUtilsAp *wlanAp);
    
    bool updateIap(int iapId, const WlanQtUtilsAp *wlanAp);
    
    void deleteIap(int iapId);

    void moveIapToInternetSnap(int iapId);
    
signals:

public slots:

protected:

protected slots:

private:

    Q_DISABLE_COPY(WlanQtUtilsIapSettings)

    void storeSettings(
        const WlanQtUtilsAp *wlanAp,
        CmConnectionMethodShim *iap);
    
    void storeWepKey(
        const QString &key,
        int index,
        CmConnectionMethodShim *iap);

private slots:

private: // data

    CmManagerShim *mCmManager;      //!< CmManager instance
    
    // Friend classes
};

#endif // WLANQTUTILSIAPSETTINGS_H
