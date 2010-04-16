/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of the License "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description: 
 *   WPS Wizard: API implementation.
 *
 */


// System includes
#include <hbdocumentloader.h>
#include <qgraphicswidget>
#include <hbmainwindow.h>
#include <hbview.h>
#include <hbstackedwidget.h>
#include <hblabel.h>
#include <hbaction.h>
#include <qapplication.h>

// Trace includes
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wpswizardTraces.h"
#endif

// User includes
// WLAN Wizard
#include "wlanwizardhelpper.h"

// Wizard 
#include "wpswizard.h"
#include "wpswizard_p.h"

// Wizard Pages
#include "wpswizardsteptwo.h"
#include "wpswizardstepthreebutton.h"
#include "wpswizardstepthreenumber.h"
#include "wpswizardstepfour.h"
#include "wpswizardstepfive.h"


class WlanWizardHelpper;

/*!
* Creates the WpsWizard object
* 
* \param WlanWizardHelpper* Helper class to from the framework
* \param BearerType Indicates the bearer type Eg. Wlan
* 
*/

WpsWizard::WpsWizard(WlanWizardHelpper *wizardHelpper, BearerType bearerType,
        int referenceId) :
    d_ptr(new WpsWizardPrivate(this, wizardHelpper, bearerType, referenceId))
    {
    OstTraceFunctionEntry1(WPSWIZARD_WPSWIZARD_ENTRY, this)
    OstTraceFunctionExit1(WPSWIZARD_WPSWIZARD_EXIT, this)
    }

/*!
* Destructor
* 
*/

WpsWizard::~WpsWizard()
    {
    OstTraceFunctionEntry1(WPSWIZARD_WPSWIZARD_ENTRY, this);
    delete d_ptr;
    OstTraceFunctionExit1(WPSWIZARD_WPSWIZARD_EXIT, this);
    }

/*!
* StoreSettings: Stores the settings in the commsdat database
* 
*/

void WpsWizard::storeSettings()
    {
    OstTraceFunctionEntry1(WPSWIZARD_STORESETTINGS_ENTRY, this);
    OstTraceFunctionExit1(WPSWIZARD_STORESETTINGS_EXIT, this);
    }

/*!
* Creates the summary page, valid only for EAP usecase
* 
* \param WlanWizardPlugin::Summary indicates the type of summary item
* \param QString &item item name
* \param QString &value item value
* 
* \return the status of the summary operation
*/

bool WpsWizard::summary(WlanWizardPlugin::Summary sum, QString &item,
        QString &value)
    {
    OstTraceFunctionEntry1(WPSWIZARD_SUMMARY_ENTRY, this);
    OstTraceFunctionExit1(WPSWIZARD_SUMMARY_EXIT, this);
    return true;
    }
