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
 * Description:  Implementation of CWsfWLANListActiveWrapper.
 *
 */

#ifndef CWSFWLANLISTACTIVEWRAPPER_H
#define CWSFWLANLISTACTIVEWRAPPER_H

//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfaicontroller.h"

//  FORWARD DECLARATIONS
class CWsfModel;
class CWsfWlanInfoArray;

/**
 *  Active object that fetches wlan list data
 *
 *  @lib wsfaiplugin.lib
 *  @since S60 v5.2
 */
class CWsfWLANListActiveWrapper : public CActive
    {
public: // Constructors and destructor

    /**
     * Destructor
     * @since S60 5.2     
     */
    ~CWsfWLANListActiveWrapper();

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfWLANListActiveWrapper* NewL( CWsfModel* aModel,
                                            TWsfAiController &aController );

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfWLANListActiveWrapper* NewLC( CWsfModel* aModel, 
                                             TWsfAiController &aController );

public:
    /**
     * Function for making the initial request
     * @since S60 5.2
     * @param aStarUp is start called from plugin start up    
     */
    void Start( TBool aStarUp );

    /**
     * Returns the list of found WLANs. 
     * Ownership not passed.
     * @since S60 5.2
     * @return Array of WLANs
     */
    CWsfWlanInfoArray* GetWlanList();
    
    /**
     * Returns the connected WLAN network
     * @since S60 5.2
     * @return TWsfWlanInfo
     */
    TWsfWlanInfo GetConnectedWLANNetwork();

private:
    
    /**
     * constructor
     */
    CWsfWLANListActiveWrapper();

    /**
     * Factory function.
     * @since S60 5.2
     * @param aModel Whether to react to screensaver events 
     * @param aController Whether to react to screensaver events
     */
    void ConstructL( CWsfModel* aModel, TWsfAiController &aController );

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
    enum TCWsfWLANListActiveWrapperState
        {
        EUninitialized, // Uninitialized
        EInitialized, // Initalized
        EProcessWLANListData, // Create wlan array from results
        EGetConnectedNetwork, // Request connected network
        EError
        // Error condition
        };

private:

    /**
     * State of the active object
     */
    TInt iState; // State of the active object

    /**
     * Needed size to allocate result data
     */
    TPckgBuf<TUint> iPckgNeededSize;

    /**
     * Data size in engine
     */
    TPckgBuf<TUint> iPckgAllocatedSize;
    
    /**
     * Connected network request result
     */
    TPckgBuf<TBool> iPckg;

    /**
     * Reference to Model
     */
    CWsfModel* iModel;

    /**
     * Array to store WLAN scan results. Owned.
     */
    CWsfWlanInfoArray* iArray;

    /**
     * TPtr to wlan data
     */
    TPtr8 iPtr;

    /**
     * Wlan data buffer
     */
    HBufC8* iBuffer;

    /**
     * Pointer to TWsfAiController
     */
    TWsfAiController *iController;

    /**
     * Is this object called from plugin startup
     */
    TBool iStartUp;

    /**
     * Retries to fetch data
     */
    TInt iRetriesLeft;
    
    /**
    * Cache of the connected WLAN data
    */
    TWsfWlanInfo iConnectedWlan;

    };

#endif // CWSFWLANLISTACTIVEWRAPPER_H
