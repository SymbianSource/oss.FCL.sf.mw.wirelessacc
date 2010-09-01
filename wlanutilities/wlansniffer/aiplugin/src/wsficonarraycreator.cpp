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
* Description:  Implementation of CWsfIconArrayCreator.
*
*/

//  EXTERNAL INCLUDES
#include <AknIconArray.h>
#include <AknIconUtils.h>
#include <AknsUtils.h>
#include <eikenv.h>
#include <AknsConstants.h>
#include <wsficons.mbg>
#include <gulicon.h>

//  INTERNAL INCLUDES
#include "wsfaiplugincommon.h"
#include "wsficonarraycreator.h"


//  LOCAL DEFINITIONS
const TInt KAIShortcutPluginGranularity = 10;
#ifdef _DEBUG
    _LIT( KIconArrayCreatorPanic, "CWsfIconArrayCreator" );
    #define _ASS_D( cond ) __ASSERT_DEBUG( (cond), \
            User::Panic( KIconArrayCreatorPanic, __LINE__) )
#else
        #define _ASS_D( cond ) {}
#endif //_DEBUG

// --------------------------------------------------------------------------
// CleanUpResetAndDestroy
// --------------------------------------------------------------------------
//
void CleanUpResetAndDestroy(TAny* aArray)
    {
    if(aArray)
        {
        CArrayPtrFlat<CBase>* array=(CArrayPtrFlat<CBase>*)aArray;
        array->ResetAndDestroy();
        delete array;
        }
    }

// --------------------------------------------------------------------------
// LoadIconLC
// --------------------------------------------------------------------------
//
CGulIcon* LoadIconLC( const TInt aIconId, const TFileName aBitmapFile )
    {
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap;
    CFbsBitmap* mask;

    // Creates bitmap an icon.
    AknIconUtils::CreateIconL( bitmap, 
                               mask,
                               aBitmapFile,
                               aIconId,
                               aIconId+1 );
                
    icon->SetBitmap( bitmap );
    icon->SetMask( mask );
    return icon;
    }

// --------------------------------------------------------------------------
// LoadSkinnableIconLC
// --------------------------------------------------------------------------
//
CGulIcon* LoadSkinnableIconLC( const TAknsItemID aAknItemId,
                               const TInt aIconId,
                               const TFileName aBitmapFile )
    {
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap;
    CFbsBitmap* mask;

    // Creates bitmap an icon.
    AknsUtils::CreateIconL( AknsUtils::SkinInstance(), 
                            aAknItemId, 
                            bitmap,
                            mask, 
                            aBitmapFile, 
                            aIconId, 
                            aIconId + 1 );

    icon->SetBitmap(bitmap);
    icon->SetMask(mask);
    return icon;
    }
      
// ---------------------------------------------------------
// LoadSkinnableBlackIconLC
// ---------------------------------------------------------
//
CGulIcon* LoadSkinnableBlackIconLC( const TAknsItemID aAknItemId, 
                                    const TInt aIconId,
                                    const TFileName aBitmapFile )
    {
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap;
    CFbsBitmap* mask;

    // Creates bitmap an icon.
    AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(), 
                                 aAknItemId, 
                                 KAknsIIDQsnIconColors, 
                                 EAknsCIQsnIconColorsCG13, 
                                 bitmap, 
                                 mask, 
                                 aBitmapFile,
                                 aIconId, 
                                 aIconId+1,
                                 KRgbBlack );

    icon->SetBitmap( bitmap );
    icon->SetMask( mask );
    return icon;
    }

