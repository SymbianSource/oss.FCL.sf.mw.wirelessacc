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
 * Description:  Implementation of CWsfConnMonActiveWrapper.
 *
 */

#ifndef CWSFCONNMONACTIVEWRAPPER_H
#define CWSFCONNMONACTIVEWRAPPER_H

//  EXTERNAL INCLUDES
#include <e32base.h>	
#include <rconnmon.h>

//  INTERNAL INCLUDES
#include "wsfcommon.h"

//  FORWARD DECLARATIONS
class MWsfModelObserver;

// CONSTANTS
const TInt KBrowserUid = 0x10008D39;

/**
 *  Active object that uses ConnMon API
 *
 *  @lib wsfaiplugin.lib
 *  @since S60 v5.2
 */
class CWsfConnMonActiveWrapper : public CActive
    {
public: // Constructors and destructor

    /**
     * Destructor
     * @since S60 5.2     
     */
    ~CWsfConnMonActiveWrapper();

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfConnMonActiveWrapper* NewL( MWsfModelObserver &aModelObserver );

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfConnMonActiveWrapper* NewLC( MWsfModelObserver &aModelObserver );

public:

    /**
     * Function for making the initial request
     * @since S60 5.2     
     */
    void Start();

private:

    /**
     * constructor
     */
    CWsfConnMonActiveWrapper( MWsfModelObserver &aModelObserver );

    /**
     * Factory function.
     * @since S60 5.2
     */
    void ConstructL();

private: // From CActive
    
    /**
     * @see CActive
     */
    void RunL();

    /**
     * @see CActive
     */
    void DoCancel();

    /**
     * @see CActive
     */
    TInt RunError( TInt aError );

private:
   
    /**
     * States of the active object
     */
    enum CWsfConnMonActiveWrapperState
        {
        EUninitialized, 
        ECountingconnections,
        ECheckingConnections,
        EConnectionsChecked,
        EReady
        };
    
private:

    /**
     * State of the active object
     */
    TInt iState; 

    /**
     * Last request to ConnMon
     */
    TInt iCancelCode;
    
    /**
     * Stores connection count from ConnMon
     */
    TUint iConnectionCount;

    /**
     * Stores connection id from ConnMon
     */
    TUint iConnectionId;
    
    /**
     * Stores bearer type from ConnMon
     */
    TConnMonBearerType iBearerType;
    
    /**
     * Stores client info from ConnMon
     */
    TConnMonClientEnumBuf iClientBuf;
  
    /**
     * Handle to ConnMon
     */
    RConnectionMonitor iConnectionMonitor;
    
    /* Reference to Model Observer */
    MWsfModelObserver &iModelObserver;
        
    };

#endif // CWSFCONNMONACTIVEWRAPPER_H
