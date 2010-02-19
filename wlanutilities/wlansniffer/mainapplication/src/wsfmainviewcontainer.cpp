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
* Description:  Implementation of CWsfMainViewContainer.
*
*/

//  EXTERNAL INCLUDES
#include <AknUtils.h>
#include <aknlists.h>
#include <badesca.h>
#include <wlansniffer.rsg>
#include <wsficons.mbg>
#include <AknIconUtils.h>
#include <AknsUtils.h>
#include <gulicon.h> 
#include <eikclbd.h> 
#include <aknnavi.h>
#include <aknnavide.h>
#include <StringLoader.h>
#include <eikenv.h> 
#include <csxhelp/sniffer.hlp.hrh>


//  INTERNAL INCLUDES
#include "wsfmainviewcontainer.h"
#include "wsfmainviewinternals.h"
#include "wsfmainviewcontrollerpartner.h"
#include "wsfdocument.h"
#include "wsfwlaninfoarrayvisitor.h"
#include "wsfappui.h"
#include "wsfmainview.h"
#include <wlansniffer.rsg>

#include "wsflogger.h"



//  LOCAL DEFINITIONS
const TUint KIconListGranularity = 12; // Default size of the icons list
// path of the app's iconfile
_LIT( KIconFileName, "\\resource\\apps\\wsficons.mif" ); 

const TInt KConnectingStepTime = 200 * 1000;

_LIT( KFormat1Icon, "%d" );


//  CONSTRUCTION AND DESTRUCTION
// ---------------------------------------------------------
// CWsfMainViewContainer::NewL
// ---------------------------------------------------------
//
CWsfMainViewContainer* CWsfMainViewContainer::NewL( const TRect& aRect,
        CWsfMainView* aParentView )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::NewL" );
    CWsfMainViewContainer* self =
            CWsfMainViewContainer::NewLC( aRect, aParentView );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::NewLC
