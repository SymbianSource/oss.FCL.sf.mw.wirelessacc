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
* Description:  Implementation of CWsfDocument.
*
*/

#include <featmgr.h>
#include "wsfappui.h"
#include "wsfdocument.h"
#include "wsfwlaninfoarrayvisitor.h"
#include "wsfactivewrappers.h"
#include "wsfmodel.h"


// ---------------------------------------------------------------------------
// CCWsfDocument::NewL
// ---------------------------------------------------------------------------
//
CWsfDocument* CWsfDocument::NewL( CEikApplication& aApp )
    {
    CWsfDocument* self = NewLC( aApp );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCWsfDocument::NewLC
// ---------------------------------------------------------------------------
//
CWsfDocument* CWsfDocument::NewLC( CEikApplication& aApp )
    {
    CWsfDocument* self = new ( ELeave ) CWsfDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CCWsfDocument::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfDocument::ConstructL()
    {
    iModel = CWsfModel::NewL( iMainController, EFalse );
    iModel->SetEngineObserver( &iMainController );
    iAsyncModel = CWsfActiveWrappers::NewL( iModel, iMainController );
    iWlanInfoBranding = CWsfWlanInfoArrayVisitor::NewL( EFalse );
    iWlanInfoBranding->LoadFilterDefinitionsL();
    }    

// ---------------------------------------------------------------------------
// CCWsfDocument::CWsfDocument
// ---------------------------------------------------------------------------
//
CWsfDocument::CWsfDocument( CEikApplication& aApp ) : CAknDocument( aApp ) 
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CCWsfDocument::~CWsfDocument
// ---------------------------------------------------------------------------
//
CWsfDocument::~CWsfDocument()
    {
    delete iWlanInfoBranding;
    // Needs iModel, delete before iModel
    delete iAsyncModel;
    delete iModel;
    }

// ---------------------------------------------------------------------------
// CCWsfDocument::CreateAppUiL
// ---------------------------------------------------------------------------
//
CEikAppUi* CWsfDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CWsfAppUi* appUi = new ( ELeave ) CWsfAppUi;
    appUi->SetUiObserver( &iMainController );
        
    // Give references to main controller.
    iMainController.InitializeL( 
            *appUi, 
            *iModel, 
            *iAsyncModel, 
            *iWlanInfoBranding );   
    
    return appUi;
    }

