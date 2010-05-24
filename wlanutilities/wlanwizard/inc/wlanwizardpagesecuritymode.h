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
 *   WLAN Wizard Page: Security mode selection
 *
 */

/*
 * %version: 10 %
 */

#ifndef WLANWIZARDPAGESECURITYMODE_H
#define WLANWIZARDPAGESECURITYMODE_H

// System includes

// User includes
#include "wlanwizardpageinternal.h"

// Forward declarations
class HbRadioButtonList;
class HbLabel;
class WlanWizardPrivate;
class HbDocumentLoader;

// External data types

// Constants

/*!
 * @addtogroup group_wlan_wizard
 * @{
 */

class WlanWizardPageSecurityMode: public WlanWizardPageInternal
{
    Q_OBJECT
public:
    /*!
     * Constructor method for the Sequrity mode query view object.
     * @param parent pointer to parent object.
     */
    WlanWizardPageSecurityMode(WlanWizardPrivate* parent);
    
    /*!
     * Destructor for the Sequrity mode query view object.
     */
    ~WlanWizardPageSecurityMode();

    /*!
     * Page initialization procedures. Inherited from WlanWizardPage.
     * @see WlanWizardPage
     */
    HbWidget* initializePage();

    /*!
     * Validates the Sequrity more selection and sets the configuration in
     * the wlanwizard.
     * @param removeFromStack output parameter that returns false.
     * @return depending on the sequrity mode, returns the appropriate view
     * identifier.
     */
    int nextId(bool &removeFromStack) const;
    
    /*!
     * This method is overrides the default implementation from WlanWizardPage.
     * It indicates whether the Next-button should be enabled or not.
     * @return true, if a mode has been selected.
     */
    bool showPage();
    
private slots:
    /*!
     * Is invoked when user selects a mode from the radio button list.
     * (HbRadioButtonList's itemSelected-signal)
     * @param index of the selected item in the  radio button list. 
     */
    void itemSelected(int index);
    
    /*!
     * Loads the document orientation information from occ_add_wlan_02_03.docml
     * This is called each time phone orientation changes.
     * @param orientation indicates whether the phone is in portrait or
     * landscape mode.
     */
    void loadDocml(Qt::Orientation orientation);
    
private:
    
    /*!
     * Support function that creates the contents of the security mode list. 
     */
    void populateSecModeList();
    
    /*!
     * A support function to map the radio button list to a generic network
     * mode list. This enables the changing of button order without it
     * affecting the entire class.
     * @param list is a reference to a local list that will be placed as an
     * argument to the radio button list.
     * @param item is a reference to the text value that is appended to the
     * list
     * @param mode is the security mode associated with the item
     * @param page is the id of the next wizard page when this item is selected.
     * @param psk indicates whether password needs to be queried. 
     */
    void addToList(QStringList &list, const QString &item, int mode, int page,
        bool psk);
    
    /*!
     * Pointer to the view.
     */
    HbWidget *mWidget;

    /*!
     * Pointer to the radio button list object.
     */
    HbRadioButtonList *mList;

    /*!
     * Pointer to the label object.
     */
    HbLabel *mLabel;

    /*!
     * Pointer to document loader object.
     */
    HbDocumentLoader *mLoader;

    /*!
     * True, if a mode has been selected. False otherwise.
     */
    bool mValid;
    
    /*!
     * List of sequrity modes. This list is tied to the radio buttons during
     * the initialization of the page.
     */
    QList<int> mSecModes;
    
    /*!
     * List of page identifiers. This list is tied to the radio buttons during
     * the initialization of the page.
     */
    QList<int> mPageIds;
    
    /*!
     * List of password usage. This list is tied to the radio buttons during
     * the initialization of the page. True, if password is to be queried,
     * false otherwise
     */
    QList<bool> mUsePsk;
};

/*! @} */

#endif
