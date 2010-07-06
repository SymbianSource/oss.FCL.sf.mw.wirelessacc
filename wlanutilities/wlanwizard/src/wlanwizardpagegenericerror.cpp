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
 *   WLAN Wizard Page: Generic error page.
 *
 */

// System includes
#include <HbMainWindow>
#include <HbDocumentLoader>
#include <HbWidget>
#include <HbLabel>

// User includes
#include "wlanwizard_p.h"
#include "wlanwizardpagegenericerror.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wlanwizardpageprocessingsettingsTraces.h"
#endif

/*!
   \class WlanWizardPageGenericError
   \brief Implements generic error page for wizard.
   
   Error text is read from the configurations 
   (WlanWizardHelper::ConfGenericErrorString) of the wizard. 
 */

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========


/*!
   Constructor.
   
   @param [in] parent pointer to private implementation of wizard.
 */
WlanWizardPageGenericError::WlanWizardPageGenericError(
    WlanWizardPrivate* parent) :
    WlanWizardPageInternal(parent), 
    mWidget(NULL), 
    mLabel(NULL)
{
}

/*!
   Destructor.
 */
WlanWizardPageGenericError::~WlanWizardPageGenericError()
{
    // signals are automatically disconnected
}

/*!
   See WlanWizardPage::initializePage()
 */
HbWidget* WlanWizardPageGenericError::initializePage()
{
    // Create the visualization at the first time
    if (!mWidget) {
        HbDocumentLoader docLoader(mWizard->mainWindow());
        
        bool ok;
        docLoader.load(":/docml/occ_add_wlan_error.docml", &ok);
        Q_ASSERT(ok);
        
        mWidget = qobject_cast<HbWidget*> (docLoader.findWidget("occ_add_wlan_error"));
        Q_ASSERT(mWidget != NULL);

        mLabel = qobject_cast<HbLabel*> (docLoader.findWidget("dialog"));
        Q_ASSERT(mLabel != NULL);
    }

    // Get the error string from the wizards configurations
    mLabel->setPlainText(
        mWizard->configuration(
            WlanWizardHelper::ConfGenericErrorString).toString());

    return mWidget;
}

/*!
   See WlanWizardPage::showPage()
   
   @return false. Next button is dimmed when the page is displayed.
 */
bool WlanWizardPageGenericError::showPage()
{
    return false;
}

/*!
    See WlanWizardPage::previousTriggered()
   
   @return steps backwards
 */
int WlanWizardPageGenericError::previousTriggered()
{
    return mWizard->configuration(
        WlanWizardHelper::ConfGenericErrorPageStepsBackwards).toInt();
}