// --------------------------------------------------------------------------
// CWsfIconArrayCreator::CreateArrayL
// --------------------------------------------------------------------------
//
CAknIconArray* CWsfIconArrayCreator::CreateArrayL()
    {
    // Used bitmap file name
    TFileName bitmapFileName;
    bitmapFileName.Append( KDC_APP_BITMAP_DIR );
    bitmapFileName.Append( KBitmapFile );

    CAknIconArray* icons = new(ELeave) CAknIconArray( 
            KAIShortcutPluginGranularity );

    CleanupStack::PushL( TCleanupItem( CleanUpResetAndDestroy,icons) );

    //ESecureNetworkIcon
    icons->InsertL( ESecureNetworkIcon, LoadSkinnableBlackIconLC( 
            KAknsIIDQgnIndiWlanSecureNetworkAdd, 
            EMbmWsficonsQgn_indi_wlan_secure_network_add, 
            bitmapFileName ) );

    CleanupStack::Pop(); /*icon*/

    //ENoSignalIcon
    icons->InsertL( ENoSignalIcon, LoadIconLC( 
            EMbmWsficonsEmpty13x13, 
            bitmapFileName ) );
    CleanupStack::Pop(); /*icon*/

    //EPoorSignal
    icons->InsertL( EPoorSignal, LoadSkinnableBlackIconLC( 
            KAknsIIDQgnIndiWlanSignalLowAdd, 
            EMbmWsficonsQgn_indi_wlan_signal_low_add, 
            bitmapFileName ) );

    CleanupStack::Pop(); /*icon*/

    //EAverageSignal
    icons->InsertL( EAverageSignal, LoadSkinnableBlackIconLC( 
            KAknsIIDQgnIndiWlanSignalMedAdd, 
            EMbmWsficonsQgn_indi_wlan_signal_med_add, 
            bitmapFileName ) );
    CleanupStack::Pop(); /*icon*/

    //EExcelentSignal
    icons->InsertL( EExcelentSignal, LoadSkinnableBlackIconLC( 
            KAknsIIDQgnIndiWlanSignalGoodAdd , 
            EMbmWsficonsQgn_indi_wlan_signal_good_add, 
            bitmapFileName ) );
    CleanupStack::Pop(); /*icon*/

    //EConnectedNWIcon
    icons->InsertL( EConnectedNWIcon, 
            LoadSkinnableIconLC( KAknsIIDQgnIndiWlanSnifferPluginOn, 
                    EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on,
                    bitmapFileName ) );

    CleanupStack::Pop(); /*icon*/

    //EKnownNWIcon
    icons->InsertL( EKnownNWIcon, LoadSkinnableIconLC( 
            KAknsIIDQgnIndiWlanSnifferPluginOn, 
            EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on, 
            bitmapFileName ) );
    CleanupStack::Pop(); /*icon*/

    //ETransparentIcon
    icons->InsertL( ETransparentIcon, LoadIconLC( 
            EMbmWsficonsQgn_prop_empty, 
            bitmapFileName ) );
    CleanupStack::Pop(); /*icon*/

    //EWlanOnIcon
    icons->InsertL( EWlanOnIcon, LoadSkinnableIconLC( 
            KAknsIIDQgnIndiWlanSnifferPluginOn, 
            EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on, 
            bitmapFileName ) );
    CleanupStack::Pop(); /*icon*/

    icons->InsertL( EWlanOnIcon_0, LoadSkinnableIconLC( 
            KAknsIIDQgnIndiWlanSnifferPluginOn0, 
            EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on_0,
            bitmapFileName ) );
    CleanupStack::Pop(); /*icon*/

    icons->InsertL( EWlanOnIcon_1, LoadSkinnableIconLC( 
            KAknsIIDQgnIndiWlanSnifferPluginOn1, 
            EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on_1, 
            bitmapFileName ) );
    CleanupStack::Pop(); /*icon*/

    //ETransparentIcon
    icons->InsertL( EWlanOffIcon, LoadSkinnableIconLC( 
            KAknsIIDQgnIndiWlanSnifferPluginOff, 
            EMbmWsficonsQgn_indi_wlan_sniffer_plugin_off, 
            bitmapFileName ) );
    CleanupStack::Pop();

    icons->InsertL( EWlanNaviBarIcon, LoadSkinnableIconLC(
            KAknsIIDQgnMenuWlanSniffer,
            EMbmWsficonsQgn_menu_wlan_sniffer,
            bitmapFileName ) );

    CleanupStack::Pop(); /*icon*/
    CleanupStack::Pop( icons );

    return icons;
    }

// --------------------------------------------------------------------------
// CWsfIconArrayCreator::GetIconLC
// --------------------------------------------------------------------------
//
CGulIcon* CWsfIconArrayCreator::GetIconLC( TInt aIconId )
    {
    // Used bitmap file name
    TFileName bitmapFileName;
    bitmapFileName.Append( KDC_APP_BITMAP_DIR );
    bitmapFileName.Append( KBitmapFile );
    TAknsItemID localAknItemId( KAknsIIDQgnMenuWlanSniffer );
    TInt iconId( EMbmWsficonsQgn_menu_wlan_sniffer );

    switch( aIconId )
        {
        case ESecureNetworkIcon:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSecureNetworkAdd;
            iconId = EMbmWsficonsQgn_indi_wlan_secure_network_add; 
            break;
            }

        case ENoSignalIcon:
            return LoadIconLC( EMbmWsficonsEmpty13x13, bitmapFileName );

        case EPoorSignal:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSignalLowAdd;
            iconId = EMbmWsficonsQgn_indi_wlan_signal_low_add; 
            break;
            }

        case EAverageSignal:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSignalMedAdd;
            iconId = EMbmWsficonsQgn_indi_wlan_signal_med_add; 
            break;
            }            

        case EExcelentSignal:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSignalGoodAdd;
            iconId = EMbmWsficonsQgn_indi_wlan_signal_good_add; 
            break;
            }            

        case EConnectedNWIcon:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSnifferPluginOn;
            iconId = EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on;  
            break;
            }                 

        case EKnownNWIcon:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSnifferPluginOn;
            iconId = EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on; 
            break;
            } 

        case ETransparentIcon:
            return LoadIconLC( EMbmWsficonsQgn_prop_empty, bitmapFileName );

        case EWlanOnIcon:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSnifferPluginOn;
            iconId = EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on; 
            break;
            } 

        case EWlanOnIcon_0:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSnifferPluginOn0;
            iconId = EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on_0; 
            break;
            }

        case EWlanOnIcon_1:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSnifferPluginOn1;
            iconId = EMbmWsficonsQgn_indi_wlan_sniffer_plugin_on_1; 
            break;
            }

        case EWlanOffIcon:
            {
            localAknItemId = KAknsIIDQgnIndiWlanSnifferPluginOff;
            iconId = EMbmWsficonsQgn_indi_wlan_sniffer_plugin_off; 
            break;
            }  

        case EWlanNaviBarIcon:
            {
            localAknItemId = KAknsIIDQgnMenuWlanSniffer;
            iconId = EMbmWsficonsQgn_menu_wlan_sniffer; 
            break;
            }  
        default:
            {
            _ASS_D( 0 );
            }
        }

    switch( aIconId )
        {
        case ESecureNetworkIcon:
        case EPoorSignal:
        case EAverageSignal:
        case EExcelentSignal:
            {
            return LoadSkinnableBlackIconLC( localAknItemId, 
                    iconId,
                    bitmapFileName );
            }
        default:
            {
            break;
            }
        } 
    return LoadSkinnableIconLC( localAknItemId, 
            iconId, 
            bitmapFileName );
    }

