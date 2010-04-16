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
* Description: WPS wizard private implementation
*
*/

#ifndef WPSWIZARD_P_H_
#define WPSWIZARD_P_H_

// System includes
#include <qobject.h>
#include <qhash.h>
#include <qmap.h>
#include <wlanmgmtcommon.h>
#include <qtranslator.h>

// User includes
#include "wlanwizardhelpper.h"
#include "wpswizard.h"

// Forward declarations
class HbStackedWidget;
class HbWidget;
class HbLabel;
class HbView;
class HbAction;
class HbDocumentLoader;
class WlanWizardHelpper;


// External data types
// Constants

// Class declaration
class WpsWizardPrivate : public QObject
    {

    friend class WpsWizard;

public:
    WpsWizardPrivate(WpsWizard* wizard, WlanWizardHelpper *wizardHelpper, int bearerType, int referenceId);
    ~WpsWizardPrivate();
public:
    /*!
     * Based on the input either disables or enables next button
     */
    void enableNextButton(bool enable);
    /*!
     * Moves automatically to the next page
     */
    void nextPage();
    /*!
     * Returns the SSID of the network 
     */
    QString getSsid();
    /*!
     * Stores the settings received from the AP
     */
    void storeSettings(int aIndex);
    /*!
     * Stores multiple settings received from the AP
     */
    void storeSettingsArray(
            QList<TWlanProtectedSetupCredentialAttribute>& aCredentials);
    QList<TWlanProtectedSetupCredentialAttribute> getSettingsArray();
    /*!
     * Stores the generated PIN
     */
    void setPin(int random);
    /*!
     * Returns the generated PIN
     */
    int getPin();

private:
    Q_DISABLE_COPY(WpsWizardPrivate)
    /*!
     * Returns id of next page. updates settings EapWizard.
     */
    void createPages();

private:
    WpsWizard *q_ptr;
    WlanWizardHelpper *mWizardHelpper;
    QTranslator *mTranslator;
    TWlanProtectedSetupCredentialAttribute mWpsAttr;
    QList<TWlanProtectedSetupCredentialAttribute> mIapList;
    int mBearerType;
    int mReferenceId;
    int mRandomNumber;
    };

#endif /* WPSWIZARD_P_H_ */
