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
* This is a source file for connmon library stub functions
*/

#include <QString>
#include <QObject>
#include <rconnmon.h>
#include <nifvar.h>
#include "wlanqtutilswlanap.h"

#ifdef __WINS__

// TODO: This function is copied from ConnMonWLANNetworksArrayPckg_v2.cpp because this
// function, although being in rconnmon.h, is not in the interface because IMPORT_C/EXPORT_C
// declarations are not used for this particular constructor. The only change that was made to 
// this version was to remove traces and some empty lines. This function is needed when
// creating output in RConnectionMonitor::GetPckgAttribute() for WLAN scan results.
// This is needed for both UT and emulator compilation.
CConnMonWlanNetworksPtrArrayPckg::CConnMonWlanNetworksPtrArrayPckg(
    const RPointerArray<CConnMonWlanNetwork>& aRef,
    TUint aBufLen )
    {
    RPointerArray<HBufC> items;
    TUint len( 0 );

    for ( TUint i( 0 ); i < aRef.Count(); ++i )
        {
        const HBufC* item( aRef[i]->ToBuf() );
        if ( item )
            {
            // Is there room in buffer
            TUint countFields( 3 ); // aRef.Count(); items->Count(); item->Length()
            if ( (countFields + len + item->Length()) > aBufLen )
                {
                delete item;
                item = NULL;
                break;
                }
            items.Append( item );
            ++( len += item->Length() ); // ++ is for item's size
            }
        else
            {
            ; // There was a trace.
            }
        }
    // Check that given buflen (aBufLen) is not smaller than one
    // item (TConnMonWLANNetwork) + aRef count + items count
    // + item length
    if ( items.Count() == 0 && aRef.Count() > 0 )
        {
        aBufLen = 2; // aRef.Count(), items.Count()
        }

    iBuf = HBufC::New( aBufLen ); // Set size exactly to a buffer length
    if ( !iBuf )
        {
        items.ResetAndDestroy();
        items.Close();
        return;
        }
    iBuf->Des().Append( aRef.Count() ); // Total amount of data
    iBuf->Des().Append( items.Count() );  // Amount of transferring data
    for ( TUint i(0); i<items.Count(); ++i )
        {
        iBuf->Des().Append( items[i]->Length() );
        iBuf->Des().Append( *items[i] );
        }
    items.ResetAndDestroy();
    items.Close();
    }

// -----------------------------------------------------------------------------
// CConnMonEventBase::CConnMonEventBase
// -----------------------------------------------------------------------------
//
CConnMonEventBase::CConnMonEventBase( const TInt aEventType, const TUint aConnectionId )
    {
    iEventType = aEventType;
    iConnectionId = aConnectionId;
    }

// Destructor
CConnMonEventBase::~CConnMonEventBase()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonEventBase::EventType
// -----------------------------------------------------------------------------
//
TInt CConnMonEventBase::EventType() const
    {
    return iEventType;
    }

// -----------------------------------------------------------------------------
// CConnMonConnectionStatusChange::CConnMonConnectionStatusChange
// -----------------------------------------------------------------------------
//
CConnMonConnectionStatusChange::CConnMonConnectionStatusChange(
    const TUint aConnectionId,
    const TUint aSubConnectionId,
    const TInt aConnectionStatus ) :
    CConnMonEventBase(EConnMonConnectionStatusChange, aConnectionId)
    {
    iSubConnectionId = aSubConnectionId;
    iConnectionStatus = aConnectionStatus;
    }

// Destructor
CConnMonConnectionStatusChange::~CConnMonConnectionStatusChange()
    {
    }

#ifdef WLANQTUTILS_T_WLANQTUTILS_UT_FLAG

// ----------------------------------------------
// Stub functions for connmon library, used in UT
// ----------------------------------------------

#include "wlanqtutilstestcontext.h"

extern WlanQtUtilsTestContext testContext;

EXPORT_C void RConnectionMonitor::GetConnectionCount(
    TUint& aConnectionCount,
    TRequestStatus& aStatus)
{
    aConnectionCount = testContext.connMon_.activeConnections_.activeConnList_.count();
    User::RequestComplete(&aStatus, KErrNone);
}

EXPORT_C TInt RConnectionMonitor::GetConnectionInfo(
        const TUint aIndex,
        TUint& aConnectionId,
        TUint& aSubConnectionCount ) const
{
    aConnectionId = testContext.connMon_.activeConnections_.activeConnList_[aIndex - 1]->connectionId();
    aSubConnectionCount = 0;
    return KErrNone; // TODO: put return value into context.
}

