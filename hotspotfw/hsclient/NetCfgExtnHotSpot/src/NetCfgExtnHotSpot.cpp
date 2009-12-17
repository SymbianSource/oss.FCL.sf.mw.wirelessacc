/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Implements Network Config Extension for HotSpot
*
*/

// INCLUDE FILES
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <nifman_internal.h>
#endif
#include "NetCfgExtnHotSpot.h"
#include "implementationproxy.h"
#include "am_debug.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// NewL
// -----------------------------------------------------------------------------
//
CNetworkConfigExtensionHotSpot* CNetworkConfigExtensionHotSpot::NewL( 
                                                            TAny* aMNifIfNotify )
	{
	MNifIfNotify* nifIfNotify = reinterpret_cast<MNifIfNotify*>( aMNifIfNotify );
	CNetworkConfigExtensionHotSpot* pDaemon = 
	    new( ELeave )CNetworkConfigExtensionHotSpot( *nifIfNotify );
	CleanupStack::PushL( pDaemon );
	pDaemon->ConstructL();
	CleanupStack::Pop( pDaemon );
	return pDaemon;
	}
	
// -----------------------------------------------------------------------------
// ConstructL
// -----------------------------------------------------------------------------
//    
void CNetworkConfigExtensionHotSpot::ConstructL()
	{
	DEBUG( "CNetworkConfigExtensionHotSpot::ConstructL()" );
	CNetworkConfigExtensionBase::ConstructL();
	iNotAuthenticated = ETrue;
	iNotDeregistered = ETrue;
	iHotspotConnect = KErrNotFound;
	DEBUG( "CNetworkConfigExtensionHotSpot::ConstructL() Done" );
	}

// -----------------------------------------------------------------------------
// SendIoctlMessageL 
// Forwards Ioctl request to the daemon and activates the AO to wait for response
// -----------------------------------------------------------------------------
//
void CNetworkConfigExtensionHotSpot::SendIoctlMessageL( const ESock::RLegacyResponseMsg& aMessage )

	{
  	TInt name = aMessage.Int1();
  	if ( aMessage.Int0() != KCOLConfiguration )
  	    {
  	    User::Leave( KErrNotSupported );
  	    }
  	else 
  		{
  		switch ( name ) 
  			{
  	    	case KConnAddrRelease:
  	    	case KConnAddrRenew:
  	    	case KConnSetDhcpRawOptionData:
		    	if ( aMessage.HasCapability(ECapabilityNetworkControl, "NetCfgExtnDhcp" ) == EFalse )
				    {
				    User::Leave (KErrPermissionDenied );
				}
  			}   
  		}
	CNetworkConfigExtensionBase::SendIoctlMessageL( aMessage );
	}
	
// -----------------------------------------------------------------------------
// ~CNetworkConfigExtensionHotSpot
// -----------------------------------------------------------------------------
//
CNetworkConfigExtensionHotSpot::~CNetworkConfigExtensionHotSpot()

	{
	DEBUG( "CNetworkConfigExtensionHotSpot::~CNetworkConfigExtensionHotSpot()" );
	}	

// -----------------------------------------------------------------------------
// ImplementationTable
// -----------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY(0x10282ECA, CNetworkConfigExtensionHotSpot::NewL)
    };

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

    return ImplementationTable;
    }


// -----------------------------------------------------------------------------
// RunL
// -----------------------------------------------------------------------------
//
void CNetworkConfigExtensionHotSpot::RunL()
	{
	DEBUG( "CNetworkConfigExtensionHotSpot::RunL()" );
	if (iLastGenericProgressStage == KConfigDaemonStartingRegistration)
		{
	
		if(iNotAuthenticated)
		    {	
		    DEBUG( "CNetworkConfigExtensionHotSpot::RunL() not auth" );
            TUint iapId = iConnectionInfoBuf().iIapId;
	    	TUint networkId = iConnectionInfoBuf().iNetId;
	    	
	    	iHotspotConnect = iClient.Connect();
	    	
	    	if ( KErrNone == iHotspotConnect )
	    		{
	    		iClient.StartLogin( iapId, networkId, iStatus);
	    		SetActive();
	    		}
	    	else
	    		{
	    		CNetworkConfigExtensionBase::RunL();
	    		}
			iNotAuthenticated = EFalse;
            }
		else
		    {
		    DEBUG( "CNetworkConfigExtensionHotSpot::RunL() close" );
			CNetworkConfigExtensionBase::RunL();
		    }
		
		}
    else if (iLastGenericProgressStage == KConfigDaemonStartingDeregistration)
        {
        if(iNotDeregistered)
		    {
		    DEBUG( "CNetworkConfigExtensionHotSpot::RunL() dereg" );
            TUint iapId = iConnectionInfoBuf().iIapId;
	    	TUint networkId = iConnectionInfoBuf().iNetId;
	    	
	    	if ( KErrNone == iHotspotConnect )
	    		{
	    		iClient.CloseConnection( iapId, iStatus);
	    		SetActive();
	    		}
	    	else
	    		{
	    		CNetworkConfigExtensionBase::RunL();
	    		}
			iNotDeregistered = EFalse;
		    }
        else
		    {
		    DEBUG( "CNetworkConfigExtensionHotSpot::RunL() dereg close" );
		    iClient.Close();
		    CNetworkConfigExtensionBase::RunL();
		    }
        }
	else // original =========================================================
	    {
	    DEBUG( "CNetworkConfigExtensionHotSpot::RunL() original" );
		CNetworkConfigExtensionBase::RunL();
	    }
	}
	
// -----------------------------------------------------------------------------
// DoCancel
// -----------------------------------------------------------------------------
//
void CNetworkConfigExtensionHotSpot::DoCancel()
    {
    DEBUG( "CNetworkConfigExtensionHotSpot::RunL() DoCancel" );
    iClient.CancelLogin();
    }

// end of file

