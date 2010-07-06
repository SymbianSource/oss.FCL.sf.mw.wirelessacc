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
 *   Wlan Wizard Scan List: Hierarchical list of scan results that can be
 *   utilized to limit available selection options in manual wlan wizard.
 *
 */

// System includes
#include <HbGlobal>
#include <QString>
#include <cmmanagerdefines_shim.h>
#include <wlanqtutilsap.h>

// User includes
#include "wlanwizardscanlist.h"
#include "wlanwizardpageinternal.h"

/*!
 * Constructor for WlanWizardScanList object.
 */
WlanWizardScanList::WlanWizardScanList()
{

}

/*!
 * Copy Constructor for WlanWizardScanList object.
 * @param [in] scanList is the object to be copied.
 */
WlanWizardScanList::WlanWizardScanList(const WlanWizardScanList &scanList):
    mOpenOptions(scanList.mOpenOptions)
{

}

/*!
 * Destructor for WlanWizardScanList object.
 */
WlanWizardScanList::~WlanWizardScanList()
{

}

/*!
 * This function builds a hash list of scan results. Key value in a hash item
 * is the network mode and the associated value is a list of security modes.
 * In other words, it is a list of network modes where each network mode points
 * to one or more security modes.
 * @param [in] results contains a list of scan results.
 */
void WlanWizardScanList::build(const QList<WlanScanResult> &results)
{
    for (int i = 0; i < results.size(); i++) {
        WlanNetworkSetting netMode;
        WlanSecuritySetting secMode;
        WlanScanResult item = results.at(i);
        
        netMode.wpsSupported = item.scanResult->value(WlanQtUtilsAp::ConfIdWpsSupported).toBool();
        netMode.mode = item.scanResult->value(WlanQtUtilsAp::ConfIdConnectionMode).toInt();
        netMode.hidden = (netMode.mode == CMManagerShim::Adhoc) ? false : item.networkHidden;
        
        secMode.mode = item.scanResult->value(WlanQtUtilsAp::ConfIdSecurityMode).toInt();
        secMode.usePsk = (secMode.mode == CMManagerShim::WlanSecModeWep) ? true
            : item.scanResult->value(WlanQtUtilsAp::ConfIdWpaPskUse).toBool();

        switch (secMode.mode) {
        case CMManagerShim::WlanSecMode802_1x:
            secMode.nextPageId = WlanWizardPage::PageEapStart;
            break;
            
        case CMManagerShim::WlanSecModeWep:
            secMode.nextPageId = WlanWizardPageInternal::PageKeyQuery;
            break;
            
        case CMManagerShim::WlanSecModeWpa:
        case CMManagerShim::WlanSecModeWpa2:
            if (secMode.usePsk) {
                secMode.nextPageId = WlanWizardPageInternal::PageKeyQuery;
            }
            else {
                secMode.nextPageId = WlanWizardPage::PageEapStart;
            }
            break;

        default:
            Q_ASSERT(CMManagerShim::WlanSecModeOpen == secMode.mode ||
                CMManagerShim::WlanSecModeWapi == secMode.mode);
            secMode.nextPageId = WlanWizardPage::PageProcessSettings;
            break;
            
        }
        
        // prevent duplicate settings.
        if (!mOpenOptions.contains(netMode)) {
            QList<WlanSecuritySetting> secList;
            secList.append(secMode);
            mOpenOptions.insert(netMode, secList);
        }
        else if (!(mOpenOptions.find(netMode)->contains(secMode))) {
            mOpenOptions.find(netMode)->append(secMode);  
        }
        // else an identical key-value pair is already in the list -> not added.
    }
}

/*!
 * This function returns a security mode setting at the requested location.
 * This function does not do any validity checks for the lists, so the user
 * needs to use netModes and secModes functions to prevent over-indexing.
 * @param [in] netMode key value for the hash list.
 * @param [in] index list index pointing to the security mode item.
 * @return security setting item reference.
 */
const WlanSecuritySetting &WlanWizardScanList::getSecMode(
    WlanNetworkSetting netMode,
    int index) const
{
    return mOpenOptions.value(netMode).at(index);
}

/*!
 * The number of key values available in the hash list.
 * @return the number of keys.
 */
int WlanWizardScanList::netModes() const
{
    return mOpenOptions.size();
}

/*!
 * The size of the security mode list associated to a network mode.
 * @param [in] netMode key value for the hash list.
 * @return the size of the list associated with the key.
 */
int WlanWizardScanList::secModes(WlanNetworkSetting netMode) const
{
    return mOpenOptions.find(netMode)->size();
}

/*!
 * This function returns the list of keys in the hash.
 * @return a list of keys.
 */
QList<WlanNetworkSetting> WlanWizardScanList::getNetModes() const
{
    return mOpenOptions.keys();
}

