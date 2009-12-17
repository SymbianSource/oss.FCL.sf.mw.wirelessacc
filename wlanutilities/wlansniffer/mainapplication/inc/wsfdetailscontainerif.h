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
* Description:  Class header for MWsfDetailsContainerIf
*
*/

#ifndef M_WSFDETAILSCONTAINERIF_H
#define M_WSFDETAILSCONTAINERIF_H

//  EXTERNAL INCLUDES
#include <e32def.h>

//  FORWARD DECLARATIONS
class MDesCArray;

//  CLASS DEFINITION
/**
 *  An interface to the Details view’s container for updating
 *  its listbox or navigation pane.
 *  @since S60 v5.0
 */
class MWsfDetailsContainerIf
    {
    public:     // Abstract methods
        
        /*
        * Refresh the content list box.
        * @param aItemTextArray - items to be show on listbox
        */
        virtual void UpdateListBoxL( MDesCArray* aItemTextArray ) = 0;
        
        /**
        * Refresh the content of Navi pane.
        * @param aPaneText - text to be show on pane
        */
        virtual void UpdateNaviPaneL( const TDesC& aPaneText ) = 0;

    };

#endif      //  M_WSFDETAILSCONTAINERIF_H

// End of file
