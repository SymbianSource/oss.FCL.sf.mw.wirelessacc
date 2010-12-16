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
 * Description:  Implementation of CWsfLaunchAiHelperActiveWrapper.
 *
 */


// INCLUDE FILES
#include "wsflogger.h"
#include "wsfmodel.h"
#include "wsfactivewrappers.h"
#include "wsflaunchaihelperactivewrapper.h"

// Category UID of AiHelper property
static const TUid KWsfAiHelperCategoryUid = { 0x10281CEB };

// key numbers within the property 
static const TUint KWsfAiHelperExitCodeKey = 0;
static const TUint KWsfAiHelperIapIdKey = 1;

/**
* Delay that we wait for the aihelper to terminate
*/
static const TInt KTimerTickInterval = 1 * 1000 * 1000; 


// ----------------------------------------------------------------------------
// CWsfLaunchAiHelperActiveWrapper::CWsfLaunchAiHelperActiveWrapper
// ----------------------------------------------------------------------------
//
CWsfLaunchAiHelperActiveWrapper::CWsfLaunchAiHelperActiveWrapper(
        CWsfActiveWrappers &aParent) :
    CActive( EPriorityStandard ), // Standard priority
    iParent(aParent)
    {
    }


// ----------------------------------------------------------------------------
// CWsfLaunchAiHelperActiveWrapper::NewLC
// ----------------------------------------------------------------------------
//
CWsfLaunchAiHelperActiveWrapper* CWsfLaunchAiHelperActiveWrapper::NewLC( 
      CWsfModel* aModel, 
      CWsfActiveWrappers &aParent )
    {
    LOG_ENTERFN( "CWsfLaunchAiHelperActiveWrapper::NewLC" );
    CWsfLaunchAiHelperActiveWrapper* self =
            new (ELeave) CWsfLaunchAiHelperActiveWrapper( aParent );
    CleanupStack::PushL( self );
    self->ConstructL( aModel );
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfLaunchAiHelperActiveWrapper::NewL
// ----------------------------------------------------------------------------
//
CWsfLaunchAiHelperActiveWrapper* CWsfLaunchAiHelperActiveWrapper::NewL( 
    CWsfModel* aModel, 
    CWsfActiveWrappers &aParent )
    {
    LOG_ENTERFN( "CWsfLaunchAiHelperActiveWrapper::NewL" );
    CWsfLaunchAiHelperActiveWrapper* self = 
            CWsfLaunchAiHelperActiveWrapper::NewLC( 
                    aModel, 
                    aParent );
    CleanupStack::Pop(); // self;
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfLaunchAiHelperActiveWrapper::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfLaunchAiHelperActiveWrapper::ConstructL( 
        CWsfModel* aModel )
    {
    LOG_ENTERFN( "CWsfLaunchAiHelperActiveWrapper::ConstructL" );
    CActiveScheduler::Add( this ); // Add to scheduler
    iModel = aModel;
    User::LeaveIfError( iTimer.CreateLocal() );
    User::LeaveIfError( iAiHelperAppExitCode.Attach( KWsfAiHelperCategoryUid, 
                                                    KWsfAiHelperExitCodeKey ) );
    
    User::LeaveIfError( iAiHelperAppIapId.Attach( KWsfAiHelperCategoryUid, 
                                                  KWsfAiHelperIapIdKey ) );
    }

// ----------------------------------------------------------------------------
// CWsfLaunchAiHelperActiveWrapper::~CWsfLaunchAiHelperActiveWrapper
// ----------------------------------------------------------------------------
//
CWsfLaunchAiHelperActiveWrapper::~CWsfLaunchAiHelperActiveWrapper()
    {
    LOG_ENTERFN( 
               "CWsfLaunchAiHelperActiveWrapper::~CWsfWLANListActiveWrapper" );
    Cancel(); // Cancel any request, if outstanding
    // Delete instance variables if any
    iAiHelperAppExitCode.Close();
    iAiHelperAppIapId.Close();
    iTimer.Close();
    }

// ----------------------------------------------------------------------------
// CWsfLaunchAiHelperActiveWrapper::DoCancel
// ----------------------------------------------------------------------------
//
void CWsfLaunchAiHelperActiveWrapper::DoCancel()
    {
    iAiHelperAppExitCode.Cancel();
    iAiHelperAppIapId.Cancel();
    iTimer.Cancel();
    iStoppingHelper = iModel->CloseHelperApplication();
    }

// ----------------------------------------------------------------------------
// CWsfLaunchAiHelperActiveWrapper::Start
// ----------------------------------------------------------------------------
//
void CWsfLaunchAiHelperActiveWrapper::Start( TWsfWlanInfo& aInfo, 
                                             TBool aConnectOnly,
                                             TBool aTestAccessPoint )
    {
    LOG_ENTERFN( "CWsfLaunchAiHelperActiveWrapper::Start" );
    iStoppingHelper = EFalse;
    Cancel(); // Cancel any request, just to be sure
    iState = EUninitialized;
    iUsedInfo = aInfo;
    iConnectOnly = aConnectOnly;
    iTestAccessPoint = aTestAccessPoint;
    
    if ( iStoppingHelper )
        {
        LOG_WRITE( "Helper was stopped - wait for a while" );
        iTimer.Cancel();
        iTimer.After( iStatus, KTimerTickInterval );
        }
    else
        {
        LOG_WRITE( "Helper was not alive" );
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status, KErrNone);
        }
    SetActive();
    }

// ----------------------------------------------------------------------------
// CWsfLaunchAiHelperActiveWrapper::RunL
// ----------------------------------------------------------------------------
//
void CWsfLaunchAiHelperActiveWrapper::RunL()
    {
    LOG_ENTERFN( "CWsfLaunchAiHelperActiveWrapper::RunL" );
    if ( iStatus == KErrNone )
        {
        if ( iState == EUninitialized )
            {
            LOG_WRITE( "Start launching ai helper" );
            // register for notification
            iAiHelperAppExitCode.Subscribe( iStatus );
            iModel->LaunchHelperApplicationL( iUsedInfo );
            iState = EInitialized;
            SetActive(); // Tell scheduler a request is active
            }
        else if (iState == EInitialized)
            {
            TInt exitCode( KErrNone );
            iAiHelperAppExitCode.Get( exitCode );

            LOG_WRITEF( "AiHelper returned %d", exitCode );

            if ( exitCode == KErrNone )
                {
                TInt iapId(0);
                iAiHelperAppIapId.Get( iapId );
                iUsedInfo.iIapId = TUint32( iapId );
                LOG_WRITEF( "AiHelper iap id = %d", iapId );
                if ( iUsedInfo.iIapId )
                    {
                    LOG_WRITE( "Iap id exist - connect" );
                    
                    TWsfIapPersistence persistence;

                    if ( iTestAccessPoint )
                        {
                        persistence = EIapExpireOnDisconnect;
                        }
                    else
                        {
                        persistence = EIapPersistent;
                        }
                    
                    iParent.Connect( 
                            iUsedInfo.iIapId,
                            iConnectOnly,
                            persistence);
                    }
                else
                    {
                    LOG_WRITE( "No iap id from ai helper" );
                    }
                }
            else
                {
                User::Leave(exitCode);
                }
            }
        else
            {
            LOG_WRITEF( "iState = %d", iState );
            }
        }
    else
        {
        LOG_WRITEF( "CWsfLaunchAiHelperActiveWrapper iStatus = %d", 
                                                               iStatus.Int() );
        }
    }

// ----------------------------------------------------------------------------
// CWsfLaunchAiHelperActiveWrapper::RunError
// ----------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CWsfLaunchAiHelperActiveWrapper::RunError( TInt aError )
    {
    LOG_ENTERFN( "CWsfLaunchAiHelperActiveWrapper::RunError" );
    LOG_WRITEF( "aError = %d", aError );
#else
TInt CWsfLaunchAiHelperActiveWrapper::RunError( TInt /*aError*/ )
    {
#endif
    return KErrNone;
    }

