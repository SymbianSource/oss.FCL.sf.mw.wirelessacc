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
* Description:  Class header for CWsfConnectedDetailsDialog
*
*/

#ifndef C_WSFCONNECTEDDETAILSDIALOG_H
#define C_WSFCONNECTEDDETAILSDIALOG_H


//  INCLUDES
#include <aknlistquerydialog.h>

// INTERNAL INCLUDES
#include "wsfconnecteddetailscontroller.h"

// FORWARD DECLARATIONS
class CWsfConnectedDetailsModel;
class TWsfConnectedDetailsController;
class CWsfActiveTimeUpdater;


// CLASS DECLARATION

/**
*  CWsfConnectedDetailsDialog class.
*  This class is used to display the Connected Details information
*/
class CWsfConnectedDetailsDialog : public CAknListQueryDialog
    {
    public:     // Constructors and destructor

        /**
        * Two-phase construction.
        */
        static CWsfConnectedDetailsDialog* NewL();
        
        /**
        * Two-phased constructor.
        */
        static CWsfConnectedDetailsDialog* NewLC();
                            
        /**
        * Destructor.
        */
        virtual ~CWsfConnectedDetailsDialog();
        
    private:
        
        /*
        * Second-phase constructor.
        */        
        void ConstructL();

        /**
        * C++ default constructor.
        * @param aSelectionIndexArray The items which are selected within 
        *        the dialogs list box list.
        */
        CWsfConnectedDetailsDialog( 
                CListBoxView::CSelectionIndexArray* aSelectionIndexArray );

    public:     // New functions
    
        /**
        * Refresh the content list box of the dialog
        * @param aItemTextArray - items to be show on listbox
        */
        void UpdateListBox( MDesCArray* aItemTextArray );
        
        /**
        * Return controller interface
        * @return controller interface
        */
        MWsfDetailsViewControllerIf& Controller();               

        
        /**
         * Set list model of the dialog.
         * @param aModel List model of the dialog
         */
        void SetListModel( CWsfConnectedDetailsModel* aModel );
        
    private:    // from CEikDialog

        /**
        * sets the default value to the dialog.
        */
        void  PreLayoutDynInitL();

    private:    // Data

        /**
        * Reference for listbox
        * Not own.
        */
        CEikListBox*                        iList;
       
        /**
        * Reference for list box model
        * Not own.
        */
        CTextListBoxModel*                  iModel;

        /*
        * Connected Details Dilaog Controller 
        */
        TWsfConnectedDetailsController  iController;
        
        /*
        * Reference for the connected details model
        * Not own. 
        */
        CWsfConnectedDetailsModel*      iListModel;
        
        /*
        * Reference for the time updater
        * Own. 
        */
        CWsfActiveTimeUpdater*              iActiveUpdater;
    };

#endif // C_WSFCONNECTEDDETAILSDIALOG_H

// End of File

