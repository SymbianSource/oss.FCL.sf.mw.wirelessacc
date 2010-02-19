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
#include <hscontentpublisher.h>
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
NONSHARABLE_CLASS( CWsfAiPlugin ): public CHsContentPublisher,
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
        static TInt CWsfAiPlugin::DoRefreshingStepL( TAny* ptr );
        
        /**
        * Steps the refresh animation
        */                
        void CWsfAiPlugin::DoRefreshingStepL();
        
        /**
        * Steps 
        * @param ptr Pointer to an instance of this class
        */
        static TInt CWsfAiPlugin::DoConnectingStepL( TAny* ptr );
        
        /**
        * Steps the refresh animation
        */                
        void CWsfAiPlugin::DoConnectingStepL();
    
    public:     // from CHSContentPublisher
        
        /**
         * @see CHsContentPublisher
         */
        void Start( TStartReason aReason );

        /**
         * @see CHsContentPublisher
         */
        void Stop( TStopReason aReason );

        /**
         * @see CHsContentPublisher
         */    
        void Resume( TResumeReason aReason );

        /**
         * @see CHsContentPublisher
         */    
        void Suspend( TSuspendReason aReason );
        
        /**
         * @see CHsContentPublisher
         */
        void SubscribeL( MAiContentObserver& aObserver );
    
        /**
         * @see CHsContentPublisher
         */
        void ConfigureL( RAiSettingsItemArray& aSettings );
    
        /**
         * @see CHsContentPublisher
         */
        TAny* GetProperty( TProperty aProperty );

        /**
         * @see CHsContentPublisher
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
