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
* Description:  Implementation of CWsfAiPlugin
*
*/



//  EXTERNAL INCLUDES
#include <aicontentobserver.h>
#include <aiutility.h>
#include <apgcli.h>
#include <wsfaipluginrsc.rsg>

#include <e32std.h>
#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>

#include <f32file.h>
#include <AknsConstants.h>
#include <AknUtils.h>
#include <StringLoader.h>
#include <bautils.h>
#include <avkon.rsg>
#include <apgtask.h>
#include <eiklabel.h>

#include <data_caging_path_literals.hrh> // KDC_APP_BITMAP_DIR

//  INTERNAL INCLUDES
#include "wsfaiplugin.h"
#include "wsfaipluginuids.hrh"
#include "wsfaiplugincontentmodel.h"
#include "wsfaimodel.h"
#include "wsfmodel.h"
#include "wsfaiview.h"
#include "wsficonarraycreator.h"
#include "wsfdbobserver.h"
#include "wsflogger.h"
#include "wsfactivewrappers.h"


// define icon id for Navigation Bar icon
LOCAL_D const TUid KUidSnifferApp = {0x10281CAA};
LOCAL_D const TInt KRefreshStepTime = 200 * 1000;
LOCAL_D const TInt KConnectingStepTime = 200 * 1000;



