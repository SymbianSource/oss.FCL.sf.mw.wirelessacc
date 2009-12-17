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
* Description:  Implementation of CWsfScreenSaverWatcher
*
*/


//  EXTERNAL INCLUDES
#include <e32cmn.h>
#include <ScreensaverInternalPSKeys.h>

//  CLASS HEADER
#include "wsfscreensaverwatcher.h"

//  INTERNAL INCLUDES
#include "wsfscreensaverstateobserver.h"



// ----------------------------------------------------------------------------
// CWsfScreenSaverWatcher::NewL
// ----------------------------------------------------------------------------
//
CWsfScreenSaverWatcher* CWsfScreenSaverWatcher::NewL( 
                                      MWsfScreenSaverStateObserver& aObserver )
    {
    CWsfScreenSaverWatcher* thisPtr = NewLC( aObserver );
    CleanupStack::Pop( thisPtr );
    return thisPtr;
    }


// ----------------------------------------------------------------------------
// CWsfScreenSaverWatcher::NewLC
// ----------------------------------------------------------------------------
//
CWsfScreenSaverWatcher* CWsfScreenSaverWatcher::NewLC( 
                                      MWsfScreenSaverStateObserver& aObserver )
    {
    CWsfScreenSaverWatcher* thisPtr = new ( ELeave ) CWsfScreenSaverWatcher( 
                                                                   aObserver );
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    return thisPtr;
    }


// ----------------------------------------------------------------------------
// CWsfScreenSaverWatcher::~CWsfScreenSaverWatcher
// ----------------------------------------------------------------------------
//
CWsfScreenSaverWatcher::~CWsfScreenSaverWatcher()
    {
    Cancel();
    iProperty.Close();
    }


// ----------------------------------------------------------------------------
// CWsfScreenSaverWatcher::CWsfScreenSaverWatcher
// ----------------------------------------------------------------------------
//
CWsfScreenSaverWatcher::CWsfScreenSaverWatcher( 
                                     MWsfScreenSaverStateObserver& aObserver ): 
    CActive( EPriorityStandard ),
    iObserver( &aObserver )
    {
    }


// ----------------------------------------------------------------------------
// CWsfScreenSaverWatcher::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfScreenSaverWatcher::ConstructL()
    {
    User::LeaveIfError( iProperty.Attach( KPSUidScreenSaver, 
                                          KScreenSaverOn ) );
    CActiveScheduler::Add( this );
    }


// ----------------------------------------------------------------------------
// CWsfScreenSaverWatcher::RunL
// ----------------------------------------------------------------------------
//
void CWsfScreenSaverWatcher::RunL()
    {
    // read the value of property
    TBool screenSaverActive( EFalse );
    iProperty.Get( screenSaverActive );
    iObserver->ScreenSaverStatusChangedL( screenSaverActive );
    // reissue the request
    StartStatusScanning();
    }


// ----------------------------------------------------------------------------
// CWsfScreenSaverWatcher::DoCancel
// ----------------------------------------------------------------------------
//
void CWsfScreenSaverWatcher::DoCancel()
    {
    iProperty.Cancel();
    }


// ----------------------------------------------------------------------------
// CWsfScreenSaverWatcher::StartStatusScanning
// ----------------------------------------------------------------------------
//
void CWsfScreenSaverWatcher::StartStatusScanning()
    {
    iProperty.Subscribe( iStatus );
    SetActive();
    }


// ----------------------------------------------------------------------------
// CWsfScreenSaverWatcher::RunError
// ----------------------------------------------------------------------------
//
TInt CWsfScreenSaverWatcher::RunError( TInt /*aError*/ )
    {
    // issue the request so we will continue to receive 
    // state change notifications
    StartStatusScanning();
    return KErrNone;
    }
