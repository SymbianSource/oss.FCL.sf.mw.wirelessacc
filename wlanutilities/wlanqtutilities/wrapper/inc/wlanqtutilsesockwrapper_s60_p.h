/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef ESOCKWRAPPER_S60_P_H_
#define ESOCKWRAPPER_S60_P_H_

// INCLUDES
#include <es_sock.h>

QT_BEGIN_NAMESPACE

// CLASS DECLARATION

/**
* EsockWrapperPrivate class.
*/
class EsockWrapperPrivate: public CActive
{

public:  // Constructor and destructor
    
    /**
    * Default constructor
    */
    EsockWrapperPrivate(EsockWrapper *aWrapper);
    
    /**
     * Destructor
     */
    ~EsockWrapperPrivate();

public: // New functions
    
    /**
    * connectIap
    * Starts connection creation.
    */
    void connectIap(int aIapId);
    
    /**
    * dsiconnectIap
    * Stops connection.
    */
    void disconnectIap();

private: // Functions from base class
    
    /**
    * RunL
    * Called when the scheduled function ends.
    */
    void RunL();
    
    /**
    * DoCancel
    * Cancels operations.
    */
    void DoCancel();
        
private: // Data
    
    /**
    * q_ptr pointer to ConnectionWrapper
    * Owned by ConnectionWrapperPrivate object, instantiated in
    * constructor.
    */
    EsockWrapper *q_ptr;
    
    RSocketServ iSocketServer;
    RConnection iConnection;
};

QT_END_NAMESPACE

#endif /*ESOCKWRAPPER_S60_P_H_*/

// End of file
