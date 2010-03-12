/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:  Implementation of CWsfConnectActiveWrapper.
 *
 */

#ifndef CWSFCONNECTACTIVEWRAPPER_H
#define CWSFCONNECTACTIVEWRAPPER_H

//  EXTERNAL INCLUDES
#include <e32base.h>	

//  INTERNAL INCLUDES
#include "wsfaicontroller.h"
#include "wsfcommon.h"

//  FORWARD DECLARATIONS
class CWsfModel;

/**
 *  Active object that connects WLAN
 *
 *  @lib wsfaiplugin.lib
 *  @since S60 v5.2
 */
class CWsfConnectActiveWrapper : public CActive
    {
public:
    // Constructors and destructor

    /**
     * Destructor
     * @since S60 5.2     
     */
    ~CWsfConnectActiveWrapper();

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfConnectActiveWrapper* NewL( CWsfModel* aModel );

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfConnectActiveWrapper* NewLC( CWsfModel* aModel );

public:

    /**
     * Function for making the initial request
     * @since S60 5.2     
     */
    void Start( TUint aIapID, TWsfIapPersistence aPersistence );

private:

    /**
     * constructor
     */
    CWsfConnectActiveWrapper();

    /**
     * Factory function.
     * @since S60 5.2
     * @param aModel CWsfModel pointer
     */
    void ConstructL( CWsfModel* aModel );

private:
    // From CActive
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
    enum CWsfConnectActiveWrapperState
        {
        EUninitialized, // Uninitialized
        EInitialized, // Initalized
        EError
        // Error condition
        };

private:

    /**
     * State of the active object
     */
    TInt iState; // State of the active object

    /**
     * Result
     */
    TPckgBuf<TBool> iPckg;

    /**
     * Reference to Model
     */
    CWsfModel* iModel;
    
    /**
     * IAP ID
     */
    TUint iIapID;
    
    /**
     * Persistence value of IAP
     */
    TWsfIapPersistence iPersistence;

    };

#endif // CWSFCONNECTACTIVEWRAPPER_H
