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
* Symbian platform specific implementation of WLAN scanning.
*/

// System includes

#include <QList>
#include <cmmanagerdefines_shim.h>
#include <utf.h>

// User includes

#include "wlanqtutils.h"
#include "wlanqtutilsap.h"
#include "wlanqtutilsscan.h"
#include "wlanqtutilsscan_symbian.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutilsscan_symbianTraces.h"
#endif


/*!
    \class WlanQtUtilsScanPrivate
    \brief Symbian platform specific implementation of WLAN scanning.

    Symbian platform specific implementation of WLAN scanning. 
*/


// External function prototypes

// Local constants

// Information Element ID for SSID as specified in 802.11.
static const TUint8 KWlan802Dot11SsidIE = 0;

// Bit mask for Capability info field to get type (Infra/AdHoc).
static const TUint8 KWlan802Dot11CapabilityEssMask = 0x0001;

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Static factory constructor.
    
    @param [in,out] wrapper Public implementation class reference.
    
    @return New instance of the class.
*/

WlanQtUtilsScanPrivate *WlanQtUtilsScanPrivate::NewL(WlanQtUtilsScan *wrapper)
{
    OstTraceFunctionEntry0(WLANQTUTILSSCANPRIVATE_NEWL_ENTRY);
    
    WlanQtUtilsScanPrivate *self = new (ELeave) WlanQtUtilsScanPrivate(wrapper);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    
    OstTraceFunctionExit0(WLANQTUTILSSCANPRIVATE_NEWL_EXIT);
    return self;
}

/*!
    Destructor.
*/

WlanQtUtilsScanPrivate::~WlanQtUtilsScanPrivate()
{
    OstTraceFunctionEntry0(WLANQTUTILSSCANPRIVATE_WLANQTUTILSSCANPRIVATE_ENTRY);
    
    Cancel();
    delete mWlanMgmtClient;
    mWlanMgmtClient = NULL;
    delete mResults;
    mResults = NULL;
    
    OstTraceFunctionExit0(WLANQTUTILSSCANPRIVATE_WLANQTUTILSSCANPRIVATE_EXIT);
}

/*!
    Starts a broadcast scan of available access points.
*/

void WlanQtUtilsScanPrivate::Scan()
{
    OstTraceFunctionEntry0(WLANQTUTILSSCANPRIVATE_SCAN_ENTRY);
    
    // Scanning while previous scan is not complete is not supported
    Q_ASSERT(!IsActive());
    
    // Make the scan
    mWlanMgmtClient->GetScanResults(iStatus, *mResults);
    SetActive();
    
    OstTraceFunctionExit0(WLANQTUTILSSCANPRIVATE_SCAN_EXIT);
}

/*!
    Starts a direct scan of available access points having given SSID.
    
    @param ssid SSID to scan.
*/

void WlanQtUtilsScanPrivate::Scan(const QString &ssid)
{
    OstTraceFunctionEntry0(WLANQTUTILSSCANPRIVATE_SCAN_SSID_ENTRY);
    
    // Scanning while previous scan is not complete is not supported
    Q_ASSERT(!IsActive());
    
    // Convert from QString to TBuf8
    TBuf<KWlanMaxSsidLength> buffer(ssid.utf16());
    TInt error = CnvUtfConverter::ConvertFromUnicodeToUtf8(mWlanSsid, buffer);
    qt_symbian_throwIfError(error);

    OstTraceExt1(
        TRACE_NORMAL,
        WLANQTUTILSSCANPRIVATE_SCAN_SSID,
        "WlanQtUtilsScanPrivate::Scan;mWlanSsid=%s",
        mWlanSsid);

    // Make the scan
    mWlanMgmtClient->GetScanResults(mWlanSsid, iStatus, *mResults);
    SetActive();

    OstTraceFunctionExit0(WLANQTUTILSSCANPRIVATE_SCAN_SSID_EXIT);
}

/*!
    Stops an ongoing scan.
*/

void WlanQtUtilsScanPrivate::StopScan()
{
    OstTraceFunctionEntry0(WLANQTUTILSSCANPRIVATE_STOPSCAN_ENTRY);
    
    Cancel();
    
    OstTraceFunctionExit0(WLANQTUTILSSCANPRIVATE_STOPSCAN_EXIT);
}

/*!
    Scan results handler.
*/

