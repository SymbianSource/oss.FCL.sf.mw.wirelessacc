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
 *   WLAN Wizard Page: Manual Network mode query page.  
 *
 */

/*
 * %version: 8 %
 */

#ifndef WLANWIZARDPAGENETWORKMODE_H
#define WLANWIZARDPAGENETWORKMODE_H

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

class WlanWizardPageNetworkMode : public WlanWizardPageInternal
{
    Q_OBJECT
public:
    
    /*!
     * Constructor method for the network mode view object
     * @param parent pointer to parent object.
     */
    WlanWizardPageNetworkMode(WlanWizardPrivate* parent);
    
    /*!
     * Destructor for the network mode view object.
     */
    ~WlanWizardPageNetworkMode();

    
public: // From WlanWizardPageInternal
    /*!
     * Page initialization procedures. Inherited from WlanWizardPage.
     */
    HbWidget* initializePage();

    /*!
     * Evaluates the network mode selection and sets the configuration in
     * the wlanwizard.
     * @param removeFromStack output parameter that returns false
     * @return WlanWizardPageSecurityMode page id.
     */
    int nextId(bool &removeFromStack) const;
    
    /*!
     * This method is overrides the default implementation from WlanWizardPage.
     * It indicates whether the Next-button should be enabled or not.
     * @return true, if a mode has been selected.
     */
    bool showPage();

public slots:
    /*!
     * Is invoked when user selects a mode from the radio button list.
     * (HbRadioButtonList's itemSelected-signal)
     */
    void itemSelected(int index);

    /*!
     * Loads the document orientation information from occ_add_wlan_02_03.docml.
     * This is called each time phone orientation changes.
     * @param orientation indicates whether the phone is in portrait or
     * landscape mode.
     */
    void loadDocml(Qt::Orientation orientation);
    
private:
    
    /*!
     * A support function to map the radio button list to a generic network
     * mode list. This enables the changing of button order without it
     * affecting the entire class.
     */
    void addToList(QStringList &list, const QString &item, int mode, bool isHidden);
    
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
     * List of network modes. This list is tied to the radio buttons during
     * the initialization of the page.
     */
    QList<int> mNetworkModes;
    
    /*!
     * List of publicity modes. This list is tied to the radio buttons during
     * the initialization of the page.
     */
    QList<bool> mIsHidden;
};

/*! @} */

#endif
