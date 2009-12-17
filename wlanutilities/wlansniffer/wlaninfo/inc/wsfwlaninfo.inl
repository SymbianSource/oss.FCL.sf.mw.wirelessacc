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
* Description:  Inline for TWsfWlanInfo
*
*/



//  INLINE FUNCTIONS

// -----------------------------------------------------------------------------
// TWsfWlanInfo::Connected
// -----------------------------------------------------------------------------
//
inline TBool TWsfWlanInfo::Connected() const
	{ 
	return ( iConnectionState == EConnected ); 
	}


// -----------------------------------------------------------------------------
// TWsfWlanInfo::ConnectionStatus
// -----------------------------------------------------------------------------
//
inline TWsfWlanConnectionState TWsfWlanInfo::ConnectionStatus() const
    { 
    return iConnectionState; 
    }


// -----------------------------------------------------------------------------
// TWsfWlanInfo::Known
// -----------------------------------------------------------------------------
//
inline TBool TWsfWlanInfo::Known() const
	{ 
	return ( iIapId );
	}


// -----------------------------------------------------------------------------
// TWsfWlanInfo::Hidden
// -----------------------------------------------------------------------------
//
inline TBool TWsfWlanInfo::Hidden() const
	{ 
	return !iVisibility;
	}


// -----------------------------------------------------------------------------
// TWsfWlanInfo::Secure
// -----------------------------------------------------------------------------
//
inline TBool TWsfWlanInfo::Secure() const
	{ 
	return ( iSecurityMode != CMManager::EWlanSecModeOpen );
	}


// -----------------------------------------------------------------------------
// TWsfWlanInfo::SetBrand
// -----------------------------------------------------------------------------
//
inline void TWsfWlanInfo::SetBrand( const TInt8 aBrandId ) 
	{
	iBrandId = aBrandId;
	}
	

// -----------------------------------------------------------------------------
// TWsfWlanInfo::BrandId
// -----------------------------------------------------------------------------
//
inline TInt8 TWsfWlanInfo::BrandId() const
	{
	return iBrandId; 
	}		
	

// -----------------------------------------------------------------------------
// TWsfWlanInfo::SetBlackListEntry
// -----------------------------------------------------------------------------
//
inline void TWsfWlanInfo::SetBlackListEntry( const TBool aBlackListEntry )
	{
	iFilterFlags = aBlackListEntry ? 
                        ( EBlackListItem | iFilterFlags ): 
                        ( ~EBlackListItem & iFilterFlags );
	}
	

// -----------------------------------------------------------------------------
// TWsfWlanInfo::BlackListEntry
// -----------------------------------------------------------------------------
//
inline TBool TWsfWlanInfo::BlackListEntry() const
	{
	return ( iFilterFlags & EBlackListItem );
	}	


// -----------------------------------------------------------------------------
// TWsfWlanInfo::FonSsid
// -----------------------------------------------------------------------------
//
inline TBool TWsfWlanInfo::FonSsid() const
	{
	return ( iFilterFlags & EFonSsid );
	}
	

// -----------------------------------------------------------------------------
// TWsfWlanInfo::SetFonSsid
// -----------------------------------------------------------------------------
//
inline void TWsfWlanInfo::SetFonSsid( const TBool aFonSsid )
	{			
	iFilterFlags = aFonSsid ? 
	                    ( EFonSsid | iFilterFlags ): 
	                    ( ~EFonSsid & iFilterFlags );			
	}


// -----------------------------------------------------------------------------
// TWsfWlanInfo::UsesPreSharedKey
// -----------------------------------------------------------------------------
//
inline TBool TWsfWlanInfo::UsesPreSharedKey() const
	{
	return ( iFilterFlags & EUsesPresharedKey );
	}
	

// -----------------------------------------------------------------------------
// TWsfWlanInfo::SetUsesPreSharedKey
// -----------------------------------------------------------------------------
//
inline void TWsfWlanInfo::SetUsesPreSharedKey( const TBool aUsePresharedKey )
	{			
	iFilterFlags = aUsePresharedKey? 
	                    ( EUsesPresharedKey | iFilterFlags ): 
	                    ( ~EUsesPresharedKey & iFilterFlags );
	}


// -----------------------------------------------------------------------------
// TWsfWlanInfo::SetPriority
// -----------------------------------------------------------------------------
//
inline void TWsfWlanInfo::SetPriority( const TUint8 aPriority ) 
    {
    iPriority = aPriority;
    }
    

// -----------------------------------------------------------------------------
// TWsfWlanInfo::Priority
// -----------------------------------------------------------------------------
//
inline TUint8 TWsfWlanInfo::Priority() const
    {
    return iPriority; 
    }

// -----------------------------------------------------------------------------
// TWsfWlanInfo::SecurityMode
// -----------------------------------------------------------------------------
//
inline CMManager::TWlanSecMode TWsfWlanInfo::SecurityMode() const
    { 
    return iSecurityMode;
    }
