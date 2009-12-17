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
* Description:   Implementation of CWsfWlanIapWizard
*
*/



//  EXTERNAL INCLUDES
#include <e32std.h>
#include <utf.h>
#include <wlanmgmtcommon.h>
#include <StringLoader.h> 
#include <cmpluginwlandef.h>
#include <cmsettingsui.h>
#include <ConnectionUiUtilities.h>
#include <data_caging_path_literals.hrh>
#include <bautils.h>
#include <AknQueryDialog.h> 
#include <aknnotewrappers.h>
#include <wsfapwizard.rsg>

//  CLASS HEADER
#include "wsfwlaniapwizard.h"

//  INTERNAL INCLUDES
#include "wsfwlaniapcreator.h"
#include "wsfwlanssiddiscovery.h"
#include "wsflogger.h"



using namespace CMManager;


/**
* ROM folder
*/
_LIT( KDriveZ, "z:" );

/**
* Resource file name
*/
_LIT( KResourceFileName, "wsfapwizard.rsc" );
    
/**
* the longest possible key is the 64 character long hex WPA key
*/
static const TInt KMaxSecureKeyLength = 64;



#ifdef _DEBUG
    _LIT( KApWizardPanic, "wsfapwizard" );
    #define __ASSERTD( v )  __ASSERT_DEBUG( (v), \
                                    User::Panic( KApWizardPanic, 0 ) )
#else
    #define __ASSERTD( v )
#endif



// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfWlanIapWizard* CWsfWlanIapWizard::NewL()
    {
    CWsfWlanIapWizard* thisPtr = NewLC();
    CleanupStack::Pop( thisPtr );
    return thisPtr;    
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::NewLC
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfWlanIapWizard* CWsfWlanIapWizard::NewLC()
    {
    CWsfWlanIapWizard* thisPtr = new (ELeave) CWsfWlanIapWizard();
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    return thisPtr;
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::CWsfWlanIapWizard
// ----------------------------------------------------------------------------
//
CWsfWlanIapWizard::CWsfWlanIapWizard():
    iEnv( *CEikonEnv::Static() ),
    iResourceFileOffset( -1 ),
    iWlanInfo( NULL )
    {
    }
   

// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfWlanIapWizard::ConstructL()
    {
    iResourceFileOffset = InitResourcesL( iEnv );    

    iCreator = CWsfWlanIapCreator::NewL();    
    iSsidDiscovery = CWsfWlanSsidDiscovery::NewL();
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::~CWsfWlanIapWizard
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfWlanIapWizard::~CWsfWlanIapWizard()
    {
    delete iCreator;

    ReleaseResources( iEnv, iResourceFileOffset );

    delete iUsedPassword;
    delete iWaitDialog;
    delete iSsidDiscovery;
    }


// ---------------------------------------------------------------------------
// CWsfWlanIapWizard::InitResourcesL
// ---------------------------------------------------------------------------
//
TInt CWsfWlanIapWizard::InitResourcesL( CEikonEnv& aEnv )
    {
    // The resource has to be loaded manually since it is not an application.
    TFileName resourceFileName;

    resourceFileName.Append( KDriveZ );
    resourceFileName.Append( KDC_RESOURCE_FILES_DIR );   
    resourceFileName.Append( KResourceFileName );

    BaflUtils::NearestLanguageFile( aEnv.FsSession(), resourceFileName );
    
    return aEnv.AddResourceFileL( resourceFileName );
    }


// ---------------------------------------------------------------------------
// CWsfWlanIapWizard::ReleaseResources
// ---------------------------------------------------------------------------
//
void CWsfWlanIapWizard::ReleaseResources( CEikonEnv& aEnv, TInt aResOffset )
    {
    if ( aResOffset != -1 )
        {
        aEnv.DeleteResourceFile( aResOffset );
        }
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::LaunchWizardL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfWlanIapWizard::LaunchWizardL( TWsfWlanInfo& aWlanInfo, 
                                                 TBool aDefineNewAccessPoint )
    {
    LOG_ENTERFN( "CWsfWlanIapWizard::LaunchWizardL" );
    LOG_WRITEF( "defineAp = %d", aDefineNewAccessPoint );
    
    if ( aDefineNewAccessPoint && 
         !( aWlanInfo.Hidden() && !aWlanInfo.Known() ) )
        {
        HBufC* ssid16 = aWlanInfo.GetSsidAsUnicodeLC();
        LOG_WRITEF( "ssid = %S", ssid16 );
        HBufC* infoText = StringLoader::LoadLC( 
                                     R_QTN_CMON_QUEST_CREATE_WLAN_IAP, 
                                     *ssid16 ); 

        CAknQueryDialog* confirmQuery = CAknQueryDialog::NewL();
        TBool result = confirmQuery->ExecuteLD( 
                                     R_QTN_WIZARD_CONFIRMATION_QUERY,
                                     *infoText );
            
        CleanupStack::PopAndDestroy( infoText );
        CleanupStack::PopAndDestroy( ssid16 );        
        
        if ( !result )
            {
            return result;
            }
        }

    iWlanInfo = &aWlanInfo;    
    iDefineNewAccessPoint = aDefineNewAccessPoint;
        
    TRAPD( err, AskNetworkDetailsL() );
    if ( err )
        {
        // We are here also if selection of 
        // the hidden network AP leaves or is 
        // cancelled by the end-user
        LOG_WRITE( "AskNetworkDetailsL failed" );
        }
        
    return ( !err );
    }
    

// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::CreateAccessPointL
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CWsfWlanIapWizard::CreateAccessPointL()
    {
    LOG_ENTERFN( "CWsfWlanIapWizard::CreateAccessPointL" );
    __ASSERTD( iWlanInfo );
    
    
    if ( iWlanInfo->iIapId != 0 )    
        {
        // Access Point already exists
        User::Leave( KErrAlreadyExists );
        }    
 
    
    // otherwise new Access Point will be created
    if ( !iWlanInfo->iSsid.Length() )
        {
        User::Leave( KErrGeneral );
        }
        
    TPtrC sharedKey = iUsedPassword ? *iUsedPassword : KNullDesC();

    TUint32 destId( 0 );

    if ( iDefineNewAccessPoint )
        {
        // define access point was selected, select destination query follows
        LOG_WRITE( "defineAp = ETrue, opening select destination dialog..." );
        
        CCmSettingsUi* settings = CCmSettingsUi::NewL();
        CleanupStack::PushL( settings );
        
        TBool result( EFalse );
        result = settings->SelectDestinationDlgL( destId );
        
        CleanupStack::PopAndDestroy( settings ); //settings
        
        if ( !result )
            {
            LOG_WRITE( "dialog cancelled" );
            // cancel was pressed, abort creation
            return EFalse;
            }
        LOG_WRITEF( "target destination = %d", destId );
        }
    
    iCreator->SetDefaultDestination( destId );
    
    iCreator->CreateAccessPointL( *iWlanInfo, sharedKey, iIsHexWep );
    
    return ETrue;
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::QueryTextL
// ----------------------------------------------------------------------------
//
TInt CWsfWlanIapWizard::QueryTextL( TInt aQueryStringResource, 
                                    TDes& aVariable, 
                                    const TInt &aMaxLength )
    {
    LOG_ENTERFN( "CWsfWlanIapWizard::QueryTextL" );
    HBufC* queryText = StringLoader::LoadLC( aQueryStringResource );
    CAknTextQueryDialog* textQuery = CAknTextQueryDialog::NewL( aVariable );
    CleanupStack::PushL( textQuery );
    textQuery->SetPromptL( *queryText );
    textQuery->SetMaxLength( aMaxLength );
    CleanupStack::Pop( textQuery );
    TInt result = textQuery->ExecuteLD( R_QTN_WIZARD_QUERY_GENERAL );    
    CleanupStack::PopAndDestroy( queryText );
    return result;
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::QuerySecureKeyL
// ----------------------------------------------------------------------------
//
TBool CWsfWlanIapWizard::QuerySecureKeyL()
    {
    LOG_ENTERFN( "CWsfWlanIapWizard::QuerySecureKeyL" );
    TBool okPressed( ETrue );

    switch ( iWlanInfo->iSecurityMode )
        {
        case EWlanSecModeWep:
            {
            LOG_WRITE( "secmode WEP" );
            CConnectionUiUtilities* connUiUtils = 
                                            CConnectionUiUtilities::NewL();
            CleanupStack::PushL( connUiUtils );
            iUsedPassword = HBufC::NewL( KMaxSecureKeyLength );
            TPtr ptr( iUsedPassword->Des() );
            okPressed = connUiUtils->EasyWepDlg( &ptr, iIsHexWep );
            CleanupStack::PopAndDestroy( connUiUtils );
            break;
            }
            
        case EWlanSecModeWpa:
        case EWlanSecModeWpa2:
        case EWlanSecMode802_1x:
            {
            if ( iWlanInfo->UsesPreSharedKey() )
                {
                // WPA-PSK
                LOG_WRITE( "secmode WPA-PSK" );
                
                CConnectionUiUtilities* connUiUtils = 
                                            CConnectionUiUtilities::NewL();
                CleanupStack::PushL( connUiUtils );
                iUsedPassword = HBufC::NewL( KMaxSecureKeyLength );
                TPtr ptr( iUsedPassword->Des() );
                okPressed = connUiUtils->EasyWpaDlg( &ptr );
                CleanupStack::PopAndDestroy( connUiUtils );
                }
            else
                {
                // no key prompt in EAP mode, but the usual note is shown
                LOG_WRITE( "secmode WPA-EAP" );

                HBufC* eapText = StringLoader::LoadLC( 
                                 R_QTN_NETW_CONSET_INFO_EAP_SETTINGS_DEFAULT );
                CAknInformationNote* informationNote = 
                                     new (ELeave) CAknInformationNote( ETrue );
                informationNote->ExecuteLD( *eapText );
                
                CleanupStack::PopAndDestroy( eapText );
                }
            
            break;                
            }
        
        case EWlanSecModeOpen:
        default:
            {
            LOG_WRITE( "secmode Open" );

            delete iUsedPassword;
            iUsedPassword = NULL;
            }
        }
        
    return okPressed;
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::DismissWaitNote
// ----------------------------------------------------------------------------
//
void CWsfWlanIapWizard::DismissWaitNote( TAny* aPtr )
    {
    LOG_ENTERFN( "CWsfWlanIapWizard::DismissWaitNote" );
    CWsfWlanIapWizard* self = static_cast<CWsfWlanIapWizard*>( aPtr );
    
    if ( self->iWaitDialog )
        {
        // close waitnote
        LOG_WRITE( "closing waitnote..." );
        TRAP_IGNORE( self->iWaitDialog->ProcessFinishedL() );

        // cleanup
        delete self->iWaitDialog;
        self->iWaitDialog = NULL;        
        }
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::AskNetworkDetailsL
// ----------------------------------------------------------------------------
//
void CWsfWlanIapWizard::AskNetworkDetailsL()
    {
    LOG_ENTERFN( "CWsfWlanIapWizard::AskNetworkDetailsL" );
    
   
    // check the wlan info, and make proper questions....
    if ( iWlanInfo->Hidden() && !iWlanInfo->Known() )
        {
        LOG_WRITE( "unlisted network" );
        
        // query network SSID
        HBufC* ssid16 = AskSsidL();
        LOG_WRITEF( "SSID: [%S]", ssid16 );

        // update SSID        
        CnvUtfConverter::ConvertFromUnicodeToUtf8( iWlanInfo->iSsid, *ssid16 );
        delete ssid16;

        // pop up waitnote
        iWaitDialog = new ( ELeave ) CAknWaitDialog( 
                       reinterpret_cast<CEikDialog**>( &iWaitDialog ), ETrue );
        iWaitDialog->SetCallback( this );
        iWaitDialog->ExecuteLD( R_SEARCHING_WLAN_WAIT_NOTE ); 
        
        CleanupStack::PushL( TCleanupItem( DismissWaitNote, this ) );
        
        // start to scan for surrounding WLAN networks
        TBool foundSsid( EFalse );
        TBool usePsk( EFalse );
        TWlanSsid ssid( iWlanInfo->iSsid );

        // make a broadcast scan
        LOG_WRITE( "broadcast scanning for SSID" );
        foundSsid = iSsidDiscovery->ScanForSsidL( ssid, ETrue, 
                                         iWlanInfo->iNetMode, 
                                         iWlanInfo->iSecurityMode, 
                                         usePsk );

        // SSID found?
        if ( !foundSsid )
            {
            // must be really hidden, direct scan for SSID
            LOG_WRITE( "broadcast scan failed, direct scanning for SSID" );
            foundSsid = iSsidDiscovery->ScanForSsidL( ssid, EFalse, 
                                             iWlanInfo->iNetMode, 
                                             iWlanInfo->iSecurityMode, 
                                             usePsk );

            // it can only be found as hidden
            iWlanInfo->iVisibility = EFalse;

            CleanupStack::PopAndDestroy( 1 ); // DismissWaitNote
            
            if ( !foundSsid )
                {
                LOG_WRITE( "direct scan failed, entering fully manual mode" );
                // pop up info note: No WLAN networks found
                HBufC* infoText = StringLoader::LoadLC( 
                                          R_QTN_WLAN_INFO_NO_NETWORKS_FOUND );
                CAknInformationNote* informationNote = 
                                    new (ELeave) CAknInformationNote( ETrue );
                informationNote->ExecuteLD( *infoText );
                CleanupStack::PopAndDestroy( infoText );
                
                
                // now entering fully manual mode

                // query network mode
                TInt mode( 0 );
                CAknListQueryDialog* listQuery = 
                                  new (ELeave) CAknListQueryDialog( &mode );
                listQuery->PrepareLC( R_QTN_WIZARD_NETWORK_MODE );
                TInt retval = listQuery->RunLD();
                if ( !retval )
                    {
                    User::Leave( KErrCancel );
                    }
                
                const TInt KModeInfraPublic = 0;
                const TInt KModeInfraHidden = 1;
                const TInt KModeAdhocPublic = 2;
                
                TBool adhoc( EFalse );
                
                switch ( mode )
                    {
                    case KModeInfraPublic: 
                        {
                        LOG_WRITE( "infra public mode" );
                        iWlanInfo->iNetMode = EInfra;
                        iWlanInfo->iVisibility = ETrue;
                        break;
                        }

                    case KModeInfraHidden: 
                        {
                        LOG_WRITE( "infra hidden mode" );
                        iWlanInfo->iNetMode = EInfra;
                        iWlanInfo->iVisibility = EFalse;
                        break;
                        }
                        
                    case KModeAdhocPublic: 
                        {
                        LOG_WRITE( "adhoc public mode" );
                        adhoc = ETrue;
                        iWlanInfo->iNetMode = EAdhoc;
                        iWlanInfo->iVisibility = ETrue;
                        break;
                        }
                    }

            
                // query security mode
                const TInt KSecModeOpen = 0;
                const TInt KSecModeWep = 1;
                const TInt KSecMode8021x = 2;
                const TInt KSecModeWpa = 3;

                mode = KSecModeOpen;
                listQuery = new (ELeave) CAknListQueryDialog( &mode );
                
                if ( adhoc )
                    {
                    listQuery->PrepareLC( R_QTN_WIZARD_AUTH_MODE_AD_HOC );
                    }
                else
                    {
                    listQuery->PrepareLC( R_QTN_WIZARD_AUTH_MODE );
                    }

                retval = listQuery->RunLD();

                if ( !retval )
                    {
                    User::Leave( KErrCancel );
                    }
                    
                // map the security mode
                
                switch ( mode )
                    {
                    case KSecModeWep:
                        {
                        LOG_WRITE( "security: WEP" );
                        iWlanInfo->iSecurityMode = EWlanSecModeWep;
                        break;
                        }
                    case KSecMode8021x:
                        {
                        LOG_WRITE( "security: 802.1x" );
                        iWlanInfo->iSecurityMode = EWlanSecMode802_1x;
                        break;    
                        }
                    case KSecModeWpa:
                        {
                        LOG_WRITE( "security: WPA" );
                        iWlanInfo->iSecurityMode = EWlanSecModeWpa;
                        break;    
                        }

                    case KSecModeOpen:
                    default:
                        {
                        LOG_WRITE( "security: Open" );
                        iWlanInfo->iSecurityMode = EWlanSecModeOpen;
                        }
                    }                

                // if WPA, query WPA mode
                if ( iWlanInfo->iSecurityMode == EWlanSecModeWpa )
                    {
                    const TInt KWpaModeEap = 0;
                    const TInt KWpaModePsk = 1;

                    mode = KWpaModeEap;
                    listQuery = new (ELeave) CAknListQueryDialog( &mode );
                    listQuery->PrepareLC( R_QTN_WIZARD_WPA_MODE );
                    retval = listQuery->RunLD();
                    if ( !retval )
                        {
                        User::Leave( KErrCancel );
                        }
                    
                    // 0 - EAP, 1 - PSK    
                    
                    usePsk = ( mode == KWpaModePsk );
                    if ( usePsk )
                        {
                        LOG_WRITE( "WPA-PSK" );
                        }
                    else
                        {
                        LOG_WRITE( "WPA-EAP" );
                        }
                    }

                } // if !foundssid (direct)

            
            } // if !foundssid (broadcast)
        else
            {
            // it was thought to be hidden but actually it isn't
            LOG_WRITE( "broadcast scan succeeded, "
                       L"network isn't hidden as it was thought" );
            LOG_WRITEF( "netmode: %d, secmode: %d, usepsk: %d", 
                        iWlanInfo->iNetMode,
                        iWlanInfo->iSecurityMode,
                        usePsk );
            
            CleanupStack::PopAndDestroy( 1 ); // DismissWaitNote

            // set WLAN to public
            iWlanInfo->iVisibility = ETrue;
            }

        // convey PSK settings to iwlaninfo 
        // (netmode and secmode are already updated)
        iWlanInfo->SetUsesPreSharedKey( usePsk );
        
        }
    else
        {
        // if NOT hidden
        LOG_WRITE( "visible network" );
        LOG_WRITEF( "netmode: %d, secmode: %d, usepsk: %d", 
        iWlanInfo->iNetMode,
        iWlanInfo->iSecurityMode,
        iWlanInfo->UsesPreSharedKey() );
        }

    // query the key and leave if cancelled
    if ( !QuerySecureKeyL() )
        {
        User::Leave( KErrCancel );
        }

    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::AskSsidL
// ----------------------------------------------------------------------------
//
HBufC* CWsfWlanIapWizard::AskSsidL()
    {
    LOG_ENTERFN( "CWsfWlanIapWizard::AskSsidL" );
    
    HBufC* apName = HBufC::NewLC( KWlanMaxSsidLength );    
    TPtr namePtr( apName->Des() );
    if ( iWlanInfo->iSsid.Length() )
        {
        CnvUtfConverter::ConvertToUnicodeFromUtf8( namePtr, iWlanInfo->iSsid );
        }
    TInt response = QueryTextL( R_QTN_ENTER_NETWORK_SSID, 
                                namePtr, 
                                KWlanMaxSsidLength );
    if ( response != EEikBidOk )
        {    
        User::Leave( KErrCancel );
        }

    CleanupStack::Pop( apName ); 
    return apName;
    }


// ----------------------------------------------------------------------------
// CWsfWlanIapWizard::DialogDismissedL
// ----------------------------------------------------------------------------
//
void CWsfWlanIapWizard::DialogDismissedL( TInt aButtonId )
    {
    LOG_ENTERFN( "CWsfWlanIapWizard::DialogDismissedL" );
    iWaitDialog = NULL;
    
    if ( aButtonId == EEikBidCancel )
        {
        // dialog was cancelled by the user
        LOG_WRITE( "dialog was cancelled" );
        iSsidDiscovery->AbortScanning();        
        }    
    }


