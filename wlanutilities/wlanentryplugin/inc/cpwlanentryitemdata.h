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

#ifndef CPWLANENTRYITEMDATA_H
#define CPWLANENTRYITEMDATA_H

// System includes

#include "cpsettingformentryitemdata.h"

// User includes

// Forward declarations

class WlanStatusInfo;

// External data types

// Constants

// Class declaration

class CpWlanEntryItemData : public CpSettingFormEntryItemData
{
    Q_OBJECT

public:

    // Data types

    explicit CpWlanEntryItemData(CpItemDataHelper &itemDataHelper);
    
    virtual ~CpWlanEntryItemData();

signals:

public slots:

protected:

protected slots:

private:

    Q_DISABLE_COPY(CpWlanEntryItemData)

    virtual CpBaseSettingView *createSettingView() const;

private slots:

    void statusUpdate();

private: // data

    WlanStatusInfo *mWlanStatusInfo;

    // Friend classes

};

#endif // CPWLANENTRYITEMDATA_H
