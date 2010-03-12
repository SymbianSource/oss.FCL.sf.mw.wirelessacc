/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  WlanPlugin class.
*
*/


// INCLUDE FILES

// User includes
#include "wlanplugin.h"
#include "wlanpluginwlanstate.h"
#include "wlanpluginlogger.h"

// System includes
#include <wlanpluginrsc.rsg>
#include <gsparentplugin.h>
#include <gsfwviewuids.h>
#include <AknNullService.h>
#include <bautils.h>
#include <StringLoader.h>
#include <featmgr.h>
#include <AknGlobalNote.h>
#include <ErrorUI.h>


// Resource file location
_LIT( KWlanPluginResourceFileName, "z:wlanpluginrsc.rsc" );

// ROM folder
_LIT( KDriveZ, "z:" );

// Name of the MBM file containing icons
_LIT( KFileIcons, "wlanplugin.mif");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CWlanPlugin::NewL
// First phase constructor
// ---------------------------------------------------------
CWlanPlugin* CWlanPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CWlanPlugin* self = new ( ELeave ) CWlanPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;     
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
CWlanPlugin::~CWlanPlugin()
    {
    CLOG_ENTERFN( "CWlanPlugin::~CWlanPlugin()" );  
            
    iResources.Close();
    
    if( iNullService )
        {
        delete iNullService;
        }
    if ( iWlanState )
        {
        delete iWlanState;
        }
        
    CLOG_ENTERFN( "CWlanPlugin::~CWlanPlugin()" );    
    }

// ---------------------------------------------------------
// CWlanPlugin::ConstructL
// ---------------------------------------------------------
TAny CWlanPlugin::ConstructL()
    {
    CLOG_CREATE;
    
    CLOG_ENTERFN( "CWlanPlugin::ConstructL()" );
    
    BaseConstructL(); 
    
    OpenLocalizedResourceFileL();    
                                
    // Check Wlan support in construction phase to
    // avoid creating of plugin when it is not necessary. 
    if( GetWlanSupported() ) 
        {
        // This is the parent plugin
        CGSParentPlugin* parent = static_cast<CGSParentPlugin*>(
                 AppUi()->View( KGSConPluginUid ) );
            
        iWlanState = CWlanPluginWlanState::NewL( parent );
        }
    else 
        {
        User::Leave( KErrNotSupported );
        }
    
    CLOG_LEAVEFN( "CWlanPlugin::ConstructL()" );
    }
    
// ---------------------------------------------------------
// CWlanPlugin::Id
// ---------------------------------------------------------
TUid CWlanPlugin::Id() const
    {
    CLOG_ENTERFN( "CWlanPlugin::Id()" );
    
    CLOG_LEAVEFN( "CWlanPlugin::Id()" );
    
    return KWlanPluginUid;
    
    }
        
// ---------------------------------------------------------
// CWlanPlugin::DoActivateL
// ---------------------------------------------------------
void CWlanPlugin::DoActivateL( const TVwsViewId& /*aPrevViewId*/, 
                                       TUid /*aCustomMessageId*/, 
                                       const TDesC8& /*aCustomMessage*/ )
    {
    CLOG_ENTERFN( "CWlanPlugin::DoActivateL()" );
    //no implementation required
    CLOG_LEAVEFN( "CWlanPlugin::DoActivateL()" );
    }

// ---------------------------------------------------------
// CWlanPlugin::DoDeactivate
// ---------------------------------------------------------
void CWlanPlugin::DoDeactivate()
    {
    CLOG_ENTERFN( "CWlanPlugin::DoDeactivate()" );
    //no implementation required
    CLOG_LEAVEFN( "CWlanPlugin::DoDeactivate()" );
    }

// ---------------------------------------------------------
// CWlanPlugin::GetCaptionL
// ---------------------------------------------------------
void CWlanPlugin::GetCaptionL( TDes& aCaption ) const
    {
    CLOG_ENTERFN( "CWlanPlugin::GetCaptionL()" );
    
    StringLoader::Load( aCaption, R_QTN_SET_FOLDER_CONN_WLAN );
    
    CLOG_LEAVEFN( "CWlanPlugin::GetCaptionL()" );    
    }

// ---------------------------------------------------------
// CWlanPlugin::HasBitmap
// ---------------------------------------------------------
TBool CWlanPlugin::HasBitmap() const
    {
    CLOG_ENTERFN( "CWlanPlugin::HasBitmap()" );
    
    CLOG_LEAVEFN( "CWlanPlugin::HasBitmap()" ); 
    
    return EFalse;
    }

// ---------------------------------------------------------
// CWlanPlugin::GetBitmapL
// ---------------------------------------------------------
void CWlanPlugin::GetBitmapL( CFbsBitmap* /*aBitmap*/, 
                                      CFbsBitmap* /*aMask*/ ) const
    {
    
    CLOG_ENTERFN( "CWlanPlugin::GetBitmapL()" );
    CLOG_LEAVEFN( "CWlanPlugin::GetBitmapL()" );    
    //no bitmap
    User::Leave( KErrNotFound );
    }
    
