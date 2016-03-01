#ifndef HUBEIEXIN_RAS_DIALER_H
#define HUBEIEXIN_RAS_DIALER_H

// Enable compiling Widechar Win32 application under mingw
#ifndef UNICODE
    #define UNICODE
#endif

#ifndef _UNICODE
    #define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>
#include <ras.h>
#include <rasdlg.h>
#include <raserror.h>
#include <tchar.h>

#include "hubeiexin.h"
#define PHONE_BOOK NULL
#define ENTRY_NAME_W L"HubeiExin"
#define RAS_DIALER_VERSION L"1.0"

#endif //HUBEIEXIN_RAS_DIALER_H
