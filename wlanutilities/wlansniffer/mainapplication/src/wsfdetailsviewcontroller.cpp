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
* Description:  Implementation of TWsfDetailsViewController.
*
*/

//  CLASS HEADER
#include "wsfdetailsviewcontroller.h"

//  EXTERNAL INCLUDES

//  INTERNAL INCLUDES
#include "wsfdetailscontainer.h"
#include "wsfdetailsviewmodel.h"
#include "wsfdetailsviewpartner.h"
#include "wsfwlaninfo.h"
#include "wsfwlaninfoarray.h"

#include "wsflogger.h"

//  LOCAL DEFINITIONS
#ifdef _DEBUG
    _LIT( KDetailsViewControllerPanic, "TWsfDetailsViewController" );
    #define _ASSERTD( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KDetailsViewControllerPanic, __LINE__) )
#else
    #define _ASSERTD( cond ) {}
#endif //_DEBUG

// ---------------------------------------------------------------------------
// TWsfDetailsViewController::SetModel
// ---------------------------------------------------------------------------
//    
void TWsfDetailsViewController::SetModel( CWsfDetailsViewModel* aModel )
    {
    iModel = aModel;
    }

// ---------------------------------------------------------------------------
// TWsfDetailsViewController::SetContainer
// ---------------------------------------------------------------------------
//   
void TWsfDetailsViewController::SetContainer( 
                                        MWsfDetailsContainerIf* aContainer )
    {
    iContainer = aContainer;
    }

// ---------------------------------------------------------------------------
// TWsfDetailsViewController::SetWlanListL
// ---------------------------------------------------------------------------
//   
void TWsfDetailsViewController::SetWlanListL(
                                        CWsfWlanInfoArray* aWlanArray, 
                                        const TDesC8& aSelectedWlanSsid )
    {
    LOG_ENTERFN( "TWsfDetailsViewController::SetWlanListL" );
    _ASSERTD( aWlanArray );
        
    //Get selected Wlan info 
    TWsfWlanInfo* info = aWlanArray->Match( aSelectedWlanSsid, 
                                         aWlanArray->Count() );

    CDesCArray* formattadList = NULL;                                         
                                         
    // If wlan info found
    if( info )
        {        
        // Update model
        formattadList = iModel->FormatWlanInfoL( info, aWlanArray );
        }    

    // if view active && and something to show
    if( iContainer && formattadList )
        {
        //Draw view
        iContainer->UpdateListBoxL( formattadList );
        }
        
    HandlePaneTextUpdateL();
    }
    
// ---------------------------------------------------------------------------
// TWsfDetailsViewController::WlanListChangedL
// ---------------------------------------------------------------------------
//        
void TWsfDetailsViewController::WlanListChangedL( 
                                        CWsfWlanInfoArray* aWlanArray )
    {
    LOG_ENTERFN( "TWsfDetailsViewController::WlanListChangedL" );
    _ASSERTD( aWlanArray );
                    
    _ASSERTD( iModel );
    
    //Get selected Wlan info 
    TWsfWlanInfo* info = aWlanArray->Match( iModel->WlanSsid(), 
                                         aWlanArray->Count() );
    //If view active
    if( iContainer )
        {
        CDesCArray* formattadList = NULL;
        
        //Current info found from new list
        if( info )
            {
            // Update model
            formattadList = iModel->FormatWlanInfoL( info, aWlanArray );
            HandlePaneTextUpdateL();        
            }
            
        // Current Wlan has been dropped            
        else
            {
            // Update model get no signal list and dont update pane text
            formattadList = iModel->FormatWlanInfoL( NULL, aWlanArray );
            }
            
        //Draw view            
        iContainer->UpdateListBoxL( formattadList );
        HandlePaneTextUpdateL();
        }
    }

// ---------------------------------------------------------------------------
// TWsfDetailsViewController::RefreshL
// ---------------------------------------------------------------------------
//         
void TWsfDetailsViewController::RefreshL( )
    {
    LOG_ENTERFN( "TWsfDetailsViewController::RefreshL" );
    _ASSERTD( iModel );
    
    if ( iContainer )
        {        
        CDesCArray* formattedDetails = iModel->GetWlanDetails();
        iContainer->UpdateListBoxL( formattedDetails );    
        HandlePaneTextUpdateL();
        }
    }
    
// ---------------------------------------------------------------------------
// TWsfDetailsViewController::SetPartner
// ---------------------------------------------------------------------------
// 
 void TWsfDetailsViewController::SetPartner( MWsfDetailsViewPartner& aPartner )
    {
    iPartner = &aPartner;    
    }

    
 // ---------------------------------------------------------------------------
 // TWsfDetailsViewController::HandleLeftKeyPressedL
 // ---------------------------------------------------------------------------
 //      
void TWsfDetailsViewController::HandleLeftKeyPressedL()
    {
    LOG_ENTERFN( "TWsfDetailsViewController::HandleLeftKeyPressedL" );    
    _ASSERTD( iModel );
    
    if( iContainer )        
        {
        CDesCArray* formattadList = iModel->FormatPreviousWlanInfoL();
        iContainer->UpdateListBoxL( formattadList );
        HandlePaneTextUpdateL();
        }
    }
    
// ---------------------------------------------------------------------------
// TWsfDetailsViewController::HandleRightKeyPressedL
// ---------------------------------------------------------------------------
//    
void TWsfDetailsViewController::HandleRightKeyPressedL()
    {
    LOG_ENTERFN( "TWsfDetailsViewController::HandleRightKeyPressedL" );
    _ASSERTD( iModel );

    if( iContainer )        
        {
        CDesCArray* formattadList = iModel->FormatNextWlanInfoL();
        iContainer->UpdateListBoxL( formattadList );
        HandlePaneTextUpdateL();        
        }
    }

// ---------------------------------------------------------------------------
// TWsfDetailsViewController::HandleSelectionKeyPressedL
// ---------------------------------------------------------------------------
//
void TWsfDetailsViewController::HandleSelectionKeyPressedL()
    {
    LOG_ENTERFN( "TWsfDetailsViewController::HandleSelectionKeyPressedL" );
    if ( iPartner )
        {
        iPartner->CloseDetailsViewL();
        }    
    }

// ---------------------------------------------------------------------------
// TWsfDetailsViewController::HandlePaneTextUpdateL
// ---------------------------------------------------------------------------
//
void TWsfDetailsViewController::HandlePaneTextUpdateL()
    {
    LOG_ENTERFN( "TWsfDetailsViewController::HandlePaneTextUpdateL" );
    _ASSERTD( iModel );
    
    if( iContainer )        
        {
        HBufC* paneText = iModel->FormatPaneTextLC();
        TPtr temp( paneText->Des() );
        AknTextUtils::DisplayTextLanguageSpecificNumberConversion( temp );
        iContainer->UpdateNaviPaneL( *paneText );
        CleanupStack::PopAndDestroy( paneText );
        }
    }
    
// ---------------------------------------------------------------------------
// void TWsfDetailsViewController::UpdateActiveTimeL
// ---------------------------------------------------------------------------
//
void TWsfDetailsViewController::UpdateActiveTimeL()
    {
    LOG_ENTERFN( "TWsfDetailsViewController::UpdateActiveTimeL" );
    if ( iPartner )
        {
        iPartner->DetailsScanL();
        }
    }    
    
// End of file




