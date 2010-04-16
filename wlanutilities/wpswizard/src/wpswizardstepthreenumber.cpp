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
 *   WPS Wizard Page: Step 3 Number enter mode.
 *
 */

// System includes
#include <hbdocumentloader.h>
#include <hbwidget.h>
#include <hbradiobuttonlist.h>
#include <hblineedit.h>
#include <hblabel.h>
#include <QTextStream>
#include <QTime>

// User includes
#include "wpswizardstepthreenumber.h"
#include "wpswizard_p.h"

// Trace includes
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "wpspagestepthreenumberTraces.h"
#endif

// Local constants
#define KMaxPINLength 8


/*!
 * Constructor for WPS page three number entry mode
 * 
 * \param WpsWizardPrivate* Pointer to the WPS wizard private implementation 
 */
WpsPageStepThreeNumber::WpsPageStepThreeNumber(WpsWizardPrivate* parent) :
    WpsWizardPage(parent), mWidget(NULL), mRadio(NULL), mValid(true)
{
OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_WPSPAGESTEPTHREENUMBER_ENTRY, this)
OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_WPSPAGESTEPTHREENUMBER_EXIT, this)

}


/*!
 * Destructor
 */
WpsPageStepThreeNumber::~WpsPageStepThreeNumber()
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_WPSPAGESTEPTHREENUMBER_ENTRY, this)

    delete mWidget;

OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_WPSPAGESTEPTHREENUMBER_EXIT, this)

}


/*!
  * Loads the page with all the widgets
  * 
  * \return HbWidget* Returns the view widget
 */
HbWidget* WpsPageStepThreeNumber::initializePage()
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_INITIALIZEPAGE_ENTRY, this)

    if (!mWidget) {
        bool ok;
        HbDocumentLoader loader;
        loader.load(":/docml/occ_wps_P3.docml", &ok);
        Q_ASSERT_X(ok, "WPS Wizard", "Invalid docml file");

        mWidget = qobject_cast<HbWidget*> (loader.findWidget("occ_wps_P3"));
        Q_ASSERT_X(mWidget != 0, "WPS Wizard", "View not found");

        mHeading = qobject_cast<HbLabel*> (loader.findWidget("label_heading"));
        Q_ASSERT_X(mHeading != 0, "WPS wizard", "Header not found");

        int randomNumber = computeRandNumber();
        mHeading->setPlainText(hbTrId("txt_occ_dialog_enter_1_on_the_wireless_station_t").arg(
            randomNumber));
        mWizard->setPin(randomNumber);
        mWizard->enableNextButton(true);
    }
    OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_INITIALIZEPAGE_EXIT, this)

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
int WpsPageStepThreeNumber::nextId(bool &removeFromStack) const
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_NEXTID_ENTRY, this)

    int id = WlanWizardPage::PageNone;
    removeFromStack = false;
    id = WpsWizardPage::PageWpsWizardStep4;
    OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_NEXTID_EXIT, this)

    return id;
}

/*!
 * Determines the Number of steps to move backwards when 'Prev' Button
 * is clicked
 * 
 * \return int Number of pages to move backwards
*/
int WpsPageStepThreeNumber::stepsBackwards()
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_STEPSBACKWARDS_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_STEPBACKWARDS_EXIT, this)

    return (PageWpsWizardStep3_Number - PageWpsWizardStep2) - 1;
}

/*!
  * Callback when the previous button is clicked
 */
void WpsPageStepThreeNumber::previousTriggered()
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_PREVIOUSTRIGGERED_ENTRY, this)

    mWizard->setPin(0);
OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_PREVIOUSTRIGGERED_EXIT, this)

}

/*!
 * CallBack when the cancel button is clicked
*/
void WpsPageStepThreeNumber::cancelTriggered()
{
    OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_CANCELTRIGGERED_ENTRY, this)

    mWizard->setPin(0);
OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_CANCELTRIGGERED_EXIT, this)

}

/*!
 * Validates the content of the pages
 * 
 * \return bool Indicating the result of the operation
*/
bool WpsPageStepThreeNumber::validate() const
{

    OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_VALIDATE_ENTRY, this)
    OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_VALIDATE_EXIT, this)

    return mValid;
}

/*!
 * Computes the Random number based on current system time
 * 
 * \return int Returns the generated random number
 */
int WpsPageStepThreeNumber::computeRandNumber()
{

    OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_COMPUTERANDNUMBER_ENTRY, this)

    int pin = 0;
    QTime time(0, 0);
    int seed = time.secsTo(QTime::currentTime());
    do {
        qsrand(seed);
        pin = qrand();
    } while (pin < (10 ^ (KMaxPINLength - 2)) || (((pin / 1000000) % 10)) == 0);
    //last digit is checksum, so we need 7 digits
    //and the first shouldn't be 0
    pin = pin % 10000000;
    TInt checkSum = computeCheckSum(pin);
    pin *= 10;
    pin += checkSum;
    OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_COMPUTERANDNUMBER_EXIT, this)
    return pin;
}

/*!
 * Computes the checksum for a given pin
 * 
 * \return Returns the generated checksum
 */
int WpsPageStepThreeNumber::computeCheckSum(int aPin)
{

    OstTraceFunctionEntry1(WPSPAGESTEPTHREENUMBER_COMPUTECHECKSUM_ENTRY, this)

    int accum = 0;
    aPin *= 10;
    accum += 3 * ((aPin / 10000000) % 10);
    accum += 1 * ((aPin / 1000000) % 10);
    accum += 3 * ((aPin / 100000) % 10);
    accum += 1 * ((aPin / 10000) % 10);
    accum += 3 * ((aPin / 1000) % 10);
    accum += 1 * ((aPin / 100) % 10);
    accum += 3 * ((aPin / 10) % 10);
    int digit = (accum % 10);
    OstTraceFunctionExit1(WPSPAGESTEPTHREENUMBER_COMPUTECHECKSUM_EXIT, this)
    return (10 - digit) % 10;
}

