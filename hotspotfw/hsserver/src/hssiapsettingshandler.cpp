/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Implementation of iapsettings
*
*/



// INCLUDE FILES
#include "hssiapsettingshandler.h"
#include "hotspotclientserver.h"
#include "am_debug.h"
#include <es_enum.h>

#include <cmconnectionmethodext.h>
#include <cmconnectionmethoddef.h>
#include <cmdestinationext.h>
#include <cmmanagerext.h>

using namespace CMManager;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHssIapSettingsHandler
// -----------------------------------------------------------------------------
//
CHssIapSettingsHandler::CHssIapSettingsHandler( )
	{
    DEBUG("CHssIapSettingsHandler::CHssIapSettingsHandler");
	}

// -----------------------------------------------------------------------------
// ConstructL
// -----------------------------------------------------------------------------
//
void CHssIapSettingsHandler::ConstructL()
    {
    DEBUG("CHssIapSettingsHandler::ConstructL()");
    }

// -----------------------------------------------------------------------------
// NewL
// -----------------------------------------------------------------------------
//
CHssIapSettingsHandler* CHssIapSettingsHandler::NewL()
    {
    CHssIapSettingsHandler* self = new( ELeave ) CHssIapSettingsHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// ~CHssIapSettingsHandler
// -----------------------------------------------------------------------------
//
CHssIapSettingsHandler::~CHssIapSettingsHandler()
	{
	DEBUG("CHssIapSettingsHandler::~CHssIapSettingsHandler()");
    }

// -----------------------------------------------------------------------------
// CreateClientIapL
// -----------------------------------------------------------------------------
//    
void CHssIapSettingsHandler::CreateClientIapL( const TDesC& aIapName, 
                                         TUint32& aIapId, 
                                         const TUid aUid )
    {
    DEBUG("CHssIapSettingsHandler::CreateClientIapL");

    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );
    
    // Read all destination(SNAP) settings into an array
    RArray<TUint32> destinations;
    CleanupClosePushL( destinations );
    
    cmManager.AllDestinationsL( destinations );
    RCmDestinationExt destination;
    // Loop through each destination
    for( TInt i = 0; i < destinations.Count(); i++ )
        {
        destination = cmManager.DestinationL(destinations[i]);
        CleanupClosePushL(destination); 
        // Internet destination will always exist in the system.
        // Internet destination will have ESnapPurposeInternet set in its metadata.
        if (destination.MetadataL(CMManager::ESnapMetadataPurpose) == CMManager::ESnapPurposeInternet)
            {
            RCmConnectionMethodExt plugin = cmManager.CreateConnectionMethodL( KUidWlanBearerType );
            CleanupClosePushL( plugin );
                
            plugin.SetStringAttributeL( ECmName, aIapName );
            plugin.SetStringAttributeL( EWlanSSID, aIapName );
            plugin.SetStringAttributeL( EWlanServiceExtensionTableName, aUid.Name() );
            plugin.SetStringAttributeL( ECmConfigDaemonManagerName, KHotSpotPlugin );
            plugin.SetBoolAttributeL( ECmProtected, ETrue );
            
            destination.AddConnectionMethodL( plugin );
            destination.UpdateL();
                
            aIapId = plugin.GetIntAttributeL( ECmIapId  );
            CleanupStack::PopAndDestroy( &plugin );
            }
        CleanupStack::PopAndDestroy(); // destination
        }
    CleanupStack::PopAndDestroy(); // destinations
    CleanupStack::PopAndDestroy( &cmManager );

    DEBUG("CHssIapSettingsHandler::CreateClientIapL Done");
    }