// ---------------------------------------------------------
// CWlanPlugin::PluginProviderCategory
// ---------------------------------------------------------
TInt CWlanPlugin::PluginProviderCategory() const
    {
    
    CLOG_ENTERFN( "CWlanPlugin::PluginProviderCategory()" );
    CLOG_LEAVEFN( "CWlanPlugin::PluginProviderCategory()" ); 
       
    return EGSPluginProviderOEM;
    }
    
// ---------------------------------------------------------
// CWlanPlugin::HandleSelection
// ---------------------------------------------------------   
void CWlanPlugin::HandleSelection(
            const TGSSelectionTypes /*aSelectionType*/ )
    {   
    CLOG_ENTERFN( "CWlanPlugin::HandleSelection()" );    
    
    TRAPD( error, LaunchSnifferAppL() );
    if( error != KErrNone && 
    	error != KErrNotFound &&
    	error != KErrServerTerminated )
        {
        ShowErrorNote( error );
        }
    
    CLOG_LEAVEFN( "CWlanPlugin::HandleSelection()" );     
    }

// ---------------------------------------------------------
// CWlanPlugin::ItemType
// ---------------------------------------------------------
TGSListboxItemTypes CWlanPlugin::ItemType()
    {
    
    CLOG_ENTERFN( "CWlanPlugin::ItemType()" );    
    CLOG_LEAVEFN( "CWlanPlugin::ItemType()" );     
    
    return EGSItemTypeSettingDialog;
    }
    
// ---------------------------------------------------------
// CWlanPlugin::CreateIconL
// ---------------------------------------------------------    
CGulIcon* CWlanPlugin::CreateIconL( const TUid aIconType )
    {

    CLOG_ENTERFN( "CWlanPlugin::CreateIconL()" );  

    TFileName iconsFileName;

    iconsFileName.Append( KDriveZ );
    iconsFileName.Append( KDC_APP_BITMAP_DIR );
    iconsFileName.Append( KFileIcons );
    
    CGulIcon* icon;
       
    TInt pic, picmask;
    TAknsItemID itemid;
    //if the main icon is asked   
    if ( aIconType == KGSIconTypeLbxItem )
        {
        if ( iWlanState->GetMainIconL( pic, picmask, itemid ) )
            {
            icon = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(),
                                              itemid, 
                                              iconsFileName, 
                                              pic, 
                                              picmask );
            }
        else
            {
            icon = CGSPluginInterface::CreateIconL( aIconType );
            }
        }
    //if the secondary icon is asked   
    else if ( aIconType == KGSIconTypeDColumn )
        {
        //if there should be icon
        if ( iWlanState->GetStrengthIconL( pic, picmask, itemid ) )
            {
            icon = CGulIcon::NewLC();
            CFbsBitmap* bitmap;
            CFbsBitmap* mask;

            // Creates bitmap an icon. 
            AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(), 
                                         itemid,
                                         KAknsIIDQsnIconColors, 
                                         EAknsCIQsnIconColorsCG13, 
                                         bitmap,
                                         mask, 
                                         iconsFileName, 
                                         pic, 
                                         picmask, 
                                         KRgbBlack );

            icon->SetBitmap( bitmap );
            icon->SetMask( mask );
            CleanupStack::Pop( icon );
            }
        //if there should be no icons
        else
            {
            icon = CGSPluginInterface::CreateIconL( aIconType );
            }
        }
     //in other case of icons
     else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CLOG_LEAVEFN( "CWlanPlugin::CreateIconL()" );     

    return icon;
    }

// ---------------------------------------------------------
// CVWlanPlugin::Visible
// ---------------------------------------------------------
TBool CWlanPlugin::Visible() const
    {
    
    CLOG_ENTERFN( "CWlanPlugin::Visible()" );    
    CLOG_LEAVEFN( "CWlanPlugin::Visible()" );     
    
    return GetWlanSupported();
    }
    
// ---------------------------------------------------------
// CWlanPlugin::CWlanPlugin
// ---------------------------------------------------------
CWlanPlugin::CWlanPlugin():
    iResources( *CCoeEnv::Static() ),
    iNullService( NULL )
    {
    // no implementation required
    }
    
// ---------------------------------------------------------
// CWlanPlugin::GetValue
// ---------------------------------------------------------
void CWlanPlugin::GetValue( const TGSPluginValueKeys aKey,
                       TDes& aValue )
    {    
    CLOG_ENTERFN( "CWlanPlugin::GetValue()" );    
    
    //if the second line of the listbox is asked
    if( aKey == EGSPluginKeySettingsItemValueString )
        {     
        TRAP_IGNORE( iWlanState->GetStateStringL( aValue ) );
        }
        
    CLOG_LEAVEFN( "CWlanPlugin::GetValue()" );     
    }

