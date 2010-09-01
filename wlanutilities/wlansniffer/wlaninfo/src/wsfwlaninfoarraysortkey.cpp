/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CWsfWlanInfoArraySortKey
*
*/


// CLASS HEADER
#include "wsfwlaninfoarraysortkey.h"

// INTERNAL INCLUDES
#include "wsfwlaninfoarray.h"


// CONSTANTS
static const TInt KLeftFirst = -1;
static const TInt KRightFirst = 1;



// ---------------------------------------------------------------------------
// CWsfWlanInfoArraySortKey::NewL
// ---------------------------------------------------------------------------
//    
CWsfWlanInfoArraySortKey* CWsfWlanInfoArraySortKey::NewLC( 
                                                    CWsfWlanInfoArray& aArray )
	{
	CWsfWlanInfoArraySortKey* thisPtr = new (ELeave) CWsfWlanInfoArraySortKey( 
	                                                                  aArray );
	CleanupStack::PushL( thisPtr );
	// no ConstructL at this stage required
	return thisPtr;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArraySortKey::NewL
// ---------------------------------------------------------------------------
//    
CWsfWlanInfoArraySortKey::CWsfWlanInfoArraySortKey( CWsfWlanInfoArray& aArray )
	: TKeyArrayFix( 0, ECmpNormal )
	{
	iArray = &aArray;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArraySortKey::NewL
// ---------------------------------------------------------------------------
//    
CWsfWlanInfoArraySortKey::~CWsfWlanInfoArraySortKey()
	{
	iArray = NULL; // not owning
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArraySortKey::NewL
// ---------------------------------------------------------------------------
//    
TInt CWsfWlanInfoArraySortKey::Compare( TInt aLeft, TInt aRight ) const
	{
	TWsfWlanInfo* left = iArray->At( aLeft );
	TWsfWlanInfo* right = iArray->At( aRight );
	TBool uiPrioritySort = iArray->GetUIPrioritySort();

    TInt ret( 0 );
    
    // connected goes first 
 	if ( left->Connected() )
 	    {
 	    ret = KLeftFirst;
 	    }
 	else if ( right->Connected() )
 	    {
 	    ret = KRightFirst;
 	    }
 	
 	// then network with connecting status
 	else if ( left->ConnectionStatus() == EConnecting )
 	    {
 	    ret = KLeftFirst;
 	    }
 	else if ( right->ConnectionStatus() == EConnecting )
        {
        ret = KRightFirst;
        }
 	
 	// then customer favourite networks
 	else if ( left->BrandId() )
 	    {
 	    if ( right->BrandId() )
 	        {
 	        // smaller brand id first
 	        ret = left->BrandId() - right->BrandId();
 	        }
 	    else
 	        {
 	        ret = KLeftFirst;
 	        }
 	    }
 	else if ( right->BrandId() )
 	    {
 	    ret = KRightFirst;
 	    }
 	
 	// then ui priority networks 
    else if ( uiPrioritySort && left->Priority() && !right->Priority() )
        {
        ret = KLeftFirst;
        }
    else if ( uiPrioritySort && !left->Priority() && right->Priority() )
        {
        ret = KRightFirst;
        }
    else if ( uiPrioritySort && left->Priority() && right->Priority() )
        {
        if ( left->Priority() < right->Priority() )
            {
            ret = KLeftFirst;
            }
        else
            {
            ret = KRightFirst;
            }
        }
 	
 	// then known networks
 	else if ( left->Known() && !right->Known() )
 	    {
 	    ret = KLeftFirst;
 	    }
 	else if ( !left->Known() && right->Known() )
 	    {
 	    ret = KRightFirst;
 	    }
 	    
 	// finally SSID alphabetic order decides
 	else
 	    {
 	    if ( ( left->iNetworkName.Length() ) && 
 	         ( right->iNetworkName.Length() ) )
 	        {
 	        ret = left->iNetworkName.CompareC( right->iNetworkName );
 	        }
 	    else if ( left->iNetworkName.Length() )
 	        {
 	        ret = left->iNetworkName.CompareC( right->iSsid );
 	        }
 	    else if ( right->iNetworkName.Length() )
 	        {
 	        ret = left->iSsid.CompareC( right->iNetworkName );
 	        }
        else
            {
            ret = left->iSsid.CompareC( right->iSsid );
            }
 	    }
 	    
 	return ret;
	}

