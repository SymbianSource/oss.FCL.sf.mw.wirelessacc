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

// System includes

#include <QSharedPointer>
#include <QVariant>
#include <rconnmon.h>
#include <cmmanagerdefines_shim.h>

// User includes

#include "wlanqtutilsap.h"
#include "wlanqtutilsconmonwrapper.h"
#include "wlanqtutilsconmonwrapperscan_s60_p.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutilsconmonwrapperscan_s60Traces.h"
#endif

/*!
    \class WlanQtUtilsConMonWrapperScan
    \brief Private wrapper for Symbian Connection Monitor library.

    Provides functionality to scan WLAN networks.
*/

// External function prototypes

// Local constants

//! Size of the scan result buffer. Good for about 100 networks.
#define SCAN_RESULT_BUFFER_SIZE 15000

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
    
    @param [in] wrapper Wrapper to report progress to.
 */

WlanQtUtilsConMonWrapperScan::WlanQtUtilsConMonWrapperScan(
    WlanQtUtilsConMonWrapper *wrapper) :
    CActive(EPriorityStandard),
    iWlanBuf(NULL),
    iWlanPtr(0, 0),
    iScanCancelled(EFalse),
    q_ptr(wrapper)
{
    OstTraceFunctionEntry1(WLANQTUTILSCONMONWRAPPERSCAN_WLANQTUTILSCONMONWRAPPERSCAN_ENTRY, this);
    
    CActiveScheduler::Add(this);
    // Errors in Connection Monitor Server connection are fatal so just
    // throw them as exceptions
    QT_TRAP_THROWING(iMonitor.ConnectL());

    OstTraceFunctionExit1(WLANQTUTILSCONMONWRAPPERSCAN_WLANQTUTILSCONMONWRAPPERSCAN_EXIT, this);
}

/*!
    Destructor.
 */

WlanQtUtilsConMonWrapperScan::~WlanQtUtilsConMonWrapperScan()
{
    OstTraceFunctionEntry1(DUP1_WLANQTUTILSCONMONWRAPPERSCAN_WLANQTUTILSCONMONWRAPPERSCAN_ENTRY, this);

    // Cancel possibly ongoing scan
    Cancel();
    // Cancel notifications and close connection monitor session
    iMonitor.Close();
    delete iWlanBuf;

    OstTraceFunctionExit1(DUP1_WLANQTUTILSCONMONWRAPPERSCAN_WLANQTUTILSCONMONWRAPPERSCAN_EXIT, this);
}

/*!
    Request a scan for available WLAN networks.
 */

void WlanQtUtilsConMonWrapperScan::ScanAvailableWlanAPs()
{
    OstTraceFunctionEntry1(WLANQTUTILSCONMONWRAPPERSCAN_SCANAVAILABLEWLANAPS_ENTRY, this);

    if (!IsActive()) {
        // Reserve the scan result buffer, if this is the first scan
        // request. Same buffer is used for the lifetime of this object.
        if (iWlanBuf == NULL) {
            iWlanBuf = new CConnMonWlanNetworksPtrArrayPckg(
                SCAN_RESULT_BUFFER_SIZE);
            iWlanPtr.Set(iWlanBuf->Buf()->Des());
        }

        iScanCancelled = EFalse;
        iMonitor.GetPckgAttribute(
            EBearerIdWLAN,
            0,
            KWlanNetworks,
            iWlanPtr,
            iStatus);
        SetActive();
    }

    OstTraceFunctionExit1(WLANQTUTILSCONMONWRAPPERSCAN_SCANAVAILABLEWLANAPS_EXIT, this);
}

/*!
    Stops a possibly ongoing WLAN scan.
 */

void WlanQtUtilsConMonWrapperScan::StopScan()
{
    OstTraceFunctionEntry1(WLANQTUTILSCONMONWRAPPERSCAN_STOPSCAN_ENTRY, this);

    iScanCancelled = ETrue;
    Cancel();

    OstTraceFunctionExit1(WLANQTUTILSCONMONWRAPPERSCAN_STOPSCAN_EXIT, this);
}

/*!
   From CActive: called when active object is cancelled.
 */

void WlanQtUtilsConMonWrapperScan::DoCancel()
{
    OstTraceFunctionEntry1(WLANQTUTILSCONMONWRAPPERSCAN_DOCANCEL_ENTRY, this);

    // Stop scan request, if one is active
    iMonitor.CancelAsyncRequest(EConnMonGetPckgAttribute);

    OstTraceFunctionExit1(WLANQTUTILSCONMONWRAPPERSCAN_DOCANCEL_EXIT, this);
}

/*!
   From CActive: called when async scan request has been completed.
   Reports results to owning wrapper.
 */

