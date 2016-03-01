#include "ras_dialer.h"


BOOL findCurrentlyConnected(BOOL* foundOut) {
    DWORD dwCb = 0;
    DWORD dwRet = ERROR_SUCCESS;
    DWORD dwConnections = 0;
    LPRASCONN lpRasConn = NULL;

    // Call RasEnumConnections with lpRasConn = NULL. dwCb is returned with the required buffer size and
    // a return code of ERROR_BUFFER_TOO_SMALL
    dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);

    if (dwRet == ERROR_BUFFER_TOO_SMALL) {
        // Allocate the memory needed for the array of RAS structure(s).
        lpRasConn = (LPRASCONN) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
        if (lpRasConn == NULL){
            fwprintf(stderr, L"HeapAlloc failed!\n");
            return FALSE;
        }
        // The first RASCONN structure in the array must contain the RASCONN structure size
        lpRasConn[0].dwSize = sizeof(RASCONN);

        // Call RasEnumConnections to enumerate active connections
        dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);

        // If successful, print the names of the active connections.
        if (ERROR_SUCCESS == dwRet){
            for (DWORD i = 0; i < dwConnections; i++) {
                if (lstrcmp(ENTRY_NAME_W, lpRasConn[i].szEntryName) == 0) {
                    *foundOut = TRUE;
                    break;
                };
            }
        }

        //Deallocate memory for the connection buffer
        HeapFree(GetProcessHeap(), 0, lpRasConn);
        lpRasConn = NULL;
        return TRUE;
    }

    return TRUE;
}

BOOL createEntry() {
    DWORD dwRet = ERROR_SUCCESS;
    BOOL nRet = TRUE;

    // Allocate heap memory and initialize RASENTRYDLG structure
    LPRASENTRYDLG lpEntry = (LPRASENTRYDLG)HeapAlloc(GetProcessHeap(),
                                                     HEAP_ZERO_MEMORY,
                                                     sizeof(RASENTRYDLG));
    if (lpEntry == NULL) {
        fwprintf(stderr, L"[createEntry] HeapAlloc\n");
        return FALSE;
    }

    // The RASENTRYDLG->dwSize member has to be initialized or the RRAS APIs will fail below.
    lpEntry->dwSize = sizeof(RASENTRYDLG);
    lpEntry->dwFlags = RASEDFLAG_NewBroadbandEntry | RASEDFLAG_NewEntry;

    // Create the new entry using a user dialog
    nRet = RasEntryDlg(PHONE_BOOK,
                       ENTRY_NAME_W,
                       lpEntry);

    // Any error codes are returned in lpEntry
    dwRet = lpEntry->dwError;

    if (nRet == TRUE) {
        wprintf(L"[createEntry] New entry created: %ls\n", lpEntry->szEntry);
    } else {
        if (dwRet != ERROR_SUCCESS) {
            fwprintf(stderr, L"[createEntry] RasEntryDlg error=%d\n", dwRet);
        }
    }

    HeapFree(GetProcessHeap(), 0, lpEntry);
    return !dwRet;
}

BOOL findEntry(DWORD* foundOut) {
    DWORD dwCb = 0;
    DWORD dwRet = ERROR_SUCCESS;
    DWORD dwEntries = 0;
    LPRASENTRYNAME lpRasEntryName = NULL;

    // Call RasEnumEntries with lpRasEntryName = NULL. dwCb is returned with the required buffer size and
    // a return code of ERROR_BUFFER_TOO_SMALL
    dwRet = RasEnumEntries(NULL, PHONE_BOOK, lpRasEntryName, &dwCb, &dwEntries);

    if (dwRet == ERROR_BUFFER_TOO_SMALL) {
        // Allocate the memory needed for the array of RAS entry names.
        lpRasEntryName = (LPRASENTRYNAME)HeapAlloc(GetProcessHeap(),
                                                   HEAP_ZERO_MEMORY,
                                                   dwCb);
        if (lpRasEntryName == NULL){
            fwprintf(stderr, L"[findEntry] HeapAlloc\n");
            return FALSE;
        }
        // The first RASENTRYNAME structure in the array must contain the structure size
        lpRasEntryName[0].dwSize = sizeof(RASENTRYNAME);

        // Call RasEnumEntries to enumerate all RAS entry names
        dwRet = RasEnumEntries(NULL,
                               PHONE_BOOK,
                               lpRasEntryName,
                               &dwCb,
                               &dwEntries);

        // If successful, print the RAS entry names
        if (ERROR_SUCCESS == dwRet){
            for (DWORD i = 0; i < dwEntries; i++){
                if (lstrcmp(ENTRY_NAME_W, lpRasEntryName[i].szEntryName) == 0) {
                    wprintf(L"Found entry: %ls\n", ENTRY_NAME_W);
                    *foundOut = 1;
                    break;
                }
            }
        }
        //Deallocate memory for the connection buffer
        HeapFree(GetProcessHeap(), 0, lpRasEntryName);
        lpRasEntryName = NULL;
        return TRUE;
    }

    return TRUE;
}

