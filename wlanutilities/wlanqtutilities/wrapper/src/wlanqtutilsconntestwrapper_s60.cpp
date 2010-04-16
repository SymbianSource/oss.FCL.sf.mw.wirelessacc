/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

// INCLUDE FILES
#include <e32base.h>
#include <ictscommon.h>

#include "wlanqtutilsconntestwrapper.h"
#include "wlanqtutilsconntestwrapper_s60_p.h"

#ifdef WLANQTUTILS_NO_OST_TRACES_FLAG
#include <opensystemtrace.h>
#else
#include "OstTraceDefinitions.h"
#endif
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutilsconntestwrapper_s60Traces.h"
#endif


// =========== PRIVATE CLASS MEMBER FUNCTIONS ===============
//
ConnTestWrapperPrivate::ConnTestWrapperPrivate(
    ConnTestWrapper *aWrapper )
: q_ptr( aWrapper ), iIct( NULL )
    {
    OstTraceFunctionEntry1( CONNTESTWRAPPERPRIVATE_CONNTESTWRAPPERPRIVATE_ENTRY, this );
    OstTraceFunctionExit1( CONNTESTWRAPPERPRIVATE_CONNTESTWRAPPERPRIVATE_EXIT, this );
    }

ConnTestWrapperPrivate::~ConnTestWrapperPrivate()
    {
    OstTraceFunctionEntry1( CONNTESTWRAPPERPRIVATE_CONNTESTWRAPPERPRIVATEDESTR_ENTRY, this );
    
    if ( !NULL ) 
        {
        delete iIct;
        }        
    
    OstTraceFunctionExit1( CONNTESTWRAPPERPRIVATE_CONNTESTWRAPPERPRIVATEDESTR_EXIT, this );
    }

void ConnTestWrapperPrivate::ConnectivityObserver( 
    TIctsTestResult aResult, 
    const TDesC& /*aString*/ )
    {
    OstTraceFunctionEntry1( CONNTESTWRAPPERPRIVATE_CONNECTIVITYOBSERVER_ENTRY, this );
    OstTrace1(
        TRACE_NORMAL,
        CONNTESTWRAPPERPRIVATE_CONNECTIVITYOBSERVER,
        "ConnTestWrapperPrivate::ConnectivityObserver;aResult=%u", aResult );
    
    switch ( aResult ) 
        {
        case EConnectionOk :
            q_ptr->connectivityTestDone( ETrue );
            break;
        case EHttpAuthenticationNeeded :
        case EConnectionNotOk :
        case ETimeout :
        default:
            q_ptr->connectivityTestDone( EFalse );
            break;
        }
    
    delete iIct;
    iIct = NULL;

    OstTraceFunctionExit1( CONNTESTWRAPPERPRIVATE_CONNECTIVITYOBSERVER_EXIT, this );
    }

void ConnTestWrapperPrivate::startConnectivityTest(
    int aIapId, int aNetId )
    {
    OstTraceFunctionEntry1( CONNTESTWRAPPERPRIVATE_STARTCONNECTIVITYTEST_ENTRY, this );
    OstTraceExt2(
        TRACE_NORMAL,
        CONNTESTWRAPPERPRIVATE_STARTCONNECTIVITYTEST,
        "ConnTestWrapperPrivate::startConnectivityTest;aIapId=%d;aNetId=%d", aIapId, aNetId );
    
    TRAPD( err, startConnectivityTestL( aIapId, aNetId ) );
    
    // Check if startConnectivityTestL() leaved.
    if ( KErrNone != err )
        {
        q_ptr->connectivityTestDone( EFalse );
        }

    OstTraceFunctionExit1( CONNTESTWRAPPERPRIVATE_STARTCONNECTIVITYTEST_EXIT, this );
    }

void ConnTestWrapperPrivate::startConnectivityTestL(
    TInt aIapId, TInt aNetId )
    {
    OstTraceFunctionEntry1( CONNTESTWRAPPERPRIVATE_STARTCONNECTIVITYTESTL_ENTRY, this );
    
    iIct = CIctsClientInterface::NewL( aIapId, aNetId, *this );
    iIct->StartL();
 
    OstTraceFunctionExit1( CONNTESTWRAPPERPRIVATE_STARTCONNECTIVITYTESTL_EXIT, this );
    }

//end of file
