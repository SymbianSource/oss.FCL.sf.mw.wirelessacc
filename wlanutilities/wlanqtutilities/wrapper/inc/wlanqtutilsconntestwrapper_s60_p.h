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

#ifndef WLANQTUTILSCONNTESTWRAPPER_S60_P_H_
#define WLANQTUTILSCONNTESTWRAPPER_S60_P_H_

#include <ictsclientinterface.h>

QT_BEGIN_NAMESPACE

/**
* ConnTestWrapperPrivate class.
*/
class ConnTestWrapperPrivate: public MIctsObserver
{

    // TestWlanQtUtils is defined as a friend class in order to be able to
    // call event handlers of wrappers.
    friend class TestWlanQtUtils;

public:
    
    /**
    * Default constructor
    */
    ConnTestWrapperPrivate(ConnTestWrapper *aWrapper);
    
    /**
    * Destructor
    */
    ~ConnTestWrapperPrivate();

public: // From MICTSObserver
    
    /**
    * Called when internet connectivity tests are being done.
    * 
    * @param[in] aResult Result of connectivity test
    * @param[in] aString String containing possible HTML response
    */
    void ConnectivityObserver( TIctsTestResult aResult, const TDesC& aString );

public: 
    
    /**
    * Starts internet connectivity test wrapped.
    * 
    * @param[in] iapId iap id
    * @param[in] netId network id
    */
    void startConnectivityTest( int aIapId, int aNetId );
    
    /**
    * Starts the actual internet connectivity test.
    * 
    * @param[in] iapId iap id
    * @param[in] netId network id
    */
    void startConnectivityTestL( TInt aIapId, TInt aNetId );

private:
    
    /**
    * Pointer to ConnTestWrapper
    * Owned by ConnTestWrapperPrivate object, instantiated in constructor.
    */
    ConnTestWrapper *q_ptr;
    
    /**
    * Pointer to Internet Connectivity Test Service
    * Owned by ConnTestWrapperPrivate.
    */
    CIctsClientInterface* iIct;

};

QT_END_NAMESPACE

#endif /* WLANQTUTILSCONNTESTWRAPPER_S60_P_H_ */
