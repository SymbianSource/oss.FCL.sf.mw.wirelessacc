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
 *   WPS Wizard Page: Step 4 progress bar display
 *
 */

/*
 * %version: 1 %
 */

// System includes
#include <hbdocumentloader.h>
#include <hbwidget.h>
#include <hbradiobuttonlist.h>
#include <hblineedit.h>
#include <hblabel.h>
#include <hbprogressbar.h>
#include <hbmessagebox.h>

// User includes
#include "wpswizardstepfour.h"
#include "wpswizard_p.h"

// Trace includes
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wpspagestepfourTraces.h"
#endif

/*!
 * Constructor for WPS page four
 * 
 * \param WpsWizardPrivate* Pointer to the WPS wizard private implementation 
 */
WpsPageStepFour::WpsPageStepFour(WpsWizardPrivate* parent) :
    WpsWizardPage(parent), mWidget(NULL), mRadio(NULL), mValid(false)
{
OstTraceFunctionEntry1(WPSPAGESTEPFOUR_WPSPAGESTEPFOUR_ENTRY, this)
OstTraceFunctionExit1(WPSPAGESTEPFOUR_WPSPAGESTEPFOUR_EXIT, this)

}

/*!
 * Destructor
 */
WpsPageStepFour::~WpsPageStepFour()
{
    OstTraceFunctionEntry1(WPSPAGESTEPFOUR_WPSPAGESTEPFOUR_ENTRY, this)

    delete mWidget;
    delete mWpsActiveRunner;

OstTraceFunctionExit1(WPSPAGESTEPFOUR_WPSPAGESTEPFOUR_EXIT, this)
}

/*!
  * Loads the page with all the widgets
  * 
  * \return HbWidget* Returns the view widget
 */
HbWidget* WpsPageStepFour::initializePage()
{
    OstTraceFunctionEntry1(WPSPAGESTEPFOUR_INITIALIZEPAGE_ENTRY, this)

    if (!mWidget) {
        bool ok;
        HbDocumentLoader loader;
        loader.load(":/docml/occ_wps_P4.docml", &ok);
        Q_ASSERT_X(ok, "WPS Wizard", "Invalid docml file");

        mWidget = qobject_cast<HbWidget*> (loader.findWidget("occ_wps_P4"));
        Q_ASSERT_X(mWidget != 0, "WPS Wizard", "View not found");

        //mTitle = qobject_cast<HbLabel*> (loader.findWidget("label_title"));
        //Q_ASSERT_X(mTitle != 0, "WPS wizard", "title not found");

        mHeading = qobject_cast<HbLabel*> (loader.findWidget("label_heading"));
        Q_ASSERT_X(mTitle != 0, "WPS wizard", "Header not found");

        mProgressBar = qobject_cast<HbProgressBar*> (loader.findWidget("horizontalProgressBar"));
        Q_ASSERT_X(mRadio != 0, "WPS Wizard", "List not found");

        // start the framework.
        mWpsActiveRunner = CWpsActiveRunner::NewL(*this);
        Q_ASSERT(error == NULL);
    }

    mProgressBar->setRange(0, 0);

    StartWpsRunner();
    OstTraceFunctionExit1(WPSPAGESTEPFOUR_INITIALIZEPAGE_EXIT, this)

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
int WpsPageStepFour::nextId(bool &removeFromStack) const
{
    OstTraceFunctionEntry1(WPSPAGESTEPFOUR_NEXTID_ENTRY, this)

    int id = WpsWizardPage::PageNone;
    removeFromStack = false;
    if (countCredentials >= 2) {
        id = WpsWizardPage::PageWpsWizardStep5;
    }
    else {
        // only one response received. So go to the summary page
        mWizard->storeSettings(0);
        id = WlanWizardPage::PageProcessSettings;
    }
    OstTraceFunctionExit1(WPSPAGESTEPFOUR_NEXTID_EXIT, this)

    return id;
}


/*!
  * Determines the Number of steps to move backwards when 'Prev' Button
  * is clicked
  * 
  * \return int Number of pages to move backwards
 */
int WpsPageStepFour::stepsBackwards()
{
    OstTraceFunctionEntry1(WPSPAGESTEPFOUR_STEPSBACKWARDS_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPFOUR_STEPBACKWARDS_EXIT, this)

    return (PageWpsWizardStep4 - PageWpsWizardStep2) - 1;
}

/*!
  * Callback when the previous button is clicked
 */
void WpsPageStepFour::previousTriggered()
{
    OstTraceFunctionEntry1(WPSPAGESTEPFOUR_PREVIOUSTRIGGERED_ENTRY, this)

    mWpsActiveRunner->DoCancel();
OstTraceFunctionExit1(WPSPAGESTEPFOUR_PREVIOUSTRIGGERED_EXIT, this)

}

/*!
 * CallBack when the cancel button is clicked
*/
void WpsPageStepFour::cancelTriggered()
{
    OstTraceFunctionEntry1(WPSPAGESTEPFOUR_CANCELTRIGGERED_ENTRY, this)

    mWpsActiveRunner->DoCancel();
OstTraceFunctionExit1(WPSPAGESTEPFOUR_CANCELTRIGGERED_EXIT, this)

}

/*!
  * Validates the content of the pages
  * 
  * \return bool Indicating the result of the operation
 */
bool WpsPageStepFour::validate() const
{
    OstTraceFunctionEntry1(WPSPAGESTEPFOUR_VALIDATE_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPFOUR_VALIDATE_EXIT, this)

    return mValid;
}


/*!
 * CallBack function when the middleware WPS call returns back
 * 
 * \param QList An array of received settngs
 * \param int Error indicating the result of the operation
 * 
 */
void WpsPageStepFour::WpsActiveRunnerStopped(
    QList<TWlanProtectedSetupCredentialAttribute>& aCredentials, TInt aError)
{
    OstTraceFunctionEntry1(WPSPAGESTEPFOUR_WPSACTIVERUNNERSTOPPED_ENTRY, this)

    mWpsErrorCode = aError;
    if (aError == KErrNone) {
        countCredentials = aCredentials.count();
        mWizard->storeSettingsArray(aCredentials);
        mWizard->nextPage();
    }
    else if (aError == KErrCancel) {
        // operation was cancelled 

    }
    else {
        QString str;
        str = QString("Error: %1").arg(aError);
        mProgressBar->setVisible(false);
        //mHeading->setPlainText("General Error");
        mHeading->setPlainText(str);
        mWizard->enableNextButton(false);

    }
OstTraceFunctionExit1(WPSPAGESTEPFOUR_WPSACTIVERUNNERSTOPPED_EXIT, this)
}


/*!
 * Provides the functionality to initiate the WPS functionality
 */
void WpsPageStepFour::StartWpsRunner()
{

    OstTraceFunctionEntry1(WPSPAGESTEPFOUR_STARTWPSRUNNER_ENTRY, this)

    // convert QString to symbian RBuf8

    QString ssid = mWizard->getSsid();

    TPtrC ptrName(reinterpret_cast<const TText*> (ssid.constData()));

    RBuf8 ssidName;
    ssidName.Create(ptrName.Length());
    ssidName.Copy(ptrName);

    mWpsActiveRunner->StartSetup(ssidName, mWizard->getPin());
    ssidName.Close();
OstTraceFunctionExit1(WPSPAGESTEPFOUR_STARTWPSRUNNER_EXIT, this)
}
