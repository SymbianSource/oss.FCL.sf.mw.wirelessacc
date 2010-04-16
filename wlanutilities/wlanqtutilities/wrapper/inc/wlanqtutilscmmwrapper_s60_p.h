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
 */

#ifndef WLANQTUTILSCMMWRAPPER_S60_P_H_
#define WLANQTUTILSCMMWRAPPER_S60_P_H_

// INCLUDES
#include <cmmanagerext.h>
#include "wlanqtutilscommon.h"

QT_BEGIN_NAMESPACE

//definitions taken from cmpluginwlandef.h and cmpluginpacketdatadef.h
const TUint KUidPacketDataBearerType = 0x10207378;

class WlanQtUtilsIap;
class WlanQtUtilsWlanIap;

// CLASS DECLARATION

/**
 * CmmWrapperPrivate class.
 */
class CmmWrapperPrivate
    {

public:

    /**
     * Default constructor
     */
    CmmWrapperPrivate(CmmWrapper *wrapper);

    /**
     * Destructor
     */
    ~CmmWrapperPrivate();

public:
    // New functions    

    /**
     * Fetches IAPs from CMM
     * 
     * @iapList, list of IAPs
     * @return error code
     */
    int fetchIaps(QList<WlanQtUtilsIap*> &iapList);

    /**
     * Creates a new WLAN IAP through CMM
     * 
     * @aWlanAp WLAN parameters to store in IAP
     * @return Created IAP, NULL in error cases
     */
    WlanQtUtilsWlanIap *createWlanIap(const WlanQtUtilsWlanAp *aWlanAp);
    
    /**
     * Move IAP to the Internet SNAP
     * 
     * @param[in] iapId Which IAP to move
     */
    void moveIapToInternetSnap(TInt32 aIapId);
        
private:

    /**
     * Fetches IAPs from CMM, leaves in error cases
     * 
     * @iapList, list of IAPs
     */
    void fetchIapsL(QList<WlanQtUtilsIap*> &iapList);
    
    /**
     * Creates a new WLAN IAP through CMM, leaves in error cases
     * 
     * @aWlanAp WLAN parameters to store in IAP
     * @return Created IAP
     */
    WlanQtUtilsWlanIap *createWlanIapL(const WlanQtUtilsWlanAp *aWlanAp);

    /**
     * Move IAP to the Internet SNAP, leaves in error cases
     * 
     * @param[in] iapId Which IAP to move
     */
    void moveIapToInternetSnapL(TInt32 aIapId);
        
    /**
     * Fetches IAPs of the given SNAP from CMM
     * 
     * @param[in] aSnapId ID of the SNAP whose IAPS are requested
     * @param[out] aSnapsIapList Where to store IAPs
     * @return Error code
     */    
    int getIapListFromSnap(
        TUint32 aSnapId,
        QList<WlanQtUtilsIap*>& aSnapsIapList);
    
    /**
     * Fetches IAP information of the given ConnectionMethod from CMM
     * 
     * @param[in] aConnectionMethod Where to read info
     * @return Created IAP
     */    
    WlanQtUtilsIap *getIapInfoL(
        RCmConnectionMethodExt aConnectionMethod);
    
    /**
     * Maps CMManager's WLAN security mode into WlanQtUtilsWlanSecMode.
     * 
     * @aSecModeFromCmManager security mode read from CMManager
     * @return: Mapped security mode to be used in Wlan Qt Utilities.
     */
    WlanQtUtilsWlanSecMode cmm2WlanQtUtilsSecModeMap(int aSecModeFromCmManager);

    /**
     * Maps WlanQtUtilsWlanSecMode into CMManager's WLAN security mode.
     * 
     * @aWlanQtUtilsSecMode: Wlan Qt Utilities' WLAN security mode 
     * @return: Mapped CMManager's WLAN security mode.
     */
    int wlanQtUtils2CmmSecModeMap(WlanQtUtilsWlanSecMode aWlanQtUtilsSecMode);

private:
    // Data

    /**
     * q_ptr pointer to CmmWrapper
     * Owned by CmmWrapperPrivate object, instantiated in
     * constructor.
     */
    CmmWrapper *q_ptr;

    RCmManagerExt iCmManagerExt;

    };

QT_END_NAMESPACE

#endif /* WLANQTUTILSCMMWRAPPER_S60_P_H_ */

// End of file
