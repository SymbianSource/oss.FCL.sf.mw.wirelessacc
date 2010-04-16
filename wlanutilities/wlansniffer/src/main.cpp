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

#ifdef WLANSNIFFER_SERVICETRACES    
#include "xqservicelog.h"
#endif

#include "qsysteminfo.h"

#include "wlansniffer.h"

using namespace QtMobility;

int main(int argc, char *argv[])
{
    // Check WLAN dynamic configuration & exit if WLAN is not enabled
    QSystemInfo sysinfo;
    if (!sysinfo.hasFeatureSupported(QSystemInfo::WlanFeature)) {
        return -1;
    }

#ifdef WLANSNIFFER_SERVICETRACES    
    qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
#endif
    
    WlanSniffer app(argc, argv);

    return app.exec();
}

// End of File
