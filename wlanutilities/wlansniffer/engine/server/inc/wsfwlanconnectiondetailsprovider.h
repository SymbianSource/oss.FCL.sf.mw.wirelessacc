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
* Description:  Class header for MWsfWlanConnectionDetailsProvider
*
*/



#ifndef M_WSFWLANCONNECTIONDETAILSPROVIDER_H
#define M_WSFWLANCONNECTIONDETAILSPROVIDER_H


//  EXTERNAL INCLUDES
#include <e32def.h>


//  FORWARD DECLARATIONS
class TWsfWlanInfo;


//  CLASS DEFINITION
/**
* Class acts as an interface to the WLAN bearer monitor to let different 
* parts of the engine query the properties of the current WLAN connection.
* 
* @since S60 5.0
*/
class MWsfWlanConnectionDetailsProvider
    {
    public:     // Abstract methods
        
        /**
        * Fills TWsfWlanInfo with the connected wlan's properties
        * @since S60 5.0
        * @param aWlanInfo The wlaninfo structure to be filled
        * @return ETrue if there is a connection and returned data is valid, 
        *         EFalse otherwise
        */ 
        virtual TBool ConnectedWlanConnectionDetailsL( 
                                                TWsfWlanInfo* aWlanInfo ) = 0;
        
        /**
        * Queries if there is an active WLAN connection.
        * @since S60 5.2
        * @return ETrue if there is an active WLAN connection, 
        *         EFalse otherwise.
        */ 
        virtual TBool IsConnected() = 0;

    };

#endif      //  M_WSFWLANCONNECTIONDETAILSPROVIDER_H

// End of file
