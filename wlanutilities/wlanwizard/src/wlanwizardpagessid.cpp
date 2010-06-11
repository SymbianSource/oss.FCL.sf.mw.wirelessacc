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
 *   WLAN Wizard Page: SSID Selection.
 *
 */

// System includes
#include <HbDocumentLoader>
#include <HbMainWindow>
#include <HbWidget>
#include <HbLineEdit>
#include <HbEditorInterface>
#include <HbLabel>
#include <wlanqtutils.h>

// User includes
#include "wlanwizardpagessid.h"
#include "wlanwizard_p.h"
#include "wlanwizardutils.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanwizardpagessidTraces.h"
#endif

/*!
 * Constructor. Member initialization.
 */
WlanWizardPageSsid::WlanWizardPageSsid(WlanWizardPrivate* parent) :
    WlanWizardPageInternal(parent),
    mWidget(NULL),
    mLabel(NULL),
    mSsid(NULL),
    mLabelError(NULL),
    mLoader(NULL)
{

}

/*!
 * Destructor. Loader widget is deleted.
 * All document widgets are deleted by wlanwizard_p destructor.
 */
WlanWizardPageSsid::~WlanWizardPageSsid()
{
    delete mLoader;
}

/*!
 * Page initialization. If view is already loaded, does nothing.
 */
HbWidget* WlanWizardPageSsid::initializePage()
{
    OstTrace0( TRACE_NORMAL, WLANWIZARDPAGESSID_INITIALIZEPAGE,
        "WlanWizardPageSsid::initializePage" );

    if (mWidget == NULL) {
        bool ok;

        mLoader = new HbDocumentLoader(mWizard->mainWindow());

        mLoader->load(":/docml/occ_add_wlan_01_04.docml", &ok);
        Q_ASSERT_X(ok, "WlanWizardPageSsid", "Invalid docml file");

        // Load orientation
        loadDocml(mWizard->mainWindow()->orientation());

        // Load widgets
        mWidget = qobject_cast<HbWidget*> (mLoader->findWidget(
            "occ_add_wlan_01"));
        Q_ASSERT_X(mWidget != NULL, "WlanWizardPageSsid", "View not found");

        mLabel = qobject_cast<HbLabel*> (mLoader->findWidget("dialog"));
        Q_ASSERT_X(mLabel != NULL, "WlanWizardPageSsid", "dialog not found");

        mSsid = qobject_cast<HbLineEdit*> (mLoader->findWidget("lineEditKey"));
        Q_ASSERT_X(mSsid != NULL, "WlanWizardPageSsid", "lineEditKey not found");

        mLabelError = qobject_cast<HbLabel*> (mLoader->findWidget(
            "labelErrorNote"));
        Q_ASSERT_X(mLabelError != NULL, "WlanWizardPageSsid",
            "labelErrorNote not found");

        mLabel->setPlainText(hbTrId(
            "txt_occ_dialog_insert_the_name_of_the_new_wlan_net"));

        // Connect orientation signal from the main window to orientation
        // loader.
        ok &= connect(mWizard->mainWindow(),
            SIGNAL(orientationChanged(Qt::Orientation)), this,
            SLOT(loadDocml(Qt::Orientation)));

        // Connect text change-signal from input dialog to handler function
        ok &= connect(mSsid, SIGNAL(textChanged(const QString &)), this,
            SLOT(textChanged(const QString &)));

        Q_ASSERT(ok);

        HbEditorInterface editInterface(mSsid);
        editInterface.setInputConstraints(HbEditorConstraintLatinAlphabetOnly);


        editInterface.setSmileyTheme(HbSmileyTheme());
        //editInterface.setEditorClass(HbInputEditorClassNetworkName);
        mSsid->setInputMethodHints(
            Qt::ImhNoPredictiveText | Qt::ImhPreferLowercase);
        mSsid->setMaxLength(WlanWizardUtils::SsidMaxLength);
        mSsid->installEventFilter(this);
    }
	
	// Open virtual keyboard by setting focus to line edit
	mSsid->setFocus();

    return mWidget;
}

/*!
 * Selection of the next page.
 */
int WlanWizardPageSsid::nextId(bool &removeFromStack) const
{
    int pageId = WlanWizardPage::PageNone;
    removeFromStack = false;

    WlanWizardUtils::SsidStatus status = WlanWizardUtils::validateSsid(
        mSsid->text());

    if (status != WlanWizardUtils::SsidStatusOk) {
        mLabelError->setPlainText(SsidStatusToErrorString(status));
    }
    else {
        // SSID is stored into configuration
        mWizard->setConfiguration(WlanWizardPrivate::ConfSsid, mSsid->text());
        pageId = WlanWizardPageInternal::PageScanning;
    }

    return pageId;
}

/*!
 * Load the document with given screen orientation.
 */
void WlanWizardPageSsid::loadDocml(Qt::Orientation orientation)
{
    OstTrace1( TRACE_FLOW, WLANWIZARDPAGESSID_LOADDOCML,
        "WlanWizardPageSsid::loadDocml - orientation ;orientation=%x",
        ( TUint )( orientation ) );

    bool ok;
    // Then load the orientation specific section
    if (orientation == Qt::Horizontal) {
        qDebug("Loading landscape section for wlan_01_04");
        mLoader->load(":/docml/occ_add_wlan_01_04.docml", "landscape_section",
            &ok);
        Q_ASSERT(ok);
    }
    else {
        Q_ASSERT(orientation == Qt::Vertical);
        qDebug("Loading portrait section for wlan_01_04");
        mLoader->load(":/docml/occ_add_wlan_01_04.docml", "portrait_section",
            &ok);
        Q_ASSERT(ok);
    }
}

/*!
 * Actions, when user makes changes to the text editor widget.
 */
void WlanWizardPageSsid::textChanged(const QString & /* text */)
{
    OstTrace0( TRACE_FLOW, WLANWIZARDPAGESSID_TEXTCHANGED,
        "WlanWizardPageSsid::textChanged in text edit widget" );

    mWizard->enableNextButton(showPage());
}

/*!
 * Returns true, if the text editor widget contains any characters.
 */
bool WlanWizardPageSsid::showPage()
{
    // Initiate the scanning of public APs here.
    mWizard->wlanQtUtils()->scanWlanAps();

   return !(mSsid->text().isEmpty());
}

/*!
 * Filter to catch focus event to the text editor widget.
 */
bool WlanWizardPageSsid::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == mSsid && event->type() == QEvent::FocusIn) {
        OstTrace0( TRACE_BORDER, WLANWIZARDPAGESSID_EVENTFILTER,
            "WlanWizardPageSsid::eventFilter text edit widget received focus" );

        mLabelError->setPlainText("");
    }
    return false;
}

/*!
 * Conversion function from SsidStatus to plain text.
 */
QString WlanWizardPageSsid::SsidStatusToErrorString(
    WlanWizardUtils::SsidStatus status) const
{
    QString errorString;
    switch (status) {
    case WlanWizardUtils::SsidStatusIllegalCharacters:
        errorString = hbTrId(
            "txt_occ_dialog_illegal_characters_in_key_please_c");
        break;
    case WlanWizardUtils::SsidStatusInvalidLength:
        errorString
            = hbTrId("txt_occ_dialog_key_is_of_incorrect_length_please");
        break;
    case WlanWizardUtils::SsidStatusOk:
    default:
        Q_ASSERT(WlanWizardUtils::SsidStatusOk == status);
        break;
    }
    return errorString;
}

