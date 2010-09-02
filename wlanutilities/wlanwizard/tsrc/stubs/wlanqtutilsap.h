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
    
    // Data types
    
    /*!
        Configuration IDs that can be read and set in this class.

        ConfID's available for scan results:
        - ConfIdSsid
        - ConfIdBssid
        - ConfIdSignalStrength
        - ConfIdConnectionMode
        - ConfIdSecurityMode
        - ConfIdWpaPskUse
        - ConfIdWpsSupported

        ConfID's for IAP creation and updates, these are
        mandatory for all security modes: 
        - ConfIdSsid
        - ConfIdConnectionMode
        - ConfIdSecurityMode
        - ConfIdWepKey1     (Empty if not used) 
        - ConfIdWepKey2     (Empty if not used)
        - ConfIdWepKey3     (Empty if not used)
        - ConfIdWepKey4     (Empty if not used)
        - ConfIdWepDefaultIndex
        - ConfIdWpaPskUse
        - ConfIdWpaPsk      (Empty if not used)
        - ConfIdHidden
        - ConfIdWlanScanSSID
    */
    enum ConfId {
        //! QString: WLAN Network Name (SSID)
        ConfIdSsid = 0,
        //! QByteArray: Basic Service Set Identifier
        ConfIdBssid = 1,
        //! int: signal strength in RSSI (dBm)
        ConfIdSignalStrength = 2,
        //! int: (CMManagerShim::WlanNetMode)
        ConfIdConnectionMode = 3,
        //! int: (CMManagerShim::WlanSecMode)
        ConfIdSecurityMode = 4,
        //! bool: true - WPA PSK, false - WPA EAP
        ConfIdWpaPskUse = 5,
        //! QString: Pre-Shared Key for WPA
        //! Length: 8-63 - Ascii key
        //! Length: 64 - Hex key
        ConfIdWpaPsk = 6,
        //! QString: WEP Key for index 1:
        //! HEX: 
        //! - 64 bit: allowed key length = 10
        //! - 128 bit: allowed key length = 26
        //! 
        //! ASCII:
        //! - 64 bit: allowed key length = 5   
        //! - 128 bit: allowed key length = 13
        ConfIdWepKey1 = 7,
        //! QString: WEP Key for index 2
        ConfIdWepKey2 = 8,
        //! QString: WEP Key for index 3
        ConfIdWepKey3 = 9,
        //! QString: WEP Key for index 4
        ConfIdWepKey4 = 10,
        //! int: (CMManagerShim::WlanWepKeyIndex): Default WEP Key index
        ConfIdWepDefaultIndex = 11,
        //! bool: Is WLAN hidden (CMManagerShim::CmHidden).
        ConfIdHidden = 12,
        //! bool: Does WLAN AP need explicit scanning (CMManagerShim::WlanScanSSID)
        ConfIdWlanScanSSID = 13,
        //! bool: true - Wifi Protected setup is supported, false - not
        ConfIdWpsSupported = 14
    };
    
public:
    WlanQtUtilsAp();
    WlanQtUtilsAp(const WlanQtUtilsAp &ref);
    ~WlanQtUtilsAp();
    QVariant value(int identifier) const;
    void setValue(int identifier, QVariant value );
    bool operator==(const WlanQtUtilsAp & rhs ) const;
    
    static int compare(
        const WlanQtUtilsAp *ap1,
        const WlanQtUtilsAp *ap2);

private:
    WlanQtUtilsApPrivate *d_ptr;

};

Q_DECLARE_METATYPE(WlanQtUtilsAp)

#endif /* WLANQTUTILSWLANIAP_H */
