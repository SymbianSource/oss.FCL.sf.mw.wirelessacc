/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

#ifndef WLANSETTINGSGROUPITEMDATA_H_
#define WLANSETTINGSGROUPITEMDATA_H_

// System includes

#include <QObject>
#include <QScopedPointer>
#include <cpsettingformitemdata.h>
#include <cpitemdatahelper.h>

// User includes

// Forward declarations

class WlanSettings;

// External data types

// Constants

// Class declaration

class CpWlanSettingsGroupItemData: public CpSettingFormItemData
{
    Q_OBJECT
    
public:
    explicit CpWlanSettingsGroupItemData(CpItemDataHelper &itemDataHelper);

    virtual ~CpWlanSettingsGroupItemData();
    
private slots:
    void scanItemChanged(const QString &text);
    
    void powerSavingItemChanged(int index);
    
    void joinWlanItemChanged(int index);
    
    void scanSliderPressed();
    
    void scanValueChanged(int value);
    
    void scanSliderReleased();
    
private:
    
    Q_DISABLE_COPY(CpWlanSettingsGroupItemData)
    
    void createScanIntervalItem();
    
    void initialise();
    
    void createWlanSettingItems();
    
private:    // data
    
    CpSettingFormItemData *mJoinWlanItem;
    CpSettingFormItemData *mScanNetworkItem;
    CpSettingFormItemData *mPowerSavingItem;
    CpSettingFormItemData *mUserDefinedItem;
    
    CpItemDataHelper &mItemDataHelper;
    
    int mScanInterval;
    
    //WlanSettings *mWlanSettings;
    QScopedPointer<WlanSettings> mWlanSettings;
};

#endif /* WLANSETTINGSGROUPITEMDATA_H_ */