void WlanQtUtilsScanPrivate::RunL()
{
    OstTraceFunctionEntry0(WLANQTUTILSSCANPRIVATE_RUNL_ENTRY);
    
    QT_TRYCATCH_LEAVING(
        if (iStatus != KErrNone) {
            emit q_ptr->scanFailed(WlanQtUtils::ScanStatusError);
        } else {
            QList< QSharedPointer<WlanQtUtilsAp> > scanResults;
            ExtractScanResults(scanResults);
            
            // Inform about the results 
            emit q_ptr->availableWlanAps(scanResults);
        }
    );
    
    OstTraceFunctionExit0(WLANQTUTILSSCANPRIVATE_RUNL_EXIT);
}

/*!
    Cancels an outstanding request.
*/

void WlanQtUtilsScanPrivate::DoCancel()
{
    OstTraceFunctionEntry0(WLANQTUTILSSCANPRIVATE_DOCANCEL_ENTRY);
    
    mWlanMgmtClient->CancelGetScanResults();

    OstTraceFunctionExit0(WLANQTUTILSSCANPRIVATE_DOCANCEL_EXIT);
}

/*!
    Constructor.
    
    @param [in,out] wrapper Public implementation class reference.
*/

WlanQtUtilsScanPrivate::WlanQtUtilsScanPrivate(WlanQtUtilsScan *wrapper) :
    CActive(EPriorityStandard),
    q_ptr(wrapper),
    mWlanMgmtClient(NULL),
    mResults(NULL)
{
    OstTraceFunctionEntry0(DUP1_WLANQTUTILSSCANPRIVATE_WLANQTUTILSSCANPRIVATE_ENTRY);
    OstTraceFunctionExit0(DUP1_WLANQTUTILSSCANPRIVATE_WLANQTUTILSSCANPRIVATE_EXIT);
}

/*!
    Second phase constructor.
*/

void WlanQtUtilsScanPrivate::ConstructL()
{
    OstTraceFunctionEntry0(WLANQTUTILSSCANPRIVATE_CONSTRUCTL_ENTRY);
    
    CActiveScheduler::Add(this);
    mWlanMgmtClient = CWlanMgmtClient::NewL();
    mResults = CWlanScanInfo::NewL();
    
    OstTraceFunctionExit0(WLANQTUTILSSCANPRIVATE_CONSTRUCTL_EXIT);
}

/*
    Extracts scan results and stores them into the given AP list.
    
    @param [out] scanResults Scan result list.
*/

void WlanQtUtilsScanPrivate::ExtractScanResults(
    QList< QSharedPointer<WlanQtUtilsAp> > &scanResults)
{
    for (mResults->First(); !mResults->IsDone(); mResults->Next()) {
        // Create an access point for each result
        QSharedPointer<WlanQtUtilsAp> ap(new WlanQtUtilsAp());
        
        // SSID
        QString ssid = ExtractSsid();

        // Skip over hidden networks. There is no spec for what
        // their names will contain, but at least names with only
        // null characters and whitespace characters are seen
        // in real life.
        if (ssid.trimmed().isEmpty()) {
            continue;
        }
        ap->setValue(WlanQtUtilsAp::ConfIdSsid, ssid);
        
        // BSSID
        QByteArray bssid = ExtractBssid();
        ap->setValue(WlanQtUtilsAp::ConfIdBssid, bssid);
        
        // Signal strength
        ap->setValue(
            WlanQtUtilsAp::ConfIdSignalStrength,
            (int)mResults->RXLevel());

        // Connection Mode
        CMManagerShim::WlanConnMode connMode;
        if (mResults->Capability() & KWlan802Dot11CapabilityEssMask) {
            connMode = CMManagerShim::Infra;
        } else {
            connMode = CMManagerShim::Adhoc;
        }
        ap->setValue(WlanQtUtilsAp::ConfIdConnectionMode, (int)connMode);

        // Security Mode parameters
        StoreSecMode(ap, mResults->ExtendedSecurityMode());
        
        // Hidden attribute
        // These scan results do not tell if the AP is hidden or not
        ap->setValue(WlanQtUtilsAp::ConfIdHidden, false);
        
        // WPS support
        TBool wpsSupported = mResults->IsProtectedSetupSupported();
        ap->setValue(
            WlanQtUtilsAp::ConfIdWpsSupported,
            (wpsSupported == EFalse) ? false : true);

        // Append the AP to scan result list
        scanResults.append(ap);
    }
}

/*!
    Extracts and cleans up the WLAN SSID from current scan result element.
    
    @return SSID string.
*/

