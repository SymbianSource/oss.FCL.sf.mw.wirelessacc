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


// System includes

#include <HbGlobal>
#include <HbDataFormModelItem>
#include <HbLineEdit>
#include <HbValidator>
#include <HbMessageBox>
#include <QStringList>
#include <cpsettingformitemdata.h>

// User includes

#include "cpwlansettingsgroupitemdata.h"
#include "wlansettings.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "cpwlansettingsgroupitemdataTraces.h"
#endif

/*!
    \class CpWlanSettingsGroupItemData
    \brief CpWlanSettingsGroupItemData class creates all widgets required for Manage 
    WLAN Settings.
*/

// External function prototypes

// Local constants
/**  Maximum Value for Scan Interval. */
const int KMaximumScanInterval = 30;
/**  Minimum Value for Scan Interval. */
const int KMinimumScanInterval = 0;
/**  To Enable the setting of a widget propery. */
const int KEnableOption = 1;
/**  Maximum widgets allowed for this Item Data. */
const int KMaxWidgets = 4;
/**  Index of Slider widget for Scan Interval. */
const int KSliderIndex = 2;

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor.
*/

CpWlanSettingsGroupItemData::CpWlanSettingsGroupItemData(
        CpItemDataHelper &itemDataHelper) :
    CpSettingFormItemData(HbDataFormModelItem::GroupItem, hbTrId(
            "txt_occ_subhead_wlan_settings")),
            mItemDataHelper(itemDataHelper)
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_CPWLANSETTINGSGROUPITEMDATA_ENTRY, this);
    
    initialise();
    
    createWlanSettingItems();
    
    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_CPWLANSETTINGSGROUPITEMDATA_EXIT, this);
}

/*!
    Destructor.
*/

CpWlanSettingsGroupItemData::~CpWlanSettingsGroupItemData()
{
    OstTraceFunctionEntry1(DUP1_CPWLANSETTINGSGROUPITEMDATA_CPWLANSETTINGSGROUPITEMDATA_ENTRY, this);
    OstTraceFunctionExit1(DUP1_CPWLANSETTINGSGROUPITEMDATA_CPWLANSETTINGSGROUPITEMDATA_EXIT, this);
}

/*!
    Initialises the member variables and reads the values of WLAN Settings.
*/

void CpWlanSettingsGroupItemData::initialise()
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_INITIALISE_ENTRY, this);
    
    mWlanSettings.reset(new WlanSettings);
    
    //Error needs to be handled.
    int error = mWlanSettings->init();
    
    //Error needs to be handled.
    error = mWlanSettings->loadSettings();
    
    mUserDefinedItem = NULL;
    
    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_INITIALISE_EXIT, this);
}

/*!
    Creates the Widgets for displaying WLAN Settings.
*/

void CpWlanSettingsGroupItemData::createWlanSettingItems()
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_CREATEWLANSETTINITEMS_ENTRY, this);
    
    mJoinWlanItem = new CpSettingFormItemData(
        HbDataFormModelItem::ComboBoxItem,
        hbTrId("txt_occ_setlabel_join_wlan_networks"));
    // name the object for test automation purposes
    mJoinWlanItem->setObjectName("switchToWlan");
    
    QStringList joinWlanItems;
    
    joinWlanItems.append(
        hbTrId("txt_occ_setlabel_join_wlan_networks_val_known"));
    
    joinWlanItems.append(
        hbTrId("txt_occ_setlabel_join_wlan_networks_val_manual"));

    mJoinWlanItem->setContentWidgetData("items", joinWlanItems);

    mItemDataHelper.addConnection(
        mJoinWlanItem,
        SIGNAL(currentIndexChanged (int)),
        this,
        SLOT(joinWlanItemChanged (int)));

    mJoinWlanItem->setContentWidgetData(
        "currentIndex",
        mWlanSettings->joinWlanMode());

    this->appendChild(mJoinWlanItem);

    mScanNetworkItem = new CpSettingFormItemData(
        HbDataFormModelItem::ComboBoxItem,
        hbTrId("txt_occ_setlabel_scan_for_networks"));
    // name the object for test automation purposes
    mScanNetworkItem->setObjectName("scanNetworkItem");
    
    QStringList scanNetworkItems;
    
    scanNetworkItems.append(
        hbTrId("txt_occ_setlabel_scan_for_networks_val_automatic"));
    
    scanNetworkItems.append(
        hbTrId("txt_occ_setlabel_scan_for_networks_val_userdefine"));

    mScanNetworkItem->setContentWidgetData("items", scanNetworkItems);

    mItemDataHelper.addConnection(
        mScanNetworkItem,
        SIGNAL(currentIndexChanged (const QString &)),
        this,
        SLOT(scanItemChanged (const QString &)));

    this->appendChild(mScanNetworkItem);

    WlanSettings::ScanNetworkType scanType = mWlanSettings->scanNetworkType();

    mScanNetworkItem->setContentWidgetData("currentIndex", scanType);

    if (scanType == WlanSettings::EScanNetworkUserDefined) {
        createScanIntervalItem();
        this->appendChild(mUserDefinedItem);
    }

    mPowerSavingItem = new CpSettingFormItemData(
        HbDataFormModelItem::ComboBoxItem,
        hbTrId("txt_occ_setlabel_power_saving"));
    // name the object for test automation purposes
    mPowerSavingItem->setObjectName("powerSaving");
    
    QStringList powerSavingItems;
    
    powerSavingItems.append(
        hbTrId("txt_occ_setlabel_power_saving_val_disabled"));
    
    powerSavingItems.append(
        hbTrId("txt_occ_setlabel_power_saving_val_enabled"));

    mPowerSavingItem->setContentWidgetData("items", powerSavingItems);

    mPowerSavingItem->setContentWidgetData(
        "currentIndex",
        mWlanSettings->isPowerSavingEnabled());

    mItemDataHelper.addConnection(
        mPowerSavingItem,
        SIGNAL(currentIndexChanged (int)),
        this,
        SLOT(powerSavingItemChanged (int)));

    this->appendChild(mPowerSavingItem);

    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_CREATEWLANSETTINITEMS_EXIT, this);
}

