/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of the License "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description: 
 *   WPS Wizard API
 *
 */

/*
 * %version: 1 %
 */


#ifndef WPSWIZARD_H_
#define WPSWIZARD_H_



#include <qobject.h>
#include "wlanwizardplugin.h"

#ifdef BUILD_WPS_WIZARD_DLL
#define WPS_WIZARD_EXPORT Q_DECL_EXPORT
#else
#define WPS_WIZARD_EXPORT Q_DECL_IMPORT
#endif

class WlanWizardHelpper;
class WpsWizardPrivate;


/*!
 * @addtogroup group_
 * @{
 */

class WPS_WIZARD_EXPORT WpsWizard: public QObject, public WlanWizardPlugin
{
private:
    Q_OBJECT
    friend class WpsWizardPrivate;
public:
    enum BearerType
    {
        BearerTypeWlan,
    };
public:
    WpsWizard(
        WlanWizardHelpper *wizardHelpper,
        BearerType bearerType, int referenceId);
    ~WpsWizard();

    void storeSettings();
    
public: // From WlanWizardPlugin
    bool summary(WlanWizardPlugin::Summary sum, QString &item, QString &value);

    

private:
    Q_DISABLE_COPY(WpsWizard)
    WpsWizardPrivate *d_ptr;
};

/*! @} */




#endif /* WPSWIZARD_H_ */
