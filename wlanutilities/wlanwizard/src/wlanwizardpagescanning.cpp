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
 * WLAN Wizard Page: Scan processing.
 */

// System includes
#include <HbMainWindow>
#include <HbDocumentLoader>
#include <HbWidget>
#include <HbLabel>
#include <HbProgressBar>
#include <QDebug>
#include <cmmanagerdefines_shim.h>
#include <wlanqtutils.h>
#include <wlanqtutilsap.h>

// User includes
#include "wlanwizard_p.h"
#include "wlanwizardpagescanning.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanwizardpagescanningTraces.h"
#endif

/*!
 * Constructor. Member initialization.
 */
WlanWizardPageScanning::WlanWizardPageScanning(WlanWizardPrivate* parent) :
    WlanWizardPageInternal(parent),
    mWidget(NULL),
    mLabel(NULL),
    mBar(NULL),
    mLoader(NULL),
    mNextPageId(WlanWizardPage::PageNone),
    mScanResultsAvailable(false)
{
    WlanQtUtils* utils = mWizard->wlanQtUtils();

    // Connect normal scan completion signal from wlanQtUtils to result
    // handler. Connect here instead of initializePage, since this signal may
    // need to be caught event if the window is not active.
    bool ok = connect(utils, SIGNAL(wlanScanApReady()), this,
        SLOT(wlanScanResultPreCheck()));
}

/*!
 * Destructor. Loader widget is deleted.
 * All document widgets are deleted by wlanwizard_p destructor.
 */
WlanWizardPageScanning::~WlanWizardPageScanning()
{
    delete mLoader;
}

/*!
 * Page initialization. If view is already loaded, does nothing.
 */
HbWidget* WlanWizardPageScanning::initializePage()
{
    OstTrace0( TRACE_NORMAL, WLANWIZARDPAGESCANNING_INITIALIZEPAGE,
        "WlanWizardPageScanning::initializePage" );

    // Next page id is reset with each initialization.    
    mNextPageId = WlanWizardPage::PageNone;

    if (mWidget == NULL) {

        mLoader = new HbDocumentLoader(mWizard->mainWindow());

        bool ok;

        mLoader->load(":/docml/occ_add_wlan_06.docml", &ok);
        Q_ASSERT_X(ok, "WlanWizardPageScanning", "Invalid docml file");

        // Initialize orientation
        loadDocml(mWizard->mainWindow()->orientation());

        // Load widgets
        mWidget = qobject_cast<HbWidget*> (mLoader->findWidget(
            "occ_add_wlan_06"));
        Q_ASSERT_X(mWidget != NULL, "WlanWizardPageScanning", "View not found");

        mLabel = qobject_cast<HbLabel*> (mLoader->findWidget("dialog"));
        Q_ASSERT_X(mLabel != NULL, "WlanWizardPageScanning",
            "textLabel not found");

        mBar = qobject_cast<HbProgressBar*> (mLoader->findWidget("progressBar"));
        Q_ASSERT_X(mBar != NULL, "WlanWizardPageScanning",
            "progressBar not found");

        WlanQtUtils* utils = mWizard->wlanQtUtils();

        // Connect orientation signal from the main window to orientation
        // loader.
        ok &= connect(mWizard->mainWindow(),
            SIGNAL(orientationChanged(Qt::Orientation)), this,
            SLOT(loadDocml(Qt::Orientation)));

        // Connect direct scan completion signal from wlanQtUtils to result
        // handler.
        ok &= connect(utils, SIGNAL(wlanScanDirectReady()), this,
            SLOT(wlanScanDirectReady()));

        Q_ASSERT(ok);

    }

    mLabel->setPlainText(hbTrId("txt_occ_dialog_searching").arg(
        mWizard->configuration(WlanWizardPrivate::ConfSsid).toString()));

    return mWidget;
}

/*!
 * Sends the user to the next page.
 */