EXPORT_C void RConnectionMonitor::GetIntAttribute(
        const TUint aConnectionId,
        const TUint /* aSubConnectionId */,
        const TUint aAttribute,
        TInt& aValue,
        TRequestStatus& aStatus )
{
    WlanQtUtilsCtxActiveConn *activeConn = testContext.connMon_.activeConnections_.findActiveConn(aConnectionId);
    if (aAttribute == KBearer) {
        aValue = activeConn->connMonBearerType_;
    } else if (aAttribute == KConnectionStatus) {
        aValue = activeConn->connMonConnectionStatus_;
    } else {
        Q_ASSERT(false);
    }
    User::RequestComplete(&aStatus, KErrNone); // TODO: Take return value from the context.
}

EXPORT_C void RConnectionMonitor::GetUintAttribute(
        const TUint aConnectionId,
        const TUint /* aSubConnectionId */,
        const TUint aAttribute,
        TUint& aValue,
        TRequestStatus& aStatus )
{
    WlanQtUtilsCtxActiveConn *activeConn = testContext.connMon_.activeConnections_.findActiveConn(aConnectionId);
    if (aAttribute == KIAPId) {
        aValue = activeConn->iapId();
    } else {
        Q_ASSERT(false);
    }

    User::RequestComplete(&aStatus, KErrNone); // TODO: Take return value from the context.
}

EXPORT_C void RConnectionMonitor::GetPckgAttribute(
    const TUint aConnectionId,
    const TUint aSubConnectionId,
    const TUint aAttribute,
    TDes8& aValue,
    TRequestStatus& aStatus ) const
{
    // This is copy-paste from real implementation and we'll use that for other requests.
    TIpcArgs args(aConnectionId, aSubConnectionId, aAttribute, &aValue);
    SendReceive(EReqGetPckgAttribute, args, aStatus);
}

// ---------------------------------------------------------
// connUtils2ConnMonSecModeMap()
// Local function to map WlanQtUtilsWlanSecMode into Connection
// monitor's security mode. Currently for UT use only.
// ---------------------------------------------------------
//
static TUint connUtils2ConnMonSecModeMap(WlanQtUtilsWlanSecMode connUtilsWlanSecMode)
{
    TUint ret;
    switch (connUtilsWlanSecMode) {
    case WlanQtUtilsWlanSecModeWep:
        ret = EConnMonSecurityWep;
        break;
    case WlanQtUtilsWlanSecMode802_1x:
        ret = EConnMonSecurity802d1x;        
        break;
    case WlanQtUtilsWlanSecModeWpa:
        ret = EConnMonSecurityWpa;
        break;
    case WlanQtUtilsWlanSecModeWpa2:
        ret = EConnMonSecurityWpaPsk;
        break;
    default:
        ret = EConnMonSecurityOpen;
        break;
    }
    return ret;
}



EXPORT_C void RConnectionMonitor::GetPckgAttribute(
    const TUint /* aConnectionId */,
    const TUint /* aSubConnectionId */,
    const TUint /* aAttribute */,
    TDes16& aValue,
    TRequestStatus& aStatus ) const
    {
    // TODO: verify input parameters, i.e., first three params...
    
    RConnMonWlanNetworksPtrArray wlanPtrArray;
    RBuf vendorData;

    for (int i = 0; i < testContext.connMon_.wlanScanResult_.wlanScanResultList_.count(); i++) {
        WlanQtUtilsWlanAp* ap = testContext.connMon_.wlanScanResult_.wlanScanResultList_[i];
        TBufC<CConnMonWlanNetwork::KMaxNameLength> name(ap->ssid().utf16());

        TBufC<CConnMonWlanNetwork::KWlanBssId> bssid16(QString("addMAC").utf16());
        TBuf8<CConnMonWlanNetwork::KWlanBssId> bssid;
        bssid.Copy(bssid16);
        
        CConnMonWlanNetwork* wlanAp = CConnMonWlanNetwork::NewL(
            name,
            ap->connectionMode(),
            ap->signalStrength(),
            connUtils2ConnMonSecModeMap(ap->securityMode()),
            bssid,
            vendorData );
        wlanPtrArray.Append(wlanAp);
    }

    CConnMonWlanNetworksPtrArrayPckg wlanBuf(wlanPtrArray, 2560);
    aValue.Copy(wlanBuf.Buf()->Des());
    
    User::RequestComplete(&aStatus, KErrNone); // TODO: enable return value modification.
    }