// ---------------------------------------------------------
//
CWsfMainViewContainer* CWsfMainViewContainer::NewLC( const TRect& aRect,
        CWsfMainView* aParentView )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::NewLC" );
    CWsfMainViewContainer* self = new( ELeave ) CWsfMainViewContainer;
    CleanupStack::PushL( self );
    self->ConstructL( aRect, aParentView );
    return self;
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::~CWsfMainViewContainer
// ---------------------------------------------------------
//
CWsfMainViewContainer::~CWsfMainViewContainer()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::~CWsfMainViewContainer" );
    
    StopConnectingAnimation();
    
    PopNaviPane();
    
    if ( iStatusConnecting )
        {
        delete iStatusConnecting;
        }
    
    delete iListBox;
    delete iNaviDecorator;
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::CWsfMainViewContainer
// ---------------------------------------------------------
//
CWsfMainViewContainer::CWsfMainViewContainer()
    {
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::ConstructL
// ---------------------------------------------------------
//
void CWsfMainViewContainer::ConstructL( const TRect& aRect,
        CWsfMainView* aParentView )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::ConstructL" );
    CreateWindowL();
    
    iParentView = aParentView;
   
    iListBox = new( ELeave ) CAknDoubleGraphicStyleListBox();
    TInt flags( EAknGenericListBoxFlags );
    iListBox->SetMopParent(this); // 
    iListBox->ConstructL( this, flags );    
    iListBox->SetContainerWindowL(*this); 

    iListBox->SetFocus(ETrue);
        
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
                                               CEikScrollBarFrame::EOff,
                                               CEikScrollBarFrame::EAuto );

    SetListboxIconsL();
    
    iConnectingIcons[0] = KWlanOnIcon_0;
    iConnectingIcons[1] = KWlanOnIcon_1;
    iConnectingIcons[2] = KWlanOnIcon;
    iConnectingIcons[3] = KWlanOnIcon_1;

    SetRect( aRect );
    iListBox->SetRect( Rect() );
    ActivateL();    
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::SizeChanged
// ---------------------------------------------------------
//
void CWsfMainViewContainer::SizeChanged()
    {
    iListBox->SetRect( Rect() );
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::CountComponentControls
// ---------------------------------------------------------
//
TInt CWsfMainViewContainer::CountComponentControls() const
    {
    return 1;// return number of controls inside this container
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::ComponentControl
// ---------------------------------------------------------
//
CCoeControl* CWsfMainViewContainer::ComponentControl(TInt aIndex) const
    {
    CCoeControl* object = NULL;
    switch ( aIndex )
        {
        case 0:
            object = iListBox;
            break; 
        default:
            object = NULL;
        }
    return object;
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::OfferKeyEventL
// ---------------------------------------------------------
//
TKeyResponse CWsfMainViewContainer::OfferKeyEventL( 
                                                const TKeyEvent& aKeyEvent,
                                                TEventCode aType )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::OfferKeyEventL" );
    CWsfAppUi* appui = static_cast<CWsfAppUi*>( iEikonEnv->AppUi() );
    
    if ( appui->SuppressingKeyEvents() )
        {
        return EKeyWasConsumed;
        }
    
    TKeyResponse result( EKeyWasNotConsumed );
    if ( aType == EEventKey || aType == EEventKeyDown )
        {
        TInt index = iListBox->CurrentItemIndex();        
        if( ( aKeyEvent.iCode == EKeyBackspace ) && ( index != -1 ) )
            {
            result =  EKeyWasConsumed;
            }
        else
            {
            result = iListBox->OfferKeyEventL( aKeyEvent, aType );

            
            if( aKeyEvent.iCode == EKeyUpArrow )
                {
                if( iPartner )
                    {
                    iPartner->HandleUpKeyPressedL();
                    }
                }   
            else if( aKeyEvent.iCode == EKeyDownArrow )
                {
                if( iPartner )
                    {
                    iPartner->HandleDownKeyPressedL();
                    }
                }
           else if( aKeyEvent.iCode == EKeyOK )
                {
                if( iPartner )
                    {
                    iPartner->HandleSelectionKeyPressedL();
                    }
  
                }
           }
        }
    return result;
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::LoadGraphicsL
// ---------------------------------------------------------
//
CAknIconArray* CWsfMainViewContainer::LoadGraphicsL()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::LoadGraphicsL" );   
    CAknIconArray* icons = new(ELeave) CAknIconArray( KIconListGranularity );
    CleanupStack::PushL( TCleanupItem( CleanUpResetAndDestroy,icons) );
    
    
    //KWlanOnIcon
    icons->InsertL( KWlanOnIcon, 
            LoadSkinnableIconLC( KAknsIIDQgnIndiWlanSnifferPluginOn, 
                    EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on) );
    CleanupStack::Pop(); /*icon*/
    
    //KWlanOnIcon_0
    icons->InsertL( KWlanOnIcon_0, 
            LoadSkinnableIconLC( KAknsIIDQgnIndiWlanSnifferPluginOn0, 
                    EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on_0) );
    CleanupStack::Pop(); /*icon*/
    
    //KWlanOnIcon_0
    icons->InsertL( KWlanOnIcon_1, 
            LoadSkinnableIconLC( KAknsIIDQgnIndiWlanSnifferPluginOn1, 
                    EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on_1) );
    CleanupStack::Pop(); /*icon*/
    
    //ESecureNetworkIcon
    icons->InsertL( KSecureNetworkIcon, LoadSkinnableBlackIconLC( 
            KAknsIIDQgnIndiWlanSecureNetworkAdd, 
            EMbmWsficonsQgn_indi_wlan_secure_network_add) );
    
    CleanupStack::Pop(); /*icon*/

    //ENoSignalIcon
    icons->InsertL( KNoSignalIcon, LoadIconLC( 
            EMbmWsficonsEmpty13x13) );
    CleanupStack::Pop(); /*icon*/

    //EPoorSignal
    icons->InsertL( KPoorSignal, LoadSkinnableBlackIconLC( 
            KAknsIIDQgnIndiWlanSignalLowAdd, 
            EMbmWsficonsQgn_indi_wlan_signal_low_add) );
    
    CleanupStack::Pop(); /*icon*/

    //EAverageSignal
    icons->InsertL( KAverageSignal, LoadSkinnableBlackIconLC( 
            KAknsIIDQgnIndiWlanSignalMedAdd, 
            EMbmWsficonsQgn_indi_wlan_signal_med_add) );
    CleanupStack::Pop(); /*icon*/

    //EExcelentSignal
    icons->InsertL( KExcelentSignal, LoadSkinnableBlackIconLC( 
            KAknsIIDQgnIndiWlanSignalGoodAdd , 
            EMbmWsficonsQgn_indi_wlan_signal_good_add) );
    CleanupStack::Pop(); /*icon*/

    //EConnectedNWIcon
    icons->InsertL( KConnectedNWIcon, 
            LoadSkinnableIconLC( KAknsIIDQgnPropCmonWlanConn, 
                    EMbmWsficonsQgn_prop_cmon_wlan_conn) );
    
    CleanupStack::Pop(); /*icon*/

    //EKnownNWIcon
    icons->InsertL( KKnownNWIcon, LoadSkinnableIconLC( 
            KAknsIIDQgnPropWlanBearer, 
            EMbmWsficonsQgn_prop_wlan_bearer) );
    CleanupStack::Pop(); /*icon*/

    //ETransparentIcon
    icons->InsertL( KTransparentIcon, LoadIconLC( 
            EMbmWsficonsQgn_prop_empty) );
    CleanupStack::Pop(); /*icon*/
     
    CleanupStack::Pop( icons );    
    return icons;   
    }



// ---------------------------------------------------------
// CWsfMainViewContainer::CleanUpResetAndDestroy
// ---------------------------------------------------------
//
void CWsfMainViewContainer::CleanUpResetAndDestroy( TAny* aArray )
    {
    if ( aArray )
        {
        CArrayPtrFlat<CBase>* array = static_cast<CArrayPtrFlat<CBase>*>( 
                                                                    aArray );
        array->ResetAndDestroy();
        delete array; 
        }
    }


// ---------------------------------------------------------
// CWsfMainViewContainer::LoadIconLC
// ---------------------------------------------------------
//
CGulIcon* CWsfMainViewContainer::LoadIconLC( TInt aIconId)
    {
    LOG_ENTERFN( "CWsfMainViewContainer::LoadIconLC" );   
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap;   
    CFbsBitmap* mask;   
   
    // Creates bitmap an icon.
    AknIconUtils::CreateIconL( bitmap, 
                               mask,
                               KIconFileName,
                               aIconId,
                               aIconId + 1 );

    icon->SetBitmap( bitmap );   
    icon->SetMask( mask ); 
    return icon;    
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::LoadBlackIconLC
// ---------------------------------------------------------
//
CGulIcon* CWsfMainViewContainer::LoadBlackIconLC( TInt aIconId )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::LoadBlackIconLC" );   
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap;   
    CFbsBitmap* mask;   
   
    // Creates bitmap an icon.

     AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(), 
                             KAknsIIDNone, 
                             KAknsIIDQsnIconColors, EAknsCIQsnIconColorsCG13, 
                             bitmap, 
                             mask, 
                             KIconFileName,
                             aIconId, 
                             aIconId+1,
                             KRgbBlack );



    icon->SetBitmap( bitmap );   
    icon->SetMask( mask ); 
    return icon;            
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::LoadSkinnableIconLC
// ---------------------------------------------------------
//
CGulIcon* CWsfMainViewContainer::LoadSkinnableIconLC( 
                                    const TAknsItemID aAknItemId, 
                                    const TInt aIconId )
    {
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap;
    CFbsBitmap* mask;

    // Creates bitmap an icon.
    
    AknsUtils::CreateIconL( AknsUtils::SkinInstance(),
                            aAknItemId,
                            bitmap,
                            mask,
                            KIconFileName,
                            aIconId,
                            aIconId+1 );

    icon->SetBitmap( bitmap );
    icon->SetMask( mask );
    return icon;
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::LoadSkinnableBlackIconLC
// ---------------------------------------------------------
//
CGulIcon* CWsfMainViewContainer::LoadSkinnableBlackIconLC( 
                                    const TAknsItemID aAknItemId, 
                                    const TInt aIconId )
    {
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap;
    CFbsBitmap* mask;

    // Creates bitmap an icon.
    
    AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(), 
                            aAknItemId, 
                            KAknsIIDQsnIconColors, EAknsCIQsnIconColorsCG13, 
                            bitmap, 
                            mask, 
                            KIconFileName,
                            aIconId, 
                            aIconId+1,
                            KRgbBlack );

    icon->SetBitmap( bitmap );
    icon->SetMask( mask );
    return icon;
    }



// ---------------------------------------------------------
// CWsfMainViewContainer::UpdateHotSpotsL
// ---------------------------------------------------------
//
void CWsfMainViewContainer::UpdateHotSpotsL( MDesCArray* aItemTextArray,
                                             TInt aCurrentItem )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::UpdateHotSpotsL" );  
    StopConnectingAnimation();
    iListBox->Reset();
    iListBox->Model()->SetItemTextArray( aItemTextArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    iListBox->HandleItemAdditionL(); 
    
    if ( aItemTextArray->MdcaCount() )
        {
        TUint lastIndex = aItemTextArray->MdcaCount() -1;
        if ( aCurrentItem <= lastIndex )
            {
            iListBox->SetCurrentItemIndex( aCurrentItem );            
            }
        else //( aCurrentItem > lastIndex ) 
            {        
            iListBox->SetCurrentItemIndex( lastIndex );
            }
        TPtrC itemtext = iListBox->Model()->ItemText(0);
        HBufC* text = itemtext.AllocLC();
        TPtr buffer = text->Des();
        if ( ParseStringForAnimationL( buffer ) )
            {
            //Get the list box items model.        
            MDesCArray* listArray = iListBox->Model()->ItemTextArray();        
            CDesCArray* itemArray = static_cast<CDesCArray*>(listArray);         
            itemArray->Delete(0,1); 
            itemArray->InsertL(0,buffer);
            LOG_WRITE("Connecting status found");
            StartConnectingAnimationL();
            }
        CleanupStack::PopAndDestroy(text);
        }
    
    iParentView->UpdateBrowserUsageInfoL();
    
    iListBox->DrawDeferred(); 
    }


// --------------------------------------------------------------------------
// CWsfAiView::ParseStringForAnimationL()
// --------------------------------------------------------------------------
//
TBool CWsfMainViewContainer::ParseStringForAnimationL( TDes& aString )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::ParseStringForAnimationL" );
    TBool found = EFalse;
    // start looking for '\t' digits and parse 
    // the icon identifiers and label text
    
    // L"\tOther (hidden netw.)"
    // L"6\tAd-hoc WPA\tConnecting\t4\t0" - this we are trying to find

    TChar delimiter('\t');
    TInt firstTabPos = aString.Locate( delimiter );
    TInt secondTabPos = aString.Mid( 
                                firstTabPos +1 ).Locate( 
                                        delimiter ) + firstTabPos + 1;
    TInt thirdTabPos = aString.Mid( 
                                secondTabPos +1 ).Locate( 
                                        delimiter ) + secondTabPos + 1;

    // ok we have the tab positions read the values...
    TInt firstIcon;
    if ( firstTabPos )
        {
        TLex( aString.Mid( 0, firstTabPos )).Val( (TInt&)firstIcon );
        LOG_WRITEF( "First Icon %d", firstIcon );
        }
    
    if ( secondTabPos && thirdTabPos )
        {
        HBufC * foundString = aString.Mid( secondTabPos+1, 
                                       thirdTabPos-secondTabPos-1 ).AllocLC();
        
        if ( !iStatusConnecting )
            {
            iStatusConnecting = StringLoader::LoadL(
                    R_QTN_SNIFFER_CONNECTING );
            }
        
        if ( !foundString->Compare( iStatusConnecting->Des() ) )
            {
            HBufC* item = HBufC::NewLC( KFormat1Icon().Length() );
            item->Des().Format( KFormat1Icon, iConnectingIcons[iConnectingIndex] );
            LOG_WRITEF( "Replace Icon %d", iConnectingIcons[iConnectingIndex] );
            
            aString.Replace(0, firstTabPos, item->Des() );
            found = ETrue;
            CleanupStack::PopAndDestroy( item );
            }
        CleanupStack::PopAndDestroy( foundString );
        }

    return found;
    }


// --------------------------------------------------------------------------
// CWsfMainViewContainer::StartConnectingAnimationL
// --------------------------------------------------------------------------
//
void CWsfMainViewContainer::StartConnectingAnimationL()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::StartConnectingAnimationL" );
    
    if ( iAnimationPeriodic )
        {
        iAnimationPeriodic->Cancel();
        delete iAnimationPeriodic;
        iAnimationPeriodic = NULL;
        }

    InitializeConnectingAnimation();
    iAnimationPeriodic = CPeriodic::NewL(
                              CActive::EPriorityLow );
                
    iAnimationPeriodic->Start(
            TTimeIntervalMicroSeconds32( KConnectingStepTime ),
            TTimeIntervalMicroSeconds32( KConnectingStepTime ),
            TCallBack( 
                    CWsfMainViewContainer::DoConnectingAnimationStepL,
                    this
                    ) );
    }


