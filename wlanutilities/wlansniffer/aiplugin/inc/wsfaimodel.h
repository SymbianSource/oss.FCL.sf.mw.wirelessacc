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
* Description:  Class header for CWsfAiModel
*
*/


#ifndef C_WSFAIMODEL_H
#define C_WSFAIMODEL_H

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "wsfwlaninfo.h"

//  FORWARD DECLARATIONS
class CWsfWlanInfoArray;
class CWsfWlanInfoArrayVisitor;
class CCoeEnv;
class CAknIconArray;
class CGulIcon;

//class TWsfWlanInfo;

/**
 * The model part. It holds the list of available WLANs and creates the 
 * suitable texts and icons to display.
 *
 * @lib wsfaiplugin.lib
 * @since S60 v5.0
 */
class CWsfAiModel : public CBase
    {
public:     // Constructors and destructor

    /**
    * Two-phased constructor
    * @since S60 5.0 
    * @return instance of CWsfAiModel class
    */
    static CWsfAiModel* NewL();
    
    /**
    * Two-phased constructor. Leaves the object on CleanupStack
    * @since S60 5.0 
    * @return instance of CWsfAiModel class
    */
    static CWsfAiModel* NewLC();

    /**
    * Destructor of CWsfAiModel class
    * @since S60 5.0     
    */
    ~CWsfAiModel();

private:    // Constructors
    
    /**
    * Default C++ constructor 
    * @since S60 5.0 
    */
    CWsfAiModel();
    
    /**
    * 2nd phase constructor
    * @since S60 5.0 
    */
    void ConstructL();

public:     // New methods

    /**
    * Creates array for the listbox, it contains a formatted array of WLANs.
    * @param aWlanList - Array of WLAN that will be formatted.
    * @return Formatted WLAN list (Ownership not passed)
    */
    CDesCArrayFlat* FormatWlanListL( CWsfWlanInfoArray* aWlanList, const TBool aUIPrioritySort );
      
    /*
    * Returns WLAN list that has already formatted
    * @return Formatted WLAN list (Ownership not passed) 
    */        
    CDesCArrayFlat* GetWlanList();        
        
    /**
    * Returns a pointerarray for TWsfWlanInfo items.
    * @return non-formatted WLAN list (Ownership not passed)
    */
    CWsfWlanInfoArray* GetInfoArray();
        
    /**
    * Format the WlanList for one-line
    * @param aWlanList - Array of WLAN that will be formatted.
    * @return Formatted WLAN list (Ownership not passed)
    */
    CDesCArrayFlat* FormatSingleLineWlanListL( 
                                    CWsfWlanInfoArray* aWlanList );
    
    /**
    * Load and format the refreshing information for the WLAN list
    * @return Formatted WLAN list (Ownership not passed) 
    */
    CDesCArrayFlat* FormatRefreshingL();
    
    /**
    * Load and format the connecting information for the WLAN list
    * @return Formatted WLAN list (Ownership not passed) 
    */
    CDesCArrayFlat* FormatConnectingL();
        
    /**
    * Appends icons of customer favourite networks to the given array
    * @param aIconArray The icon array to which brand icons 
    *                   should be appended
    */
    void AppendBrandIconsL( CAknIconArray& aIconArray );
    
    /**
    * Set the scanning status flag on
    */
    inline void SetScanningOn();
        
    /**
    * Set the scanning status flag off
    */
    inline void SetScanningOff();
    
    /**
    * Returns with the scanning status flag
    * @since S60 5.0
    * @return ETrue if the scanning on, if it is off EFalse
    */    
    inline TBool ScanningOn();
    
    /**
    * Returns that the WLAN item connected or not.
    * @return ETrue if it is connected otherwise EFalse
    */    
    inline TBool Connected();
    
    /**
    * Sets the connected flag
    * @param - aConnected True if connected
    */
    inline void SetConnected( TBool aConnected );
    
    /**
    * Sets the connected flag
    * @param - aConnected True if connected
    */
    inline void SetConnecting( TBool aConnecting );
    
    /**
    * Sets the connected wlan info
    * @param aConnected  - connected wlan info
    */
    inline void SetConnectedWlanInfo( TWsfWlanInfo& aWlanInfo );

    /**
     *  Initialize the refresh animation
     */
    void InitializeRefreshAnimation();
    
    /*
     * Step the animation
     */
    void AnimateRefresh();
    
    /**
     *  Initialize the refresh animation
     */
    void InitializeConnectingAnimation();
    
    /*
     * Step the animation
     */
    void AnimateConnecting();
    
private:    // New methods        
        
    /**
    * Format the Hidden WLANs for the listbox.(text and icon)
    * @since S60 v5.0
    * @param aWlan - Reference to wlaninfo
    * @param aItem - Listbox item
    */    
    void FormatHiddenWlanItemL( TWsfWlanInfo& aWlan, TPtr& aItem );

    /**
     * Format the listbox filed with the scanning status off information
     * @since S60 v5.0
     * @param aItem - Listbox item
     */  
    void FormatStatusOffL( TPtr& aItem );
    
    /**
     * Format the listbox filed with the No Wlan found information
     * @since S60 v5.0
     * @param aItem - Listbox item
     */  
    void FormatNoWlansAvailableL( TPtr& aItem );
        
    /**
    * Format the WlanList item for one-line or (visible networks) items for
    * the listbox.(text and icon)
    * @param aWlan - reference to wlaninfo
    * @param aItem - Listbox item
    * @param aSingleLine - ETrue if Single Line Item wanted to be shown 
    *                      EFalse if Visible Wlan Item wanted to be shown
    */
    void FormatWlanSingleLineL( TWsfWlanInfo& aWlan, 
                                TPtr& aItem,
                                TBool aSingleLine );
        
    /**
    * Format the WlanList items in one-line for Unknown networks
    * @param aMultipleUnknownWlans - ETreue if more unknown network found 
    * @param aItem - Listbox item
    */
    void FormatUnknownWlansSingleLineL( const TBool aMultipleUnknownWlans, 
                                        TPtr& aItem );

    /**
    * Go through the WLANs list and find Max signal strength amont know wlans
    * @param aWlanList - array of WLANs
    * @return the ID of the Wlan with the Max signal strength 
    */
    TInt FindPreferredKnownWlan( CWsfWlanInfoArray* aWlanList  );
        
    /**
    * Check if there's connection and it is the first item in array
    * @param aWlanList - array of WLANs
    * @param aItem - Listbox item
    */
    void CheckWlansL( CWsfWlanInfoArray* aWlanList, TPtr& aItem );
    
    /**
    * Formats the connected string.
    * @param aItem - The name of the connected network to be put
    */
    void FormatConnectedL( TPtr& aItem );
        
private:    // Data
       
    /**
    * Array of WLANs
    * Not Own.
    */
    CWsfWlanInfoArray* iWlanInfoArray;

    /**
    * Coe env for loading string from resource
    * Not Own.
    */
    CCoeEnv* iCoeEnv;
        
    /**
    * List that contains the WLAN items 
    * Own.
    */
    CDesCArrayFlat* iFormattedWlanList;  

    /**
     * WlanInfoArrayVisitor
     * Own.
     */
    CWsfWlanInfoArrayVisitor *iWlanInfoBranding; 

    /**
     * The scanning status flag
     */
    TBool iScanning;

    /**
     * Connected status flag
     */
    TBool iConnected;
    
    /**
     * Connected status flag
     */
    TBool iConnecting;
    
    /**
     * Info of the connected WLAN
     */
    TWsfWlanInfo iConnectedWlanInfo;

    /**
     * List of animation states of refreshing icon
     */
    TFixedArray<TInt, 4> iRefreshIcons;
    
    /**
     * List of animation states of connecting icon
     */
    TFixedArray<TInt, 4> iConnectingIcons;
    
    /**
     * The refreshing animation frame index
     */
    TInt iRefreshIndex;
    
    /**
     * The connecting animation frame index
     */
    TInt iConnectingIndex;
    
    /**
    * Constants for 'WLAN scanning off' text
    * Own.
    */
    HBufC* iStatusScanningOff;
    
    /**
    * Constants for 'Refreshing' text
    * Own.
    */
    HBufC* iStatusRefreshing;

    /**
    * Constants for 'Connecting' text
    * Own.
    */
    HBufC* iStatusConnecting;
    
    /**
    * Constants for 'Other (hidden netw.)' text
    * Own.
    */
    HBufC* iManualSSIDInputting;
    
    /**
    * Constants for 'No WLAN network found' text
    * Own.
    */
    HBufC* iNoWLANsAvailable;
    
    };


#include "wsfaimodel.inl"


#endif // C_WSFAIMODEL_H

// End of file
