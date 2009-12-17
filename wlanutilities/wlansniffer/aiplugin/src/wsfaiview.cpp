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
* Description:  Implementation of CWsfAiView
*
*/


//  EXTERNAL INCLUDES
#include <AknsUtils.h>
#include <AknUtils.h>
#include <aknlists.h>
#include <eikfrlbd.h>
#include <eikclbd.h>
#include <wsfaipluginrsc.rsg>
#include <eikimage.h>
#include <eiklabel.h>
#include <gulicon.h>
#include <AknIconUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <gdi.h>
#include <aknlayout.cdl.h>
#include <skinlayout.cdl.h>
#include <AknBidiTextUtils.h>
#include <StringLoader.h>

//  INTERNAL INCLUDES
#include "wsfaiview.h"
#include "wsfaipublishobserver.h"
#include "wsflogger.h"

//  LOCAL DEFINITIONS
// one second in milliseconds
LOCAL_D const TInt KDeferredWaitTime = 1000 * 1000;
LOCAL_D const TInt KAIPublishIconArrayGranularity = 3;

// index value to suppress 1 second refreshing state
LOCAL_D const TInt KSuppressRefreshIndex = 1024;


// --------------------------------------------------------------------------
// CWsfAiView::NewL()
// --------------------------------------------------------------------------
//
CWsfAiView* CWsfAiView::NewL( MWsfAiPublishObserver& aPublishObserver )
    {
    CWsfAiView *thisPtr = NewLC( aPublishObserver );
    CleanupStack::Pop( thisPtr );
    return thisPtr;
    }


// --------------------------------------------------------------------------
// CWsfAiView::NewLC()
// --------------------------------------------------------------------------
//
CWsfAiView* CWsfAiView::NewLC( MWsfAiPublishObserver& aPublishObserver )
    {
    CWsfAiView *thisPtr = new (ELeave) CWsfAiView( aPublishObserver );
    CleanupStack::PushL( thisPtr );
    thisPtr->ConstructL();
    return thisPtr;
    }


// --------------------------------------------------------------------------
// CWsfAiView::~CWsfAiView()
// --------------------------------------------------------------------------
//
CWsfAiView::~CWsfAiView()
    {
    delete iPublishIconArray;
    delete iDeferredUpdateCallBack;
    delete iTextLabel;

    if ( iDeferredUpdater )
        {
        iDeferredUpdater->Cancel();
        delete iDeferredUpdater;
        }
    
    if ( iStatusScanningOff )
        {
        delete iStatusScanningOff;
        }
    if ( iStatusRefreshing )
        {
        delete iStatusRefreshing;
        }
    if ( iStatusConnecting )
        {
        delete iStatusConnecting;
        }
    }


// --------------------------------------------------------------------------
// CWsfAiView::CWsfAiView()
// --------------------------------------------------------------------------
//
CWsfAiView::CWsfAiView( MWsfAiPublishObserver& aPublishObserver ) :
    iUpdateSecureIconDeferred( ETransparentIcon ),
    iUpdateSignalStrengthIconDeferred( ETransparentIcon ),
    iUpdateStatusIconDeferred( ETransparentIcon ),
    iSavedUpdateStatusIconDeferred( ETransparentIcon ),
    iPublishObserver( &aPublishObserver )
    {
    }


// --------------------------------------------------------------------------
// CWsfAiView::ConstructL()
// --------------------------------------------------------------------------
//
void CWsfAiView::ConstructL()
    {
    LOG_ENTERFN( "CWsfAiView::ConstructL" );
    iEnv = CCoeEnv::Static();

    iDeferredUpdateCallBack = new ( ELeave )
        TCallBack( CWsfAiView::DoCompleteUpdateL, this );
    iDeferredUpdater = CPeriodic::NewL( CActive::EPriorityStandard );

    iPublishIconArray = new ( ELeave ) CArrayFixFlat<TInt>( 
                                            KAIPublishIconArrayGranularity );

    iTextLabel = new (ELeave) CEikLabel;
    if ( !iStatusScanningOff )
        {
        iStatusScanningOff = StringLoader::LoadL( 
                R_QTN_SNIFFER_PLUG_IN_SCANNING_OFF );
        }
    iTextLabel->SetTextL( *iStatusScanningOff );

    iUpdateStatusIconDeferred = EWlanOffIcon;
    }


