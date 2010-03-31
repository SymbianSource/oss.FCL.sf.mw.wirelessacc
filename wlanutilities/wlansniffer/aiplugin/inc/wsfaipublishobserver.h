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
* Description:  Class header for MWsfAiPublishObserver
*
*/


#ifndef M_WSFAIPUBLISHOBSERVER_H
#define M_WSFAIPUBLISHOBSERVER_H

#include <e32std.h>

class CAknIconArray;
class CEikLabel;
class CArrayFix< TInt >;
/**
 *  Abstract interface for wlan active idle plugin
 *
 */
class MWsfAiPublishObserver
    {
public:
    
    /**
    * Publishes images from array and text
    * @since S60 5.0
    * @param aPublishIconArray the published icons
    * @param aText1 title text label
    */
    virtual void PublishContentL( CArrayFix< TInt >* aPublishIconArray,
                                  CEikLabel* aText1 ) = 0;
    
    /**
    * Sets Scanning status on/off.
    * @since S60 5.0
    * @param aScanState Scanning satus flag
    */
    virtual void SetScanningState( TBool aScanState ) = 0;
    
    /**
    * Sets refreshing status on/off.
    * @since S60 5.0
    * @param aScanState Refreshing status flag
    */
    virtual void SetRefreshingL( TBool aRefreshing ) = 0;
    
    /**
    * Sets connecting status on.
    * @since S60 5.0
    * @param aScanState Refreshing status flag
    */
    virtual void StartConnectingL() = 0;
    
    /**
    * Publishes state image from array 
    * @since S60 5.0
    * @param aPublishIconArray the published icons
    * @param aText1 title text label
    */
    virtual void PublishStatusIconL( CArrayFix<TInt>* aPublishIconArray, 
                                                CEikLabel* aText1 ) = 0;
    
    };


#endif // M_WSFAIPUBLISHOBSERVER_H
