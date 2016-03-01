#include <string.h>
#include "utils.h"
#include "hubeiexin.h"

void fprintHexByte(FILE* outChannel, unsigned char in) {
    fprintf(outChannel, "%02X", in);
}

void fprintHexByteArray(FILE* outChannel, const unsigned char* ptr, const unsigned int length) {
    for (unsigned int i = 1; i <= length; i++) {
        fprintf(outChannel, " ");
        fprintHexByte(outChannel, ptr[i-1]);

        if (i % 0x8 == 0) {
            fprintf(outChannel, "\n");
        }
    };
    if (length % 0x8 != 0) {
        fprintf(outChannel, "\n");
    }
}

int expectEq(const void* ptr1, const void* ptr2, const unsigned int length) {
    return memcmp(ptr1, ptr2, length);
}

int cmpPIN(const void* username, const void* PIN29) {
    int result = 0;
    if (expectEq(username + CRLF_LENGTH, PIN29, PROC1_OUT_LENGTH) != 0) {
        fprintf(stderr, "PIN27 failed:\n");
        fprintHexByteArray(stderr, username + CRLF_LENGTH, PROC1_OUT_LENGTH);
        fprintHexByteArray(stderr, PIN29, PROC1_OUT_LENGTH);
        result |= 1;
    } else {
    };

    if (expectEq(username + CRLF_LENGTH + PROC1_OUT_LENGTH, PIN29 + PROC1_OUT_LENGTH, MD5_HEAD_LENGTH) != 0) {
        fprintf(stderr, "MD5Head failed:\n");
        fprintHexByteArray(stderr, username + CRLF_LENGTH + PROC1_OUT_LENGTH, MD5_HEAD_LENGTH);
        fprintHexByteArray(stderr, PIN29 + PROC1_OUT_LENGTH, MD5_HEAD_LENGTH);
        result |= 2;
    } else {
    };
    return result;
}
