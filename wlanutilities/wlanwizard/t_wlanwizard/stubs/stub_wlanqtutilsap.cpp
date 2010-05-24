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
 * This is the source file for WlanQtUtilsAp class.
 */

#include <QString>
#include <QHash>
#include <QVariant>
#include <QDebug>
#include "wlanqtutilscommon.h"
#include "wlanqtutilsap.h"

class WlanQtUtilsApPrivate
{
    friend class WlanQtUtilsAp;

private:
    QHash<int, QVariant> mConfigurations;
};

WlanQtUtilsAp::WlanQtUtilsAp() :
    d_ptr(new WlanQtUtilsApPrivate())
{
}

WlanQtUtilsAp::WlanQtUtilsAp(const WlanQtUtilsAp &ref) :
    d_ptr(new WlanQtUtilsApPrivate())
{
    d_ptr->mConfigurations = ref.d_ptr->mConfigurations;
}

WlanQtUtilsAp::~WlanQtUtilsAp()
{
    d_ptr->mConfigurations.clear();
    delete d_ptr;
}

QVariant WlanQtUtilsAp::value(int identifier) const
{
    return d_ptr->mConfigurations[identifier];
}

void WlanQtUtilsAp::setValue(int identifier, QVariant value)
{
    d_ptr->mConfigurations[identifier] = value;
}

bool WlanQtUtilsAp::operator==(const WlanQtUtilsAp & rhs ) const
{
    bool ret = true;
    if (d_ptr->mConfigurations.size() != rhs.d_ptr->mConfigurations.size()) {
        qWarning("WlanQtUtilsAp::operator==(): size: expect %d, actual %d ",
            d_ptr->mConfigurations.size(),
            rhs.d_ptr->mConfigurations.size() );
        ret = false;
    }
    QHashIterator<int, QVariant> i(d_ptr->mConfigurations);
    
    while (i.hasNext()) {
        i.next();
        if (!rhs.d_ptr->mConfigurations.contains(i.key())){
            qWarning("WlanQtUtilsAp::operator==(): key not found: %d", i.key());
            ret = false;
            
        }
        if (i.value() != rhs.d_ptr->mConfigurations[i.key()]){
            qWarning("WlanQtUtilsAp::operator==(): values not match");
            qDebug() << "Expect: " << i.value();
            qDebug() << "Actual: " << i.value();
            ret = false;
        }
    }
    return ret;
}


