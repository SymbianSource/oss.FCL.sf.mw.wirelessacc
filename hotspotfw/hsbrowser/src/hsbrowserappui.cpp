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
* Description:   The UI class for HotSpot Browser Application.
*
*/



// INCLUDE FILES
#include <avkon.hrh>
#include <es_enum.h>
#include <hsbrowser.rsg>
#include <akntitle.h>
#include <AknUtils.h>
#include <hlplch.h>

#include "hsbrowserappui.h"
#include "hsbrowsercontainer.h" 
#include "hsbrowserdocument.h"
#include "hsbrowsercommon.h"
#include "am_debug.h"

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CHsBrowserAppUi::ConstructL()
// ----------------------------------------------------------
//
void CHsBrowserAppUi::ConstructL()
    {
    DEBUG( "CHsBrowserAppUi::ConstructL()" );
    BaseConstructL( CAknAppUi::EAknEnableSkin );
    CHsBrowserModel* model;
    model = static_cast<CHsBrowserDocument*>(iDocument)->Model();
    iAppContainer = new (ELeave) CHsBrowserContainer( model );
    iAppContainer->SetMopParent( this );
    iAppContainer->ConstructL( ClientRect() );
    AddToStackL( iAppContainer );
    }

// ----------------------------------------------------
// CHsBrowserAppUi::~CHsBrowserAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------
//
CHsBrowserAppUi::~CHsBrowserAppUi()
    {
    DEBUG( "CHsBrowserAppUi::~CHsBrowserAppUi()" );
    if ( iAppContainer )
        {
        RemoveFromStack( iAppContainer );
        delete iAppContainer;
        }        
   }

// ----------------------------------------------------
// CHsBrowserAppUi::ProcessCommandParametersL
// ----------------------------------------------------
//
TBool CHsBrowserAppUi::ProcessCommandParametersL(
    CApaCommandLine& aCommandLine )
    {
    DEBUG( "CHsBrowserAppUi::ProcessCommandParametersL()" );
    
    TApaTaskList taskList( CEikonEnv::Static()->WsSession() );
	TApaTask task = taskList.FindApp( KUidHsBrowserApp );
    task.BringToForeground();
    
    TBuf<KBufSize> args;
    args.Copy( aCommandLine.DocumentName() );

    if ( args.Length() == 0 )
    {
        // No arguments, meaning started from application menu.
        // Continue normally, for testing purposes
        return EFalse;	
    }    
    
    TLex wordLex( args );
    TPtrC word;
    // get IAP ID
    word.Set( wordLex.NextToken() );
    TInt iapId;
    TLex x1( word );
    TInt err = x1.Val( iapId );
    if ( err != KErrNone )
        {
        DEBUG1( "CHsBrowserAppUi::ProcessCommandParametersL() x1.Val(), err=%d",
            err );
        }
    // get net ID
    word.Set( wordLex.NextToken() );
    TInt netId;
    TLex x2( word );
    err = x2.Val( netId );
    if ( err != KErrNone )
        {
        DEBUG1( "CHsBrowserAppUi::ProcessCommandParametersL() x2.Val(), err=%d",
            err );
        }
    // get URL
    TPtrC url( wordLex.NextToken() );
    
    err = iAppContainer->LoadRedirect( url, iapId, netId );
    if ( err != KErrNone )
        {
        DEBUG1( "CHsBrowserAppUi::ProcessCommandParametersL() LoadRedirect(), err=%d",
            err );
        }
	return EFalse;
}

// ----------------------------------------------------
// CHsBrowserAppUi::HandleCommandL(TInt aCommand)
// ----------------------------------------------------
//
void CHsBrowserAppUi::HandleCommandL( TInt aCommand )
    {
    DEBUG1( "CHsBrowserAppUi::HandleCommandL() aCommmand=%d", aCommand );
    
    switch ( aCommand )
        {
        case EAknSoftkeyCancel:
            {          
            iAppContainer->HandleCommandL( aCommand );
            Exit();                	
            break;
            }
        case EAknSoftkeyClose:
            {
            // Now for sure we know that we are in input box. 
            // Next close aCommand should be treated as EAknSoftkeyCancel
            iAppContainer->SetActiveInputBox( ETrue );
            // change softkey to Cancel...
        	CEikButtonGroupContainer* cba = CEikonEnv::Static()->
                AppUiFactory()->Cba();
            if ( cba )
            	{
                cba->SetCommandSetL( R_AVKON_SOFTKEYS_CANCEL );
                cba->DrawNow();
            	}
        	break;
            }
        case EAknSoftkeyExit:
        case EAknSoftkeyBack:
        case EEikCmdExit:
            {
            iAppContainer->HandleCommandL( aCommand );
            Exit();
            break;
            }
        case EAknCmdHelp:
            {
            HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(),
                AppHelpContextL());
            break;
            }

        default:
            iAppContainer->HandleCommandL( aCommand );
            break;      
        }
    }
    
// ----------------------------------------------------
// CHsBrowserAppUi::HandleKeyEventL()
// ----------------------------------------------------
//
TKeyResponse CHsBrowserAppUi::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    //DEBUG( "CHsBrowserAppUi::HandleKeyEventL()" )
    return iAppContainer->HandleKeyEventL(aKeyEvent, aType);
    }
 
// ---------------------------------------------------------
// CHsBrowserAppUi::HandleForegroundEventL()
// ---------------------------------------------------------
//
void CHsBrowserAppUi::HandleForegroundEventL( TBool aForeground )
    {
    DEBUG( "CHsBrowserAppUi::HandleForegroundEventL()" );
 
    CAknAppUi::HandleForegroundEventL( aForeground );
    if ( iAppContainer )
   	    {
        iAppContainer->ProcessForegroundEvent( aForeground );
        }
    }
    
    
// End of File  
