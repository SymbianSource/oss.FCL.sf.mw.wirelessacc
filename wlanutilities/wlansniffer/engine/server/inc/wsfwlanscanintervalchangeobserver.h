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
* Description:  Class header for MWsfWlanScanIntervalChangeObserver
*
*/


#ifndef M_WSFWLANSCANINTERVALCHANGEOBSERVER_H
#define M_WSFWLANSCANINTERVALCHANGEOBSERVER_H

//  EXTERNAL INCLUDES
#include <e32def.h>

//  CLASS DEFINITION
/**
* Class acts as an interface for the class monitoring the background scan 
* interval to notify its clients of changes in that property.
* 
* @since S60 5.0
*/
class MWsfWlanScanIntervalChangeObserver
    {
    public:     // Abstract methods
        /**
        * Called when the value of the background scan interval or the
        * show availability flag has been changed
        * @since S60 5.0
        * @param aNewScanInterval The new value for the scan interval
        * @param aShowAvailability The new value for the 
        *                          Show WLAN availability flag
        */
        virtual void WlanScanIntervalChangedL( TUint aNewScanInterval,
                                               TBool aShowAvailability ) = 0;
       
    };

#endif      //  M_WSFWLANSCANINTERVALCHANGEOBSERVER_H

// End of file
