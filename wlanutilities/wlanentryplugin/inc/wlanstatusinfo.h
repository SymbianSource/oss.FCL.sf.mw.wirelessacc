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

#ifndef WLANSTATUSINFO_H
#define WLANSTATUSINFO_H

// System includes

#include <QObject>

// User includes

// Forward declarations

class WlanQtUtils;

// External data types

// Constants

// Class declaration

class WlanStatusInfo : public QObject
{
    Q_OBJECT
    
public:

    // Data types

    /**
     * WLAN Status value.
     */
    enum {
        /** WLAN is configured OFF. */
        WlanStatusOff = 0,
        /** No WLAN connections. */
        WlanStatusIdle,
        /** WLAN IAP is connected. */
        WlanStatusConnected,
    };

    explicit WlanStatusInfo(QObject *parent = 0);
    
    ~WlanStatusInfo();

    int status();

    QString statusText();

signals:

    /**
     * Signal that informs that the WLAN status has changed.
     */
    void statusUpdated();

public slots:

protected:

protected slots:

private:

    Q_DISABLE_COPY(WlanStatusInfo)

private slots:

    void updateStatus();

private: // data

    WlanQtUtils *mWlanQtUtils;      // WlanQtUtils instance.
    QString mStatusText;            // WLAN status text.
    int mStatus;                    // WLAN status value WlanStatus*.

    // Friend classes

};

#endif // WLANSTATUSINFO_H
