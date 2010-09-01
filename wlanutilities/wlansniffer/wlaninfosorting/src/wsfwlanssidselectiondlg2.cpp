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
* Description:  Implementation of CWsfWlanSsidSelectionDlg2
*
*/



//  EXTERNAL INCLUDES
#include <avkon.mbg>
#include <AknIconUtils.h> 
#include <wsfwlaninfosorting.rsg>
#include <AknIconArray.h> // CAknIcon
#include <akntitle.h> // akntitle pane
#include <aknlists.h> // CAknSingleGraphicStyleListBox

//  CLASS HEADER
#include "wsfwlanssidselectiondlg2.h"

// INTERNAL INCLUDES
#include "wsfwlaninfosorting.hrh"
#include "wsflogger.h"


/**
* Format string for listbox items
*/
_LIT( KFormatString, "1\t%S" );

/**
* Number of icons used
*/
static const TInt KNumIcons = 2;



// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::NewL
// ----------------------------------------------------------------------------
//
CWsfWlanSsidSelectionDlg2* CWsfWlanSsidSelectionDlg2::NewL( 
                     CDesCArrayFlat& aSsidArray,
                     CListBoxView::CSelectionIndexArray& aSelectionIndexArray )
    {
    CWsfWlanSsidSelectionDlg2* self = NewLC( aSsidArray, 
                                             aSelectionIndexArray );
    CleanupStack::Pop( self );
    return self;
    }

    
// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::NewLC
// ----------------------------------------------------------------------------
//
CWsfWlanSsidSelectionDlg2* CWsfWlanSsidSelectionDlg2::NewLC( 
                     CDesCArrayFlat& aSsidArray,
                     CListBoxView::CSelectionIndexArray& aSelectionIndexArray )
    {
    CWsfWlanSsidSelectionDlg2* self = new (ELeave) CWsfWlanSsidSelectionDlg2( 
                                            aSsidArray, aSelectionIndexArray );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfWlanSsidSelectionDlg2::ConstructL()
    {
    }


// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::CWsfWlanSsidSelectionDlg2
// ----------------------------------------------------------------------------
//
CWsfWlanSsidSelectionDlg2::CWsfWlanSsidSelectionDlg2( 
                    CDesCArrayFlat& aSsidArray,
                    CListBoxView::CSelectionIndexArray& aSelectionIndexArray ):
    CAknListQueryDialog( &aSelectionIndexArray ),
    iSsidArray( &aSsidArray ),
    iSelectedItems( &aSelectionIndexArray )
    {
    }


// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::~CWsfWlanSsidSelectionDlg2
// ----------------------------------------------------------------------------
//
CWsfWlanSsidSelectionDlg2::~CWsfWlanSsidSelectionDlg2()
	{
	// restore the title....	
	if ( iTitle && iPane )
        {
        iPane->SetText( iTitle );
        }

	iPane = NULL;
	iTitle = NULL;
	iSsidArray = NULL; 
	iSelectedItems = NULL; 
	}


// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::OfferKeyEventL
// ----------------------------------------------------------------------------
//
TKeyResponse CWsfWlanSsidSelectionDlg2::OfferKeyEventL( 
                                                    const TKeyEvent& aKeyEvent, 
                                                    TEventCode aType )
    {
    if ( aKeyEvent.iCode == EKeyApplicationF )
        {
        return EKeyWasConsumed;
        }
    else
        {
        return CAknListQueryDialog::OfferKeyEventL( aKeyEvent, aType );
        }
    }


// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::PreLayoutDynInitL
// ----------------------------------------------------------------------------
//
void CWsfWlanSsidSelectionDlg2::PreLayoutDynInitL()
    {
    LOG_ENTERFN( "CWsfWlanSsidSelectionDlg2::PreLayoutDynInitL" );

    CAknListQueryDialog::PreLayoutDynInitL();

    CAknListQueryControl *control = static_cast<CAknListQueryControl*>( 
                                                Control( EListQueryControl ) );
    CEikListBox* listBox = control->Listbox();

   	CDesCArrayFlat *formattedSsids = new (ELeave) CDesCArrayFlat( 
   	                                                     iSsidArray->Count() );
	CleanupStack::PushL( formattedSsids );
	CopyAndFormatArrayContentsL( iSsidArray, formattedSsids );
	
	SetItemTextArray( formattedSsids );

	CleanupStack::Pop( formattedSsids );
	 
    CAknIconArray* iconArray = CreateQueryIconArrayL();
    CleanupStack::PushL( iconArray );
    SetIconArrayL( iconArray );
    CleanupStack::Pop( iconArray );
    
    for (TInt i = 0; i < iSelectedItems->Count(); i++)
        {
        listBox->View()->SelectItemL( (*iSelectedItems)[i] );
        }
	
    CEikStatusPane* statusPane = iAvkonAppUi->StatusPane();
    iPane = (CAknTitlePane*) statusPane->ControlL( 
                                         TUid::Uid( EEikStatusPaneUidTitle ) );
    iTitle = iPane->Text()->AllocL();
    }
    
	
// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::CopyAndFormatArrayContentsL
// ----------------------------------------------------------------------------
//
void CWsfWlanSsidSelectionDlg2::CopyAndFormatArrayContentsL( 
                                                       CDesCArrayFlat* aSource,
                                                       CDesCArrayFlat* aDest )
	{
	TBuf<80> lineBuffer;
	TInt sourceCount( aSource->Count() );
	for ( TInt i( 0 ) ; i < sourceCount ; i++ )
		{
		const TDesC& item = aSource->MdcaPoint( i );
   		lineBuffer.Format(KFormatString, &item );
		aDest->AppendL( lineBuffer );
   		}
	}  


// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::CreateQueryIconArrayL
// ----------------------------------------------------------------------------
//
CAknIconArray* CWsfWlanSsidSelectionDlg2::CreateQueryIconArrayL()
    {
    CAknIconArray* iconarray = new( ELeave )CAknIconArray( KNumIcons );
    CleanupStack::PushL( iconarray );
    
    const TDesC& iconfilename = AknIconUtils::AvkonIconFileName();
    
    // empty
    iconarray->AppendL( CreateIconLC( iconfilename, 
        EMbmAvkonQgn_prop_checkbox_on, 
        EMbmAvkonQgn_prop_checkbox_on_mask, 
        KAknsIIDQgnPropEmpty ));  
    CleanupStack::Pop(); // icon
    
    // copying
    iconarray->AppendL(CreateIconLC( iconfilename, 
        EMbmAvkonQgn_prop_checkbox_off, 
        EMbmAvkonQgn_prop_checkbox_off_mask, 
        KAknsIIDNone ));
    CleanupStack::Pop(); // icon
    CleanupStack::Pop(); // iconarray

  
    return iconarray;    
    }
    

// ----------------------------------------------------------------------------
// CWsfWlanSsidSelectionDlg2::CreateIconLC
// ----------------------------------------------------------------------------
//
CGulIcon* CWsfWlanSsidSelectionDlg2::CreateIconLC( const TDesC& aFileName, 
                                                   TInt aIcon, 
                                                   TInt aMask, 
                                                   const TAknsItemID& aID )
    {
    MAknsSkinInstance* skinInstance = AknsUtils::SkinInstance();
    
    CGulIcon* icon = AknsUtils::CreateGulIconL( skinInstance, aID, aFileName, 
                                                aIcon, aMask );
    CleanupStack::PushL( icon );
        
    return icon;
    }



