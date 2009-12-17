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
* Description:  Implementation of TWsfConnectedDetailsController.
*
*/


//  CLASS HEADER
#include "wsfconnecteddetailscontroller.h"

//  EXTERNAL INCLUDES

//  INTERNAL INCLUDES
#include "wsfconnecteddetailsmodel.h"
#include "wsfwlaninfo.h"
#include "wsfwlaninfoarray.h"
#include "wsfconnecteddetailsdialog.h"

#include "wsflogger.h"


//  LOCAL DEFINITIONS
#ifdef _DEBUG
    _LIT( KDetailsViewControllerPanic, "TWsfConnectedDetailsController" );
    #define _ASSERTD( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KDetailsViewControllerPanic, __LINE__) )
#else
    #define _ASSERTD( cond ) {}
#endif //_DEBUG




// ---------------------------------------------------------------------------
// void TWsfConnectedDetailsController::SetWlanListL
// ---------------------------------------------------------------------------
//
void TWsfConnectedDetailsController::SetWlanListL(
                                        CWsfWlanInfoArray* aWlanArray, 
                                        const TDesC8& aSelectedWlanSsid )
    {
    LOG_ENTERFN( "TWsfConnectedDetailsController::SetWlanListL" );
    _ASSERTD( aWlanArray );
        
    //Get selected Wlan info 
    TWsfWlanInfo* info = aWlanArray->Match( aSelectedWlanSsid, 
                                         aWlanArray->Count() );

    CDesCArray* formattedList = NULL;                                         
                                         
    // if view active && and something to show
    if ( iDialog && iModel)
        {
        // Update model
        formattedList = iModel->FormatWlanInfoL( info, aWlanArray );

        //Draw view
        iDialog->UpdateListBox( formattedList );
        }
    }
    
// ---------------------------------------------------------------------------
// void TWsfConnectedDetailsController::WlanListChangedL
// ---------------------------------------------------------------------------
//
void TWsfConnectedDetailsController::WlanListChangedL( 
                                        CWsfWlanInfoArray* aWlanArray )
    {
    LOG_ENTERFN( "TWsfConnectedDetailsController::WlanListChangedL" );    
    _ASSERTD( aWlanArray );
                    
    //If view active
    if ( iDialog && iModel )
        {
        //Get selected Wlan info 
        TWsfWlanInfo* info = aWlanArray->Match( iModel->WlanSsid(), 
                                             aWlanArray->Count() );

        CDesCArray* formattedList = NULL;
        
        //Current info found from new list
        // Update model
        formattedList = iModel->FormatWlanInfoL( info, aWlanArray );
            
        //Draw view            
        iDialog->UpdateListBox( formattedList );
        }
    }

// ---------------------------------------------------------------------------
// void TWsfConnectedDetailsController::RefreshL
// ---------------------------------------------------------------------------
//
void TWsfConnectedDetailsController::RefreshL()
    {
    LOG_ENTERFN( "TWsfConnectedDetailsController::RefreshL" );
    
    if ( iDialog && iModel)
        {        
        CDesCArray* formattedDetails = iModel->GetWlanDetails();
        
        iDialog->UpdateListBox( formattedDetails );
        }
    }
    
// ---------------------------------------------------------------------------
// void TWsfConnectedDetailsController::UpdateActiveTimeL
// ---------------------------------------------------------------------------
//
void TWsfConnectedDetailsController::UpdateActiveTimeL()
    {
    LOG_ENTERFN( "TWsfConnectedDetailsController::UpdateActiveTimeL" );
    if (iModel)
        {
        iModel->RefreshCurrentWlanInfoL();
        RefreshL();
        }
    }

void TWsfConnectedDetailsController::SetPartner( 
                                    MWsfDetailsViewPartner& /*aPartner*/ )
    {
    }

// End of file
