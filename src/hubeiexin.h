#ifndef HUBEIEXIN_HUBEIEXIN_H
#define HUBEIEXIN_HUBEIEXIN_H

// Headers
#include <time.h>
#include <string.h>
#include <stdint.h>

#include "platform.h"

#define DEBUGGING 0

// Constants
#define EXIN_VERSION "0.1"
#define RAD "hubtxinli01"

// Sizes and Lengths
// The lengths of following string-likes do not include the ending \0.
// Many of them do not even allocate memory for the ending \0.
#define CRLF_LENGTH 2
#define PROC1_OUT_LENGTH 0x6
#define RAD_LENGTH 11
#define MD5_HEAD_LENGTH 2
#define CELLPHONE_LENGTH 11
#define MAX_USERNAME_LENGTH CRLF_LENGTH + PROC1_OUT_LENGTH + MD5_HEAD_LENGTH + CELLPHONE_LENGTH

// Functions
void calcPIN(time_t timeIn, const char* accountIn, const char* radIn, char* usernameOut);
void calcPinProc1(const signed int timeIn, unsigned char* out);

#endif //HUBEIEXIN_HUBEIEXIN_H
