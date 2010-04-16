/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
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

#ifndef WLANSETTINGS_H_
#define WLANSETTINGS_H_

// System includes

#include <QObject>

// User includes

#include "wlansettings_s60_p.h"

// Forward declarations

// External data types

// Constants

// Class declaration

class WlanSettings : public QObject
{
     Q_OBJECT
     
public:
     
     enum ScanNetworkType {
         EScanNetworkAuto = 0,
         EScanNetworkUserDefined        
     };
     
     WlanSettings();

     ~WlanSettings();
     
     int init();
     
     int loadSettings();
     
     ScanNetworkType scanNetworkType();
     
     uint scanInterval();
     
     int joinWlanMode() const;
     
     int setJoinWlanMode(int mode);
     
     int isPowerSavingEnabled() const;
     
     int isPsmEnabled() const;
     
     int setWlanPowerSaving(int powerSavingOption);
     
     int setWlanScanInterval(uint scanInterval);

private:
     
     void readPsmKey();
     
     Q_DISABLE_COPY(WlanSettings)
     
private:    //data
     
     CWlanSettingsPrivate *mImpl;
     
     int mPsmKeyValue;
     
};


#endif /* WLANSETTINGS_H_ */
