/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef WLANQTUTILSCONNTESTWRAPPER_H_
#define WLANQTUTILSCONNTESTWRAPPER_H_

#include <QObject>
#include <QScopedPointer>

class ConnTestWrapperPrivate;

/**
* ConnTestWrapper class.
*/
class ConnTestWrapper: public QObject
{
    Q_OBJECT
    
    // TestWlanQtUtils is defined as a friend class in order to be able to
    // call event handlers of wrappers.
    friend class TestWlanQtUtils;

public: // Constructor and destructor
    
    /**
    * Default constructor
    */
    ConnTestWrapper(QObject *parent = 0);
    
    /**
    * Destructor
    */
    ~ConnTestWrapper();
    
public: 
    
    /**
    * Called by private wrapper when connectivity test is ready.
    * 
    * @param[in] result Result of internet connectivity test
    */
    void connectivityTestDone(bool result);

public slots:

    /**
    * Starts internet connectivity test.
    * 
    * @param[in] iapId iap id
    * @param[in] netId network id
    */
    void startConnectivityTest(int iapId, int netId);
        
signals:

    /**
    * Signal for emitting connectivity test result.
    * 
    * @param[in] result Result of internet connectivity test
    */
    void connectivityTestResult(bool result);

private:
    
    /**
    * d_ptr pointer to ConnTestWrapperPrivate
    * Owned by ConnTestWrapper object, instantiated in constructor.
    */
    QScopedPointer<ConnTestWrapperPrivate> d_ptr_;
};

#endif /* WLANQTUTILSCONNTESTWRAPPER_H_ */
