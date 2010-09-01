/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Declares container control for HotSpot Browser Application.
*
*/



#ifndef HSBROWSERCONTAINER_H
#define HSBROWSERCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <coecobs.h>
#include <ictscommon.h>
#include <ictsclientinterface.h>
#include <eikmenup.h>
#include <brctlinterface.h>
#include <wlanmgmtcommon.h>
#include <wlanmgmtclient.h>

// FORWARD DECLARATIONS
class CBrCtlInterface;
class CHsBrowserSpecialLoadObserver;
class CHsBrowserLoadEventObserver;
class CHsBrowserIctsObserver;
class CHsBrowserHsSrvNotifs;
class CHsBrowserModel;
class TCoeEvent;
class CAknNavigationControlContainer;
class CAknNavigationDecorator;
class CAknIndicatorContainer;
class CAknNaviLabel;       

// CONSTANTS
const TInt KComponentsNumber = 1;
const TInt KBrCtlComponentIndex = 0;

// CLASS DECLARATION
/**
*  CHsBrowserContainer class.
*  This is the container control class.
*/
class CHsBrowserContainer : public CCoeControl
    {
    public: // Constructors and destructor
        
        /**
        * C++ default constructor.
        *
        * @param aModel Pointer to model of HsBrowser.
        */
        CHsBrowserContainer( CHsBrowserModel* aModel );
        
        /**
        * EPOC default constructor.
        *
        * @param aRect Frame rectangle for container.
        */
        void ConstructL( const TRect& aRect );

        /**
        * Destructor.
        */
        ~CHsBrowserContainer();

    public:
            enum TNaviPaneEvent
            {
            ELoadingLoginPage,
            ELoadingUrl,
            EPageTitleAvailable,
            ELoadingFinished
            };


    public: // new

        /**
        * Initializes the indicators in the navi pane.
        */   
        void InitNaviPaneL();

        /**
        * Updates the indicators in the navi pane.
        */      
        void UpdateNaviPane( TNaviPaneEvent aUpdateEvent );

        /**
        * Pass a command to the Browser Control
        *
        * @param aCommand The command that the Browser Control should process.
        */
        void HandleCommandL( TInt aCommand );        

        /**
        * Handles key events
        *
        * @param aKeyEvent The key event that occurred.
        * @param aType The window server event type that is being handled
        * @return TKeyResponse Value indicates whether or not the key event was consumed by the control.
        *                      The default implementation simply returns EKeyWasNotConsumed.
        */
        TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
        
        /**
        * Handle events when when an application switches to, 
        * or from, the foreground.
        *
        * @param aForeground ETrue if the application is in the foreground,
        * otherwise EFalse.
        */
        void ProcessForegroundEvent( TBool aForeground );
        
        /**
        * Send application to foreground.
        */
        void SendToForeground();
        
        /**
        * Restore original ordinary position and priority.
        */
        void RestorePositionAndPriority();

        /**
        * Send application to background.
        */        
        void SendToBackground();
        
        /**
        * Update softkeys according to model state. 
        */
        void UpdateSoftkeys();
        
        /**
        * Show login complete information note.
        */
        void ShowLoginCompleteNote();
        
        /**
        * Show connection closed information note.
        */
        void ShowConnClosedNote();
        
        /**
        * Initialize model, attach to connection, and load specified URL.
        *
        * @param aUrl URL of authentication web page.
        * @param aIapId ID of IAP.
        * @param aNetId Network ID.
        * @return KErrNone, if successful, otherwice one of system wide errorcodes.
        */
        TInt LoadRedirect( const TPtrC& aUrl, TInt aIapId, TInt aNetId );
        
        /**
        * Called when wlan connection is lost.
        * Shows information note and exits.
        */
        void ProcessWlanConnModeNotConnected();
                
        /**
        * Returns boolean value for application foreground status.
        *
        * @return ETrue / EFalse
        */
        TBool IsForeground();
        
    public: // access

        /**
        * Accessor method for iBrCtlInterface
        *
        * @return CBrCtlInterface* A pointer to the browser control interface.
        */
		inline CBrCtlInterface* BrCtlInterface();

        /**
        * Accessor method for iHsBrowserSpecialLoadObserver.
        *
        * @return CHsBrowserSpecialLoadObserver* A pointer to the special load observer.
        */
		inline CHsBrowserSpecialLoadObserver* HsBrowserSpecialLoadObserver();
        
        /**
        * Accessor method for iHsBrowserIctsObserver.
        *
        * @return CHsBrowserIctsObserver* A pointer to the Internet connectivity test observer.
        */
		inline CHsBrowserIctsObserver* HsBrowserIctsObserver();
        
        /**
        * Accessor method for iModel.
        *
        * @return CHsBrowserModel* A pointer to the model of HsBrowser.
        */
        inline CHsBrowserModel* Model();        
        
        /**
        * Accessor method for iNaviLabel.
        *
        * @return CAknNaviLabel* A pointer to the label control in the navigation pane.
        */
        inline CAknNaviLabel* NaviLabel();
        
        /**
        * Get boolean value if input box is active or not.
        *
        * @return ETrue / EFalse
        */
        inline TBool ActiveInputBox();
        
        /**
        * Set input box active flag
        */
        inline void SetActiveInputBox( TBool aActiveInputBox );
        
    private: // From CoeControl

        /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

        /**
        * From CoeControl,CountComponentControls.
        */
        TInt CountComponentControls() const;

        /**
        * From CCoeControl,ComponentControl.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CCoeControl,Handle key events.
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType);
    
        /**
        * From CCoeControl,Handle resource change events.
        */
        void HandleResourceChange( TInt aType );
    
    private: //data 
    
        /**
        * Pointer to the browser control interface.
        * Owned.
        */
        CBrCtlInterface* iBrCtlInterface;

        /**
        * Command Base
        */
        TInt iCommandBase;

        /**
        * Special Load Observer
        */
        CHsBrowserSpecialLoadObserver* iHsBrowserSpecialLoadObserver;
        
        /**
        * Load Event Observer
        */
        CHsBrowserLoadEventObserver* iHsBrowserLoadEventObserver;

        /**
        * Internet connectivity test observer
        */
        CHsBrowserIctsObserver* iHsBrowserIctsObserver;
        
        /**
        * Notifications from hotspot server.
        */
        CHsBrowserHsSrvNotifs* iHsBrowserHsSrvNotifs;
        
        /**
        * Model of HsBrowser
        */
        CHsBrowserModel* iModel;
        
        /**
        * Pointer to default control in the status pane's navigation pane.
        * Not owned.
        */
        CAknNavigationControlContainer* iNaviPane;
        
        /**
        * Pointer to the editor indicator control in the navigation pane.
        * Owned.
        */
        CAknNavigationDecorator* iEditorIndicatorContainer;
                
        /**
        * Pointer to the editor indicator control in the navigation pane
        * Not owned.
        */
        CAknIndicatorContainer* iIndiContainer;
        
       /**
        * Resource.
        */
        TInt iResource; 
        
        /**
        * Flag for softkey press.
        */
        TBool iActiveInputBox;
        
        /**
        * handle to WlanMgmtClient
        */
     	CWlanMgmtClient *iMgtClient;
    };

#include "hsbrowsercontainer.inl"

#endif

// End of File