QString WlanQtUtilsScanPrivate::ExtractSsid()
{
    // Get the SSID in raw data format
    TUint8 ieLen;
    const TUint8* ieData;
    TInt ret = mResults->InformationElement(KWlan802Dot11SsidIE, ieLen, &ieData);
    
    // Convert into QString
    QString ssid;
    if (ret == KErrNone && ieLen > 0) {
        // Trace the buffer as data to ease low level debugging
        OstTraceData(
            TRACE_DUMP,
            WLANQTUTILSSCANPRIVATE_EXTRACTSSID_DATA,
            "WlanQtUtilsScanPrivate::ExtractSsid data 0x%{hex8[]}",
            ieData,
            ieLen);

        ssid = QString::fromUtf8((char *)ieData, ieLen);

#ifdef OST_TRACE_COMPILER_IN_USE
        TPtrC16 string(ssid.utf16(), ssid.length());
        OstTraceExt1(
            TRACE_DUMP,
            WLANQTUTILSSCANPRIVATE_EXTRACTSSID_STRING,
            "WlanQtUtilsScanPrivate::ExtractSsid string;ssid=%S",
            string);
#endif
    }
    
    // Remove nul characters
    ssid.remove(QChar());
    
    return ssid;
}

/*!
    Extracts the WLAN BSSID from current scan result element.
    
    @return BSSID array.
*/

QByteArray WlanQtUtilsScanPrivate::ExtractBssid()
{
    TWlanBssid wlanBssid;
    mResults->Bssid(wlanBssid);
    QByteArray bssid;
    for (int i = 0; i < (int)wlanBssid.Length(); i++) {
        bssid[i] = (char)wlanBssid[i];
    }

#ifdef OST_TRACE_COMPILER_IN_USE
    QString bssidHex(bssid.toHex());
    TPtrC16 string(bssidHex.utf16(), bssidHex.length());

    OstTraceExt1(
        TRACE_DUMP,
        WLANQTUTILSSCANPRIVATE_EXTRACTBSSID,
        "WlanQtUtilsScanPrivate::ExtractBssid;bssid=%S",
        string);
#endif

    return bssid;
}

/*!
    Stores Security mode parameters to \a ap class.

    @param [in,out] ap Access Point object to where the configuration is stored.
    @param [in] secMode Security mode to store.
*/

void WlanQtUtilsScanPrivate::StoreSecMode(
    QSharedPointer<WlanQtUtilsAp> ap,
    TUint wlanSecMode)
{
    CMManagerShim::WlanSecMode secMode = CMManagerShim::WlanSecModeOpen;
    ap->setValue(WlanQtUtilsAp::ConfIdWpaPskUse, false);
    
    switch (wlanSecMode) {
    case EWlanConnectionExtentedSecurityModeWepOpen:
    case EWlanConnectionExtentedSecurityModeWepShared:
        secMode = CMManagerShim::WlanSecModeWep;
        break;
    
    case EWlanConnectionExtentedSecurityMode802d1x:
        secMode = CMManagerShim::WlanSecMode802_1x;
        break;
        
    case EWlanConnectionExtentedSecurityModeWpa:
        secMode = CMManagerShim::WlanSecModeWpa;
        break;
        
    case EWlanConnectionExtentedSecurityModeWpaPsk:
        secMode = CMManagerShim::WlanSecModeWpa;
        ap->setValue(WlanQtUtilsAp::ConfIdWpaPskUse, true);
        break;
        
    case EWlanConnectionExtentedSecurityModeWpa2:
        secMode = CMManagerShim::WlanSecModeWpa2;
        break;
        
    case EWlanConnectionExtentedSecurityModeWpa2Psk:
        secMode = CMManagerShim::WlanSecModeWpa2;
        ap->setValue(WlanQtUtilsAp::ConfIdWpaPskUse, true);
        break;

    case EWlanConnectionExtentedSecurityModeWapi:
    case EWlanConnectionExtentedSecurityModeWapiPsk:
        secMode = CMManagerShim::WlanSecModeWapi;
        break;

    case EWlanConnectionExtentedSecurityModeOpen:
        secMode = CMManagerShim::WlanSecModeOpen;
        break;
        
#ifndef QT_NO_DEBUG
    default:
        // Invalid security mode detected
        Q_ASSERT(0);
        break;
#endif        
    }
    
    ap->setValue(WlanQtUtilsAp::ConfIdSecurityMode, secMode);
}
