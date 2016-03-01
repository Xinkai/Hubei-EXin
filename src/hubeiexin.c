#include "hubeiexin.h"
#include "utils.h"

signed int calcPinProc0(const time_t timeIn) {
    // divide time by 5, and cast it into a signed 32-bit integer.
    return (signed int)timeIn / 5;
}

void calcPinProc1(const signed int timeIn, unsigned char* out) {
    // memory allocations
    // [rbp-0x30, rbp-0x2d] <- timeOverFive signed int == timeIn

    // [rbp-0x34, rbp-0x31] -- bswap'ed timeOverFive signed int
    unsigned int bswaped;

    // [rbp-0x40, rbp-0x3d] -- m40
    unsigned char m40[sizeof(int32_t)];

    // [rbp-0x46, rbp-0x41] -> PIN27 == out
    unsigned char PIN27[PROC1_OUT_LENGTH] = {0};

    bswaped = bswap(timeIn); // 0x100011b63
    memcpy(m40, &bswaped, sizeof(int32_t));

    // [rbp-0x38, rbp-0x35] -- m38
    unsigned char m38[sizeof(int32_t)] = {0};

    for (unsigned char m = 0; m < 4; m++) {
        for (unsigned char n = 0; n < 8; n++) {
            m38[m] += ((timeIn >> (m + 4 * n)) & 0x1) << (7 - n);
        }
    }

    unsigned int c;
    memcpy(&c, m38, sizeof(int32_t));

    PIN27[0] = (unsigned char)((c & 0xff) >> 0x2);
    PIN27[1] = (unsigned char)(((c << 0x4) & 0x30) | ((c & 0xf000) >> 0xc));
    PIN27[2] = (unsigned char)(((c >> 0x6) & 0x3c) | ((c >> 0x16) & 0x3));
    PIN27[3] = (unsigned char)((c >> 0x10) & 0x3f);
    PIN27[4] = (unsigned char)(c >> 0x1a);
    PIN27[5] = (unsigned char)((c >> 0x14) & 0x30);

    for (unsigned char i = 0; i < PROC1_OUT_LENGTH; i++) {
        if (((PIN27[i] + 0x20) & 0xff) < 0x40) {
            PIN27[i] = (PIN27[i] + 0x20) & 0xff;
        } else {
            PIN27[i] = (PIN27[i] + 0x21) & 0xff;
        }
    }

    memcpy(out, PIN27, PROC1_OUT_LENGTH);
}

void calcPIN(time_t timeIn,
             const char* accountIn,
             const char* radIn,
             char* usernameOut) {
    // deal with time
    #if DEBUGGING
        printf("[DBG] Time is: \n");
        fprintHexByteArray(stdout, (const unsigned char *)&timeIn, sizeof(timeIn));
    #endif

    signed int timeOverFive = calcPinProc0(timeIn);
    #if DEBUGGING
        printf("[DBG] Time/5 is: \n");
        fprintHexByteArray(stdout, (const unsigned char *)&timeOverFive, sizeof(timeOverFive));
    #endif

    signed int reversedTimeOverFive = bswap(timeOverFive);
    #if DEBUGGING
        printf("[DBG] reversedTimeOverFive is: \n");
        fprintHexByteArray(stdout, (const unsigned char *)&reversedTimeOverFive, sizeof(reversedTimeOverFive));
    #endif

    // proc1 transform
    unsigned char proc1[PROC1_OUT_LENGTH] = {0};
    calcPinProc1(timeOverFive, proc1);
    #if DEBUGGING
        printf("[DBG] proc1 is: \n");
        fprintHexByteArray(stdout, proc1, PROC1_OUT_LENGTH);
    #endif

    // prepare for md5 hash
    const unsigned int toHashLength = sizeof(signed int) + CELLPHONE_LENGTH + RAD_LENGTH;
    char toHash[sizeof(signed int) + CELLPHONE_LENGTH + RAD_LENGTH] = {0};
    memcpy(toHash, &reversedTimeOverFive, sizeof(reversedTimeOverFive));
    memcpy(toHash + sizeof(reversedTimeOverFive), accountIn, CELLPHONE_LENGTH);
    memcpy(toHash + sizeof(reversedTimeOverFive) + CELLPHONE_LENGTH, radIn, RAD_LENGTH);
    #if DEBUGGING
        printf("[DBG] toHash is: \n");
        fprintHexByteArray(stdout, (const unsigned char*)toHash, toHashLength);
    #endif

    // md5 hash
    unsigned char hashed[MD5_DIGEST_LENGTH] = {0};
    MD5((const unsigned char *)toHash, toHashLength, hashed);
    #if DEBUGGING
        printf("[DBG] hashed is: \n");
        fprintHexByteArray(stdout, hashed, MD5_DIGEST_LENGTH);
    #endif

    // get md5 head
    char md5Head[MD5_HEAD_LENGTH + 1] = {0};
    sprintf(md5Head, "%02x", hashed[0]);
    #if DEBUGGING
        printf("[DBG] md5Head is: \n");
        fprintHexByteArray(stdout, (const unsigned char*)md5Head, MD5_HEAD_LENGTH);
    #endif

    // set output
    memcpy(usernameOut, "\r\n", CRLF_LENGTH);
    memcpy(usernameOut + CRLF_LENGTH, (const char*)proc1, PROC1_OUT_LENGTH);
    memcpy(usernameOut + CRLF_LENGTH + PROC1_OUT_LENGTH, md5Head, MD5_HEAD_LENGTH);
    memcpy(usernameOut + CRLF_LENGTH + PROC1_OUT_LENGTH + MD5_HEAD_LENGTH, accountIn, CELLPHONE_LENGTH);
}
