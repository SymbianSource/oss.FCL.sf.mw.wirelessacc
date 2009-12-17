/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  view class
*     
*
*/


// EXTERNAL INCLUDES
#include <wlansniffer.rsg>
#include <aknViewAppUi.h>

// INTERNAL INCLUDES
#include "wsfdetailsview.h"
#include "wsfdetailscontainer.h" 
#include "wsfdetailsviewmodel.h"
#include "wsfmainapplication.hrh"
#include "wsfappui.h"
#include "wsfdetailscontainerif.h"

#include "wsflogger.h"



// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CWsfDetailsView* CWsfDetailsView::NewL()
// ---------------------------------------------------------
//
CWsfDetailsView* CWsfDetailsView::NewL()
    {
    LOG_ENTERFN( "CWsfDetailsView::NewL" );
    CWsfDetailsView* self = CWsfDetailsView::NewLC();
    CleanupStack::Pop(self);
    return self;
    } 

// ---------------------------------------------------------
// CWsfDetailsView* CWsfDetailsView::NewLC()
// ---------------------------------------------------------
//
CWsfDetailsView* CWsfDetailsView::NewLC()
    {
    LOG_ENTERFN( "CWsfDetailsView::NewLC" );
    CWsfDetailsView* self = new (ELeave) CWsfDetailsView();
    CleanupStack::PushL(self);
    self->ConstructL( );
    return self;
    }


// ---------------------------------------------------------
// CWsfDetailsView::CWsfDetailsView()
// ---------------------------------------------------------
//
CWsfDetailsView::CWsfDetailsView() : CAknView()
    {
    }

// ---------------------------------------------------------
// CWsfDetailsView::ConstructL()
// ---------------------------------------------------------
//
void CWsfDetailsView::ConstructL()
    {
    LOG_ENTERFN( "CWsfDetailsView::ConstructL" );
    BaseConstructL( R_SNIFFER_DETAILSVIEW );
    iModel = CWsfDetailsViewModel::NewL();
    iController.SetModel( iModel );
    }

// ---------------------------------------------------------
// CWsfDetailsView::~CWsfDetailsView()
// ---------------------------------------------------------
//
CWsfDetailsView::~CWsfDetailsView()
    {
    LOG_ENTERFN( "CWsfDetailsView::~CWsfDetailsView" );
    delete iContainer;
    delete iModel;
    }

// ---------------------------------------------------------
// CWsfDetailsView::Id()
// ---------------------------------------------------------
//
TUid CWsfDetailsView::Id() const
    {
    return TUid::Uid( EDetailsViewId );     
    }

// ---------------------------------------------------------
// CWsfDetailsView::HandleCommandL()
// ---------------------------------------------------------
//
void CWsfDetailsView::HandleCommandL( TInt aCommand )
    {
    LOG_ENTERFN( "CWsfDetailsView::HandleCommandL" );
    switch ( aCommand )
        {
        case EAknSoftkeyOk:
            {
            iController.HandleSelectionKeyPressedL();
            AppUi()->HandleCommandL( ESnifferCmdShowWlans );
            break;
            }
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }

// ---------------------------------------------------------
// CWsfDetailsView::Controller
// ---------------------------------------------------------
//
MWsfDetailsViewControllerIf& CWsfDetailsView::Controller()
    {
    return iController;     
    }

// ---------------------------------------------------------
// CWsfDetailsView::DoActivateL()
// ---------------------------------------------------------
//
void CWsfDetailsView::DoActivateL( const TVwsViewId& /*aPrevViewId*/, 
                                       TUid /*aCustomMessageId*/,
                                       const TDesC8& /*aCustomMessage*/ )
    {
    LOG_ENTERFN( "CWsfDetailsView::DoActivateL" );
    if( !iContainer )
        {
        iContainer = CWsfDetailsContainer::NewL( ClientRect(),
                                                 iController );
        iContainer->SetMopParent( this );
        AppUi()->AddToViewStackL( *this, iContainer );
        iContainer->SetPartner( iController );
        iContainer->SetDetailsModel( iModel );   
        iContainer->MakeVisible( ETrue );         
        iController.SetContainer( iContainer );
        iController.RefreshL();
        }        
    }

// ---------------------------------------------------------
// CWsfDetailsView::DoDeactivate
// ---------------------------------------------------------
//
void CWsfDetailsView::DoDeactivate()
    {
    LOG_ENTERFN( "CWsfDetailsView::DoDeactivate" );
    AppUi()->RemoveFromViewStack( *this, iContainer );
    iContainer->MakeVisible( EFalse );
    delete iContainer;
    iContainer = NULL;
    iController.SetContainer( NULL );          
    }

// ---------------------------------------------------------
// CWsfDetailsView::HandleListBoxEventL
// ---------------------------------------------------------
//
void CWsfDetailsView::HandleListBoxEventL( CEikListBox* /*aListBox*/,
                                               TListBoxEvent /*aEventType*/ )
    {
    }

// End of File
