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
 *   WPS Wizard: Private implementation.
 *
 */

/*
 * %version: 1 %
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
#include "wpswizardprivateTraces.h"
#endif

// User includes
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
 * Constructor for the private implementation of wps wiazard
 * 
 * \param WpsWizard* pointer to the framework object
 * \param WlanWizardHelpper * pointer to the wizard helpper class 
 * 
 */
WpsWizardPrivate::WpsWizardPrivate(WpsWizard* wizard,
        WlanWizardHelpper *wizardHelpper, int bearerType, int referenceId) :
    q_ptr(wizard), mBearerType(bearerType), mReferenceId(referenceId),
            mWizardHelpper(wizardHelpper), mRandomNumber(0)

    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_WPSWIZARDPRIVATE_ENTRY, this);

    // TODO: load localization file
    mTranslator = new QTranslator(this);
    mTranslator->load(":/loc/wpswizardplugin.qm");
    qApp->installTranslator(mTranslator);

    createPages();

    OstTraceFunctionExit1(WPSWIZARDPRIVATE_WPSWIZARDPRIVATE_EXIT, this);
    }

/*!
 * Destructor
 */
WpsWizardPrivate::~WpsWizardPrivate()
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_WPSWIZARDPRIVATE_ENTRY, this);
    // have to delete the memory that was allocated in this object
    mIapList.clear();
    delete mTranslator;
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_WPSWIZARDPRIVATE_EXIT, this);
    }

/*!
 * Provides the caller with the functionality to enable/disable the 'Next' button
 * 
 * \param bool to indicate whether to enable or disable the 'next button'
 */
void WpsWizardPrivate::enableNextButton(bool enable)
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_ENABLENEXTBUTTON_ENTRY, this);
    mWizardHelpper->enableNextButton(enable);
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_ENABLENEXTBUTTON_EXIT, this);
    }

/*!
 * Provides the functionality to automatically move to the next page.
 */
void WpsWizardPrivate::nextPage()
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_NEXTPAGE_ENTRY, this);
    mWizardHelpper->nextPage();
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_NEXTPAGE_EXIT, this);
    }

/*!
 * Provides the functionality to store the recieved settings
 * 
 * \param int indicates the index in the array of received settings to be stored
 */
void WpsWizardPrivate::storeSettings(int aIndex)
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_STORESETTINGS_ENTRY, this);
    mWpsAttr = mIapList[aIndex];

    QString ssid = QString::fromUtf8((const char*) mWpsAttr.iSsid.Ptr(),
            mWpsAttr.iSsid.Length());

    mWizardHelpper->setConfigurations(WlanWizardHelpper::ConfSsid, ssid);
    mWizardHelpper->setConfigurations(WlanWizardHelpper::ConfNetworkMode,
            mWpsAttr.iOperatingMode);
    mWizardHelpper->setConfigurations(WlanWizardHelpper::ConfSecurityMode,
            mWpsAttr.iSecurityMode);
    mWizardHelpper->setConfigurations(WlanWizardHelpper::ConfKeyWepDefault,
            mWpsAttr.iWepDefaultKey);
    QString str = QString::fromUtf8((const char*) mWpsAttr.iWepKey1.Ptr(),
        mWpsAttr.iWepKey1.Length());
    mWizardHelpper->setConfigurations(WlanWizardHelpper::ConfKeyWep1, str);
    str = QString::fromUtf8((const char*) mWpsAttr.iWepKey2.Ptr(), mWpsAttr.iWepKey2.Length());
    mWizardHelpper->setConfigurations(WlanWizardHelpper::ConfKeyWep2, str);
    str = QString::fromUtf8((const char*) mWpsAttr.iWepKey3.Ptr(), mWpsAttr.iWepKey3.Length());
    mWizardHelpper->setConfigurations(WlanWizardHelpper::ConfKeyWep3, str);
    str = QString::fromUtf8((const char*) mWpsAttr.iWepKey4.Ptr(), mWpsAttr.iWepKey4.Length());
    mWizardHelpper->setConfigurations(WlanWizardHelpper::ConfKeyWep4, str);
            
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_STORESETTINGS_EXIT, this);
    }

/*!
 * Provides the functionality to retrieve the ssid of the network to connect
 * \return returns the SSID as a QString
 */
QString WpsWizardPrivate::getSsid()
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_GETSSID_ENTRY, this);
    QString ssid;
    ssid = mWizardHelpper->configurations(WlanWizardHelpper::ConfSsid).toString();
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_GETSSID_EXIT, this);
    return ssid;
    }

/*!
 * Provides functionality for the caller to store the array of recieved settings from AP
 * \param QList<TWlanProtectedSetupCredentialAttribute>& received settings
 * 
 */
void WpsWizardPrivate::storeSettingsArray(QList<
        TWlanProtectedSetupCredentialAttribute>& aIapList)
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_STORESETTINGSARRAY_ENTRY, this);
    mIapList = aIapList;
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_STORESETTINGSARRAY_EXIT, this);
    }

/*!
 * Returns the array of settings received from the AP
 * 
 * \return Array containing the settings
 */
QList<TWlanProtectedSetupCredentialAttribute> WpsWizardPrivate::getSettingsArray()
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_GETSETTINGSARRAY_ENTRY, this);
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_GETSETTINGSARRAY_EXIT, this);
    return mIapList;
    }



/*!
 * Creates the required pages for WPS setup
 */
void WpsWizardPrivate::createPages()
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_CREATEPAGES_ENTRY, this);
    mWizardHelpper->addPage(WpsWizardPage::PageWpsWizardStep2,
            new WpsPageStepTwo(this));
    mWizardHelpper->addPage(WpsWizardPage::PageWpsWizardStep3_Button,
            new WpsPageStepThreeButton(this));
    mWizardHelpper->addPage(WpsWizardPage::PageWpsWizardStep3_Number,
            new WpsPageStepThreeNumber(this));
    mWizardHelpper->addPage(WpsWizardPage::PageWpsWizardStep4,
            new WpsPageStepFour(this));
    mWizardHelpper->addPage(WpsWizardPage::PageWpsWizardStep5,
            new WpsPageStepFive(this));
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_CREATEPAGES_EXIT, this);
    }

/*!
 * Provides with the functionality to store the PIN generated
 * 
 * \param int The generated PIN code to be used in succesive steps
 * 
 */
void WpsWizardPrivate::setPin(int random)
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_SETPIN_ENTRY, this);
    mRandomNumber = random;
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_SETPIN_EXIT, this);
    }

/*!
 * Returns the PIN code stored in one of the WPS Pages
 * 
 * \return PIN code to be used in the WPS operation.
 */
int WpsWizardPrivate::getPin()
    {
    OstTraceFunctionEntry1(WPSWIZARDPRIVATE_GETPIN_ENTRY, this);
    OstTraceFunctionExit1(WPSWIZARDPRIVATE_GETPIN_EXIT, this);
    return mRandomNumber;
    
    }