// -----------------------------------------------------------------------------
// CreateIapL
// -----------------------------------------------------------------------------
//    
void CHssIapSettingsHandler::CreateIapL()
    {
    DEBUG("CHssIapSettingsHandler::CreateIapL");

    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );
    
    // Read all destination(SNAP) settings into an array
    RArray<TUint32> destinations;
    CleanupClosePushL( destinations );
    
    cmManager.AllDestinationsL( destinations );
    RCmDestinationExt destination;
    // Loop through each destination
    for( TInt i = 0; i < destinations.Count(); i++ )
        {
        destination = cmManager.DestinationL( destinations[i] );
        CleanupClosePushL( destination ); 
        // Internet destination will always exist in the system.
        // Internet destination will have ESnapPurposeInternet set in its metadata.
        if (destination.MetadataL( CMManager::ESnapMetadataPurpose ) == 
		                                CMManager::ESnapPurposeInternet )
            {
            CWlanMgmtClient* wlanMgmtClient = CWlanMgmtClient::NewL();
            CleanupStack::PushL( wlanMgmtClient );
            
            TWlanConnectionMode connMode( EWlanConnectionModeNotConnected );
            wlanMgmtClient->GetConnectionMode( connMode );

            TWlanConnectionSecurityMode secMode( EWlanConnectionSecurityOpen );
            wlanMgmtClient->GetConnectionSecurityMode( secMode );
                        
            HBufC* ssid( NULL );
            TWlanSsid ssidConn;
            
            wlanMgmtClient->GetConnectionSsid( ssidConn );
            
            ssid = HBufC::NewLC( ssidConn.Length() );
            ssid->Des().Copy( ssidConn ); 
            
            TUint32 serviceId(0);
            TUint32 easyWlanIapId(0);
            easyWlanIapId = cmManager.EasyWlanIdL();
            DEBUG1("CHssIapSettingsHandler::SaveDestinationL easyWlanIapId: %d", 
                    easyWlanIapId);            
            
            RCmConnectionMethodExt easyWlanPlugin;
            easyWlanPlugin = cmManager.ConnectionMethodL( easyWlanIapId );
            CleanupClosePushL( easyWlanPlugin );
            TBool scanSsid = easyWlanPlugin.GetBoolAttributeL( EWlanScanSSID );
            DEBUG1("CHssIapSettingsHandler::SaveDestinationL scanSsid: %d", scanSsid);
            TUint32 easyWlanServiceId = easyWlanPlugin.GetIntAttributeL( EWlanServiceId );
            DEBUG1("CHssIapSettingsHandler::SaveDestinationL easyWlanServiceId: %d", easyWlanServiceId);
            CleanupStack::PopAndDestroy(); // easyWlanPlugin;
            
            RCmConnectionMethodExt plugin =
                cmManager.CreateConnectionMethodL( KUidWlanBearerType );
            CleanupClosePushL( plugin );        
            plugin.SetStringAttributeL( ECmName, *ssid );
            plugin.SetStringAttributeL( EWlanSSID, *ssid );
            plugin.SetIntAttributeL( CMManager::EWlanSecurityMode,
                    ConvertConnectionSecurityModeToSecModeL( secMode ));
            plugin.SetIntAttributeL( EWlanConnectionMode, 
                    ConvertConnectionModeToNetModeL( connMode ) );
            plugin.SetBoolAttributeL( EWlanScanSSID, scanSsid );
            destination.AddConnectionMethodL( plugin );
            destination.UpdateL();
            serviceId = plugin.GetIntAttributeL( EWlanServiceId );                
           
            CleanupStack::PopAndDestroy( &plugin ); // Close() called
            if ( secMode == EWlanConnectionSecurityWep )
                {
                SaveWEPKeyL( easyWlanServiceId, serviceId );
                }
                    
            if ( secMode == EWlanConnectionSecurityWpaPsk )
                {
                SaveWPAKeyL( easyWlanServiceId, serviceId );
                }
            CleanupStack::PopAndDestroy( ssid ); // ssid
            CleanupStack::PopAndDestroy( wlanMgmtClient );
            }
        CleanupStack::PopAndDestroy(); // destination
        }
    CleanupStack::PopAndDestroy(); // destinations
    CleanupStack::PopAndDestroy( &cmManager );

    DEBUG("CHssIapSettingsHandler::CreateIapL Done");
    }

// -----------------------------------------------------------------------------
// DeleteIapL
// -----------------------------------------------------------------------------
//
void CHssIapSettingsHandler::DeleteIapL( const TUint aIapId )
    {
    DEBUG1("CHssIapSettingsHandler:::DeleteIapL aIapId=%d ", aIapId);

    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );

    RCmConnectionMethodExt plugin = cmManager.ConnectionMethodL( aIapId );
    CleanupClosePushL( plugin );

    cmManager.RemoveAllReferencesL( plugin );
    CleanupStack::Pop( &plugin );

    TBool result = plugin.DeleteL();
    DEBUG1("CHssIapSettingsHandler:::DeleteIapL result = %d ", result);

    CleanupStack::PopAndDestroy( &cmManager );
    }
   
