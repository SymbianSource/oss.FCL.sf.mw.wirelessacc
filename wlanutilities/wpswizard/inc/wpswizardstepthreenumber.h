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
* Description: Step 3 Number mode
*
*/

#ifndef WPSWIZARDSTEPTHREENUMBER_H_
#define WPSWIZARDSTEPTHREENUMBER_H_


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
class WpsPageStepThreeNumber : public WpsWizardPage
    {
    Q_OBJECT
    public:
    WpsPageStepThreeNumber(WpsWizardPrivate* parent);
    ~WpsPageStepThreeNumber();
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

    private:
    /*!
     * This method computes a random number based on current syste time
     */
    int computeRandNumber();
    /*!
     * This method computes the check sum on the input number
     */
    int computeCheckSum(int aPin);
    
    private slots:
    
    private: // data
    Q_DISABLE_COPY(WpsPageStepThreeNumber)
     HbWidget *mWidget;
     HbRadioButtonList *mRadio;
     HbLabel *mTitle;
     HbLabel *mHeading;
     bool mValid;
    };

/*! @} */

#endif /* WPSWIZARDSTEPTHREENUMBER_H_ */
