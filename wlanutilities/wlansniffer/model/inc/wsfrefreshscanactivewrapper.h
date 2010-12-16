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
 * Description:  Implementation of CWsfRefreshScanActiveWrapper.
 *
 */

#ifndef CWSFREFRESHSCANACTIVEWRAPPER_H
#define CWSFREFRESHSCANACTIVEWRAPPER_H

//  EXTERNAL INCLUDES
#include <e32base.h>	

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CWsfModel;

/**
 *  Active object that requests scans
 *
 *  @lib wsfaiplugin.lib
 *  @since S60 v5.2
 */
class CWsfRefreshScanActiveWrapper : public CActive
    {
public:
    // Constructors and destructor

    /**
     * Destructor
     * @since S60 5.2     
     */
    ~CWsfRefreshScanActiveWrapper();

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfRefreshScanActiveWrapper* NewL(CWsfModel* aModel);

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfRefreshScanActiveWrapper* NewLC(CWsfModel* aModel);

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
    CWsfRefreshScanActiveWrapper();

    /**
     * Factory function.
     * @since S60 5.0
     * @param aModel CWsfModel pointer
     */
    void ConstructL(CWsfModel* aModel);

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
    TInt RunError(TInt aError);

private:
    /**
     * States of the active object
     */
    enum CWsfRefreshScanActiveWrapperState
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
     * Request result
     */
    TPckgBuf<TBool> iPckg;

    /**
     * Reference to Model
     */
    CWsfModel* iModel;

    };

#endif // CWSFREFRESHSCANACTIVEWRAPPER_H
