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
* Description:  Class header for CWsfSearchWlansDialog
*
*/


#ifndef C_WSFSEARCHWLANSDIALOG_H
#define C_WSFSEARCHWLANSDIALOG_H

#include <e32base.h>
#include <aknlistquerydialog.h>

//  INTERNAL INCLUDES
#include "wsfaiuiobserver.h"


NONSHARABLE_CLASS( CWsfSearchWlansDialog ) : public CAknListQueryDialog, 
                                             public MWsfAiUiObserver
    {
public:
    
    /**
    * Two-phased constructor
    * @since S60 5.0 
    * @param aSelectedItem - reference of the selected item
    * @return instance of CWsfSearchWlansDialog class
    */
    static CWsfSearchWlansDialog* NewL( TInt &aSelectedItem );
    
    /**
    * Two-phased constructor. Leaves the object on CleanupStack
    * @since S60 5.0 
    * @param aSelectedItem - reference of the selected item
    * @return instance of CWsfSearchWlansDialog class
    */
    static CWsfSearchWlansDialog* NewLC( TInt &aSelectedItem );
    
    /**
    * Destructor of CWsfSearchWlansDialog class
    * @since S60 5.0     
    */
    ~CWsfSearchWlansDialog();

private:
    
    /**
    * Default C++ constructor 
    * @since S60 5.0 
    * @param aSelectedItem - reference of the selected item 
    */
    CWsfSearchWlansDialog( TInt &aSelectedItem );

    /**
    * 2nd phase constructor
    * @since S60 5.0 
    */
    void ConstructL();

private: // from CEikDialog
    
    /**
    * From CEikDialog
    * Framework method called to processs a command ID
    * @param aCommandId ID of the command to respond to.
    */
    void ProcessCommandL( TInt aCommandId );
    
    /**
    * Sets the default value to the dialog.
    */
    void PreLayoutDynInitL();
    
private: // from MWsfAiUiObserver

    /**
    * Update the contents of control
    * Set the focus on aCurrentImte
    * @since S60 5.0 
    * @param aItemTextArray text of the current item
    * @param aCurrentItem index of the current item
    */ 
    void UpdateHotSpotsL( MDesCArray* aItemTextArray, 
                          TInt aCurrentItem );        
    
    /**
    * Gives back the index of the selected item
    * @since S60 5.0 
    * @return the index of the selected item
    */ 
    TInt SelectedItem();
    
    /*
    * Check if the control is multiline capable control 
    * @since S60 5.0 
    * @return Etrue if control is multiline capable control
    */
    TBool MultilineControl(); 

    /*
    * Display engine off text in UI
    * @since S60 5.0 
    */
    void DisplayEngineOffL();
    
    /*
    * Set the refressing UI note visible
    * @since S60 5.0 
    */
    void ForceRefreshingL();
    
    /*
    * Starts connecting animation
    * @since S60 5.2 
    */
    void StartConnectingAnimationL();
    
    /**
    * Update the contents of control
    * @since S60 5.0 
    * @param aItemTextArray text of the current item
    */ 
    void UpdateViewL( MDesCArray* aItemTextArray );


private: 
    
    /**
     * Reference of the selecte item
     */
    TInt *iSelectedItem;
    };



#endif // C_WSFSEARCHWLANSDIALOG_H
