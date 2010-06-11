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
* WLAN Wizard: Private implementation.
*/

// System includes
#include <QLocale>
#include <QApplication>
#include <QGraphicsWidget>
#include <QTimer>
#include <QDebug>
#include <HbTranslator>
#include <HbDocumentLoader>
#include <HbMainWindow>
#include <HbDialog>
#include <HbStackedWidget>
#include <HbLabel>
#include <HbAction>
#include <HbStyleLoader>

// User includes
#ifdef WLAN_WIZARD_RND_EAP
#include "eapwizard.h"
#endif

#ifdef WLAN_WIZARD_RND_WPS
#include "wpswizard.h"
#endif
#include "wlanqtutils.h"
#include "wlanqtutilsap.h"
#include "wlanwizardplugin.h"
#include "wlanwizard.h"
#include "wlanwizard_p.h"
#include "wlanwizardutils.h"
#include "wlanwizardpagekeyquery.h"
#include "wlanwizardpagesummary.h"
#include "wlanwizardpageprocessingsettings.h"
#include "wlanwizardpagegenericerror.h"

#ifdef WLAN_WIZARD_RND
#include "wlanwizardpagessid.h"
#include "wlanwizardpagenetworkmode.h"
#include "wlanwizardpagescanning.h"
#include "wlanwizardpagesecuritymode.h"
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanwizard_pTraces.h"
#endif

/*!
   \class WlanWizardPrivate
   \brief Private implementation of WlanWizard. Implements the interface
   WlanWizardHelper to the wizard plugins. 
 */

// External function prototypes

// Local constants

/*!
   \var PageTimeout Timeout for timer protected pages. 1.5sec
 */

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
   Constructor of private implementation.
   
   @param [in] wizard Pointer to the API class.
   @param [in] mainWindow pointer to mainwindow.
 */
WlanWizardPrivate::WlanWizardPrivate(
    WlanWizard* wizard, 
    HbMainWindow *mainWindow) :
    QObject(wizard),
    q_ptr(wizard), 
    mMainWindow(mainWindow), 
    mPageTimer(new QTimer(this)),
    mTitle(NULL),
    mActionNext(NULL),
    mActionPrevious(NULL),
    mActionFinish(NULL),
    mActionCancel(NULL),
    mStackedWidget(NULL),
    mDialog(NULL),
    mTranslator(new HbTranslator("wlanwizard")),
    mWlanQtUtils(new WlanQtUtils()),
    mDocLoader(new HbDocumentLoader(mainWindow)),
    mEapWizard(NULL), 
    mWpsWizard(NULL), 
    mFirstPageId(WlanWizardPageInternal::PageNone),
    mPageFinished(false),
    mClosed(false)
{
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_WLANWIZARDPRIVATE,
        "WlanWizardPrivate::WlanWizardPrivate;this=%x", 
        this );
    
    // Set initial values for configurations
    setConfiguration(ConfIapId, WlanQtUtils::IapIdNone);
    setConfiguration(ConfConnected, false);
    setConfiguration(ConfHiddenWlan, false);
    setConfiguration(ConfProcessSettings, false);
    setConfiguration(ConfIctStatus, false);

    // Initialize timer for timer protected pages
    mPageTimer->setSingleShot(true);

    // Load the wizard frame (title, content, actions) from docml 
    loadDocml();

    // Creates the control object of the wlan wizard pages. 
    createPages();

    // First page is SSID query, unless client sets configurations via
    // setParameters(), which decides the first page..
    mFirstPageId = WlanWizardPageInternal::PageSsid;

    OstTraceExt2( TRACE_BORDER, WLANWIZARDPRIVATE_WLANWIZARDPRIVATE_DONE,
        "WlanWizardPrivate::WlanWizardPrivate - done;"
        "this=%x;mFirstPageId=%{PageIds}",
        ( unsigned )this, mFirstPageId );
}

/*!
   Destructor.
 */
WlanWizardPrivate::~WlanWizardPrivate()
{
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_DWLANWIZARDPRIVATE,
        "WlanWizardPrivate::~WlanWizardPrivate;this=%x",
        this );
    
    // Remove wizard pages from stackedwidgets, since stackedwidget owns the
    // objects and all pages are deleted below. 
    while (mStackedWidget->count()) {
        mStackedWidget->removeAt(0);
    }

    // Delete UI instances (HbWidget) of pages
    QHashIterator<HbWidget*, WlanWizardPage*> i(mPageMapper);
    while (i.hasNext()) {
        i.next();
        delete i.key();
    }
    mPageMapper.clear();

    // WlanWizardPage objects are automatically deleted since this is the
    // parent of the objects.
    mPages.clear();
    
