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
 *   WPS Wizard Page: Step 3 Push button number
 *
 */

// System includes
#include <hbdocumentloader.h>
#include <hbwidget.h>
#include <hbradiobuttonlist.h>
#include <hblineedit.h>
#include <hblabel.h>

// User includes
#include "wpswizardstepthreebutton.h"
#include "wpswizard_p.h"

// Trace includes
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wpspagestepthreebuttonTraces.h"
#endif


/*!
 * Constructor for WPS page three button
 * 
 * \param WpsWizardPrivate* Pointer to the WPS wizard private implementation 
 */
WpsPageStepThreeButton::WpsPageStepThreeButton(WpsWizardPrivate* parent) :
    WpsWizardPage(parent), mWidget(NULL), mRadio(NULL), mValid(true)
{
OstTraceFunctionEntry1(WPSPAGESTEPTHREEBUTTON_WPSPAGESTEPTHREEBUTTON_ENTRY, this)
OstTraceFunctionExit1(WPSPAGESTEPTHREEBUTTON_WPSPAGESTEPTHREEBUTTON_EXIT, this)

}

/*!
 * Destructor
 */
WpsPageStepThreeButton::~WpsPageStepThreeButton()
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREEBUTTON_WPSPAGESTEPTHREEBUTTON_ENTRY, this)

    delete mWidget;

OstTraceFunctionExit1(WPSPAGESTEPTHREEBUTTON_WPSPAGESTEPTHREEBUTTON_EXIT, this)
}

/*!
  * Loads the page with all the widgets
  * 
  * \return HbWidget* Returns the view widget
 */
HbWidget* WpsPageStepThreeButton::initializePage()
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREEBUTTON_INITIALIZEPAGE_ENTRY, this)

    if (!mWidget) {
        bool ok;
        HbDocumentLoader loader;
        loader.load(":/docml/occ_wps_P2.docml", &ok);
        Q_ASSERT_X(ok, "WPS Wizard", "Invalid docml file");

        mWidget = qobject_cast<HbWidget*> (loader.findWidget("occ_wps_P2"));
        Q_ASSERT_X(mWidget != 0, "WPS Wizard", "View not found");

        //mTitle = qobject_cast<HbLabel*> (loader.findWidget("label_title"));
        //Q_ASSERT_X(mTitle != 0, "WPS wizard", "title not found");

        mHeading = qobject_cast<HbLabel*> (loader.findWidget("label_heading"));
        Q_ASSERT_X(mHeading != 0, "WPS wizard", "Header not found");

        mWizard->enableNextButton(true);
    }
    OstTraceFunctionExit1(WPSPAGESTEPTHREEBUTTON_INITIALIZEPAGE_EXIT, this)

    return mWidget;
}

/*!
  * Funtion to determine the next page to be displayed in the wizard process
  * 
  * \param bool& RemoveFromStack indicating whether the current page should be 
  * removed from the stack
  * 
  * \return int Page Id of the next page to be displayed.
 */
int WpsPageStepThreeButton::nextId(bool &removeFromStack) const
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREEBUTTON_NEXTID_ENTRY, this)
    int id = WlanWizardPage::PageNone;
    removeFromStack = false;
    id = WpsWizardPage::PageWpsWizardStep4;
    OstTraceFunctionExit1(WPSPAGESTEPTHREEBUTTON_NEXTID_EXIT, this)

    return id;
}

/*!
 * Determines the Number of steps to move backwards when 'Prev' Button
 * is clicked
 * 
 * \return int Number of pages to move backwards
*/
int WpsPageStepThreeButton::stepsBackwards()
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREEBUTTON_STEPSBACKWARDS_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPTHREEBUTTON_STEPBACKWARDS_EXIT, this)

    return (PageWpsWizardStep3_Button - PageWpsWizardStep2);
}

/*!
  * Callback when the previous button is clicked
 */
void WpsPageStepThreeButton::previousTriggered()
{
OstTraceFunctionEntry1(WPSPAGESTEPTHREEBUTTON_PREVIOUSTRIGGERED_ENTRY, this)
OstTraceFunctionExit1(WPSPAGESTEPTHREEBUTTON_PREVIOUSTRIGGERED_EXIT, this)

}

/*!
  * CallBack when the cancel button is clicked
 */
void WpsPageStepThreeButton::cancelTriggered()
{
OstTraceFunctionEntry1(WPSPAGESTEPTHREEBUTTON_CANCELTRIGGERED_ENTRY, this)
OstTraceFunctionExit1(WPSPAGESTEPTHREEBUTTON_CANCELTRIGGERED_EXIT, this)

}

/*!
 * Validates the content of the pages
 * 
 * \return bool Indicating the result of the operation
*/
bool WpsPageStepThreeButton::validate() const
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREEBUTTON_VALIDATE_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPTHREEBUTTON_VALIDATE_EXIT, this)

    return mValid;
}