// --------------------------------------------------------------------------
// CWsfAiView::UpdateHotSpotsL()
// --------------------------------------------------------------------------
//
void CWsfAiView::UpdateHotSpotsL( MDesCArray* aItemTextArray,
                                  TInt aCurrentItem )
    {
    LOG_ENTERFN( "CWsfAiView::UpdateHotSpotsL" );
    if ( ParseStringL( aItemTextArray->MdcaPoint( 0 ) ) )
        {
        if ( aCurrentItem != KSuppressRefreshIndex )
            {
            // init the deferred callback
            ActivateRefreshTimer();
            }
        else
            {
            // immediate callback (no Refreshing text)
            DoCompleteUpdateL();
            return;
            }
        }
    if ( !iStatusRefreshing )
        {
        iStatusRefreshing = StringLoader::LoadL( 
                R_QTN_SNIFFER_PLUG_IN_REFRESHING );
        }

    if ( !iStatusConnecting )
        {
        iStatusConnecting = StringLoader::LoadL( 
                R_QTN_SNIFFER_PLUG_IN_CONNECTING );
        }

    TInt resultRefreshing = iTextLabel->Text()->Compare( *iStatusRefreshing );
    TInt resultConnecting = iCurrentText.Compare( *iStatusConnecting );
    
    if ( !resultConnecting )
        {
        MakeTransparentPublishIconsL();  // all icons transparent
        }
    else if ( !resultRefreshing )
        {
        iPublishObserver->SetRefreshingL( ETrue );
        MakeTransparentPublishIconsL();  // all icons transparent
        }
    else
        {
        MakePublishIconsL(); // icons based on the member info
        }

    iPublishObserver->SetScanningState( ETrue );
    iPublishObserver->PublishContentL( iPublishIconArray, iTextLabel );
    }


// --------------------------------------------------------------------------
// CWsfAiView::SelectedItem()
// --------------------------------------------------------------------------
//
TInt CWsfAiView::SelectedItem()
    {
    LOG_ENTERFN( "CWsfAiView::SelectedItem" );
    return 0;
    }


// --------------------------------------------------------------------------
// CWsfAiView::ForceRefreshingL()
// --------------------------------------------------------------------------
//
void CWsfAiView::ForceRefreshingL()
    {
    LOG_ENTERFN( "CWsfAiView::ForceRefreshingL" );
    }

// --------------------------------------------------------------------------
// CWsfAiPlugin::SetRefreshingL
// --------------------------------------------------------------------------
//
void CWsfAiView::StartConnectingAnimationL()
    {
    iPublishObserver->StartConnectingL();    
    }

// --------------------------------------------------------------------------
// CWsfAiView::MultilineControl
// --------------------------------------------------------------------------
//
TBool CWsfAiView::MultilineControl() 
    {
    LOG_ENTERFN( "CWsfAiView::MultilineControl" );
    return EFalse; 
    }
      

// --------------------------------------------------------------------------
// CWsfAiView::ParseStringL()
// --------------------------------------------------------------------------
//
TBool CWsfAiView::ParseStringL( const TDesC& aString )
    {
    LOG_ENTERFN( "CWsfAiView::ParseStringL" );
    // start looking for '\t' digits and parse 
    // the icon identifiers and label text

    // if the data changes - ie - label text has changed 
    // - launch deferred update...
    TChar delimiter('\t');
    TWsfPluginIcons statusIcon;
    TWsfPluginIcons secureicon;
    TWsfPluginIcons strengthIcon;
    TInt firstTabPos = aString.Locate( delimiter );
    TInt secondTabPos = aString.Mid( 
                                firstTabPos +1 ).Locate( 
                                        delimiter ) + firstTabPos + 1;
    TInt thirdTabPos = aString.Mid( 
                                secondTabPos +1 ).Locate( 
                                        delimiter ) + secondTabPos + 1;

    // ok we have the tab positions read the values...

    TInt secVal = 0;
    //read the icons
    TLex( aString.Mid( 0, firstTabPos )).Val( (TInt&)statusIcon );
    TLex( aString.Mid(
        secondTabPos+1 , thirdTabPos - (secondTabPos+1) )
         ).Val( (TInt&) strengthIcon);

    TLex( aString.Mid(
        thirdTabPos+1 , aString.Length() - ( thirdTabPos + 1 ) )
        ).Val( (TInt&) secureicon );
    
    secVal = secureicon;
    HBufC* secureString = NULL;

    iPublishObserver->SetStrengthAndSecure( NULL, secureString );
    if ( secVal == ESecureNetworkIcon )
        {
        delete secureString;
        secureString = NULL;
        }

    // and the label text
    TPtrC labelText = aString.Mid( firstTabPos+1, 
                                   secondTabPos - ( firstTabPos + 1 ) );

    iCurrentText.Copy( labelText );

    if ( !iStatusRefreshing )
        {
        iStatusRefreshing = StringLoader::LoadL( 
                R_QTN_SNIFFER_PLUG_IN_REFRESHING );
        }
    iTextLabel->SetTextL( *iStatusRefreshing );

    iUpdateSecureIconDeferred = secureicon;
    iUpdateSignalStrengthIconDeferred = strengthIcon;
    iUpdateStatusIconDeferred = statusIcon;
    iSavedUpdateStatusIconDeferred = statusIcon;
        
    return ETrue;
    }


// --------------------------------------------------------------------------
// CWsfAiView::DoCompleteUpdateL()
// --------------------------------------------------------------------------
//
void CWsfAiView::DoCompleteUpdateL()
    {
    LOG_ENTERFN( "CWsfAiView::DoCompleteUpdateL" );    
    // cancel the periodic
    iDeferredUpdater->Cancel();

    // update the label text
    iTextLabel->SetTextL( iCurrentText );
    iCurrentText = KNullDesC();
    
    iUpdateStatusIconDeferred = iSavedUpdateStatusIconDeferred;
    
    MakePublishIconsL();
    iPublishObserver->PublishContentL( iPublishIconArray, iTextLabel );
    iPublishObserver->SetRefreshingL( EFalse );
    }


