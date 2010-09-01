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
* Description:  Class header for CWsfActiveWaiter
*
*/


#ifndef C_WSFACTIVEWAITER_H
#define C_WSFACTIVEWAITER_H

//  EXTERNAL INCLUDES
#include <e32base.h>


//  CLASS DEFINITION
/**
 * CWsfActiveWaiter class,
 * an active object to replace User::WaitForRequest()
 * 
 * Usage:
 * @code
 * CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewL();
 * CleanupStack::PushL( waiter );
 * server.AsyncFunction( waiter->iStatus );
 * if ( waiter->WaitForRequest() == KErrNone )
 *      {
 *      // handle success
 *      }
 *      else 
 *      {
 *      // handle failure
 *      }
 * CleanupStack::PopAndDestroy( waiter );
 * @endcode
 *
 * @lib wsfserver.exe
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CWsfActiveWaiter ): public CActive
    {
    public:
        /**
        * Factory function
        * @since S60 5.0
        * @return CWsfActiveWaiter instance.
        */
        static CWsfActiveWaiter* NewL();

        /**
        * Factory function
        * @since S60 5.0
        * @return CWsfActiveWaiter instance.
        */
        static CWsfActiveWaiter* NewLC();

        /**
        * Destructor
        * @since S60 5.0
        */    
        virtual ~CWsfActiveWaiter();          

    protected:
        /**
        * Constructor
        * @since S60 5.0
        */    
        CWsfActiveWaiter();

    protected:  // from CActive

        /**
        * Implements cancellation of an outstanding request.
        * @since S60 5.0
        */
        void DoCancel();

        /**
        * Handles an active object's request completion event.
        * @since S60 5.0
        */
        void RunL();

    public:     // new methods
        /**
        * Waits for its TRequestStatus to be completed
        * @since S60 5.0
        * @return Error code returned from server after request is completed
        */
        TInt WaitForRequest();
    
    protected:  // data
        /**
        * Nested waitloop
        */
        CActiveSchedulerWait iWait;
    };     

#endif // C_WSFACTIVEWAITER_H

// End of file
