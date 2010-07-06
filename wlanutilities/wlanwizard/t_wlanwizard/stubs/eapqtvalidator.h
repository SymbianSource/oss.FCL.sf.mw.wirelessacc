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

#ifndef EAPQTVALIDATOR_H
#define EAPQTVALIDATOR_H

#include <QVariant>
#include <eapqtconfigdefs.h>

/*!
 * @addtogroup group_eap_config_api
 * @{
 */

class HbLineEdit;

/*!
 * Eap Qt Validator interface. This interface provides a method to configure
 * the editor to use required constraints, hints, editor classes and so one
 * depending on the EAP type and the configuration identifier.
 * See updateEditor() method for further details.
 * 
 * Another important method of this class is the validate() method. With
 * that method you can check that the content and format is valid for 
 * given configuration compination.
 * 
 * An instance of validator can be created with 
 * EapQtConfigInterface::validatorEap() method.
 */

class EapQtValidator
{
public:

    /*!
     * Validation status.
     */
    enum Status {
        /// Ok
        StatusOk,
        /// Content is invalid
        StatusInvalid,
        /// The length is not valid
        StatusInvalidLength,
        /// Invalid characters detected
        StatusInvalidCharacters,
        /// Input is too short
        StatusTooShort, 
        /// Input is too long
        StatusTooLong,  
    };
    
public:

    EapQtValidator() {};
    virtual ~EapQtValidator() {};
    virtual Status validate(QVariant value) = 0;
    virtual void updateEditor(HbLineEdit* edit) = 0;
    
private:

    Q_DISABLE_COPY(EapQtValidator)
};

/*! @} */

#endif