#ifdef WLAN_WIZARD_RND_EAP
    delete mEapWizard;
#endif
#ifdef WLAN_WIZARD_RND_WPS
    delete mWpsWizard;
#endif
    // timer is cancelled/deleted automatically when the parent (this) is deleted 
    
    // TODO: See TSW Error: MTAA-854DK8 and loadDocml()
    HbStyleLoader::unregisterFilePath(":/css/custom.css");
    
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_DWLANWIZARDPRIVATE_DONE,
        "WlanWizardPrivate::~WlanWizardPrivate-Done;this=%x",
        this );
}

/*!
   See WlanWizard::setParameters().
 */
void WlanWizardPrivate::setParameters(
    const QString &ssid, 
    int networkMode, 
    int securityMode, 
    bool usePsk,
    bool hidden, 
    bool wps)
{
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_SETPARAMETERS, 
        "WlanWizardPrivate::setParameters;this=%x", 
        this );
    
    mFirstPageId = getNextPageId(
        ssid, networkMode, securityMode, usePsk, hidden, wps);
    
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_SETPARAMETERS_DONE, 
        "WlanWizardPrivate::setParameters - Done;this=%x", 
        this );
}

/*!
   See WlanWizard::show().
 */
void WlanWizardPrivate::show()
{
    OstTraceExt2( TRACE_BORDER, WLANWIZARDPRIVATE_SHOW,
        "WlanWizardPrivate::show;this=%x;mFirstPageId=%{PageIds}", 
        ( unsigned )this, mFirstPageId );
    
    Q_ASSERT(mClosed == false);
    showPage(mFirstPageId, false);
    mDialog->show();
    
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_SHOW_DONE,
        "WlanWizardPrivate::show - Done;this=%x;",
        (unsigned)this );
}

/*!
   See WlanWizard::setParameters() for descriptions of parameters.
   
   Based on the provided parameters next page id is returned.
   
   @return next page id based on provided configurations.
 */
int WlanWizardPrivate::getNextPageId(
    const QString &ssid, 
    int networkMode, 
    int securityMode, 
    bool usePsk,
    bool hidden, 
    bool wps)
{
    setConfiguration(ConfProcessSettings, true);
    setConfiguration(ConfSsid, ssid);
    setConfiguration(ConfNetworkMode, networkMode);
    setConfiguration(ConfSecurityMode, securityMode);
    setConfiguration(ConfUsePsk, usePsk);
    setConfiguration(ConfHiddenWlan, hidden);

    OstTrace1( TRACE_NORMAL, WLANWIZARDPRIVATE_GETNEXTPAGEID, 
        "WlanWizardPrivate::getNextPageId;wps=%u", 
        wps );

    return nextPageId(wps);
}

/*!
   Utility method for WLAN Wizard pages to query the wlanqtutils class.
   
   @return a valid pointer to WlanQtUtils class.
 */
WlanQtUtils* WlanWizardPrivate::wlanQtUtils() const
{
    Q_ASSERT(mWlanQtUtils);
    return mWlanQtUtils.data();
}

/*!
   Utility method for WLAN Wizard pages to query active wizard plugin object
   
   @return NULL in case plugin is not active.
 */ 
WlanWizardPlugin* WlanWizardPrivate::wlanWizardPlugin() const
{
    WlanWizardPlugin* plugin = NULL;

    if (isEapEnabled()) {
#ifdef WLAN_WIZARD_RND_EAP
        plugin = mEapWizard;
        Q_ASSERT(plugin);
#endif
    }
    
    OstTrace1( TRACE_NORMAL, WLANWIZARDPRIVATE_WLANWIZARDPLUGIN, 
        "WlanWizardPrivate::wlanWizardPlugin;plugin=%x", 
        plugin );
    
    return plugin;
}

/*!
   Utility method for WLAN Wizard pages to query if EAP mode is active at the
   moment
   
   @return true in case EAP is currently activated.
 */
bool WlanWizardPrivate::isEapEnabled() const
{
    bool ret = false;
    int secMode = configuration(ConfSecurityMode).toInt();

    if (((secMode == CMManagerShim::WlanSecModeWpa || 
          secMode == CMManagerShim::WlanSecModeWpa2) && 
          !configuration(ConfUsePsk).toBool()) || 
        secMode == CMManagerShim::WlanSecMode802_1x) {
        ret = true;
    }

    OstTrace1( TRACE_NORMAL, WLANWIZARDPRIVATE_ISEAPENABLED, 
        "WlanWizardPrivate::isEapEnabled;ret=%u", 
        ret );
    
    return ret;
}

