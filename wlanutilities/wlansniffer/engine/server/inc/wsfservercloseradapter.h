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
* Description:  Class header for MWsfServerCloserAdapter
*
*/




#ifndef M_WSFSERVERCLOSERADAPTER_H
#define M_WSFSERVERCLOSERADAPTER_H

//  EXTERNAL INCLUDES
#include <e32def.h>

 

//  CLASS DEFINITION
/**
* Class acts as an interface for the Sniffer server closer to let different 
* parts of the engine modify the default closing behaviour. Basically, the 
* functions set the appropriate semaphore in the server closer.
* 
* @since S60 5.0
*/
class MWsfServerCloserAdapter
    {
    public:     // Abstract methods

        /**
        * Tells the server whether to shut down if there are no clients
        * connected to it.
        * @since S60 5.0
        * @param aWait If ETrue, the server must wait for the clients to
        *              disconnect first before closing up.
        */
        virtual void WaitForClients( TBool aWait ) = 0;

        /**
        * Tells the server whether to shut down while there is an owned
        * connection open.
        * @since S60 5.0
        * @param aWait If ETrue, the server must wait for the connection to
        *              terminate first before closing up.
        */
        virtual void WaitForOwnedConnection( TBool aWait ) = 0;

        /**
        * Tells the server whether to shut down while there is an IAP
        * whose deletion is the server's responsibility.
        * @since S60 5.0
        * @param aWait If ETrue, the server must wait for the IAP to be
        *              deleted first before closing up.
        */
        virtual void WaitForBrowserExit( TBool aWait ) = 0;
        
    };


#endif      //  M_WSFSERVERCLOSERADAPTER_H

// End of file

