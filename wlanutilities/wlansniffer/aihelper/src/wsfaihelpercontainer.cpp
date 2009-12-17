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
* Description:  Implementation of CWsfAiHelperContainer
*
*/



//  EXTERNAL INCLUDES
#include <AknsDrawUtils.h>
#include <AknsBasicBackgroundControlContext.h> 
#include <AknUtils.h>

//  CLASS HEADER
#include "wsfaihelpercontainer.h"

//  INTERNAL INCLUDES
#include "wsfaihelperappui.h"
#include "wsfaihelperdocument.h"
#include "wsfmodelobserver.h"
#include "wsfaihelper.hrh"
#include "wsfmodel.h"
#include "wsflogger.h"


//  CONSTRUCTION AND DESTRUCTION


// ---------------------------------------------------------------------------
// CWsfAiHelperContainer::~CWsfAiHelperContainer
// ---------------------------------------------------------------------------
//
CWsfAiHelperContainer::~CWsfAiHelperContainer()
    {
    delete iBackGround;
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperContainer::ConstructL
// ---------------------------------------------------------------------------
//
void CWsfAiHelperContainer::ConstructL( TRect aRect, 
                                        CWsfAiHelperAppUi* aAppUi )
    {
    iAppUi = aAppUi;
    iBackGround = CAknsBasicBackgroundControlContext::NewL( 
                                     KAknsIIDQsnBgAreaMain, Rect(), EFalse );
    
    CreateWindowL();
    SetRect( aRect );
    ActivateL();
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperContainer::SizeChanged
// ---------------------------------------------------------------------------
//
void CWsfAiHelperContainer::SizeChanged()
    {
    LOG_ENTERFN( "CWsfAiHelperContainer::SizeChanged" );
    
    if ( iBackGround )
        {
        delete iBackGround;
        iBackGround = NULL;
        // Throw away the error
        TRAP_IGNORE( iBackGround = CAknsBasicBackgroundControlContext::NewL( 
                                    KAknsIIDQsnBgAreaMain, Rect(), EFalse ) );
        }
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperContainer::Draw
// ---------------------------------------------------------------------------
//
void CWsfAiHelperContainer::Draw( const TRect& aRect ) const 
    { 
    CWindowGc& gc = SystemGc(); 
    MAknsSkinInstance* skin = AknsUtils::SkinInstance(); 
    MAknsControlContext* cc = AknsDrawUtils::ControlContext( this ); 
    AknsDrawUtils::Background( skin, cc, this, gc, aRect ); 
    } 


// ---------------------------------------------------------------------------
// CWsfAiHelperContainer::HandleResourceChange
// ---------------------------------------------------------------------------
//
void CWsfAiHelperContainer::HandleResourceChange( TInt aType )
    {
    LOG_ENTERFN( "CWsfAiHelperContainer::HandleResourceChange" );
    CCoeControl::HandleResourceChange( aType );

    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, 
                                           mainPaneRect );
        SetRect( mainPaneRect );
        DrawDeferred();
        }
    }


// ---------------------------------------------------------------------------
// CWsfAiHelperContainer::MopSupplyObject
// ---------------------------------------------------------------------------
//
TTypeUid::Ptr CWsfAiHelperContainer::MopSupplyObject( TTypeUid aId )
    {
    LOG_ENTERFN( "CWsfAiHelperContainer::MopSupplyObject" );
    if ( aId.iUid == MAknsControlContext::ETypeId && iBackGround )
        {
        return MAknsControlContext::SupplyMopObject( aId, iBackGround );
        }

    return CCoeControl::MopSupplyObject( aId );
    }
    

// End of file
