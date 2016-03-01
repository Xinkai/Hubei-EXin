#include "platform.h"

#include <windows.h>


signed int bswap(const int intIn) {
    return _byteswap_ulong(intIn);
}

unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md) {
    HCRYPTPROV hProv = (HCRYPTPROV)NULL;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        exit(1);
    }

    BOOL hash_ok = FALSE;
    HCRYPTPROV hHash = (HCRYPTPROV)NULL;
    hash_ok = CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);

    if (!hash_ok) {
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    if (!CryptHashData(hHash, d, (DWORD)n, 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    DWORD cbHashSize = 0;
    DWORD dwCount = sizeof(DWORD);
    if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&cbHashSize, &dwCount, 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    if (!CryptGetHashParam(hHash, HP_HASHVAL, md, &cbHashSize, 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return md;
}
