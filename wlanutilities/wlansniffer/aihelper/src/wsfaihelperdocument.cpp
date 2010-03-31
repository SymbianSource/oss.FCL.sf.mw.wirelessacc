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
* Description:  Implementation of CWsfAiHelperDocument
*
*/


#include "wsfaihelperappui.h"
#include "wsfaihelperdocument.h"
#include "wsfmodel.h"
#include "wsfwlaninfo.h"



// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::NewL
// ---------------------------------------------------------------------------
//
CWsfAiHelperDocument* CWsfAiHelperDocument::NewL( CEikApplication& aApp )
    {
    CWsfAiHelperDocument* self = NewLC( aApp );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::NewLC
// ---------------------------------------------------------------------------
//
CWsfAiHelperDocument* CWsfAiHelperDocument::NewLC( CEikApplication& aApp ) 
    {
    CWsfAiHelperDocument* self = new (ELeave) CWsfAiHelperDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperDocument::ConstructL()
    {
    }    


// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::CWsfAiHelperDocument
// ---------------------------------------------------------------------------
//
CWsfAiHelperDocument::CWsfAiHelperDocument( CEikApplication& aApp ): 
    CAknDocument( aApp )
    {
    // no implementation required
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::~CWsfAiHelperDocument
// ---------------------------------------------------------------------------
//
CWsfAiHelperDocument::~CWsfAiHelperDocument()
    {
    delete iModel;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::CreateAppUiL
// ---------------------------------------------------------------------------
//
CEikAppUi* CWsfAiHelperDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CWsfAiHelperAppUi* appUi = new (ELeave) CWsfAiHelperAppUi;
    iModel = CWsfModel::NewL( *appUi, EFalse );
    
    iModel->EnableScanL();
    return appUi;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::OpenFileL
// ---------------------------------------------------------------------------
//
CFileStore* CWsfAiHelperDocument::OpenFileL( TBool /*aDoOpen*/, 
                                             const TDesC& aFilename, 
                                             RFs &/*aFs*/ )
    {
    // Read the afilename arg
    // Take that into a TWlanInfo and start creation of accesspoint
    TPckgBuf<TWsfWlanInfo> info;

    info.Copy( aFilename.Left( sizeof( TWsfWlanInfo ) ) );
    
    iWlanInfo = info();
    
    static_cast<CWsfAiHelperAppUi*>( iAppUi )->NotifyFileName();

    return NULL;
    }



