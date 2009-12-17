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
* Description:  Interface for wlan active idle plugin ui control
*
*/


#ifndef M_WSFAIUIOBSERVER_H
#define M_WSFAIUIOBSERVER_H


#include <e32std.h>
/**
 * Abstract interface for wlan active idle plugin ui control 
 * - from the point of view of model
 * @since S60 5.0 
 */
class MWsfAiUiObserver
    {

    public:

        /**
        * Update the contents of control
        * Set the focus on aCurrentImte
        * @since S60 5.0 
        * @param aItemTextArray text of the current item
        * @param aCurrentItem index of the current item
        */ 
        virtual void UpdateHotSpotsL( MDesCArray* aItemTextArray, 
                                      TInt aCurrentItem ) = 0;        
                
        /**
        * Gives back the index of the selected item
        * @since S60 5.0 
        * @return the index of the selected item
        */ 
        virtual TInt SelectedItem() = 0;

        
        /*
        * Check if the control is multiline capable control 
        * @since S60 5.0 
        * @return Etrue if control is multiline capable control
        */
        virtual TBool MultilineControl() = 0; 
        
        /*
        * Display engine off text in UI
        * @since S60 5.0 
        */
        virtual void DisplayEngineOffL() = 0;
        
        /*
        * Set the refressing UI note visible
        * @since S60 5.0 
        */
        virtual void ForceRefreshingL() = 0;
        
        /**
        * Update the contents of control
        * @since S60 5.0 
        * @param aItemTextArray text of the current item
        */ 
        virtual void UpdateViewL( MDesCArray* aItemTextArray ) = 0;
        
        /**
        * Update the contents of control
        * @since S60 5.0 
        * @param aItemTextArray text of the current item
        */ 
        virtual void StartConnectingAnimationL() = 0;
    };


#endif // M_WSFAIUIOBSERVER_H