// --------------------------------------------------------------------------
// CWsfAiPlugin::NewL
// --------------------------------------------------------------------------
//
CWsfAiPlugin* CWsfAiPlugin::NewL()
    {
    LOG_ENTERFN( "CWsfAiPlugin::NewL" );
    CWsfAiPlugin* self = new (ELeave) CWsfAiPlugin;

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::~CWsfAiPlugin
// --------------------------------------------------------------------------
//
CWsfAiPlugin::~CWsfAiPlugin()
    {
    LOG_ENTERFN( "CWsfAiPlugin::~CWsfAiPlugin" );
    // Cancel periodic animation update
    if ( iAnimationPeriodic )
        {
        LOG_WRITE( "Cancel animation update" );
        iAnimationPeriodic->Cancel();
        delete iAnimationPeriodic;
        iAnimationPeriodic = NULL;
        }
    
    delete iDbObserver;
    
    delete iActiveWrappers;
    
    delete iModel;
    delete iAiModel;
    delete iUi;
    
    if ( iMskActivate )
        {
        delete iMskActivate;
        }
    
    if ( iConnectedTo )
        {
        delete iConnectedTo;
        }
    
    if ( iKnownNetworkFound )
        {
        delete iKnownNetworkFound;
        }
    
    iEnv->DeleteResourceFile( iResourceFileOffset );

    iObservers.Close();

    Release( iContent );
    Release( iEvents );
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::CWsfAiPlugin
// --------------------------------------------------------------------------
//
CWsfAiPlugin::CWsfAiPlugin()
    {
    iEnv = CEikonEnv::Static();
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::ConstructL
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::ConstructL()
    {
    LOG_ENTERFN( "CWsfAiPlugin::ConstructL" );
    AllocateResourcesL();
    
    iContent = AiUtility::CreateContentItemArrayIteratorL( 
                                                    KAiWizardContent );
    iEvents = AiUtility::CreateContentItemArrayIteratorL( KAiWizardEvents );
    
    // create the engine first...
    iModel = CWsfModel::NewL( iController, EFalse );

    // then model
    iAiModel = CWsfAiModel::NewL();
    iUi = CWsfAiView::NewL( *this );
    
    iActiveWrappers = CWsfActiveWrappers::NewL( iModel, iController );
    
    iDbObserver = CWsfDbObserver::NewL();
    
    iController.SetUi( *static_cast<CWsfAiView*>( iUi ) );
    
    iController.InitializeL( iEnv, iModel, iAiModel, iDbObserver, 
							 iActiveWrappers );
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::PublishContentL
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::PublishContentL( CArrayFix<TInt>* aPublishIconArray, 
                                    CEikLabel* aText1 )
    {
    LOG_ENTERFN( "CWsfAiPlugin::PublishContentL" );
    TInt leftBoundary( 0 );
    TInt rightBoundary( 0 );
    TChar boundary('\'');
    HBufC* localCurrentStatusText( NULL );
    
    HBufC* localCurrentNetworkStatus( NULL );
    HBufC* localCurrentNetworkName( NULL );

    HBufC* localFullText = ( aText1->Text() )->AllocLC();

    // Locate ' chars from the full string
    TPtrC fullText = localFullText->Des();
    leftBoundary = fullText.Locate( boundary );    // char = '
    rightBoundary = fullText.LocateReverse( boundary );

    // if we have status name then we parse it
    if ( leftBoundary != rightBoundary && 
         leftBoundary != KErrNotFound && 
         rightBoundary != KErrNotFound )
        {
        LOG_WRITE( "status name exists" );
        TPtrC begin = fullText.Mid( leftBoundary, 
                                    rightBoundary - leftBoundary + 1 );
        localCurrentStatusText = begin.AllocLC();
        localCurrentNetworkName = begin.AllocLC();

        TPtrC end = fullText.Mid( rightBoundary + 1 );
       }
    else
        {
        LOG_WRITE( "status name does not exist" );
        localCurrentStatusText = fullText.AllocLC();
        }
    
    //Connected
    if ( localCurrentNetworkName )
        {
        if ( iAiModel->Connected() )
            {
            LOG_WRITE( "ai model is connected" );
            if ( !iConnectedTo )
                {
                iConnectedTo = StringLoader::LoadL( 
                        R_QTN_SNIFFER_PLUG_IN_CONNECTED_TO );
                }
            localCurrentNetworkStatus = iConnectedTo;
            }
        else
            {
            if ( !iKnownNetworkFound )
                {
                iKnownNetworkFound = StringLoader::LoadL( 
                        R_QTN_SNIFFER_PLUG_IN_WLAN_NETWORK_FOUND );
                }
            LOG_WRITE( "there is known network" );
            localCurrentNetworkStatus = iKnownNetworkFound;
            }
        }
    else
        {
        LOG_WRITE( "use current status text" );
        localCurrentNetworkStatus = localCurrentStatusText;
        }

    TBool published( EFalse );
    TInt iconId( 0 );

    for ( iCurrentObserverIndex = 0; 
          iCurrentObserverIndex < iObservers.Count(); 
          ++iCurrentObserverIndex )
        {
        LOG_WRITEF( "Start publish - index = %d", iCurrentObserverIndex );
        
        MAiContentObserver* observer = iObservers[iCurrentObserverIndex];
        observer->StartTransaction( reinterpret_cast<TInt32>( this ) );
        // make sure we cancel the tracsaction if leaves
        CleanupStack::PushL( TCleanupItem( PublishCleanup, this ) );

        if ( localCurrentStatusText )
            {
            LOG_WRITE( "Publish -> status icon" );
            // Publish Status icon: EAiWizardContentStatusIcon
            iconId = aPublishIconArray->At( 0 );
            published = PublishIconL( observer, 
                                      EAiWizardContentStatusIcon,
                                      iconId ) || published;
            }
        else 
            {
            observer->Clean( 
                *this, 
                EAiWizardContentStatusIcon, 
                EAiWizardContentStatusIcon );
            }

        LOG_WRITE( "Publish -> Strength icon" );
        // Publish Strength icon: EAiWizardContentStrengthIcon
        iconId = aPublishIconArray->At( 2 );
        published = PublishIconL( observer, 
                                  EAiWizardContentStrengthIcon,
                                  iconId ) || published;

        LOG_WRITE( "Publish -> secure icon" );
        // Publish Secure icon: EAiWizardContentSecureIcon
        iconId = aPublishIconArray->At( 1 );
        published = PublishIconL( observer, 
                                  EAiWizardContentSecureIcon,
                                  iconId ) || published;
        
        if ( localCurrentNetworkName )
            {
            LOG_WRITE( "Publish -> localCurrentNetworkName" );
            // Publish NetworkName text: EAiWizardContentNetworkName
            published = PublishText( observer, 
                                     EAiWizardContentNetworkName,
                                     *localCurrentNetworkName ) || published;
            }
        else 
            {
            observer->Clean( 
                *this, 
                EAiWizardContentNetworkName, 
                EAiWizardContentNetworkName );
            }
        
        if ( localCurrentNetworkStatus )
            {
            LOG_WRITE( "Publish -> localCurrentNetworkStatus" );
            // Publish NetworkName text: EAiWizardContentNetworkName
            published = PublishText( observer, 
                                     EAiWizardContentNetworkStatus,
                                     *localCurrentNetworkStatus ) || published;
            }
        
        // If we published something then commit, 
        // otherwise cancel transaction
        if ( published )
            {
            LOG_WRITE( "Commit" );
            observer->Commit( reinterpret_cast<TInt32>( this ) );
            published = EFalse;
            }
        else
            {
            LOG_WRITE( "Cancel transaction" );
            observer->CancelTransaction( reinterpret_cast<TInt32>( this ) );
            }
        CleanupStack::Pop( 1 ); // PublishCleanup()
        }
    
    LOG_WRITE( "Publishing ready" );
    
    if ( localCurrentNetworkName ) 
        {
        CleanupStack::PopAndDestroy( localCurrentNetworkName );
        }
    
    if ( localCurrentStatusText ) 
        {
        CleanupStack::PopAndDestroy( localCurrentStatusText );
        }

    CleanupStack::PopAndDestroy( localFullText );
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::AllocateResourcesL
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::AllocateResourcesL()
    {
    LOG_ENTERFN( "CWsfAiPlugin::AllocateResourcesL" );
    // create resourcefile 
    TFileName resourceFile;
    resourceFile.Append( KResourceDrive );
    resourceFile.Append( KDC_RESOURCE_FILES_DIR );
    resourceFile.Append( KResourceFile );

    BaflUtils::NearestLanguageFile( iEnv->FsSession(), resourceFile );
    iResourceFileOffset = iEnv->AddResourceFileL( resourceFile );    
    }

// --------------------------------------------------------------------------
// CWsfAiPlugin::Start
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::Start( TStartReason /*aReason*/ )
    {
    LOG_ENTERFN( "CWsfAiPlugin::Start");
    iController.StartupRefresh();
    }

// --------------------------------------------------------------------------
// CWsfAiPlugin::Stop
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::Stop( TStopReason /*aReason*/ )
    {
    LOG_ENTERFN( "CWsfAiPlugin::Stop" );
    // HS page switch -> dismiss open dialogs/menus
    TRAP_IGNORE( iController.DeInitializeL() );
    if ( iAnimationPeriodic )
        {
        LOG_WRITE( "Cancel animation update" );
        iAnimationPeriodic->Cancel();
        delete iAnimationPeriodic;
        iAnimationPeriodic = NULL;
        }
    }

// --------------------------------------------------------------------------
// CWsfAiPlugin::Resume
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::Resume( TResumeReason aReason )
    {
    LOG_ENTERFN( "CWsfAiPlugin::Resume" );
    if( aReason == EForeground ) 
        {
        // HS came to foreground -> make refresh scan
        if ( !iAiModel->Connected() && !iModel->IsConnecting() )
            {
            LOG_WRITE( "Call Refresh scan" );
        	iActiveWrappers->RefreshScan();
            }
        else
            {
            LOG_WRITE( "Connected no need to refresh scan results" );
            }
        }
    }

// --------------------------------------------------------------------------
// CWsfAiPlugin::Suspend
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::Suspend( TSuspendReason aReason )
    {
    LOG_ENTERFN( "CWsfAiPlugin::Suspend" );
    
    if( aReason == EBackground )
        {
        if ( iAnimationPeriodic )
            {
            LOG_WRITE( "Cancel animation update" );
            iAnimationPeriodic->Cancel();
            delete iAnimationPeriodic;
            iAnimationPeriodic = NULL;
            }
        }
    }

// --------------------------------------------------------------------------
// CWsfAiPlugin::SubscribeL
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::SubscribeL( MAiContentObserver& aObserver )
    {
    LOG_ENTERFN( "CWsfAiPlugin::Subscribe" );
    iObservers.AppendL( &aObserver );
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::ConfigureL
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::ConfigureL( RAiSettingsItemArray& /*aSettings*/ )
    {
    LOG_ENTERFN( "CWsfAiPlugin::ConfigureL" );
    }

// --------------------------------------------------------------------------
// CWsfAiPlugin::GetPropertyL
// --------------------------------------------------------------------------
//
TAny* CWsfAiPlugin::GetProperty( TProperty aProperty )
    {
    if( aProperty == EPublisherContent )
        {
        return iContent;
        }
    else if( aProperty == EPublisherEvents )
        {
        return iEvents;
        }

    return NULL;
    }

// --------------------------------------------------------------------------
// CWsfAiPlugin::HandleEvent
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::HandleEvent( TInt aEvent, const TDesC& /*aParam*/ )
    {
    LOG_ENTERFN( "CWsfAiPlugin::HandleEvent" );
    switch ( aEvent )
        {
        case EAiWizardEventLaunchApplication:
            {
            TRAP_IGNORE( LaunchApplicationL() );
            break;
            }
            
        case EAiWizardGainFocusInfoArea:
            {
            iInfoAreaFocus = ETrue;
            TRAP_IGNORE( SetMskL() );
            break;
            }
            
        case EAiWizardLostFocusInfoArea:
            {
            iInfoAreaFocus = EFalse;
            break;            
            }
            
        case EAiWizardOpenMenu:
            {
            if ( !iRefreshing )
                {
                TRAP_IGNORE( iController.HandleSelectionKeyL() );
                }
            break;
            }
            
        default:
            {
            }
        }
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::SetScanningState
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::SetScanningState( TBool aScanState )
    {
    LOG_ENTERFN( "CWsfAiPlugin::SetScanningState" );
    iScanState = aScanState;
    TRAP_IGNORE( SetMskL() );
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::SetRefreshingL
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::SetRefreshingL( TBool aRefreshing )
    {
    LOG_ENTERFN( "CWsfAiPlugin::SetRefreshingL" );
    if( iRefreshing != aRefreshing )
        {
        LOG_WRITEF( "iRefreshing = %d and aRefreshing = %d ", 
                     iRefreshing, aRefreshing );
        if( aRefreshing )
            {
            if ( iAnimationPeriodic )
                {
                iAnimationPeriodic->Cancel();
                delete iAnimationPeriodic;
                iAnimationPeriodic = NULL;
                }
            iPublishNetworkStatusText = ETrue;
            iAiModel->InitializeRefreshAnimation();
            iAnimationPeriodic = CPeriodic::NewL(
                    CActive::EPriorityStandard );
                        
            iAnimationPeriodic->Start(
                    TTimeIntervalMicroSeconds32( KRefreshStepTime ),
                    TTimeIntervalMicroSeconds32( KRefreshStepTime ),
                    TCallBack( 
                            CWsfAiPlugin::DoRefreshingStepL,
                            this
                            ) );
            }
        else
            {
            if ( iAnimationPeriodic )
                {
                iAnimationPeriodic->Cancel();
                delete iAnimationPeriodic;
                iAnimationPeriodic = NULL;                
                }
            }
        }
    iRefreshing = aRefreshing;
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::SetRefreshingL
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::StartConnectingL()
    {
    LOG_ENTERFN( "CWsfAiPlugin::StartConnectingL" );
    
    if ( iAnimationPeriodic )
        {
        iAnimationPeriodic->Cancel();
        delete iAnimationPeriodic;
        iAnimationPeriodic = NULL;
        }

    iPublishNetworkStatusText = ETrue;
    iAiModel->InitializeConnectingAnimation();
    iAnimationPeriodic = CPeriodic::NewL(
                                    CActive::EPriorityStandard );
                
    iAnimationPeriodic->Start(
            TTimeIntervalMicroSeconds32( KConnectingStepTime ),
            TTimeIntervalMicroSeconds32( KConnectingStepTime ),
            TCallBack( 
                    CWsfAiPlugin::DoConnectingStepL,
                    this
                    ) );
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::NewL
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::LaunchApplicationL()
    {
    LOG_ENTERFN( "CWsfAiPlugin::LaunchApplication" );
    RApaLsSession appArcSession;

    // connect to AppArc server
    User::LeaveIfError( appArcSession.Connect() );
    CleanupClosePushL( appArcSession );


    // check if the app is already running ... and brings it to foreground.
    TUid id( TUid::Uid( KUidSnifferApp.iUid ) );
    TApaTaskList taskList( iEnv->WsSession() );
    TApaTask task = taskList.FindApp( id );

    if ( task.Exists() )
        {
        task.BringToForeground();
           }
    else
        {
        TThreadId threadId;
        User::LeaveIfError( appArcSession.StartDocument( 
                                        KNullDesC, 
                                        TUid::Uid( KUidSnifferApp.iUid ),
                                        threadId ) );
        }
        
    CleanupStack::PopAndDestroy( &appArcSession ); //appArcSession
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::SetMskL
// --------------------------------------------------------------------------
//
void CWsfAiPlugin::SetMskL()
    {
    LOG_ENTERFN( "CWsfAiPlugin::SetMsk" );
    
    if ( AknLayoutUtils::PenEnabled() )
        {
        // MSK not updated in touch products
        return;
        }
    
    CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
    
    if( cba )
        {
        if ( iAiModel->ScanningOn() || iAiModel->Connected() )
            {
            // Show context menu icon
            cba->SetCommandL( CEikButtonGroupContainer::EMiddleSoftkeyPosition, 
                          EAknSoftkeyContextOptions, 
                          KNullDesC );
            }
        else
            {
            if ( !iMskActivate )
                {
                iMskActivate = StringLoader::LoadL( R_QTN_MSK_SNIFFER_ACTIVATE );
                }
            // Show 'Activate'
            cba->SetCommandL( CEikButtonGroupContainer::EMiddleSoftkeyPosition, 
                          EAknSoftkeyOk, 
                          *iMskActivate );
            }
        cba->DrawDeferred();
    	}
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::PublishText
// --------------------------------------------------------------------------
//
TBool CWsfAiPlugin::PublishText( MAiContentObserver* aObserver,
                                 TWsfAiWizardContentIds aContentId,
                                 const TDesC16& aText )
    {
    LOG_ENTERFN( "CWsfAiPlugin::PublishText" );
    TBool published( EFalse );
    TInt error( 0 );
    if ( aObserver->CanPublish( *this, aContentId, aContentId ) )
        {
        error = aObserver->Publish( *this,
                                    aContentId,
                                    aText,
                                    aContentId );

        if ( error == KErrNone )
            {
            published = ETrue;
            }
        }
    return published;
    }


// --------------------------------------------------------------------------
// CWsfAiPlugin::PublishIconL
// --------------------------------------------------------------------------
//
TBool CWsfAiPlugin::PublishIconL( MAiContentObserver* aObserver,
                                  TWsfAiWizardContentIds aContentId,
                                  TInt aIconId )
    {
    LOG_ENTERFN( "CWsfAiPlugin::PublishIcon" );
    TBool published( EFalse );
    TInt error( 0 );
    if ( aObserver->CanPublish( *this, aContentId, aContentId ) )
        {
        error = aObserver->PublishPtr( 
                              *this,
                              aContentId,
                              CWsfIconArrayCreator::GetIconLC( aIconId ),
                              aContentId );

        CleanupStack::Pop(); // pop the icon

        if ( error == KErrNone )
            {
            published = ETrue;
            }
        }
    return published;
    }


// ---------------------------------------------------------------------------
// CWsfAiPlugin::PublishCleanup
// ---------------------------------------------------------------------------
//
void CWsfAiPlugin::PublishCleanup( TAny* aPtr )
    {
    LOG_ENTERFN( "CWsfAiPlugin::PublishCleanup" );
    CWsfAiPlugin* self = static_cast<CWsfAiPlugin*>( aPtr );
    
    if ( self && self->iCurrentObserverIndex < self->iObservers.Count() )
        {
        LOG_WRITE( "Cancel transaction" );
        self->iObservers[self->iCurrentObserverIndex]
                     ->CancelTransaction( reinterpret_cast<TInt32>( self ) );
        }
    }


// ---------------------------------------------------------------------------
// CWsfAiPlugin::DoRefreshingStepL
// ---------------------------------------------------------------------------
//
TInt CWsfAiPlugin::DoRefreshingStepL( TAny* ptr )
	{
	LOG_ENTERFN( "CWsfAiPlugin::DoRefreshingStepL( ptr )" );
	static_cast<CWsfAiPlugin*>( ptr )->DoRefreshingStepL();
	return ETrue;
	}

// ---------------------------------------------------------------------------
// CWsfAiPlugin::DoRefreshingStepL
// ---------------------------------------------------------------------------
//
void CWsfAiPlugin::DoRefreshingStepL()
	{
	LOG_ENTERFN( "CWsfAiPlugin::DoRefreshingStepL" );
	
	if ( !iAiModel->ScanningOn() )
	    {
	    LOG_WRITE( "Disable refreshing" );
	    if ( iAnimationPeriodic )
	        {
	        LOG_WRITE( "Cancel animation update" );
	        iAnimationPeriodic->Cancel();
	        delete iAnimationPeriodic;
	        iAnimationPeriodic = NULL;
	        }
	    iRefreshing = EFalse;
	    }
	else
	    {
	    iAiModel->AnimateRefresh();
	    iController.RefreshRefreshingL();
	    }	
	}

// ---------------------------------------------------------------------------
// CWsfAiPlugin::DoConnectingStepL
// ---------------------------------------------------------------------------
//
TInt CWsfAiPlugin::DoConnectingStepL( TAny* ptr )
    {
    LOG_ENTERFN( "CWsfAiPlugin::DoConnectingStepL( ptr )" );
    static_cast<CWsfAiPlugin*>( ptr )->DoConnectingStepL();
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CWsfAiPlugin::DoConnectingStepL
// ---------------------------------------------------------------------------
//
void CWsfAiPlugin::DoConnectingStepL()
    {
    LOG_ENTERFN( "CWsfAiPlugin::DoConnectingStepL" );
    
    if ( !iModel->IsConnecting() )
        {
        LOG_WRITE( "Disable refreshing" );
        if ( iAnimationPeriodic )
            {
            LOG_WRITE( "Cancel animation update" );
            iAnimationPeriodic->Cancel();
            delete iAnimationPeriodic;
            iAnimationPeriodic = NULL;
            }
        }
    else
        {
        iAiModel->AnimateConnecting();
        iController.RefreshConnectingL();
        }   
    }

// ---------------------------------------------------------------------------
// CWsfAiPlugin::PublishStatusIconL
// ---------------------------------------------------------------------------
//
void CWsfAiPlugin::PublishStatusIconL( CArrayFix<TInt>* aPublishIconArray, 
                                       CEikLabel* aText1 )
    {
    LOG_ENTERFN( "CWsfAiPlugin::PublishStatusIconL" );
    TBool published( EFalse );
    TInt iconId( 0 );

    for ( iCurrentObserverIndex = 0; 
          iCurrentObserverIndex < iObservers.Count(); 
          ++iCurrentObserverIndex )
        {
        MAiContentObserver* observer = iObservers[iCurrentObserverIndex];
        
        // Publish Status icon: EAiWizardContentStatusIcon
        iconId = aPublishIconArray->At( 0 );
        published = PublishIconL( observer, 
                                  EAiWizardContentStatusIcon,
                                  iconId ) || published;
        
        if ( iPublishNetworkStatusText && aText1 )
            {
            iPublishNetworkStatusText = EFalse;
            HBufC *statusText = aText1->Text()->AllocLC();
            published = PublishText( observer, 
                                     EAiWizardContentNetworkStatus,
                                     *statusText ) || published;
            CleanupStack::PopAndDestroy( statusText );
            }
        
        observer->Clean( 
                *this, 
                EAiWizardContentStrengthIcon, 
                EAiWizardContentStrengthIcon );
        
        observer->Clean( 
                *this, 
                EAiWizardContentSecureIcon, 
                EAiWizardContentSecureIcon );
        
        observer->Clean( 
                *this, 
                EAiWizardContentNetworkName, 
                EAiWizardContentNetworkName );
        
        // If we published something then commit, 
        // otherwise cancel transaction
        if ( published )
            {
            published = EFalse;
            }
        }
    }


// End of File.
