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
* Description:   Contains implementation for HTTP::HEAD test
*
*/



#ifndef ICTSHTTPHANDLER_H
#define ICTSHTTPHANDLER_H

#include <ecom/ecom.h>
#include <es_enum.h>
#include <http.h>

#include "ictscommon.h"

class CIctsEngine;

/**
 *  Handles HTTP related things.
 *
 *  Sends HTTP::HEAD and interact with HTTP FW. Returns results to 
 *  caller.
 *
 *  @lib ictsclientinterface.lib
 */
class CIctsHttpHandler : public CTimer, public MHTTPTransactionCallback 
    {
    
    public:

        /**
        * Static constructor.
        */    
        static CIctsHttpHandler* NewL( CIctsEngine& aOwner,
                                        TInt aHttpResponseTime );
    
        /**
        * Destructor.
        */
        virtual ~CIctsHttpHandler();
    
        /**
        * Send HTTP::HEAD request 
        *
        * @param aIPAddress IP address
        * @param aIapID IAP id
        * @param aNetworkId Network id
        * @return KErrNone if successful
        */
        TInt SendHttpRequestL( TDesC8& aIPAddress, 
                            TUint32 aIapID, 
                             TUint32 aNetworkId );
        
        /**
        * Cancels HTTP::HEAD request 
        *
        * @return None
        */
        void CancelHttpRequestL();
    
    public: // from MHTTPTransactionCallback

        /**
        * Callback for HTTP FW to call when HTTP events occur
        *
        * @param aTransaction HTTP transaction
        * @param aEvent HTTP event
        * @return None
        */
        virtual void MHFRunL( RHTTPTransaction aTransaction, 
                                const THTTPEvent& aEvent);
    
        /**
        * Callback for HTTP FW to call when HTTP errors occur
        *
        * @param aError error code
        * @param aTransaction HTTP transaction
        * @param aEvent HTTP event
        * @return None
        */                        
        virtual TInt MHFRunError( TInt aError, 
                                    RHTTPTransaction aTransaction, 
                                      const THTTPEvent& aEvent);
    

    protected:  // Functions from base classes
        
        /**
        * From CActive Handles an active object's request completion event.
        */
        void RunL();
    
    private:
    
        /**
        * C++ default constructor.
        *
        * @param aOwner Owner of this instance
        * @param aHttpResponseTime Max time for response
        */
        CIctsHttpHandler( CIctsEngine& aOwner, TInt aHttpResponseTime );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
        /**
        * Set HTTP connection info 
        *
        * @param aConnection Network connection
        * @param aSocketServ Socket server
        * @return None
        */
        void SetHttpConnectionInfoL( RConnection& aConnection, 
                                     RSocketServ& aSocketServ );
        /**
        * Send HTTP request 
        *
        * @param aIp IP address
        * @return None
        */
        void SendHttpGetL( TDesC8& aIp ); 

        /**
        * Check http prefix in URL/IP address
        *
        * @param aIPAddress URL
        * @return ETrue if found, else EFalse
        */ 
        TBool CheckHttp( TDesC8& aIPAddress );
    
    private: // data

        /**
        * Handle to HTTP session
        * Own.  
        */
        RHTTPSession iHttpSession;
    
        /**
        * Handle to HTTP transaction
        * Own.  
        */
        RHTTPTransaction iHttpTransaction;

        /**
        * Handle to socket server
        * Own.  
        */
        RSocketServ iSocketServ;
    
        /**
        * Handle to connection
        * Own.  
        */
        RConnection iConnection;
    
        /**
        * String from HTTP FW
        */
        TBuf< KHttpResponseLength > iString;
    
    
        /**
        * Reference of owner class
        * Not Own.  
        */
        CIctsEngine& iOwner;
    
        /**
        * Waiting time of HTTP response 
        */
        TInt iHttpResponseTime;
        
        /**
        * Is attached to connection
        */
        TBool iAttachDone;
    };

#endif // ICTSHTTPHANDLER_H