// --------------------------------------------------------------------------
// CWsfAiView::DoCompleteUpdateL()
// --------------------------------------------------------------------------
//
TInt CWsfAiView::DoCompleteUpdateL( TAny* aPtr )
    {
    LOG_ENTERFN( "CWsfAiView::DoCompleteUpdateL" );
    static_cast<CWsfAiView*>( aPtr )->DoCompleteUpdateL();
    return ETrue;
    }


// --------------------------------------------------------------------------
// CWsfAiView::DisplayEngineOffL()
// --------------------------------------------------------------------------
//
void CWsfAiView::DisplayEngineOffL()
    {
    LOG_ENTERFN( "CWsfAiView::DisplayEngineOffL" );
    iDeferredUpdater->Cancel();
    iPublishObserver->SetRefreshingL( EFalse );
    
    if ( !iStatusScanningOff )
        {
        iStatusScanningOff = StringLoader::LoadL( 
                R_QTN_SNIFFER_PLUG_IN_SCANNING_OFF );
        }

    iTextLabel->SetTextL( *iStatusScanningOff );

    iUpdateSecureIconDeferred = ETransparentIcon;
    iUpdateSignalStrengthIconDeferred = ETransparentIcon;
    iUpdateStatusIconDeferred = EWlanOffIcon;
    iPublishObserver->SetScanningState( EFalse );
    MakePublishIconsL();
    iPublishObserver->PublishContentL( iPublishIconArray, iTextLabel );
    }


// --------------------------------------------------------------------------
// CWsfAiView::ActivateRefreshTimer()
// --------------------------------------------------------------------------
//
void CWsfAiView::ActivateRefreshTimer()
    {
    LOG_ENTERFN( "CWsfAiView::ActivateRefreshTimer" );
    iDeferredUpdater->Cancel();
    iDeferredUpdater->Start(
        TTimeIntervalMicroSeconds32( KDeferredWaitTime ),
        TTimeIntervalMicroSeconds32( KDeferredWaitTime ),
        *iDeferredUpdateCallBack );
    }


// --------------------------------------------------------------------------
// CWsfAiView::MakePublishIconsL()
// --------------------------------------------------------------------------
//
void CWsfAiView::MakePublishIconsL()
    {
    LOG_ENTERFN( "CWsfAiView::MakePublishIconsL" );
    if ( iPublishIconArray )
        {
        iPublishIconArray->Reset();
        iPublishIconArray->AppendL( iUpdateStatusIconDeferred );
        iPublishIconArray->AppendL( iUpdateSecureIconDeferred );
        iPublishIconArray->AppendL( iUpdateSignalStrengthIconDeferred );
        }

    }


// --------------------------------------------------------------------------
// CWsfAiView::MakeTransparentPublishIconsL()
// --------------------------------------------------------------------------
//
void CWsfAiView::MakeTransparentPublishIconsL()
    {
    LOG_ENTERFN( "CWsfAiView::MakeTransparentPublishIconsL" );
    TWsfPluginIcons tmp2, tmp3;

    tmp2 = iUpdateSecureIconDeferred;
    tmp3 = iUpdateSignalStrengthIconDeferred;

    iUpdateSecureIconDeferred = ETransparentIcon;
    iUpdateSignalStrengthIconDeferred = ETransparentIcon;

    MakePublishIconsL();

    iUpdateSecureIconDeferred = tmp2;
    iUpdateSignalStrengthIconDeferred = tmp3;
    }

void CWsfAiView::UpdateViewL( MDesCArray* aItemTextArray )
	{
	LOG_ENTERFN( "CWsfAiView::UpdateViewL" );
	TChar delimiter('\t');
    TWsfPluginIcons statusIcon;
    TWsfPluginIcons secureicon;
    TWsfPluginIcons strengthIcon;
    const TDesC& aString = aItemTextArray->MdcaPoint( 0 ) ;
    
    TInt firstTabPos = aString.Locate( delimiter );
    TInt secondTabPos = aString.Mid( 
                                firstTabPos +1 ).Locate( 
                                        delimiter ) + firstTabPos + 1;
    TInt thirdTabPos = aString.Mid( 
                                secondTabPos +1 ).Locate( 
                                        delimiter ) + secondTabPos + 1;
    
        //read the icons
    TLex( aString.Mid( 0, firstTabPos )).Val( (TInt&)statusIcon );
    TLex( aString.Mid(
        secondTabPos+1 , thirdTabPos - (secondTabPos+1) )
         ).Val( (TInt&) strengthIcon);

    TLex( aString.Mid(
        thirdTabPos+1 , aString.Length() - ( thirdTabPos + 1 ) )
        ).Val( (TInt&) secureicon );
            
    iUpdateStatusIconDeferred = statusIcon;
    
    MakeTransparentPublishIconsL();
    
	iPublishObserver->PublishStatusIconL( iPublishIconArray, iTextLabel );
	}

// END OF FILE