/*!
    Creates the slider widget for displaying scan interval value.
*/

void CpWlanSettingsGroupItemData::createScanIntervalItem()
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_CREATESCANINTERVALITEM_ENTRY, this);
    
    mUserDefinedItem = new CpSettingFormItemData(
        HbDataFormModelItem::SliderItem,
        hbTrId("txt_occ_setlabel_scan_interval_minutes"));

    mUserDefinedItem->setContentWidgetData("maximum", KMaximumScanInterval);
    mUserDefinedItem->setContentWidgetData("minimum", KMinimumScanInterval);
    mUserDefinedItem->setContentWidgetData("toolTipVisible", KEnableOption);
    mUserDefinedItem->setContentWidgetData("tracking", KEnableOption);

    mItemDataHelper.addConnection(
        mUserDefinedItem,
        SIGNAL(valueChanged (int)),
        this,
        SLOT(scanValueChanged (int)));
    
    mItemDataHelper.addConnection(
        mUserDefinedItem,
        SIGNAL(sliderReleased ()),
        this,
        SLOT(scanSliderReleased ()));
    
    mItemDataHelper.addConnection(
        mUserDefinedItem,
        SIGNAL(sliderPressed ()),
        this,
        SLOT(scanSliderPressed ()));

    mScanInterval = mWlanSettings->scanInterval();

    mUserDefinedItem->setContentWidgetData("value", mScanInterval);
    
    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_CREATESCANINTERVALITEM_EXIT, this);
}

/*!
    Slot for handling pressed singal of scan interval slider widget.
*/

void CpWlanSettingsGroupItemData::scanSliderPressed()
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_SCANSLIDERPRESSED_ENTRY, this);
    
    mUserDefinedItem->setContentWidgetData("text",QString("%1").arg(mScanInterval));
    
    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_SCANSLIDERPRESSED_EXIT, this);
}

/*!
    Slot for handling value changed singal of scan interval slider widget.
*/

void CpWlanSettingsGroupItemData::scanValueChanged(int value)
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_SCANVALUECHANGED_ENTRY, this);
    
    mUserDefinedItem->setContentWidgetData("text",QString("%1").arg(value));
    mScanInterval = value;
    
    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_SCANVALUECHANGED_EXIT, this);
}

/*!
    Slot for handling Released singal of scan interval slider widget.
    Current value will be commited to database.
*/

void CpWlanSettingsGroupItemData::scanSliderReleased()
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_SCANSLIDERRELEASED_ENTRY, this);
    
    mWlanSettings->setWlanScanInterval(mScanInterval);
    
    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_SCANSLIDERRELEASED_EXIT, this);
}

/*!
    Slot for handling item changed singal of Scan for network combo box widget.
*/

void CpWlanSettingsGroupItemData::scanItemChanged(const QString &text)
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_SCANITEMCHANGED_ENTRY, this);
    
    if (hbTrId("txt_occ_setlabel_scan_for_networks_val_userdefine") == text) {
        //In case of PSM mode for device user-defined value is not 
        //allowed,make it automatic forcefully.
        if (mWlanSettings->isPsmEnabled()) {
            //Need to check for dimming of the combobox option.
            mScanNetworkItem->setContentWidgetData("currentIndex",
                    WlanSettings::EScanNetworkAuto);
        }
        else {
            //Add Scan Interval Slider, if it was not added.
            if (this->childCount() < KMaxWidgets) {
                createScanIntervalItem();
                this->insertChild(KSliderIndex,mUserDefinedItem);
                mWlanSettings->setWlanScanInterval(mWlanSettings->scanInterval());
            }
        }
    }
    else {
        //Remove Scan Interval slider, if user selects automatic option
        //for Scan network setting and if slider was added earlier.
        if (KMaxWidgets == this->childCount()) {
            this->removeChild(KSliderIndex);
            mUserDefinedItem = NULL;    //removeChild() will delete the item.
            mWlanSettings->setWlanScanInterval(KWlanSettingsScanNetworkAuto);
        }
    }
    
    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_SCANITEMCHANGED_EXIT, this);
}

/*!
    Slot for handling item changed singal of Power saving combo box widget.
*/

void CpWlanSettingsGroupItemData::powerSavingItemChanged(int index)
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_POWERSAVINGITEMCHANGED_ENTRY, this);
    
    mWlanSettings->setWlanPowerSaving(index);
    
    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_POWERSAVINGITEMCHANGED_EXIT, this);
}

/*!
    Slot for handling item changed singal of Join WLAN networks combo box widget.
*/

void CpWlanSettingsGroupItemData::joinWlanItemChanged(int index)
{
    OstTraceFunctionEntry1(CPWLANSETTINGSGROUPITEMDATA_JOINWLANITEMCHANGED_ENTRY, this);
    
    mWlanSettings->setJoinWlanMode(index);
    
    OstTraceFunctionExit1(CPWLANSETTINGSGROUPITEMDATA_JOINWLANITEMCHANGED_EXIT, this);
}
