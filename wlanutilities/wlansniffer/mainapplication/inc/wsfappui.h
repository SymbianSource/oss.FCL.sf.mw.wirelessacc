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
* Description:  Class header for CWsfAppUi
*
*/

#ifndef C_WSFAPPUI_H
#define C_WSFAPPUI_H

#include <e32std.h>
#include <AknWaitDialog.h>
#include <aknViewAppUi.h>


//  FORWARD DECLARATIONS
class CWsfMainView;
class MWsfMainUiObserver;
class MWsfMainViewControllerIf;
class MWsfDetailsViewControllerIf;
class CAknWaitDialog;
class CWsfDocument;
class CWsfConnectedDetailsDialog;
class CWsfWlanInfoArray;

/**
 * 'AppUi' class.
 *
 * @since S60 v5.0
 * @lib wlansniffer.exe
 */
NONSHARABLE_CLASS( CWsfAppUi ): public CAknViewAppUi, 
                                public MProgressDialogCallback
    {
    public:
        
        /**
        * 2nd phase constructor
        * @since S60 5.0 
        */
        void ConstructL();
    
        /**
        * Default C++ constructor 
        * @since S60 5.0 
        */
        CWsfAppUi();
    
        /**
        * Destructor of CWsfAppUi class
        * @since S60 5.0     
        */
        ~CWsfAppUi();
    
    
    public: // from CAknAppUi
        /**
        * Handle user menu selections
        * @since S60 5.0     
        * @param aCommand The enumerated code for the option selected
        */
        void HandleCommandL( TInt aCommand );
    
        /**
        * Handles a change to the application's resources
        * which are shared across the environment.
        * @since S60 5.0     
        * @param aType type of resources that have changed. 
        */
        void HandleResourceChangeL( TInt aType );

        /**
        * Handles changes in keyboard focus when an application switches to, 
        * or from, the foreground.
        * @since S60 5.0     
        * @param aForeground ETrue if the application is in the foreground,
        * otherwise EFalse.
        */
        void HandleForegroundEventL( TBool aForeground );    

        
    public:
    
        /**
        * Set an interface to the main controller for the AppUI.
        * @since S60 5.0   
        * @param aObserver Observer object
        */
        void SetUiObserver( MWsfMainUiObserver* aObserver );
        
        /**
        * Activate the Details View
        * @since S60 5.0   
        * @param aDetailsViewId View ID of Details View
        */
        void ActivateDetailsViewL( TUid aDetailsViewId );
        
        /**
        * Activate the Main View
        * @since S60 5.0   
        */
        void ActivateMainViewL();    
    
        /**
        * Create the Main View 
        * @since S60 5.0
        * @return reference to the Main View's controller
        */
        MWsfMainViewControllerIf& MainView();
    
        /**
        * Create the Details View
        * @since S60 5.0
        * @return reference to the Details View's controller
        */
        MWsfDetailsViewControllerIf& DetailsView( TInt aDetailsViewType );
        
        /**
        * Starts the wait note dialog
        * @since S60 5.0
        * @param aVisibilityDelayOff set the visibility of the dialog
        */
        void StartWaitNoteL( TBool aVisibilityDelayOff );
    
        /**
        * Dismiss the wait note dialog
        * @since S60 5.0
        */
        void HideWaitNoteL();

        /**
        * Displays the Main View's menu bar.
        * @since S60 5.0
        */
        void ShowMenuBarL();
    
        /**
        * Displays "No WLAN found" note.
        * @since S60 5.0
        */
        void ShowNoWlansFoundInfoL();
        
        /**
        * Displays an error note.
        * @since S60 5.0
        * @param aResourceId the text of the error note
        */
        void ShowErrorNoteL( TInt aResourceId ); 
        
        /**
        * Displays a Global error note.
        * @since S60 5.0
        * @param aError error ID
        */
        void ShowGlobalErrorNoteL( TInt aError );
        
        /**
        * Returns a pointer to the CWsfDocument
        * @since S60 5.0
        * @return pointer to the CWsfDocument
        */
        CWsfDocument* Doc();
        
        /**
        * Returns CWlanSettingsUi is in foreground
        * @since S60 5.0
        * @return ETrue if CWlanSettingsUi is in foreground
        *         EFalse if CWlanSettingsUi is in background
        */
        TBool Foreground();
        
        /**
        * Starts the Connected Details View 
        * @since S60 5.0
        * @param aWlanArray pointer to the Wlan Info Array
        * @param aSelectedWlanSsid string of the selected Wlan SSID
        */
        void StartConnectedDetailsL( CWsfWlanInfoArray* aWlanArray,
                                     const TDesC8& aSelectedWlanSsid );
    
        /**
        * Update content of the Connected Details View 
        * @since S60 5.0
        * @param aWlanArray pointer to the Wlan Info Array
        */
        void UpdateConnectedDetailsL( CWsfWlanInfoArray* aWlanArray );

        /**
         * Returns the value of key event suppression flag
         * @since S60 5.0
         * @return ETrue if key events are suppressed
         */
        TBool SuppressingKeyEvents() const;
        
        /**
         * Sets the value of key event suppression flag
         * @since S60 5.0
         * @param aSuppressing The new value of the flag
         */
        void SetSuppressingKeyEvents( TBool aSuppressing );
        
        
    protected:  // from MProgressDialogCallback
    
        /**
        * Callback function for MProgressDialogCallback
        * @since S60 5.0
        * @param aButtonId the ID of the button that was activated
        */
        void DialogDismissedL( TInt aButtonId ); 
    
        
    private:    // new methods
        
        /**
        * This function is used for querying whether the application
        * is launched in embedded mode or not.
        * @return ETrue:  The application is launched in embedded mode.
        *         EFalse: The application is launched in standalone mode.
        */
        TBool IsEmbedded() const;
    
        /**
        * Callback for CIdle. 
        * @since S60 5.0 
        * @param aObject pointer to this
        * @return zero value means not be called again.(Stops timer)
        */
        static TInt AppLaunchCompleteL( TAny* aObject );
    
    
    private:
    
        /**
        * Reference to UI observer
        * Not own.
        */
        MWsfMainUiObserver* iObserver;
    
        /**
        * Wait dialog
        * Own.
        */
        CAknWaitDialog* iWaitDialog;
    
        /**
        * Connected details view dialog
        * Own.
        */
        CWsfConnectedDetailsDialog* iConnectedDetailsDialog;
        
        /**
        * Used to launch wait dialog when application has been 
        * fully started and screen has been drawn
        * Own.
        */
        CIdle* iIdle;
        
        /*
        * Used to check CWlanSettingsUi is in foreground
        */
        TBool iForeground;
        
        /**
        * indicates whether the launching has ended
        */
        TBool iAppLaunchCompleted;
        
        /**
        * Indicates Featuremanger is initialized or not.
        */
        TBool iFeatureManagerInitialized;
        
        /**
         * Indicates whether keypress events are suppressed
         */
        TBool iSuppressingKeyEvents;
    };


#endif // C_WSFAPPUI_H

