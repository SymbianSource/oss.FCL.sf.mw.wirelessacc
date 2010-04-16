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
* This is a source file for CmmWrapper class.
*/

// INCLUDE FILES
#include "wlanqtutilscmmwrapper.h"
#include "wlanqtutilscmmwrapper_s60_p.h"
#include "wlanqtutilsiap.h"
#include "wlanqtutilswlaniap.h"
#include "wlanqtutilswlanap.h"

// ================= MEMBER FUNCTIONS =======================
//

CmmWrapper::CmmWrapper( QObject *parent ) :
    QObject( parent )
{
    d_ptr = new CmmWrapperPrivate( this );
}

CmmWrapper::~CmmWrapper()
{
    delete d_ptr;
}

int CmmWrapper::fetchIaps( QList<WlanQtUtilsIap*>& iapList )
{
    int error = d_ptr->fetchIaps( iapList );
    return error;
}

WlanQtUtilsWlanIap *CmmWrapper::createWlanIap( const WlanQtUtilsWlanAp *wlanAp )
{
    return d_ptr->createWlanIap( wlanAp );
}

void CmmWrapper::moveIapToInternetSnap( int iapId )
{
    return d_ptr->moveIapToInternetSnap( iapId );
}

//end of file
