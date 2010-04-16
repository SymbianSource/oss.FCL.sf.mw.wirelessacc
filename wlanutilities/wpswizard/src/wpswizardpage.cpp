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
 *   WPS Wizard: API for wizard pages.
 *
 */

// System includes
#include <hbwidget.h>

// User includes
#include "wpswizardpage.h"

// Trace includes
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wpswizardpageTraces.h"
#endif

#define WPS_DEFAULT_STEPBACK 1

 /*!
 * Constructor for the WPS page baseclass
 * 
 * \param WpsWizardPrivate* pointer to the implementation class
 */
 
WpsWizardPage::WpsWizardPage(WpsWizardPrivate* parent) :
    mWizard(parent)
{
OstTraceFunctionEntry1(WPSWIZARDPAGE_WPSWIZARDPAGE_ENTRY, this)
OstTraceFunctionExit1(WPSWIZARDPAGE_WPSWIZARDPAGE_EXIT, this)
}

/*!
* Destructor
*/
WpsWizardPage::~WpsWizardPage()
{
    OstTraceFunctionEntry1(WPSWIZARDPAGE_WPSWIZARDPAGE_ENTRY, this)
    mWizard = NULL;
OstTraceFunctionExit1(WPSWIZARDPAGE_WPSWIZARDPAGE_EXIT, this)
}

/*!
* Determines how many pages to step back.
* \return number of pages to step backwards
*/
int WpsWizardPage::stepsBackwards()
{
    OstTraceFunctionEntry1(WPSWIZARDPAGE_STEPBACKWARDS_ENTRY, this)
    OstTraceFunctionExit1(WPSWIZARDPAGE_STEPBACKWARDS_EXIT, this)
    return WPS_DEFAULT_STEPBACK;
}

/*!
* Validates the contents of the page
*/
bool WpsWizardPage::validate() const
{
    OstTraceFunctionEntry1(WPSWIZARDPAGE_VALIDATE_ENTRY, this)
    OstTraceFunctionExit1(WPSWIZARDPAGE_VALIDATE_EXIT, this)
    return true;
}