/*!
   Creates new iap or updates existing iap.
   
   @return false in case IAP creation/update failed.
 */
bool WlanWizardPrivate::handleIap()
{
    OstTrace0( TRACE_FLOW, WLANWIZARDPRIVATE_HANDLEIAP,
        "WlanWizardPrivate::handleIap" );
    
    bool ret = true;
    bool usePsk = true;
    int securityMode = configuration(ConfSecurityMode).toInt();
    WlanQtUtilsAp wlanAp;
   
    // Set configuration
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSsid, configuration(ConfSsid));
    wlanAp.setValue(WlanQtUtilsAp::ConfIdConnectionMode, 
        configuration(ConfNetworkMode));
    wlanAp.setValue(WlanQtUtilsAp::ConfIdSecurityMode, securityMode );
    wlanAp.setValue(WlanQtUtilsAp::ConfIdHidden, configuration(ConfHiddenWlan));
    
    switch (securityMode) {
    case CMManagerShim::WlanSecModeWep:
        wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey1, configuration(ConfKeyWep1));
        wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey2, configuration(ConfKeyWep2));
        wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey3, configuration(ConfKeyWep3));
        wlanAp.setValue(WlanQtUtilsAp::ConfIdWepKey4, configuration(ConfKeyWep4));
        wlanAp.setValue(
            WlanQtUtilsAp::ConfIdWepDefaultIndex,
            configuration(ConfKeyWepDefault));
        break;

    case CMManagerShim::WlanSecModeWpa:
    case CMManagerShim::WlanSecModeWpa2:
        usePsk = configuration(ConfUsePsk).toBool();
        wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPskUse, usePsk);
        if (usePsk){
            wlanAp.setValue(WlanQtUtilsAp::ConfIdWpaPsk, configuration(ConfKeyWpa));
        }
        break;
    
    default:
        Q_ASSERT(securityMode == CMManagerShim::WlanSecModeOpen ||
            securityMode == CMManagerShim::WlanSecMode802_1x);
        // No WLAN Specific configuration
        break;
    }
    
    // Create IAP if does not exists or update the existing IAP
    int referenceId = configuration(ConfIapId).toInt();
    if (referenceId == WlanQtUtils::IapIdNone) {
        OstTrace0( TRACE_FLOW, WLANWIZARDPRIVATE_HANDLEIAP_CREATE, 
            "WlanWizardPrivate::handleIap: Create IAP" );
        
        referenceId = mWlanQtUtils->createIap(&wlanAp);
        setConfiguration(ConfIapId, referenceId);
        
        if (referenceId == WlanQtUtils::IapIdNone) {
            ret = false;
        }
    } else {
        OstTrace0( TRACE_FLOW, WLANWIZARDPRIVATE_HANDLEIAP_UPDATE, 
            "WlanWizardPrivate::handleIap: Update IAP" );

        ret = mWlanQtUtils->updateIap(referenceId, &wlanAp);
    }
   
    if (ret) {
        // Store Wizard plugin specific settings here.
        WlanWizardPlugin* plugin = wlanWizardPlugin();
        if (plugin) {
            OstTrace0( TRACE_FLOW, WLANWIZARDPRIVATE_HANDLEIAP_PLUGIN, 
                "WlanWizardPrivate::handleIap: Plugin" );
            // Plugin gets the IAP ID from configuration
            ret = plugin->storeSettings();
        }
    }
    
    OstTrace1( TRACE_FLOW, WLANWIZARDPRIVATE_HANDLEIAP_DONE,
        "WlanWizardPrivate::handleIap: Done;ret=%d",
        ret );
    
    return ret;
}

/*!
   See WlanWizardHelper::configuration().
 */
QVariant WlanWizardPrivate::configuration(ConfigurationId confId) const
{
    Q_ASSERT(mConfigurations.contains(confId));

#ifdef OST_TRACE_COMPILER_IN_USE
    QString tmp;
    QDebug tmpStream(&tmp);
    tmpStream << mConfigurations[confId];
    TPtrC16 string( tmp.utf16(), tmp.length() );
    
    OstTraceExt2( TRACE_NORMAL, WLANWIZARDPRIVATE_CONFIGURATIONS, 
        "WlanWizardPrivate::configuration;confId=%{ConfigurationId};string=%S", 
        (uint)confId, string );
#endif
    
    return mConfigurations[confId];
}

/*!
   See WlanWizardHelper::setConfiguration().
 */
