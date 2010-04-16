/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*/

#ifndef WLANQTUTILSCMMWRAPPER_H_
#define WLANQTUTILSCMMWRAPPER_H_

// INCLUDES
#include <QObject>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

// FORWARD DECLARATIONS
class CmmWrapperPrivate;
class WlanQtUtilsIap;
class WlanQtUtilsWlanIap;
class WlanQtUtilsWlanAp;

// CLASS DECLARATION

/**
 * CmmWrapper class.
 */
class CmmWrapper : public QObject
{
Q_OBJECT

public: // Constructor and destructor

    /**
     * Default constructor
     */
    CmmWrapper( QObject *parent = 0 );

    /**
     * Destructor
     */
    ~CmmWrapper();

public: 

    /**
     * Fetch all IAPs
     * 
     * @param[in/out] iapList List of IAPs
     */
    int fetchIaps( QList<WlanQtUtilsIap*>& iapList );

    /**
     * Create a new WLAN IAP as uncategorized
     * 
     * @param[in] wlanAp Information about the WLAN AP
     * @return New IAP class
     */
    WlanQtUtilsWlanIap *createWlanIap(const WlanQtUtilsWlanAp *wlanAp);
    
    /**
     * Move IAP to the Internet SNAP
     * 
     * @param[in] iapId Which IAP to move
     * @param[in] snapId Where to move it
     */
    void moveIapToInternetSnap(int iapId);
    
private: // Data

    /**
     * d_ptr pointer to CmmWrapperPrivate
     * Owned by CmmWrapper object, instantiated in
     * constructor.
     */
    CmmWrapperPrivate *d_ptr;
};

QT_END_HEADER

QT_END_NAMESPACE

#endif /* WLANQTUTILSCMMWRAPPER_H_ */

// End of file
