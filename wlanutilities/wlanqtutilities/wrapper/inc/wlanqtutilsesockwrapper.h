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
* This is a header file for EsockWrapper class.
*/

#ifndef ESOCKWRAPPER_H_
#define ESOCKWRAPPER_H_

#include <QObject>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class EsockWrapperPrivate;

/**
 * Wrapper for connection handling.
 */
class EsockWrapper: public QObject
{
    Q_OBJECT
    
public:
    
    /**
     * Default constructor
     */
    EsockWrapper(QObject *parent = 0);
    
    /**
     * Destructor.
     */
    ~EsockWrapper();
    
public:

    /**
     * 
     */
    void updateConnection(bool isOpened);

public slots:
    
    /**
     * Starts connection creation to given IAP.
     * 
     * @param[in] iapId IAP ID.
     */
    void connectIap(int iapId);
    
    /**
     * Disconnects connection.
     */
    void disconnectIap();
    
signals:

    void connectionStatusFromWrapper(bool isOpened);   

private: // Data
    
    /** Private esock wrapper. */
    EsockWrapperPrivate *d_ptr;
};

QT_END_HEADER

QT_END_NAMESPACE

#endif /*ESOCKWRAPPER_H_*/

// End of file
