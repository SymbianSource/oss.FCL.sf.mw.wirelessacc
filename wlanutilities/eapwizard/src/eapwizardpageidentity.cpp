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
 *   EAP Wizard Page: EAP Identity.
 *
 */

// System includes
#include <HbDocumentLoader>
#include <HbMainWindow>
#include <HbWidget>
#include <HbCheckBox>
#include <HbLineEdit>
#include <HbLabel>
#include <HbEditorInterface>
#include <HbParameterLengthLimiter>
#include <eapqtexpandedeaptype.h>
#include <eapqtpluginhandle.h>
#include <eapqtplugininfo.h>
#include <eapqtconfiginterface.h>
#include <eapqtvalidator.h>

// User includes
#include "wlanwizardhelper.h"
#include "eapwizardpageidentity.h"
#include "eapwizard_p.h"

/*!
   \class EapWizardPageIdentity
   \brief Implements EAP wizard page: Identify for outer EAP type
 */

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========


/*!
   Constructor.
   
   @param [in] parent Pointer to EAP Wizard private implementation.
 */
EapWizardPageIdentity::EapWizardPageIdentity(EapWizardPrivate* parent) :
    EapWizardPage(parent), 
    mDocumentLoader(NULL), 
    mValidatorRealm(NULL),
    mValidatorUsername(NULL),
    mWidget(NULL), 
    mEditUsername(NULL), 
    mEditRealm(NULL), 
    mCheckUsername(NULL), 
    mCheckRealm(NULL), 
    mLabelRealm(NULL),
    mLabelUsername(NULL)
{
}

/*!
   Destructor.
 */
EapWizardPageIdentity::~EapWizardPageIdentity()
{
}

/*!
   See WlanWizardPage. 
 */
HbWidget* EapWizardPageIdentity::initializePage()
{
    if (!mWidget) {
        bool ok = true;
        mDocumentLoader.reset(
            new HbDocumentLoader(mWizard->wizardHelper()->mainWindow()));
        mDocumentLoader->load(":/docml/occ_eap_wizard_03.docml", &ok);
        Q_ASSERT(ok);

        loadDocmlSection(mWizard->wizardHelper()->mainWindow()->orientation());
        
        mWidget = qobject_cast<HbWidget*> (mDocumentLoader->findWidget("occ_eap_wizard_03"));
        Q_ASSERT(mWidget);

        mEditUsername = qobject_cast<HbLineEdit*> (mDocumentLoader->findWidget("lineEditUsername"));
        Q_ASSERT(mEditUsername);

        mEditRealm = qobject_cast<HbLineEdit*> (mDocumentLoader->findWidget("lineEditRealm"));
        Q_ASSERT(mEditRealm);

        mCheckUsername = qobject_cast<HbCheckBox*> (mDocumentLoader->findWidget("setlabel_53_val"));
        Q_ASSERT(mCheckUsername);

        mCheckRealm = qobject_cast<HbCheckBox*> (mDocumentLoader->findWidget("setlabel_54_val"));
        Q_ASSERT(mCheckRealm);
        
        mLabelUsername = qobject_cast<HbLabel*> (mDocumentLoader->findWidget("setlabel_53"));
        Q_ASSERT(mLabelUsername);
        
        mLabelRealm = qobject_cast<HbLabel*> (mDocumentLoader->findWidget("setlabel_54"));
        Q_ASSERT(mLabelRealm);
        
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
            mEditRealm, SIGNAL(textChanged(const QString &)),
            this, SLOT(textChanged(const QString &)));
        Q_ASSERT(ok);
        
        ok = connect(
            mCheckUsername, SIGNAL(stateChanged(int)), 
            this, SLOT(stateChanged(int)));
        Q_ASSERT(ok);
        
        ok = connect(
            mCheckRealm, SIGNAL(stateChanged(int)), 
            this, SLOT(stateChanged(int)));
        Q_ASSERT(ok);
        // by default both checkboxes are checked
    }
    EapQtPluginHandle plugin((EapQtPluginHandle::Plugin)     
        mWizard->configurations(EapWizardPrivate::OuterType).toInt() );

    mLabelUsername->setPlainText(
        HbParameterLengthLimiter(
            hbTrId("txt_occ_setlabel_user_name_for_1")).arg(
                mWizard->eapTypeToString(plugin.pluginId())));
    
    // Configure editors properties
    mValidatorUsername.reset( 
        mWizard->eapConfigurationInterface()->validatorEap(
            plugin.type(), 
            EapQtConfig::Username) );
    Q_ASSERT(mValidatorUsername.data());
    mValidatorUsername->updateEditor(mEditUsername);

    mValidatorRealm.reset( 
        mWizard->eapConfigurationInterface()->validatorEap(
            plugin.type(), 
            EapQtConfig::Realm) );
    Q_ASSERT(mValidatorRealm.data());
    mValidatorRealm->updateEditor(mEditRealm);

    return mWidget;
}

