/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    NIFMAN configuration 
*
*/




#ifndef __NET_CFG_EXTN_HOTSPOT_H__
#define __NET_CFG_EXTN_HOTSPOT_H__

//  INCLUDES
#include <e32base.h>
#include <comms-infras/networkconfigextensionbase.h>
#include <nifman.h>
#include "hssinterface.h"

// CLASS DECLARATION
/**
*  ECom plugin of NIFMAN configuration
*
*  @lib NetCfgExtnHotSpot.lib
*  @since S60 5.0
*/
class CNetworkConfigExtensionHotSpot : public CNetworkConfigExtensionBase
	{

public:

    /**
    * Two-phased constructor.
    */
  	static CNetworkConfigExtensionHotSpot* NewL( TAny* aMNifIfNotify );
   	
   	/**
    * Destructor.
    */
   	virtual ~CNetworkConfigExtensionHotSpot();

	/**
    * From CNetworkConfigExtensionBase
    * @since S60 5.0
    * @param aMessage the message
    * @return None
    */
   	void SendIoctlMessageL( const ESock::RLegacyResponseMsg& aMessage );

protected:

    /**
    * C++ default constructor.
    */
   	CNetworkConfigExtensionHotSpot( MNifIfNotify& aNifIfNotify );
  	
  	/**
    * By default Symbian 2nd phase constructor is private.
    */
  	void ConstructL();
   	
    /**
    * From CTimer. Waits for StartLogin() completion from Hotspot Server 
    * @since S60 5.0
    * @return None
    */
   	virtual void RunL();
   	
   	/**
    * From CTimer. Implements cancel routines.
    * @since S60 5.0
    * @return None
    */
   	virtual void DoCancel();

    // Authentication flag  	  		
	TBool iNotAuthenticated;
	
	// Authentication flag  	  		
	TBool iNotDeregistered;
	
	// Flagging if Connect() to Hotspot done successfully
	TInt iHotspotConnect;
 	
private:
    
    // Hotspot client interface 
    RHssInterface iClient;
 	};

    /**
    * CNetworkConfigExtensionBase - constructor
    * @param aNifIfNotify - client of the control
    */
    inline CNetworkConfigExtensionHotSpot::CNetworkConfigExtensionHotSpot( MNifIfNotify& aNifIfNotify ) :
    CNetworkConfigExtensionBase( aNifIfNotify )
	{
	}

#endif

