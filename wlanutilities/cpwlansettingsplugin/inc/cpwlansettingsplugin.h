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

#ifndef WLANSETTINGSPLUGIN_H_
#define WLANSETTINGSPLUGIN_H_

// System includes

#include <QObject>
#include <QTranslator>
#include <cpplugininterface.h>

// User includes

// Forward declarations

// External data types

// Constants

// Class declaration

class CpWlanSettingsPlugin 
        : public QObject,
          public CpPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CpPluginInterface)
public:
    CpWlanSettingsPlugin();
    virtual ~CpWlanSettingsPlugin();
    virtual QList<CpSettingFormItemData*> createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;

private:
    
    Q_DISABLE_COPY(CpWlanSettingsPlugin)
    
private:    //data

    QTranslator *translator;
    
};


#endif /* WLANSETTINGSPLUGIN_H_ */
