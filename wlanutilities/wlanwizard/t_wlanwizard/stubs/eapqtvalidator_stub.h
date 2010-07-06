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
 *   Stub for EAP WIZARD testing: EAP QT configuration validator interface
 *
 */

/*
 * %version: 2 %
 */

#ifndef EAPQTVALIDATOR_STUB_H
#define EAPQTVALIDATOR_STUB_H

#include <eapqtvalidator.h>

/*!
 * @addtogroup group_eap_config_api
 * @{
 */

class HbLineEdit;

class EapQtValidatorStub : public EapQtValidator 
{
   
public:

    EapQtValidatorStub(EapQtValidator::Status status);
    ~EapQtValidatorStub();
    virtual EapQtValidator::Status validate(QVariant value);
    void updateEditor(HbLineEdit* edit );
    void setReturnValue(EapQtValidator::Status status);
    
private:
    EapQtValidator::Status mStatus;
    Q_DISABLE_COPY(EapQtValidatorStub)
};

/*! @} */

#endif