#else // WLANQTUTILS_T_WLANQTUTILS_UT_FLAG

// ----------------------------------------------------
// Stub functions for connmon library, used in emulator
// ----------------------------------------------------

#if 0
// In emulator, there are always two active connections.
// The following functions return different properties of these active connection
// so that it's easy to test in emulator.

EXPORT_C void RConnectionMonitor::GetConnectionCount(
    TUint& aConnectionCount,
    TRequestStatus& aStatus)
{
    aConnectionCount = 2;
    User::RequestComplete(&aStatus, KErrNone);
}

EXPORT_C TInt RConnectionMonitor::GetConnectionInfo(
        const TUint aIndex,
        TUint& aConnectionId,
        TUint& aSubConnectionCount ) const
{
    if (aIndex == 1) {
        aConnectionId = 10;
    } else if (aIndex == 2) {
        aConnectionId = 20;
    } else {
        Q_ASSERT(false);
    }

    aSubConnectionCount = 0;
    return KErrNone;
}

EXPORT_C void RConnectionMonitor::GetIntAttribute(
        const TUint aConnectionId,
        const TUint /* aSubConnectionId */,
        const TUint aAttribute,
        TInt& aValue,
        TRequestStatus& aStatus )
{
    if (aAttribute == KBearer) {
        if (aConnectionId == 10) {
            aValue = EBearerGPRS;
        } else if (aConnectionId == 20) {
            aValue = EBearerWLAN;
        } else {
            Q_ASSERT(false);
        }
    } else if (aAttribute == KConnectionStatus) {
        if (aConnectionId == 10) {
            aValue = KConnectionOpen;
        } else if (aConnectionId == 20) {
            aValue = KStartingConnection;
        } else {
            Q_ASSERT(false);
        }
    } else if (aAttribute == KSignalStrength) {
        if (aConnectionId == 10) {
            Q_ASSERT(false);
        } else if (aConnectionId == 20) {
            aValue = 78;
        } else {
            Q_ASSERT(false);
        }
    } else if (aAttribute == KNetworkMode) {
        if (aConnectionId == 10) {
            Q_ASSERT(false);
        } else if (aConnectionId == 20) {
            aValue = EConnMonInfraStructure;
        } else {
            Q_ASSERT(false);
        }
    } else if (aAttribute == KSecurityMode) {
        if (aConnectionId == 10) {
            Q_ASSERT(false);
        } else if (aConnectionId == 20) {
            aValue = EConnMonSecurityWpaPsk;
        } else {
            Q_ASSERT(false);
        }
    } else {
        Q_ASSERT(false);
    }
    User::RequestComplete(&aStatus, KErrNone);
}

EXPORT_C void RConnectionMonitor::GetUintAttribute(
        const TUint aConnectionId,
        const TUint /* aSubConnectionId */,
        const TUint aAttribute,
        TUint& aValue,
        TRequestStatus& aStatus )
{
    if (aAttribute == KIAPId) {
        if (aConnectionId == 10) {
            aValue = 1;
        } else if (aConnectionId == 20) {
            aValue = 10;
        } else {
            Q_ASSERT(false);
        }
    } else if (aAttribute == KDownlinkData) {
        if (aConnectionId == 10) {
            aValue = 123;
        } else if (aConnectionId == 20) {
            aValue = 12345;
        } else {
            Q_ASSERT(false);
        }
    } else if (aAttribute == KUplinkData) {
        if (aConnectionId == 10) {
            aValue = 987654321;
        } else if (aConnectionId == 20) {
            aValue = 1234567890;
        } else {
            Q_ASSERT(false);
        }
    } else if (aAttribute == KTransmitPower) {
        if (aConnectionId == 10) {
            Q_ASSERT(false);
        } else if (aConnectionId == 20) {
            aValue = 50;
        } else {
            Q_ASSERT(false);
        }
    } else {
        Q_ASSERT(false);
    }

    User::RequestComplete(&aStatus, KErrNone);
}

