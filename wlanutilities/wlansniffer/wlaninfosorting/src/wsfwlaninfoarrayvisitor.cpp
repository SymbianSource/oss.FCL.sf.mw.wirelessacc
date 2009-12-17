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
* Description:  Implementation of CWsfWlanInfoArrayVisitor
*
*/



//  EXTERNAL INCLUDES
#include <badesca.h> 
#include <e32base.h> 
#include <eikenv.h>
#include <bautils.h>
#include <data_caging_path_literals.hrh>

//  CLASS HEADER
#include "wsfwlaninfoarrayvisitor.h"

//INTERNAL INCLUDES
#include "wsfwlaninfosortingbrand.h"
#include "wsfwlaninfosortingdefault.h"
#include "wsfwlaninfosortingblacklist.h"
#include "wsfwlaninfoarray.h"
#include "wsfwlaninfosortingdbchangenotifier.h"


/**
* Blacklist database identifier
*/
_LIT( KBlacklistDbFormat, "SECURE[2000cf30]" );

/**
* Blacklist database file name
*/
_LIT( KBlacklistDbName, "c:wsfblacklist.db" );

/**
* ROM folder
*/
_LIT( KDriveZ, "z:" );

/**
* Resource file name
*/
_LIT( KResourceFileName, "wsfwlaninfosorting.rsc" );


#ifdef _DEBUG
    _LIT( KCWsfWlanInfoArrayVisitor, "arrayvisitor" );
	#define __ASSERTD( cond ) __ASSERT_DEBUG( (cond),  User::Panic( KCWsfWlanInfoArrayVisitor, __LINE__) )
#else
    #define __ASSERTD( cond )
