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
* Description:   Declaration of HsBrowserModel class.
*
*/



#ifndef HSBROWSERMODEL_H
#define HSBROWSERMODEL_H

// INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include "hssinterface.h"

// FORWARD DECLARATIONS
class RConnection;
class RSocketServ;
class RHssInterface;

// CLASS DECLARATION
/**
 * Manages the HsBrowser data.
 */
class CHsBrowserModel : public CBase
    {

    public:
    
        /**
        * Static constructor.
        */
        static CHsBrowserModel* NewL();
        
        /**
        * Destructor.
        */
        ~CHsBrowserModel();
    
    public: // new methods
            
        /**
        * Set model state.
        *
        * @param aState State of the model.
        * @param aSendToHss Flag for sending status to hotspot server.
        */
        void SetState( TUint aState, TBool aSendToHss );
        
        /**
        * Attach to connection using known iIapId and iNetId.
        */
        TInt Attach();
        
    public: // access
    
        inline TInt IapId() const;        
        inline TInt NetId() const;
        inline HBufC* Url();
        inline RConnection& Connection();
        inline RSocketServ& SocketServ();
        inline RHssInterface& HsServer();
        inline TBool Foreground();
        inline TUint State();
        inline TBool LoginCompleteNoteShown(); 
        inline TInt Position();
        inline TInt Priority();
        inline HBufC* StaticTitle();
        inline HBufC* PageTitle();
        
        inline void SetNetId( TInt aNetId );
        inline void SetIapId( TInt aIapId ); 
        inline void SetUrl( const TPtrC& aUrl );
        inline void SetState( TUint aState );
        inline void SetForeground( TBool aForeground );
        inline void SetLoginCompleteNoteShown( TBool aLoginCompleteNoteShown ); 
        inline void SetStaticTitle( const TDesC& aStaticTitle );
        inline void SetPageTitle( const TDesC& aPageTitle );
        
    private:
    
        /**
        * C++ default constructor.
        */
        CHsBrowserModel();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // data
		
        /**
        * IAP ID.
        */
        TInt iIapId;

        /**
        * Network ID.
        */
        TInt iNetId;
        
        /**
        * URL of web page for user authentication.
        * Owned.
        */
        HBufC* iUrl;
        
        /**
        * The management interface for a network connection.
        */
        RConnection iConnection;
        
        /**
        * Provides the Connect() function to create an IPC communication
        * channel to the socket server.
        */
        RSocketServ iSocketServ;
        
        /**
        * Handle to a session with a hotspot server.
        */
        RHssInterface iHsServer;
        
        /**
        * Application state. Possible values are EHsBrowserUiUndefined, EHsBrowserUiRunning,
        * EHsBrowserUiAuthenticatedOk, EHsBrowserUiAuthenticatedNok, EHsBrowserUiClosed, defined
        * in hotspotclientserver.h
        */       
        TUint iState;
        
        /**
        * Boolean flag for foreground state.
        */
        TBool iForeground;
        
        /**
        * Boolean flag for showing login complete note.
        */
        TBool iLoginCompleteNoteShown;
        
        /**
        * The window's original ordinal position.
        */
        TInt iPosition;
        
        /**
        * The window's original ordinal priority. 
        */
        TInt iPriority;

        /**
        * Default page title, "WLAN login" 
        * Owned.
        */       
        HBufC* iStaticTitle;

        /**
        * Web page title, used instead of default if available. 
        * Owned.
        */               
        HBufC* iPageTitle;
        
    };

#include "hsbrowsermodel.inl"

#endif // HSBROWSERMODEL_H

// end of file

