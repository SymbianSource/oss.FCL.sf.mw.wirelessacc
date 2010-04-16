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
* Description: Step 3 Button press mode
*
*/

#ifndef WPSWIZARDSTEPTHREEBUTTON_H_
#define WPSWIZARDSTEPTHREEBUTTON_H_

// System includes
#include <QObject>

// User includes
#include "wpswizardpage.h"

// Forward declarations
class HbRadioButtonList;
class HbWidget;
class HbLabel;
class WpsWizardPrivate;

// External data types

// Constants

// Class declaration
class WpsPageStepThreeButton : public WpsWizardPage
    {
Q_OBJECT
public:
    WpsPageStepThreeButton(WpsWizardPrivate* parent);
    ~WpsPageStepThreeButton();

public:
    /*!
     * Creates a visualization of the page.
     */
    HbWidget* initializePage();
    /*!
     * Returns id of next page. updates settings EapWizard.
     */
    int nextId(bool &removeFromStack) const;
    /*!
     * Returns how many steps should be gone backwards. 
     */
    int stepsBackwards();
    /*!
     * This method is called when Previous button has been pressed.
     */
    void previousTriggered();
    /*!
     * This method is called when Cancel button has been pressed.
     */
    void cancelTriggered();
    /*!
     * This method is called when a visualization is displayed to detect
     * whether next button should be enabled or not.
     */
    bool validate() const;
    
public slots:

private:
    Q_DISABLE_COPY(WpsPageStepThreeButton)
    HbWidget *mWidget;
    HbRadioButtonList *mRadio;
    HbLabel *mTitle;
    HbLabel *mHeading;
    bool mValid;
    };

/*! @} */

#endif /* WPSWIZARDSTEPTHREEBUTTON_H_ */