EXPORT_C void RConnectionMonitor::GetBoolAttribute(
        const TUint aConnectionId,
        const TUint /* aSubConnectionId */,
        const TUint aAttribute,
        TBool& aValue,
        TRequestStatus& aStatus )
{
    if (aAttribute == KConnectionActive) {
        if (aConnectionId == 10) {
            aValue = EFalse;
        } else if (aConnectionId == 20) {
            aValue = ETrue;
        } else {
            Q_ASSERT(false);
        }
    } else {
        Q_ASSERT(false);
    }

    User::RequestComplete(&aStatus, KErrNone);
}

EXPORT_C void RConnectionMonitor::GetStringAttribute(
    const TUint aConnectionId,
    const TUint /* aSubConnectionId */,
    const TUint aAttribute,
    TDes& aValue,
    TRequestStatus& aStatus ) const
{
    if (aAttribute == KIAPName) {
        if (aConnectionId == 10) {
            _LIT(iapNameLit, "PACKET DATA 1");
            TBufC<KConnMonMaxStringAttributeLength> iapName(iapNameLit);
            aValue = iapName.Des();
        } else if (aConnectionId == 20) {
            _LIT(iapNameLit, "WLAN IAP 3");
            TBufC<KConnMonMaxStringAttributeLength> iapName(iapNameLit);
            aValue = iapName.Des();
        } else {
            Q_ASSERT(false);
        }
    } else if (aAttribute == KNetworkName) {
        if (aConnectionId == 10) {
            Q_ASSERT(false);
        } else if (aConnectionId == 20) {
            _LIT(ssidLit, "WLAN SSID 3");
            TBufC<KConnMonMaxStringAttributeLength> ssid(ssidLit);
            aValue = ssid.Des();
        } else {
            Q_ASSERT(false);
        }
    } else if (aAttribute == KAccessPointName) {
        if (aConnectionId == 10) {
            _LIT(iapNameLit, "PACKET DATA 1");
            TBufC<KConnMonMaxStringAttributeLength> iapName(iapNameLit);
            aValue = iapName.Des();
        } else if (aConnectionId == 20) {
            Q_ASSERT(false);
        } else {
            Q_ASSERT(false);
        }
    } else {
        Q_ASSERT(false);
    }

    User::RequestComplete(&aStatus, KErrNone);
}

EXPORT_C void RConnectionMonitor::GetPckgAttribute(
    const TUint aConnectionId,
    const TUint aSubConnectionId,
    const TUint aAttribute,
    TDes8& aValue,
    TRequestStatus& aStatus ) const
{
    if (aAttribute == KStartTime) {
        if (aConnectionId == 10) {
            TDateTime dateTime(2009, EMarch, 31-1, 15, 15, 15, 0);
            TTime time(dateTime);
            TConnMonTimeBuf timePckg(time);
            aValue.Copy(timePckg);
        } else if (aConnectionId == 20) {
            TTime time;
            time.UniversalTime();
            TConnMonTimeBuf timePckg(time);
            aValue.Copy(timePckg);
        } else {
            Q_ASSERT(false);
        }
        User::RequestComplete(&aStatus, KErrNone);
    } else if (aAttribute == KClientInfo) {
        TConnMonClientEnum applications;
        if (aConnectionId == 10) {
            applications.iCount = 1;
            applications.iUid[0].iUid = 0x10008D39; // Web
            applications.iUid[1].iUid = 0;
            applications.iUid[2].iUid = 0;
            applications.iUid[3].iUid = 0;
            applications.iUid[4].iUid = 0;
            applications.iUid[5].iUid = 0;
            applications.iUid[6].iUid = 0;
            applications.iUid[7].iUid = 0;
            applications.iUid[8].iUid = 0;
            applications.iUid[9].iUid = 0;
        } else if (aConnectionId == 20) {
            applications.iCount = 7;
            applications.iUid[0].iUid = 0x101fd9c5; // KBannedServerUID
            applications.iUid[1].iUid = 0x1020728E; // KFeedsServerUid
            applications.iUid[2].iUid = 0x10008D60; // KDownloadMgrServerUid
            applications.iUid[3].iUid = 0x1000484b; // KMessagingServerUid
            applications.iUid[4].iUid = 0x102033E6; // KJavaVMUid
            applications.iUid[5].iUid = 0x102073CA; // KSUPLServerUid
            applications.iUid[6].iUid = 0x200212F3; // Connect Screen
            applications.iUid[7].iUid = 0;
            applications.iUid[8].iUid = 0;
            applications.iUid[9].iUid = 0;
        } else {
            Q_ASSERT(false);
        }
        TPckgBuf< TConnMonClientEnum > applicationsPckg( applications );
        aValue.Copy(applicationsPckg);
        User::RequestComplete(&aStatus, KErrNone);
    } else {
        // This is copy-paste from real implementation and we'll use that for other requests.
        TIpcArgs args( aConnectionId, aSubConnectionId, aAttribute, &aValue );
        SendReceive( EReqGetPckgAttribute, args, aStatus );
    }
}
#endif

