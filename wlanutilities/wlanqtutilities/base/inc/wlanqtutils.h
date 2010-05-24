/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* WLAN Qt Utilities interface.
*/

#ifndef WLANQTUTILS_H
#define WLANQTUTILS_H

// System includes

#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>

// User includes

// Forward declarations

class WlanQtUtilsIap;
class WlanQtUtilsAp;
class WlanQtUtilsPrivate;

// External data types

// Constants

//! Library interface export macro
#ifdef BUILD_WLANQTUTILITIES_DLL
#define WLANQTUTILS_EXPORT Q_DECL_EXPORT
#else
#define WLANQTUTILS_EXPORT Q_DECL_IMPORT
#endif

// Class declaration

class WLANQTUTILS_EXPORT WlanQtUtils : public QObject
{
    Q_OBJECT

public:

    // Data types

    //! "None" IAP ID value (e.g. for "not found" cases)
    static const int IapIdNone = -1;

    /*!
        WLAN connection status.
        Remember to update traces/trace.properties when modifying this enum.
    */        
    enum ConnStatus {
        ConnStatusNone = 0,         //!< Reserved.
        ConnStatusConnecting,       //!< Connecting.
        ConnStatusConnected,        //!< Connected.
        ConnStatusDisconnected      //!< Disconnected.
    };
    
    WlanQtUtils();
    
    ~WlanQtUtils();

    void scanWlans();
    
    void scanWlanAps();
   
    void scanWlanDirect(const QString &ssid);

    void stopWlanScan();

    void availableWlans(
        QList< QSharedPointer<WlanQtUtilsIap> > &wlanIapList,
        QList< QSharedPointer<WlanQtUtilsAp> > &wlanApList) const;
    
    void availableWlanAps(
        QList< QSharedPointer<WlanQtUtilsAp> > &wlanApList) const;
    
    int createIap(const WlanQtUtilsAp *wlanAp);

    bool updateIap(int iapId, const WlanQtUtilsAp *wlanAp);

    void deleteIap(int iapId);

    void connectIap(int iapId, bool runIct = false);

    void disconnectIap(int iapId);

    ConnStatus connectionStatus() const;
    
    int activeIap() const;
    
    QString iapName(int iapId) const;

signals:

    /*!
        Signal indicating that WLAN scan results are available. 
    */
    void wlanScanReady();
    
    /*!
        Signal indicating that WLAN scan results are available when scanning
        is requested with scanWlanAps() method.
    */
    void wlanScanApReady();
    
    /*!
        Signal indicating that WLAN scan results are available when scanning
        is requested with scanWlanDirect() method.
    */
    void wlanScanDirectReady();
    
    /*!
        Signal indicating that new WLAN network has been opened. 

        @param [in] iapId ID of the opened IAP.
    */
    void wlanNetworkOpened(int iapId);
    
    /*!
        Signal indicating that a WLAN network has been closed.

        @param [in] iapId ID of the closed IAP.
        @param [in] reason RConnection::Start() error code.
                           This is the reason of the closure if user requested
                           connection creation and it failed.
                           Otherwise KErrNone.
    */
    void wlanNetworkClosed(int iapId, int reason);

    /*!
        Signal indicating result of finished ICT run.

        @param [in] iapId ID of IAP ICT was run for.
        @param [in] result True: ICT passed, False: ICT failed.
    */
    void ictResult(int iapId, bool result);
    
public slots:

protected:

protected slots:

private:

    Q_DISABLE_COPY(WlanQtUtils)

private slots:

private: // data

    //! Pointer to private implementation
    QScopedPointer<WlanQtUtilsPrivate> d_ptr;

    // Friend classes
    
    // This is defined as a friend class in order to be able to emit public 
    // signals directly from private implementation code.
    friend class WlanQtUtilsPrivate;

    // This is defined as a friend class in order to be able to
    // call event handlers of wrappers from test code.
    friend class TestWlanQtUtils;
};

#endif // WLANQTUTILS_H
