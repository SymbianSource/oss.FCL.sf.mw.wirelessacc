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
* Description:   Inline method definitions for CHsBrowserModel.   
*
*/



#ifndef HSBROWSERMODEL_INL
#define HSBROWSERMODEL_INL

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CHsBrowserModel::IapId
// ---------------------------------------------------------
//
TInt CHsBrowserModel::IapId() const
    {
    return iIapId;	
    }

// ---------------------------------------------------------
// CHsBrowserModel::SetIapId
// ---------------------------------------------------------
//
void CHsBrowserModel::SetIapId( TInt aIapId )
    {
    iIapId = aIapId;	
    }

// ---------------------------------------------------------
// CHsBrowserModel::IapId
// ---------------------------------------------------------
//
TInt CHsBrowserModel::NetId() const
    {
    return iNetId;	
    }

// ---------------------------------------------------------
// CHsBrowserModel::Url
// ---------------------------------------------------------
//
HBufC* CHsBrowserModel::Url()
	{
	return iUrl;
	}

// ---------------------------------------------------------
// CHsBrowserModel::SetNetId
// ---------------------------------------------------------
//
void CHsBrowserModel::SetNetId( TInt aNetId )
    {
    iNetId = aNetId;	
    }

// ---------------------------------------------------------
// CHsBrowserModel::Connection
// ---------------------------------------------------------
//
RConnection& CHsBrowserModel::Connection()
    {
    return iConnection;	
    }

// ---------------------------------------------------------
// CHsBrowserModel::SocketServ
// ---------------------------------------------------------
//
RSocketServ& CHsBrowserModel::SocketServ()
    {
    return iSocketServ;	
    }

// ---------------------------------------------------------
// CHsBrowserModel::HsServer
// ---------------------------------------------------------
//
RHssInterface& CHsBrowserModel::HsServer()
	{
	return iHsServer;
	}

// ---------------------------------------------------------
// CHsBrowserModel::SetUrl
// ---------------------------------------------------------
//
void CHsBrowserModel::SetUrl( const TPtrC& aUrl )
	{
	if ( iUrl )
		{
		delete iUrl;
		iUrl = NULL;
		}
	iUrl = aUrl.Alloc();
	}

// ---------------------------------------------------------
// CHsBrowserModel::Foreground
// ---------------------------------------------------------
//
TBool CHsBrowserModel::Foreground()
	{
	return iForeground;
	}

// ---------------------------------------------------------
// CHsBrowserModel::State
// ---------------------------------------------------------
//
TUint CHsBrowserModel::State()
	{
	return iState;
	}
	
// ---------------------------------------------------------
// CHsBrowserModel::ShowLoginCompleteNote
// ---------------------------------------------------------
//
/*TBool CHsBrowserModel::ShowLoginCompleteNote()
	{
	return iShowLoginCompleteNote;
	}
*/

// ---------------------------------------------------------
// CHsBrowserModel::LoginCompleteNoteShown
// ---------------------------------------------------------
//
TBool CHsBrowserModel::LoginCompleteNoteShown()
	{
	return iLoginCompleteNoteShown;
	}

// ---------------------------------------------------------
// CHsBrowserModel::Position
// ---------------------------------------------------------
//
TInt CHsBrowserModel::Position()
	{
	return iPosition;
	}
	
// ---------------------------------------------------------
// CHsBrowserModel::Priority
// ---------------------------------------------------------
//
TInt CHsBrowserModel::Priority()
	{
	return iPriority;
	}

HBufC* CHsBrowserModel::StaticTitle()
	{
	return iStaticTitle;
	}

HBufC* CHsBrowserModel::PageTitle()
	{
	return iPageTitle;
	}

// ---------------------------------------------------------
// CHsBrowserModel::ShowConnClosedNote
// ---------------------------------------------------------
//
/*
TBool CHsBrowserModel::ShowConnClosedNote()
	{
	return iShowConnClosedNote;
	}
*/

// ---------------------------------------------------------
// CHsBrowserModel::SetState
// ---------------------------------------------------------
//
void CHsBrowserModel::SetState( TUint aState )
	{
	iState = aState;
	}

// ---------------------------------------------------------
// CHsBrowserModel::SetForeground
// ---------------------------------------------------------
//
void CHsBrowserModel::SetForeground( TBool aForeground )
	{
	iForeground = aForeground;
	}

// ---------------------------------------------------------
// CHsBrowserModel::SetShowLoginCompleteNote
// ---------------------------------------------------------
//
/*void CHsBrowserModel::SetShowLoginCompleteNote( TBool aShowLoginCompleteNote )
	{
	iShowLoginCompleteNote = aShowLoginCompleteNote;
	}
*/

// ---------------------------------------------------------
// CHsBrowserModel::SetLoginCompleteNoteShown
// ---------------------------------------------------------
//
void CHsBrowserModel::SetLoginCompleteNoteShown(
    TBool aLoginCompleteNoteShown )
	{
	iLoginCompleteNoteShown = aLoginCompleteNoteShown;
	}

// ---------------------------------------------------------
// CHsBrowserModel::SetShowConnClosedNote
// ---------------------------------------------------------
//
/*
void CHsBrowserModel::SetShowConnClosedNote( TBool aShowConnClosedNote )
	{
	iShowConnClosedNote = aShowConnClosedNote;
	}        
*/

// ---------------------------------------------------------
// CHsBrowserModel::SetStaticTitle
// ---------------------------------------------------------
//
void CHsBrowserModel::SetStaticTitle( const TDesC& aStaticTitle )
	{
	if ( iStaticTitle )
		{
		delete iStaticTitle;
		iStaticTitle = NULL;
		}
	iStaticTitle = aStaticTitle.Alloc();
	}

// ---------------------------------------------------------
// CHsBrowserModel::SetPageTitle
// ---------------------------------------------------------
//        
void CHsBrowserModel::SetPageTitle( const TDesC& aPageTitle )
	{
	if ( iPageTitle )
		{
		delete iPageTitle;
		iPageTitle = NULL;
		}
	iPageTitle = aPageTitle.Alloc();
	}


#endif // HSBROWSERMODEL_INL
