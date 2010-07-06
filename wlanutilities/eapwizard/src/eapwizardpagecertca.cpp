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
 *   EAP Wizard Page: Certificate Authority Selection.
 *
 */

// System includes
#include <HbDocumentLoader>
#include <HbMainWindow>
#include <HbWidget>
#include <HbRadioButtonList>
#include <HbLabel>
#include <eapqtpluginhandle.h>
#include <eapqtplugininfo.h>
#include <eapqtconfiginterface.h>
#include <eapqtcertificateinfo.h>

// User includes
#include "wlanwizardhelper.h"
#include "eapwizard_p.h"
#include "eapwizardpagecertca.h"

/*!
   \class EapWizardPageCertCa
   \brief Implements EAP wizard page: Certificate Authority selection
 */

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========


/*!
   Constructor.
   
   @param [in] parent Pointer to EAP Wizard private implementation.
 */
EapWizardPageCertCa::EapWizardPageCertCa(EapWizardPrivate* parent) :
    EapWizardPage(parent), 
    mDocumentLoader(NULL), 
    mWidget(NULL), 
    mTitle(NULL), 
    mCertList(NULL)
{
}

/*!
   Destructor.
 */
EapWizardPageCertCa::~EapWizardPageCertCa()
{
}

/*!
   See WlanWizardPage. 
 */
HbWidget* EapWizardPageCertCa::initializePage()
{
    if (!mWidget) {
        mDocumentLoader.reset( new HbDocumentLoader(
            mWizard->wizardHelper()->mainWindow()) );
        
        bool ok;
        mDocumentLoader->load(":/docml/occ_eap_wizard_01_02_04.docml", &ok);
        Q_ASSERT(ok);
        
        loadDocmlSection(mWizard->wizardHelper()->mainWindow()->orientation());
        
        mWidget = qobject_cast<HbWidget*> (
            mDocumentLoader->findWidget("occ_eap_wizard_01"));
        Q_ASSERT(mWidget);

        mCertList = qobject_cast<HbRadioButtonList*> (
            mDocumentLoader->findWidget("list"));
        Q_ASSERT(mCertList);
        
        mTitle = qobject_cast<HbLabel*> (mDocumentLoader->findWidget("title"));
        Q_ASSERT(mTitle);
        
        mTitle->setPlainText(
            hbTrId("txt_occ_title_select_authority_certificate"));
        
        ok = connect(
            mWizard->wizardHelper()->mainWindow(),
            SIGNAL(orientationChanged(Qt::Orientation)), 
            this, 
            SLOT(loadDocmlSection(Qt::Orientation)));
        Q_ASSERT(ok);
    }
    
    EapQtConfigInterface* confIf = mWizard->eapConfigurationInterface();
    // ignore return value
    confIf->updateCertificates();
    mCerts = confIf->certificateAuthorityCertificates();

    QStringList list;
    list << hbTrId("txt_occ_setlabel_val_select_automatically");

     for (int i = 0; i < mCerts.count(); ++i) {
         // TODO: workaround for BBHA-863EJN
         // graphics memory runs out for long lists and items
         QString tmp(mCerts.at(i).value(
             EapQtCertificateInfo::CertificateLabel).toString());
         tmp.truncate(10);
         list << tmp;
    }
    
    // If certificate list has been changed update the content otherwise do not
    // change it
    if (list != mCertList->items()) {
        mCertList->setItems(list);
        // Automatic is selected by default
        mCertList->setSelected(IndexForAutomatic);
    }
    return mWidget;
}

/*!
   Loads the required orientation of docml.
   
   @param [in] orientation Orientation to be loaded. 
 */
void EapWizardPageCertCa::loadDocmlSection(Qt::Orientation orientation)
{
    EapWizardPage::loadDocmlSection(
        mDocumentLoader.data(),
        orientation,
        ":/docml/occ_eap_wizard_01_02_04.docml",
        "portrait_section",
        "landscape_section");
}

/*!
   See WlanWizardPage.
   
   @return next wizard page:
   - EapWizardPage::PageCertificateUser: For EAP-TLS 
   - EapWizardPage::PageIdentity: For EAP-TTLS and PEAP
 */
int EapWizardPageCertCa::nextId() const
{
    int id = WlanWizardPage::PageNone;
    bool ok;
    int type = mWizard->configurations(EapWizardPrivate::OuterType).toInt(&ok);
    Q_ASSERT(ok);

    if (type == EapQtPluginHandle::PluginEapTls) {
        id = EapWizardPage::PageCertificateUser;
    } else {
        Q_ASSERT(type == EapQtPluginHandle::PluginEapTtls || 
            type == EapQtPluginHandle::PluginPeap);
        id = EapWizardPage::PageIdentity;
    }

    int selected = mCertList->selected();
    if (selected == IndexForAutomatic) {
        mWizard->setConfigurations(EapWizardPrivate::CertificateCa, QVariant());
    } else {
        mWizard->setConfigurations(
            EapWizardPrivate::CertificateCa, qVariantFromValue(mCerts.at(
                selected - AmountOfNonCertItems)));
    }

    return id;
}
