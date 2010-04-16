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
* Description: WPS wizard page step 4
*
*/
#ifndef WPSWIZARDSTEPFOUR_H_
#define WPSWIZARDSTEPFOUR_H_


// System includes
#include <QObject>
#include<e32base.h>

// User includes
#include "wpswizardpage.h"
#include "mwpsactiverunnercallback.h"
#include "cwpsactiverunner.h"

// Forward declarations
class HbRadioButtonList;
class HbWidget;
class HbLabel;
class HbProgressBar;
class WpsWizardPrivate;

// External data types
// Constants

// Class declaration
class WpsPageStepFour : public WpsWizardPage, public MWpsActiveRunnerCallback
    {
Q_OBJECT
public:
    WpsPageStepFour(WpsWizardPrivate* parent);
    ~WpsPageStepFour();

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
    /*!
     * From mwpsactiverunnercallback.h
     * This method is called from the active runner class to notify the end
     * of operation
     */
    void WpsActiveRunnerStopped(
            QList<TWlanProtectedSetupCredentialAttribute>& aCredentials,
            TInt aError);

public slots:

private:
    Q_DISABLE_COPY(WpsPageStepFour)
    void StartWpsRunner();

    HbWidget *mWidget;
    HbRadioButtonList *mRadio;
    HbLabel *mTitle;
    HbLabel *mHeading;
    HbProgressBar *mProgressBar;
    CWpsActiveRunner *mWpsActiveRunner;
    int mWpsErrorCode;
    int countCredentials;
    bool mValid;

    };

/*! @} */

#endif /* WPSWIZARDSTEPFOUR_H_ */
