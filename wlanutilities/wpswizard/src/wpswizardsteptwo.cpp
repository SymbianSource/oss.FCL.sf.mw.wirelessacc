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
 *   WPS Wizard Page: Step 2
 *
 */

// System includes
#include <hbdocumentloader.h>
#include <hbwidget.h>
#include <hbradiobuttonlist.h>
#include <hblineedit.h>
#include <hblabel.h>

// User includes
#include "wpswizardsteptwo.h"
#include "wpswizard_p.h"

// Trace includes
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wpspagesteptwoTraces.h"
#endif


/*!
 * Constructor for WPS page two
 * 
 * \param WpsWizardPrivate* Pointer to the WPS wizard private implementation 
 */
WpsPageStepTwo::WpsPageStepTwo(WpsWizardPrivate* parent) :
    WpsWizardPage(parent), mWidget(NULL), mRadio(NULL), mValid(false)
    {
    OstTraceFunctionEntry1(WPSPAGESTEPTWO_WPSPAGESTEPTWO_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPTWO_WPSPAGESTEPTWO_EXIT, this)

    }

/*!
 * Destructor
 */
WpsPageStepTwo::~WpsPageStepTwo()
    {
    OstTraceFunctionEntry1(WPSPAGESTEPTWO_WPSPAGESTEPTWO_ENTRY, this)

    delete mWidget;
    OstTraceFunctionExit1(WPSPAGESTEPTWO_WPSPAGESTEPTWO_EXIT, this)

    }

/*!
  * Loads the page with all the widgets
  * 
  * \return HbWidget* Returns the view widget
 */
HbWidget* WpsPageStepTwo::initializePage()
    {
    OstTraceFunctionEntry1(WPSPAGESTEPTWO_INITIALIZEPAGE_ENTRY, this)

    if (!mWidget)
        {
        bool ok;
        HbDocumentLoader loader;
        loader.load(":/docml/occ_wps_P1.docml", &ok);
        Q_ASSERT_X(ok, "WPS Wizard", "Invalid docml file");

        mWidget = qobject_cast<HbWidget*> (loader.findWidget("occ_wps_P1"));
        Q_ASSERT_X(mWidget != 0, "WPS Wizard", "View not found");

        mHeading
                = qobject_cast<HbLabel*> (loader.findWidget("label_heading"));
        Q_ASSERT_X(mTitle != 0, "WPS wizard", "Header not found");

        mRadio = qobject_cast<HbRadioButtonList*> (loader.findWidget(
                "radioButtonList"));
        Q_ASSERT_X(mRadio != 0, "WPS Wizard", "List not found");

        connect(mRadio, SIGNAL(itemSelected(int)), this,
                SLOT(itemSelected(int)));

        }
    OstTraceFunctionExit1(WPSPAGESTEPTWO_INITIALIZEPAGE_EXIT, this)

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
int WpsPageStepTwo::nextId(bool &removeFromStack) const
    {
    OstTraceFunctionEntry1(WPSPAGESTEPTWO_NEXTID_ENTRY, this)
    int id = WpsWizardPage::PageWpsWizardStep3_Button;
    removeFromStack = false;
    if (mItemSelected == 0)
        id = WpsWizardPage::PageWpsWizardStep3_Button;
    else
        id = WpsWizardPage::PageWpsWizardStep3_Number;
    OstTraceFunctionExit1(WPSPAGESTEPTWO_NEXTID_EXIT, this)


    return id;
    }

/*!
  * Determines the Number of steps to move backwards when 'Prev' Button
  * is clicked
  * 
  * \return int Number of pages to move backwards
 */
int WpsPageStepTwo::stepsBackwards()
    {
    OstTraceFunctionEntry1(WPSPAGESTEPTWO_STEPSBACKWARDS_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPTWO_STEPBACKWARDS_EXIT, this)

    return 1;
    }

/*!
 * Callback when the previous button is clicked
*/
void WpsPageStepTwo::previousTriggered()
    {
    OstTraceFunctionEntry1(WPSPAGESTEPTWO_PREVIOUSTRIGGERED_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPTWO_PREVIOUSTRIGGERED_EXIT, this)

    }

/*!
  * CallBack when the cancel button is clicked
 */
void WpsPageStepTwo::cancelTriggered()
    {
    OstTraceFunctionEntry1(WPSPAGESTEPTWO_CANCELTRIGGERED_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPTWO_CANCELTRIGGERED_EXIT, this)

    }

/*!
  * Validates the content of the pages
  * 
  * \return bool Indicating the result of the operation
 */
bool WpsPageStepTwo::validate() const
    {
    OstTraceFunctionEntry1(WPSPAGESTEPTWO_VALIDATE_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPTWO_VALIDATE_EXIT, this)

    return mValid;
    }

/*!
  * Call back when an item is selected from the list on the UI
  * 
  * \param int index Indcating the relative position in the list
 */
void WpsPageStepTwo::itemSelected(int index)
    {
    OstTraceFunctionEntry1(WPSPAGESTEPTWO_ITEMSELECTED_ENTRY, this)

    mValid = true;
    mWizard->enableNextButton(mValid);
    mItemSelected = index;
    OstTraceFunctionExit1(WPSPAGESTEPTWO_ITEMSELECTED_EXIT, this)

    }

