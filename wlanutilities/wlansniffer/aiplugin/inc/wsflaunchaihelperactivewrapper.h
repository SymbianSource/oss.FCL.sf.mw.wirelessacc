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
 * Description:  Implementation of CWsfLaunchAiHelperActiveWrapper.
 *
 */

#ifndef CWSFLAUNCHAIHELPERACTIVEWRAPPER_H
#define CWSFLAUNCHAIHELPERACTIVEWRAPPER_H

//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfaicontroller.h"

//  FORWARD DECLARATIONS
class CWsfModel;

/**
 *  Active object launches ai helper
 *
 *  @lib wsfaiplugin.lib
 *  @since S60 v5.2
 */
class CWsfLaunchAiHelperActiveWrapper : public CActive
    {
public: // Constructors and destructor

    /**
     * Destructor
     * @since S60 5.2     
     */
    ~CWsfLaunchAiHelperActiveWrapper();

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfLaunchAiHelperActiveWrapper* NewL( CWsfModel* aModel,
                                            TWsfAiController &aController );

    /**
     * Two-phased constructor.
     * @since S60 5.2     
     */
    static CWsfLaunchAiHelperActiveWrapper* NewLC( CWsfModel* aModel, 
                                             TWsfAiController &aController );

public:
    /**
     * Function for making the initial request
     * @since S60 5.2
     * @param aWlanInfo The WlanInfo object to be passed to the helper app
     * @param aConnectOnly ETrue if we are only connecting, 
     *                     EFalse if we should also launch the browser   
     * @param aTestAccessPoint ETrue if ICT is executed, 
     *                         EFalse if ICT is not executed  
     */
    void Start( TWsfWlanInfo& aInfo, TBool aConnectOnly, 
                TBool aTestAccessPoint );

private:
    
    /**
     * constructor
     */
    CWsfLaunchAiHelperActiveWrapper();

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
        EError // Error condition
        };

private:

    /**
     * State of the active object
     */
    TInt iState; 

    /**
     * Reference to Model
     */
    CWsfModel* iModel;

    /**
     * Pointer to TWsfAiController
     */
    TWsfAiController *iController;
    
    /**
    * Handle to the ai helper exit code property value
    */
    RProperty iAiHelperAppExitCode;
    
    /**
    * Handle to the ai helper iap id property value
    */
    RProperty iAiHelperAppIapId;
    
    /**
    * Cache of the used info
    */
    TWsfWlanInfo iUsedInfo;
    
    /**
    * Indicates if "Connect" or "Start Web browsing" is selected.
    */
    TBool iConnectOnly;
    
    /**
    * Indicates if ICT needs to be executed
    */
    TBool iTestAccessPoint;
    
    /**
    * Indicates if ai helper was stopped
    */
    TBool iStoppingHelper;
    
    /**
    * Timer to wait for ai helper to close
    */
    RTimer iTimer; 

    };

#endif // CWSFLAUNCHAIHELPERACTIVEWRAPPER_H
