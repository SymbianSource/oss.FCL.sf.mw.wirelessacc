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
* Description:  Class header for CWsfServer
*
*/



#ifndef C_WSFSERVER_H
#define C_WSFSERVER_H

//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfservercloseradapter.h"


//  FORWARD DECLARATIONS
class CWsfEngine;
class CWsfSession;


//  CLASS DEFINITION
/**
* CWsfServer is the server class that realises the necessary server functions 
*
* @since S60 5.0
* @lib wsfserver.exe
*/
NONSHARABLE_CLASS( CWsfServer ): public CPolicyServer
    {
    private:
        
        /**
        * CWsfServerCloser is an auxiliary class that is responsible for 
        * closing the server when there is no need for it any more
        *
        * @since S60 5.0
        * @lib wsfserver.exe
        */
        NONSHARABLE_CLASS( CWsfServerCloser ): public CTimer, 
                                               public MWsfServerCloserAdapter
            {
            public:
                /**
                * Factory function.
                * @since S60 5.0
                * @return Server closer instance
                */
                static CWsfServerCloser* NewL();
                
                /**
                * Destructor.
                */
                ~CWsfServerCloser();
            
            public:     // from MWsfServerCloserAdapter
            
                /**
                * Sets whether the server closer should wait for the 
                * termination of the owned connection
                * @since S60 5.0
                * @param aWait If ETrue, the server closer will wait.
                */
                void WaitForOwnedConnection( TBool aWait );

                /**
                * Sets whether the server closer should wait for the 
                * termination of the browser
                * @since S60 5.0
                * @param aWait If ETrue, the server closer will wait.
                */
                void WaitForBrowserExit( TBool aWait );

                /**
                * Sets whether the server closer should wait for the 
                * clients of the owned connection to detach
                * @since S60 5.0
                * @param aWait If ETrue, the server closer will wait.
                */
                void WaitForClients( TBool aWait );
                
            private:
                
                /**
                * Constructor.
                * @since S60 5.0
                */
                CWsfServerCloser();
                
                /**
                * Second-phase constructor.
                * @since S60 5.0
                */
                void ConstructL();
                
                           
            private:    // from CActive

                /**
                * Handles an active object's request completion event.
                * @since S60 5.0
                */
                void RunL();
                

            private:    // new methods
                /** 
                * Start the closing timer
                */    
                void StartClosureWait();
                
            private:
                /**
                * Indicates whether the server should wait for the owned
                * connection to terminate.
                */
                TBool iWaitForOwnedConnection;
                
                /**
                * Indicates whether the server should wait for the browser
                * to exit.
                */
                TBool iWaitForBrowserExit;

                /**
                * Indicates whether the server should wait for the clients
                * of the owned connection to detach.
                */
                TBool iWaitForClients;
            };

    public:     // Constructors and destructor
        /**
        * Factory function.
        * @since S60 5.0
        * @return Server instance
        */        
        static CWsfServer* NewLC();
        
        /**
        * Destructor.
        * @since S60 5.0
        */
        ~CWsfServer();

    private:    // Constructors
        /**
        * Constructor
        * @since S60 5.0
        * @param aPriority Active object priority value
        */
        CWsfServer( TInt aPriority );
        
        /**
        * Second-phase constructor.
        * @since S60 5.0
        */
        void ConstructL();


    private:    // from CActive
        /**
        * Handles a leave occurring in RunL().
        * @since S60 5.0
        * @param aError Leave code
        * @return aError
        */    
        TInt RunError( TInt aError );


    private:  // from CPolicyServer
    
        /**
        * Performs a custom security check for IPCs marked with 
        * TSpecialCase::ECustomCheck.
        * @since S60 5.0
        * @param aMsg The message to check. 
        * @param aAction A reference to the action to take if the security 
        *                check fails.
        * @param aMissing A reference to the list of security attributes 
        *                 missing from the checked process
        * @return The result of the security check 
        *         (TCustomResult::EPass/TCustomResult::EFail).
        */
        TCustomResult CustomSecurityCheckL( const RMessage2& aMsg, 
                                            TInt& aAction, 
                                            TSecurityInfo& aMissing );
    
    public:  // from CServer2
    
        /**
        * Creates a server-side session object.
        * @since S60 5.0
        * @param aVersion The version information supplied by the client. 
        * @param aMessage Represents the details of the client request that is
        *                 requesting the creation of the session.
        * @return A pointer to the newly created server-side session object. 
        */
        CSession2 *NewSessionL( const TVersion& aVersion, 
                                const RMessage2& /*aMessage*/) const;

    public:                                        
        /** 
        * The entry point for server startup. 
        * Creates the cleanupstack and continues to RunServerL
        * @since S60 5.0
        * @return KErrNone if success, otherwise error code
        */
        static TInt RunServer();

        /**
        * Reduce client count and start server shutdown sequence if no client 
        * sessions are available
        * @since S60 5.0
        */
        void ClientRelease();
     
        /**
        * Increment the client count counter by one
        * @since S60 5.0
        */
        void ClientAttach();

        /**
        * Returns pointer to the engine
        * @since S60 5.0
        * @return Pointer to the engine object
        */
        CWsfEngine* Engine();
        
    private:        
        /**
        * Creates active schduler and thread for server and start waiting for 
        * client requests
        * @since S60 5.0
        */
        static void RunServerL();
        
   
    private:    // Data       
        /**
        * Sniffer engine instance (owned)
        */
        CWsfEngine *iEngine;
        
        /**
        * Server closer object (owned)
        */
        CWsfServerCloser* iCloser;

        /**
        * Number of currently attached clients
        */
        TInt iClientCount;

    };


#endif // C_WSFSERVER_H

