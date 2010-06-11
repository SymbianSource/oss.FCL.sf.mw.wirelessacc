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
 *   WLAN Wizard: Interface for wizard pages (wlan internal).
 *
 */

// System includes
#include <HbWidget>

// User includes
#include "wlanwizard_p.h"
#include "wlanwizardpageinternal.h"

/*!
   \class WlanWizardPageInternal
   \brief Interface for internal wlan wizard pages. Contains pointer to private
   implementation of wlan wizard.
 */

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

/*!
   Constructor
   
   @param [in] wizard pointer to private implementation of wizard
 */
WlanWizardPageInternal::WlanWizardPageInternal(WlanWizardPrivate *wizard) :
    QObject(wizard), 
    mWizard(wizard)
{
}

/*!
   Destructor.
 */
WlanWizardPageInternal::~WlanWizardPageInternal()
{
}
