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

// System includes

// User includes

#include "wlanqtutilsiap.h"

/*!
    \class WlanQtUtilsIapPrivate
    \brief Private implementation of WlanQtUtilsIap.
    
    This class is reserved for WlanQtUtilsIap private implementation.

    NOTE: Even though there is currently no implementation, this is
    required for possible later additions to avoid binary compatibility
    break.
*/

class WlanQtUtilsIapPrivate
{
    friend class WlanQtUtilsIap;
};

/*!
    \class WlanQtUtilsIap
    \brief WLAN Internet Access Point class.

    Contains the information related to known WLAN access points.
*/

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
*/

WlanQtUtilsIap::WlanQtUtilsIap() :
    WlanQtUtilsAp(),
    d_ptr(new WlanQtUtilsIapPrivate())
{
}

/*!
    Destructor.
*/

WlanQtUtilsIap::~WlanQtUtilsIap()
{
}
