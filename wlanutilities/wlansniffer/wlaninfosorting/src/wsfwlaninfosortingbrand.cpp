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
* Description:  Implementation of CWsfWlanInfoSortingBrand
*
*/



//  EXTERNAL INCLUDES
#include <eikenv.h>
#include <barsread.h>
#include <gulicon.h>
#include <AknIconArray.h>
#include <AknIconUtils.h>
#include <mifconvdefs.h>
#include <wsfwlaninfosorting.rsg>

//  CLASS HEADER
#include "wsfwlaninfosortingbrand.h"



/**
* Compare result: left-hand item goes first
*/
static const TInt KLeftFirst = -1;

/**
* Compare result: right-hand item goes first
*/
static const TInt KRightFirst = 1;



#ifdef _DEBUG
    // Add own assert debug 
    // implementation if needed
	#define __ASSERTD( cond )
#else
    #define __ASSERTD( cond )
#endif //_DEBUG



// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::TBrandableItem::TBrandableItem
// ---------------------------------------------------------------------------
//
CWsfWlanInfoSortingBrand::TBrandableItem::TBrandableItem():
	iSsid( KNullDesC8 ),
	iBrandId( 0 )
	{
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::TBrandableItem::TBrandableItem
// ---------------------------------------------------------------------------
//
CWsfWlanInfoSortingBrand::TBrandableItem::TBrandableItem( const TDesC8& aSsid, 
                                                        const TInt8 aBrandId ):
	iSsid( aSsid ),
	iBrandId( aBrandId )
	{
	}
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::TBrandableItem::Match
// ---------------------------------------------------------------------------
//
TBool CWsfWlanInfoSortingBrand::TBrandableItem::Match(
                                            const TBrandableItem& aItemLeft, 
                                            const TBrandableItem& aItemRight )
	{
	return aItemLeft.iSsid.Compare( aItemRight.iSsid ) == 0;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::TBrandableItem::SortByBrandId
// ---------------------------------------------------------------------------
//
TInt CWsfWlanInfoSortingBrand::TBrandableItem::SortByBrandId( 
                                            const TBrandableItem& aItemLeft, 
                                            const TBrandableItem& aItemRight )
	{
	return aItemLeft.iBrandId - aItemRight.iBrandId;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::TBrandableItem::SortBySsid
// ---------------------------------------------------------------------------
//
TInt CWsfWlanInfoSortingBrand::TBrandableItem::SortBySsid( 
                                            const TBrandableItem& aItemLeft, 
                                            const TBrandableItem& aItemRight )
	{
	return aItemLeft.iSsid.Compare( aItemRight.iSsid );	
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::NewLC
// ---------------------------------------------------------------------------
//
CWsfWlanInfoSortingBrand* CWsfWlanInfoSortingBrand::NewLC()
	{
	CWsfWlanInfoSortingBrand* thisPtr = 
	                                new (ELeave) CWsfWlanInfoSortingBrand();
	CleanupStack::PushL( thisPtr );
	thisPtr->ConstructL();
	return thisPtr;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::NewL
// ---------------------------------------------------------------------------
//
CWsfWlanInfoSortingBrand* CWsfWlanInfoSortingBrand::NewL()
	{
	CWsfWlanInfoSortingBrand* thisPtr = NewLC();
	CleanupStack::Pop( thisPtr );
	return thisPtr;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::CWsfWlanInfoSortingBrand
// ---------------------------------------------------------------------------
//
CWsfWlanInfoSortingBrand::CWsfWlanInfoSortingBrand():
	iBitmapFileName( NULL ),
	iBrandingDataOk( EFalse )
	{
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::~CWsfWlanInfoSortingBrand
// ---------------------------------------------------------------------------
//
CWsfWlanInfoSortingBrand::~CWsfWlanInfoSortingBrand()
	{
	delete iBitmapFileName;
	iBrandItemArray.ResetAndDestroy();
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBrand::ConstructL()
	{
	// nothing to do...
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::Compare
// ---------------------------------------------------------------------------
//
TInt CWsfWlanInfoSortingBrand::Compare( const TWsfWlanInfo& aLeft, 
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
// CWsfWlanInfoSortingBrand::SetupOnRuleMatch
// ---------------------------------------------------------------------------
//
TBool CWsfWlanInfoSortingBrand::SetupOnRuleMatch( TWsfWlanInfo& aInfoLeft )
	{
	TBool ret( EFalse );
	// check if the entry already has an branding flag on - return ETrue
	
	if ( iBrandingDataOk && !aInfoLeft.BrandId() )
		{
    	// using the ssid... try to find the ssid from iBrandItemArray
    	TBrandableItem item;
    	item.iSsid = aInfoLeft.iSsid;
    	
    	TLinearOrder<TBrandableItem> findBinarySsid( 
                    CWsfWlanInfoSortingBrand::TBrandableItem::SortBySsid );
    	
    	TInt entryFindValue = iBrandItemArray.FindInOrder( &item, 
    	                                                  findBinarySsid );
    	if ( entryFindValue != KErrNotFound )
    		{
    		// mark the entry...
    		aInfoLeft.SetBrand( 
    		                iBrandItemArray[entryFindValue]->iBrandId );
    		ret = ETrue;
    		}

		}
		
	return ret;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::MarkMatchedFilterL
// ---------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBrand::MarkMatchedFilterL( 
                                              TWsfWlanInfo& /*aFilterEntry*/ )
	{
	// we do not modify the contents in default filter...
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::EditFilterDefsL
// ---------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBrand::EditFilterDefsL( 
                                           CWsfWlanInfoArray& /*aInfoArray*/ )
	{
	// we have hardcoded rules... we don't need editui..
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::ReadFilterDefsL
// ---------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBrand::ReadFilterDefsL()
	{
	ReadBrandedSsidsL();
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::ReadBrandedSsidsL
// ---------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBrand::ReadBrandedSsidsL()
	{
	iBrandItemArray.ResetAndDestroy();
	iBrandingDataOk = EFalse;

	TResourceReader reader;
	CEikonEnv& env = *CEikonEnv::Static();
	env.CreateResourceReaderLC( reader, R_QTN_SSID_BRANDING );
	
	delete iBitmapFileName;
	iBitmapFileName = NULL;

	// ok - lets read the filename...
	iBitmapFileName = reader.ReadHBufCL();
	
	if ( iBitmapFileName ) // not iconfilename defined.... 
		{
    	// try to locate the icon file 
    	TFindFile finder( env.FsSession() );
    	
    	TParsePtrC parser( *iBitmapFileName );

        TInt findError = finder.FindByDir( parser.NameAndExt(), 
                                           parser.Path() );

    	delete iBitmapFileName;
    	iBitmapFileName = NULL;

        if ( !findError )
        	{
        	iBitmapFileName = finder.File().AllocL();
        	
        	TInt brandedItemCount = reader.ReadInt16();
        	TBrandableItem* brandableItem = NULL;

        	CDesC8ArrayFlat* ssidArray = NULL;
        	TInt iconId( 0 );
        	TInt maskId( 0 );

        	for ( TInt readCount( 0 ); readCount < brandedItemCount; 
        	                                                      ++readCount )
        		{
        		ssidArray = reader.ReadDesC8ArrayL();
        		CleanupStack::PushL( ssidArray );

        		iconId = reader.ReadInt16();
        		maskId = reader.ReadInt16();

        		TInt ssidCount( ssidArray->Count() );
        		
        		for ( TInt ssidIter( 0 ); ssidIter < ssidCount; ssidIter++ )
        			{
        			brandableItem = new (ELeave) TBrandableItem;
        			brandableItem->iSsid = ssidArray->MdcaPoint( ssidIter );
        			brandableItem->iBrandId = readCount + 1;
        			brandableItem->iIconId = iconId;
        			brandableItem->iMaskId = maskId;
        			iBrandItemArray.AppendL( brandableItem );
        			}		
        		CleanupStack::PopAndDestroy( ssidArray );
        		}

        	// and to speed up the ssid match sort the array by ssid
        	TLinearOrder<TBrandableItem> sortingRuleSSId( 
        	            CWsfWlanInfoSortingBrand::TBrandableItem::SortBySsid );
        	
        	iBrandItemArray.Sort( sortingRuleSSId );
        	iBrandingDataOk = ( iBrandItemArray.Count() > 0 );
        	}
    		
		}
        
	CleanupStack::PopAndDestroy(); //reader	        
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::AppendBrandIconsL
// ---------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBrand::AppendBrandIconsL( CAknIconArray& aIconArray )
	{
	if ( iBrandingDataOk )
		{
    	// sort the array by brandid	
    	TLinearOrder<TBrandableItem> sortingRule( 
    	             CWsfWlanInfoSortingBrand::TBrandableItem::SortByBrandId );
    	
    	iBrandItemArray.Sort( sortingRule );
    		
    		
    	// loop here - load icon and append to array....
    	TBrandableItem* item;
    	TInt brandArrayCount( iBrandItemArray.Count() );
    	TInt brandId( 0 );
    	for ( TInt loopCounter( 0 ); loopCounter < brandArrayCount; 
    	                                                        ++loopCounter )
    		{
    		item = iBrandItemArray[ loopCounter ];
    		if ( item->iBrandId > brandId ) 
    			{
        		brandId	= item->iBrandId;
        		aIconArray.AppendL( MakeIconLC( item->iIconId, 
        		                                             item->iMaskId ) );
        		CleanupStack::Pop(); // icon
    			}
    		}
    		
    	// and to speed up the ssid match sort the array by ssid
    	TLinearOrder<TBrandableItem> sortingRuleSSId( 
    	                CWsfWlanInfoSortingBrand::TBrandableItem::SortBySsid );
    	
    	iBrandItemArray.Sort( sortingRuleSSId );

		}
		
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoSortingBrand::MakeIconLC
// ---------------------------------------------------------------------------
//
CGulIcon* CWsfWlanInfoSortingBrand::MakeIconLC( const TInt aIconId, 
                                                const TInt aMaskId )
	{
	CGulIcon* icon = CGulIcon::NewLC();
	CFbsBitmap* bitmap( NULL );   
	CFbsBitmap* mask( NULL );   
      
	// Creates bitmap an icon.
    AknIconUtils::CreateIconL( bitmap, mask, *iBitmapFileName, 
                               aIconId, aMaskId );

   	icon->SetBitmap( bitmap );   
   	icon->SetMask( mask ); 
    return icon;    		  	
	}




