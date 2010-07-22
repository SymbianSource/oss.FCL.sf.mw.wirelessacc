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
* Main view for the WLAN Login application
* 
*/


#include <QGraphicsLinearLayout>
#include <QWebFrame>

// System includes
#include <HbDocumentLoader>
#include <HbToolbar>
#include <HbAction>
#include <HbProgressBar>
#include <HbScrollBar>
#include <HbScrollArea>
#include <HbWidget>

// User includes
#include "wlanloginview.h"
#include "wlanloginwebview.h"
#include "wlanloginwebpage.h"
#include "wlanloginmainwindow.h"
#include "wlanloginengine.h"
#include "ictsqtwrapper.h"

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanloginviewTraces.h"
#endif

/*!
    \class WlanLoginView
    \brief Main view for the WLAN Login application
*/

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
    Constructor       
 */
WlanLoginView::WlanLoginView(WlanLoginApplication* appRef):
    mAppRef(appRef),
    mMainLayout(NULL),
    mContentLayout(NULL),
    mProgressBar(NULL),
    mScrollArea(NULL),
    mScrollAreaContent(NULL),
    mWebView(NULL),
    mToolBar(NULL),
    mCancelAction(NULL),
    mNextAction(NULL),
    mContinueAction(NULL)
{   
    OstTraceFunctionEntry0(WLANLOGINVIEW_WLANLOGINVIEW_ENTRY);
    
    setTitleBarVisible(false);
    setStatusBarVisible(false);
    
    mMainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mMainLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mMainLayout->setContentsMargins(0,0,0,0);
    setLayout(mMainLayout);
      
    mScrollArea = new HbScrollArea();
    mScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mScrollArea->setVerticalScrollBarPolicy(HbScrollArea::ScrollBarAlwaysOff);
    mScrollArea->setHorizontalScrollBarPolicy(HbScrollArea::ScrollBarAlwaysOff);  
    mScrollArea->setScrollDirections(Qt::Vertical | Qt::Horizontal);
    mScrollArea->setClampingStyle( HbScrollArea::StrictClamping );
    mScrollArea->setScrollingStyle( HbScrollArea::PanWithFollowOn );
    mMainLayout->addItem(mScrollArea);
   
    mScrollAreaContent = new HbWidget();
    mScrollAreaContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    
    mContentLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mContentLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mContentLayout->setContentsMargins(0,0,0,0);
    mScrollAreaContent->setLayout(mContentLayout);
    mScrollArea->setContentWidget(mScrollAreaContent);
    
    // Set white background to content widget
    QPixmap pixmap(10,10);
    pixmap.fill(Qt::white);
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    mScrollAreaContent->setBackgroundItem(pixmapItem);
    
    //Note: Progress bar is added to layout when loadStarted signal is handled and
    //removed when loadFinished signal is handled
    mProgressBar = new HbProgressBar();  
    mProgressBar->setRange( 0,100);
    mProgressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mProgressBar->setVisible(false);
    
    mWebView = new WlanLoginWebView();
    mContentLayout->addItem(mWebView);
    mWebView->page()->setNetworkAccessManager(appRef->engine()->networkAccessManager());
    
    //Create tool bar
    mToolBar = new HbToolBar();   
    mCancelAction = mToolBar->addAction(HbIcon("qtg_mono_exit"),"");
    mNextAction = mToolBar->addAction(HbIcon("qtg_mono_arrow_right"),"");
    mContinueAction = mToolBar->addAction(HbIcon("qtg_mono_tick"),"");
    
    setToolBar(mToolBar);
    setActiveToolBarAction(Cancel);

    mToolBar->show();
    mWebView->show();
    mScrollArea->show();
    
    bool connectStatus = connect(
        mWebView,
        SIGNAL(loadStarted()),
        this,
        SLOT(handleLoadStarted())); 
    Q_ASSERT(connectStatus == true);
    
    connectStatus = connect(
        mWebView,
        SIGNAL(loadProgress(int)),
        this,
        SLOT(handleLoadProgress(int))); 
    Q_ASSERT(connectStatus == true);
    
    connectStatus = connect(
        mWebView,
        SIGNAL(loadFinished(bool)),
        this,
        SLOT(handleLoadFinished(bool)));
    Q_ASSERT(connectStatus == true);
    
    connectStatus = connect(
    mWebView,
    SIGNAL(urlChanged(const QUrl&)),
    this,
    SLOT(handleUrlChanged(const QUrl&)));
    Q_ASSERT(connectStatus == true);
    
    connectStatus = connect(
        mWebView->page(),
        SIGNAL(formSubmitted()),
        this,
        SLOT(handleFormSubmitted()));
    Q_ASSERT(connectStatus == true);
    
    connectStatus = connect(
        mCancelAction,
        SIGNAL(triggered()),
        this,
        SLOT(handleCancelAction()));
    Q_ASSERT(connectStatus == true);
    
    connectStatus = connect(
        mNextAction,
        SIGNAL(triggered()),
        this,
        SLOT(handleNextAction()));
    Q_ASSERT(connectStatus == true);
    
    connectStatus = connect(
        mContinueAction,
        SIGNAL(triggered()),
        this,
        SLOT(handleContinueAction()));
    Q_ASSERT(connectStatus == true);
    
    show();
    
    OstTraceFunctionExit0(WLANLOGINVIEW_WLANLOGINVIEW_EXIT);
}


