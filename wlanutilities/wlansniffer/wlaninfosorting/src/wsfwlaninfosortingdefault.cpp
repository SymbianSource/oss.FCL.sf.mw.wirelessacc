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
* Description:  Implementation of CWsfWlanInfoSortingDefault
*
*/



//  CLASS HEADER
#include "wsfwlaninfosortingdefault.h"

// INTERNAL INCLUDES
#include "wsfwlaninfo.h"


/**
* Compare result: left-hand item goes first
*/
static const TInt KLeftFirst = -1;

/**
* Compare result: right-hand item goes first
*/
static const TInt KRightFirst = 1;



// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::NewLC
// ---------------------------------------------------------------------------
//    
CWsfWlanInfoSortingDefault* CWsfWlanInfoSortingDefault::NewLC()
	{
	CWsfWlanInfoSortingDefault* thisPtr = 
	                                 new (ELeave) CWsfWlanInfoSortingDefault();
	CleanupStack::PushL( thisPtr );
	thisPtr->ConstructL();
	return thisPtr;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::NewL
// ---------------------------------------------------------------------------
//    
CWsfWlanInfoSortingDefault* CWsfWlanInfoSortingDefault::NewL()
	{
	CWsfWlanInfoSortingDefault* thisPtr = NewLC();
	CleanupStack::Pop( thisPtr );
	return thisPtr;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::CWsfWlanInfoSortingDefault
// ---------------------------------------------------------------------------
//    
CWsfWlanInfoSortingDefault::CWsfWlanInfoSortingDefault()
	{
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::~CWsfWlanInfoSortingDefault
// ---------------------------------------------------------------------------
//    
CWsfWlanInfoSortingDefault::~CWsfWlanInfoSortingDefault()
	{
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::ConstructL
// ---------------------------------------------------------------------------
//    
void CWsfWlanInfoSortingDefault::ConstructL()
	{
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::Compare
// ---------------------------------------------------------------------------
//    
TInt CWsfWlanInfoSortingDefault::Compare( const TWsfWlanInfo& aLeft, 
                                          const TWsfWlanInfo& aRight )
	{
	TInt ret( 0 );
	
    // connected goes first 
 	if ( aLeft.Connected() )
 	    {
 	    ret = KLeftFirst;
 	    }
 	else if ( aRight.Connected() )
 	    {
 	    ret = KRightFirst;
 	    }
 	
 	// then customer favourite networks
 	else if ( aLeft.BrandId() )
 	    {
 	    if ( aRight.BrandId() )
 	        {
 	        // smaller brand id first
 	        ret = aLeft.BrandId() - aRight.BrandId();
 	        }
 	    else
 	        {
 	        ret = KLeftFirst;
 	        }
 	    }
 	else if ( aRight.BrandId() )
 	    {
 	    ret = KRightFirst;
 	    }
 	
 	
 	// then known networks
 	else if ( aLeft.Known() && !aRight.Known() )
 	    {
 	    ret = KLeftFirst;
 	    }
 	else if ( !aLeft.Known() && aRight.Known() )
 	    {
 	    ret = KRightFirst;
 	    }
 	    
 	// finally SSID alphabetic order decides
 	else
 	    {
 	    if ( ( aLeft.iNetworkName.Length() ) && 
 	         ( aRight.iNetworkName.Length() ) )
 	        {
 	        ret = aLeft.iNetworkName.CompareC( aRight.iNetworkName );
 	        }
 	    else if ( aLeft.iNetworkName.Length() )
 	        {
 	        ret = aLeft.iNetworkName.CompareC( aRight.iSsid );
 	        }
 	    else if ( aRight.iNetworkName.Length() )
 	        {
 	        ret = aLeft.iSsid.CompareC( aRight.iNetworkName );
 	        }
        else
            {
            ret = aLeft.iSsid.CompareC( aRight.iSsid );
            }
 	    }
 	    
 	return ret;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::SetupOnRuleMatch
// ---------------------------------------------------------------------------
//    
TBool CWsfWlanInfoSortingDefault::SetupOnRuleMatch( TWsfWlanInfo& 
                                                                /*aInfoLeft*/ )
	{
	// as a default filter - every rules is a match
	return ETrue;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::MarkMatchedFilterL
// ---------------------------------------------------------------------------
//    
void CWsfWlanInfoSortingDefault::MarkMatchedFilterL( 
                                              TWsfWlanInfo& /*aFilterEntry*/ )
	{
	// we do not modify the contents in default filter...
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::EditFilterDefsL
// ---------------------------------------------------------------------------
//    
void CWsfWlanInfoSortingDefault::EditFilterDefsL( 
                                            CWsfWlanInfoArray& /*aInfoArray*/)
	{
	// we have hardcoded rules... we don't need editui..
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingDefault::ReadFilterDefsL
// ---------------------------------------------------------------------------
//    
void CWsfWlanInfoSortingDefault::ReadFilterDefsL()
	{
	// we have static definitions... nothing to do...
	}



