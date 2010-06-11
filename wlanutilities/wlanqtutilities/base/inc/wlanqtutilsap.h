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
* WLAN AP (Access Point, unknown network) class.
*/

#ifndef WLANQTUTILSAP_H
#define WLANQTUTILSAP_H

// System includes

#include <QObject>
#include <QMetaType>
#include <QVariant>
#include <QScopedPointer>

// User includes

// Forward declarations

class WlanQtUtilsApPrivate;

// External data types

// Constants

//! Library interface export macro
#ifdef BUILD_WLANQTUTILITIES_DLL
#define WLANQTUTILSAP_EXPORT Q_DECL_EXPORT
#else
#define WLANQTUTILSAP_EXPORT Q_DECL_IMPORT
#endif

// Class declaration

class WLANQTUTILSAP_EXPORT WlanQtUtilsAp
{
    
public:
    
    // Data types
    
    /*!
        Configuration IDs that can be read and set in this class.
        Remember to update traces/trace.properties file when modifying
        this enum (tracing is also the cause for fixing the enum values).

        Scan Results:
        - ConfIdSsid
        - ConfIdSignalStrength
        - ConfIdConnectionMode
        - ConfIdSecurityMode
        - ConfIdWpaPskUse
        - ConfIdWpsSupported

        IAP creation and updates:
        Mandatory for all security modes:
        - ConfIdSsid
        - ConfIdConnectionMode
        - ConfIdSecurityMode
        - ConfIdHidden

        Open: No extra configurations. 
        802.1x: No extra configurations.

        WEP:
        - ConfIdWepKey1
        - ConfIdWepKey2
        - ConfIdWepKey3
        - ConfIdWepKey4
        - ConfIdWepDefaultIndex

        WPA/WPA2:
        - ConfIdWpaPskUse
        - ConfIdWpaPsk
    */
    enum ConfId {
        //! QString: WLAN Network Name (SSID)
        ConfIdSsid = 0,
        //! int: signal strength in RSSI (dBm)
        ConfIdSignalStrength = 1,
        //! int: (CMManagerShim::WlanNetMode)
        ConfIdConnectionMode = 2,
        //! int: (CMManagerShim::WlanSecMode)
        ConfIdSecurityMode = 3,
        //! bool: true - WPA PSK, false - WPA EAP
        ConfIdWpaPskUse = 4,
        //! QString: Pre-Shared Key for WPA
        //! Length: 8-63 - Ascii key
        //! Length: 64 - Hex key
        ConfIdWpaPsk = 5,
        //! QString: WEP Key for index 1:
        //! HEX: 
        //! - 64 bit: allowed key length = 10
        //! - 128 bit: allowed key length = 26
        //! 
        //! ASCII:
        //! - 64 bit: allowed key length = 5   
        //! - 128 bit: allowed key length = 13
        ConfIdWepKey1 = 6,
        //! QString: WEP Key for index 2
        ConfIdWepKey2 = 7,
        //! QString: WEP Key for index 3
        ConfIdWepKey3 = 8,
        //! QString: WEP Key for index 4
        ConfIdWepKey4 = 9,
        //! int: (CMManagerShim::WlanWepKeyIndex): Default WEP Key index
        ConfIdWepDefaultIndex = 10,
        //! bool: Is WLAN Hidden: true - hidden, false - not hidden.
        ConfIdHidden = 11,
        //! bool: true - Wifi Protected setup is supported, false - not
        ConfIdWpsSupported = 12
    };
    
    WlanQtUtilsAp();

    WlanQtUtilsAp(const WlanQtUtilsAp &ref);
    
    virtual ~WlanQtUtilsAp();
    
    QVariant value(int identifier) const;
    
    void setValue(int identifier, QVariant value);
    
    static bool compare(
        const WlanQtUtilsAp *ap1,
        const WlanQtUtilsAp *ap2);
    
signals:

public slots:

protected:

protected slots:

private:

    // Disabling implicit assignment operator
    WlanQtUtilsAp &operator=(const WlanQtUtilsAp &);
    
private slots:

private: // data

    QScopedPointer<WlanQtUtilsApPrivate> d_ptr; //!< Private implementation

    // Friend classes
};

// Metatype support is needed since we use this class with QVariant
Q_DECLARE_METATYPE(WlanQtUtilsAp)

#endif // WLANQTUTILSAP_H