/*!
    Destructor       
 */
WlanLoginView::~WlanLoginView()
{
    OstTraceFunctionEntry0(WLANLOGINVIEW_DESTRUCTOR_ENTRY);
    
    //delete progress bar here as it is not on the layout all the time and
    //thus may not have a parent at all.
    if (mProgressBar) {
        delete mProgressBar;
    }
    
    OstTraceFunctionExit0(WLANLOGINVIEW_DESTRUCTOR_EXIT);
}


/*!
    This function handles connectionReady signal from engine and starts loading of
    provided web page
    
    @param [in] url URL to be loaded
 */
void WlanLoginView::handleConnectionReady(QUrl url)
{
    OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLECONNECTIONREADY_ENTRY);
    
    mWebView->load(url);
    
    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLECONNECTIONREADY_EXIT);
}


/*!
    This function handles orientationChanged signal
    
    @param [in] orientation New Orientation of the screen
 */
void WlanLoginView::handleOrientationChanged(Qt::Orientation orientation)
{
    Q_UNUSED(orientation);
    OstTraceFunctionEntry0( WLANLOGINVIEW_ORIENTATIONCHANGED_ENTRY );
    
    adjustViewSize();
    
    OstTraceFunctionEntry0( WLANLOGINVIEW_ORIENTATIONCHANGED_EXIT );
}

void WlanLoginView::adjustViewSize()
{
    OstTraceFunctionEntry0( WLANLOGINVIEW_ADJUSTVIEWSIZE_ENTRY );
    
    //Store current screen size
    QSizeF screenSize = mAppRef->mainWindow()->layoutRect().size();
    
    //Store current content size
    QSize contentSize = mWebView->page()->mainFrame()->contentsSize();
    
    //Set viewPortSize to biggest values of content size or current screen size 
    QSize newViewPortSize;
    if (screenSize.toSize().width() > contentSize.width()) {
        newViewPortSize.setWidth(screenSize.toSize().width());
    } else {    
        newViewPortSize.setWidth(contentSize.width());
    }
    
    if (screenSize.toSize().height() > contentSize.height()) {
        newViewPortSize.setHeight(screenSize.toSize().height());
    } else {    
        newViewPortSize.setHeight(contentSize.height());
    }
    mWebView->page()->setViewportSize(newViewPortSize);
    
    
    //Set Web View size to same size as viewport
    mWebView->setMinimumWidth((qreal)newViewPortSize.width());
    mWebView->setMaximumWidth((qreal)newViewPortSize.width());
    mWebView->setPreferredWidth((qreal)newViewPortSize.width());
    
    mWebView->setMinimumHeight((qreal)newViewPortSize.height());
    mWebView->setMaximumHeight((qreal)newViewPortSize.height());
    mWebView->setPreferredHeight((qreal)newViewPortSize.height());
    
    
    //Set preferred content size to current screen size
    mWebView->page()->setPreferredContentsSize(mAppRef->mainWindow()->layoutRect().size().toSize());
      
    OstTraceFunctionEntry0( WLANLOGINVIEW_ADJUSTVIEWSIZE_EXIT );
}


/*!
    This function handles urlChanged signal from QGraphicsWebView
    
    @param [in] newUrl New URL
 */
void WlanLoginView::handleUrlChanged(const QUrl& newUrl )
{
    OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLEURLCHANGED_ENTRY);
 
