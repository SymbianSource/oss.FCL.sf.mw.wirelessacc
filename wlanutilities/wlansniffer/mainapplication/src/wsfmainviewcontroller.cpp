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
* Description:  Implementation of TWsfMainViewController.
*
*/

//  INTERNAL INCLUDES
#include "wsfmainviewcontroller.h"
#include "wsfmainviewmodel.h"
#include "wsfmainviewcontainer.h"
#include "wsfwlaninfo.h"
#include "wsfwlaninfoarray.h"

#include "wsflogger.h"


//  LOCAL DEFINITIONS
#ifdef _DEBUG
    _LIT( KMainViewControllerPanic, "TWsfMainViewController"  );
    #define _ASSERTD( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KMainViewControllerPanic, __LINE__) )
#else
    #define _ASSERTD( cond ) {}
#endif //_DEBUG

    
// ---------------------------------------------------------------------------
// TWsfMainViewController::SetModel
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::SetModel( CWsfMainViewModel* aModel )
    {
    iModel = aModel;
    iValidItem = EFalse;
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::SetContainer
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::SetContainer( CWsfMainViewContainer* aContainer )
    {
    iContainer = aContainer;
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::UpdateHotSpotsL
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::UpdateHotSpotsL( CWsfWlanInfoArray* aWlanList )
    {
    LOG_ENTERFN( "TWsfMainViewController::UpdateHotSpotsL" );   
    // Format WLAN list which was passed as a parameter
    CDesCArray* formattedList = iModel->FormatWlanListL( aWlanList );

    if ( iContainer )
        {        
        // If some WLAN was already selected, don't lose the focus
        // By default show the first item (index == 0)
        TInt index = 0;
        TWsfWlanInfo* wlanInfo = aWlanList->Match( iModel->SelectedWlan(),
                                                   aWlanList->Count() );
        if ( wlanInfo )
            {            
            index = aWlanList->GetArrayIndex( wlanInfo );
            }
        iContainer->UpdateHotSpotsL( formattedList, index );
        } 
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::UpdateNaviPaneL
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::UpdateNaviPaneL()
    {
    LOG_ENTERFN( "TWsfMainViewController::UpdateNaviPaneL" );   
    HBufC* naviText = iModel->FormatNaviPaneLC();
    iContainer->UpdateNaviPaneL( *naviText );
    CleanupStack::PopAndDestroy( naviText );   
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::UpdateWlanListL
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::UpdateWlanListL( CWsfWlanInfoArray* aWlanArray )
    {
    LOG_ENTERFN( "TWsfMainViewController::UpdateWlanListL" );   
    UpdateHotSpotsL( aWlanArray );
    UpdateNaviPaneL();
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::RefreshL
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::RefreshL()
    {
    LOG_ENTERFN( "TWsfMainViewController::RefreshL" );   
    CDesCArray* formattedList =  iModel->GetWlanList();
    iContainer->UpdateHotSpotsL( formattedList, iModel->SelectedIndex() );  
    UpdateNaviPaneL();      
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::SelectedItem
// ---------------------------------------------------------------------------
//
TWsfWlanInfo* TWsfMainViewController::SelectedItem()
    {
    LOG_ENTERFN( "TWsfMainViewController::SelectedItem" );
    
    return iValidItem? &iSelectedItem: NULL; 
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::SetPartner
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::SetPartner( MWsfMainViewPartner& aPartner )
    {
    iPartner = &aPartner;
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::UpdateSelectedItemL
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::UpdateSelectedItemL()
    {
    LOG_ENTERFN( "TWsfMainViewController::UpdateSelectedItemL" );   

    _ASSERTD( iModel && iContainer );
    
    TInt selectedItem = iContainer->SelectedItem();
    CWsfWlanInfoArray* array = iModel->GetInfoArray();
    if ( array )
        {
        TWsfWlanInfo* item = NULL;
        if ( selectedItem < array->Count() )
            {
            item = array->At( selectedItem );
            }
        
        if ( item )
            {
            iSelectedItem = *item;
            iValidItem = ETrue;
            }
        else
            {
            iValidItem = EFalse;
            }
        }
    
    // Store name of focused WLAN
    if ( iValidItem )
        {        
        HBufC8* ssid = iSelectedItem.GetSsidAsUtf8LC();
        iModel->SetSelectedWlan( ssid );
        CleanupStack::Pop( ssid );
        }
    
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::HandleUpKeyPressedL
// ---------------------------------------------------------------------------
//    
void TWsfMainViewController::HandleUpKeyPressedL()
    {
    LOG_ENTERFN( "TWsfMainViewController::HandleUpKeyPressedL" );   
    _ASSERTD( iModel );
    
    // Store just listbox index. Needed
    iModel->SetSelectedIndex( iContainer->SelectedItem() );    
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::HandleDownKeyPressedL
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::HandleDownKeyPressedL()
    {
    LOG_ENTERFN( "TWsfMainViewController::HandleDownKeyPressedL" );   
    _ASSERTD( iModel );

    iModel->SetSelectedIndex( iContainer->SelectedItem() );
    }


// ---------------------------------------------------------------------------
// TWsfMainViewController::HandleSelectionKeyPressedL
// ---------------------------------------------------------------------------
//
void TWsfMainViewController::HandleSelectionKeyPressedL()
    {
    LOG_ENTERFN( "TWsfMainViewController::HandleSelectionKeyPressedL" );   
    _ASSERTD( iPartner );    

    iPartner->ShowMenuBarL();
    }

// End of file

