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
* Description:  Class header for MWsfWlanScannerObserver
*
*/



#ifndef M_WSFWLANSCANNEROBSERVER_H
#define M_WSFWLANSCANNEROBSERVER_H

//  EXTERNAL INCLUDES
#include <e32def.h>


//  CLASS DEFINITION
/**
* Class acts as an interface for the CWsfWlanScanner to be able to call back 
* its clients in case of different scanning events.
*
* @since S60 5.0
*/
class MWsfWlanScannerObserver
    {
    public:     // Abstract methods
        /**
        * When the scan cycle has completed this function is called to report 
        * the completion.
        * @since S60 5.0
        */
        virtual void WlanScanCompleteL() = 0;

        /**
        * Called when the scan cycle has started  
        * @since S60 5.0
        */
        virtual void WlanScanStarted() = 0;

        /**
        * Called when an error occured while scanning wlan data  
        * @since S60 5.0
        * @param aError System wide error code
        */
        virtual void NotifyError( TInt aError ) = 0;        
        
    };

#endif      //  M_WSFWLANSCANNEROBSERVER_H

// End of file
