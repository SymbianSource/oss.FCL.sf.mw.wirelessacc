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
 *   EAP Wizard Page: Common Username & password query.
 *
 */

// System includes
#include <HbDocumentLoader>
#include <HbMainWindow>
#include <HbWidget>
#include <HbLineEdit>
#include <HbLabel>
#include <HbEditorInterface>
#include <HbParameterLengthLimiter>
#include <eapqtexpandedeaptype.h>
#include <eapqtpluginhandle.h>
#include <eapqtconfiginterface.h>
#include <eapqtvalidator.h>

// User includes
#include "wlanwizardhelper.h"
#include "eapwizardpageusernamepassword.h"
#include "eapwizard_p.h"

/*!
   \class EapWizardPageUsernamePassword
   \brief Implements EAP wizard page: Generic Username & Password page
 */

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========


/*!
   Constructor.
   
   @param [in] parent Pointer to EAP Wizard private implementation.
 */
EapWizardPageUsernamePassword::EapWizardPageUsernamePassword(
    EapWizardPrivate* parent) :
    EapWizardPage(parent), 
    mDocumentLoader(NULL), 
    mValidatorUsername(NULL), 
    mValidatorPassword(NULL),
    mWidget(NULL), 
    mEditUsername(NULL),
    mEditPassword(NULL), 
    mTitleUsername(NULL), 
    mTitlePassword(NULL)
{
}

/*!
   Destructor.
 */
EapWizardPageUsernamePassword::~EapWizardPageUsernamePassword()
{
}

/*!
   See WlanWizardPage. 
 */
HbWidget* EapWizardPageUsernamePassword::initializePage()
{
    if (!mWidget) {
        bool ok = true;
        mDocumentLoader.reset(
            new HbDocumentLoader(mWizard->wizardHelper()->mainWindow()));
        mDocumentLoader->load(":/docml/occ_eap_wizard_05_07.docml", &ok);
        Q_ASSERT(ok);
        loadDocmlSection(mWizard->wizardHelper()->mainWindow()->orientation());

        mWidget = qobject_cast<HbWidget*> (mDocumentLoader->findWidget("view"));
        Q_ASSERT(mWidget);

        mTitleUsername = qobject_cast<HbLabel*> (mDocumentLoader->findWidget("setlabel_55"));
        Q_ASSERT(mTitleUsername);

        mTitlePassword = qobject_cast<HbLabel*> (mDocumentLoader->findWidget("setlabel_56"));
        Q_ASSERT(mTitlePassword);

        mEditUsername = qobject_cast<HbLineEdit*> (mDocumentLoader->findWidget("lineEditUsername"));
        Q_ASSERT(mEditUsername);

        mEditPassword = qobject_cast<HbLineEdit*> (mDocumentLoader->findWidget("lineEditPassword"));
        Q_ASSERT(mEditPassword);
        
        mTitlePassword->setPlainText(hbTrId("txt_occ_setlabel_eap_password"));
        
        ok = connect(
            mWizard->wizardHelper()->mainWindow(),
            SIGNAL(orientationChanged(Qt::Orientation)), 
            this, 
            SLOT(loadDocmlSection(Qt::Orientation)));
        Q_ASSERT(ok);
        
        ok = connect(
            mEditUsername, SIGNAL(textChanged(const QString &)), 
            this, SLOT(textChanged(const QString &)));
        Q_ASSERT(ok);
        
        ok = connect(
            mEditPassword, SIGNAL(textChanged(const QString &)), 
            this, SLOT(textChanged(const QString &)));
        Q_ASSERT(ok);
    }

    int eapType = mWizard->configurations(EapWizardPrivate::OuterType).toInt();

    if (eapType != EapQtPluginHandle::PluginLeap) {
        eapType = mWizard->configurations(EapWizardPrivate::InnerType).toInt();
    }
    
    EapQtPluginHandle plugin(static_cast<EapQtPluginHandle::Plugin>(eapType));

    mValidatorUsername.reset(
        mWizard->eapConfigurationInterface()->validatorEap(
            plugin.type(), 
            EapQtConfig::Username));
    Q_ASSERT(mValidatorUsername.data());
    mValidatorUsername->updateEditor(mEditUsername);

    mValidatorPassword.reset(
        mWizard->eapConfigurationInterface()->validatorEap(
            plugin.type(), 
            EapQtConfig::Password));
    Q_ASSERT(mValidatorPassword.data());
    mValidatorPassword->updateEditor(mEditPassword);
    
    mTitleUsername->setPlainText(
        HbParameterLengthLimiter(
            hbTrId("txt_occ_setlabel_user_name_for_1")).arg(
                mWizard->eapTypeToString(eapType)));

    return mWidget;
}

/*!
   Loads the required orientation of docml.
   
   @param [in] orientation Orientation to be loaded. 
 */
void EapWizardPageUsernamePassword::loadDocmlSection(Qt::Orientation orientation)
{
    EapWizardPage::loadDocmlSection(
        mDocumentLoader.data(),
        orientation,
        ":/docml/occ_eap_wizard_05_07.docml",
        "portrait_section",
        "landscape_section");
}

/*!
   See WlanWizardPage.
   
   Validates the content of the page.
   
   @return true if content is valid.
 */
bool EapWizardPageUsernamePassword::showPage()
{
    bool valid = false;
    if (mValidatorUsername->validate(mEditUsername->text()) == 
        EapQtValidator::StatusOk && 
        mValidatorPassword->validate(mEditPassword->text()) == 
        EapQtValidator::StatusOk) {
        valid = true;
    }
    return valid;
}

/*!
   See WlanWizardPage.
   
   @return next wizard page: EapWizardPage::PageProcessSettings
 */
int EapWizardPageUsernamePassword::nextId() const
{
    mWizard->setConfigurations(EapWizardPrivate::Username, mEditUsername->text());
    mWizard->setConfigurations(EapWizardPrivate::Password, mEditPassword->text());
    return EapWizardPage::PageProcessSettings;
}

/*!
   Slot for textChanged() signal from HbLineEdit.
   
   @param [in] text NOT USED.
 */
void EapWizardPageUsernamePassword::textChanged(const QString & text )
{
    Q_UNUSED(text);
    mWizard->wizardHelper()->enableNextButton(showPage());
}
