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
 *   EAP Wizard Page: New PAC store password
 *
 */

// System includes
#include <HbDocumentLoader>
#include <HbMainWindow>
#include <HbWidget>
#include <HbLineEdit>
#include <HbLabel>
#include <eapqtexpandedeaptype.h>
#include <eapqtconfiginterface.h>
#include <eapqtconfig.h>
#include <eapqtvalidator.h>

// User includes
#include "wlanwizardhelper.h"
#include "eapwizardpagepacstorepasswordconfirm.h"
#include "eapwizard_p.h"

/*!
   \class EapWizardPagePacStorePasswordConfirm
   \brief Implements EAP wizard page: Confirm PAC store password
 */

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========


/*!
   Constructor.
   
   @param [in] parent Pointer to EAP Wizard private implementation.
 */
EapWizardPagePacStorePasswordConfirm::EapWizardPagePacStorePasswordConfirm(
    EapWizardPrivate* parent) :
    EapWizardPage(parent),
    mDocumentLoader(NULL),
    mValidator(NULL),
    mWidget(NULL),
    mEditPasswordConfirm(NULL),
    mLabelError(NULL)
{
}

/*!
   Destructor.
 */
EapWizardPagePacStorePasswordConfirm::~EapWizardPagePacStorePasswordConfirm()
{
}

/*!
   See WlanWizardPage. 
 */
HbWidget* EapWizardPagePacStorePasswordConfirm::initializePage()
{
    if (!mWidget) {
        bool ok;
        mDocumentLoader.reset(
            new HbDocumentLoader(mWizard->wizardHelper()->mainWindow()));
        mDocumentLoader->load(":/docml/occ_eap_wizard_08.docml", &ok);
        Q_ASSERT(ok);
        
        loadDocmlSection(mWizard->wizardHelper()->mainWindow()->orientation());

        mWidget = qobject_cast<HbWidget*> (mDocumentLoader->findWidget("view"));
        Q_ASSERT(mWidget);

        mEditPasswordConfirm = qobject_cast<HbLineEdit*> (mDocumentLoader->findWidget(
            "lineEditPasswordConfirm"));
        Q_ASSERT(mEditPasswordConfirm);

        mLabelError = qobject_cast<HbLabel*> (mDocumentLoader->findWidget("labelError"));
        Q_ASSERT(mLabelError);
        
        mValidator.reset(
            mWizard->eapConfigurationInterface()->validatorPacStore(
                EapQtPacStoreConfig::PacStorePasswordConfirmation));
        Q_ASSERT(mValidator.data());
        
        mLabelError->setPlainText("");
        // Configurate editor properties and configurations
        mValidator->updateEditor(mEditPasswordConfirm);
        
        mEditPasswordConfirm->installEventFilter(this);
        
        ok = connect(
            mWizard->wizardHelper()->mainWindow(),
            SIGNAL(orientationChanged(Qt::Orientation)), 
            this, 
            SLOT(loadDocmlSection(Qt::Orientation)));
        Q_ASSERT(ok);
    }

    return mWidget;
}


/*!
   Loads the required orientation of docml.
   
   @param [in] orientation Orientation to be loaded. 
 */
void EapWizardPagePacStorePasswordConfirm::loadDocmlSection(Qt::Orientation orientation)
{
    EapWizardPage::loadDocmlSection(
        mDocumentLoader.data(),
        orientation,
        ":/docml/occ_eap_wizard_08.docml", 
        "portrait_section",
        "landscape_section");
}

/*!
   See WlanWizardPage.
   
   @return next wizard page: 
   - EapWizardPage::PageUsernamePassword: When password is ok
   - WlanWizardPage::PageNone: When password is not ok
 */
int EapWizardPagePacStorePasswordConfirm::nextId() const
{
    int id;
    if (mValidator->validate(mEditPasswordConfirm->text()) == 
        EapQtValidator::StatusOk) {
        
        mWizard->setConfigurations(
            EapWizardPrivate::PacStorePassword,
            mEditPasswordConfirm->text());
        
        id = EapWizardPage::PageUsernamePassword;
    } else {
        id = WlanWizardPage::PageNone;
        mLabelError->setPlainText(hbTrId("txt_occ_info_incorrect_password"));
    }

    return id;
}

/*!
   See WlanWizardPage::showPage().
   
   Sets the focus to line editor, opens virtual keyboard.
   
   @return true, content is valid always.
 */
bool EapWizardPagePacStorePasswordConfirm::showPage()
{
    mEditPasswordConfirm->setFocus();
    return true;
}

/*!
   Event filter for HbLineEdit. When editor gets focus a possible error text
   is cleared.
   
   @param [in] obj Pointer to object to where the event is sent
   @param [in] event Event
 */
bool EapWizardPagePacStorePasswordConfirm::eventFilter(
    QObject *obj, QEvent *event)
{
    if (obj == mEditPasswordConfirm && event->type() == QEvent::FocusIn) {
        mLabelError->setPlainText("");
    }
    return false;
}
