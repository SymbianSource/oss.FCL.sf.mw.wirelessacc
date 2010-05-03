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

#ifndef WLANINDICATORPLUGIN_H
#define WLANINDICATORPLUGIN_H

#include <QtCore/QVariant>
#include <QtCore/QProcess>
#include <hbindicatorplugininterface.h>
#include <hbindicatorinterface.h>

const int wlanNotConnected = 0;
const int wlanConnected = 1;

class WlanIndicatorPlugin : public HbIndicatorInterface, public HbIndicatorPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(HbIndicatorPluginInterface)
    
#ifdef WLANINDICATOR_UT    
    friend class WlanIndicatorUtCases;
#endif
    
public:
    WlanIndicatorPlugin();
    ~WlanIndicatorPlugin();

public:
    //from HbIndicatorPluginInterface
    QStringList indicatorTypes() const;
    bool accessAllowed(const QString &indicatorType,
                       const QVariantMap &securityInfo ) const;
    HbIndicatorInterface* createIndicator(const QString &indicatorType);
    int error() const;
public:
    //from HbIndicatorInterface
    bool handleClientRequest (RequestType type, const QVariant &parameter);
    bool handleInteraction (InteractionType type); 
    QVariant indicatorData(int role) const;

private slots:
    void processError(QProcess::ProcessError err); // handler for error codes
    
private:
    
    Q_DISABLE_COPY(WlanIndicatorPlugin)
    QStringList mIndicatorTypes;  // Type of indicator
    int mError;                   // Indicator error
    QVariant mParameter;          // Used to contain the status of wlan    
    QProcess process;             // Process to start external programs
};
#endif //WLANINDICATORPLUGIN_H
