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
* Description:  Declaration of CWsfActiveWaiter
*
*/



#ifndef __WSFMAINAPPLICATION_ACTIVE_WAITER_H__
#define __WSFMAINAPPLICATION_ACTIVE_WAITER_H__

// INCLUDES
#include <e32base.h>

/**
 * CWsfActiveWaiter class,
 * an active object to replace User::WaitForRequest
 * 
 * Usage:
 * CWsfActiveWaiter* waiter = CWsfActiveWaiter::NewL();
 * CleanupStack::PushL( waiter );
 * server.AsyncFunction( waiter->iStatus );
 * if ( waiter->WaitForRequest == KErrNone )
 *      {
 *      // handle success
 *      }
 *      else 
 *      {
 *      // handle failure
 *      }
 * CleanupStack::PopAndDestroy( waiter );
 * 
 */     
NONSHARABLE_CLASS( CWsfActiveWaiter ) : public CActive
    {
    public:

        /**
        * Two-phased constructor
        * @since S60 5.0 
        * @return instance of CWsfActiveWaiter class
        */
        static CWsfActiveWaiter* NewL();

        /**
        * Destructor of CWsfActiveWaiter class
        * @since S60 5.0     
        */  
        virtual ~CWsfActiveWaiter();          

        /**
        * Waits for it's TRequestStatus to be completed
        * @since S60 5.0     
        * @return Error code returned from server after request is
        * completed
        */
        TInt WaitForRequest();
        
    private:

        /**
        * 2nd phase constructor
        * @since S60 5.0 
        */
        void ConstructL();
    
        /**
        * Default C++ constructor 
        * @since S60 5.0 
        */ 
        CWsfActiveWaiter();
            
        /**
        * DoCancel from CActive
        * @since S60 5.0 
        */    
        virtual void DoCancel();
      
        /**
        * RunL from CActive
        * @since S60 5.0 
        */      
        virtual void RunL();
    
    private:

        /**
        * Used to make asynchronous call synchronous
        */
        CActiveSchedulerWait iWait;
    };     

#endif // __WSFMAINAPPLICATION_ACTIVE_WAITER_H__

// End of file
