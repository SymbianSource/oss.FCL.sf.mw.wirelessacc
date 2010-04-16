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
* This is the header file for WlanQtUtilsWlanAp class.
*/

#ifndef WLANQTUTILSWLANAP_H
#define WLANQTUTILSWLANAP_H

#include <QObject>
#include <QMetaType>
#include "wlanqtutilscommon.h"

class QString;

/**
 * This class represents WLAN access point (AP).
 */
class WLANQTUTILITIESDLL_EXPORT WlanQtUtilsWlanAp : public QObject
{
public:

    /**
     * Constructor.
     */
    WlanQtUtilsWlanAp();

    /**
     * Copy constructor.
     */
    WlanQtUtilsWlanAp(const WlanQtUtilsWlanAp &ref);

    /**
     * Constructor.
     * 
     * @param ssid SSID.
     * @param bssid BSSID, that is, MAC address.
     * @param signalStrength Signal strength.
     * @param connectionMode Connection mode as TConnMonNetworkMode.
     * @param securityMode WLAN Security mode.
     */
    WlanQtUtilsWlanAp(
        QString ssid,
        QString bssid,
        int signalStrength,
        int connectionMode,
        WlanQtUtilsWlanSecMode securityMode);

    /**
     * Destructor.
     */
    virtual ~WlanQtUtilsWlanAp();

    /**
     * Returns SSID.
     * 
     * @return SSID.
     */
    QString ssid() const;

    /**
     * Getter.
     * 
     * @return BSSID.
     */
    QString bssid() const;

    /**
     * Getter.
     * 
     * @return Signal strength.
     */
    int signalStrength() const;

    /**
     * Getter.
     * 
     * @return Connection mode as TConnMonNetworkMode.
     */
    int connectionMode() const;

    /**
     * Getter.
     * 
     * @return Security mode as WlanQtUtilsWlanSecMode.
     */
    WlanQtUtilsWlanSecMode securityMode() const;

    /**
     * Getter.
     * 
     * @return Security key.
     */
    QString securityKey() const;

    /**
     * Setter.
     * 
     * @param SSID.
     */
    void setSsid(QString ssid);

    /**
     * Setter.
     * 
     * @param BSSID.
     */
    void setBssid(QString bssid);

    /**
     * Setter.
     * 
     * @param Signal strength.
     */
    void setSignalStrength(int signalStrength);

    /**
     * Setter.
     * 
     * @param connectionMode Connection mode as TConnMonNetworkMode.
     */
    void setConnectionMode(int connectionMode);

    /**
     * Setter.
     * 
     * @param securityMode WLAN Security mode.
     */
    void setSecurityMode(WlanQtUtilsWlanSecMode securityMode);

    /**
     * Setter.
     * 
     * @param securityKey WLAN Security key.
     */
    void setSecurityKey(QString securityKey);

private: //data

    /** SSID */
    QString ssid_;
    /** BSSID, that is, MAC address. */
    QString bssid_; // TODO: Check if QString is suitable
    /** Signal strength. */
    int signalStrength_;
    /** Connection mode as TConnMonNetworkMode. */
    int connectionMode_;
    /** WLAN security mode. */
    WlanQtUtilsWlanSecMode securityMode_;
    /** Security key, if available. */
    QString securityKey_;
};

Q_DECLARE_METATYPE(WlanQtUtilsWlanAp)

#endif /* WLANQTUTILSWLANIAP_H */
