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
 *   EAP Wizard: API for wizard pages.
 *
 */

// System includes
#include <HbWidget>
#include <HbDocumentLoader>
#include <HbMainWindow>

// User includes
#include "eapwizardpage.h"
#include "eapwizard_p.h"

/*!
   \class EapWizardPage
   \brief Defines the interface for EAP specific wizard pages.
 */

/*!
   \fn HbWidget* EapWizardPage::initializePage()
   See WlanWizardPage.
 */

/*!
   \fn int EapWizardPage::nextId() const
   See WlanWizardPage.
 */

// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========


/*!
   Constructor.
   
   @param [in] parent Pointer to EAP Wizard private implementation.
 */
EapWizardPage::EapWizardPage(EapWizardPrivate* parent) :
    QObject(parent), 
    mWizard(parent)
{
}

/*!
   Destructor.
 */
EapWizardPage::~EapWizardPage()
{
    mWizard = NULL;
}

/*!
   See WlanWizardPage. 
 */
int EapWizardPage::nextId(bool &removeFromStack) const
{
    removeFromStack = false;
    return nextId();
}

/*!
   Loads requested section from given docml file.
   
   @param [in] loader Document loader for docml
   @param [in] orientation Orientation to be loaded
   @param [in] filename the name of docml filename
   @param [in] portraitSection the name of section to be loaded in portrait mode
   @param [in] landscapeSection the name of section to be loaded in landscape mode
 */
void EapWizardPage::loadDocmlSection(
    HbDocumentLoader *loader,
    Qt::Orientation orientation,
    const QString &filename,
    const QString &portraitSection,
    const QString &landscapeSection) const
{
    bool ok;
    
    // Then load the orientation specific section
    if (orientation == Qt::Horizontal) {
        loader->load(filename, landscapeSection, &ok);
        Q_ASSERT(ok);
    } else {
        Q_ASSERT(orientation == Qt::Vertical);
        loader->load(filename, portraitSection, &ok);
        Q_ASSERT(ok);
    }
}
