/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: WPS Wizard: Wrapper for middleware calls 
*
*/

#ifndef CWPSACTIVERUNNER_H_
#define CWPSACTIVERUNNER_H_

// System includes
#include <QObject>
#include <QStringList>
#include <e32base.h>
#include <wlanmgmtclient.h>

// User includes
#include "mwpsactiverunnercallback.h"

// Forward declarations
// External data types
// Constants


/*! 
 * The wrapper class for wlan management engine calls  
 */
// Class declaration
class CWpsActiveRunner : public CActive
    {

public:
    static CWpsActiveRunner* NewL(MWpsActiveRunnerCallback& aObserver);
    virtual ~CWpsActiveRunner();
public:
    
    void InitL();
    void RunL();
    void DoCancel();
    TInt RunError(TInt aError);
    void StartSetup(RBuf8& aSsid, int aPin);

private:
    CWpsActiveRunner(MWpsActiveRunnerCallback& aObserver);
    void ConstructL();
    void InitializeL();
private:    
    MWpsActiveRunnerCallback& iObserver;
#ifdef __arm
    CWlanMgmtClient* iWLANMgmtClient;
    CArrayFixSeg<TWlanProtectedSetupCredentialAttribute>* iIapParametersArray;
#else
    //Temporary for testing in WINSCW.
    RTimer iTimer;
    void RunProtectedSetup_Stub();
#endif    
    };

#endif /* CWPSACTIVERUNNER_H_ */
