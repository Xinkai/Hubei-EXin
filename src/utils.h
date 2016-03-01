#ifndef HUBEIEXIN_UTILS_H
#define HUBEIEXIN_UTILS_H

#include <stdio.h>

void fprintHexByte(FILE*, unsigned char in);
void fprintHexByteArray(FILE* outChannel, const unsigned char* ptr, const unsigned int length);

int expectEq(const void* ptr1, const void* ptr2, const unsigned int length);
int cmpPIN(const void* username, const void* PIN29);

#endif //HUBEIEXIN_UTILS_H
