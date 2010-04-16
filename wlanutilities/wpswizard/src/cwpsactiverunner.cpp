/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of the License "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description: Implementation of middleware wrapper class
 *   
 *
 */


// User includes
#include "cwpsactiverunner.h"
#include "wlanmgmtcommon.h"

//Need to check this.
const TInt KArrayGranularity = 4;

/*!
 \class CWpsActiveRunner
 \brief CWpsActiveRunner is a wrapper class wlan Mgmt engine interface
 */


/*!
 * Creates the instance of active runner which encapsulates the wlan mgmt
 * interface middleware calls
 * 
 * 
 * \param MWpsActiveRunnerCallback call back interface to notify the completion of the middleware calls
 */
CWpsActiveRunner* CWpsActiveRunner::NewL(MWpsActiveRunnerCallback& aObserver)
{
    CWpsActiveRunner* self = new (ELeave) CWpsActiveRunner( aObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}
/*!
 * Creates the instance of active runner which encapsulates the wlan mgmt
 * interface middleware calls
 * 
 * 
 * \param MWpsActiveRunnerCallback call back interface to notify the completion of the middleware calls
 */
CWpsActiveRunner::CWpsActiveRunner(MWpsActiveRunnerCallback& aObserver) :
    CActive(EPriorityStandard), iObserver(aObserver)
{
#ifdef __arm
    iIapParametersArray = NULL;
    iWLANMgmtClient = NULL;
#endif
}

/*!
 * Initializes the member variables
 * 
 * 
 * \return integer error code indicating the result of the operation
 */
void CWpsActiveRunner::InitL()
{
    CActiveScheduler::Add(this);
#ifdef __arm
    InitializeL();
#else
    iTimer.CreateLocal();
#endif

}

void CWpsActiveRunner::ConstructL()
{
    InitL();
}

/*!
 * Initializes the member variables for making the middleware calls
 * 
 */

void CWpsActiveRunner::InitializeL()
{
#ifdef __arm    
    if (!iWLANMgmtClient)
        iWLANMgmtClient = CWlanMgmtClient::NewL();
    if (!iIapParametersArray)
        iIapParametersArray = new (ELeave) CArrayFixSeg<
    TWlanProtectedSetupCredentialAttribute> (KArrayGranularity);
#endif    
}

/*!
 * Destructor
 * 
 */

CWpsActiveRunner::~CWpsActiveRunner()
{
    Cancel();
#ifdef __arm
    delete iWLANMgmtClient;
    delete iIapParametersArray;
#else
    iTimer.Cancel();
    iTimer.Close();
#endif
}

/*!
 * Initiates the call to the middleware
 */
void CWpsActiveRunner::StartSetup(RBuf8& aSsid, int aPin)
{
#ifdef __arm
    TWlanSsid ssid;
    TWlanWpsPin pin;

    ssid.Copy(aSsid);
    TBuf8<32> pinCode;
    pinCode.AppendNum(aPin);
    pin.Copy(pinCode);
    iWLANMgmtClient->RunProtectedSetup(iStatus,ssid,pin,*iIapParametersArray);
#else
    iTimer.After(iStatus, 4 * 1000000);
#endif
    SetActive();

}

/*!
 * RunL implementation
 */
void CWpsActiveRunner::RunL()
{
    TInt completionCode = iStatus.Int();
    QList<TWlanProtectedSetupCredentialAttribute> credentials;

    if (completionCode < KErrNone) {
        //Raise Error
        iObserver.WpsActiveRunnerStopped(credentials, completionCode);
    }
    else {
#ifdef __arm
        TInt len = iIapParametersArray->Length();
        TInt count;
        for(count=0;count<iIapParametersArray->Count();count++)
        {
            TWlanProtectedSetupCredentialAttribute attr =(*iIapParametersArray)[count];
            credentials.append(attr);
        }
        iObserver.WpsActiveRunnerStopped(credentials,completionCode);
#else

        RunProtectedSetup_Stub();
#endif
    }

}

/*!
 * Cancels the ongoing call
 */
void CWpsActiveRunner::DoCancel()
{
#ifdef __arm    
    if(iWLANMgmtClient)
    iWLANMgmtClient->CancelProtectedSetup();
#else
    iTimer.Cancel();
#endif
}

/*!
 * Handles the error usecase
 */
TInt CWpsActiveRunner::RunError(TInt aError)
{
    QList<TWlanProtectedSetupCredentialAttribute> credentials;
#ifdef __arm    
    if(iWLANMgmtClient)
    iWLANMgmtClient->CancelProtectedSetup();
#else
    iTimer.Cancel();
#endif

    iObserver.WpsActiveRunnerStopped(credentials, aError);
}

#ifndef __arm
void CWpsActiveRunner::RunProtectedSetup_Stub()
{
    QList<TWlanProtectedSetupCredentialAttribute> credentials;
    TWlanProtectedSetupCredentialAttribute attr1 = { EWlanOperatingModeInfrastructure, // iOperatingMode
        EWlanAuthenticationModeOpen, // iAuthenticationMode
        EWlanIapSecurityModeWpa, // iSecurityMode,
        _L8( "STUB01" ), // iSsid
        _L8( "" ), // iWepKey1
        _L8( "" ), // iWepKey2
        _L8( "" ), // iWepKey3
        _L8( "" ), // iWepKey4
        EWlanDefaultWepKey1, // iWepDefaultKey
        _L8( "testtest" )
    // iWpaPreSharedKey
        };

    TWlanProtectedSetupCredentialAttribute attr2 = { EWlanOperatingModeAdhoc,
        EWlanAuthenticationModeOpen, EWlanIapSecurityModeWep, _L8( "STUB02" ), _L8( "12345" ),
        _L8( "" ), _L8( "" ), _L8( "" ), EWlanDefaultWepKey1, _L8( "" ) };

    credentials.append(attr1);
    credentials.append(attr2);

    iObserver.WpsActiveRunnerStopped(credentials, KErrNone);
}
#endif