BOOL setEntryProperties() {
    DWORD error = ERROR_SUCCESS;
    // Find buffer size
    DWORD dwBufferSize = 0;
    RasGetEntryProperties(NULL, L"", NULL, &dwBufferSize, NULL, NULL);

    if (dwBufferSize == 0) {
        fwprintf(stderr, L"[setEntryProperties] error=%d\n", error);
        return FALSE;
    }

    LPRASENTRY entry = NULL;
    entry = (LPRASENTRY)HeapAlloc(GetProcessHeap(),
                                  HEAP_ZERO_MEMORY,
                                  dwBufferSize);

    entry->dwType = RASET_Broadband;
    entry->dwfNetProtocols = RASNP_Ip;
    entry->dwFramingProtocol = RASFP_Ppp;
    entry->dwSize = dwBufferSize;
    entry->dwfOptions |= RASEO_RemoteDefaultGateway;
    lstrcpy(entry->szDeviceType, RASDT_PPPoE);
    lstrcpy(entry->szLocalPhoneNumber, ENTRY_NAME_W);

    error = RasSetEntryProperties(PHONE_BOOK,
                                  ENTRY_NAME_W,
                                  entry,
                                  dwBufferSize,
                                  NULL,
                                  0);
    if (error) {
        fwprintf(stderr, L"[setEntryProperties] error=%d\n", error);
    }
    HeapFree(GetProcessHeap(), 0, entry);
    return !error;
}

