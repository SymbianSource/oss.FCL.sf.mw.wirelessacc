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


#include "context_wlanqtutils.h"
#include "wlanqtutils.h"
#include "wlanqtutilsap.h"

WlanQtUtilsContext::WlanQtUtilsContext(WlanQtUtils *utils )
: mUtils(utils)
{
    
    
}

WlanQtUtilsContext::~WlanQtUtilsContext()
{
    
}
  
void WlanQtUtilsContext::setScanWlanApsResult(
    QList<QSharedPointer<WlanQtUtilsAp> > result )
{
    mUtils->mScanWlanAps = result;
}

void WlanQtUtilsContext::setScanWlanDirectResult( QString ssid,
    QList<QSharedPointer<WlanQtUtilsAp> > result )
{
    mUtils->mScanWlanDirectSsid = ssid;
    mUtils->mScanWlanDirect = result;
}

void WlanQtUtilsContext::setCreateWlanIapResult(int iapId)
{
    mUtils->mCreateWlanIapReturn = iapId;
}
 
void WlanQtUtilsContext::setUpdateWlanIapResult(bool returnValue)
{
    mUtils->mUpdateWlanIapReturn = returnValue;
}
        
void WlanQtUtilsContext::setConnectionSuccessed(bool value)
{
    mUtils->mConnectionSuccess = value;
}
    
void WlanQtUtilsContext::setSignalWlanNetworkOpened(int iapId)
{
    mUtils->mWlanNetworkOpenedIapId = iapId;
}
 
void WlanQtUtilsContext::setSignalWlanNetworkClosed(int iapId, int reason)
{
    mUtils->mWlanNetworkClosedIapId = iapId;
    mUtils->mWlanNetworkClosedReason = (bool)reason;
}
#ifdef ICT_RESULT_ENUM
void WlanQtUtilsContext::setSignalIctResult(int iapId, WlanLoginIctsResultType result)
#else
void WlanQtUtilsContext::setSignalIctResult(int iapId, bool result)
#endif
{
    mUtils->mIctResultIapId = iapId;
    mUtils->mIctResultResults = result;
}
    
QStringList WlanQtUtilsContext::calledMethods()
{
    QStringList methods = mUtils->mCalledMethods;
    mUtils->mCalledMethods.clear();
    return methods;
}
    
bool WlanQtUtilsContext::verifyWlanIapSettings(WlanQtUtilsAp &ref)
{
    bool ret = true;
    if (!mUtils->mWlanAp){
        qWarning("WlanQtUtilsContext::verifyWlanIapSettings: iap not created");
        ret = false;
    }
    
    if (!(*(mUtils->mWlanAp) == ref)){
        qWarning("WlanQtUtilsContext::verifyWlanIapSettings: settings does not match");
        ret = false;
    }
    return ret;
}