/*!
   Loads the required orientation of docml.
   
   @param [in] orientation Orientation to be loaded. 
 */
void EapWizardPageIdentity::loadDocmlSection(Qt::Orientation orientation)
{
    EapWizardPage::loadDocmlSection(
        mDocumentLoader.data(),
        orientation,
        ":/docml/occ_eap_wizard_03.docml",
        "portrait_section",
        "landscape_section");
}

/*!
   See WlanWizardPage.
   
   @return next wizard page:
   - WlanWizardPage::PageProcessSettings: For EAP-TLS 
   - EapWizardPage::PageInnerTypeEapTtls: For EAP-TTLS
   - EapWizardPage::PageInnerTypePeap: For PEAP
 */
int EapWizardPageIdentity::nextId() const
{
    int id = WlanWizardPage::PageNone;
    bool ok;
    int type = mWizard->configurations(EapWizardPrivate::OuterType).toInt(&ok);
    Q_ASSERT(ok);

    switch (type) {
    case EapQtPluginHandle::PluginEapTls:
        id = WlanWizardPage::PageProcessSettings;
        break;
        
    case EapQtPluginHandle::PluginEapTtls:
        id = EapWizardPage::PageInnerTypeEapTtls;
        break;
        
    case EapQtPluginHandle::PluginPeap:
        id = EapWizardPage::PageInnerTypePeap;
        break;
    }

    mWizard->setConfigurations(EapWizardPrivate::TunnelUsernameAutomatic,
        mCheckUsername->isChecked());
    
    mWizard->setConfigurations(
        EapWizardPrivate::TunnelUsername, mEditUsername->text());
    
    mWizard->setConfigurations(
        EapWizardPrivate::TunnelRealmAutomatic, mCheckRealm->isChecked());
    
    mWizard->setConfigurations(EapWizardPrivate::TunnelRealm, mEditRealm->text());

    return id;
}

/*!
   See WlanWizardPage.
   
   Validates the content of the page.
   
   @return true if content is valid.
 */
bool EapWizardPageIdentity::showPage()
{
    bool valid = false;

    if (validateGroup(mEditUsername, mCheckUsername, mValidatorUsername.data()) && 
        validateGroup(mEditRealm, mCheckRealm, mValidatorRealm.data())) {
        valid = true;
    }

    return valid;
}

/*!
   Slot for textChanged() signal from HbLineEdit.
   
   @param [in] text NOT USED.
 */
void EapWizardPageIdentity::textChanged(const QString & text)
{
    Q_UNUSED(text);
    mWizard->wizardHelper()->enableNextButton(showPage());
}

/*!
   Slot for stateChanged() signal from HbCheckBox.
   
   @param [in] state NOT USED.
 */
void EapWizardPageIdentity::stateChanged(int state )
{
    Q_UNUSED(state);
    mEditUsername->setEnabled(!mCheckUsername->isChecked());
    mEditRealm->setEnabled(!mCheckRealm->isChecked());
    mWizard->wizardHelper()->enableNextButton(showPage());
}

/*!
   Validates the settings group, that is line edit and checkbox using
   given validator.
   
   @param [in] edit pointer to line edit to which content must be validated
   @param [in] checkBox pointer to checkbox
   @param [in] validator pointer to validator which is used to validate the content
                    of the line editor.
                    
   @return true if validation is ok, false otherwise. 
 */
bool EapWizardPageIdentity::validateGroup(
    HbLineEdit *edit, 
    HbCheckBox *checkBox,
    EapQtValidator *validator) const
{
    bool status = false;
    if (checkBox->isChecked() || 
        checkBox->isChecked() == false && 
        EapQtValidator::StatusOk == validator->validate(edit->text())) {
        status = true;
    }
    return status;
}
