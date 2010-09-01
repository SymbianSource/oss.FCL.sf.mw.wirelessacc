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
* Description:  Implementation of CWsfWlanInfoSortingDbChangeNotifier
*
*/



//  CLASS HEADER
#include "wsfwlaninfosortingdbchangenotifier.h"

// INTERNAL INCLUDES
#include "wsfwlaninfosortingfiltervisitor.h"



// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::NewLC
// ----------------------------------------------------------------------------
//
CWsfWlanInfoSortingDbChangeNotifier* 
           CWsfWlanInfoSortingDbChangeNotifier::NewLC( RDbDatabase& aDatabase )
	{
	CWsfWlanInfoSortingDbChangeNotifier* thisPtr = 
	             new (ELeave) CWsfWlanInfoSortingDbChangeNotifier( aDatabase );
	CleanupStack::PushL( thisPtr );
	thisPtr->ConstructL();
	return thisPtr;
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::NewL
// ----------------------------------------------------------------------------
//
CWsfWlanInfoSortingDbChangeNotifier* CWsfWlanInfoSortingDbChangeNotifier::NewL(
                                                       RDbDatabase& aDatabase )
	{
	CWsfWlanInfoSortingDbChangeNotifier* thisPtr = NewLC( aDatabase );
	CleanupStack::Pop( thisPtr );
	return thisPtr;
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::~CWsfWlanInfoSortingDbChangeNotifier
// ----------------------------------------------------------------------------
//
CWsfWlanInfoSortingDbChangeNotifier::~CWsfWlanInfoSortingDbChangeNotifier()
	{
	Cancel();
	iDbAwareFilters.Reset();
	iNotifier.Close();
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::CWsfWlanInfoSortingDbChangeNotifier
// ----------------------------------------------------------------------------
//
CWsfWlanInfoSortingDbChangeNotifier::CWsfWlanInfoSortingDbChangeNotifier( 
                                                      RDbDatabase& aDatabase ):
	CActive( CActive::EPriorityLow ),
	iDatabase( &aDatabase )
	{
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingDbChangeNotifier::ConstructL()
	{
	User::LeaveIfError( iNotifier.Open( *iDatabase ) );
	CActiveScheduler::Add( this );

	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::StartDbObservation
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingDbChangeNotifier::StartDbObservation()
	{
	iNotifier.NotifyChange( iStatus );
	SetActive();
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::DoCancel
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingDbChangeNotifier::DoCancel()
	{
	iNotifier.Cancel();
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::RunL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingDbChangeNotifier::RunL()
	{
	if ( iStatus.Int() >= RDbNotifier::ECommit )
		{
		TInt dbAwareFiltersCount( iDbAwareFilters.Count() );
		if ( dbAwareFiltersCount )
			{
			TInt i( 0 );
			for ( ; i < dbAwareFiltersCount ; i++ )
				{
				(iDbAwareFilters)[i]->ReadFilterDefsL();
				}
			}
		}
	StartDbObservation();
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::AppendDbRelatedFilterL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingDbChangeNotifier::AppendDbRelatedFilterL( 
                             MWsfWlanInfoSortingFilterVisitor* aRelatedFilter )
	{
	iDbAwareFilters.AppendL( aRelatedFilter );
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingDbChangeNotifier::RemoveDbRelatedFilter
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingDbChangeNotifier::RemoveDbRelatedFilter( 
                             MWsfWlanInfoSortingFilterVisitor* aRelatedFilter )
	{
	TInt index = iDbAwareFilters.Find( aRelatedFilter );
    if ( index != KErrNotFound )
    	{
    	iDbAwareFilters.Remove( index );
    	}
	}


