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
#include "wsfmodel.h"

//  FORWARD DECLARATIONS
class MWsfModelObserver;
class CWsfWLANListActiveWrapper;
class CWsfRefreshScanActiveWrapper;
class CWsfDisconnectActiveWrapper;
class CWsfConnectActiveWrapper;
class CWsfLaunchAiHelperActiveWrapper;
class CWsfConnMonActiveWrapper;

// CLASS DECLARATION

/**
 *  Active object helper class that capsulates active wrapper classes
 *
 *  @lib wsfaiplugin.lib
 *  @since Symbian^3
 * 
 */
class CWsfActiveWrappers : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     * @since Symbian^3
     */
    IMPORT_C ~CWsfActiveWrappers();

    /**
     * Two-phased constructor.
     * @since Symbian^3  
     */
    IMPORT_C static CWsfActiveWrappers* NewL(
            CWsfModel* aModel,
            MWsfModelObserver &aModelObserver );

    /**
     * Two-phased constructor.
     * @since Symbian^3 
     */
    IMPORT_C static CWsfActiveWrappers* NewLC(
            CWsfModel* aModel,
            MWsfModelObserver &aModelObserver );

public:

    /**
     * Starts disconnecting 
     * @since S60 5.2     
     */
    IMPORT_C void Disconnect();

    /**
     * Starts connecting
     * 
     * @since S60 5.2     
     * @param aIapId WLAN IAP id to connect to.
     * @param aConnectOnly ETrue if Connect selected
     * @param aPersistence Persistence property of the IAP
     */
    IMPORT_C void Connect( 
            TUint aIapID, 
            TBool aConnectOnly, 
            TWsfIapPersistence aPersistence );

    /**
     * Starts refresh scan
     * @since Symbian^3    
     */
    IMPORT_C void RefreshScan();

    /**
     * Starts wlan list refreshing
     * @since Symbian^3   
     * @param aStarUp is this called called from plugin start up   
     */
    IMPORT_C void RefreshWLANList( TBool aStarUp );
    
    /**
     * Starts launching of ai helper 
     * @since Symbian^3   
     * @param aWlanInfo The WlanInfo object to be passed to the helper app
     * @param aConnectOnly ETrue if we are only connecting, 
     *                     EFalse if we should also launch the browser  
     * @param aTestAccessPoint ETrue if ICT is executed, 
     *                         EFalse if ICT is not executed 
     */
    IMPORT_C void LaunchHelperApplicationL( TWsfWlanInfo& aInfo, TBool aConnectOnly, 
                                   TBool aTestAccessPoint );

    /**
     * Returns the list of found WLANs. 
     * Ownership not passed.
     * @since Symbian^3
     * @return Array of WLANs
     */
    IMPORT_C CWsfWlanInfoArray* GetWLANList();

    /**
     * Returns the connected wlan network 
     * @since Symbian^3
     * @return TWsfWlanInfo
     */
    IMPORT_C TWsfWlanInfo GetConnectedWLANNetwork();
	
	/**
     * Checks if WLAN is used by browser
     * @since Symbian^3
     */
    IMPORT_C void CheckIsWlanUsedByBrowserL();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CWsfActiveWrappers();

    /**
     * Constructor for performing 2nd stage construction
     */
    void ConstructL( CWsfModel* aModel, MWsfModelObserver &aModelObserver );

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

    /**
     * Pointer to CWsfConnMonActiveWrapper
     * Own.
     */
    CWsfConnMonActiveWrapper* iConnMonActiveWrapper;
	
    };

#endif // WSFACTIVEWRAPPERS_H
