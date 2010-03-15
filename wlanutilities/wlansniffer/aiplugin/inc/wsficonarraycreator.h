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
* Description:  Class header for CWsfIconArrayCreator
*
*/


#ifndef C_WSFICONARRAYCREATOR_H
#define C_WSFICONARRAYCREATOR_H

//  EXTERNAL INCLUDES
#include <e32base.h>


//  FORWARD DECLARATIONS
class CAknIconArray;
class CGulIcon;

//  CLASS DEFINITION
/**
 * Responsible for the creation of the suitable needed icons.
 * @since S60 5.0 
 */
NONSHARABLE_CLASS( CWsfIconArrayCreator ) : public CBase
    {
    public:     // Constructors and destructor
    	
    	/**
        * Create an icon array
        * @since S60 5.0
        * @return The constructed icon array. Ownership passed.
        */
        static CAknIconArray* CreateArrayL();

        /**
         * Returns the pointer to icon loaded by the AknIconUtils/AknUtil
         * @param aIconId The id of the icon to load
         * @return Pointer of the icon.
         */
        static CGulIcon* GetIconLC( TInt aIconId );
    	
    };

#endif // C_WSFICONARRAYCREATOR_H

// end of file
