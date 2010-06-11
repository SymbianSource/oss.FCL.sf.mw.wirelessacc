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
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanwizardpagenetworkmodeTraces.h"
#endif

/*!
 * Contructor. Member initialization.
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
 */
HbWidget* WlanWizardPageNetworkMode::initializePage()
{
    OstTrace0( TRACE_NORMAL, WLANWIZARDPAGENETWORKMODE_INITIALIZEPAGE,
        "WlanWizardPageNetworkMode::initializePage" );

    if (mWidget == NULL) {
        mLoader = new HbDocumentLoader(mWizard->mainWindow());

        bool ok;

        mLoader->load(":/docml/occ_add_wlan_02_03.docml", &ok);
        Q_ASSERT_X(ok, "WLAN Wizard", "Invalid docml file");

        loadDocml(mWizard->mainWindow()->orientation());

        mWidget = qobject_cast<HbWidget*> (mLoader->findWidget(
            "occ_add_wlan_02"));
        Q_ASSERT_X(mWidget != NULL, "WlanWizardPageNetworkMode", "View not found");

        mList = qobject_cast<HbRadioButtonList*> (mLoader->findWidget("list"));
        Q_ASSERT_X(mList != NULL, "WlanWizardPageNetworkMode", "List not found");

        mLabel = qobject_cast<HbLabel*> (mLoader->findWidget("dialog_6"));
        Q_ASSERT_X(mLabel != NULL, "WlanWizardPageNetworkMode", "Label not found");

        // Connect document loading to main window orientation changes.
        ok &= connect(mWizard->mainWindow(),
            SIGNAL(orientationChanged(Qt::Orientation)), this,
            SLOT(loadDocml(Qt::Orientation)));

        // Connect a function to a radio button selection.
        ok &= connect(mList, SIGNAL(itemSelected(int)), this,
            SLOT(itemSelected(int)));

        Q_ASSERT_X(ok, "WLAN Wizard", "orientationChanged slot connection failed");

        mLabel->setPlainText(hbTrId(
            "txt_occ_dialog_select_network_mode_and_status"));

        QStringList items;

        addToList(items, hbTrId("txt_occ_dblist_val_infrastructure_public"),
            CMManagerShim::Infra, false);

        addToList(items, hbTrId("txt_occ_list_infrastructure_hidden"),
            CMManagerShim::Infra, true);

        addToList(items, hbTrId("txt_occ_list_adhoc_1"), CMManagerShim::Adhoc,
            false);

        mList->setItems(items);

    }
    return mWidget;
}

/*!
 * When moving to next page, write user selections to the wizard and return
 * Security mode query page id.
 */
int WlanWizardPageNetworkMode::nextId(bool &removeFromStack) const
{
    removeFromStack = false;
    // TODO: wk18: proper code
    mWizard->setConfiguration(WlanWizardPrivate::ConfNetworkMode,
        mNetworkModes.at(mList->selected()));
    
    mWizard->setConfiguration(WlanWizardPrivate::ConfHiddenWlan,
        mIsHidden.at(mList->selected()));

    return WlanWizardPageInternal::PageNetworkSecurity;
}

/*!
 * Indicates the validity of the network mode page.
 * @see WlanWizardPage
 */
bool WlanWizardPageNetworkMode::showPage()
{
    return mValid;
}

/*!
 * Sets the page as valid, if any mode is selected.
 * (Initially none is selected.) 
 */
void WlanWizardPageNetworkMode::itemSelected(int /* index */)
{
    OstTrace0( TRACE_BORDER, WLANWIZARDPAGENETWORKMODE_ITEMSELECTED,
        "WlanWizardPageNetworkMode::itemSelected" );

    mValid = true;
    mWizard->enableNextButton(mValid);
}

/*!
 * Loads the ui layout file with current orientation.
 */
void WlanWizardPageNetworkMode::loadDocml(Qt::Orientation orientation)
{
    OstTrace1( TRACE_NORMAL, WLANWIZARDPAGENETWORKMODE_LOADDOCML,
        "WlanWizardPageNetworkMode::loadDocml - Orientation;orientation=%x",
        ( TUint )( orientation ) );

    bool ok;
    if (orientation == Qt::Horizontal) {
        mLoader->load(":/docml/occ_add_wlan_02_03.docml", "landscape_section",
            &ok);
        Q_ASSERT_X(ok, "WLAN Wizard", "Landscape section not found");
    }
    else {
        mLoader->load(":/docml/occ_add_wlan_02_03.docml", "portrait_section",
            &ok);
        Q_ASSERT_X(ok, "WLAN Wizard", "Portrait section not found");
    }
}

/*!
 * Mode and visibility lists are populated to the same order as the radio
 * button list.
 */
void WlanWizardPageNetworkMode::addToList(QStringList &list,
    const QString &item, int mode, bool isHidden)
{
    list << item;
    mNetworkModes.append(mode);
    mIsHidden.append(isHidden);
}

