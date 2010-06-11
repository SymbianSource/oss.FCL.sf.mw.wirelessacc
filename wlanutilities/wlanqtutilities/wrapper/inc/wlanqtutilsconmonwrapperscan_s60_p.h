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
* Private implementation of wrapper for Symbian Connection Monitor
* library's scan interface.
*/

#ifndef WLANQTUTILSCONMONWRAPPERSCAN_S60_P_H
#define WLANQTUTILSCONMONWRAPPERSCAN_S60_P_H

// System includes

#include <e32base.h>
#include <rconnmon.h>

// User includes

// Forward declarations

class WlanQtUtilsAp;
class WlanQtUtilsConMonWrapper;

// External data types

// Constants

// Class declaration

class WlanQtUtilsConMonWrapperScan : public CActive
{
public:

    // Data types

    WlanQtUtilsConMonWrapperScan(WlanQtUtilsConMonWrapper *wrapper);

    ~WlanQtUtilsConMonWrapperScan();

    void ScanAvailableWlanAPs();

    void StopScan();

protected:

private:

    void DoCancel();

    void RunL();

    void StoreConMonSecMode(
        QSharedPointer<WlanQtUtilsAp> ap,
        TUint conMonSecMode);
    
    void StoreConMonConnMode(
        QSharedPointer<WlanQtUtilsAp> ap,
        TInt conMonConnMode);

private: // data
    
    // Owned data

    RConnectionMonitor iMonitor; //!< Handle to Connection Monitor Server

    //! Scan package used with Connection Monitor Server scan request
    CConnMonWlanNetworksPtrArrayPckg* iWlanBuf;

    //! wlan pointer used with Connection Monitor Server scan request
    TPtr iWlanPtr;

    /*!
       Has scanning been cancelled by client, are we allowed to report
       results?
     */
    TBool iScanCancelled;
    
    // Not owned data

    //! Public implementation to report progress to
    WlanQtUtilsConMonWrapper *q_ptr;
    
    // Friend classes
};

#endif // WLANQTUTILSCONMONWRAPPERSCAN_S60_P_H
