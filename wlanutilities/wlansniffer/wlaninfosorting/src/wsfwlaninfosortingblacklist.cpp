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
* Description:  Implementation of CWsfWlanInfoSortingBlackList
*
*/



//  EXTERNAL INCLUDES
#include <StringLoader.h>
#include <aknselectionlist.h>
#include <aknnotewrappers.h>
#include <utf.h>

//  CLASS HEADER
#include "wsfwlaninfosortingblacklist.h"

// INTERNAL INCLUDES
#include <wsfwlaninfosorting.rsg>
#include "wsfwlaninfoarray.h"
#include "wsfwlanssidselectiondlg2.h"
#include "wsflogger.h"


/**
* SSID array granularity
*/
static const TInt KIntSsidArrayGranularity = 3;

/**
* SSID blacklist table name in DB
*/
_LIT( KBlackListTableName,"WIZARD_BLACKLIST");

/**
* SSID column name in blacklist table
*/
_LIT( KBlackListTableIdColumnName,"WIZARD_BLACKLIST_SSID");  



// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::NewLC
// ----------------------------------------------------------------------------
//
CWsfWlanInfoSortingBlackList* CWsfWlanInfoSortingBlackList::NewLC( 
                                                  RDbNamedDatabase& aDatabase )
	{
	CWsfWlanInfoSortingBlackList* thisPtr = 
	                    new (ELeave) CWsfWlanInfoSortingBlackList( aDatabase );
	CleanupStack::PushL( thisPtr );
	thisPtr->ConstructL();
	return thisPtr;
	}
	
	
// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::NewL
// ----------------------------------------------------------------------------
//
CWsfWlanInfoSortingBlackList* CWsfWlanInfoSortingBlackList::NewL( 
                                                  RDbNamedDatabase& aDatabase )
	{
	CWsfWlanInfoSortingBlackList* thisPtr = NewLC( aDatabase );
	CleanupStack::Pop( thisPtr );
	return thisPtr;
	}
	
	
// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::CWsfWlanInfoSortingBlackList
// ----------------------------------------------------------------------------
//
CWsfWlanInfoSortingBlackList::CWsfWlanInfoSortingBlackList( 
                                                 RDbNamedDatabase& aDatabase ):
	iDefsDb( &aDatabase )
	{
	}
	
	
// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::~CWsfWlanInfoSortingBlackList
// ----------------------------------------------------------------------------
//
CWsfWlanInfoSortingBlackList::~CWsfWlanInfoSortingBlackList()
	{
	// free the blacklist entries...
	if ( iBlackListSsids )
		{
		iBlackListSsids->Reset();
		}
	delete iBlackListSsids;
	
	iBlackListSsidTable.Close();
	}
	
	
// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::ConstructL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBlackList::ConstructL()
	{
	CheckTableL();
	iBlackListSsids = new (ELeave) CDesC8ArrayFlat( KIntSsidArrayGranularity );
	}
	

// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::BlackListItemCount
// ----------------------------------------------------------------------------
//
TInt CWsfWlanInfoSortingBlackList::BlackListItemCount() const
    {
    return iBlackListSsids->Count();
    }

	
// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::Compare
// ----------------------------------------------------------------------------
//
TInt CWsfWlanInfoSortingBlackList::Compare( const TWsfWlanInfo& /*aInfoLeft*/, 
                                            const TWsfWlanInfo& /*aInfoRight*/ )
	{
	return 0;
	}
	
	
// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::SetupOnRuleMatch
// ----------------------------------------------------------------------------
//
TBool CWsfWlanInfoSortingBlackList::SetupOnRuleMatch( TWsfWlanInfo& aInfoLeft )
	{
	// look through the ssid array - and look for a match
	TInt rulePosition( 0 );
	
	if ( aInfoLeft.BlackListEntry() )
		{
		return ETrue;
		}
	
	if ( iBlackListSsids->FindIsq( aInfoLeft.iSsid, rulePosition ) == 0 )
		{
		// the rule matches....
		aInfoLeft.SetBlackListEntry( ETrue );
		return ETrue;
		}
		
	return EFalse;
	}
	
	
// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::MarkMatchedFilterL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBlackList::MarkMatchedFilterL( 
                                                   TWsfWlanInfo& aFilterEntry )
	{
	aFilterEntry.SetBlackListEntry( ETrue );
	}
	

// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::EditFilterDefsL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBlackList::EditFilterDefsL( 
                                                CWsfWlanInfoArray& aInfoArray )
	{
	LOG_ENTERFN( "CWsfWlanInfoSortingBlackList::EditFilterDefsL" );
	
	// ok - lets map aInfoArray contents and iBlackListSsids and build our selves 
	// a list of blacklist entries and current non sorted entries...

	// first loop through the aInfoArray and ...

	// make 16 bit copy of the ssid array...
	TInt arrayItemCount( iBlackListSsids->Count() + aInfoArray.Count() );
	if ( !arrayItemCount ) 
		{
		// there's nothing to edit 
		// just return before showing edit dialog
		return;
		}
	
	LOG_WRITE( "there are items to show" );
	CDesCArrayFlat *arrayOfItems = 
	                             new (ELeave) CDesCArrayFlat( arrayItemCount );
	CleanupStack::PushL( arrayOfItems );


	// go through the ssid's in aInfoArray and append them to the array
	TInt wlanArrayCount( aInfoArray.Count() );
	TWsfWlanInfo* wlanInfo( NULL );
	TWsfWlanInfo* connectedWlan( NULL );
	for ( TInt loopCounter( 0 ); loopCounter < wlanArrayCount; ++loopCounter )
		{
		wlanInfo = aInfoArray.At( loopCounter );

//      formerly (custfav or hidden -> does not appear in the list):
//		if ( wlanInfo->BrandId() || wlanInfo->Hidden() )
//      instead now (connected -> does not appear in the list):
		if ( wlanInfo->Connected() )
			{
			connectedWlan = wlanInfo;
			continue;
			}
		HBufC* ssid = wlanInfo->GetSsidAsUnicodeLC();
		arrayOfItems->AppendL( *ssid );
		CleanupStack::PopAndDestroy( ssid );
		}

	// simple alphabetical sort
	arrayOfItems->Sort();

	LOG_WRITE( "items added, list sorted" );


    // now insert the already selected items onto the top
	CArrayFix<TInt> *selectedItems = new (ELeave) CArrayFixFlat<TInt>( 
	                                                          arrayItemCount ); 
 	CleanupStack::PushL( selectedItems );	

    HBufC* connectedSsid( NULL );
    if ( connectedWlan )
        {
        connectedSsid = connectedWlan->GetSsidAsUnicodeLC();
        }

    TBool connectedBlacklisted( EFalse );

	TInt blackListLength( iBlackListSsids->Count() );
	TInt index( 0 );
	for ( TInt loopCounter( 0 ); loopCounter < blackListLength; ++loopCounter )
		{
    	HBufC* ssidUnicode = TWsfWlanInfo::GetSsidAsUnicodeLC(
    	                           iBlackListSsids->MdcaPoint( loopCounter ) );
	
		if ( !connectedWlan || ssidUnicode->Compare( *connectedSsid ) )
		    {
		    // only add the ssid to the list if it is not connected right now
    		arrayOfItems->InsertL( index, *ssidUnicode );
    		// these are already selected
    		selectedItems->AppendL( index );
    		++index;
		    }
		else if ( connectedWlan && !ssidUnicode->Compare( *connectedSsid ) )
	        {
	        connectedBlacklisted = ETrue;
	        }

		    
		CleanupStack::PopAndDestroy( ssidUnicode );
		}
	
	if ( !arrayOfItems->Count() ) 
		{
		// there's nothing to edit
		// just return before showing edit dialog
    	if ( connectedWlan )
    	    {
    	    CleanupStack::PopAndDestroy( connectedSsid );
    	    }
        
    	CleanupStack::PopAndDestroy( selectedItems );	
    	CleanupStack::PopAndDestroy( arrayOfItems );
		return;
		}
	
	LOG_WRITE( "creating the dialog" );
 	CWsfWlanSsidSelectionDlg2* dlg = CWsfWlanSsidSelectionDlg2::NewL( 
 	                                           *arrayOfItems, *selectedItems );
 	dlg->PrepareLC( R_QTN_PREFERRED_FILTER_SELECTION_DIALOG2 );
 
    LOG_WRITE( "launching the dialog" );
 	if ( dlg->RunLD() )
 		{
 		LOG_WRITE( "dialog closed" );
		// go throught the list of selected items... and store them...
		iBlackListSsids->Reset();
		
		TInt selectedCount( selectedItems->Count() );

		for ( TInt loopCounter( 0 ); loopCounter < selectedCount; ++loopCounter )
			{
			TPtrC ssid = arrayOfItems->MdcaPoint( selectedItems->At( 
			                                                   loopCounter ) );
			HBufC8* ssidUtf8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L( 
			                                                            ssid );
            CleanupStack::PushL( ssidUtf8 );
			iBlackListSsids->AppendL( *ssidUtf8 );
			CleanupStack::PopAndDestroy( ssidUtf8 );
			}
			
	    if ( connectedBlacklisted )
	        {
			HBufC8* ssidUtf8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L( 
			                                                  *connectedSsid );
            CleanupStack::PushL( ssidUtf8 );
			iBlackListSsids->AppendL( *ssidUtf8 );
			CleanupStack::PopAndDestroy( ssidUtf8 );
	        }
	        
		iBlackListSsids->Sort();	
		SaveDefsL();	
 		} 
 		
	if ( connectedWlan )
	    {
	    CleanupStack::PopAndDestroy( connectedSsid );
	    }
 		
 	CleanupStack::PopAndDestroy( selectedItems );	
	CleanupStack::PopAndDestroy( arrayOfItems );
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::ReadFilterDefsL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBlackList::ReadFilterDefsL()
	{
	RestoreDefsL();
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::SaveDefsL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBlackList::SaveDefsL()
	{	
    LOG_ENTERFN( "CWsfWlanInfoSortingBlackList::SaveDefsL" );
    
	// clear the db-table
	if ( iBlackListSsidTable.FirstL() != EFalse )
		{
		do 
			{
			iBlackListSsidTable.DeleteL();
			} 
		while ( iBlackListSsidTable.NextL() );
		}
	
	// and add the iBlackListSsids contents there....
	TInt ssidCount( iBlackListSsids->Count() );
	
	if ( ssidCount )
		{
		// there are items
    	CDbColSet* colSet = iBlackListSsidTable.ColSetL();
    	CleanupStack::PushL( colSet );	
    	TDbColNo ssidColumnNumber = colSet->ColNo( 
    	                                         KBlackListTableIdColumnName );
    	
    	for ( TInt i( 0 ); i < ssidCount; ++i )
    		{
    		iBlackListSsidTable.InsertL();
    		iBlackListSsidTable.SetColL( ssidColumnNumber, 
    		                             iBlackListSsids->MdcaPoint( i ) );		
    		iBlackListSsidTable.PutL();
    		}
    	CleanupStack::PopAndDestroy( colSet );
		}
		
	}


// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::RestoreDefsL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBlackList::RestoreDefsL()
	{
	LOG_ENTERFN( "CWsfWlanInfoSortingBlackList::RestoreDefsL" );	
	iBlackListSsids->Reset();
	iBlackListSsidTable.Reset();
	// go through the table and fill the ssid array...
	if ( iBlackListSsidTable.FirstL() )
		{
    	CDbColSet* colSet = iBlackListSsidTable.ColSetL();
    	CleanupStack::PushL( colSet );	
    	TDbColNo ssidColumnNumber = colSet->ColNo( 
    	                                         KBlackListTableIdColumnName );
    	do
    		{
    		iBlackListSsidTable.GetL();
    		iBlackListSsids->AppendL( iBlackListSsidTable.ColDes8( 
    		                                              ssidColumnNumber ) );
    		} 
    	while ( iBlackListSsidTable.NextL() );
    		
    	CleanupStack::PopAndDestroy( colSet );	
    	// sort the array...
    	iBlackListSsids->Sort();

		}
	}
	
	
// ----------------------------------------------------------------------------
// CWsfWlanInfoSortingBlackList::CheckTableL
// ----------------------------------------------------------------------------
//
void CWsfWlanInfoSortingBlackList::CheckTableL()
	{
	LOG_ENTERFN( "CWsfWlanInfoSortingBlackList::CheckTableL" );
	
	TInt tableStatus = iBlackListSsidTable.Open( *iDefsDb, 
	                                             KBlackListTableName );
	if ( tableStatus == KErrNotFound )
		{
		// the table does not exist... create it
		CDbColSet *ssidTableColumnSet = CDbColSet::NewLC();
			
		ssidTableColumnSet->AddL( TDbCol( KBlackListTableIdColumnName, 
		                                  EDbColText8 ) );
		
		iDefsDb->Begin();
		User::LeaveIfError( iDefsDb->CreateTable( KBlackListTableName, 
		                                          *ssidTableColumnSet ) );
		iDefsDb->Commit();
		
		CleanupStack::PopAndDestroy( ssidTableColumnSet );
		}

	if ( tableStatus != KErrNone )
		{
		User::LeaveIfError( iBlackListSsidTable.Open( *iDefsDb,  
		                                              KBlackListTableName ) );
		}
	}