#endif //_DEBUG


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::NewLC
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfWlanInfoArrayVisitor* CWsfWlanInfoArrayVisitor::NewLC( 
                                                        const TBool aReadOnly )
	{
	CWsfWlanInfoArrayVisitor* thisPtr = new (ELeave) CWsfWlanInfoArrayVisitor( 
	                                                               aReadOnly );
	CleanupStack::PushL( thisPtr );
	thisPtr->ConstructL();
	return thisPtr;
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfWlanInfoArrayVisitor* CWsfWlanInfoArrayVisitor::NewL( 
                                                        const TBool aReadOnly ) 
	{
	CWsfWlanInfoArrayVisitor* thisPtr = NewLC( aReadOnly );
	CleanupStack::Pop();
	return thisPtr;
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::~CWsfWlanInfoArrayVisitor
// ----------------------------------------------------------------------------
//
EXPORT_C CWsfWlanInfoArrayVisitor::~CWsfWlanInfoArrayVisitor()  
	{
	delete iDbChangeNotifier;
	delete iBlackListSorting;
	delete iBrandSorting; 
	delete iDefaultSorting;
	
	iFilterDb.Close();
	
	
	iDbms.Close();
	iFileServer.Close();
	
	// let the resourcefile go..
	if ( iResourceFileOffset != -1 )
		{
		iCoeEnv.DeleteResourceFile( iResourceFileOffset );			
		}

	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::CWsfWlanInfoArrayVisitor
// ----------------------------------------------------------------------------
//
CWsfWlanInfoArrayVisitor::CWsfWlanInfoArrayVisitor( const TBool aReadOnly ): 
    TKeyArrayFix( 0, ECmpNormal ),
	iCoeEnv( *CEikonEnv::Static() ),
	iResourceFileOffset( -1 ),
	iReadOnly( aReadOnly )
	{
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoArrayVisitor::ConstructL()
	{
    TFileName resourceFileName;

    resourceFileName.Append( KDriveZ );
    resourceFileName.Append( KDC_RESOURCE_FILES_DIR );   
    resourceFileName.Append( KResourceFileName );

    BaflUtils::NearestLanguageFile( iCoeEnv.FsSession(), resourceFileName );
    iResourceFileOffset = iCoeEnv.AddResourceFileL( resourceFileName );

	
	User::LeaveIfError( iFileServer.Connect() );
	User::LeaveIfError( iDbms.Connect() );
	
	OpenDataBaseL();

	
	iDefaultSorting = CWsfWlanInfoSortingDefault::NewL();
	iBrandSorting = CWsfWlanInfoSortingBrand::NewL();
	iBlackListSorting = CWsfWlanInfoSortingBlackList::NewL( iFilterDb );
	
	iDbChangeNotifier = CWsfWlanInfoSortingDbChangeNotifier::NewL( iFilterDb );
	iDbChangeNotifier->AppendDbRelatedFilterL( iBlackListSorting );
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::FilterPreSortL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfWlanInfoArrayVisitor::FilterPreSortL( 
                                        CWsfWlanInfoArray* aArrayToBeFiltered )
	{
	__ASSERTD( aArrayToBeFiltered );
	
	// go through the filters  
	// store the array ref
	
	TInt arrayLoopCount( aArrayToBeFiltered->Count() );
	TWsfWlanInfo* wlanInfo;
	
	for ( TInt arrayLoop( 0 ); arrayLoop < arrayLoopCount; ++arrayLoop )
		{
		wlanInfo = aArrayToBeFiltered->At( arrayLoop );
		
		// blacklist sorting must come first to be able to filter out 
		// customer favourite networks, too
		if ( !iBlackListSorting->SetupOnRuleMatch( *wlanInfo ) )
			{
		    iBrandSorting->SetupOnRuleMatch( *wlanInfo );
			}
		}
	
	iInfoArray = aArrayToBeFiltered;
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::FilterPostSortL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfWlanInfoArrayVisitor::FilterPostSortL( 
                                        CWsfWlanInfoArray* aArrayToBeFiltered )
	{
	__ASSERTD( aArrayToBeFiltered );
	
	// go through the filters....
	
	// we could remove the entries which are marked as blacklist entries...
	TInt arrayLoopCount( aArrayToBeFiltered->Count() );
	TWsfWlanInfo* wlanInfo;

	for ( TInt arrayLoop( arrayLoopCount - 1); arrayLoop >= 0; --arrayLoop )
		{
		wlanInfo =  aArrayToBeFiltered->At( arrayLoop );
		if ( wlanInfo->BlackListEntry() && !wlanInfo->Connected() )
			{
			// filter out only non-connected items
			aArrayToBeFiltered->Delete( wlanInfo );
			}
		}
	// and let the iInfoArray expire..
	iInfoArray = NULL;
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::SortingKey
// ----------------------------------------------------------------------------
//
EXPORT_C TKeyArrayFix& CWsfWlanInfoArrayVisitor::SortingKey() 
	{
	return *this;
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::LoadFilterDefinitionsL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfWlanInfoArrayVisitor::LoadFilterDefinitionsL()
	{
	// filter by filter - call the load
	iDefaultSorting->ReadFilterDefsL();
	iBrandSorting->ReadFilterDefsL();
	iBlackListSorting->ReadFilterDefsL();
	if ( iReadOnly && !iDbChangeNotifier->IsActive() )
		{
		iDbChangeNotifier->StartDbObservation();
		}
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::AppendBrandingIconsL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfWlanInfoArrayVisitor::AppendBrandingIconsL( 
                                                    CAknIconArray& aIconArray )
	{
	iBrandSorting->AppendBrandIconsL( aIconArray );
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::EditBlackListL
// ----------------------------------------------------------------------------
//
EXPORT_C void CWsfWlanInfoArrayVisitor::EditBlackListL( 
                                            CWsfWlanInfoArray& aInfoItemArray )
	{
	iBlackListSorting->EditFilterDefsL( aInfoItemArray );
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::BlackListItemCount
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CWsfWlanInfoArrayVisitor::BlackListItemCount() const
    {
    return iBlackListSorting->BlackListItemCount();
    }
    
	
// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::Compare
// ----------------------------------------------------------------------------
//
TInt CWsfWlanInfoArrayVisitor::Compare( TInt aLeft, TInt aRight ) const
	{
	TWsfWlanInfo* left = iInfoArray->At( aLeft );
	TWsfWlanInfo* right = iInfoArray->At( aRight );
	
	__ASSERTD( left );
	__ASSERTD( right );	
	
	return iDefaultSorting->Compare( *left, *right );
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoArrayVisitor::OpenDataBaseL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoArrayVisitor::OpenDataBaseL()
	{
    if ( iFilterDb.Open( iDbms, KBlacklistDbName, KBlacklistDbFormat ) != 
                                                                     KErrNone )
        {
		User::LeaveIfError( iFilterDb.Create( iDbms, 
		                                      KBlacklistDbName, 
		                                      KBlacklistDbFormat ) );
		
		// create tables in the new database
		CWsfWlanInfoSortingBlackList::NewLC( iFilterDb );
		CleanupStack::PopAndDestroy();
		}
	}


