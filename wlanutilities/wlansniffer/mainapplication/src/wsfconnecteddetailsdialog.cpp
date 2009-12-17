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
* Description:  Implementation of CWsfConnectedDetailsDialog.
*
*/


// INCLUDE FILES
#include <avkon.hrh>
#include <aknlists.h>
#include <StringLoader.h>
#include <wlansniffer.rsg>
#include "wsfconnecteddetailsdialog.h"
#include "wsfconnecteddetailsmodel.h"
#include "wsfconnecteddetailscontroller.h"
#include "wsfactivetimeupdater.h"

#include "wsflogger.h"

// CONSTANTS
LOCAL_D const TInt KUpdateInterval = 1000000; // in micro seconds (1 sec)
// Time item's place in the listbox.
LOCAL_D const TInt  KActiveTimeItem = 1;
// Transferred data item's place in the listbox.
LOCAL_D const TInt  KTransferredAmountItem = 2;

    
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CWsfConnectedDetailsDialog::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfConnectedDetailsDialog::ConstructL()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsDialog::ConstructL" );    
    iController.SetDialog( this );
    // Create and start Active Object 'Active Updater'
    iActiveUpdater = CWsfActiveTimeUpdater::NewL( &iController );
    iActiveUpdater->Start( KUpdateInterval );
    }


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsDialog::NewL
// ---------------------------------------------------------------------------
//
CWsfConnectedDetailsDialog* CWsfConnectedDetailsDialog::NewL()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsDialog::NewL" );    
    CWsfConnectedDetailsDialog* self = CWsfConnectedDetailsDialog::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsDialog::NewLC
// ---------------------------------------------------------------------------
//
CWsfConnectedDetailsDialog* CWsfConnectedDetailsDialog::NewLC()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsDialog::NewLC" );
    CWsfConnectedDetailsDialog* self = 
                            new( ELeave ) CWsfConnectedDetailsDialog( NULL );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsDialog::CWsfConnectedDetailsDialog
// ---------------------------------------------------------------------------
//
CWsfConnectedDetailsDialog::CWsfConnectedDetailsDialog( 
            CListBoxView::CSelectionIndexArray* aSelectionIndexArray )
    : CAknListQueryDialog( aSelectionIndexArray ),
    iList( NULL ),
    iModel( NULL )
    {
    }


// ---------------------------------------------------------------------------
// CWsfConnectedDetailsDialog::~CWsfConnectedDetailsDialog
// ---------------------------------------------------------------------------
//
CWsfConnectedDetailsDialog::~CWsfConnectedDetailsDialog()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsDialog::~CWsfConnectedDetailsDialog" );
    iController.SetModel(NULL);
    iController.SetDialog(NULL);
    if( iActiveUpdater )
        {
        iActiveUpdater->Stop();
        }
    delete iActiveUpdater;
    // iListModel is deleted in WSFAppui::StartConnectedDetailsL
    }

// ---------------------------------------------------------------------------
// CWsfConnectedDetailsDialog::SetListModel
// ---------------------------------------------------------------------------
//
void CWsfConnectedDetailsDialog::SetListModel( CWsfConnectedDetailsModel* aModel)
    {
    LOG_ENTERFN( "CWsfConnectedDetailsDialog::SetListModel" );
    iListModel = aModel;
    iController.SetModel(iListModel);
    }


// ---------------------------------------------------------
// CWsfConnectedDetailsDialog::PreLayoutDynInitL
// called by framework before dialog is shown
// ---------------------------------------------------------
//
void CWsfConnectedDetailsDialog::PreLayoutDynInitL()
    {
    LOG_ENTERFN( "CWsfConnectedDetailsDialog::PreLayoutDynInitL" );
    // parent creates the private listbox
    CAknListQueryDialog::PreLayoutDynInitL();
    // and now we get access to it...
    CAknListQueryControl *control = 
            ( CAknListQueryControl* )Control( EListQueryControl );
    iList = control->Listbox();
    iModel = ( CTextListBoxModel* )iList->Model();
    
    // 1st Refresh of the listbox
    iController.RefreshL();
    ActivateL();
    }
    
// ---------------------------------------------------------
// CWsfConnectedDetailsContainer::UpdateListBox
// ---------------------------------------------------------
//
void CWsfConnectedDetailsDialog::UpdateListBox( MDesCArray* aItemTextArray )
    {
    LOG_ENTERFN( "CWsfConnectedDetailsDialog::UpdateListBox" );
    for( TInt i = 0; i < aItemTextArray->MdcaCount(); i++)
        {
        TPtrC temp = aItemTextArray->MdcaPoint( i );
        TBuf<100> tempElement = temp;
        LOG_WRITEF( "aItemTextArray[0]: %S", &tempElement );
        }

    if ( iList && aItemTextArray->MdcaCount() )
        {
        iList->Reset();
        iModel->SetItemTextArray( aItemTextArray );
        iModel->SetOwnershipType( ELbmDoesNotOwnItemArray );
        // draw updated items
        iList->DrawItem( KActiveTimeItem );
        iList->DrawItem( KTransferredAmountItem );
        
        DrawNow();
        }
    }
    
// ---------------------------------------------------------
// CWsfConnectedDetailsContainer::Controller
// ---------------------------------------------------------
//
MWsfDetailsViewControllerIf& CWsfConnectedDetailsDialog::Controller()
    {
    return iController;     
    }    

//  End of File