void WlanQtUtilsConMonWrapperScan::RunL()
{
    OstTraceFunctionEntry1(WLANQTUTILSCONMONWRAPPERSCAN_RUNL_ENTRY, this);
    
    OstTrace1(
        TRACE_NORMAL,
        WLANQTUTILSCONMONWRAPPERSCAN_RUNL_STATUS,
        "WlanQtUtilsConMonWrapperScan::RunL;iStatus=%d", iStatus.Int() );
    
    if (iStatus == KErrNone) {
        RConnMonWlanNetworksPtrArray wlanPtrArray;
        iWlanBuf->UnpackToL(wlanPtrArray);
        QList< QSharedPointer<WlanQtUtilsAp> > wlanScanResult;

        OstTrace1(
            TRACE_NORMAL,
            WLANQTUTILSCONMONWRAPPERSCAN_RUNL_COUNT,
            "WlanQtUtilsConMonWrapperScan::RunL;count=%d",
            wlanPtrArray.Count());
        
        for (TInt i = 0; i < wlanPtrArray.Count(); i++) {
            // Convert name to QString
            TBuf<CConnMonWlanNetwork::KMaxNameLength> wlanName =
                    wlanPtrArray[i]->Name();
            QString wlanNameInQt =
                QString::fromUtf16(wlanName.Ptr(), wlanName.Length());

            OstTraceExt1(
                TRACE_DUMP,
                WLANQTUTILSCONMONWRAPPERSCAN_RUNL,
                "WlanQtUtilsConMonWrapperScan::RunL;name=%S",
                wlanPtrArray[i]->Name());
            
            // Skip over hidden networks. There is no spec for what
            // their names will contain, but at least names with only
            // null characters and whitespace characters are seen
            // in real life.
            wlanNameInQt.remove(QChar());
            if (wlanNameInQt.trimmed().isEmpty()) {
                delete wlanPtrArray[i];
                continue;
            }
            
            // Go ahead and create the access point
            QSharedPointer<WlanQtUtilsAp> ap = 
                QSharedPointer<WlanQtUtilsAp>(new WlanQtUtilsAp());

            ap->setValue(WlanQtUtilsAp::ConfIdSsid, wlanNameInQt);

            StoreConMonConnMode(ap, wlanPtrArray[i]->ConnectionMode());
            ap->setValue(
                WlanQtUtilsAp::ConfIdSignalStrength, 
                wlanPtrArray[i]->SignalStrength());

            // Security mode values defined in enum TConnMonSecurityModeV2,
            // rconnmon.h.
            StoreConMonSecMode(ap, wlanPtrArray[i]->SecurityModeV2());

            // TODO: "Adding WLAN network manually" subfreature:
            // wps (Wifi Protected Setup) support, read this from connmon
            ap->setValue(WlanQtUtilsAp::ConfIdWpsSupported, false);
            // TODO: "Adding WLAN network manually" subfreature:
            // different scan requests needed to identify hidden networks
            ap->setValue(WlanQtUtilsAp::ConfIdHidden, false);
            
            wlanScanResult.append(ap);
            delete wlanPtrArray[i];
        }
        // Don't report results, if scan was cancelled
        if (!iScanCancelled) {
            emit q_ptr->availableWlanApsFromWrapper(wlanScanResult);                
        }
        // else: scan was probably stopped before results were reveived
        
        wlanPtrArray.Close();
        
        // QSharedPointer takes care of memory deallocation
        wlanScanResult.clear();
    }

    OstTraceFunctionExit1(WLANQTUTILSCONMONWRAPPERSCAN_RUNL_EXIT, this);
}

/*!
   Stores Security mode parameters received from ConnMon to \a ap class.

   @param ap Access Point object to where the configuration is stored
   @param conMonSecMode Security mode to store.
 */

void WlanQtUtilsConMonWrapperScan::StoreConMonSecMode(
    QSharedPointer<WlanQtUtilsAp> ap,
    TUint conMonSecMode)
    {
    // Entry-exit traces left out on purpose, because this is a simple
    // function and called many times inside a loop.
    
    CMManagerShim::WlanSecMode secMode;
    ap->setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);
    
    switch (conMonSecMode) {
    case EConnMonSecurityV2WepOpen:
    case EConnMonSecurityV2WepShared:
        secMode = CMManagerShim::WlanSecModeWep;
        break;
        
    case EConnMonSecurityV2802d1x:
        secMode = CMManagerShim::WlanSecMode802_1x;
        break;
        
    case EConnMonSecurityV2Wpa:
        secMode = CMManagerShim::WlanSecModeWpa;
        break;
        
    case EConnMonSecurityV2WpaPsk:
        secMode = CMManagerShim::WlanSecModeWpa;
        ap->setValue(WlanQtUtilsAp::ConfIdWpaPskUse, true);
        break;
        
    case EConnMonSecurityV2Wpa2:
        secMode = CMManagerShim::WlanSecModeWpa2;
        break;
        
    case EConnMonSecurityV2Wpa2Psk:
        secMode = CMManagerShim::WlanSecModeWpa2;
        ap->setValue(WlanQtUtilsAp::ConfIdWpaPskUse, true);
        break;
        
    case EConnMonSecurityV2Wapi:
    case EConnMonSecurityV2WapiPsk:
        secMode = CMManagerShim::WlanSecModeWapi;
        break;
        
    default:
        Q_ASSERT(conMonSecMode == EConnMonSecurityV2Open);
        secMode = CMManagerShim::WlanSecModeOpen;
        break;
    }
    
    ap->setValue(WlanQtUtilsAp::ConfIdSecurityMode, secMode);
}

/*!
   Stores connection mode parameters received from ConnMon to \a ap class.

   @param ap Access Point object to where the configuration is stored
   @param conMonConnMode Connection mode to store.
 */

void WlanQtUtilsConMonWrapperScan::StoreConMonConnMode(
    QSharedPointer<WlanQtUtilsAp> ap,
    TInt conMonConnMode)
{
    // Entry-exit traces left out on purpose, because this is a simple
    // function and called many times inside a loop.
    
    CMManagerShim::WlanConnMode connMode; 
    
    switch (conMonConnMode){
    case EConnMonInfraStructure:
    case EConnMonSecureInfra:
        connMode = CMManagerShim::Infra;
        break;
        
    default:
        Q_ASSERT(conMonConnMode == EConnMonAdHoc);
        connMode = CMManagerShim::Adhoc;
        break;
    }
    
    ap->setValue(WlanQtUtilsAp::ConfIdConnectionMode, connMode);
}
