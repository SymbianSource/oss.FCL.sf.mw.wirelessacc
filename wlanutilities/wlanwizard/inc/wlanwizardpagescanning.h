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
*   WLAN Wizard Page: Scan processing
*/

#ifndef WLANWIZARDPAGESCANNING_H
#define WLANWIZARDPAGESCANNING_H

// System includes

// User includes
#include "wlanwizardpageinternal.h"
#include "wlanwizard.h"

// Forward declarations
class WlanWizardPrivate;
class HbLabel;
class HbProgressBar;
class HbDocumentLoader;

// External data types

// Constants

/*!
 * @addtogroup group_wlan_wizard
 * @{
 */

class WlanWizardPageScanning: public WlanWizardPageInternal
{
    Q_OBJECT
    
public:
    /*!
     * Constructor method for the scanning view object.
     * @param parent pointer to parent object.
     */
    WlanWizardPageScanning(WlanWizardPrivate* parent);
    
    /*!
     * Destructor for the scanning view object.
     */
    ~WlanWizardPageScanning();

    /*!
     * Page initialization procedures. Inherited from WlanWizardPage.
     * @see WlanWizardPage
     */
    HbWidget* initializePage();
    
    /*!
     * Validates the scan results and sets the configuration in the wlanwizard.
     * @param removeFromStack output parameter that returns true.
     * @return depending on the scan results, returns the appropriate view
     * identifier.
     */
    int nextId(bool &removeFromStack) const;
    
    /*!
     * Reimplements the default function in WlanWizardPage.
     * @return true
     * @see WlanWizardPage
     */
    bool requiresStartOperation();
    
    /*!
     * Reimplements the empty default function in WlanWizardPage. Initiates
     * AP scan.
     * @see WlanWizardPage
     */
    void startOperation();
    
    /*!
     * This method is overrides the default implementation from WlanWizardPage.
     * It indicates whether the Next-button should be enabled or not.
     * @return always false - the scanning proceeds to next window
     * automatically or not at all.
     */
    bool showPage();

public slots:
    /*!
     * Loads the document orientation information from occ_add_wlan_06.docml
     * This is called each time phone orientation changes.
     * @param orientation indicates whether the phone is in portrait or
     * landscape mode.
     */
    void loadDocml(Qt::Orientation orientation);
    
    /*!
     * Checks whether the view is active. If it is, execute wlanScanApReady. If
     * not, set mScanResultsAvailable to true.
     */
    void wlanScanResultPreCheck();
    
    /*!
     * Processes the direct scan results. If no results were found, the next
     * page is network mode query.
     * If match is found and scan result indicates
     * a) Open network, the next page is result processing.
     * b) Network with WEP/WPA/WPA2 protection, the next page is key query.
     * c) EAP protected network, the next page is EAP type query.
     * d) WPS support, the next page is WPS configuration page.
     * All matching results are considered hidden networks.
     */
    void wlanScanDirectReady();
    
private:
    /*!
     * Processes the Access Point scan results in search for an access point
     * with an SSID that matches the SSID given by the user. If no match is
     * found, a direct scan is initiated with the SSID given by the user.
     * If match is found and scan result indicates
     * a) Open network, the next page is result processing.
     * b) Network with WEP/WPA/WPA2 protection, the next page is key query.
     * c) EAP protected network, the next page is EAP type query.
     * d) WPS support, the next page is WPS configuration page.
     * All matching results are considered public networks.
     */
    void wlanScanApReady();

    /*!
     * Pointer to the view.
     */
    HbWidget *mWidget;

    /*!
     * Pointer to the label widget.
     */
    HbLabel *mLabel;

    /*!
     * Pointer to the progress bar widget.
     */
    HbProgressBar *mBar;

    /*!
     * Pointer to document loader object.
     */
    HbDocumentLoader *mLoader;

    /*!
     * The next page identifier. 
     */
    int mNextPageId;
    
    /*!
     * Indicator, whether AP scan results are available or not
     */
    bool mScanResultsAvailable;
};

/*! @} */

#endif