void WlanWizardPrivate::setConfiguration(
    ConfigurationId confId, 
    const QVariant &value)
{
#ifdef OST_TRACE_COMPILER_IN_USE
    QString tmp;
    QDebug tmpStream(&tmp);
    tmpStream << value;
    TPtrC16 string( tmp.utf16(), tmp.length() );
    
    OstTraceExt2( TRACE_NORMAL, WLANWIZARDPRIVATE_SETCONFIGURATION, 
        "WlanWizardPrivate::setConfiguration;"
        "confId=%{ConfigurationId};string=%S", 
        (uint)confId, string );
#endif

    mConfigurations[confId] = value;
}

/*!
   See WlanWizardHelper::enableNextButton().
 */
void WlanWizardPrivate::enableNextButton(bool enable)
{
    OstTraceExt2( TRACE_FLOW, WLANWIZARDPRIVATE_ENABLENEXTBUTTON,
        "WlanWizardPrivate::enableNextButton;this=%x;enable=%x",
        (unsigned)this, (uint)enable );
    mActionNext->setEnabled(enable);
}

/*!
   See WlanWizardHelper::enablePrevButton().
 */
void WlanWizardPrivate::enablePrevButton(bool enable)
{
    OstTraceExt2( TRACE_FLOW, WLANWIZARDPRIVATE_ENABLEPREVBUTTON,
        "WlanWizardPrivate::enablePrevButton;this=%x;enable=%x", 
        (unsigned)this, (uint)(enable) );
    
    mActionPrevious->setEnabled(enable);
}

/*!
   See WlanWizardHelper::addPage().
 */
void WlanWizardPrivate::addPage(int pageId, WlanWizardPage *page)
{
    OstTraceExt3( TRACE_FLOW, WLANWIZARDPRIVATE_ADDPAGE, 
        "WlanWizardPrivate::addPage;this=%x;pageId=%{PageIds};page=%x",
        (unsigned)this, pageId, (uint)(page) );

    Q_ASSERT(!mPages.contains(pageId));
    mPages[pageId] = page;
}

/*!
   See WlanWizardHelper::nextPage().
 */
void WlanWizardPrivate::nextPage()
{
    OstTrace1( TRACE_FLOW, WLANWIZARDPRIVATE_NEXTPAGE,
        "WlanWizardPrivate::nextPage;this=%x",
        this );
    
    mPageFinished = true;
    toNextPage();
}

/*!
   See WlanWizardHelper::mainWindow().
 */
HbMainWindow* WlanWizardPrivate::mainWindow() const
{
    return mMainWindow;
}

/*!
   See WlanWizardHelper::isCurrentPage().
 */
bool WlanWizardPrivate::isCurrentPage(const HbWidget *page) const
{
    bool ret = false;
    if (mStackedWidget->currentWidget() == page) {
        ret = true;
    }
    
    OstTraceExt2( TRACE_FLOW, WLANWIZARDPRIVATE_ISCURRENTPAGE, 
        "WlanWizardPrivate::isCurrentPage;page=%x;ret=%d", 
        (uint)page, ret);
    
    return ret;
}

/*!
   See WlanWizardHelper::nextPageId()
 */
int WlanWizardPrivate::nextPageId(bool useWps)
{
    int ret;
    int secMode = configuration(WlanWizardHelper::ConfSecurityMode).toInt();
    switch (secMode) {
    case CMManagerShim::WlanSecModeWep:
        ret = WlanWizardPageInternal::PageKeyQuery;
        break;
        
    case CMManagerShim::WlanSecModeWpa:
    case CMManagerShim::WlanSecModeWpa2:
        if (configuration(WlanWizardHelper::ConfUsePsk).toBool()) {
            ret = WlanWizardPageInternal::PageKeyQuery;
        } else {
#ifdef WLAN_WIZARD_RND_EAP
            ret = WlanWizardPage::PageEapStart;
#else
            ret = WlanWizardPageInternal::PageProcessSettings;
            setConfiguration(ConfSecurityMode, CMManagerShim::WlanSecModeOpen);
#endif

        }
        break;
        
    case CMManagerShim::WlanSecMode802_1x:
#ifdef WLAN_WIZARD_RND_EAP
        ret = WlanWizardPage::PageEapStart;
#else
        ret = WlanWizardPageInternal::PageProcessSettings;
        setConfiguration(ConfSecurityMode, CMManagerShim::WlanSecModeOpen);
#endif
        break;
        
    case CMManagerShim::WlanSecModeWapi:
    case CMManagerShim::WlanSecModeOpen:
    default:
        Q_ASSERT(
            secMode == CMManagerShim::WlanSecModeOpen || 
            secMode == CMManagerShim::WlanSecModeWapi);
        
        setConfiguration(ConfSecurityMode, CMManagerShim::WlanSecModeOpen);
        ret = WlanWizardPageInternal::PageProcessSettings;
        break;
    }

    Q_UNUSED(useWps);
#ifdef WLAN_WIZARD_RND_WPS
    // TODO: WPS: and switch case above to else branch
    if (useWps) {
        ret = WlanWizardPage::PageWpsStart;
    }
#endif

    OstTraceExt3( TRACE_NORMAL, WLANWIZARDPRIVATE_NEXTPAGEID,
        "WlanWizardPrivate::nextPageId;this=%x;useWps=%x;ret=%{PageIds}",
        ( unsigned )this, ( TUint )( useWps ), ret );

    return ret;
}

