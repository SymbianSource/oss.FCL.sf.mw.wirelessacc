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
* Description:  Class header for CWsfConnectedDetailsModel
*
*/


#ifndef C_WSFCONNECTEDDETAILSMODEL_H
#define C_WSFCONNECTEDDETAILSMODEL_H

//EXTERNAL INCLUDES
#include <e32base.h>
#include <rconnmon.h>
#include <badesca.h>

//INTERNAL INCLUDES

//FORWARD DECLARATIONS
class CCoeEnv;
class CWsfWlanInfoArray;
class TWsfWlanInfo;


/**
 *  Details view Model
 *
 *  Holds & formats TWsfWlanInfo for list box showable format
 *
 *  @since S60 v5.0
 */
class CWsfConnectedDetailsModel : public CBase
    {

    public: // Constructors and destructor
    
        /**
        * Two-phase construction.
        */
        static CWsfConnectedDetailsModel* NewL();
    
        /**
        * Two-phased construction.
        */
        static CWsfConnectedDetailsModel* NewLC();
        
        /**
        * Destructor.
        */
        ~CWsfConnectedDetailsModel();
    
    
    public: // New methods
        
        /**
         * Finds out the current WLAN connection id
         * @since S60 5.0
         */
        void FindWlanConnectionL();

        /**
         * Formats Wlan info for showing
         *
         * @since S60 v5.0
         * @param aWlanInfo - selected Wlan info
         * @param aWlanArray - Wlan info array
         * @return formated text array
         */
        CDesCArrayFlat* FormatWlanInfoL( TWsfWlanInfo* aWlanInfo,
                                         CWsfWlanInfoArray* aWlanArray );
        
        /**
         * Refresh current Wlan info for showing
         *
         * @since S60 v5.0
         * @return refreshed text array
         */
        CDesCArrayFlat* RefreshCurrentWlanInfoL();
    
        /**
         * Creates listbox items from header and value (formatter %S\t%S)
         * @since S60 v5.0
         * @param aResourceId - id of loaded header string
         * @param aValueText - string containing value data 
         * @return formated string 
         */        
        HBufC* FormatListBoxItemLC( const TUint aResourceId,
                                    const TDesC& aValueText ) const;
        
        /**
         * Returns WLAN details that has already formatted
         * @since S60 v5.0
         * @return Formatted WLAN details 
         */        
        CDesCArrayFlat* GetWlanDetails();  
    
        /**
         * Returns current Wlan Ssid
         * @since S60 v5.0
         * @return Ssid
         */        
        const TDesC8& WlanSsid() const;
    
    private:
    
        /**
        * C++ default constructor.
        */
        CWsfConnectedDetailsModel();
        
        /*
        * Second-phase constructor.
        */       
        void ConstructL();
        
        // New methods
    
    private:
        /**
         * Appends Wlan Ssid to 
         * iFormattedConnectedWlanInfo
         * @since S60 v5.0
         */            
        void FormatAndAppendConnectedWlanSsidL();
     
        /**
         * Appends duration of connection
         * to iConnectedFormattedWlanInfo
         * @since S60 v5.0
         */            
        void FormatAndAppendDurationL();
        
        /**
         * Appends amount of transferred data 
         * to iConnectedFormattedWlanInfo
         * @since S60 v5.0
         */     
        void FormatAndAppendTransferredL();
    
        /**
         * Formats connection duration
         * to string 
         * @since S60 v5.0
         * @return formated string
         */        
        HBufC* FormatDurationLC();
    
        /**
         * Formats amount of transferred
         * data to string 
         * @since S60 v5.0
         * @return formated string
         */        
        HBufC* FormatTransferredLC();
    
        /**
        * Calculate number of visible (Not hidden) WLANs
        * @since S60 v.5.0
        * @param aArray Array of WLAN passed as reference
        * @return Number of visible WLANs
        */ 
        TInt VisibleWlans( CWsfWlanInfoArray& aArray );
    
    private: // Data
    
        /**
         * Copy of showed Wlan info
         */
        TWsfWlanInfo iWlanInfo;
        
        /**
         * Formater Wlan info
         * Own.
         */
        CDesCArrayFlat* iFormattedWlanInfo;    
        
        /**
         * Time when connection started
         */    
        TTime iStartTime;
    
        /**
         * True if the connection is still active
         */    
        TBool iIsConnActive;
    
        /**
         * Duration of the connection
         */    
        TTimeIntervalMicroSeconds iDuration;
    
        /**
         * Total number of bytes transmitted
         */    
        TUint iBytesTransmitted;    
            
        /**
         * The Connection Monitor
         */    
        RConnectionMonitor iConnectionMonitor;
            
        /**
         * The Connection ID of the WLAN
         */    
        TInt iConnectionId;
        
        /**
         * Coe env for loading string from resource
         * Ref.
         */
        CCoeEnv* iCoeEnv; 
        
        /**
         * Reference to Wlan array
         * ref.
         */
        CWsfWlanInfoArray* iWlanArray;

        /**
         * Acts as a mutex for refreshing functions
         */
        TBool iRefreshing;
    };

#endif // C_WSFCONNECTEDDETAILSMODEL_H
