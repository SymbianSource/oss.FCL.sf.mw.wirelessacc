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
* Description:  Implementation of CWsfAiHelperAppUi
*
*/


//  EXTERNAL INCLUDES
#include <avkon.hrh>
#include <apgcli.h>
#include <cmmanager.h>
#include <cmconnectionmethod.h>
#include <StringLoader.h>
#include <wsfaihelper.rsg>
#include <AknQueryDialog.h>
#include <e32property.h>
#include <ErrorUI.h>
#include <wlanerrorcodes.h>

//  CLASS HEADER
#include "wsfaihelperappui.h"

//  INTERNAL INCLUDES
#include "wsfaihelperdocument.h"
#include "wsfaihelperapplication.h"
#include "wsfmodel.h"
#include "wsfwlaninfo.h"
#include "wsfaihelperview.h"
#include "wsfaihelper.hrh"
#include "wsflogger.h"


//  CONSTANTS

// property keys for AiHelper return 
static const TUint KWsfAiHelperExitCodeKey = 0;
static const TUint KWsfAiHelperIapIdKey = 1;


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::ConstructL()
    {
    BaseConstructL( EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible );

    iIdle = CIdle::NewL( CActive::EPriorityIdle );
    
    // Create stub view for Wizard launching
    CWsfAiHelperView* view = CWsfAiHelperView::NewLC( this );
    AddViewL( view );
    CleanupStack::Pop( view );
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::CWsfAiHelperAppUi
// ---------------------------------------------------------------------------
//
CWsfAiHelperAppUi::CWsfAiHelperAppUi():
    iResult( KErrCancel )
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::~CWsfAiHelperAppUi
// ---------------------------------------------------------------------------
//
CWsfAiHelperAppUi::~CWsfAiHelperAppUi()
    {
    delete iIdle;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::HandleCommandL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::HandleCommandL( TInt aCommand )
    {
    LOG_ENTERFN( "CWsfAiHelperAppUi::HandleCommandL" );
    LOG_WRITEF( "command = %d", aCommand );
    
    if ( iSuppressingKeyEvents )
        {
        return;
        }
    
    switch ( aCommand )
        {
        case EAknCmdExit:
        case EEikCmdExit:
        case EAknSoftkeyBack:
        case EAknSoftkeyExit:
            {
            // set the "exit code" value
            RProperty::Define( Application()->AppDllUid(), 
                               KWsfAiHelperExitCodeKey, 
                               RProperty::EInt );
            
            TInt err = RProperty::Set( Application()->AppDllUid(), 
                            KWsfAiHelperExitCodeKey, 
                            iResult );
            
            if ( err == KErrNone )
                {
                LOG_WRITEF( "exitcode property set to %d", iResult );
                }
            else
                {
                LOG_WRITEF( "exitcode property setting failed with code = %d",
                            err );
                User::LeaveIfError( err );
                }
            
            Exit();
            break;
            }

        default:;
        }
    }
    

// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::ProcessCommandParametersL
// ---------------------------------------------------------------------------
//
TBool CWsfAiHelperAppUi::ProcessCommandParametersL( 
                                                TApaCommand /*aCommand*/,
                                                TFileName& /*aDocumentName*/,
                                                const TDesC8& /*aTail*/ )
    {
    return EFalse;    
    }
    

// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::NotifyFileName
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::NotifyFileName()
    {
    LOG_ENTERFN( "CWsfAiHelperAppUi::NotifyFileName" );
    if ( !iIdle->IsActive() )
        {
        iIdle->Start( TCallBack( LaunchCompletedL, this ) );    
        }
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::Doc
// ---------------------------------------------------------------------------
//
CWsfAiHelperDocument* CWsfAiHelperAppUi::Doc()
    {
    return static_cast<CWsfAiHelperDocument*>( iDocument );
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::BrowserLaunchCompleteL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::BrowserLaunchCompleteL()
    {
    LOG_ENTERFN( "CWsfAiHelperAppUi::BrowserLaunchCompleteL" );
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::BrowserExitL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::BrowserExitL()
    {
    LOG_ENTERFN( "CWsfAiHelperAppUi::BrowserExitL" );
    HandleCommandL( EAknCmdExit );
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::BrowserLaunchFailed
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::BrowserLaunchFailed( TInt /*aError*/ )
    {
    LOG_ENTERFN( "CWsfAiHelperAppUi::BrowserLaunchFailed" );
    TRAP_IGNORE( HandleCommandL( EAknCmdExit ) );
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::ConnectingL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::ConnectingL( TUint32 /*aIapId*/ )
    {
    LOG_ENTERFN( "CWsfAiHelperAppUi::ConnectingL" );
    Doc()->Model().SetConnecting( ETrue );
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::ConnectingFinishedL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::ConnectingFinishedL( TInt aResult )
    {
    LOG_ENTERFN( "CWsfAiHelperAppUi::ConnectingFinishedL" );
    if ( Doc()->Model().IsConnecting() )
        {
        Doc()->Model().SetConnecting( EFalse );
        }
    
    if ( aResult )
        {
        // show an error note
        LOG_WRITE( "showing error note" );
        CErrorUI* errorUi = CErrorUI::NewLC( *iCoeEnv );
        errorUi->ShowGlobalErrorNoteL( aResult );
        CleanupStack::PopAndDestroy( errorUi );
        }
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::SendHelperToBackground
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::SendHelperToBackground()
    {
    LOG_ENTERFN( "CWsfAiHelperAppUi::SendHelperToBackground" );
    TApaTask task( iEikonEnv->WsSession() );
    task.SetWgId( iEikonEnv->RootWin().Identifier());
    task.SendToBackground();
    }
    

// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::LaunchCompletedL
// ---------------------------------------------------------------------------
//
TInt CWsfAiHelperAppUi::LaunchCompletedL( TAny* aObject )
    {
    LOG_ENTERFN( "CWsfAiHelperAppUi::LaunchCompletedL" );
    User::LeaveIfNull( aObject );
    CWsfAiHelperAppUi* appUi = static_cast<CWsfAiHelperAppUi*>( aObject );
   
    MWsfModelObserver* observer = static_cast<MWsfModelObserver*>( appUi );
    CWsfModel& model( appUi->Doc()->Model() );
    
    model.SetEngineObserver( observer );

    TWsfWlanInfo* info( &appUi->Doc()->WlanInfo() );

    if ( !info->Connected() )
        {
        // make sure we don't suppress our key events forever
        CleanupStack::PushL( TCleanupItem( ReleaseSuppressingKeyEventsL, appUi ) );
        // No appUi key input accepted during IAP connecting & connection test.
        appUi->iSuppressingKeyEvents = ETrue;

        TBool setIapProperty = EFalse;
        
        if ( !info->Known() )
            {
            TBool accessPointCreated = EFalse;
                        
            // If not known yet, create an IAP. 
            TRAPD( accessPointCreatedError, 
                    accessPointCreated = model.CreateAccessPointL( *info, 
                                                                   EFalse ) );
            
            // notify user that Disk is full or creation of a WAPI IAP 
            // is not supported on-the-fly and leave
            if ( accessPointCreatedError == KErrDiskFull ||
                 accessPointCreatedError == KErrWlanProtectedSetupSetupLocked )
                {
                appUi->ConnectingFinishedL(accessPointCreatedError);
                User::Leave(accessPointCreatedError);
                }
            // just leave because we failed to create accesspoint
            else if ( accessPointCreatedError != KErrNone )
                {
                User::Leave(accessPointCreatedError);
                }
            
            appUi->SendHelperToBackground();
                     
            if ( accessPointCreated )
                {
                setIapProperty = ETrue;
                }                
            }
        else
            {
            setIapProperty = ETrue;     
            appUi->iResult = KErrNone;
            }
        
        if ( setIapProperty )
            {
            LOG_WRITE( "Set iap Property" );
            // Connected succesfully set IAP's property.       
            CWsfAiHelperApplication* app = 
                    static_cast<CWsfAiHelperApplication*>( 
                    appUi->Application() );
        
            RProperty::Define( app->AppDllUid(), 
                    KWsfAiHelperIapIdKey, 
                    RProperty::EInt );
                
            TInt err = RProperty::Set( app->AppDllUid(), 
                    KWsfAiHelperIapIdKey, 
                    info->iIapId );
                    
            if ( err == KErrNone )
                {
                LOG_WRITEF( "IAP id property "
                                L"set to %d", info->iIapId );
                }
            else
                {
                LOG_WRITEF( "IAP id property setting failed "
                        L"with code = %d", err );
                User::LeaveIfError( err );
                }
                appUi->iResult = KErrNone;
            }
        // pop cleanup item ReleaseSuppressingKeyEvents
        CleanupStack::Pop();
        appUi->iSuppressingKeyEvents = EFalse;
        }    

    appUi->HandleCommandL( EAknCmdExit );
    
    return 0;   // Return false (zero) value not be called again.
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::WlanListChangedL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::WlanListChangedL()
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::NotifyEngineError
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::NotifyEngineError( TInt /*aError*/ )
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::ScanDisabledL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::ScanDisabledL()
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::ScanEnabledL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::ScanEnabledL()
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::WlanConnectionActivatedL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::WlanConnectionActivatedL()
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::WlanConnectionClosedL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::WlanConnectionClosedL()
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::ConnectingFinishedL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::ConnectionCreationProcessFinishedL( TInt /*aResult*/ )
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperAppUi::ReleaseSuppressingKeyEventsL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperAppUi::ReleaseSuppressingKeyEventsL( TAny* aPtr )
    {
    CWsfAiHelperAppUi* self = 
                        static_cast<CWsfAiHelperAppUi*>( aPtr );
    self->iSuppressingKeyEvents = EFalse;
    self->HandleCommandL( EAknCmdExit );
    }