// This function returns WLAN scan results.
// Six results in every second scan and the others have two.
EXPORT_C void RConnectionMonitor::GetPckgAttribute(
    const TUint /* aConnectionId */,
    const TUint /* aSubConnectionId */,
    const TUint aAttribute,
    TDes16& aValue,
    TRequestStatus& aStatus ) const {

    if (aAttribute == KWlanNetworks) {
        RConnMonWlanNetworksPtrArray wlanPtrArray;
        RBuf vendorData;
    
        TBufC<CConnMonWlanNetwork::KWlanBssId> bssid16(QString("addMAC").utf16());
        TBuf8<CConnMonWlanNetwork::KWlanBssId> bssid;
        bssid.Copy(bssid16);
    
        _LIT(wlanAp1Name, "Test AP 1");
        TBufC<CConnMonWlanNetwork::KMaxNameLength> name1(wlanAp1Name);
        CConnMonWlanNetwork* wlanAp1 = CConnMonWlanNetwork::NewL(
            name1,
            EConnMonInfraStructure,
            20,
            EConnMonSecurityOpen,
            bssid,
            vendorData );
        wlanPtrArray.Append(wlanAp1);
    
        _LIT(wlanAp2Name, "Test AP 2");
        TBufC<CConnMonWlanNetwork::KMaxNameLength> name2(wlanAp2Name);
        CConnMonWlanNetwork* wlanAp2 = CConnMonWlanNetwork::NewL(
            name2,
            EConnMonAdHoc,
            20,
            EConnMonSecurityOpen,
            bssid,
            vendorData );
        wlanPtrArray.Append(wlanAp2);
    
        static bool refresh = true;
    
        if (refresh) {
            _LIT(wlanAp3Name, "Test AP 3");
            TBufC<CConnMonWlanNetwork::KMaxNameLength> name3(wlanAp3Name);
            CConnMonWlanNetwork* wlanAp3 = CConnMonWlanNetwork::NewL(
                name3,
                EConnMonInfraStructure,
                20,
                EConnMonSecurityWep,
                bssid,
                vendorData );
            wlanPtrArray.Append(wlanAp3);
    
            _LIT(wlanAp4Name, "Test AP 4");
            TBufC<CConnMonWlanNetwork::KMaxNameLength> name4(wlanAp4Name);
            CConnMonWlanNetwork* wlanAp4 = CConnMonWlanNetwork::NewL(
                name4,
                EConnMonInfraStructure,
                20,
                EConnMonSecurityOpen,
                bssid,
                vendorData );
            wlanPtrArray.Append(wlanAp4);
    
            _LIT(wlanAp5Name, "Test AP 5");
            TBufC<CConnMonWlanNetwork::KMaxNameLength> name5(wlanAp5Name);
            CConnMonWlanNetwork* wlanAp5 = CConnMonWlanNetwork::NewL(
                name5,
                EConnMonInfraStructure,
                20,
                EConnMonSecurityWpa,
                bssid,
                vendorData );
            wlanPtrArray.Append(wlanAp5);
    
            _LIT(wlanAp6Name, "Test AP 6");
            TBufC<CConnMonWlanNetwork::KMaxNameLength> name6(wlanAp6Name);
            CConnMonWlanNetwork* wlanAp6 = CConnMonWlanNetwork::NewL(
                name6,
                EConnMonInfraStructure,
                20,
                EConnMonSecurityWpaPsk,
                bssid,
                vendorData );
            wlanPtrArray.Append(wlanAp6);
    
            refresh = false;
        } else {
            refresh = true;
        }
    
        CConnMonWlanNetworksPtrArrayPckg wlanBuf(wlanPtrArray, 2560); // TODO: buffer size to more dynamic or use constant
        aValue.Copy(wlanBuf.Buf()->Des());
    } else {
        Q_ASSERT(false);
    }
    
    User::RequestComplete(&aStatus, KErrNone);
}

#endif // WLANQTUTILS_T_WLANQTUTILS_UT_FLAG
#endif // __WINS__
