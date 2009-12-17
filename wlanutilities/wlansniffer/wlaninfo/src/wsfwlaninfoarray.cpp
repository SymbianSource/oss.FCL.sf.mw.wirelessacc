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
* Description:  Implementation of CWsfWlanInfoArray
*
*/



//  EXTERNAL INCLUDES
#include <s32mem.h>


//  CLASS HEADER
#include "wsfwlaninfoarray.h"


//  INTERNAL INCLUDES
#include "wsfwlaninfoarraysortkey.h"
#include "wsfwlaninfoarrayfiltervisitor.h"


using namespace CMManager;

//  LOCAL DEFINITIONS
static const TUint KInfoArrayGranuality = 10;    

    
// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::NewL
// ---------------------------------------------------------------------------
//    
EXPORT_C CWsfWlanInfoArray* CWsfWlanInfoArray::NewL()
	{
	CWsfWlanInfoArray* thisPtr = NewLC();
	CleanupStack::Pop( thisPtr );
	return thisPtr; 
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::NewLC
// ---------------------------------------------------------------------------
//    
EXPORT_C CWsfWlanInfoArray* CWsfWlanInfoArray::NewLC()
	{
	CWsfWlanInfoArray* thisPtr = new ( ELeave ) CWsfWlanInfoArray();
	CleanupStack::PushL( thisPtr );
	thisPtr->ConstructL();
	return thisPtr;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::~CWsfWlanInfoArray
// ---------------------------------------------------------------------------
//    
CWsfWlanInfoArray::~CWsfWlanInfoArray()
	{
	if ( iInfoArray )
		{
		iInfoArray->ResetAndDestroy();
    	delete iInfoArray;		
		}
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::CWsfWlanInfoArray
// ---------------------------------------------------------------------------
//    
CWsfWlanInfoArray::CWsfWlanInfoArray() :
    iUIPrioritySort( EFalse )
	{
	}
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::ConstructL
// ---------------------------------------------------------------------------
//    
void CWsfWlanInfoArray::ConstructL()
	{
	iInfoArray = new (ELeave) CArrayPtrFlat<TWsfWlanInfo>( 
	                                                    KInfoArrayGranuality );
	}
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::Reset
// ---------------------------------------------------------------------------
//    
EXPORT_C void CWsfWlanInfoArray::Reset()
	{
	iInfoArray->ResetAndDestroy();
	}
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::Count
// ---------------------------------------------------------------------------
//    
EXPORT_C TUint CWsfWlanInfoArray::Count()
	{
	return iInfoArray->Count();	
	}
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::AppendL
// ---------------------------------------------------------------------------
//    
EXPORT_C void CWsfWlanInfoArray::AppendL( TWsfWlanInfo* aWlanInfo )
	{
    iInfoArray->AppendL( aWlanInfo );
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::At
// ---------------------------------------------------------------------------
//    
EXPORT_C TWsfWlanInfo* CWsfWlanInfoArray::At( TInt aIndex ) const
    {
    TWsfWlanInfo* temp( NULL );
    TInt count = iInfoArray->Count();
    
    if ( count && ( aIndex < count ) )
        {
        temp = ( *iInfoArray )[aIndex];
        } 
    return temp;
    }    
    

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::Delete
// ---------------------------------------------------------------------------
//    
EXPORT_C void CWsfWlanInfoArray::Delete( TWsfWlanInfo* aWlanInfo )
	{
	TInt count = iInfoArray->Count();
    
    for( TInt i = 0; i < count; ++i )
        {
        if ( ( *iInfoArray )[i] == aWlanInfo )
            {
            delete ( *iInfoArray )[i];
            iInfoArray->Delete( i );
            iInfoArray->Compress();
            break;
            }
        }

	}
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::GetArrayIndex
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CWsfWlanInfoArray::GetArrayIndex( TWsfWlanInfo* aWlanInfo ) const
	{
	TBool found( EFalse );
    TInt index( 0 );
    TWsfWlanInfo* temp;
    TInt count = iInfoArray->Count();

    while( ( index <  count ) && !found  )
        {
        temp = ( *iInfoArray )[index]; 
        if ( !temp->iSsid.Compare( aWlanInfo->iSsid ) )
            {
            found = ETrue;
            }
        else
            {
            ++index;            
            }        
        }
    return ( found ? index : KErrNotFound );        
	}	
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::operator[]
// ---------------------------------------------------------------------------
//    
EXPORT_C TWsfWlanInfo* CWsfWlanInfoArray::operator[]( TUint aIndex ) const
    {
    return ( *iInfoArray )[aIndex];
    }	
    

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::Match
// ---------------------------------------------------------------------------
//    
EXPORT_C TWsfWlanInfo* CWsfWlanInfoArray::Match( const TDesC8& aSsid, 
                                                 const TInt aPriorThis )
	{
	TInt count = iInfoArray->Count();
    TWsfWlanInfo* ret( NULL );

    count = count < aPriorThis ? count: aPriorThis;

    for( TInt i = 0; i < count; i++ )
        {
        TWsfWlanInfo* temp = ( *iInfoArray )[i]; 
        if ( !temp->iSsid.Compare( aSsid ) )
            {
			ret = temp;
			break;
            }
        }
	return ret;
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::Match
// ---------------------------------------------------------------------------
//    
EXPORT_C TWsfWlanInfo* CWsfWlanInfoArray::Match( const TUint32 aIapID, 
                                                 const TInt aPriorThis )
    {
    TInt count = iInfoArray->Count();
    TWsfWlanInfo* ret( NULL );

    count = count < aPriorThis ? count: aPriorThis;

    for( TInt i = 0; i < count; i++ )
        {
        TWsfWlanInfo* temp = ( *iInfoArray )[i]; 
        if ( temp->iIapId == aIapID )
            {
            ret = temp;
            break;
            }
        }
    return ret;
    }


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::MatchWithIapIDL
// ---------------------------------------------------------------------------
//      
EXPORT_C void CWsfWlanInfoArray::MatchWithIapIDL( const TUint aIapID, 
                                                   const TInt aPriorThis, 
                                                   RPointerArray<TWsfWlanInfo>& aMatchArray )
    {
    aMatchArray.Reset();

    TInt count = iInfoArray->Count();
    TWsfWlanInfo* temp(NULL);

    count = count < aPriorThis ? count: aPriorThis;

    for( TInt i = 0; i < count ; i++ )
        {
        temp = ( *iInfoArray )[i]; 
        if( temp->iIapId == aIapID )
            {
            aMatchArray.AppendL(temp);
            }
        }
    }


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::MatchL
// ---------------------------------------------------------------------------
//    	
EXPORT_C void CWsfWlanInfoArray::MatchL( const TDesC8& aSsid, 
                                         CMManager::TWlanSecMode aSecMode, 
                                         CMManager::TWlanNetMode aNetMode, 
                                         const TInt aPriorThis, 
                                         RPointerArray<TWsfWlanInfo>& aMatchArray )
    {
    aMatchArray.Reset();

    TInt count = iInfoArray->Count();
    TWsfWlanInfo* temp(NULL);

    count = count < aPriorThis ? count: aPriorThis;

    for( TInt i = 0; i < count ; i++ )
        {
        temp = ( *iInfoArray )[i]; 
        if( !temp->iSsid.Compare( aSsid ) && temp->iSecurityMode == aSecMode && temp->iNetMode == aNetMode)
            {
            aMatchArray.AppendL(temp);
            }
        }
    }
// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::SerializeContentLC
// ---------------------------------------------------------------------------
//    
EXPORT_C HBufC8* CWsfWlanInfoArray::SerializeContentLC()
	{
	HBufC8* buffer( NULL );
	
	if ( !iInfoArray->Count() )
		{
		buffer = KNullDesC8().AllocLC();
		}
	else
	    {
    	TInt32 requiredBufferSize = sizeof( TUint32 ) + 
    	                          iInfoArray->Count() * sizeof( TWsfWlanInfo );
    	buffer = HBufC8::NewLC( requiredBufferSize );
    	
    	// create a stream..
    	TPtr8 bufferPtr = buffer->Des();
    	RDesWriteStream writeStream( bufferPtr );
    	writeStream.Open( bufferPtr);
    	CleanupClosePushL( writeStream );
    	writeStream.WriteInt16L( iInfoArray->Count() );
    	TWsfWlanInfo* infoPtr = NULL;
     	for ( TInt i( 0 ); i < iInfoArray->Count(); i++)
     		{
     		infoPtr = ( *iInfoArray )[i]; 
     		writeStream << *infoPtr;	
     		}
     	writeStream.CommitL();
     	// try to realloc - save space realloc to required level..
     	TStreamPos position = writeStream.Sink()->TellL( MStreamBuf::EWrite );
     
        CleanupStack::PopAndDestroy( &writeStream ); // closes the stream
     	CleanupStack::Pop( buffer );
     	HBufC8* newBuffer = buffer->ReAlloc( position.Offset() );
    	if ( newBuffer ) // the realloc succeeded - swap the buffer 
    		{
    		buffer = newBuffer;
    		}
    	CleanupStack::PushL( buffer);		
	    }

 	return buffer;
	}
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::AppendFromStreamBufferL
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CWsfWlanInfoArray::AppendFromStreamBufferL( 
                                                  const TDesC8& aStreamBuffer )
	{
	RDesReadStream reader( aStreamBuffer );
	reader.Open( aStreamBuffer ); // codescanner false alarm
	// ignoring Open() return value.. Open returns void
	CleanupClosePushL( reader );
	
	TInt infoCount = reader.ReadInt16L();
	
	TWsfWlanInfo *infoPtr = NULL;
	for ( TInt i(0); i < infoCount; i++ )
		{
		infoPtr = new (ELeave)TWsfWlanInfo;
		CleanupStack::PushL( infoPtr );
		reader >> *infoPtr;
		AppendL( infoPtr );
		CleanupStack::Pop( infoPtr );
		}

	CleanupStack::PopAndDestroy( &reader ); 
	return Count();
	}
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::FindHiddenEntry
// ---------------------------------------------------------------------------
//    
EXPORT_C TWsfWlanInfo* CWsfWlanInfoArray::FindHiddenEntry( 
                                                        const TInt aPriorThis )
	{
	TInt count = iInfoArray->Count();
    TWsfWlanInfo* ret( NULL );
    count = count < aPriorThis ? count : aPriorThis; 
    
    for ( TInt i = 0; i < count; ++i )
        {
        TWsfWlanInfo* temp = ( *iInfoArray )[i]; 
        if ( !temp->iVisibility )
            {
			ret = temp;
			break;
            }
        }
	return ret;
	}
	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::DeleteFromTail
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CWsfWlanInfoArray::DeleteFromTail( const TInt aCount )
	{
	TInt arrayItemCount = iInfoArray->Count();

	if ( aCount >= arrayItemCount )
		{
		iInfoArray->ResetAndDestroy();
		}
    else
        {
        --arrayItemCount;
    	for ( TInt i( 1 ) ; i <= aCount; ++i, --arrayItemCount )
    		{
          	delete ( *iInfoArray )[arrayItemCount];
            iInfoArray->Delete( arrayItemCount );
    		}
        iInfoArray->Compress();
        }

    return iInfoArray->Count();
  	}
  	

// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::SortArrayL
// ---------------------------------------------------------------------------
//    
EXPORT_C void CWsfWlanInfoArray::SortArrayL()
	{
	if ( iInfoArray->Count() > 1 ) 
		{
        CWsfWlanInfoArraySortKey* sortKey = CWsfWlanInfoArraySortKey::NewLC( 
                                                                       *this );

        // Sort returns KErrGeneral if stack overflow, otherwise, returns
        // KErrNone. So we will Leave only if stack overflow,
        // but than that really does not matter...
        User::LeaveIfError( iInfoArray->Sort( *sortKey ) );	

        CleanupStack::PopAndDestroy( sortKey );
		}			
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::SortArrayL
// ---------------------------------------------------------------------------
//    
EXPORT_C void CWsfWlanInfoArray::SortArrayL( 
                                     MWsfWlanInfoArrayFilterVisitor& aSortKey )
	{
	// the visitor sortkey may do some other stuff too 
	// so don't make any asumptions...
	// just go for the sorting...
	
	aSortKey.FilterPreSortL( this );
	
	User::LeaveIfError( iInfoArray->Sort( aSortKey.SortingKey() ) );
	
	aSortKey.FilterPostSortL( this );
	}


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::SetUIPrioritySort
// ---------------------------------------------------------------------------
//    
EXPORT_C void CWsfWlanInfoArray::SetUIPrioritySort( const TBool aUIPrioritySort )
    {
    iUIPrioritySort = aUIPrioritySort;
    }


// ---------------------------------------------------------------------------
// CWsfWlanInfoArray::GetUIPrioritySort
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool CWsfWlanInfoArray::GetUIPrioritySort( )
    {
    return iUIPrioritySort;
    }

