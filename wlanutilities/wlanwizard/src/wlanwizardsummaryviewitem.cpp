/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 *   Wlan Wizard Page: Summary: List view item
 *
 */

// System includes

// User includes

#include "wlanwizardsummaryviewitem.h"

/*!
   \class WlanWizardSummaryListViewItem
   \brief Implements custom List view for Summary page.
   
   HbListWidget does not support directly views where selection is disabled.
   
   This implementation removes required operations to disable list selection.
 */

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// 

/*!
   Constructor.
   
   @param [in] parent parent class
 */
WlanWizardSummaryListViewItem::WlanWizardSummaryListViewItem(
    QGraphicsItem* parent) : 
    HbListViewItem(parent)
{
}

/*!
   Destructor.
 */
WlanWizardSummaryListViewItem::~WlanWizardSummaryListViewItem()
{
}

/*!
   See HbListViewItem::updateChildItems().
   
   Disables selection. 
 */
void WlanWizardSummaryListViewItem::updateChildItems()
{
    HbListViewItem::updateChildItems();
    
    ungrabGesture(Qt::TapGesture);
    
    GraphicsItemFlags itemFlags = flags();
    itemFlags &= ~QGraphicsItem::ItemIsFocusable;
    setFlags(itemFlags);
}

/*!
   See HbListViewItem::createItem().
 */
HbAbstractViewItem*  WlanWizardSummaryListViewItem::createItem()
{
    return new WlanWizardSummaryListViewItem(*this);
}
