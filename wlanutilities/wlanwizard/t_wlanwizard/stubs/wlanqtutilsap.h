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

#ifndef STUB_WLANQTUTILSWLANAP_H
#define STUB_WLANQTUTILSWLANAP_H

#include <QObject>
#include <QMetaType>
#include <QVariant>
#include "cmmanagerdefines_shim.h"

class QString;
class WlanQtUtilsApPrivate;

/**
 * This class represents WLAN access point (AP).
 */
class WlanQtUtilsAp : public QObject
{
public:
    /*!
     * Scan Results:
     * - ConfIdSsid
     * - ConfIdBssid
     * - ConfIdSignalStrength
     * - ConfIdConnectionMode
     * - ConfIdSecurityMode
     * - ConfIdWpaPskUse
     * - ConfIdWpsSupported
     * 
     * IAP creation and updates:
     * Mandatory for all security modes:
     * - ConfIdSsid
     * - ConfIdConnectionMode
     * - ConfIdSecurityMode
     * - ConfIdHidden
     * 
     * Open: No extra configurations. 
     * 802.1x: No extra configurations.
     * 
     * WEP:
     * - ConfIdWepKey1
     * - ConfIdWepKey2
     * - ConfIdWepKey3
     * - ConfIdWepKey4
     * - ConfIdWepDefaultIndex
     * 
     * WPA/WPA2:
     * - ConfIdWpaPskUse
     * - ConfIdWpaPsk
     * 
     */
    enum ConfId{
        /// int: IAP ID, -1 for invalid IAP (WlanQtUtilsInvalidIapId)
        ConfIdIapId,
        /// QString: WLAN Network Name (SSID)
        ConfIdSsid,
        /// QString: Basic Service Set Identifier
        ConfIdBssid,
        /// int: signal strength in RSSI (dBm)
        ConfIdSignalStrength,
        /// int: (CMManagerShim::WlanNetMode)
        ConfIdConnectionMode,
        /// int: (CMManagerShim::WlanSecMode)
        ConfIdSecurityMode,
        /// bool: true - WPA PSK, false - WPA EAP
        ConfIdWpaPskUse,
        /// QString: Pre-Shared Key for WPA
        /// Length: 8-63 - Ascii key
        /// Length: 64 - Hex key
        ConfIdWpaPsk,
        /// QString: WEP Key for index 1:
        /// HEX: 
        /// - 64 bit: allowed key length = 10
        /// - 128 bit: allowed key length = 26
        /// 
        /// ASCII:
        /// - 64 bit: allowed key length = 5   
        /// - 128 bit: allowed key length = 13
        ConfIdWepKey1,
        /// QString: WEP Key for index 2
        ConfIdWepKey2,
        /// QString: WEP Key for index 3
        ConfIdWepKey3,
        /// QString: WEP Key for index 4
        ConfIdWepKey4,
        /// int: (CMManagerShim::WlanWepKeyIndex): Default WEP Key index
        ConfIdWepDefaultIndex,
        /// bool: Is WLAN Hidden: true - hidden, false - not hidden.
        ConfIdHidden,
        /// bool: true - Wifi Protected setup is supported, false - not
        ConfIdWpsSupported,
    };
    
public:
    WlanQtUtilsAp();
    WlanQtUtilsAp(const WlanQtUtilsAp &ref);
    ~WlanQtUtilsAp();
    QVariant value(int identifier) const;
    void setValue(int identifier, QVariant value );
    bool operator==(const WlanQtUtilsAp & rhs ) const;
    
    static bool compare(
        const WlanQtUtilsAp *ap1,
        const WlanQtUtilsAp *ap2);

private:
    WlanQtUtilsApPrivate *d_ptr;

};

Q_DECLARE_METATYPE(WlanQtUtilsAp)

#endif /* WLANQTUTILSWLANIAP_H */
