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
* Description:  Implementation of CWsfAiHelperView
*
*/



// EXTERNAL INCLUDES
#include  <aknViewAppUi.h>
#include <wsfaihelper.rsg>

//  CLASS HEADER
#include  "wsfaihelperview.h"

// INTERNAL INCLUDES
#include "wsfaihelperappui.h"
#include "wsfmodelobserver.h"
#include "wsfaihelper.hrh"
#include "wsfmodel.h"
#include "wsfaihelperdocument.h"
#include "wsfaihelpercontainer.h"
#include "wsflogger.h"



//  CONSTRUCTION AND DESTRUCTION

// ---------------------------------------------------------------------------
// CWsfAiHelperView::NewL
// ---------------------------------------------------------------------------
//
CWsfAiHelperView* CWsfAiHelperView::NewL( CWsfAiHelperAppUi* aAppUi )
    {
    CWsfAiHelperView* self = CWsfAiHelperView::NewLC( aAppUi );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperView::NewLC
// ---------------------------------------------------------------------------
//
CWsfAiHelperView* CWsfAiHelperView::NewLC( CWsfAiHelperAppUi* aAppUi )
    {
    CWsfAiHelperView* self = new (ELeave) CWsfAiHelperView( aAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperView::CWsfAiHelperView
// ---------------------------------------------------------------------------
//
CWsfAiHelperView::CWsfAiHelperView( CWsfAiHelperAppUi* aAppUi ): 
    CAknView(),
    iAppUi( aAppUi )
    {
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperView::~CWsfAiHelperView
// ---------------------------------------------------------------------------
//
CWsfAiHelperView::~CWsfAiHelperView()
    {
    delete iContainer;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperView::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperView::ConstructL()
    {
    BaseConstructL( R_SNIFFER_AI_HELPER_VIEW );

    iContainer = new (ELeave) CWsfAiHelperContainer();
    iContainer->ConstructL( ClientRect(), iAppUi );
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperView::Id
// ---------------------------------------------------------------------------
//
TUid CWsfAiHelperView::Id() const
    {
    return TUid::Uid( ESnifferAiHelperViewId );
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperView::DoActivateL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/ )
    {
    LOG_ENTERFN( "CWsfAiHelperView::DoActivateL" );
    AppUi()->AddToStackL( *this, iContainer );
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperView::DoDeactivate
// ---------------------------------------------------------------------------
//
void CWsfAiHelperView::DoDeactivate()
    {
    LOG_ENTERFN( "CWsfAiHelperView::DoDeactivate" );
    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        }
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperView::HandleCommandL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperView::HandleCommandL( TInt aCommand )
    {
    LOG_ENTERFN( "CWsfAiHelperView::HandleCommandL" );
    AppUi()->HandleCommandL( aCommand );
    }


// End of File