// -----------------------------------------------------------------------------
// FindClientL
// -----------------------------------------------------------------------------
//
TInt CHssIapSettingsHandler::FindClientL( const TUint aIapId, TUid& aUid, TDes8& aUidText )
    {   
    DEBUG("CHssIapSettingsHandler::FindClientL");
    TInt ret( KErrNone );
    aUid = TUid::Null();
    RCmManagerExt cmManager;
    cmManager.OpenL();
    CleanupClosePushL( cmManager );
    
	RCmConnectionMethodExt plugin = cmManager.ConnectionMethodL( aIapId );
    CleanupClosePushL( plugin );
    
    HBufC* uid = plugin.GetStringAttributeL( EWlanServiceExtensionTableName );
    
    TBuf8<KExtensionAPILength> buffer;
    buffer.Copy( *uid );
    delete uid;
    // Find and remove [ and ]. 
    // If found [ it's known that buffer contains UID
    TInt indx = buffer.Find( KMark1 );
    	 if ( KErrNotFound != indx )
         {
         DEBUG("CHssIapSettingsHandler::FindClientL Client is found");
         buffer.Delete( indx, 1 );
         indx = buffer.Find( KMark2 );
         if ( KErrNotFound != indx )
             {
             buffer.Delete( indx, 1 );
             }
         // Convert TBuf to TUid
         TLex8 lex( buffer );
         TUint value( 0 );
         User::LeaveIfError( lex.Val( value, EHex ) );
         aUid.iUid = value;
         aUidText = buffer;
         }
    
    CleanupStack::PopAndDestroy( &plugin ); // Close() called on "plugin"
    CleanupStack::PopAndDestroy( &cmManager );
    DEBUG("CHssIapSettingsHandler::FindClientL");

    return ret;
    }

// ---------------------------------------------------------
// SaveWEPKeyL()
// ---------------------------------------------------------
//
void CHssIapSettingsHandler::SaveWEPKeyL( const TUint32 aFrom, 
        const TUint32 aTo )
    {
    DEBUG("CHssIapSettingsHandler::SaveWEPKey");
    TInt err( KErrNone );
    TUint attempts( KHssMaxTransAttempts );

    CMDBSession* dbSession = 
    CMDBSession::NewLC( CMDBSession::LatestVersion() );
    
    // open a transaction to make sure key is saved
    do
        {
        TRAP( err, dbSession->OpenTransactionL() );
        if ( err )
            {
            DEBUG1("CHssIapSettingsHandler::SaveWEPKey err: %d", err);
            User::After( KHssRetryAfter );
            }
        }
    while ( err && attempts-- );
    
    if ( err != KErrNone )
        {
        dbSession->Close();
        CleanupStack::PopAndDestroy( dbSession );
        User::LeaveIfError( err );
        }
    
    CleanupStack::PushL( TCleanupItem( RollbackCommsDat, dbSession ) );
    
    CMDBGenericRecord* wlanRecord = static_cast<CMDBGenericRecord*>
            ( CCDRecordBase::RecordFactoryL( 0));
    CleanupStack::PushL( wlanRecord );
    wlanRecord->InitializeL( KGenericTable(),NULL );
    wlanRecord->SetRecordId( aFrom );
    wlanRecord->LoadL( *dbSession );
        
    TInt format = 
        *((CMDBField<TUint32>*)wlanRecord->GetFieldByIdL( KCDTIdWlanFormatKey1 ));
    CMDBField<TDesC8>* wepKey = 
            static_cast<CMDBField<TDesC8>*>(wlanRecord->GetFieldByIdL(KCDTIdWlanWepKey1));
        
    TBuf8<KWLMMaxWEPKeyLength> buf8;
    buf8.Copy( *wepKey );
    CleanupStack::PopAndDestroy( wlanRecord );
        
    CMDBGenericRecord* wlanRecordNew = static_cast<CMDBGenericRecord*>
        ( CCDRecordBase::RecordFactoryL( 0));
    CleanupStack::PushL( wlanRecordNew );
    wlanRecordNew->InitializeL( KGenericTable(),NULL );
    wlanRecordNew->SetRecordId( aTo );
    wlanRecordNew->LoadL( *dbSession );
        
    CMDBField<TDesC8>* wepKeyNew = 
        static_cast<CMDBField<TDesC8>*>(wlanRecordNew->GetFieldByIdL(KCDTIdWlanWepKey1));
    *((CMDBField<TUint32>*)wlanRecordNew->GetFieldByIdL( KCDTIdWlanFormatKey1 )) = format;
    
    wepKeyNew->SetL(buf8); 
    
    wlanRecordNew->ModifyL( *dbSession );        
    CleanupStack::PopAndDestroy( wlanRecordNew );
    
    dbSession->CommitTransactionL();
    
    dbSession->Close();
    CleanupStack::PopAndDestroy( dbSession );
    
    CleanupStack::Pop( 1 ); // transaction rollback popped
        
    dbSession->CommitTransactionL();
   
    
    DEBUG("CHssIapSettingsHandler::SaveWEPKey Done");
    }

