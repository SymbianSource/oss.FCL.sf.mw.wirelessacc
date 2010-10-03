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

#ifndef WLANINDICATORUTCASES_H
#define WLANINDICATORUTCASES_H

#include <QObject>
#include "wlanindicatorplugin.h"


class WlanIndicatorUtCases: public QObject
{
    Q_OBJECT

    public:
    
    WlanIndicatorUtCases();
    ~WlanIndicatorUtCases();
    
    private slots:

        // Test cases
        void initTestCase();
        void wlanNotConnected();
        void wlanConnected();
        void errorHandling();
        void differentIndicatorData();
        void handleInteractions();
        void clientParamatersHandler();
        void processErrorHandling();
        
    private:

        // Sub-test cases
        bool activateIndicator( WlanIndicatorPlugin* plugin );        
        bool clientParameterHandler( WlanIndicatorPlugin* plugin,
                                     HbIndicatorInterface::RequestType ReqType,
                                     QVariant parameter,
                                     bool expectedResult );
        
        bool indicatorDataFeeder( WlanIndicatorPlugin* plugin,
                                  int role,
                                  QString expectedString );            
        bool indicatorIconFeeder( WlanIndicatorPlugin* plugin,
                                  int role );                   
        bool error( WlanIndicatorPlugin* plugin, int expectedError );
                
        bool handleInteraction( WlanIndicatorPlugin* plugin,
                                HbIndicatorInterface::InteractionType type,
                                bool handleValue );
                
        bool processError( WlanIndicatorPlugin* plugin, QProcess::ProcessError err );

        WlanIndicatorPlugin mWlanIndicatorPlugin;
};

#endif // WLANINDICATORUTCASES_H
