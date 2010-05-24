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
 *   WLAN Wizard Page: Security mode Selection.
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
#include "wlanwizardpagesecuritymode.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanwizardpagesecuritymodeTraces.h"
#endif

/*!
 * Constructor. Member initialization.
 */
WlanWizardPageSecurityMode::WlanWizardPageSecurityMode(
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
WlanWizardPageSecurityMode::~WlanWizardPageSecurityMode()
{
    delete mLoader;
}

/*!
 * Page initialization. If view is already loaded, does nothing.
 */
HbWidget* WlanWizardPageSecurityMode::initializePage()
{
    OstTrace0( TRACE_NORMAL, WLANWIZARDPAGESECURITYMODE_INITIALIZEPAGE,
        "WlanWizardPageSecurityMode::initializePage" );
    
    if (mWidget==NULL) {
        
        mLoader = new HbDocumentLoader(mWizard->mainWindow());
        
        bool ok;

        mLoader->load( ":/docml/occ_add_wlan_02_03.docml", &ok );
        Q_ASSERT_X(ok, "WlanWizardPageSecurityMode", "Invalid docml file");
              
        // Initialize orientation
        loadDocml( mWizard->mainWindow()->orientation() );

        // Load widgets
        mWidget = qobject_cast<HbWidget*> (mLoader->findWidget("occ_add_wlan_02"));
        Q_ASSERT_X(mWidget != NULL, "WlanWizardPageSecurityMode", "View not found");

        mList = qobject_cast<HbRadioButtonList*> (mLoader->findWidget("list"));
        Q_ASSERT_X(mList != NULL, "WlanWizardPageSecurityMode", "List not found");

        mLabel = qobject_cast<HbLabel*> (mLoader->findWidget("dialog_6"));
        Q_ASSERT_X(mLabel != NULL, "WlanWizardPageSecurityMode", "Label not found");
        
        mLabel->setPlainText(hbTrId("txt_occ_dialog_select_network_security_mode"));

        // Create contents to the security mode radio button list.
        populateSecModeList();

        // Connect orientation signal from the main window to orientation
        // loader.
        ok &= connect( mWizard->mainWindow(),
                 SIGNAL(orientationChanged(Qt::Orientation)),
                 this, SLOT(loadDocml(Qt::Orientation)));
 
        // Connect signal from the radio button list indicating that an item
        // has been selected to validation handler.
        ok &= connect( mList, SIGNAL(itemSelected(int)),
                 this, SLOT(itemSelected(int)));
        
        Q_ASSERT_X(ok, "WlanWizardPageSecurityMode", "slot connection failed");
        
        

    }
    return mWidget;
}

/*!
 * Indicates the validity of the security mode page.
 * @see WlanWizardPage
 */
bool WlanWizardPageSecurityMode::showPage()
{
    return mValid;
}

/*!
 * Sets the page as valid, if any mode is selected.
 * (Initially none is selected.) 
 */
void WlanWizardPageSecurityMode::itemSelected(int /* index */)
{
    OstTrace0( TRACE_BORDER, WLANWIZARDPAGESECURITYMODE_ITEMSELECTED,
        "WlanWizardPageSecurityMode::itemSelected" );
    
    mValid = true;
    mWizard->enableNextButton(mValid);
}

/*!
 * Evaluates and returns the next page id.
 * @see initializePage()
 */
int WlanWizardPageSecurityMode::nextId(bool &removeFromStack) const
{
    removeFromStack = false;
    
    // The configuration is selected from the mSecModes list, which is ordered
    // during the page initialization.
    mWizard->setConfiguration( WlanWizardPrivate::ConfSecurityMode,
        mSecModes.at( mList->selected() ) );

    // The configuration is selected from the mUsePsk list, which is ordered
    // during the page initialization.
    mWizard->setConfiguration( WlanWizardPrivate::ConfUsePsk,
        mUsePsk.at( mList->selected() ) );

    return mPageIds.at( mList->selected() );
}

/*!
 * Loads the document with given orientation.
 */
void WlanWizardPageSecurityMode::loadDocml(Qt::Orientation orientation)
{
    OstTrace1( TRACE_NORMAL, WLANWIZARDPAGESECURITYMODE_LOADDOCML,
        "WlanWizardPageSecurityMode::loadDocml - orientation;orientation=%x",
        ( TUint )( orientation ) );
    
    bool ok;
    if( orientation == Qt::Horizontal ) {
        mLoader->load(":/docml/occ_add_wlan_02_03.docml", "landscape_section", &ok);
        Q_ASSERT_X(ok, "WlanWizardPageSecurityMode", "Landscape section not found");
    }
    else {
        mLoader->load(":/docml/occ_add_wlan_02_03.docml", "portrait_section", &ok);
        Q_ASSERT_X(ok, "WlanWizardPageSecurityMode", "Portrait section not found");
    }
}

/*!
 * Support function that creates the contents of the security mode list. 
 */
void WlanWizardPageSecurityMode::populateSecModeList()
{
    QStringList items;

    mSecModes.clear();
    mPageIds.clear();

    // Create the radio button list to correspond to correct security mode
    // identifiers and page identifiers.
    // Populate the list according to network mode selection.
    addToList(items, hbTrId("txt_occ_list_open"), 
        CMManagerShim::WlanSecModeOpen,
        WlanWizardPage::PageProcessSettings,
        false);

    addToList(items, hbTrId("txt_occ_list_wep_1"),
        CMManagerShim::WlanSecModeWep,
        WlanWizardPageInternal::PageKeyQuery,
        true);

    // In case of Ad-hoc network, exclude wpa, eap and 802.1X modes.
    if(mWizard->configuration(WlanWizardPrivate::ConfNetworkMode).toInt()
        != CMManagerShim::Adhoc) {

        addToList(items, hbTrId("txt_occ_list_wpa_with_password"),
            CMManagerShim::WlanSecModeWpa,
            WlanWizardPageInternal::PageKeyQuery,
            true);

        // TODO: Fix these codes
        addToList(items, hbTrId("txt_occ_list_wpa_with_eap"),
            CMManagerShim::WlanSecModeWpa,
            WlanWizardPage::PageEapStart,
            false);

        addToList(items, hbTrId("txt_occ_list_8021x_1"),
            CMManagerShim::WlanSecMode802_1x,
            WlanWizardPage::PageEapStart,
            false);
    }

    mList->setItems(items);
}

/*!
 * Creates lists for security modes and page id:s so that they can be referred
 * with radio button widget index.
 */
void WlanWizardPageSecurityMode::addToList(QStringList &list,
    const QString &item, int mode, int page, bool psk)
{
    list << item;
    mSecModes.append(mode);
    mPageIds.append(page);
    mUsePsk.append(psk);
}


