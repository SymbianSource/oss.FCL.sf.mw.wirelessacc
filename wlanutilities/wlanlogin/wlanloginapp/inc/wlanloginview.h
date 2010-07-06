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
* 
*/

#ifndef WLANLOGINVIEW_H
#define WLANLOGINVIEW_H

// System includes
#include <QUrl>
#include <HbView>
#include <HbScrollArea>
#include <QGraphicsLinearLayout>

// User includes
#include "wlanloginapplication.h"
#include "wlanloginwebpage.h"
#include "ictsqtwrapper.h"

// Forward declarations
class HbProgressBar;
class WlanLoginWebView;
class QNetworkAccessManager;
class HbWidget;

// External data types

// Constants

// Class declaration
class WlanLoginView : public HbView
    {
    Q_OBJECT
    
public:
    WlanLoginView(WlanLoginApplication* appref);
    virtual ~WlanLoginView();
    
    /*!
        Enumeration type for current toolbar action in use
    */ 
    enum ActionType {
        Cancel,     //!< "Cancel" button
        Next,       //!< "Next" button
        Continue    //!< "Continue" button
    };
    void setActiveToolBarAction(ActionType newAction);
    
private slots:

    void handleConnectionReady(QUrl url);    
    void handleOrientationChanged(Qt::Orientation orientation);
    void adjustViewSize();
    
    void handleUrlChanged(const QUrl& newUrl);    
    void handleLoadStarted();
    void handleLoadProgress(int progressValue);
    void handleLoadFinished(bool status );

    void handleFormSubmitted();
    void handleCancelAction();
    void handleNextAction();
    void handleContinueAction();
    
    void handleIctsOk();
    
signals:
    void cancelTriggered();
    void nextTriggered();
    void continueTriggered();
    void startIcts();
    
private: // Data
    
    // Not owned data
    WlanLoginApplication* mAppRef;  //!< pointer to application instance 

    // Owned data

    QGraphicsLinearLayout* mMainLayout; //!< pointer to main layout
    QGraphicsLinearLayout* mContentLayout; //!< pointer to scroall area content layout
    
    HbProgressBar* mProgressBar;    //!< pointer to HB progressbar  
    HbScrollArea* mScrollArea;      //!< pointer to HB scroll area
    HbWidget* mScrollAreaContent;
    WlanLoginWebView* mWebView;     //!< pointer to web view
    HbToolBar* mToolBar;            //!< pointer to HB toolbar
    HbAction* mCancelAction;        //!< pointer to "cancel" action 
    HbAction* mNextAction;          //!< pointer to "next" action
    HbAction* mContinueAction;      //!< pointer to "continue" action

    };

#endif // WLANLOGINWEBVIEW_H
