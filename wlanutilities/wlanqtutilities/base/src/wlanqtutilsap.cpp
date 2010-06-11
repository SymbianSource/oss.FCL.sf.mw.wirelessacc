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

// System includes

#include <QString>
#include <QHash>
#include <QVariant>
#include <QDebug>

// User includes

#include "wlanqtutilsap.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutilsapTraces.h"
#endif

/*!
    \class WlanQtUtilsApPrivate
    \brief Private implementation of WlanQtUtilsAp.
*/

class WlanQtUtilsApPrivate
{
private:
    //! Access Point configuration data
    QHash<int, QVariant> mConfigurations;

    friend class WlanQtUtilsAp;
};

/*!
    \class WlanQtUtilsAp
    \brief WLAN Access Point class.

    Contains the information related to unknown WLAN access points.
*/

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
*/

WlanQtUtilsAp::WlanQtUtilsAp() :
    d_ptr(new WlanQtUtilsApPrivate())
{
}

/*!
    Copy constructor.

    @param [in] ref AP to create a copy of.
*/

WlanQtUtilsAp::WlanQtUtilsAp(const WlanQtUtilsAp &ref) :
    d_ptr(new WlanQtUtilsApPrivate())
{
    d_ptr->mConfigurations = ref.d_ptr->mConfigurations;
}

/*!
    Destructor.
*/

WlanQtUtilsAp::~WlanQtUtilsAp()
{
}

/*!
    Getter for AP data.

    @param [in] identifier Identifier of value to get.
    
    @return Value.
*/

QVariant WlanQtUtilsAp::value(int identifier) const
{
    Q_ASSERT(d_ptr->mConfigurations.contains(identifier));
    Q_ASSERT(d_ptr->mConfigurations[identifier].isValid());
    
#ifdef OST_TRACE_COMPILER_IN_USE
    QString tmp;
    QDebug tmpStream(&tmp);
    tmpStream << d_ptr->mConfigurations[identifier];
    TPtrC16 string(tmp.utf16(), tmp.length());
    
    OstTraceExt2(
        TRACE_DUMP,
        WLANQTUTILSAP_VALUE,
        "WlanQtUtilsAp::value;identifier=%{ConfId};string=%S",
        (TUint)identifier,
        string);
#endif
    
    return d_ptr->mConfigurations[identifier];
}

/*!
    Setter for AP data.

    @param [in] identifier Identifier of value to set.
    @param [in] value Value to set.
*/

void WlanQtUtilsAp::setValue(int identifier, QVariant value)
{
#ifdef OST_TRACE_COMPILER_IN_USE
    QString tmp;
    QDebug tmpStream(&tmp);
    tmpStream << value;
    TPtrC16 string(tmp.utf16(), tmp.length());
    
    OstTraceExt2(
        TRACE_DUMP,
        WLANQTUTILSAP_SETVALUE,
        "WlanQtUtilsAp::setValue;identifier=%{ConfId};string=%S",
        (TUint)identifier,
        string);
#endif
    
    d_ptr->mConfigurations[identifier] = value;
}

/*!
    AP comparison function.

    @param [in] ap1 First AP to compare.
    @param [in] ap2 Second AP to compare.

    @return TRUE, if APs are considered to be same.
*/

bool WlanQtUtilsAp::compare(
    const WlanQtUtilsAp *ap1,
    const WlanQtUtilsAp *ap2)
{
    bool equal = false;
    
    // SSID (case sensitive) and security mode (with or without PSK)
    // are the values, which identify a unique access point.
    if (ap1->value(WlanQtUtilsAp::ConfIdSsid) ==
        ap2->value(WlanQtUtilsAp::ConfIdSsid)
        && ap1->value(WlanQtUtilsAp::ConfIdSecurityMode) == 
           ap2->value(WlanQtUtilsAp::ConfIdSecurityMode)
           && ap1->value(WlanQtUtilsAp::ConfIdWpaPskUse) ==
              ap2->value(WlanQtUtilsAp::ConfIdWpaPskUse)) {
        equal = true;
    }
    
    return equal;
}