// --------------------------------------------------------------------------
// CWsfMainViewContainer::StopConnectingAnimation
// --------------------------------------------------------------------------
//
void CWsfMainViewContainer::StopConnectingAnimation()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::StopConnectingAnimation" );
    
    if ( iAnimationPeriodic )
        {
        iAnimationPeriodic->Cancel();
        delete iAnimationPeriodic;
        iAnimationPeriodic = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CWsfAiPlugin::DoConnectingAnimationStepL
// ---------------------------------------------------------------------------
//
TInt CWsfMainViewContainer::DoConnectingAnimationStepL( TAny* ptr )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::DoConnectingAnimationStepL" );
    static_cast<CWsfMainViewContainer*>( ptr )->DoConnectingAnimationStepL();
        return ETrue;
    }

// ---------------------------------------------------------------------------
// CWsfAiPlugin::DoConnectingAnimationStepL
// ---------------------------------------------------------------------------
//
void CWsfMainViewContainer::DoConnectingAnimationStepL()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::DoConnectingAnimationStepL" );
    AnimateConnecting();
    if ( iListBox->Model()->ItemTextArray()->MdcaCount() )
        {
        TPtrC itemtext = iListBox->Model()->ItemText(0);
        HBufC* text = itemtext.AllocLC();
        TPtr buffer = text->Des();
        if ( ParseStringForAnimationL( buffer ))
            {
            //Get the list box items model.        
            MDesCArray* listArray = iListBox->Model()->ItemTextArray();        
            CDesCArray* itemArray = static_cast<CDesCArray*>(listArray);         
            itemArray->Delete(0,1); 
            itemArray->InsertL(0,buffer);
            iListBox->HandleItemAdditionL(); 
            iListBox->DrawDeferred(); 
            LOG_WRITE("Connecting status found");
            }
        else
            {
            StopConnectingAnimation();
            }
        CleanupStack::PopAndDestroy( text );
        }
    }


