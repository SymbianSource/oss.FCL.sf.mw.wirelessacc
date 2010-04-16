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
 * This is a source file for CmmWrapperPrivate class.
 */

// INCLUDE FILES
#include <e32base.h>
#include <cmmanagerdef.h>
#include <cmconnectionmethodext.h>
#include <cmdestinationext.h>
#include <cmpluginwlandef.h>
#include "wlanqtutilscommon.h"
#include "wlanqtutilsiap.h"
#include "wlanqtutilswlanap.h"
#include "wlanqtutilswlaniap.h"
#include "wlanqtutilscmmwrapper.h"
#include "wlanqtutilscmmwrapper_s60_p.h"

#ifdef WLANQTUTILS_NO_OST_TRACES_FLAG
#include <opensystemtrace.h>
#else
#include "OstTraceDefinitions.h"
#endif
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutilscmmwrapper_s60Traces.h"
#endif


// =========== PRIVATE CLASS MEMBER FUNCTIONS ===============
//
// ---------------------------------------------------------
// CmmWrapperPrivate::CmmWrapperPrivate()
// Constructor
// ---------------------------------------------------------
//
CmmWrapperPrivate::CmmWrapperPrivate(CmmWrapper *wrapper) :
    q_ptr(wrapper)
    {
    OstTraceFunctionEntry1( CMMWRAPPERPRIVATE_CMMWRAPPERPRIVATE_ENTRY, this );
    
    TRAPD(err, iCmManagerExt.OpenL());
    if (err != KErrNone) {
        // todo
    }
    
    OstTraceFunctionExit1( CMMWRAPPERPRIVATE_CMMWRAPPERPRIVATE_EXIT, this );
    }

// ---------------------------------------------------------
// CmmWrapperPrivate::CmmWrapperPrivate()
// Destructor
// ---------------------------------------------------------
//
CmmWrapperPrivate::~CmmWrapperPrivate()
    {
    OstTraceFunctionEntry1( CMMWRAPPERPRIVATE_CMMWRAPPERPRIVATEDESTR_ENTRY, this );
    
    iCmManagerExt.Close();

    OstTraceFunctionExit1( CMMWRAPPERPRIVATE_CMMWRAPPERPRIVATEDESTR_EXIT, this );
    }

// ---------------------------------------------------------
// CmmWrapperPrivate::fetchIaps()
// ---------------------------------------------------------
//
int CmmWrapperPrivate::fetchIaps(QList<WlanQtUtilsIap*>& iapList)
    {    
    TRAPD(err, fetchIapsL(iapList));
    return err;
    }

// ---------------------------------------------------------
// CmmWrapperPrivate::fetchIapsL()
// ---------------------------------------------------------
//
void CmmWrapperPrivate::fetchIapsL(QList<WlanQtUtilsIap*>& iapList)
    {
    OstTraceFunctionEntry1( CMMWRAPPERPRIVATE_FETCHIAPSL_ENTRY, this );
    
    // reset the list, just in case
    iapList.clear();
    
    //Array granularity 10
    RArray<TUint32> destinationIdArray = RArray<TUint32> (10);

    destinationIdArray.Reset();
    iCmManagerExt.AllDestinationsL(destinationIdArray);

    // looping all the SNAPs through
    QList<WlanQtUtilsIap*> snapsIapList;
    for (TInt i = 0; i < destinationIdArray.Count(); i++)
        {
        snapsIapList.clear();
        RCmDestinationExt dest;
        dest = iCmManagerExt.DestinationL(destinationIdArray[i]);
        TUint32 id = dest.Id();

        if (getIapListFromSnap(id, snapsIapList) == KErrNone)
            {
            iapList.append(snapsIapList);
            }
        }

    OstTraceFunctionExit1( CMMWRAPPERPRIVATE_FETCHIAPSL_EXIT, this );
    }

// ---------------------------------------------------------
// CmmWrapperPrivate::createWlanIap()
// Creates a new WLAN IAP
// ---------------------------------------------------------
//
WlanQtUtilsWlanIap *CmmWrapperPrivate::createWlanIap(const WlanQtUtilsWlanAp *aWlanAp)
    {
    WlanQtUtilsWlanIap *iap = NULL;

    TRAPD(err, iap = createWlanIapL(aWlanAp));

    if (err != KErrNone)
        {
        // Todo: trace
        }
    return iap;
    }

