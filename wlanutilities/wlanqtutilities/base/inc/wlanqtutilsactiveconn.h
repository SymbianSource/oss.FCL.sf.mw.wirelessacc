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
* This is the header file for WlanQtUtilsActiveConn class.
*/

#ifndef WLANQTUTILSACTIVECONN_H
#define WLANQTUTILSACTIVECONN_H

#include <QObject>
#include "wlanqtutilscommon.h"


/**
 * This class represents an active connection.
 * TODO: consider deriving own classes for WLAN and Cellular connections.
 * Still, we can keep the same interface and avoid dynamic_casts.
 */
class WlanQtUtilsActiveConn : public QObject
{
public:

    /**
     * Constructor.
     */
    WlanQtUtilsActiveConn();

    /**
     * Destructor.
     */
    virtual ~WlanQtUtilsActiveConn();

    /**
     * Getter.
     * 
     * @return Connection ID specific for Connection Monitor Server.
     */
    uint connectionId() const;

    /**
     * Setter.
     * 
     * @param[in] connectionId Connection ID specific for Connection Monitor Server.
     */
    void setConnectionId(uint connectionId);

    /**
     * Getter.
     * 
     * @return IAP ID.
     */
    uint iapId() const;

    /**
     * Setter.
     * 
     * @param[in] iapId IAP ID.
     */
    void setIapId(uint iapId);

    /**
     * Getter.
     * 
     * @return Status of the connection.
     */
    WlanQtUtilsConnectionStatus connectionStatus() const;

    /**
     * Setter.
     * 
     * @param[in] connectionStatus Status of the connection.
     */
    void setConnectionStatus(WlanQtUtilsConnectionStatus connectionStatus);

    /**
     * Getter.
     * 
     * @return Bearer type.
     */
    WlanQtUtilsBearerType bearerType() const;

    /**
     * Setter.
     * 
     * @param[in] bearerType Bearer type.
     */
    void setBearerType(WlanQtUtilsBearerType bearerType);

private: // Data

    /** Connection Monitor Server specific connection ID. */
    uint connectionId_;
    /** IAP ID. */
    uint iapId_;

    /** Connection status. */
    WlanQtUtilsConnectionStatus connectionStatus_;
    /** Bearer type. */
    WlanQtUtilsBearerType bearerType_;
};

#endif /* WLANQTUTILSACTIVECONN_H */
