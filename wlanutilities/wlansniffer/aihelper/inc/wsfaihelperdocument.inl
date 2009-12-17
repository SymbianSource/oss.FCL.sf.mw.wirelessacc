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
* Description:  Inline for CWsfAiHelperDocument
*
*/



//  INLINE FUNCTIONS

// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::Model
// ---------------------------------------------------------------------------
//
CWsfModel& CWsfAiHelperDocument::Model()
    {
    return *iModel;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::WlanInfo
// ---------------------------------------------------------------------------
//
TWsfWlanInfo& CWsfAiHelperDocument::WlanInfo()
    {
    return iWlanInfo;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::ShouldConnectOnly()
// ---------------------------------------------------------------------------
//
TBool CWsfAiHelperDocument::ShouldConnectOnly()
    {
    return iConnectOnly;
    }

// ---------------------------------------------------------------------------
// CWsfAiHelperDocument::UseDefaultConnection()
// ---------------------------------------------------------------------------
//
TBool CWsfAiHelperDocument::UseDefaultConnection()
    {
    return iUseDefaultConnection;
    }