// ---------------------------------------------------------
// CWlanPlugin::OpenLocalizedResourceFileL
// ---------------------------------------------------------
void CWlanPlugin::OpenLocalizedResourceFileL()
    {
    CLOG_ENTERFN( "CWlanPlugin::OpenLocalizedResourceFileL()" );    
    
    
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );

    // Find the resource file:
    TParse parse;
    parse.Set( KWlanPluginResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file:
    BaflUtils::NearestLanguageFile( fsSession, fileName );

    // Open resource file:
    iResources.OpenL( fileName );

    // If leave occurs before this, close is called automatically when the
    // thread exits.
    fsSession.Close();
    
    CLOG_LEAVEFN( "CWlanPlugin::OpenLocalizedResourceFileL()" );     
    }
    
// ---------------------------------------------------------
// CWlanPlugin::LaunchSnifferAppL
// ---------------------------------------------------------    
void CWlanPlugin::LaunchSnifferAppL()
    {
    CLOG_ENTERFN( "CWlanPlugin::LaunchSnifferAppL()" );    
    
    // Launch app
    if ( iNullService )
        {
        delete iNullService;
        iNullService = NULL;
        }         
    iNullService = CAknNullService::NewL( KSnifferAppUid, this );
       
    CLOG_LEAVEFN( "CWlanPlugin::LaunchSnifferAppL()" );         
    }

// ---------------------------------------------------------
// CWlanPlugin::GetWlanSupported
// ---------------------------------------------------------
TBool CWlanPlugin::GetWlanSupported() const
    {
    CLOG_ENTERFN( "CWlanPlugin::GetWlanSupported()" );    
     
    TBool isWlanSupported = EFalse;
    
    TRAPD( error, FeatureManager::InitializeLibL() );
    
    if( !error )
        {
        isWlanSupported = 
            FeatureManager::FeatureSupported( KFeatureIdProtocolWlan );
        
        FeatureManager::UnInitializeLib();
        }
    
    CLOG_LEAVEFN( "CWlanPlugin::GetWlanSupported()" );         
    
    return isWlanSupported;
    }

// ---------------------------------------------------------
// CWlanPlugin::ShowErrorNote
// ---------------------------------------------------------
//
void CWlanPlugin::ShowErrorNote(TInt aErrorCode)
    {
    CLOG_ENTERFN( "CWlanPlugin::ShowErrorNote()" );    

    
    TBool errorNoteShowed = EFalse;
        
    TRAPD(error, errorNoteShowed = ShowErrorNoteL(aErrorCode));
            
    if(error || !errorNoteShowed) 
        {
        TRAP_IGNORE( ShowGeneralErrorNoteL() );
        } 
        
    CLOG_LEAVEFN( "CWlanPlugin::ShowErrorNote()" );           
    }

// ---------------------------------------------------------
// CWlanPlugin::ShowErrorNoteL
// ---------------------------------------------------------
//
TBool CWlanPlugin::ShowErrorNoteL(TInt aErrorCode)
    {
    CLOG_ENTERFN( "CWlanPlugin::ShowErrorNoteL()" );    
    
    CErrorUI* errorUi = CErrorUI::NewLC();
    
    TBool errorNoteShowed = errorUi->ShowGlobalErrorNoteL( aErrorCode );
    
    CleanupStack::PopAndDestroy( errorUi );
    
    CLOG_LEAVEFN( "CWlanPlugin::ShowErrorNoteL()" );           
        
    return errorNoteShowed;
    }    

// ---------------------------------------------------------
// CWlanPlugin::ShowGeneralErrorNoteL
// ---------------------------------------------------------
//
void CWlanPlugin::ShowGeneralErrorNoteL()
    {
    CLOG_ENTERFN( "CWlanPlugin::ShowGeneralErrorNoteL()" );    
    
    HBufC *msg = StringLoader::LoadLC( R_QTN_ERR_OS_GENERAL );
    
    CAknGlobalNote *note = CAknGlobalNote::NewLC(); 
    
    note->ShowNoteL( EAknGlobalErrorNote, *msg );

    CleanupStack::PopAndDestroy( note );
    CleanupStack::PopAndDestroy( msg );
    
    CLOG_LEAVEFN( "CWlanPlugin::ShowGeneralErrorNoteL()" );           
    }

// ---------------------------------------------------------
// CWlanPlugin::HandleServerAppExit
// ---------------------------------------------------------
//
void CWlanPlugin::HandleServerAppExit(TInt aReason)
	{
	CLOG_ENTERFN( "CWlanPlugin::HandleServerAppExit()" );  
	//MAknServerAppExitObserver::HandleServerAppExit( aReason );
	if( aReason == EAknCmdExit )
		{
		iAvkonViewAppUi->Exit();
		}
		
	CLOG_LEAVEFN( "CWlanPlugin::HandleServerAppExit()" ); 
	}

    
//End of file
