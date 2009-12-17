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
* Description:  Class header for CWsfWlanSsidSelectionDlg2
*
*/



#ifndef C_WSFWLANSSIDSELECTIONDLG2_H
#define C_WSFWLANSSIDSELECTIONDLG2_H


//  EXTERNAL INCLUDES
#include <aknlistquerydialog.h>
#include <badesca.h>


// FORWARD DECLARATIONS
class CAknTitlePane;
class CAknIconArray;
class CGulIcon;


//  CLASS DEFINITION
/**
* Dialog for modifying the SSID blacklist
*
* @lib wsfwlaninfosorting.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CWsfWlanSsidSelectionDlg2 ): public CAknListQueryDialog,
                                                public MEikListBoxObserver
    {
    public:     
        /**
        * Factory function.
        * @since S60 5.0
        * @param aSsidArray Array of all SSIDs. Ownership not passed.
        * @param aSelectionIndexArray Array to indicate selected state of 
        *                             the SSIDs. Ownership not passed.
        * @return Class instance
        */
        static CWsfWlanSsidSelectionDlg2* NewL( CDesCArrayFlat& aSsidArray,
                    CListBoxView::CSelectionIndexArray& aSelectionIndexArray );

        /**
        * Factory function.
        * @since S60 5.0
        * @param aSsidArray Array of all SSIDs. Ownership not passed.
        * @param aSelectionIndexArray Array to indicate selected state of 
        *                             the SSIDS. Ownership not passed.
        * @return Class instance
        */
        static CWsfWlanSsidSelectionDlg2* NewLC( CDesCArrayFlat& aSsidArray,
                    CListBoxView::CSelectionIndexArray& aSelectionIndexArray );

        ~CWsfWlanSsidSelectionDlg2();


    private:
        /**
        * Constructor.
        * @since S60 5.0
        * @param aSsidArray Array of all SSIDs. Ownership not passed.
        * @param aSelectionIndexArray Array to indicate selected state of 
        *                             the SSIDS. Ownership not passed.
        * @return Class instance
        */
        CWsfWlanSsidSelectionDlg2( CDesCArrayFlat& aSsidArray,
                    CListBoxView::CSelectionIndexArray& aSelectionIndexArray );
                    
        void ConstructL();

        
    public: // from CEikDialog
        void PreLayoutDynInitL();
        

    public: // From MEikListBoxObserver
        void HandleListBoxEventL( CEikListBox* aListBox, 
                                  TListBoxEvent aEventType );                       
            
    private:    // new methods

        /**
        * Creates a listbox item array of the SSID list
        * @since S60 5.0
        * @param aSource The source SSID array. Ownership not passed.
        * @param aDest The array to put the resulting listbox items in.
        *              Ownership not passed.
        */
        void CopyAndFormatArrayContentsL( CDesCArrayFlat* aSource,
                                          CDesCArrayFlat* aDest );

        /**
        * Function to create checkbox icons for the markable list
        * @since S60 5.0
        * @param aFileName Path to the icon file
        * @param aIcon Icon id
        * @param aMask Mask id
        * @param aID Item ID of the masked bitmap to be created.
        * @return The constructed icon. Ownership passed.
        */
        CGulIcon* CreateIconLC( const TDesC& aFileName, TInt aIcon, TInt aMask,
                                const TAknsItemID& aID );
    
        /**
        * Create an icon array of the checkbox icons
        * @since S60 5.0
        * @return The constructed icon array. Ownership passed.
        */
        CAknIconArray* CreateQueryIconArrayL();        


    private: //data
    
        /**
        * Array of SSIDs in the list
        */
        CDesCArrayFlat* iSsidArray;
    
        /**
        * Array of selection states
        */
        CArrayFix<TInt>* iSelectedItems;
    
        /**
        * Reference to the title pane of the list. Not owned.
        */
        CAknTitlePane* iPane;
    
        /**
        * Title text. Owned.
        */
        HBufC* iTitle;

    };

    
#endif // C_WSFWLANSSIDSELECTIONDLG2_H