BOOL dial() {
    DWORD error = ERROR_SUCCESS;

    // get old username
    RASCREDENTIALS rasCredentials;
    rasCredentials.dwMask = 0;
    rasCredentials.dwMask |= RASCM_UserName;
    rasCredentials.dwMask |= RASCM_Password;
    rasCredentials.dwSize = sizeof(RASCREDENTIALS);
    error = RasGetCredentials(PHONE_BOOK, ENTRY_NAME_W, &rasCredentials);
    if (error != ERROR_SUCCESS) {
        fwprintf(stderr, L"[dial] RasGetCredentials error=%d\n", error);
        return FALSE;
    }
    if (!(rasCredentials.dwMask & RASCM_UserName)) {
        fwprintf(stderr, L"[dial] RasGetCredentials doesn't contain username\n");
        return FALSE;
    }

    if (!(rasCredentials.dwMask & RASCM_Password)) {
        fwprintf(stderr, L"[dial] RasGetCredentials doesn't contain password\n");
        return FALSE;
    }
    // Load username to rasCredentials.szUserName, if previously encrypted, restore it
    if (wcsncmp(rasCredentials.szUserName, L"\r\n", CRLF_LENGTH) == 0) {
        wprintf(L"Previous encrypted username found: %ls\n", rasCredentials.szUserName);
        lstrcpy(rasCredentials.szUserName, rasCredentials.szUserName + CRLF_LENGTH + PROC1_OUT_LENGTH + MD5_HEAD_LENGTH);
    } else {
        wprintf(L"Previous vanilla username found\n");
    }
    wprintf(L"Vanilla username: %ls\n", rasCredentials.szUserName);

    // Get ANSI username, calcPIN, and make the result Unicode again.
    char strUserName[MAX_USERNAME_LENGTH + 1] = {0};
    char exinDynaAccount[MAX_USERNAME_LENGTH + 1] = {0};
    BOOL ok = WideCharToMultiByte(CP_ACP,
                                  WC_COMPOSITECHECK,
                                  rasCredentials.szUserName,
                                  -1, // NULL-terminated
                                  strUserName,
                                  MAX_USERNAME_LENGTH,
                                  NULL, NULL);
    if (!ok) {
        fwprintf(stderr, L"[dial] WideCharToMultiByte error=%d\n", GetLastError());
        return FALSE;
    }

    time_t t;
    time(&t);
    calcPIN(t, strUserName, RAD, exinDynaAccount);

    ok = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                             exinDynaAccount,
                             -1, // NULL-terminated
                             rasCredentials.szUserName, UNLEN);
    if (!ok) {
        fwprintf(stderr, L"[dial] MultiByteToWideChar error=%d\n", GetLastError());
        return FALSE;
    }
    wprintf(L"Newly encrtpted username: %ls\n", rasCredentials.szUserName);

    // set credentials
    error = RasSetCredentials(PHONE_BOOK, ENTRY_NAME_W, &rasCredentials, FALSE);
    if (error) {
        fwprintf(stderr, L"[dial] RasSetCredentials error=%d\n", error);
        return FALSE;
    }

    // dial

    RASDIALPARAMS rdParams;
    LPWSTR entryName = ENTRY_NAME_W;
    lstrcpy(rdParams.szEntryName, entryName);
    rdParams.dwSize = sizeof(RASDIALPARAMS);
    rdParams.szPhoneNumber[0] = '\0';
    rdParams.szCallbackNumber[0] = '\0';
    rdParams.szDomain[0] = '\0';
    lstrcpy(rdParams.szUserName, rasCredentials.szUserName);
    lstrcpy(rdParams.szPassword, rasCredentials.szPassword);

    HRASCONN hRasConn = NULL;
    error = RasDial(NULL, NULL, &rdParams, 0L, NULL, &hRasConn);
    if (error) {
        fwprintf(stderr, L"[dial] RasDial error=%d\n", error);
        RasHangUp(hRasConn);
    }
    return !error;
}

BOOL isSupportedPlatform() {
    DWORD version = GetVersion();
    DWORD major = (DWORD) (LOBYTE(LOWORD(version)));

    return major >= 6; // Vista or greater
}

int main() {
    if (!isSupportedPlatform()) {
        fwprintf(stderr, L"Windows Vista or greater is required.\n");
        return 1;
    }

    DWORD entryFound = FALSE;
    BOOL connectedFound = FALSE;
    BOOL ok = TRUE;

    wprintf(L"=== Hubei EXin Dialer for Windows ===\n");
    wprintf(L"Algorithm Version: ");
    printf("%s\n", EXIN_VERSION);
    wprintf(L"Windows Dialer Version： %ls\n\n", RAS_DIALER_VERSION);
    wprintf(L"Homepage: https://github.com/Xinkai/Hubei-Exin\n");

    ok = findCurrentlyConnected(&connectedFound);
    if (!ok) {
        fwprintf(stderr, L"Error occured when tried to test if HubeiExin is currently connected\n");
        return 1;
    }
    if (connectedFound) {
        fwprintf(stderr, L"Already Connected\n");
        return 0;
    }

    ok = findEntry(&entryFound);
    if (!ok) {
        fwprintf(stderr, L"Error occurred when tried to find Entry 'HubeiExin'\n");
        return 1;
    }

    if (!entryFound) {
        wprintf(L"Entry not found, trying to create\n");
        ok = createEntry();
        if (!ok) {
            fwprintf(stderr, L"Cannot create entry\n");
            return 1;
        };
    }

    wprintf(L"Entry is OK\n");

    ok = setEntryProperties();
    if (!ok) {
        return 1;
    }

    ok = dial();
    if (!ok) {
        return 1;
    }

    return 0;
}