int WlanWizardPageScanning::nextId(bool &removeFromStack) const
{
    removeFromStack = true;

    return mNextPageId;
}

/*!
 * Always false. Moving to next page is not done manually.
 * @see WlanWizardPage
 */
bool WlanWizardPageScanning::showPage()
{
    return false;
}

/*!
 * Loads the document orientation-specific parameters.
 */
void WlanWizardPageScanning::loadDocml(Qt::Orientation orientation)
{
    OstTrace1( TRACE_NORMAL, WLANWIZARDPAGESCANNING_LOADDOCML,
        "WlanWizardPageScanning::loadDocml - orientation ;orientation=%x",
        ( TUint )( orientation ) );

    bool ok;
    // Then load the orientation specific section
    if (orientation == Qt::Horizontal) {
        mLoader->load(":/docml/occ_add_wlan_06.docml", "landscape_section", &ok);
        Q_ASSERT(ok);
    } else {
        Q_ASSERT(orientation == Qt::Vertical);
        mLoader->load(":/docml/occ_add_wlan_06.docml", "portrait_section", &ok);
        Q_ASSERT(ok);
    }
}

void WlanWizardPageScanning::wlanScanResultPreCheck()
{
    if (!mWidget) {
        OstTrace1( TRACE_BORDER, WLANWIZARDPAGESCANNING_WLANSCANRESULTPRECHECK,
            "WlanWizardPageScanning::wlanScanResultPreCheck no widget;this=%x",
            this );

        mScanResultsAvailable = true;
    }
    else {
        if (mWizard->isCurrentPage(mWidget)) {
            OstTrace0( TRACE_BORDER,
                DUP1_WLANWIZARDPAGESCANNING_WLANSCANRESULTPRECHECK,
                "WlanWizardPageScanning::wlanScanResultPreCheck go to results");

            wlanScanApReady();
        }
        else {
            OstTrace0( TRACE_BORDER,
                DUP2_WLANWIZARDPAGESCANNING_WLANSCANRESULTPRECHECK,
                "WlanWizardPageScanning::wlanScanResultPreCheck"
                " not current widget" );

            mScanResultsAvailable = true;
        }
    }
}

/*!
 * Public networks did not reveal matches. Direct scan has been performed to
 * find out if the network is hidden but in range. If network is found, proceed
 * to next page according to scan results.
 * If nothing is found, proceed to manual network 
 * mode selection page.
 */
void WlanWizardPageScanning::wlanScanDirectReady()
{
    OstTrace0( TRACE_NORMAL, WLANWIZARDPAGESCANNING_WLANSCANDIRECTREADY,
        "WlanWizardPageScanning::wlanScanDirectReady" );

    WlanQtUtils* utils = mWizard->wlanQtUtils();

    Q_ASSERT(utils);

    QList<QSharedPointer<WlanQtUtilsAp> > wlanApList;

    utils->availableWlanAps(wlanApList);

    qDebug("WlanWizardPageScanning::wlanScanDirectReady - results read");

    QSharedPointer<WlanQtUtilsAp> item;

    qDebug("%d APs found", wlanApList.size());
    if(wlanApList.isEmpty()) {
        qDebug("Match not found. Go to manual settings.");
        mNextPageId = WlanWizardPageInternal::PageNetworkMode;
    }
    else {
        int strongestSignal = 0;
        int strongestIndex = 0;

        for (int i = 0; i < wlanApList.size(); i++) {
            // All direct scan results are matches. No need to compare ssids.
            // In case there are multiple matches to direct scan, use the
            // strongest signal on the result list.
            item = wlanApList.at(i);
            int str = item->value(WlanQtUtilsAp::ConfIdSignalStrength).toInt();
            if (str > strongestSignal) {
                qDebug("Strongest signal %d at %d", str, i);
                strongestSignal = str;
                strongestIndex = i;
            }
        }

        item = wlanApList.at(strongestIndex);
        qDebug("Select AP at %d", strongestIndex);
        mNextPageId = mWizard->getNextPageId(
            item->value(WlanQtUtilsAp::ConfIdSsid).toString(),
            item->value(WlanQtUtilsAp::ConfIdConnectionMode).toInt(),
            item->value(WlanQtUtilsAp::ConfIdSecurityMode).toInt(),
            item->value(WlanQtUtilsAp::ConfIdWpaPskUse).toBool(),
            true, // hidden
            item->value(WlanQtUtilsAp::ConfIdWpsSupported).toBool());
    }

    mWizard->nextPage();

}

