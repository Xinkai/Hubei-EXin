#ifndef HUBEIEXIN_PLATFORM_H
#define HUBEIEXIN_PLATFORM_H


// use definitions from openssl
#define MD5_DIGEST_LENGTH 16
#ifdef _WIN32
    #include <windef.h> // need size_t
#elif __linux
    #include <stdlib.h> // need size_t
#endif

unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md);

signed int bswap(const int intIn);

#endif //HUBEIEXIN_PLATFORM_H
