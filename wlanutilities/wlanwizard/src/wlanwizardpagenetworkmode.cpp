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
 *   WLAN Wizard Page: Network mode Selection.
 *
 */

// System includes
#include <HbDocumentLoader>
#include <HbWidget>
#include <HbRadioButtonList>
#include <HbMainWindow>
#include <HbLabel>
#include <cmmanagerdefines_shim.h>

// User includes
#include "wlanwizard_p.h"
#include "wlanwizard.h"
#include "wlanwizardpagenetworkmode.h"
#include "wlanwizardscanlist.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanwizardpagenetworkmodeTraces.h"
#endif

// Local constants

/*!
 * Constructor method for the network mode view object
 * @param parent [in] pointer to parent object.
 */
WlanWizardPageNetworkMode::WlanWizardPageNetworkMode(
    WlanWizardPrivate* parent) :
    WlanWizardPageInternal(parent),
    mWidget(NULL),
    mList(NULL),
    mLabel(NULL),
    mLoader(NULL),
    mValid(false)
{
}

/*!
 * Destructor. Loader widget is deleted.
 * All document widgets are deleted by wlanwizard_p destructor. 
 */
WlanWizardPageNetworkMode::~WlanWizardPageNetworkMode()
{
    delete mLoader;
}

/*!
 * Page initialization. If view is already loaded, does nothing.
 * Inherited from WlanWizardPage.
 */
HbWidget* WlanWizardPageNetworkMode::initializePage()
{
    OstTrace0( TRACE_NORMAL, WLANWIZARDPAGENETWORKMODE_INITIALIZEPAGE,
        "WlanWizardPageNetworkMode::initializePage" );

    if (mWidget == NULL) {
        mLoader = new HbDocumentLoader(mWizard->mainWindow());

        bool ok;

        mLoader->load(":/docml/occ_add_wlan_02_03.docml", &ok);
        Q_ASSERT(ok);

        loadDocmlSection(mWizard->mainWindow()->orientation());

        mWidget = qobject_cast<HbWidget*> (mLoader->findWidget(
            "occ_add_wlan_02"));
        Q_ASSERT(mWidget != NULL);

        mList = qobject_cast<HbRadioButtonList*> (mLoader->findWidget("list"));
        Q_ASSERT(mList != NULL);

        mLabel = qobject_cast<HbLabel*> (mLoader->findWidget("dialog_6"));
        Q_ASSERT(mLabel != NULL);

        // Connect document loading to main window orientation changes.
        ok = connect(mWizard->mainWindow(),
            SIGNAL(orientationChanged(Qt::Orientation)), this,
            SLOT(loadDocmlSection(Qt::Orientation)));
        Q_ASSERT(ok);
        
        // Connect a function to a radio button selection.
        ok = connect(mList, SIGNAL(itemSelected(int)), this,
            SLOT(itemSelected()));
        Q_ASSERT(ok);

        mLabel->setPlainText(hbTrId(
            "txt_occ_dialog_select_network_mode_and_status"));
    }
    
    QStringList items;
    
    populateRadioButtonList(items);
    
    mList->setItems(items);
    
    return mWidget;
}

/*!
 * Evaluates the network mode selection and sets the configuration in
 * the wlanwizard.
 * @param [out] removeFromStack always returns false.
 * @return WlanWizardPageSecurityMode page id.
 */
int WlanWizardPageNetworkMode::nextId(bool &removeFromStack) const
{
    removeFromStack = false;
    WlanNetworkSetting setting;
    setting.mode = mNetworkModes.at(mList->selected());
    setting.hidden = mIsHidden.at(mList->selected());
    
    // TODO: Still no wps handling.
    setting.wpsSupported = false;
    
    mWizard->setConfiguration(WlanWizardPrivate::ConfNetworkMode,
        setting.mode);
    
    mWizard->setConfiguration(WlanWizardPrivate::ConfHiddenWlan,
        setting.hidden);

    if (mWizard->configurationExists(WlanWizardPrivate::ConfAvailableNetworkOptions)) {
        return selectNextPage(setting);
    }
    
    return WlanWizardPageInternal::PageNetworkSecurity;
}

/*!
 * This method is overrides the default implementation from WlanWizardPage.
 * It indicates whether the Next-button should be enabled or not.
 * @return mValid, which is true if a radio button has been selected.
 */
bool WlanWizardPageNetworkMode::showPage()
{
    return mValid;
}