// ---------------------------------------------------------
// CmmWrapperPrivate::createWlanIapL()
// Creates a new WLAN IAP
// ---------------------------------------------------------
//
WlanQtUtilsWlanIap *CmmWrapperPrivate::createWlanIapL(const WlanQtUtilsWlanAp *aWlanAp)
    {
    OstTraceFunctionEntry1( CMMWRAPPERPRIVATE_CREATEWLANIAPL_ENTRY, this );
    
    // Create the new IAP
    RCmConnectionMethodExt plugin = iCmManagerExt.CreateConnectionMethodL(
            KUidWlanBearerType);
    CleanupClosePushL(plugin);

    // Convert QString to Symbian buffer
    TBufC<KMaxSsidLen> buffer(aWlanAp->ssid().utf16());

    WlanQtUtilsWlanSecMode secMode = aWlanAp->securityMode();
    int cmManagersSecMode = wlanQtUtils2CmmSecModeMap(secMode);

    plugin.SetStringAttributeL(CMManager::ECmName, buffer);
    plugin.SetStringAttributeL(CMManager::EWlanSSID, buffer);
    plugin.SetIntAttributeL(CMManager::EWlanSecurityMode, cmManagersSecMode);
    // Todo: other parameters such as security keys
#if 0
    // TODO the security settings API plugin needs to be initialized here
    // with the iap id.
    // Check if the key value has been given
    QString key = aWlanAp->securityKey();
    if ( key.length() > 0 )
        {
        // Set the security key values here
        if ( secMode == CMManager::EWlanSecModeWpa || 
             secMode == CMManager::EWlanSecModeWpa2 )
            {
            // Store the WPA key to the database using the security settings API
            TBufC<KMaxWEPKeyLen> bufKey(key.utf16());
            plugin.SetStringAttributeL( CMManager::EWlanWpaPreSharedKey, bufKey );
        
            }
        else if ( secMode == CMManager::EWlanSecModeWep )
            {
            // Store the WPA key to the database using the security settings API
            TBufC<KMaxWPAKeyLen> bufKey(key.utf16());
            plugin.SetStringAttributeL( CMManager::EWlanWepKey1, bufKey );
            }
        }
#endif

    plugin.UpdateL();

    TInt iapId = plugin.GetIntAttributeL(CMManager::ECmIapId);
    
    CleanupStack::PopAndDestroy(&plugin); // Close() called on "plugin"

    // Read details of the new IAP and create the WLAN IAP class
    RCmConnectionMethodExt connectionMethod = iCmManagerExt.ConnectionMethodL( iapId );
    WlanQtUtilsIap *newIap = getIapInfoL(connectionMethod);
    
    OstTraceFunctionExit1( CMMWRAPPERPRIVATE_CREATEWLANIAPL_EXIT, this );
    return qobject_cast<WlanQtUtilsWlanIap *>(newIap);
    }
    
// ---------------------------------------------------------
// CmmWrapperPrivate::moveIapToInternetSnap()
// ---------------------------------------------------------
//
void CmmWrapperPrivate::moveIapToInternetSnap( TInt32 aIapId )
    {
    TRAPD( err, moveIapToInternetSnapL( aIapId ) );
    if (err != KErrNone)
        {
        // todo: add trace
        }
    }

// ---------------------------------------------------------
// CmmWrapperPrivate::moveIapToInternetSnapL()
// ---------------------------------------------------------
//
void CmmWrapperPrivate::moveIapToInternetSnapL( TInt32 aIapId )
    {
    OstTraceFunctionEntry1( CMMWRAPPERPRIVATE_MOVEIAPTOINTERNETSNAPL_ENTRY, this );
    
    // Read all destination(SNAP) settings into an array
    RArray<TUint32> destinations;
    CleanupClosePushL(destinations);
    iCmManagerExt.AllDestinationsL(destinations);
    RCmDestinationExt destination;
    // Loop through each destination
    for(TInt i = 0; i < destinations.Count(); i++)
        {
        destination = iCmManagerExt.DestinationL(destinations[i]);
        CleanupClosePushL(destination); 
        // Internet destination will always exist in the system.
        // Internet destination will have ESnapPurposeInternet set in its metadata.
        if (destination.MetadataL(CMManager::ESnapMetadataPurpose) == CMManager::ESnapPurposeInternet)
            {
            RCmConnectionMethodExt iap = iCmManagerExt.ConnectionMethodL( aIapId );
            CleanupClosePushL( iap );        
            destination.AddConnectionMethodL( iap );
            destination.UpdateL();
            CleanupStack::PopAndDestroy(); // iap
            }
        CleanupStack::PopAndDestroy(); // destination
        }
    CleanupStack::PopAndDestroy(); // destinations

    OstTraceFunctionExit1( CMMWRAPPERPRIVATE_MOVEIAPTOINTERNETSNAPL_EXIT, this );
    }
    