/*!
   Called when Cancel toolbar button is pressed. In case IAP has been created
   it will be disconnected and the IAP settings are deleted and finally the
   wizard is closed. 
   
   Indicates also to the current wizard page that cancel has been pressed.
 */
void WlanWizardPrivate::cancelTriggered()
{
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_CANCELTRIGGERED,
        "WlanWizardPrivate::cancelTriggered;this=%x",
        this );
    
    // Disconnect receiving more signals from any actions
    disconnectActions();
    
    mPageTimer->stop();

    HbWidget *widget = qobject_cast<HbWidget*>(mStackedWidget->currentWidget());
    Q_ASSERT(widget);
    mPageMapper[widget]->cancelTriggered();

    int referenceId = configuration(ConfIapId).toInt();
    if (referenceId != WlanQtUtils::IapIdNone) {
        // call disconnect even if
        // - connection is not open
        // - connection establishment is ongoing
        mWlanQtUtils->disconnectIap(referenceId);
        
        // if IAP deletion fails, there is nothing we can do with it
        mWlanQtUtils->deleteIap(referenceId);
        WlanWizardPlugin* plugin = wlanWizardPlugin();
        if (plugin) {
            // if deletion of plugin specific configuration fails we are not
            // able to do anything
            plugin->deleteSettings();
        }
        setConfiguration(ConfIapId, WlanQtUtils::IapIdNone);
    }
    closeViews();
    Q_ASSERT(q_ptr);

    OstTrace0( TRACE_BORDER, WLANWIZARDPRIVATE_CANCELTRIGGERED_EMIT,
        "WlanWizardPrivate::cancelTriggered - emit cancelled()" );
    
    emit q_ptr->cancelled();
    
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_CANCELTRIGGERED_DONE,
        "WlanWizardPrivate::cancelTriggered - Done;this=%x", this );
}

/*!
   Called when Previous toolbar button is pressed. Shows the previous wizard 
   page in stacked widget and indicates wizard page that previous has been 
   pressed.
 */
void WlanWizardPrivate::previousTriggered()
{
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_PREVIOUSTRIGGERED,
        "WlanWizardPrivate::previousTriggered;this=%x",
        this );
    
    mPageTimer->stop();

    int index = mStackedWidget->currentIndex();
    HbWidget *widget = qobject_cast<HbWidget*>(mStackedWidget->currentWidget());
    Q_ASSERT(widget);
    int back = mPageMapper[widget]->previousTriggered();

    // check that at least one page is left in the stack
    Q_ASSERT( back < mStackedWidget->count());

    // When a widget which is last in the stack is removed the currentindex
    // is automatically updated.
    for (int i = 0; i < back; ++i) {
        mStackedWidget->removeAt(index - i);
    }

    widget = qobject_cast<HbWidget*> (mStackedWidget->currentWidget());
    Q_ASSERT(widget);
    WlanWizardPage* page = mPageMapper[widget];
    Q_ASSERT(page);
    // In error case if page (value) is not found default contructed key is 
    // returned, in this case default value for int is 0 which means PageNone.
    updateFrame(mPages.key(page));
    enableNextButton(page->showPage());
    
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_PREVIOUSTRIGGERED_DONE,
        "WlanWizardPrivate::previousTriggered - Done;this=%x",
        this );
}

/*!
   Next toolbar button has been pressed. Wizard framework asks from the current
   page what should be the next page id and shows it.
 */
void WlanWizardPrivate::nextTriggered()
{
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_NEXTTRIGGERED,
        "WlanWizardPrivate::nextTriggered;this=%x",
        this );
    
    mPageTimer->stop();

    HbWidget *widget = qobject_cast<HbWidget*>(mStackedWidget->currentWidget());
    Q_ASSERT(widget);

    bool removeFromStack;
    int pageId = mPageMapper[widget]->nextId(removeFromStack);
    showPage(pageId, removeFromStack);
    
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_NEXTTRIGGERED_DONE,
        "WlanWizardPrivate::nextTriggered - Done;this=%x",
        this );
}

