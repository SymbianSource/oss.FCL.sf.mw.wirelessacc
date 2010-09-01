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
 * Description:  Implementation of CWsfActiveWrappers.
 *
 */



#ifndef WSFACTIVEWRAPPERS_H
#define WSFACTIVEWRAPPERS_H

//  EXTERNAL INCLUDES
#include <e32std.h>
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfcommon.h"
#include "wsfaicontroller.h"
#include "wsfmodel.h"

//  FORWARD DECLARATIONS
class CWsfWLANListActiveWrapper;
class CWsfRefreshScanActiveWrapper;
class CWsfDisconnectActiveWrapper;
class CWsfConnectActiveWrapper;
class CWsfLaunchAiHelperActiveWrapper;

// CLASS DECLARATION

/**
 *  Active object helper class that capsulates active wrapper classes
 *
 *  @lib wsfaiplugin.lib
 *  @since S60 v5.2
 * 
 */
class CWsfActiveWrappers : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     * @since S60 5.2 
     */
    ~CWsfActiveWrappers();

    /**
     * Two-phased constructor.
     * @since S60 5.2  
     */
    static CWsfActiveWrappers* NewL( CWsfModel* aModel,
            TWsfAiController &aController );

    /**
     * Two-phased constructor.
     * @since S60 5.2 
     */
    static CWsfActiveWrappers* NewLC(CWsfModel* aModel,
            TWsfAiController &aController );

public:

    /**
     * Starts disconnecting 
     * @since S60 5.2     
     */
    void Disconnect();

    /**
     * Starts connecting
     * @since S60 5.2     
     * @param aIapId WLAN IAP id to connect to.
     * @param aConnectOnly ETrue if Connect selected
     * @param aPersistence Persistence property of the IAP
     */
    void Connect( TUint aIapID, TBool aConnectOnly, 
                  TWsfIapPersistence aPersistence );

    /**
     * Starts refresh scan
     * @since S60 5.2     
     */
    void RefreshScan();

    /**
     * Starts wlan list refreshing
     * @since S60 5.2   
     * @param aStarUp is this called called from plugin start up   
     */
    void RefreshWLANList( TBool aStarUp );
    
    /**
     * Starts launching of ai helper 
     * @since S60 5.2   
     * @param aWlanInfo The WlanInfo object to be passed to the helper app
     * @param aConnectOnly ETrue if we are only connecting, 
     *                     EFalse if we should also launch the browser  
     * @param aTestAccessPoint ETrue if ICT is executed, 
     *                         EFalse if ICT is not executed 
     */
    void LaunchHelperApplicationL( TWsfWlanInfo& aInfo, TBool aConnectOnly, 
                                   TBool aTestAccessPoint );

    /**
     * Returns the list of found WLANs. 
     * Ownership not passed.
     * @since S60 5.2
     * @return Array of WLANs
     */
    CWsfWlanInfoArray* GetWLANList();

    /**
     * Returns the connected wlan network 
     * @since S60 5.2
     * @return TWsfWlanInfo
     */
    TWsfWlanInfo GetConnectedWLANNetwork();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CWsfActiveWrappers();

    /**
     * Constructor for performing 2nd stage construction
     */
    void ConstructL( CWsfModel* aModel, TWsfAiController &aController );

private:
    // Data

    /**
     * Pointer to CWsfWLANListActiveWrapper
     * Own.
     */
    CWsfWLANListActiveWrapper* iWLANListActiveWrapper;

    /**
     * Pointer to CWsfRefreshScanActiveWrapper
     * Own.
     */
    CWsfRefreshScanActiveWrapper* iRefreshScanActiveWrapper;

    /**
     * Pointer to CWsfDisconnectActiveWrapper
     * Own.
     */
    CWsfDisconnectActiveWrapper* iDisconnectActiveWrapper;

    /**
     * Pointer to CWsfConnectActiveWrapper
     * Own.
     */
    CWsfConnectActiveWrapper* iConnectActiveWrapper;

    /**
     * Pointer to CWsfLaunchAiHelperActiveWrapper
     * Own.
     */
    CWsfLaunchAiHelperActiveWrapper* iLaunchAiHelperActiveWrapper;

    };

#endif // WSFACTIVEWRAPPERS_H