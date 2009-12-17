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
* Description:   Inline method definitions for CHsBrowserContainer.   
*
*/



#ifndef HSBROWSERCONTAINER_INL
#define HSBROWSERCONTAINER_INL

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CHsBrowserContainer::BrCtlInterface
// ---------------------------------------------------------------------------
//
CBrCtlInterface* CHsBrowserContainer::BrCtlInterface()
    {
    return iBrCtlInterface;
    }

// ---------------------------------------------------------------------------
// CHsBrowserContainer::HsBrowserSpecialLoadObserver
// ---------------------------------------------------------------------------
//
CHsBrowserSpecialLoadObserver* CHsBrowserContainer::
    HsBrowserSpecialLoadObserver()
    {
    return iHsBrowserSpecialLoadObserver;
    }
        
// ---------------------------------------------------------------------------
// CHsBrowserContainer::HsBrowserIctsObserver
// ---------------------------------------------------------------------------
//
CHsBrowserIctsObserver* CHsBrowserContainer::HsBrowserIctsObserver()
    {
    return iHsBrowserIctsObserver;
    }
        
// ---------------------------------------------------------------------------
// CHsBrowserContainer::Model
// ---------------------------------------------------------------------------
//
CHsBrowserModel* CHsBrowserContainer::Model()
    {
    return iModel;
    }
    
// ---------------------------------------------------------------------------
// CHsBrowserContainer::NaviLabel
// ---------------------------------------------------------------------------
//
CAknNaviLabel* CHsBrowserContainer::NaviLabel()
    {
    return NULL;
    }

// ---------------------------------------------------------------------------
// CHsBrowserContainer::ActiveInputBox
// ---------------------------------------------------------------------------
//
TBool CHsBrowserContainer::ActiveInputBox()
    {
	return iActiveInputBox;
    }

// ---------------------------------------------------------------------------
// CHsBrowserContainer::SetActiveInputBox
// ---------------------------------------------------------------------------
//
void CHsBrowserContainer::SetActiveInputBox( TBool aActiveInputBox )
    {
	iActiveInputBox = aActiveInputBox;
    }

#endif