#ifdef OST_TRACE_COMPILER_IN_USE
    QString url = newUrl.toString();
    TPtrC tmp(url.utf16(),url.length() );
    
    OstTraceExt1(
        TRACE_NORMAL,
        WLANLOGINVIEW_HANDLEURLCHANGED_URL_TRACE, 
        "WlanLoginEngine::handleUrlChanged;newUrl=%S",
        tmp);
#endif

    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLEURLCHANGED_EXIT);
}


/*!
    This function handles loadStarted signal from QGraphicsWebView
 */
void WlanLoginView::handleLoadStarted()
{
   OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLELOADSTARTED_ENTRY);
   
   mContentLayout->insertItem(0, mProgressBar);
    mProgressBar->setVisible(true);
    
    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLELOADSTARTED_EXIT);
}


/*!
    This function handles loadProgress signal from QGraphicsWebView

    @param [in] progressValue Indicates page loading progress: 0..100
 */
void WlanLoginView::handleLoadProgress(int progressValue)
{
    OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLELOADPROGRESS_ENTRY);
    
    mProgressBar->setProgressValue(progressValue);
    
    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLELOADPROGRESS_EXIT);
}


/*!
    This function handles loadfinished signal from QGraphicsWebView

    @param [in] status Success status
 */
void WlanLoginView::handleLoadFinished(bool status)
{
   OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLELOADFINISHED_ENTRY);
   
    if(status)
    {   
        adjustViewSize();
    }
   
    mProgressBar->setVisible(false);
    mContentLayout->removeItem(mProgressBar);
  
    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLELOADFINISHED_EXIT);
}


/*!
    This function handles formSumitted signal from WlanLoginWebPage

 */
void WlanLoginView::handleFormSubmitted()
{
    OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLEFORMSUBMITTED_ENTRY);
               
    //TODO: Check why QWebPage sends this signal to times in a row when user pushes submit button in a web page
    
    //User has submitted credentials, let's start internet connectivity test
    emit startIcts();       
    
    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLEFORMSUBMITTED_EXIT);
}


/*!
    This function handles triggered signal from "cancel" button

 */
void WlanLoginView::handleCancelAction()
{
    OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLECANCELACTION_ENTRY);

    emit cancelTriggered();
               
    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLECANCELACTION_EXIT);
}


/*!
    This function handles triggered signal from "next" button

 */
void WlanLoginView::handleNextAction()
{
    OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLENEXTACTION_ENTRY);

    emit nextTriggered();
    
    //Change toolabr action to "Continue" as we are going to background 
    setActiveToolBarAction(Continue);
        
    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLENEXTACTION_EXIT);
}


/*!
    This function handles triggered signal from "continue" button

 */
void WlanLoginView::handleContinueAction()
{
    OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLECONTINUEACTION_ENTRY);

    emit continueTriggered();
    
    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLECONTINUEACTION_EXIT);
}


/*!
    This function handles ictsOk signal from engine

 */
void WlanLoginView::handleIctsOk()
{
    OstTraceFunctionEntry0(WLANLOGINVIEW_HANDLEICTSRESULT_ENTRY);
            
    setActiveToolBarAction(Next);
    
    OstTraceFunctionExit0(WLANLOGINVIEW_HANDLEICTSRESULT_EXIT);
}

/*!
    This function sets active action to the toolbar and hides other actions

     @param [in] newAction Action to be set to toolbar
 */
void WlanLoginView::setActiveToolBarAction(ActionType newAction)
{
    OstTraceFunctionEntry0( WLANLOGINVIEW_SETACTIVETOOLBARACTION_ENTRY );
    
    switch (newAction) {
    case Cancel:
        mNextAction->setEnabled(false);
        mNextAction->setVisible(false);
        
        mContinueAction->setEnabled(false);
        mContinueAction->setVisible(false);
        
        mCancelAction->setVisible(true);
        mCancelAction->setEnabled(true);
        break;
            
    case Next:
        mCancelAction->setEnabled(false);
        mCancelAction->setVisible(false);
        
        mContinueAction->setEnabled(false);
        mContinueAction->setVisible(false);
        
        mNextAction->setVisible(true);
        mNextAction->setEnabled(true);
        break;
            
    case Continue:
        mCancelAction->setEnabled(false);
        mCancelAction->setVisible(false);
        
        mNextAction->setEnabled(false);
        mNextAction->setVisible(false);
        
        mContinueAction->setVisible(true);
        mContinueAction->setEnabled(true);
        break;
            
    default:
        break;
            
    }
    
    OstTraceFunctionExit0( WLANLOGINVIEW_SETACTIVETOOLBARACTION_EXIT );
}
