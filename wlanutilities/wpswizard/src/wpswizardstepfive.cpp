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
 *   WPS Wizard Page: Step 5
 *
 */

// System includes
#include <hbdocumentloader.h>
#include <hbwidget.h>
#include <hbradiobuttonlist.h>
#include <hblineedit.h>
#include <hblabel.h>

// User includes
#include "wpswizardstepfive.h"
#include "wpswizard_p.h"

// Trace Includes
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wpspagestepfiveTraces.h"
#endif

/*!
 * Constructor for WPS page five
 * 
 * \param WpsWizardPrivate* Pointer to the WPS wizard private implementation 
 */
WpsPageStepFive::WpsPageStepFive(WpsWizardPrivate* parent) :
    WpsWizardPage(parent), mWidget(NULL), mRadio(NULL), mValid(false)
{
OstTraceFunctionEntry1(WPSPAGESTEPFIVE_WPSPAGESTEPFIVE_ENTRY, this)
OstTraceFunctionExit1(WPSPAGESTEPFIVE_WPSPAGESTEPFIVE_EXIT, this)

}

/*!
 * Destructor
 */
WpsPageStepFive::~WpsPageStepFive()
{
    OstTraceFunctionEntry1(WPSPAGESTEPFIVE_WPSPAGESTEPFIVE_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPFIVE_WPSPAGESTEPFIVE_EXIT, this)

    delete mWidget;
}

/*!
 * Loads the page with all the widgets
 * 
 * \return HbWidget* Returns the view widget
 */
HbWidget* WpsPageStepFive::initializePage()
{
    OstTraceFunctionEntry1(WPSPAGESTEPFIVE_INITIALIZEPAGE_ENTRY, this)

    if (!mWidget) {
        bool ok;
        HbDocumentLoader loader;
        loader.load(":/docml/occ_wps_P5.docml", &ok);
        Q_ASSERT_X(ok, "WPS Wizard", "Invalid docml file");

        mWidget = qobject_cast<HbWidget*> (loader.findWidget("occ_wps_P5"));
        Q_ASSERT_X(mWidget != 0, "WPS Wizard", "View not found");

        //mTitle = qobject_cast<HbLabel*> (loader.findWidget("label_title"));
        //Q_ASSERT_X(mTitle != 0, "WPS wizard", "title not found");

        mHeading = qobject_cast<HbLabel*> (loader.findWidget("label_heading"));
        Q_ASSERT_X(mTitle != 0, "WPS wizard", "Header not found");

        mRadio = qobject_cast<HbRadioButtonList*> (loader.findWidget("radioButtonList"));
        Q_ASSERT_X(mRadio != 0, "WPS Wizard", "List not found");

        QList<TWlanProtectedSetupCredentialAttribute> arr = mWizard->getSettingsArray();

        QStringList ssidList;
        for (int count = 0; count < arr.count(); count++) {
            TWlanProtectedSetupCredentialAttribute attr = arr[count];
            //file.Write(attr.iSsid);
            //Append (attr.iSsid) to a list and send to UI Engine.
            QString ssid = QString::fromUtf8((const char*) attr.iSsid.Ptr(), attr.iSsid.Length());
            ssidList.append(ssid);
        }

        mRadio->setItems(ssidList);

        connect(mRadio, SIGNAL(itemSelected(int)), this, SLOT(itemSelected(int)));
    }
    OstTraceFunctionExit1(WPSPAGESTEPFIVE_INITIALIZEPAGE_EXIT, this)
    return mWidget;
}

/*!
 * Callback when the previous button is clicked
 */
void WpsPageStepFive::previousTriggered()
{
OstTraceFunctionEntry1(WPSPAGESTEPFIVE_PREVIOUSTRIGGERED_ENTRY, this)
OstTraceFunctionExit1(WPSPAGESTEPFIVE_PREVIOUSTRIGGERED_EXIT, this)

}
/*!
 * CallBack when the cancel button is clicked
 */
void WpsPageStepFive::cancelTriggered()
{
OstTraceFunctionEntry1(WPSPAGESTEPFIVE_CANCELTRIGGERED_ENTRY, this)
OstTraceFunctionExit1(WPSPAGESTEPFIVE_CANCELTRIGGERED_EXIT, this)

}
/*!
 * Funtion to determine the next page to be displayed in the wizard process
 * 
 * \param bool& RemoveFromStack indicating whether the current page should be 
 * removed from the stack
 * 
 * \return int Page Id of the next page to be displayed.
 */
int WpsPageStepFive::nextId(bool &removeFromStack) const
{
    OstTraceFunctionEntry1(WPSPAGESTEPFIVE_NEXTID_ENTRY, this)

    int id = WlanWizardPage::PageProcessSettings;
    removeFromStack = false;
    OstTraceFunctionExit1(WPSPAGESTEPFIVE_NEXTID_EXIT, this)
    return id;
}

/*!
 * Determines the Number of steps to move backwards when 'Prev' Button
 * is clicked
 * 
 * \return int Number of pages to move backwards
 */
int WpsPageStepFive::stepsBackwards()
{
    OstTraceFunctionEntry1(WPSPAGESTEPFIVE_STEPSBACKWARDS_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPFIVE_STEPBACKWARDS_EXIT, this)

    return (PageWpsWizardStep5 - PageWpsWizardStep2) - 1;
}

/*!
 * Validates the content of the pages
 * 
 * \return bool Indicating the result of the operation
 */
bool WpsPageStepFive::validate() const
{
    OstTraceFunctionEntry1(WPSPAGESTEPFIVE_VALIDATE_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPFIVE_VALIDATE_EXIT, this)

    return mValid;
}

/*!
 * Call back when an item is selected from the list on the UI
 * 
 * \param int index Indcating the relative position in the list
 */
void WpsPageStepFive::itemSelected(int index)
{
    OstTraceFunctionEntry1(WPSPAGESTEPFIVE_ITEMSELECTED_ENTRY, this)

    mValid = true;
    mWizard->storeSettings(index);
    mWizard->enableNextButton(mValid);

    OstTraceFunctionExit1(WPSPAGESTEPFIVE_ITEMSELECTED_EXIT, this)
}

