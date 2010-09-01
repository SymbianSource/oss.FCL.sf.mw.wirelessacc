/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class header for CWsfModel
*
*/

#ifndef WSFLOGGER_H
#define WSFLOGGER_H


#ifdef _DEBUG
#define USE_WSFLOGGER
#endif


#ifdef USE_WSFLOGGER

// undef this if you want logs into file
#define WSFLOGGER_LOGGING_METHOD_RDEBUG

//  EXTERNAL INCLUDES
#include <flogger.h>
#include <e32debug.h>


//  CONSTANTS
_LIT( KLogDir,                  "Sniffer" );
_LIT( KLogFile,                 "Sniffer.txt" );
_LIT( KLogBanner,               "Sniffer 0.1.0" );
_LIT( KLogEnterFn,              "%04x\t% *S-> %S" );
_LIT( KLogLeaveFn,              "%04x\t% *S<- %S" );
_LIT( KLogExit,                 "Sniffer: Application exit" );
_LIT( KLogFormat,               "%04x\t% *S%S" );


//  CLASS DEFINITION
/**
* Class to log functionentry/exit
*
* Example usage:
*
* @code
* #include "wsflogger.h"
*
* ...
* 
* void MyClass::DoIt() 
*     {
*     LOG_ENTERFN( "MyClass::DoIt" );
*     // doing it...
*     }
*
* @endcode
*
* Since the macros create instances on the stack, the class is destructed when
* the stack is unwound. Thus, if LOG_ENTERFN was called at the highest level 
* within the function then the destructor can detect the exit from the 
* function without the need of an explicit "LOG_LEAVEFN" to each exit point.
*
* Note: the destructor is not called if a leave occurs.
*
* @since S60 5.0
*/
NONSHARABLE_CLASS( CFunctionEntryExit ): public CBase
    {
    public:

        /**
        * Constructor
        * @since S60 5.0
        * @param aFunc Name of the function being entered
        */
        CFunctionEntryExit( TRefByValue<const TDesC> aFunc )
            {
            iFunc.Set( aFunc );

            RThread thread;
            TUint tid = TUint( thread.Id() ) & 0xffff;

#ifndef WSFLOGGER_LOGGING_METHOD_RDEBUG
            RFileLogger::WriteFormat( KLogDir, 
                                      KLogFile, 
                                      EFileLoggingModeAppend, 
                                      KLogEnterFn, 
                                      tid,
                                      4*((tid>>2)&0x3),
                                      &KNullDesC,
                                      &iFunc);
#else
            RDebug::Print( KLogEnterFn, tid,
                           4*((tid>>2)&0x3),
                           &KNullDesC,
                           &iFunc);
#endif
            }
    
        /**
        * Destructor
        * @since S60 5.0
        */
        ~CFunctionEntryExit()
            {
            RThread thread;
            TUint tid = TUint( thread.Id() ) & 0xffff;

#ifndef WSFLOGGER_LOGGING_METHOD_RDEBUG
            RFileLogger::WriteFormat( KLogDir, 
                                      KLogFile, 
                                      EFileLoggingModeAppend, 
                                      KLogLeaveFn, 
                                      tid,
                                      4*((tid>>2)&0x3),
                                      &KNullDesC,
                                      &iFunc);
#else
            RDebug::Print( KLogLeaveFn, 
                           tid,
                           4*((tid>>2)&0x3),
                           &KNullDesC,
                           &iFunc );
#endif
            }

    public:     // data
        /**
        * Function name
        */
        TPtrC iFunc;
    };
       

//  LOG MACROS       
#ifndef WSFLOGGER_LOGGING_METHOD_RDEBUG
// Create/overwrite the log file
#define LOG_CREATE                                                      \
            {                                                           \
            TFileName path( _L( "c:\\logs\\" ) );                       \
            path.Append( KLogDir );                                     \
            path.Append( _L( "\\" ) );                                  \
            RFileLogger::WriteFormat( KLogDir, KLogFile,                \
                           EFileLoggingModeOverwrite, KLogBanner );     \
            }


// Close the log
#define LOG_DELETE                                                      \
            RFileLogger::Write( KLogDir, KLogFile,                      \
                           EFileLoggingModeAppend, KLogExit );          \


// Place a function entry/exit watchdog            
#define LOG_ENTERFN(a)                                                  \
            CFunctionEntryExit entryExit( _L(a) );


// Write a solid string to the log
#define LOG_WRITE(a)                                                    \
            {                                                           \
            _LIT( temp, a );                                            \
            RThread thread;                                             \
            TUint tid = TUint( thread.Id() ) & 0xffff;                  \
            TBuf<5+12+sizeof(L##a)/2-1> buf;                            \
            buf.Format( KLogFormat,                                     \
                        tid, 4*((tid>>2)&0x3), &KNullDesC, &temp );     \
            RFileLogger::Write( KLogDir, KLogFile,                      \
                                EFileLoggingModeAppend, buf );          \
            }
            

// Write a formatted string to the log
#define LOG_WRITEF(a, s...)                                             \
            {                                                           \
            _LIT( temp, a );                                            \
            RThread thread;                                             \
            TUint tid = TUint( thread.Id() ) & 0xffff;                  \
            TBuf<5+12+sizeof(L##a)/2-1> buf;                            \
            buf.Format( KLogFormat,                                     \
                        tid, 4*((tid>>2)&0x3), &KNullDesC, &temp );     \
            RFileLogger::WriteFormat( KLogDir, KLogFile,                \
                        EFileLoggingModeAppend, buf, s );               \
            }

#else // WSFLOGGER_LOGGING_METHOD_RDEBUG

// Create/overwrite the log file
#define LOG_CREATE                                                      \
            RDebug::Print(_L("%S"), &KLogBanner);


// Close the log
#define LOG_DELETE                                                      \
            RDebug::Print(_L("%S"), &KLogExit);


// Place a function entry/exit watchdog            
#define LOG_ENTERFN(a)                                                  \
            CFunctionEntryExit entryExit( _L(a) );


// Write a solid string to the log
#define LOG_WRITE(a)                                                    \
            {                                                           \
            _LIT( temp, a );                                            \
            RThread thread;                                             \
            TUint tid = TUint( thread.Id() ) & 0xffff;                  \
            TBuf<5+12+sizeof(L##a)/2-1> buf;                            \
            buf.Format( KLogFormat,                                     \
                        tid, 4*((tid>>2)&0x3), &KNullDesC, &temp );     \
            RDebug::Print(buf);                                         \
            }
            

// Write a formatted string to the log
#define LOG_WRITEF(a, s...)                                             \
            {                                                           \
            _LIT( temp, a );                                            \
            RThread thread;                                             \
            TUint tid = TUint( thread.Id() ) & 0xffff;                  \
            TBuf<5+12+sizeof(L##a)/2-1> buf;                            \
            buf.Format( KLogFormat,                                     \
                        tid, 4*((tid>>2)&0x3), &KNullDesC, &temp );     \
            RDebug::Print( buf, s);                                     \
            }

#endif // WSFLOGGER_LOGGING_METHOD_RDEBUG


#else // _DEBUG


#define LOG_CREATE
#define LOG_DELETE
#define LOG_ENTERFN(a)
#define LOG_WRITE(a)
#define LOG_WRITEF(a, s...)


#endif // _DEBUG




#endif // WSFLOGGER_H


// End of File
