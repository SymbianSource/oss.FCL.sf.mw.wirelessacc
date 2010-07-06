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
 *   EAP-FAST PAC store configuration data
 *
 */

/*
 * %version: 2 %
 */

#ifndef EAPQTPACSTORECONFIG_H
#define EAPQTPACSTORECONFIG_H

#include <QVariant>

class EapQtPacStoreConfig
{
public:

    // PAC store is global, i.e. only a single PAC store exists in a device

    enum PacStoreState
    {
        // PAC store does not exist and needs to be created
        PacStoreStateStoreNotExists = 0,
        // PAC store exists but the password has not been stored and
        // is needed to access the PAC store
        PacStoreStatePasswordRequired,
        // PAC store password has been stored to settings database
        // and is not needed to use the PAC store
        PacStoreStatePasswordStored,
        // TODO: remove? delete automatically if corrupted
        // Existing PAC store is corrupted and should be reset
        PacStoreStateCorrupted,
        // marker for the last item
        PacStoreStateLast
    };

    enum PacStoreSettings
    {
        /*! write-only: QString
         *      password to be used for accessing the PAC store
         *      - must be new password if the read PacStoreState is PacStoreStateStoreNotExists
         *      - must be password of the existing PAC store if the PacStoreState state is
         *        PacStoreStatePasswordRequired */
        PacStorePassword = 0,
        /*! write-only: bool
         *      true: saves the password given in settings
         *      false: clears the earlier stored password from database, i.e. password
         *        will be prompted when needed, PAC store itself remains valid
         *      QVariant::Invalid: no actions */
        PacStoreSavePassword,
        /*! write-only: bool
         *      true: deletes PAC store, i.e. clears everything related to the
         *        existing PAC store inlcuding saved password
         *      false/QVariant::Invalid: no actions */
        PacStoreReset,
        /*! read-only: int (PacStoreState) */
        PacStoreState,
        /*! no write nor read operation, for validator usage only
         *  - uses the validator to check if the given password
         *      can open the existing PAC store */
        PacStorePasswordConfirmation,
        // marker for the last item
        PacStoreLast
    };

public:
    EapQtPacStoreConfig();
    ~EapQtPacStoreConfig();

    QVariant value(PacStoreSettings id);
    void setValue(PacStoreSettings id, QVariant newValue);
    void clear();

public: // FOR TESTING PURPOSE
    bool operator==(const EapQtPacStoreConfig & rhs ) const;
    bool operator=(const EapQtPacStoreConfig & rhs );
    
private:
    QHash <int, QVariant> mPacStoreSettings;
};

#endif /* EAPQTPACSTORECONFIG_H */
