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
* WLAN IAP (Internet Access Point, known network) class.
*/

#ifndef WLANQTUTILSIAP_H
#define WLANQTUTILSIAP_H

// System includes

#include <QScopedPointer>

// User includes

#include "wlanqtutilsap.h"

// Forward declarations

class WlanQtUtilsIapPrivate;

// External data types

// Constants

//! Library interface export macro
#ifdef BUILD_WLANQTUTILITIES_DLL
#define WLANQTUTILSIAP_EXPORT Q_DECL_EXPORT
#else
#define WLANQTUTILSIAP_EXPORT Q_DECL_IMPORT
#endif

// Class declaration

class WLANQTUTILSIAP_EXPORT WlanQtUtilsIap : public WlanQtUtilsAp
{
    
public:

    // Data types
    
    /*!
        Configuration IDs that can be read and set in this class (via
        functions declared in the parent class). Values under 100 are
        reserved for the parent class.
        
        Remember to update traces/trace.properties file when modifying
        this enum (tracing is also the cause for fixing the enum values).
        
        Values available for IAPs:
        - ConfIdIapId
        - ConfIdName
        - ConfIdNetworkId
        - WlanQtUtilsAp::ConfIdSsid
        - WlanQtUtilsAp::ConfIdSignalStrength
        - WlanQtUtilsAp::ConfIdConnectionMode
        - WlanQtUtilsAp::ConfIdSecurityMode
        - WlanQtUtilsAp::ConfIdWpaPskUse
     */
    enum ConfId {
        //! int: IAP ID
        ConfIdIapId = 100,
        //! QString: Name
        ConfIdName = 101,
        //! int: Network ID
        ConfIdNetworkId = 102
    };
    
    WlanQtUtilsIap();

    virtual ~WlanQtUtilsIap();

signals:

public slots:

protected:

protected slots:

private:

    Q_DISABLE_COPY(WlanQtUtilsIap)

private slots:

private: // data

    QScopedPointer<WlanQtUtilsIapPrivate> d_ptr; //!< Private implementation

    // Friend classes
};

#endif // WLANQTUTILSIAP_H
