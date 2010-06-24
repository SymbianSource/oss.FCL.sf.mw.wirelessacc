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
* WLAN scan platform specific implementation wrapper.
*/

// System includes

// User includes

#include "wlanqtutilsscan_symbian.h"
#include "wlanqtutilsscan.h"

/*!
    \class WlanQtUtilsScan
    \brief WLAN scan platform specific implementation wrapper.

    This class implements the WLAN scanning functionality platform independently.
    The private implementation class implements the platform specific parts. 
*/

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
    
    @param [in] parent Parent object.
*/

WlanQtUtilsScan::WlanQtUtilsScan(QObject *parent) :
    QObject(parent)
{
    QT_TRAP_THROWING(d_ptr = WlanQtUtilsScanPrivate::NewL(this));
}

/*!
    Destructor.
*/

WlanQtUtilsScan::~WlanQtUtilsScan()
{
    delete d_ptr;
}

/*!
    Scans available WLAN AP's (i.e. broadcast scan).
*/

void WlanQtUtilsScan::scanWlanAps()
{
    d_ptr->Scan();
}

/*!
    Scans with given WLAN SSID (i.e. direct scan).
    
    @param [in] ssid WLAN SSID to scan.
*/

void WlanQtUtilsScan::scanWlanDirect(const QString &ssid)
{
    d_ptr->Scan(ssid);
}

/*!
    Stops current scan. Can also be called if there is no scan in progres.
*/

void WlanQtUtilsScan::stopScan()
{
    d_ptr->StopScan();
}
