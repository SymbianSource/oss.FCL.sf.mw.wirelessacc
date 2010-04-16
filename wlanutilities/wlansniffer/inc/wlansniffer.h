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

#ifndef WLANSNIFFER_H
#define WLANSNIFFER_H

#include <HbApplication>

class QTranslator;
class WlanSnifferMainWindow;
class WlanQtUtils;

class WlanSniffer : public HbApplication
    {
    Q_OBJECT
    
public:
    WlanSniffer(int argc, char* argv[]);
    virtual ~WlanSniffer();
    
    /**
     * This function (from QObject) handles timers. Initiates a new WLAN scan.
     * 
     * @param Unused.
     */
    void timerEvent(QTimerEvent *event);

    WlanQtUtils *wlanQtUtils() const;
    
private slots:
    void exitApplication();
    
private:
    QTranslator* mTranslator;
    WlanQtUtils *mWlanQtUtils;
    // Todo: why does the example use QSharedPointer for these?
    WlanSnifferMainWindow *mMainWindow; // owned
    int mScanTimerId;
    };

#endif

// End of File