// --------------------------------------------------------------------------
// CWsfAiModel::InitializeRefreshAnimation
// --------------------------------------------------------------------------
//
void CWsfMainViewContainer::InitializeConnectingAnimation()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::InitializeConnectingAnimation" );
    iConnectingIndex = 0;
    }


// --------------------------------------------------------------------------
// CWsfAiModel::AnimateConnecting
// --------------------------------------------------------------------------
//
void CWsfMainViewContainer::AnimateConnecting()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::AnimateConnecting" );
    ++iConnectingIndex %= iConnectingIcons.Count();
    }


// ---------------------------------------------------------
// CWsfMainViewContainer::SelectedItem
// ---------------------------------------------------------
//
TInt CWsfMainViewContainer::SelectedItem()
    {
    return iListBox->CurrentItemIndex();
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::HandleResourceChange
// ---------------------------------------------------------
//
void CWsfMainViewContainer::HandleResourceChange( TInt aType )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::HandleResourceChange" );   
    CCoeControl::HandleResourceChange( aType );
    
    if ( aType == KAknsMessageSkinChange )
        {
        TRAP_IGNORE( SetListboxIconsL() );
        SizeChanged();
        }
        
    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, 
                                           mainPaneRect );        
        SetRect( mainPaneRect );                
        DrawNow();                
        }        
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::SetPartner
// ---------------------------------------------------------
//    
void CWsfMainViewContainer::SetPartner( 
                                    MWsfMainViewControllerPartner& aPartner )
    {
    iPartner = &aPartner;
    }
    