// ---------------------------------------------------------
// CmmWrapperPrivate::getIapListFromSnap()
// ---------------------------------------------------------
//
int CmmWrapperPrivate::getIapListFromSnap(TUint32 aDestId,
        QList<WlanQtUtilsIap*>& iapList)
    {
    OstTraceFunctionEntry1( CMMWRAPPERPRIVATE_GETIAPLISTFROMSNAP_ENTRY, this );
    
    RCmDestinationExt dest;
    TRAPD(err, dest = iCmManagerExt.DestinationL(aDestId));
    if (err != KErrNone)
        {
        OstTraceFunctionExit1( CMMWRAPPERPRIVATE_GETIAPLISTFROMSNAP_EXIT, this );
        return err;
        }

    // Copy IAPs from the SNAP into iapList.
    int destIapCount = dest.ConnectionMethodCount();
    for (TInt i = 0; i < destIapCount; i++)
        {
        WlanQtUtilsIap* iap = NULL;
        TRAP(err, iap = getIapInfoL(dest.ConnectionMethodL(i)));
        
        if (err == KErrNone)
            {
            iapList.append(iap);
            }
        }

    OstTraceFunctionExit1( DUP1_CMMWRAPPERPRIVATE_GETIAPLISTFROMSNAP_EXIT, this );
    return err;
    }

// ---------------------------------------------------------
// CmmWrapperPrivate::getIapInfoL()
// ---------------------------------------------------------
//
WlanQtUtilsIap *CmmWrapperPrivate::getIapInfoL(
    RCmConnectionMethodExt aConnectionMethod)
    {
    OstTraceFunctionEntry1( CMMWRAPPERPRIVATE_GETIAPINFOL_ENTRY, this );
    
    TUint iapId = aConnectionMethod.GetIntAttributeL(CMManager::ECmIapId);
    
    TUint netId = aConnectionMethod.GetIntAttributeL(CMManager::ECmNetworkId);

    HBufC* iapName = aConnectionMethod.GetStringAttributeL(CMManager::ECmName);

    TUint iapBearerType = aConnectionMethod.GetIntAttributeL(CMManager::ECmBearerType);

    QString name = QString::fromUtf16(iapName->Ptr(), iapName->Length());
    WlanQtUtilsBearerType bearer;

    WlanQtUtilsIap* newIap = NULL;
    if (iapBearerType == KUidPacketDataBearerType)
        {
        bearer = WlanQtUtilsBearerTypeCellular;
        newIap = new WlanQtUtilsIap(iapId, netId, name, bearer);
        }
    else
        {
        TInt secModeFromCmManager =
                aConnectionMethod.GetIntAttributeL(CMManager::EWlanSecurityMode);
        WlanQtUtilsWlanSecMode secMode = cmm2WlanQtUtilsSecModeMap(secModeFromCmManager);
        bearer = WlanQtUtilsBearerTypeWlan;
        //Note ssid is set same as iap name
        WlanQtUtilsWlanIap* wlanIap = 
            new WlanQtUtilsWlanIap(iapId, netId, name, bearer, name, secMode);
        newIap = wlanIap;
        }

    OstTraceFunctionExit1( CMMWRAPPERPRIVATE_GETIAPINFOL_EXIT, this );
    return newIap;
    }

// ---------------------------------------------------------
// CmmWrapperPrivate::cmm2WlanQtUtilsSecModeMap()
// Maps CMManager's WLAN security mode into WlanQtUtilsWlanSecMode.
// CMManager uses sec mode type TConnMonSecurityMode defined
// in rconnmon.h.
// ---------------------------------------------------------
//
WlanQtUtilsWlanSecMode CmmWrapperPrivate::cmm2WlanQtUtilsSecModeMap(int aSecModeFromCmManager)
    {
    WlanQtUtilsWlanSecMode ret;
    switch (aSecModeFromCmManager)
        {
        case CMManager::EWlanSecModeWep:
            ret = WlanQtUtilsWlanSecModeWep;
            break;
        case CMManager::EWlanSecMode802_1x:
            ret = WlanQtUtilsWlanSecMode802_1x;
            break;
        case CMManager::EWlanSecModeWpa:
            ret = WlanQtUtilsWlanSecModeWpa;
            break;
        case CMManager::EWlanSecModeWpa2:
            ret = WlanQtUtilsWlanSecModeWpa2;
            break;
        default:
            ret = WlanQtUtilsWlanSecModeOpen;
            break;
        }
    
    return ret;
    }

// ---------------------------------------------------------
// Maps WlanQtUtilsWlanSecMode into CMManager's WLAN security mode
// (TWlanSecMode, defined in cmpluginwlandef.h).
// ---------------------------------------------------------
//
int CmmWrapperPrivate::wlanQtUtils2CmmSecModeMap(WlanQtUtilsWlanSecMode aWlanQtUtilsSecMode)
    {
    int ret;
    switch (aWlanQtUtilsSecMode)
        {
        case WlanQtUtilsWlanSecModeWep:
            ret = CMManager::EWlanSecModeWep;
            break;
        case WlanQtUtilsWlanSecMode802_1x:
            ret = CMManager::EWlanSecMode802_1x;
            break;
        case WlanQtUtilsWlanSecModeWpa:
            ret = CMManager::EWlanSecModeWpa;
            break;
        case WlanQtUtilsWlanSecModeWpa2:
            ret = CMManager::EWlanSecModeWpa2;
            break;
        default:
            ret = CMManager::EWlanSecModeOpen;
            break;
        }

    return ret;
    }

//end of file

