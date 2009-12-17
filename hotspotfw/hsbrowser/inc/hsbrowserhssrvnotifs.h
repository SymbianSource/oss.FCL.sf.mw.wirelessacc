/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Implements notifications interface from Hotspot server in order
*                to receive wlan connection closed notification.
*
*/



#ifndef HSBROWSERHSSRVNOTIFS_H
#define HSBROWSERHSSRVNOTIFS_H

//  INCLUDES
#include <e32base.h>
#include <ictscommon.h>
#include "hsssrvnotifications.h"
#include <wlanmgmtcommon.h>
#include <wlanmgmtinterface.h>

// FORWARD DECLARATION
class CHsBrowserContainer;

// CLASS DECLARATION
/**
* CHsBrowserHsSrvNotifs class.
* Class implements notifications interface for Hotspot server in order
* to receive wlan connection closed notification.
*/
class CHsBrowserHsSrvNotifs : public CBase, public MWlanMgmtNotifications
	{
	
	    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aContainer Pointer to container of browser application
        */
        static CHsBrowserHsSrvNotifs* NewL( CHsBrowserContainer* aContainer );

        /**
        * Destructor.
        */
        ~CHsBrowserHsSrvNotifs();

    public: // from MWlanMgmtNotifications
    
      	/**
        * Connection state has changed .
        */
    	virtual void ConnectionStateChanged( TWlanConnectionMode aNewState );
    
	    /**
	    * BSSID has changed (i.e. AP handover).
	    */
	    virtual void BssidChanged( TWlanBssid& /* aNewBSSID */ ) {};
	
	    /**
	    * Connection has been lost.
	    */
	    virtual void BssLost() {};
	
	    /**
	    * Connection has been regained.
	    */
	    virtual void BssRegained() {};
	
	    /**
	    * New networks have been detected during scan.
	    */
	    virtual void NewNetworksDetected() {};
	
	    /**
	    * One or more networks have been lost since the last scan.
	    */
	    virtual void OldNetworksLost() {};
	
	    /**
	    * The used transmit power has been changed.
	    * @param aPower The transmit power in mW.
	    */
	    virtual void TransmitPowerChanged( TUint /* aPower */ ) {};
	    
	    /**
	    * Received signal strength level has been changed.
	    * @param aRssClass specifies the current class of the received signal
	    * @param aRss RSS level in absolute dBm values.
	    */
	    virtual void RssChanged(TWlanRssClass /* aRssClass */, TUint /* aRss */ ) {};

                        
    private:
    
        /**
        * C++ default constructor.
        *
        * @param aContainer Pointer to container of browser application.
        */
        CHsBrowserHsSrvNotifs( CHsBrowserContainer* aContainer );
        
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private:
    
 	    /**
        * Container of hotspot browser application.
        */
        CHsBrowserContainer* iContainer;
	};

#endif // HSBROWSERHSSRVNOTIFS_H
            
// End of File