/*!
   Finish button has been pressed. Closes the wizard and sends finished()
   signal to the client. 
 */
void WlanWizardPrivate::finishTriggered()
{
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_FINISHTRIGGERED,
        "WlanWizardPrivate::finishTriggered;this=%x",
        this );
    
    // Disconnect receiving more signals from any actions
    disconnectActions();
    
    mPageTimer->stop();
    closeViews();

    Q_ASSERT(q_ptr);
    emit q_ptr->finished(
        configuration(ConfIapId).toInt(), 
        configuration(ConfConnected).toBool());
    
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_FINISHTRIGGERED_DONE,
        "WlanWizardPrivate::finishTriggered - Done;this=%x",
        this );
}

/*!
   In case wizard page is using timer protection for the page, this is the
   slot to handle the callback from the timer.
 */
void WlanWizardPrivate::onTimeOut()
{
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_ONTIMEOUT,
        "WlanWizardPrivate::onTimeOut;this=%x",
        this );
    
    toNextPage();
    
    OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_ONTIMEOUT_DONE,
        "WlanWizardPrivate::onTimeOut - Done;this=%x",
        this );
}

/*!
   Slot to start wizard page operation asynchronous. Starts also the timer
   to protect the wizard page.
 */
void WlanWizardPrivate::startPageOperation()
{
    // Process this if wizard has not been closed
    if (mClosed == false) {
        OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_STARTPAGEOPERATION,
            "WlanWizardPrivate::startPageOperation;this=%x",
            this );
        
        HbWidget *widget = 
            qobject_cast<HbWidget*>(mStackedWidget->currentWidget());
        Q_ASSERT(widget);
        mPageFinished = false;
        mPageTimer->start(PageTimeout);
        mPageMapper[widget]->startOperation();
        
        OstTrace1( TRACE_BORDER, WLANWIZARDPRIVATE_STARTPAGEOPERATION_DONE,
            "WlanWizardPrivate::startPageOperation - DONE;this=%x",
            this );
    }
}

/*!
   In case the wizard page uses timer protection this method determines when
   the next page is shown.
 */
void WlanWizardPrivate::toNextPage()
{
    if (mPageFinished && !(mPageTimer->isActive())) {
        OstTrace1( TRACE_FLOW, WLANWIZARDPRIVATE_TONEXTPAGE,
            "WlanWizardPrivate::toNextPage;this=%x",
            this );
        
        // process this asynchronous. Main purpose is to release the current
        // call stack and process the page change using new call stack
        QMetaObject::invokeMethod(this, "nextTriggered", Qt::QueuedConnection);
    }
}

/*!
   Show requested page and removes the current wizard page from the stack if
   required. This method updates the title and toolbar accordingly.
   
   See WlanWizardPage::nextId()
   
   @param [in] pageId Wizard Page ID to be shown
   @param [in] removeFromStack if true the current wizard page is removed from
                               the stackedwidget. 
 */
