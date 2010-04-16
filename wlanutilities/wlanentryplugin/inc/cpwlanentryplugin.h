/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 
#ifndef	CPWLANENTRYPLUGIN_H
#define	CPWLANENTRYPLUGIN_H


// System includes

#include <QObject>
#include <cppluginplatinterface.h>

// User includes

// Forward declarations

class QTranslator;

// External data types

// Constants

// Class declaration

class CpWlanEntryPlugin : public QObject, public CpPluginPlatInterface
{
    Q_OBJECT
    Q_INTERFACES(CpPluginPlatInterface)
    
public:

    // Data types

    CpWlanEntryPlugin();
    
    virtual ~CpWlanEntryPlugin();
    
    virtual CpSettingFormItemData *createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;

signals:

public slots:

protected:

protected slots:

private:

    Q_DISABLE_COPY(CpWlanEntryPlugin)

private slots:

private: // data

    bool mWlanSupported;            // WLAN dynamic ON/OFF configuration
    QTranslator *mTranslator;       // Translator for text ID's used in this plugin

    // Friend classes

};

#endif	// CPWLANENTRYPLUGIN_H
