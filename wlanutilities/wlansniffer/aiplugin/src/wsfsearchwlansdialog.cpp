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
* Description:  Implementation of CWsfSearchWlansDialog
*
*/


//  EXTERNAL INCLUDES
#include <badesca.h>
#include <coemain.h>
#include <eiklbx.h>
#include <eikclbd.h>
#include <eikclb.h>
#include <AknIconArray.h>
#include <wsfaipluginrsc.rsg>
//  INTERNAL INCLUDES
#include "wsfsearchwlansdialog.h"

//  MACROS
#ifdef _DEBUG    
    _LIT( KSearchDialogPanic, "CWsfSearchWlansDialog" );
    #define _ASS_D( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KSearchDialogPanic, __LINE__) )
#else
    #define _ASS_D( cond ) {}
#endif // DEBUG    


//  CONSTRUCTION AND DESTRUCTION
// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::NewL
// --------------------------------------------------------------------------
//    
CWsfSearchWlansDialog* CWsfSearchWlansDialog::NewL( TInt &aSelectedItem )
    {
    CWsfSearchWlansDialog* thisPtr = NewLC( aSelectedItem );
    CleanupStack::Pop( thisPtr );
    return thisPtr;
    }
    
// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::NewLC
// --------------------------------------------------------------------------
//    
CWsfSearchWlansDialog* CWsfSearchWlansDialog::NewLC( TInt &aSelectedItem )
    {
    CWsfSearchWlansDialog* thisPtr = 
                    new ( ELeave ) CWsfSearchWlansDialog( aSelectedItem );
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    return thisPtr;
    }

// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::~CWsfSearchWlansDialog
// --------------------------------------------------------------------------
//    
CWsfSearchWlansDialog::~CWsfSearchWlansDialog()
    {
    }
    
// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::CWsfSearchWlansDialog
// --------------------------------------------------------------------------
//    
CWsfSearchWlansDialog::CWsfSearchWlansDialog( TInt &aSelectedItem ) : 
    CAknListQueryDialog( &aSelectedItem ),
    iSelectedItem( &aSelectedItem )
    {
    }
    
// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::ConstructL
// --------------------------------------------------------------------------
//    
void CWsfSearchWlansDialog::ConstructL()
    {
    // nothing at the moment...
    }
    
// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::ProcessCommandL
// --------------------------------------------------------------------------
//    
void CWsfSearchWlansDialog::ProcessCommandL( TInt aCommandId )
    {
    CAknListQueryDialog::ProcessCommandL( aCommandId );    
    }
    
// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::UpdateHotSpotsL
// --------------------------------------------------------------------------
//    
void CWsfSearchWlansDialog::UpdateHotSpotsL( MDesCArray* aItemTextArray,
                                             TInt aCurrentItem )
    {
    _ASS_D( ListBox() );
    
    SetItemTextArray( aItemTextArray );
    SetOwnershipType( ELbmDoesNotOwnItemArray );
    ListBox()->HandleItemAdditionL();
    if ( aCurrentItem < 0 || aCurrentItem >= aItemTextArray->MdcaCount() )
        {
        ListBox()->ClearSelection();
        }
    else
        {
        ListBox()->SetCurrentItemIndex( aCurrentItem );     
        }    
    }
    
// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::SelectedItem
// --------------------------------------------------------------------------
//    
TInt CWsfSearchWlansDialog::SelectedItem()
    {
    _ASS_D( ListBox() );

    return ListBox()->CurrentItemIndex();
    }

// --------------------------------------------------------------------------
// CWsfAiPlugin::StartConnectingAnimationL
// --------------------------------------------------------------------------
//
void CWsfSearchWlansDialog::StartConnectingAnimationL()
    {
    }

// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::ForceRefreshingL
// --------------------------------------------------------------------------
//    
void CWsfSearchWlansDialog::ForceRefreshingL()
    {
    }


// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::MultilineControl
// --------------------------------------------------------------------------
//
TBool CWsfSearchWlansDialog::MultilineControl() 
    { 
    return ETrue; 
    }
      

// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::UpdateViewL
// --------------------------------------------------------------------------
//    
void CWsfSearchWlansDialog::UpdateViewL( MDesCArray* /*aItemTextArray*/ )
    {
    }

// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::DisplayEngineOffL
// --------------------------------------------------------------------------
//    
void CWsfSearchWlansDialog::DisplayEngineOffL()
    {
    }


    
// --------------------------------------------------------------------------
// CWsfSearchWlansDialog::PreLayoutDynInitL
// --------------------------------------------------------------------------
//    
void CWsfSearchWlansDialog::PreLayoutDynInitL()
    {
    _ASS_D( ListBox() );
    // add the empty text here....
    HBufC* noWlansAvailable = iCoeEnv->AllocReadResourceLC( 
                                        R_QTN_STATUS_NO_WLANS_AVAILABLE );
    
    ListBox()->View()->SetListEmptyTextL( *noWlansAvailable );
    
    CleanupStack::PopAndDestroy( noWlansAvailable );
    CAknListQueryDialog::PreLayoutDynInitL();
    }
    
// End of file

