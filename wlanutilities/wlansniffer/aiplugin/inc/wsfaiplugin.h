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
* Description:  Class header for CWsfAiPlugin
*
*/


#ifndef C_WSFAIPLUGIN_H
#define C_WSFAIPLUGIN_H

//  EXTERNAL INCLUDES
#include <aicontentpublisher.h>
#include <aicontentobserver.h>
#include <aipropertyextension.h>
#include <aieventhandlerextension.h>
#include <e32cmn.h>

//  INTERNAL INCLUDES
#include "wsfaicontroller.h"
#include "wsfaipublishobserver.h"
#include "wsfaiplugincontentmodel.h"

//  CONSTANTS
LOCAL_D const TInt  KDeferredWaitTime = 1000 * 1000; 
                            // one second in milliseconds

//  FORWARD DECLARATIONS
class MAiPluginSettings;
class MAiContentObserver;
class MAiContentItemIterator;

class CWsfModel;
class CWsfAiModel;
class CWsfAiView;
class CAknIconArray;
class CGulIcon;
class CWsfDbObserver;


// CLASS DECLARATION

/**
 * The main class, containing the plugin behavior. 
 *
 * @lib wsfaiplugin.lib
 * @since S60 v5.0
 */
NONSHARABLE_CLASS( CWsfAiPlugin ): public CAiContentPublisher,
                                   public MAiPropertyExtension,
                                   public MAiEventHandlerExtension,
                                   public MWsfAiPublishObserver
    {
    public: // Constructors and destructor
        
        /**
        * Two-phased constructor
        * @since S60 5.0 
        * @return instance of CWsfAiPlugin class
        */
        static CWsfAiPlugin* NewL();
        
        /**
        * Destructor of CWsfAiPlugin class
        * @since S60 5.0     
        */
        virtual ~CWsfAiPlugin();
    
    private:// Constructors
        
        /**
        * Default C++ constructor 
        * @since S60 5.0 
        */
        CWsfAiPlugin();
        
        /**
        * 2nd phase constructor
        * @since S60 5.0 
        */
        void ConstructL();
        
        /**
        * Steps 
        * @param ptr Pointer to an instance of this class
        */
        static TInt DoRefreshingStepL( TAny* ptr );
        
        /**
        * Steps the refresh animation
        */                
        void DoRefreshingStepL();
        
        /**
        * Steps 
        * @param ptr Pointer to an instance of this class
        */
        static TInt DoConnectingStepL( TAny* ptr );
        
        /**
        * Steps the refresh animation
        */                
        void DoConnectingStepL();
    
    public:     // from CAiContentPublisher
        
        /**
        * This method transit the plugin to "Alive" state.
        * The method is called by the framework to instruct plug-in that it is
        * allowed to actively publish its data to its observers. This means the plugin
        * is allowed to consume memory and CPU resources, e.g plug-in is able load 
        * engines, run timers, perform asynchronous operations, etc. The method 
        * transits the plug-in to "Alive" state. There can be many concurrent
        * calls to resume, with different or the same reason code, this allows
        * the plugin to properly respond to enviroment change that raise the
        * need to re-publish content (changes like date/time change etc).
        *
        * @param aReason reason for state change, see TAiTransitionChange.
        */
        void Resume( TAiTransitionReason aReason );
        
        /**
        * This method transits the plug-in to "Suspendend" state.
        * The method is called by the framework to instruct plug-in that it is
        * not allowed to consume CPU resources, e.g plug-in MUST stop each
        * timer, cancel outstanding asynchronous operations, etc. 
        *
        * @param aReason reason for state change, see TAiTransitionChange.
        */
        void Suspend( TAiTransitionReason aReason );
        
        /**
        * This method transits the plug-in to "Idle" state.
        * The method is called by the framework to request the plug-in free all
        * memory and CPU resources and close all its open files, the plug-in 
        * should unload its engines during backup operation.
        *
        * @param aReason reason for state change, see TAiTransitionChange.
        */
        void Stop( TAiTransitionReason aReason );
        
        /**
        * Adds the content observer / subscriber to plug-in. The plug-in MUST
        * maintain a registry of subscribers and send notification to all them
        * whenever the plug-in changes state or new content available.
        *
        * @param aObserver content observer to register.
        */
        void SubscribeL( MAiContentObserver& aObserver );
    
        /**
        * Configures the plug-in.
        *
        * @param aSettings setting items defined in the UI definition.
        *                  This plugin takes ownership of the
        *                  MAiPluginSettings objects in the array.
        *                  If this method leaves the caller will handle the cleanup.
        */
        void ConfigureL( RAiSettingsItemArray& aSettings );
        
        /**
        * Returns interface extension. In S60 3.2 only event & property
        * extensions are supported. See MAiEventHandlerExtension & MAiPropertyExtension
        * interfaces.
        *
        * @param  aUid - UID of the extension interface to access.
        * @return the extension interface. Actual type depends on the passed aUid 
        *         argument.
        */
        TAny* Extension( TUid aUid );

    
    public:     // from MAiPropertyExtension
    
        /**
        * Read property of publisher plug-in.
        *
        * @param aProperty - identification of property.
        * @return Pointer to property value.
        */
        TAny* GetPropertyL( TInt aProperty );
    
        /**
        * Write property value.
        *
        * @param aProperty - identification of property.
        * @param aValue - contains pointer to property value.
        */
        void SetPropertyL( TInt aProperty, TAny* aValue );
    
    
    public:     // from MAiEventHandlerExtension
    
        /**
        * See aieventhandlerextension.h for detailed description.
        * @since S60 5.0
        */
        void HandleEvent( TInt aEvent, const TDesC& aParam );

    
    public: // From MWsfAiPublishObserver
    
        /**
        * Publishes images from array and text
        * @since S60 5.0
        * @param aPublishIconArray the published icons
        * @param aText1 title text label
        */
        void PublishContentL( CArrayFix<TInt>* aPublishIconArray,
                              CEikLabel* aText1 );
        
        /**
        * Sets signal strenght and the security string
        * @since S60 5.0  
        * @param aStrength siganl strength string
        * @param aSecure security string
        */
        void SetStrengthAndSecure( TDesC* aStrength, TDesC* aSecure );
        
        /**
        * Sets Scanning status on/off.
        * @since S60 5.0
        * @param aScanState Scanning satus flag
        */
        void SetScanningState( TBool aScanState );
        
        /**
        * Sets refreshing status on/off.
        * @since S60 5.0
        * @param aScanState Refreshing status flag
        */
        void SetRefreshingL( TBool aRefreshing );
        
        /**
        * Set connecting status on.
        * @since S60 5.0
        */
        void StartConnectingL();
        
        /**
        * Publishes just status image from array 
        * @since S60 5.0
        * @param aPublishIconArray the published icons
        * @param aText1 title text label
        */
        void PublishStatusIconL( CArrayFix<TInt>* aPublishIconArray, 
                                            CEikLabel* aText1 );
        
    private:
    
        /**
        * Allocates Engine, Model and functional UI for plugin
        */
        void AllocateResourcesL();
    
        /**
        * Unpublishes (clears) all plugin content from AI2 view
        */
        void ClearL();
    
        /**
        * Launch WLAN Wizard application
        */
        void LaunchApplicationL();
    
        /**
        * Set the value of Middle Soft Key based on the
        * Scanning status of the WLAN Wizard
        */
        void SetMskL();
        
        /**
        * Publish the given text
        * @param aObserver - pointer to the observer
        * @param aContentId - content ID
        * @param aText1 - title text label
        * @return ETrue - if it can be published
        *         EFalse - if it can not be published
        */
        TBool PublishText( MAiContentObserver* aObserver,
                           TWsfAiWizardContentIds aContentId,
                           const TDesC16& aText );
        
        /**
        * Publish the given icon
        * @param aObserver - pointer to the observer
        * @param aContentId - content ID
        * @param aIconId - icon ID
        * @return ETrue - if it can be published
        *         EFalse - if it can not be published
        */
        TBool PublishIconL( MAiContentObserver* aObserver,
                            TWsfAiWizardContentIds aContentId,
                            TInt aIconId );
    
        /**
        * Cleanup function to to cancel publish transaction
        * @since S60 5.0
        * @param aPtr Pointer for the observer class
        */
        static void PublishCleanup( TAny* aPtr );
       
        /**
        * Callback function for carrying out refresh at startup
        * @since S60 5.0
        * @param aPtr Pointer to this class
        * @return Error code        
        */
        static TInt StartupRefresh( TAny* aPtr );

        /**
        * Callback function for carrying out refresh at startup
        * @since S60 5.0
        * @return Error code        
        */
        TInt StartupRefreshL();

        
    private: // Data
    
        /**
        * Array of content observers
        */
        RPointerArray<MAiContentObserver> iObservers;
    
        /**
        * Iterator for plugin content
        * Own.
        */
        MAiContentItemIterator* iContent;
    
        /**
        * Iterator for plugin resources
        * Own.
        */
        MAiContentItemIterator* iResources;
    
        /**
        * Iterator for plugin events
        * Own.
        */
        MAiContentItemIterator* iEvents;
    
        /**
        * Information about the content publisher (this plug-in)
        */
        TAiPublisherInfo iInfo;
    
        /**
        * Pointer to CWsfModel
        * Own.
        */
        CWsfModel* iModel;
        
        /**
        * Pointer to AI Model
        * Own.
        */
        CWsfAiModel* iAiModel;
    
        /**
        * AI Controller
        * Own.
        */
        TWsfAiController iController;
        
        /**
        * Pointer to CWsfAiView
        * Own.
        */
        CWsfAiView* iUi;
   
        /**
        * The database observer obseving changes in
        * show WLAN availablity
        */
        CWsfDbObserver* iDbObserver;
        
        /**
        * The resource file offset
        */
        TInt iResourceFileOffset;
    
        /**
        * Constants for Signal Strength text
        * Own.
        */
        HBufC* iCurrentSignalStrength;
    
        /**
        * Constants for Secure Information text
        * Own.
        */
        HBufC* iCurrentSecureInfo;
    
        /**
        * Constants for Title text
        * Own.
        */
        HBufC* iWlanTitle;
    
        /**
        * Constants for MSK activate text
        * Own.
        */
        HBufC* iMskActivate;
    
        /**
        * Scanning status flag
        */
        TBool iScanState;
    
        /**
        * Flag for focus of information area 
        */
        TBool iInfoAreaFocus;
        
        /**
        * The current observer item index 
        */
        TInt iCurrentObserverIndex;
        
        /**
         * The current refreshing state
         */
        TBool iRefreshing;
        
        /**
        * Active Object callback for displaying data at startup
        */
        CAsyncCallBack* iStartupRefresh;
        
        /*
	     * The periodic that steps the refreshing animation
	     */
	    CPeriodic* iAnimationPeriodic;
	    
	    /**
	     * Constants for 'Connected to:' text
	     * Own.
	     */
        HBufC* iConnectedTo;
        
        /**
         * Constants for 'Known network found:' text
         * Own.
         */
        HBufC* iKnownNetworkFound;
	    	    
    };


#endif // C_WSFAIPLUGIN_H

// End of File.
