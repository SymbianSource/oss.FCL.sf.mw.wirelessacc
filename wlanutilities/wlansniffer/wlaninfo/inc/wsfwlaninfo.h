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
* Description:  Class header for TWsfWlanInfo
*
*/



#ifndef T_WSFWLANINFO_H
#define T_WSFWLANINFO_H

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <cmpluginwlandef.h>
#include <wlanmgmtcommon.h>


// ENUMS

/**
 * Signal strength categories for UI
 */
enum TWsfWlanSignalStrengthLevel
    {
    ENoSignal,
    EPoor,
    EAverage,
    EExcelent
    };


/**
 * Connection status
 */
enum TWsfWlanConnectionState
    {
    ENotConnected,
    EConnecting,
    EConnected
    };
    

/**
 * Signal strength category boundaries in dB (absolute value).
 * Smaller value means better signal.
 */    
enum TWsfWlanSignalStrength
    {
    EWlanSignalStrengthMax = 50,
    EWlanSignalStrengthGood = 65,
    EWlanSignalStrengthLow = 81,
    EWlanSignalStrengthMin = 95,
    EWlanSignalUnavailable = 9999
    };


/**
 * Flags for different types of networks
 */
enum TWsfBrandDataDefs
    {
    EBrandedItem = 0xFF,
    EBlackListItem = 0x01,
    EUserSortListItem = 0x02,
    EFonSsid = 0x04,
    EUsesPresharedKey = 0x08
    };
    

/**
 * Maximal length of IAP name that the class should be able to store
 */    
const TInt KWlanMaxAccessPointNameLength = 50;


// FORWARD DECLARATIONS
class RReadStream;
class RWriteStream;


