/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Inline for CWsfDocument
*
*/



//  INLINE FUNCTIONS

// ---------------------------------------------------------------------------
// CWsfDocument::Model
// ---------------------------------------------------------------------------
//
inline CWsfModel& CWsfDocument::Model() const
    {
    return *iModel;
    }


// ---------------------------------------------------------------------------
// CWsfDocument::WlanInfoBranding
// ---------------------------------------------------------------------------
//
inline CWsfWlanInfoArrayVisitor& CWsfDocument::WlanInfoBranding() const
    {
    return *iWlanInfoBranding;
    }


// ---------------------------------------------------------------------------
// CWsfDocument::MainController
// ---------------------------------------------------------------------------
//
inline const TWsfMainController* CWsfDocument::MainController() const
    {
    return &iMainController;
    }
