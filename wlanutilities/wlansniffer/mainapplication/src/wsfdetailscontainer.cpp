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
* Description:  Implementation of CWsfDetailsContainer.
*
*/

// INCLUDE FILES
#include <aknnavi.h>
#include <aknnavide.h>
#include <wlansniffer.rsg>
#include <aknnavilabel.h>
#include "wsfdetailscontainer.h"
#include "wsfdetailsviewcontrollerpartner.h"
#include "wsfappui.h"
#include "wsfdetailsviewmodel.h"
#include "wsfactivetimeupdater.h"

#include "wsflogger.h"

// CONSTANTS
LOCAL_D const TInt KUpdateInterval = 10000000; // in micro seconds (10 sec)


// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------
// CCSnifferDetailsContainer::NewL()
// ---------------------------------------------------------
//
CWsfDetailsContainer* CWsfDetailsContainer::NewL( 
                                        const TRect& aRect,
                                        MWsfActiveTimerHandler& aController )
    {
    LOG_ENTERFN( "CWsfDetailsContainer::NewL" );
    CWsfDetailsContainer* self = CWsfDetailsContainer::NewLC( aRect, 
                                                              aController );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CCSnifferDetailsContainer::NewLC()
// ---------------------------------------------------------
//
CWsfDetailsContainer* CWsfDetailsContainer::NewLC( 
                                        const TRect& aRect,
                                        MWsfActiveTimerHandler& aController )
    {
    LOG_ENTERFN( "CWsfDetailsContainer::NewLC" );
    CWsfDetailsContainer* self = new( ELeave ) CWsfDetailsContainer;
    CleanupStack::PushL( self );
    self->ConstructL( aRect, aController );
    return self;
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::~CCSnifferDetailsContainer
// ---------------------------------------------------------
//
CWsfDetailsContainer::~CWsfDetailsContainer()
    {
    LOG_ENTERFN( "CWsfDetailsContainer::~CWsfDetailsContainer" );
    PopNaviPane();
    delete iListBox;
    delete iNaviDecorator;
    
    if( iActiveUpdater )
        {
        iActiveUpdater->Stop();
        }
    delete iActiveUpdater;
    }

// ---------------------------------------------------------
// CCSnifferDetailsContainer::ConstructL()
// ---------------------------------------------------------
//
void CWsfDetailsContainer::ConstructL( const TRect& aRect, 
                                       MWsfActiveTimerHandler& aController )
    {
    LOG_ENTERFN( "CWsfDetailsContainer::ConstructL" );
    CreateWindowL();

    iListBox = new( ELeave ) CAknSingleHeadingStyleListBox;
    iListBox->SetContainerWindowL( *this );
    TInt flags = 0;
    flags |= EAknListBoxViewerFlags | EAknListBoxDisableItemSpecificMenu;
    iListBox->ConstructL( this, flags );
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( 
                                                CEikScrollBarFrame::EOff,
                                                CEikScrollBarFrame::EAuto );

    iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );

    SetRect( aRect );

    // Create and start Active Object 'Active Updater'
    iActiveUpdater = CWsfActiveTimeUpdater::NewL( &aController );
    iActiveUpdater->Start( KUpdateInterval );

    ActivateL();
    }


// ---------------------------------------------------------
// CWsfDetailsContainer::CConnectionMonitorUiContainer
// ---------------------------------------------------------
//
CWsfDetailsContainer::CWsfDetailsContainer() : iDetailsModel( NULL )
    {
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::CConnectionMonitorUiContainer
// ---------------------------------------------------------
//
void CWsfDetailsContainer::SetDetailsModel( CWsfDetailsViewModel* aModel )
    {
    iDetailsModel = aModel;
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::SizeChanged()
// ---------------------------------------------------------
//
void CWsfDetailsContainer::SizeChanged()
    {
    iListBox->SetRect( Rect() );
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::CountComponentControls()
// ---------------------------------------------------------
//
TInt CWsfDetailsContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::ComponentControl()
// ---------------------------------------------------------
//
CCoeControl* CWsfDetailsContainer::ComponentControl( TInt aIndex ) const
    {
    LOG_ENTERFN( "CWsfDetailsContainer::ComponentControl" );
    switch ( aIndex )
        {
        case 0:
            return iListBox;
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::OfferKeyEventL
// ---------------------------------------------------------
//
TKeyResponse CWsfDetailsContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                       TEventCode aType )
    {
    LOG_ENTERFN( "CWsfDetailsContainer::OfferKeyEventL" );
    TKeyResponse result = EKeyWasNotConsumed;
    
    if ( aType == EEventKey || aType == EEventKeyDown )
        {
        result = iListBox->OfferKeyEventL( aKeyEvent, aType );
        
        if ( aKeyEvent.iCode == EKeyLeftArrow )
            {
            if ( iPartner )
                {
                iPartner->HandleLeftKeyPressedL();
                result = EKeyWasConsumed;
                }
            }
        else if ( aKeyEvent.iCode == EKeyRightArrow )
            {
            if ( iPartner )
                {
                iPartner->HandleRightKeyPressedL();
                result = EKeyWasConsumed;
                }
            }
        else if ( aKeyEvent.iCode == EKeyOK )
            {
            if ( iPartner )
                {
                iPartner->HandleSelectionKeyPressedL();
                }
            }
        }
    
    return result;
    }
    
// ---------------------------------------------------------
// CWsfDetailsContainer::GetPanesL
// ---------------------------------------------------------
//
void CWsfDetailsContainer::GetPanesL()
    {
    LOG_ENTERFN( "CWsfDetailsContainer::GetPanesL" );
    if( !iNaviPane )
        {
        CEikStatusPane* statusPane = iEikonEnv->AppUiFactory()->StatusPane();

        iNaviPane =  static_cast<CAknNavigationControlContainer*>( 
                        statusPane->ControlL( 
                                    TUid::Uid(EEikStatusPaneUidNavi ) ) );
        }
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::UpdateNaviPaneL
// ---------------------------------------------------------
//
void CWsfDetailsContainer::UpdateNaviPaneL( const TDesC& aPaneText )
    {
    LOG_ENTERFN( "CWsfDetailsContainer::UpdateNaviPaneL" );
    LOG_WRITEF("UpdateNaviPaneL - aPaneTest: %S", &aPaneText );
    if ( !( ( CWsfAppUi *)( iEikonEnv->AppUi() ) )->Foreground() )
        {
        return;
        }
    
    GetPanesL();

    if ( !iNaviDecorator )
        {
        delete iNaviDecorator;
        iNaviDecorator = NULL;
        if( iNaviPane )
            {
            iNaviDecorator = iNaviPane->CreateNavigationLabelL( aPaneText );
            iNaviPane->PushL( *iNaviDecorator );
            }
        }
    else
        {
        CAknNaviLabel* naviLabel = static_cast<CAknNaviLabel*>( 
                                        iNaviDecorator->DecoratedControl() );
        naviLabel->SetTextL( aPaneText );
        // make sure at top and redraw
        iNaviPane->PushL( *iNaviDecorator );                  
        }
    
    if ( iNaviDecorator )
        {
        // Set the Observer of the NaviDecorator
        iNaviDecorator->SetNaviDecoratorObserver( this );
        // Show or hide the left and right arrow button
        iNaviDecorator->SetScrollButtonDimmed( 
                                CAknNavigationDecorator::ELeftButton,
                                iDetailsModel->FormatLeftScrollButton() );
        iNaviDecorator->SetScrollButtonDimmed( 
                                CAknNavigationDecorator::ERightButton,
                                iDetailsModel->FormatRightScrollButton() );
        // Set the visibility of the buttons
        iNaviDecorator->MakeScrollButtonVisible( ETrue );
        }
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::UpdateListBoxL
// ---------------------------------------------------------
//
void CWsfDetailsContainer::UpdateListBoxL( MDesCArray* aItemTextArray )
    {
    LOG_ENTERFN( "CWsfDetailsContainer::UpdateListBoxL" );    
    for( TInt i = 0; i < aItemTextArray->MdcaCount(); i++)
        {
        TPtrC temp = aItemTextArray->MdcaPoint( i );
        TBuf<100> tempElement = temp;
        LOG_WRITEF( "aItemTextArray: %S", &tempElement );
        }
        
    iListBox->Reset();
    iListBox->Model()->SetItemTextArray( aItemTextArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    iListBox->HandleItemAdditionL();
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::SetPartner
// ---------------------------------------------------------
//
void CWsfDetailsContainer::SetPartner( 
                               MWsfDetailsViewControllerPartner& aPartner )
    {
    iPartner = &aPartner;
    }

// ---------------------------------------------------------
// CWsfDetailsContainer::PopNaviPane
// ---------------------------------------------------------
//
void CWsfDetailsContainer::PopNaviPane()
    {
    LOG_ENTERFN( "CWsfDetailsContainer::PopNaviPane" );
    if ( iNaviPane && iNaviDecorator )
        {        
        iNaviPane->Pop( iNaviDecorator );
        delete iNaviDecorator;
        iNaviDecorator = NULL;
        }
    }


// ---------------------------------------------------------
// CCSnifferDetailsContainer::HandleResourceChange
// ---------------------------------------------------------
//
void CWsfDetailsContainer::HandleResourceChange( TInt aType )
    {
    LOG_ENTERFN( "CWsfDetailsContainer::HandleResourceChange" );
    CCoeControl::HandleResourceChange( aType );
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
// CWsfDetailsContainer::FocusChanged
// ---------------------------------------------------------
//
void CWsfDetailsContainer::FocusChanged( TDrawNow aDrawNow )
    {
    LOG_ENTERFN( "CWsfDetailsContainer::FocusChanged" );
    CCoeControl::FocusChanged( aDrawNow );    
    if( iListBox )
        {
        iListBox->SetFocus( IsFocused() );
        }        
    }
    
// --------------------------------------------------------------------------
// CWsfDetailsContainer::HandleNaviDecoratorEventL
// --------------------------------------------------------------------------
//
void CWsfDetailsContainer::HandleNaviDecoratorEventL( TInt aEventID )
    {
    LOG_ENTERFN( "CWsfDetailsContainer::HandleNaviDecoratorEventL" );
    if ( AknLayoutUtils::PenEnabled() )
        {
        switch ( aEventID )
            {
            case EAknNaviDecoratorEventRightTabArrow:
                {
                if( iPartner && !iDetailsModel->FormatRightScrollButton() )
                    {
                    iPartner->HandleRightKeyPressedL();
                    }
                break;
                }
            case EAknNaviDecoratorEventLeftTabArrow:
                {
                if( iPartner && !iDetailsModel->FormatLeftScrollButton() )
                    {
                    iPartner->HandleLeftKeyPressedL();
                    }
                break;
                }
            default:
                return;
            }
        }
    }

// End of File

