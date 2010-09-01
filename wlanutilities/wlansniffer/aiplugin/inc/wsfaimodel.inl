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
* Description:  Inline for CWsfAiModel
*
*/


//  INLINE FUNCTIONS

// --------------------------------------------------------------------------
// CWsfAiModel::SetScanningOn
// --------------------------------------------------------------------------
//
inline void CWsfAiModel::SetScanningOn()
    {
    // Set the scanning status flag on
    iScanning = ETrue;
    }
      
// --------------------------------------------------------------------------
// CWsfAiModel::SetScanningOff
// --------------------------------------------------------------------------
//
inline void CWsfAiModel::SetScanningOff() 
    {
    // Set the scanning status flag off
    iScanning = EFalse;
    }
           
// --------------------------------------------------------------------------
// CWsfAiModel::ScanningOn
// --------------------------------------------------------------------------
//
inline TBool CWsfAiModel::ScanningOn()
    {
    return iScanning;
    }
       
// --------------------------------------------------------------------------
// CWsfAiModel::Connected
// --------------------------------------------------------------------------
//
inline TBool CWsfAiModel::Connected() 
    {
    return iConnected;
    }

// -----------------------------------------------------------------------------
// CWsfAiModel::SetConnected
// -----------------------------------------------------------------------------
//
void CWsfAiModel::SetConnected( TBool aConnected )
	{
	iConnected = aConnected;
	}

// -----------------------------------------------------------------------------
// CWsfAiModel::SetConnected
// -----------------------------------------------------------------------------
//
void CWsfAiModel::SetConnecting( TBool aConnecting )
    {
    iConnecting = aConnecting;
    }

// -----------------------------------------------------------------------------
// CWsfAiModel::SetConnectedWlanInfo
// -----------------------------------------------------------------------------
//
void CWsfAiModel::SetConnectedWlanInfo( TWsfWlanInfo& aWlanInfo )
	{
	iConnectedWlanInfo = aWlanInfo;
	}


// End of file