void WlanWizardPrivate::showPage(int pageId, bool removeFromStack)
{
    OstTraceExt3( TRACE_FLOW, WLANWIZARDPRIVATE_SHOWPAGE,
        "WlanWizardPrivate::showPage;this=%x;"
        "pageId=%{PageIds};removeFromStack=%x",
        ( unsigned )this, pageId, ( TUint )( removeFromStack ) );
    
    // PageNone is returned by wizard pages when some validation of page content
    // has not passed and the page does not want to process wizard to next page
    if (pageId == WlanWizardPage::PageNone) {
        return;
    }
    
    //  PageProcessSettings is shown only when WLAN AP is found in scanning
    // 1/ when client calls setParameters()
    // 2/ scanning page has found match with given SSID
    if (pageId == WlanWizardPage::PageProcessSettings) {
        if (configuration(ConfProcessSettings).toBool() == false) {
            if (handleIap()){
                pageId = WlanWizardPageInternal::PageSummary;
            } else {
                pageId = WlanWizardPage::PageGenericError;
                setConfiguration(
                    ConfGenericErrorString, 
                    hbTrId("txt_occ_dialog_unable_to_save_settings_please_ret"));
            }
            OstTraceExt2( TRACE_FLOW, WLANWIZARDPRIVATE_SHOWPAGE_UPDATE,
                "WlanWizardPrivate::showPage - change page;this=%x;"
                "pageId=%{PageIds}",
                ( unsigned )this, pageId);
        }
    }
    
#ifdef WLAN_WIZARD_RND_EAP
    if (pageId == WlanWizardPage::PageEapStart) {
        if (!mEapWizard) {
            // EAP Wizard will add wizard pages at construction phase using
            // WlanWizardHelper::addPage()
            mEapWizard = new EapWizard(this);
        }
    }
#endif
#ifdef WLAN_WIZARD_RND_WPS
    if (pageId == WlanWizardPage::PageWpsStart) {
        if (!mWpsWizard) {
            // WPS Wizard will add wizard pages at construction phase using
            // WlanWizardHelper::addPage()
            mWpsWizard = new WpsWizard(this);
        }
    }
#endif

    // Create visualization of next page and store it to mappers
    WlanWizardPage* page = mPages[pageId];
    Q_ASSERT(page);
    HbWidget* widget = page->initializePage();
    Q_ASSERT(widget);
    mPageMapper[widget] = page;

    // index calculation has to be happened before adding new widget into
    // the stacked widget. Since the internal implementation of stackedwidget
    // sets the first widget in the stack to current widget..
    // and when there are not any widgets in the stack the current index is
    // -1. Valid index starts from 0.
    int index = mStackedWidget->currentIndex();

    if (removeFromStack) {
        // widget is removed from the stacked widget, not deleted
        mStackedWidget->removeAt(index);
    } else {
        index++;
    }
    mStackedWidget->addWidget(widget);
    mStackedWidget->setCurrentIndex(index);
    updateFrame(pageId);
    enableNextButton(page->showPage());    

    // If wizard page needs to start some control operation trigger it 
    // asyncronously so that the UI gets more priority to get painted correctly
    // before any operation takes place in wizard page. This is important for
    // timer protected pages. Makes wizard to work smother from UI perspective
    if (page->requiresStartOperation()) {
        OstTrace0( TRACE_FLOW, WLANWIZARDPRIVATE_SHOWPAGE_INVOKE,
            "WlanWizardPrivate::showPage - Invoke startOperation" );
        
        QMetaObject::invokeMethod(this, "startPageOperation", Qt::QueuedConnection);
    }
}

/*!
   Creates all control objects of WLAN wizard pages which are inherited from
   WlanWizardPage and adds those into a internal database. 
 */
void WlanWizardPrivate::createPages()
{
    OstTrace0( TRACE_NORMAL, WLANWIZARDPRIVATE_CREATEPAGES,
        "WlanWizardPrivate::createPages" );
    
    addPage(WlanWizardPageInternal::PageKeyQuery, 
        new WlanWizardPageKeyQuery(this));
    
    addPage(WlanWizardPageInternal::PageProcessSettings, 
        new WlanWizardPageProcessingSettings(this));
    
    addPage(WlanWizardPageInternal::PageSummary, 
        new WlanWizardPageSummary(this));

    addPage(WlanWizardPageInternal::PageGenericError, 
        new WlanWizardPageGenericError(this));
    
#ifdef WLAN_WIZARD_RND
    addPage(WlanWizardPageInternal::PageSsid, new WlanWizardPageSsid(this));
    
    addPage(WlanWizardPageInternal::PageScanning, 
        new WlanWizardPageScanning(this));
    
    addPage(WlanWizardPageInternal::PageNetworkMode, 
        new WlanWizardPageNetworkMode(this));
    
    addPage(WlanWizardPageInternal::PageNetworkSecurity, 
        new WlanWizardPageSecurityMode(this));
#endif
}

/*!
   Called when wizard is closed 
   - cancelled by the user
   - finished by the user
 */
void WlanWizardPrivate::closeViews()
{
    mClosed = true;
    mDialog->close();
}

/*!
   This method takes care of the title of wizard and toolbutton. Correct items
   are selected based on the \a pageId and the amount of objects in the stacked
   widget.
   
   @param [in] pageId Wizard Page Id
 */
