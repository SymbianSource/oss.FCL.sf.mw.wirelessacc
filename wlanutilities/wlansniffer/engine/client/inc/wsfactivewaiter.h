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
* Description:   Declaration of CWsfActiveWaiter
*
*/




#ifndef __WSFCLIENT_ACTIVE_WAITER_H__
#define __WSFCLIENT_ACTIVE_WAITER_H__

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
        * NewL function
        * @param  aPlugin a pointer to notifier plugin
        */
        static CWsfActiveWaiter* NewL( );

        /**
        * Destructor
        */    
        virtual ~CWsfActiveWaiter();          

        /**
        * Waits for it's TRequestStatus to be completed
        * @return Error code returned from server after request is
        * completed
        */
        TInt WaitForRequest( );
        
    protected:
        /**
        * ConstructL 
        */
        void ConstructL();
    
        /**
        * Constructor
        * @param  aPlugin a pointer to notifier plugin    
        */    
        CWsfActiveWaiter();
            
        /**
        * DoCancel from CActive
        */    
        virtual void DoCancel();
      
        /**
        * RunL from CActive
        */      
        virtual void RunL();
    
    protected:
        CActiveSchedulerWait iWait;
    };     

#endif // __WSFCLIENT_ACTIVE_WAITER_H__

// End of file
