/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 *   EAP method configuration QT interface
 *
 */

/*
 * %version: 5 %
 */

#ifndef EAPQTCONFIGINTERFACE_H
#define EAPQTCONFIGINTERFACE_H

#include <qglobal.h>
#include <QStringList>
#include <eapqtexpandedeaptype.h>
#include <eapqtpluginhandle.h>
#include <eapqtconfigdefs.h>
#include "eapqtconfig.h"
#include "eapqtpacstoreconfig.h"
#include "eapqtcertificateinfo.h"
#include "eapqtvalidator.h"

class EapQtConfigInterfaceContext;
/*!
 * @addtogroup group_eap_config_api
 * @{
 */

/*!
 */
class EapQtConfigInterface
{
public:

    enum EapBearerType
    {
        // EAP configuration interface for WLAN
        EapBearerTypeWlan,
        // EAP configuration interface for VPN
        EapBearerTypeVpn,
    };

    static const int IapIdUndefined = -1;

public:
    EapQtConfigInterface(const EapBearerType bearerType, const int iapId);
    ~EapQtConfigInterface();
    bool setConfigurationReference(const int iapId);
    QList<EapQtCertificateInfo> certificateAuthorityCertificates();
    QList<EapQtCertificateInfo> userCertificates();
    
    bool updateCertificates();
    
    bool isSupportedOuterType(const EapQtPluginHandle& handle);
    bool isSupportedInnerType(const EapQtPluginHandle& outerHandle,
        const EapQtPluginHandle& innerHandle);
    
    bool setSelectedOuterTypes(const QList<EapQtPluginHandle>& outerHandles);
    bool saveConfiguration(const EapQtPluginHandle& pluginHandle, EapQtConfig &config);
    bool deleteConfiguration();
    EapQtValidator *validatorEap(EapQtExpandedEapType type, EapQtConfig::SettingsId id);
    
    bool readPacStoreConfiguration(EapQtPacStoreConfig &config);
    bool savePacStoreConfiguration(EapQtPacStoreConfig &config);
    EapQtValidator *validatorPacStore(EapQtPacStoreConfig::PacStoreSettings id);
    
private:
    Q_DISABLE_COPY(EapQtConfigInterface)
    QStringList mCalledMethods;
    
    // setConfigurationReference
    int mIapId;
    bool mReturnSetConfigurationReference;
    
    // certificateAuthorityCertificates
    QList<EapQtCertificateInfo> mCertsCa;
    
    // userCertificates
    QList<EapQtCertificateInfo> mCertsUser;
    
    // isSupportedOuterType
    QList<EapQtPluginHandle> mSupportedOuterTypes;
    
    // isSupportedInnerType
    QList<EapQtPluginHandle> mSupportedInnerTypes;
    int mSupportedInnerTypesOuterHandle;
    
    // setSelectedOuterTypes
    QList<EapQtPluginHandle> mSetSelectectedOuterTypes;
    bool mReturnSetSelectedOuterTypes;
    
    // saveConfiguration
    EapQtPluginHandle::Plugin mOuterPlugin;
    EapQtConfig mSaveConfigurationsOuter;
    bool mReturnSaveConfigurationOuter;
    EapQtPluginHandle::Plugin mInnerPlugin;
    EapQtConfig mSaveConfigurationsInner;
    bool mReturnSaveConfigurationInner;

    // validatorEap
    QHash<EapQtExpandedEapType::Type, QHash<EapQtConfig::SettingsId, EapQtValidator *> > mValidator;
    
    // readPacStoreConfiguration
    EapQtPacStoreConfig mReadPacStoreConfiguration;
    bool mReadPacStoreConfigurationReturn;
    
    // savePacStoreConfiguration
    EapQtPacStoreConfig mSavePacStoreConfiguration;
    bool mSavePacStoreConfigurationReturn;
    
    // validatorPacStore, key: EapQtPacStoreConfig::PacStoreSettings
    QHash<int ,EapQtValidator* > mValidatorPacStore;
    
    friend class EapQtConfigInterfaceContext;
};

/*! @} */

#endif