void WlanWizardPrivate::updateFrame(int pageId)
{
    int currentIndex = mStackedWidget->currentIndex();

    OstTraceExt3( TRACE_FLOW, WLANWIZARDPRIVATE_UPDATEFRAME, 
        "WlanWizardPrivate::updateFrame;this=%x;pageId=%{PageIds};currentIndex=%d",
        (unsigned)this, pageId, (uint)(currentIndex) );
    
    // For last page (summary) show Finish instead of Next button
    if (pageId == WlanWizardPageInternal::PageSummary) {
        mTitle->setPlainText(hbTrId("txt_occ_title_wlan_setup_wizard_summary"));
        mActionFinish->setVisible(true);
        mActionNext->setVisible(false);
        mActionPrevious->setVisible(false);
    } else {
        // Index starts from zero, wizard page numbering starts from one.
        mTitle->setPlainText(
            hbTrId("txt_occ_title_wlan_setup_wizard_step_l1").arg(
                currentIndex + 1));
        mActionFinish->setVisible(false);
        mActionNext->setVisible(true);

        // If first page is shown then Previous button is disabled
        if (currentIndex < 1) {
            mActionPrevious->setVisible(false);
        } else {
            mActionPrevious->setVisible(true);
            if (pageId == WlanWizardPage::PageProcessSettings) {
                mActionPrevious->setEnabled(false);
            } else {
                mActionPrevious->setEnabled(true);
            }
        }
    }
}

/*!
   Loads widgets and objects from the docml file. 
 */
void WlanWizardPrivate::loadDocml()
{
    bool ok = true;
    
    mDocLoader->load(":/docml/occ_wlan_wizard_main.docml", &ok);
    Q_ASSERT(ok);

    mDialog.reset( qobject_cast<HbDialog*> (mDocLoader->findWidget("dialog")) );
    Q_ASSERT(mDialog != NULL);

    mTitle = qobject_cast<HbLabel*> (mDocLoader->findWidget("title"));
    Q_ASSERT(mTitle != NULL);

    mStackedWidget = 
        qobject_cast<HbStackedWidget*> (mDocLoader->findWidget("stackedWidget"));
    Q_ASSERT(mStackedWidget != NULL);

    mActionNext = qobject_cast<HbAction*> (mDocLoader->findObject("actionNext"));
    Q_ASSERT(mActionNext != NULL);

    mActionPrevious = 
        qobject_cast<HbAction*> (mDocLoader->findObject("actionPrevious"));
    Q_ASSERT(mActionPrevious != NULL);

    mActionFinish = 
        qobject_cast<HbAction*> (mDocLoader->findObject("actionFinish"));
    Q_ASSERT(mActionFinish != NULL);

    mActionCancel = 
        qobject_cast<HbAction*> (mDocLoader->findObject("actionCancel"));
    Q_ASSERT(mActionCancel != NULL);
    
    // Actions are added from left to right to the toolbar of dialog
    mDialog->addAction(mActionPrevious);
    mDialog->addAction(mActionCancel);
    mDialog->addAction(mActionNext);
    mDialog->addAction(mActionFinish);
    
    // TODO: workaround to prevent action to close the dialog
    disconnect(mActionPrevious, SIGNAL(triggered()), mDialog.data(), SLOT(close()));
    disconnect(mActionCancel, SIGNAL(triggered()), mDialog.data(), SLOT(close()));
    disconnect(mActionNext, SIGNAL(triggered()), mDialog.data(), SLOT(close()));
    disconnect(mActionFinish, SIGNAL(triggered()), mDialog.data(), SLOT(close()));

    ok = true;
    ok = connect(mPageTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    Q_ASSERT(ok);
    
    ok = connect(
        mActionNext, SIGNAL(triggered()), 
        this, SLOT(nextTriggered()));
    Q_ASSERT(ok);
    
    ok = connect(
        mActionPrevious, SIGNAL(triggered()), 
        this, SLOT(previousTriggered()));
    Q_ASSERT(ok);
    
    ok = connect(
        mActionFinish, SIGNAL(triggered()), 
        this, SLOT(finishTriggered()));
    Q_ASSERT(ok);
    
    ok = connect(
        mActionCancel, SIGNAL(triggered()), 
        this, SLOT(cancelTriggered()));
    Q_ASSERT(ok);

    // TODO: workaround for full screen dialog, with docml it is possible to
    // define fullscreen dialog, mut resize is not done correctly when orientation
    // is changed. See TSW Error: MTAA-854DK8
    ok = HbStyleLoader::registerFilePath(":/css/custom.css");
    Q_ASSERT(ok);
}

/*!
   Disconnect receiving triggered() signal from any toolbar action.
 */
void WlanWizardPrivate::disconnectActions()
{
    disconnect(
        mActionNext, SIGNAL(triggered()), 
        this, SLOT(nextTriggered()));
    
    disconnect(
        mActionPrevious, SIGNAL(triggered()), 
        this, SLOT(previousTriggered()));
    
    disconnect(
        mActionFinish, SIGNAL(triggered()), 
        this, SLOT(finishTriggered()));

    disconnect(
        mActionCancel, SIGNAL(triggered()), 
        this, SLOT(cancelTriggered()));
}