/*!
 * All wlan channels have been scanned for public networks. Now process the
 * results. If a network is found which matches the ssid set by the user,
 * proceed to next page according to the network attributes.
 * If no ssid match is found, proceed to a direct scan.
 */
void WlanWizardPageScanning::wlanScanApReady()
{
    OstTrace0( TRACE_NORMAL, WLANWIZARDPAGESCANNING_WLANSCANAPREADY,
        "WlanWizardPageScanning::wlanScanApReady" );

    WlanQtUtils* utils = mWizard->wlanQtUtils();
    Q_ASSERT(utils);
    
    QList<QSharedPointer<WlanQtUtilsAp> > wlanApList;
    
    // Fetch the list of scan results.
    utils->availableWlanAps(wlanApList);

    qDebug("WlanWizardPageScanning::wlanScanApReady - results read");

    QString ssid =
        mWizard->configuration(WlanWizardPrivate::ConfSsid).toString();
    qDebug() << "SSID scanned by user " << ssid;

    bool matchFound = false;
    int strongestSignal = -1;
    int strongestIndex = 0;
    QSharedPointer<WlanQtUtilsAp> item;
        
    qDebug("%d APs found", wlanApList.size());

    for (int i = 0; i < wlanApList.size(); i++) {
        // In case there are multiple matches to direct scan, use the
        // strongest signal on the result list.
        item = wlanApList.at(i);
        if (item->value(WlanQtUtilsAp::ConfIdSsid).toString() == ssid) {
            qDebug("Match found");
            matchFound = true;

            int str = item->value(WlanQtUtilsAp::ConfIdSignalStrength).toInt();
            if (str > strongestSignal) {
                qDebug("Strongest signal %d at %d", str, i);
                strongestSignal = str;
                strongestIndex = i;
            }
        }
    }

    if (matchFound) {
        qDebug("Select AP at %d", strongestIndex);
        item = wlanApList.at(strongestIndex);
        mNextPageId = mWizard->getNextPageId(
            item->value(WlanQtUtilsAp::ConfIdSsid).toString(),
            item->value(WlanQtUtilsAp::ConfIdConnectionMode).toInt(),
            item->value(WlanQtUtilsAp::ConfIdSecurityMode).toInt(),
            item->value(WlanQtUtilsAp::ConfIdWpaPskUse).toBool(),
            false, // public
            item->value(WlanQtUtilsAp::ConfIdWpsSupported).toBool());
        // The ssid indicated by the user was found. Proceed to next page.
        mWizard->nextPage();
    }
    else {
        qDebug("Match not found. Perform direct scan.");
        utils->scanWlanDirect(ssid);
    }

}

/*!
 * Overloaded function from WlanWizardPage. Indicates that this window requires
 * post-initialization start operations.
 */
bool WlanWizardPageScanning::requiresStartOperation()
{
    return true;
}

/*!
 * Wlan scan is performed as a "post-initialization" which is executed in a
 * separate scheduler loop.
 */
void WlanWizardPageScanning::startOperation()
{
    OstTrace0( TRACE_NORMAL, WLANWIZARDPAGESCANNING_STARTOPERATION,
        "WlanWizardPageScanning::startOperation - start AP scan if results"
        " are available." );

    if (mScanResultsAvailable) {
        mScanResultsAvailable = false;
        wlanScanApReady();
    }
}