//  CLASS DEFINITION
/**
 * Class to encapsulate network parameters
 *
 * TWsfWlanInfo serves for transferring the WLAN network related data
 * between the different submodules of Wlan Sniffer.
 *
 * @lib wsfwlaninfo.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( TWsfWlanInfo )
    {
    public:     // Constructors

        IMPORT_C TWsfWlanInfo();

        /**
         * Constructor.
         *
         * @since S60 5.0
         * @param aSsid SSID of the WLAN network
         * @param aStrengthLevel Signal strength in dB (absolute value)
         * @param aSecurityMode Network security mode
         * @param aVisibility ETrue for public networks, EFalse for hidden ones
         * @param aNetMode Network mode (infrastructure or adhoc)
         * @param aIapId IAP id if known, zero otherwise.
         * @param aCoverage Number of APs with the same SSID
         * @param aTransferRate Maximal transfer rate reported by the AP
         */
        IMPORT_C TWsfWlanInfo( TDesC8& aSsid,
                        TUint16 aStrengthLevel,
                        CMManager::TWlanSecMode aSecurityMode,
                        TInt aVisibility,
                        CMManager::TWlanNetMode aNetMode,
                        TUint32 aIapId,
                        TUint8 aCoverage,
                        TUint32 aTransferRate );

    public:     // New methods

        /**
        * Gets WLAN name (Ssid) as unicode format 
        * @since S60 5.0
        * @return Ssid as unicode format
        */
        IMPORT_C HBufC* GetSsidAsUnicodeLC();

        /**
        * Gets WLAN name (Ssid) as unicode format
        * @since S60 5.0
        * @return Ssid as unicode format
        */ 
        IMPORT_C static HBufC* GetSsidAsUnicodeLC( const TWlanSsid& aSsid );

        /**
        * Gets WLAN name (Ssid) as 8-bit format 
        * @since S60 5.0
        * @return Ssid as 8-bit format
        */
        IMPORT_C HBufC8* GetSsidAsUtf8LC();

        /**
        * Serializes data from the stream
        * @since S60 5.0
        * @param aStream Stream from where data is serialized
        */
        IMPORT_C void InternalizeL( RReadStream& aStream );
        
        /**
        * Serializes data to the stream
        * @since S60 5.0
        * @param aStream Stream where data will be serialized
        */
        IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

        /**
        * Get signal strength as enumeration
        * @since S60 5.0
        * @return Signal stregth as enumeration
        */
        IMPORT_C TWsfWlanSignalStrengthLevel SignalStrength(); 

        /**
        * Get signal strength as percentage
        * @since S60 5.0
        * @return Signal stregth as percentage
        */
        IMPORT_C TInt SignalStrengthPercentage();
        
        /**
        * Gets WLAN IAP name as unicode format 
        * @since S60 5.0
        * @return Ssid as unicode format
        */
        IMPORT_C HBufC* GetIapNameAsUnicodeLC();

        /**
        * Gets WLAN IAP name as 8-bit format 
        * @since S60 5.0
        * @return Ssid as 8-bit format
        */
        IMPORT_C HBufC8* GetIapNameAsUtf8LC();

        /**
        * Get WLAN connection state
        * @since S60 5.0
        * @return ETrue if connected
        */
        inline TBool Connected() const;
        
        /**
        * Get WLAN connection state
        * @since S60 5.2
        * @return TWsfWlanConnectionState
        */
        inline TWsfWlanConnectionState ConnectionStatus() const;

        /**
        * Get WLAN Known state
        * @since S60 5.0
        * @return ETrue if access point has been defined
        */
        inline TBool Known() const;

        /**
        * Get WLAN visibility state
        * @since S60 5.0
        * @return ETrue if WLAN is hidden
        */
        inline TBool Hidden() const;

        /**
        * Get WLAN security state
        * @since S60 5.0
        * @return ETrue if WLAN is secured. 
        */
        inline TBool Secure() const;

        /**
        * Set the brand id for entry
        * @since S60 5.0
        * @param aBrandId The brand id.
        */ 
        inline void SetBrand( const TInt8 aBrandId );

        /**
        * Get brand id
        * @since S60 5.0
        * @return 8-bit value representing brand id
        */
        inline TInt8 BrandId() const;

        /**
        * Set blacklist entry flag
        * @since S60 5.0
        * @param aBlackListEntry ETrue if the network is blacklisted.
        */
        inline void SetBlackListEntry( const TBool aBlackListEntry );

        /** 
        * Get blacklist flag
        * @since S60 5.0
        * @return ETrue if the entry is marked as black list entry
        */
        inline TBool BlackListEntry() const;

        /**
        * Get Fon entry flag
        * @since S60 5.0
        * @return ETrue if the network is recognised as FON.
        */
        inline TBool FonSsid() const;

        /** 
        * Set Fon flag
        * @since S60 5.0
        * @param aFonSsid ETrue if the entry is a Fon list entry
        */
        inline void SetFonSsid( const TBool aFonSsid );

        /**
        * Query preshared key flag in WPA and 802.1x security modes.
        * Note that the result is undefined in case of other security modes.
        * @since S60 5.0
        * @return ETrue, if preshared key is used.
        */
        inline TBool UsesPreSharedKey() const;

        /** 
        * Set preshared key flag for WPA and 802.1x security modes.
        * @since S60 5.0
        * @param aUsePresharedKey ETrue if the network uses preshared key
        */
        inline void SetUsesPreSharedKey( const TBool aUsePresharedKey );
        
        /**
        * Set the priority for entry
        * @since S60 5.1
        * @param aPriority The priority id.
        */ 
        inline void SetPriority( const TUint8 aPriority );

        /**
        * Get priority
        * @since S60 5.1
        * @return 8-bit value representing priority
        */
        inline TUint8 Priority() const;

        /**
        * Get WLAN security mode
        * @since S60 5.2
        * @return WLAN security mode
        */
        inline CMManager::TWlanSecMode SecurityMode() const;


    public:    // Data

        // Network name (unknown: SSID, known: IAP name)
        TWlanSsid iSsid;

        // Network strength
        TUint16 iStrengthLevel;			

        //Network encryption
        CMManager::TWlanSecMode iSecurityMode;		

        //Network visibility
        TBool iVisibility;		

        //Network capability
        CMManager::TWlanNetMode iNetMode;	

        //Internet Access Point Id. 0 if the IAP has not been defined. 
        TUint32 iIapId;

        // Available WLAN hotspots in this WLAN network
        TUint8 iCoverage;

        // Transferrate
        TUint32 iTransferRate;

        // Connection state
        TWsfWlanConnectionState iConnectionState;

        // filter flags
        TUint8 iFilterFlags;

        //brand id
        TUint8 iBrandId;
        
        // The name of the WLAN IAP
        TBuf8<KWlanMaxAccessPointNameLength> iNetworkName;
        
        //UI priority for known networks
        TUint8 iPriority;
        
        //Raw SSID - not converted
        TWlanSsid iRawSsid;
    };


#include "wsfwlaninfo.inl"


#endif // T_WSFWLANINFO_H

// End of file
