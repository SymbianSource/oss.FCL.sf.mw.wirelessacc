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
* Description:  Class header for MWsfMainViewControllerIf
*
*/

#ifndef M_WSFMAINVIEWCONTROLLERIF_H
#define M_WSFMAINVIEWCONTROLLERIF_H


#include <e32std.h>

//Forward declarations
class CWsfWlanInfoArray;
class TWsfWlanInfo;
class MWsfMainViewPartner;

/**
 *  Mainview controlling interface (Internal)
 *
 *  @lib Sniffer.exe
 *  @since S60 v5.0
 */
class MWsfMainViewControllerIf
    {
    public:
    
        /**
         * Updates WLAN list with values passed as parameter
         * @param aWlanArray list of WLAN
         * @since S60 5.0
         */
        virtual void UpdateWlanListL( CWsfWlanInfoArray* aWlanArray ) = 0;

        /**
         * Updates WLAN list with values passed as parameter
         * when the main view is in the background
         * @param aWlanArray list of WLAN
         * @since S60 5.2
         */
        virtual void UpdateWlanListInBackgroundL(
                CWsfWlanInfoArray* aWlanArray ) = 0;
    
        /**
         * Request to refresh the mainview
         * @since S60 5.0
         */
        virtual void RefreshL( ) = 0;
    
        /**
         * Request to refresh the mainview
         * @return Details of selected WLAN
         * @since S60 5.0
         */
        virtual TWsfWlanInfo* SelectedItem() = 0;
       
        /**
         * Sets partner object
         * @aPartner The partner
         * @since S60 5.0
         */
        virtual void SetPartner( MWsfMainViewPartner& aPartner ) = 0;
    };


#endif // M_WSFMAINVIEWCONTROLLERIF_H
