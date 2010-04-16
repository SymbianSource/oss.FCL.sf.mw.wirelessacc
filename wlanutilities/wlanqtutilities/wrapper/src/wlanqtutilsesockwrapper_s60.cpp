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
#include <es_sock.h>
#include <in_sock.h>
#include <commdbconnpref.h>
#include "wlanqtutilsesockwrapper.h"
#include "wlanqtutilsesockwrapper_s60_p.h"

#ifdef WLANQTUTILS_NO_OST_TRACES_FLAG
#include <opensystemtrace.h>
#else
#include "OstTraceDefinitions.h"
#endif
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanqtutilsesockwrapper_s60Traces.h"
#endif


// =========== PRIVATE CLASS MEMBER FUNCTIONS ===============
//
// ---------------------------------------------------------
// EsockWrapperPrivate::EsockWrapperPrivate()
// Constructor
// ---------------------------------------------------------
//
EsockWrapperPrivate::EsockWrapperPrivate(EsockWrapper *aWrapper)
: CActive(EPriorityStandard), q_ptr(aWrapper)
    {
    OstTraceFunctionEntryExt( ESOCKWRAPPERPRIVATE_ESOCKWRAPPERPRIVATE_ENTRY, this );
    
    CActiveScheduler::Add(this);
    
    iSocketServer.Connect();
    
    OstTraceFunctionExit1( ESOCKWRAPPERPRIVATE_ESOCKWRAPPERPRIVATE_EXIT, this );
    }

// ---------------------------------------------------------
// EsockWrapperPrivate::EsockWrapperPrivate()
// Destructor
// ---------------------------------------------------------
//
EsockWrapperPrivate::~EsockWrapperPrivate()
    {
    OstTraceFunctionEntry1( ESOCKWRAPPERPRIVATE_ESOCKWRAPPERPRIVATEDESTR_ENTRY, this );
    
    Cancel();
    // Closing active RConnection is not mandatory, but is recommended.
    // ==> add checking here when implementing cancel/error cases.
    iSocketServer.Close();
    
    OstTraceFunctionExit1( ESOCKWRAPPERPRIVATE_ESOCKWRAPPERPRIVATEDESTR_EXIT, this );
    }

// ---------------------------------------------------------
// EsockWrapperPrivate::connectIap()
// Start connection creation to given IAP.
// ---------------------------------------------------------
//
void EsockWrapperPrivate::connectIap(int aIapId)
    {
    OstTraceFunctionEntryExt( ESOCKWRAPPERPRIVATE_CONNECTIAP_ENTRY, this );
    
    // Open an RConnection object.
    iConnection.Open(iSocketServer);
    
    // Create overrides to force opening of the given IAP without any user prompts.
    TCommDbConnPref prefs;
    prefs.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
    prefs.SetDirection(ECommDbConnectionDirectionOutgoing);
    prefs.SetIapId(aIapId);
    
    // Start the Connection with overrides
    iConnection.Start(prefs, iStatus);
    
    // TODO: Currently SetActive Panics when connecting "furiously" in Visual view...
    // Panicking line in SetActive was this:
    // __ASSERT_ALWAYS(!(iStatus.iFlags&TRequestStatus::EActive),Panic(EReqAlreadyActive));
    SetActive();
    
    OstTraceFunctionExit1( ESOCKWRAPPERPRIVATE_CONNECTIAP_EXIT, this );
    }

// ---------------------------------------------------------
// EsockWrapperPrivate::disconnectIap()
// Stop connection.
// ---------------------------------------------------------
//
void EsockWrapperPrivate::disconnectIap()
    {      
    OstTraceFunctionEntry1( ESOCKWRAPPERPRIVATE_DISCONNECTIAP_ENTRY, this );
    
    // TODO: Error checking
    iConnection.Close();
    
    OstTraceFunctionExit1( ESOCKWRAPPERPRIVATE_DISCONNECTIAP_EXIT, this );
    }

// ---------------------------------------------------------
// EsockWrapperPrivate::RunL()
// Called when connection creation has finished.
// ---------------------------------------------------------
//
void EsockWrapperPrivate::RunL()
    {
    OstTraceFunctionEntry1( ESOCKWRAPPERPRIVATE_RUNL_ENTRY, this );
    OstTrace1( TRACE_NORMAL, ESOCKWRAPPERPRIVATE_RUNL, "EsockWrapperPrivate::RunL;iStatus.Int()=%d", iStatus.Int() );
    
    bool success = false;
    
    if (iStatus == KErrNone)
        {
        success = true;
        }
    
    q_ptr->updateConnection(success);
    
    OstTraceFunctionExit1( ESOCKWRAPPERPRIVATE_RUNL_EXIT, this );
    }

// ---------------------------------------------------------
// EsockWrapperPrivate::DoCancel()
// 
// ---------------------------------------------------------
//
void EsockWrapperPrivate::DoCancel()
    {
    OstTraceFunctionEntry1( ESOCKWRAPPERPRIVATE_DOCANCEL_ENTRY, this );
    OstTraceFunctionExit1( ESOCKWRAPPERPRIVATE_DOCANCEL_EXIT, this );
    }

//end of file
