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
*
*/

#ifndef WLANQTUTILSCONMONWRAPPER_H
#define WLANQTUTILSCONMONWRAPPER_H

// INCLUDES
#include <QObject>
#include <QList>
#include <QStringList>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

#include "wlanqtutilscommon.h"

// FORWARD DECLARATIONS
class WlanQtUtilsWlanAp;
class WlanQtUtilsActiveConn;
class ConnMonScanWlanAps;
class ConnMonConnInfo;
class ConnMonConnDisconnect;

// CLASS DECLARATION

/**
* ConMonWrapper class.
*/
class ConMonWrapper : public QObject
{
    Q_OBJECT
    
public:
        
    /**
     * Constructor.
     */
    ConMonWrapper(QObject *parent = 0);
    
    /**
    * Destructor.
    */
    ~ConMonWrapper();

    /**
     * Requests wlan scanning
     *
     * @return ???.
     */
    int scanAvailableWlanAPs();
    
    /**
     * Emits available WLANs to engine.
     * 
     * @param[in] availableWlanAPs Available WLAN access points found in scan.
     */
    void emitAvailableWlans(QList<WlanQtUtilsWlanAp *> &availableWlanAPs);

    /**
     * Emits signal indicating that a new connection has been created.
     * 
     * @param[in] connectionId Connection ID.
     */
    void emitConnCreatedEvent(uint connectionId);
    
    /**
     * Emits signal indicating that a connection has been deleted.
     * 
     * @param[in] connectionId Connection ID.
     */
    void emitConnDeletedEvent(uint connectionId);
    
    /**
     * Emits signal indicating that status of a connection has changed.
     * 
     * @param[in] connectionId Connection ID.
     * @param[in] connectionStatus Connection status.
     */
    void emitConnStatusEvent(uint connectionId, WlanQtUtilsConnectionStatus connectionStatus);

    /**
     * Return active connection information.
     * 
     * @return Information of active connection, if one exists.
     */ 
    WlanQtUtilsActiveConn *activeConnection();

    /**
     * Returns information of a connection with the given connection ID.
     * 
     * @param[in] connectionId Connection ID.
     * @return Information of the given connection, if one exists.
     */ 
    WlanQtUtilsActiveConn *connectionInfo(uint connectionId);

    /**
     * Stops given connection regardless of how many applications are using it.
     * 
     * @param[in] iapId IAP ID to disconnect.
     */ 
    void disconnectIap(int iapId);
        
signals:

    /**
     * Signal indicating available WLAN access points.
     * 
     * @param[in] availableWlans Available WLAN access points found in scan.
     */
    void availableWlanApsFromWrapper(QList<WlanQtUtilsWlanAp *> &availableWlans);
    
    /**
     * Signal indicating that a new connection has been created.
     * 
     * @param[in] connectionId Connection ID.
     */
    void connCreatedEventFromWrapper(uint connectionId);

    /**
     * Signal indicating that a connection has been deleted.
     * 
     * @param[in] connectionId Connection ID.
     */
    void connDeletedEventFromWrapper(uint connectionId);

    /**
     * Signal indicating that status of a connection has changed.
     * 
     * @param[in] connectionId Connection ID.
     * @param[in] connectionStatus Connection status.
     */
    void connStatusEventFromWrapper(uint connectionId, WlanQtUtilsConnectionStatus connectionStatus);

private: // Data
    
    /**
    * d_ptrScanWlans pointer to ConMonWrapperPrivate
    * Owned by ConMonWrapper object, instantiated in
    * constructor.
    */
    ConnMonScanWlanAps *d_ptrScanWlans;
    
    /**
    * d_ptrConnInfo pointer to ConMonWrapperPrivate
    * Owned by ConMonWrapper object, instantiated in
    * constructor.
    */
    ConnMonConnInfo *d_ptrConnInfo;

    /**
    * d_ptrConnDisconnect pointer to ConMonWrapperPrivate
    * Owned by ConMonWrapper object, instantiated in
    * constructor.
    */
    ConnMonConnDisconnect *d_ptrConnDisconnect;

private: // Friend classes
    // TestWlanQtUtils is defined as a friend class in order to be able to
    // call event handlers of wrappers.
    friend class TestWlanQtUtils;
};

QT_END_HEADER

QT_END_NAMESPACE

#endif /* WLANQTUTILSCONMONWRAPPER_H */

// End of file
