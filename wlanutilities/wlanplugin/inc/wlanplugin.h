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
* Description: 
*        Header file for CWlanPlugin class.
*
*/

#ifndef __C_WLANPLUGIN_H__
#define __C_WLANPLUGIN_H__

// System includes
#include <gsplugininterface.h>
#include <ConeResLoader.h>
#include <AknServerApp.h>

//forward declarations
class CAknNavigationDecorator;
class CAknViewAppUi;
class CAknNullService;
class CGSParentPlugin;
class CWlanPluginWlanState;

/**
* WLAN plugin's UID
*/
const TUid KWlanPluginUid = { 0x10281BCA };

/**
* WlanSniffer applicatoin's UID
*/
const TUid KSnifferAppUid = { 0x10281CAA };

/**
* WLAN plugin class.
*/
class CWlanPlugin : public CGSPluginInterface,
                    public MAknServerAppExitObserver 
    {
    public://Constructors & destructors

        /**
        * First phase constructor
        * Creates new instance of CWlanPlugin 
        * @param aInitParams Initialising parameters
        * @return a plugin instance
        */ 
        static CWlanPlugin* NewL( TAny* aInitParams );
                        
        /**
        * Destructor
        */
        ~CWlanPlugin();
        
    public: // Functions from base class
        
        /**
        * From CAknView
        */
        
        /**
        * Returns the Plugin's UID
        * @return UID of the plugin
        */
        TUid Id() const;
             
        /*
        * Funtion called when This is activated.
        * @param aPrevViewId The previous View Id
        * @param aCustomMessageId Id of a custom message
        * @param aCustomMessage Content of the custom message
        */
        void DoActivateL( const TVwsViewId& aPrevViewId, 
                          TUid aCustomMessageId, 
                          const TDesC8& aCustomMessage );
        
        /*
        * Funtion called when this is deactivated.
        */
        void DoDeactivate();

    public:
        
        /**
        * from CGSPluginInterface
        */

        /*
        * Called by the CP framework. This returns the
        * caption of the plugin, the first line of the listbox
        * element of it, shown in the Control Panel UI.
        * @param aCaption The caption is copied here
        */
        void GetCaptionL( TDes& aCaption ) const;

        /*
        * Returns if it has bitmap.
        * @return The logical value of having birmap
        */
        TBool HasBitmap() const;
        
        /*
        * Returns a bitmap.
        * @param aBitmap the returned bitmap
        * @param aMask the returned mask of the bitmap
        */
        void GetBitmapL( CFbsBitmap* aBitmap, CFbsBitmap* aMask ) const;
        
        
        /*
        * Returns a the plugin provider's cathegory
        * @return The id of the cathegory
        */
        TInt PluginProviderCategory() const;
        
        /*
        * This function is called, when the plugin is selected.
        * In this case it starts sniffer application
        * @param aSelectionType The type of the selection
        */
        void HandleSelection( const TGSSelectionTypes aSelectionType );
          
        /*
        * Returns the type of the item.
        * @return The type of the item
        */
        TGSListboxItemTypes ItemType();
        
        /*
        * Creates the asked type icon (main icon or D column icon)
        * and returns a pointer to it.
        * @param aIconType Type of the icon
        * @return the asked icon
        */
        CGulIcon* CreateIconL( const TUid aIconType );
        
        /**
        * Method for checking, if plugin should be visible and used in GS FW.
        * (for example shown in listbox of the parent view).
        *
        * On default plugin is visible. Overwrite this function to enable or
        * disable your plugin dynamically.
        *
        * @return ETrue if plugin should be visible in GS.
        * @return EFalse if plugin should not be visible in GS.
        */
        virtual TBool Visible() const;
        
        /*
        * The aKey identified value is given back by it. Actually
        * the the second line of the listbox element of this plugin
        * is given by this function.
        * @param aKey The id of the asked value.
        * @param aValue The returned value.
        */
        void GetValue( const TGSPluginValueKeys aKey,
                          TDes& aValue );  
        
        
        void HandleServerAppExit(TInt aReason);
        
        
    protected:
    
        /**
        * Constructor
        */
        CWlanPlugin();

    private:
        
        /**
        * Second phase constructor
        */
        TAny ConstructL();
               
        /**
        * Opens the localized resource file 
        */
        void OpenLocalizedResourceFileL();
        
        /**
        * Launches Sniffer application 
        */
        void LaunchSnifferAppL();
              
        /**
        * Queries if wlan is supported
        * @return if the wlan is supported
        */
        TBool GetWlanSupported() const;
        
        /**
        * Shows an error note in case of error. 
        * @param aErrorCode The error code.
        */
        void ShowErrorNote(TInt aErrorCode);

        /**
        * Shows the error note and resolves the error.
        * @param aErrorCode The error code.
        */
        TBool ShowErrorNoteL(TInt aErrorCode);
    
        /**
        * Shows a general error note
        */
        void ShowGeneralErrorNoteL();
         
    private: //data
        
        /*
        * @var resource loader
        */
        RConeResourceLoader iResources;
        
        /*
        * @var null service
        */
        CAknNullService* iNullService;
        
        /*    
        * @var stores the present status line    
        */
        HBufC* iStatusLine;

        /*
        * @var wlan state handler/observer
        */
        CWlanPluginWlanState* iWlanState;       
        
    };

#endif //__C_WLANPLUGIN_H__

//End of file