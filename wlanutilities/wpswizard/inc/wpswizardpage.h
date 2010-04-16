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
* Description: API for wizard pages
*
*/

#ifndef WPSWIZARDPAGE_H_
#define WPSWIZARDPAGE_H_


// System includes

// User includes
#include "wlanwizardpage.h"

// Forward declarations
class WpsWizardPrivate;
class HbWidget;


// External data types
// Constants

// Class declaration
class WpsWizardPage : public QObject, public WlanWizardPage
    {
public:
    enum WpsPageIds
        {
        PageWpsWizardStep2 = WlanWizardPage::PageWpsStart,
        PageWpsWizardStep3_Button,
        PageWpsWizardStep3_Number,
        PageWpsWizardStep4,
        PageWpsWizardStep5,
        PageWpsWizardStep6
        };
public:
    WpsWizardPage(WpsWizardPrivate* parent);
    virtual ~WpsWizardPage();

    /*!
     * Creates a visualization of the page.
     */
    virtual HbWidget* initializePage() = 0;
    /*!
     * Returns id of next page. updates settings EapWizard.
     */
    virtual int nextId(bool &removeFromStack) const = 0;
    /*!
     * Returns how many steps should be gone backwars. 
     */
    virtual int stepsBackwards();
    /*!
     * This method is called when a visualization is displayed to detect
     * whether next button should be enabled or not.
     */
    virtual bool validate() const;
    /*!
     * This method is called when Previous button has been pressed.
     */
    virtual void previousTriggered() = 0;
    /*!
     * This method is called when Cancel button has been pressed.
     */
    virtual void cancelTriggered() = 0;
    WpsWizardPrivate* mWizard;
private:
    Q_DISABLE_COPY(WpsWizardPage)
    };

/*! @} */

#endif /* WPSWIZARDPAGE_H_ */