// ---------------------------------------------------------
// CWsfMainViewContainer::UpdateNaviPaneL
// ---------------------------------------------------------
//
void CWsfMainViewContainer::UpdateNaviPaneL( const TDesC& aPaneText )
    {
    LOG_ENTERFN( "CWsfMainViewContainer::UpdateNaviPaneL" );   
    if ( !( ( CWsfAppUi *)( iEikonEnv->AppUi() ) )->Foreground() )
        {
        return;
        }
        
    GetPanesL();
    
    if ( iNaviDecorator )
        {
        PopNaviPane();
        }
        
    iNaviDecorator = iNaviPane->CreateNavigationLabelL( aPaneText );
    iNaviDecorator->MakeScrollButtonVisible( ETrue );
    iNaviPane->PushL( *iNaviDecorator );    
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::ListBox
// ---------------------------------------------------------
//
CAknDoubleGraphicStyleListBox* CWsfMainViewContainer::ListBox() const
    {
    return iListBox;
    }    

// ---------------------------------------------------------
// CWsfMainViewContainer::PopNaviPane
// ---------------------------------------------------------
//
void CWsfMainViewContainer::PopNaviPane()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::PopNaviPane" );   
    if ( iNaviPane && iNaviDecorator )
        {        
        iNaviPane->Pop( iNaviDecorator );
        delete iNaviDecorator;
        iNaviDecorator = NULL;
        }
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::GetPanesL
// ---------------------------------------------------------
//
void CWsfMainViewContainer::GetPanesL()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::GetPanesL" );   
    if( !iNaviPane )
        {
        CEikStatusPane* statusPane = iEikonEnv->AppUiFactory()->StatusPane();

        iNaviPane =  static_cast<CAknNavigationControlContainer*>(
                        statusPane->ControlL( 
                                TUid::Uid(EEikStatusPaneUidNavi ) ) );
        }
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::SetListboxIconsL
// ---------------------------------------------------------
//
void CWsfMainViewContainer::SetListboxIconsL()
    {
    LOG_ENTERFN( "CWsfMainViewContainer::SetListboxIconsL" );   
    CAknIconArray* icons = LoadGraphicsL();

    CWsfDocument* doc = static_cast<CWsfDocument*>
            ( iEikonEnv->EikAppUi()->Document() );

    doc->WlanInfoBranding().AppendBrandingIconsL( *icons );
    
    iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
    
    }

// ---------------------------------------------------------
// CWsfMainViewContainer::GetHelpContext
// ---------------------------------------------------------
//
void CWsfMainViewContainer::GetHelpContext( TCoeHelpContext& aContext) const
    {
    aContext.iMajor = TUid::Uid( 0x10281CAA );
    aContext.iContext = KSNIFFER_HLP_MAIN;
    }

    
// End of file
