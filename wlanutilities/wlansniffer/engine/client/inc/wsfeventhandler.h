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
* Description:   Class header for CWsfEventHandler
*
*/





#ifndef C_WSFEVENTHANDLER_H
#define C_WSFEVENTHANDLER_H

//  EXTERNAL INCLUDES
#include <e32base.h>

//  INTERNAL INCLUDES
#include "wsfsession.h"
#include "wsfservercommon.h"
#include "wsfstatechangeobserver.h"


//  CLASS DEFINITION
/**
 * Class to handle callbacks from the server
 *
 * CWsfEventHandler acts as a gateway between the client and the server. 
 * Its main task is to hand over notifications of server events if the client 
 * has requested that. 
 *
 * @lib wsfclient.lib
 * @since S60 5.0
 */

NONSHARABLE_CLASS( CWsfEventHandler ): public CActive
    {
    public:     // Constructors and destructor
        /**
        * Factory function.
        * @since S60 5.0
        * @param aSession Client-side handle to the server session
        *                 Ownership not passed.
        * @param aObserver The object which should be notified on event
        *                  Ownership not passed.
        */
        static CWsfEventHandler* NewL( RWsfSession& aSession, 
                                       MWsfStateChangeObserver& aObserver );

        ~CWsfEventHandler();
        
        
    private:
        /**
        * Constructor
        * @since S60 5.0
        * @param aSession Client-side handle to the server session.
        *                 Ownership not passed.
        * @param aObserver The object which should be notified on event. 
        *                  Ownership not passed.
        */
        CWsfEventHandler( RWsfSession& aSession, 
                          MWsfStateChangeObserver& aObserver );
        
        void ConstructL();
        
        
    public:     // new methods
        /**
        * Set the listening active object as active
        * @since S60 5.0
        */
        void Start();
        
        /**
        * Get reference to the event container 
        * @since S60 5.0
        * @return Reference to the event container.
        */
        TPckgBuf<TWsfNotifyEventContainer>& EventContainer();
         
        /**
        * Change the event observer by resetting the reference
        * @since S60 5.0
        * @param aObserver The object which should be notified on event. 
        *                  Ownership not passed.
        */ 
        void ResetObserver( MWsfStateChangeObserver& aObserver );

        /**
        * Prevents the ongoing connecting process from notifying the observer
        * if the connecting flag is set
        * @since S60 5.0
        */
        void BlockNextConnectedEvent();

        /**
        * Sets the connecting flag. Used by RWsfSession to mark the interval
        * of the connection creation.
        * @since S60 5.0
        * @param aConnecting The value of the flag.
        */
        void SetConnecting( TBool aConnecting );
         
        /**
        * Gets the connecting flag. 
        * @since S60 5.0
        * @return The value of the flag.
        */
        TBool Connecting() const;

         
    private:    // from CActive
        void RunL();
        void DoCancel();
        TInt RunError( TInt aError );
    
    private:
        /**
        * Handle the RunL trapped
        * @since S60 5.0
        */
        void DoRunL();

    private:
        /**
        * Client-side handle for the server session. Not owned.
        */        
        RWsfSession* iSession;
        
        /**
        * The object which should be notified on event. Not owned.
        */
        MWsfStateChangeObserver* iObserver;
        
        /**
        * Event container
        */
        TPckgBuf<TWsfNotifyEventContainer> iEvent;
        
        /**
        * Indicates whether the connection created event should be suppressed
        */
        TBool iBlockConnectedEvent;

        /**
        * Indicates whether there is an ongoing connecting process
        */
        TBool iConnecting;
    
    };
    
    
#endif // C_WSFEVENTHANDLER_H