/*!
 * This function is invoked when user selects a mode from the radio button list.
 * (HbRadioButtonList's itemSelected-signal)
 */
void WlanWizardPageNetworkMode::itemSelected()
{
    OstTrace0( TRACE_BORDER, WLANWIZARDPAGENETWORKMODE_ITEMSELECTED,
        "WlanWizardPageNetworkMode::itemSelected" );

    mValid = true;
    mWizard->enableNextButton(mValid);
}

/*!
 * Loads the document orientation information from occ_add_wlan_02_03.docml.
 * This is called each time phone orientation changes.
 * @param [in] orientation indicates whether the phone is in portrait or
 * landscape mode.
 */
void WlanWizardPageNetworkMode::loadDocmlSection(Qt::Orientation orientation)
{
    OstTrace1( TRACE_NORMAL, WLANWIZARDPAGENETWORKMODE_LOADDOCML,
        "WlanWizardPageNetworkMode::loadDocml - Orientation;orientation=%x",
        ( TUint )( orientation ) );

    WlanWizardPageInternal::loadDocmlSection(
        mLoader,
        orientation,
        ":/docml/occ_add_wlan_02_03.docml", 
        "portrait_section",
        "landscape_section");
}

/*!
 * A support function to map the radio button list to a generic network
 * mode list. This enables the changing of button order without it
 * affecting the entire class.
 * In case previous scan has revealed multiple results with the same ssid, only
 * available network mode options are shown in the radio button list.
 * @param [out] list is the list of captions used for the radio buttons.
 * @param [in] item is the caption to be added.
 * @param [in] mode is the network mode to be added.
 * @param [in] isHidden is the visibility status of the network.
 */
void WlanWizardPageNetworkMode::addToList(QStringList &list,
    const QString &item, int mode, bool isHidden)
{
        
    if (mWizard->configurationExists(WlanWizardHelper::ConfAvailableNetworkOptions)) {
        WlanWizardScanList networkOptions = mWizard->configuration(
            WlanWizardHelper::ConfAvailableNetworkOptions).value<WlanWizardScanList> ();

        QList<WlanNetworkSetting> modes = networkOptions.getNetModes();

        for (int i = 0; i < modes.size(); i++) {
            if (modes[i].mode == mode && modes[i].hidden == isHidden) {
                list << item;
                mNetworkModes.append(mode);
                mIsHidden.append(isHidden);
            }
        }
    }
    else {  
        list << item;
        mNetworkModes.append(mode);
        mIsHidden.append(isHidden);
    }
}

/*!
 * This function checks the sets up the radio button list for user input.
 * @param [out] list is the list of captions used for the radio buttons.
 */
void WlanWizardPageNetworkMode::populateRadioButtonList(QStringList &list)
{
    list.clear();
    mNetworkModes.clear();
    mIsHidden.clear();

    // A list is created. Since there is no practical way of knowing whether
    // the new contents are different from the previous contents (if there
    // even were any in the first place) the validity is always reset.
    mValid = false;
        
    addToList(list, hbTrId("txt_occ_dblist_val_infrastructure_public"),
        CMManagerShim::Infra, false);

    addToList(list, hbTrId("txt_occ_list_infrastructure_hidden"),
        CMManagerShim::Infra, true);

    addToList(list, hbTrId("txt_occ_list_adhoc_1"), CMManagerShim::Adhoc,
        false);
}

/*!
 * This function selects the next page. Security mode may be skipped in case
 * scan results exist and only single option remains.
 * @param [in] setting indicates the selected network mode.
 * @return Security mode query page identifier unless it can be skipped.
 */
int WlanWizardPageNetworkMode::selectNextPage(const WlanNetworkSetting &setting) const
{
    WlanWizardScanList networkOptions = mWizard->configuration(
        WlanWizardHelper::ConfAvailableNetworkOptions).value<WlanWizardScanList>();
    
    if (networkOptions.secModes(setting) == SingleResult) {
        WlanSecuritySetting secMode = networkOptions.getSecMode(setting);
        
        mWizard->setConfiguration(WlanWizardHelper::ConfSecurityMode, secMode.mode);
        mWizard->setConfiguration(WlanWizardHelper::ConfUsePsk, secMode.usePsk);
        return secMode.nextPageId;
    }
    
    return WlanWizardPageInternal::PageNetworkSecurity;
}

