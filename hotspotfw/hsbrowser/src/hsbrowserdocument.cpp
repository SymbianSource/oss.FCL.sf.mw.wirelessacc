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
* Description:   Implements the document for application.
*
*/



// INCLUDE FILES
#include "hsbrowserdocument.h"
#include "hsbrowserappui.h"
#include "hsbrowsermodel.h"
#include "am_debug.h"

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// CHsBrowserDocument::CHsBrowserDocument
// ----------------------------------------------------
//
CHsBrowserDocument::CHsBrowserDocument( CEikApplication& aApp )
: CAknDocument(aApp), iModel( NULL )  
    {
    }

// ----------------------------------------------------
// CHsBrowserDocument::~CHsBrowserDocument
// ----------------------------------------------------
//
CHsBrowserDocument::~CHsBrowserDocument()
    {
    DEBUG( "CHsBrowserDocument::~CHsBrowserDocument()" );
    if ( iModel )
    	{
    	delete iModel;
    	}
    }

// ----------------------------------------------------
// CHsBrowserDocument::ConstructL
// ----------------------------------------------------
//
void CHsBrowserDocument::ConstructL()
    {
    DEBUG( "CHsBrowserDocument::ConstructL()" );
    iModel = CHsBrowserModel::NewL();
    }

// ----------------------------------------------------
// CHsBrowserDocument::NewL
// ----------------------------------------------------
//
CHsBrowserDocument* CHsBrowserDocument::NewL(
        CEikApplication& aApp)     
    {
    CHsBrowserDocument* self = new (ELeave) CHsBrowserDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }
    
// ----------------------------------------------------
// CHsBrowserDocument::CreateAppUiL()
// constructs CBrCtlSampleAppUi
// ----------------------------------------------------
//
CEikAppUi* CHsBrowserDocument::CreateAppUiL()
    {
    return new (ELeave) CHsBrowserAppUi;
    }

// End of File  