// ---------------------------------------------------------
// SaveWPAKeyL()
// ---------------------------------------------------------
//
void CHssIapSettingsHandler::SaveWPAKeyL( const TUint32 aFrom, 
        const TUint32 aTo )
    {
    DEBUG("CHssIapSettingsHandler::SaveWPAKey");
    TInt err( KErrNone );
    TUint attempts( KHssMaxTransAttempts );
    
    CMDBSession* dbSession = 
    CMDBSession::NewLC( CMDBSession::LatestVersion() );
    
    // open a transaction to make sure key is saved
    do
        {
        TRAP( err, dbSession->OpenTransactionL() );
        if ( err )
            {
            DEBUG1("CHssIapSettingsHandler::SaveWPAKey err: %d", err);
            User::After( KHssRetryAfter );
            }
        }
    while ( err && attempts-- );
    
    if ( err != KErrNone )
        {
        dbSession->Close();
        CleanupStack::PopAndDestroy( dbSession );
        User::LeaveIfError( err );
        }
    
    CleanupStack::PushL( TCleanupItem( RollbackCommsDat, dbSession ) );
   
    CMDBGenericRecord* wlanRecord = static_cast<CMDBGenericRecord*>
           ( CCDRecordBase::RecordFactoryL( 0));
    CleanupStack::PushL( wlanRecord );
    wlanRecord->InitializeL( KGenericTable(), NULL );
    wlanRecord->SetRecordId( aFrom );
    wlanRecord->LoadL( *dbSession );
       
    CMDBField<TDesC8>* binField = 
            (CMDBField<TDesC8>*)wlanRecord->GetFieldByIdL(KCDTIdWlanWpaPreSharedKey);
    TBuf8<KWLMMaxWpaPskLength> buf8;
    buf8.Copy( *binField );
    CleanupStack::PopAndDestroy( wlanRecord );
       
    CMDBGenericRecord* wlanRecordNew = static_cast<CMDBGenericRecord*>
           ( CCDRecordBase::RecordFactoryL( 0));
    CleanupStack::PushL( wlanRecordNew );
    wlanRecordNew->InitializeL( KGenericTable(),NULL );
    wlanRecordNew->SetRecordId( aTo );
    wlanRecordNew->LoadL( *dbSession );
       
    CMDBField<TDesC8>* binFieldNew = 
           (CMDBField<TDesC8>*)wlanRecordNew->GetFieldByIdL(KCDTIdWlanWpaPreSharedKey);
    binFieldNew->SetMaxLengthL( KWLMMaxWpaPskLength );
    binFieldNew->SetL( buf8 );
    *((CMDBField<TUint32>*)wlanRecordNew->GetFieldByIdL(KCDTIdWlanEnableWpaPsk)) = ETrue;
    wlanRecordNew->ModifyL( *dbSession );
    CleanupStack::PopAndDestroy( wlanRecordNew );
       
    CleanupStack::Pop( 1 ); // transaction rollback popped
          
    dbSession->CommitTransactionL();
       
    dbSession->Close();
    CleanupStack::PopAndDestroy( dbSession );
    DEBUG("CHssIapSettingsHandler::SaveWPAKey Done");
}

// ----------------------------------------------------------------------------
// RollbackCommsDat
// ----------------------------------------------------------------------------
void CHssIapSettingsHandler::RollbackCommsDat( TAny* aDbSession )
    {
    DEBUG("CHssIapSettingsHandler::RollbackCommsDat");
    CommsDat::CMDBSession* session = static_cast<CommsDat::CMDBSession*>( aDbSession );
    TRAP_IGNORE( session->RollbackTransactionL() );
    }

// ---------------------------------------------------------
// ConvertConnectionSecurityModeToSecModeL
// ---------------------------------------------------------
//
TWlanSecMode CHssIapSettingsHandler::ConvertConnectionSecurityModeToSecModeL(
                                     TWlanConnectionSecurityMode aSecurityMode )
     {
     DEBUG( "CHssIapSettingsHandler::ConvertConnectionSecurityModeToSecModeL" );
     switch( aSecurityMode )
         {
         case EWlanConnectionSecurityOpen:
             {
             return EWlanSecModeOpen;
             }
         
         case EWlanConnectionSecurityWep:
             {
             return EWlanSecModeWep;
             }
         
         case EWlanConnectionSecurity802d1x:
             {
             return EWlanSecMode802_1x;
             }
         
         case EWlanConnectionSecurityWpa:
         case EWlanConnectionSecurityWpaPsk:
             {
             return EWlanSecModeWpa;
             }
         
         default:
             {
             User::Leave( KErrNotSupported );
             }
         }
 
     return EWlanSecModeOpen;
     } 

// ---------------------------------------------------------
// ConvertConnectionModeToNetModeL
// ---------------------------------------------------------
//
TWlanNetMode CHssIapSettingsHandler::ConvertConnectionModeToNetModeL(
        TWlanConnectionMode aConnMode )
    {
    DEBUG( "CHssIapSettingsHandler::ConvertConnectionModeToNetModeL" );
    if ( EWlanConnectionModeAdhoc == aConnMode )
        {
        return EAdhoc;
        }
    else
        {
        return EInfra;
        }
    }

// End of File
