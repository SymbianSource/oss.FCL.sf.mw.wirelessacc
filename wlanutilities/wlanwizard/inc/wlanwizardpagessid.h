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
 *   WLAN Wizard Page: Ssid query
 *
 */

#ifndef WLANWIZARDPAGESSID_H
#define WLANWIZARDPAGESSID_H

// System includes

// User includes
#include "wlanwizardpageinternal.h"
#include "wlanwizardutils.h"

// Forward declarations
class WlanWizardPrivate;
class HbLabel;
class HbLineEdit;
class HbDocumentLoader;
class TestWlanWizardUi;

// External data types

// Constants

/*!
 * @addtogroup group_wlan_wizard
 * @{
 */

class WlanWizardPageSsid: public WlanWizardPageInternal
{
    Q_OBJECT
    friend class TestWlanWizardUi;
public:
    /*!
     * Constructor method for the SSID query view object.
     * @param parent pointer to parent object.
     */
    WlanWizardPageSsid(WlanWizardPrivate* parent);
    
    /*!
     * Destructor for the SSID query view object.
     */
    ~WlanWizardPageSsid();

    /*!
     * Page initialization procedures. Inherited from WlanWizardPage.
     */
    HbWidget* initializePage();
    
    /*!
     * This method is overrides the default implementation from WlanWizardPage.
     * It indicates whether the Next-button should be enabled or not.
     * @return true, if mSsid edit field is not empty.
     */
    virtual bool showPage();

    /*!
     * Validates the SSID selection and sets the configuration in
     * the wlanwizard.
     * @param removeFromStack output parameter that returns false.
     * @return WlanWizardPageScanning page id.
     */
    int nextId(bool &removeFromStack) const;
   
public slots:
    /*!
     * Loads the document orientation information from occ_add_wlan_01_04.docml
     * This is called each time phone orientation changes.
     * @param orientation indicates whether the phone is in portrait or
     * landscape mode.
     */
    void loadDocml(Qt::Orientation orientation);
    
    /*!
     * Executed when changes have been made to the mSsid field.
     * @param text is not used at this point.
     */
    void textChanged(const QString &text);
    
protected:
    /*!
     * Reimplements QObject function that is invoked on any event. This
     * function handles only a focus event to mSsid edit field.
     * @param obj contains the object, where the event originates from.
     * @param event contains the event id.
     */
    bool eventFilter(QObject *obj, QEvent *event);
    
private:
    /*!
     * Converts status identifier to plain-text QString format.
     * @param status indicates SSID status code.
     * @return status text.
     */
    QString SsidStatusToErrorString(WlanWizardUtils::SsidStatus status) const;
    
private:
    /*!
     * Pointer to the view.
     */
    HbWidget *mWidget;
    
    /*!
     * Pointer to the header label object.
     */
    HbLabel *mLabel;
    
    /*!
     * Pointer to the line edit object.
     */
    HbLineEdit *mSsid;
    
    /*!
     * Pointer to error label object.
     */
    HbLabel *mLabelError;
    
    /*!
     * Pointer to the document loader object.
     */
    HbDocumentLoader *mLoader;
};

/*! @} */

#endif
