#include "exam.h"

int main() {
    const char account[] = "12345678901";
    unsigned int failed_count = 0;
    {
        time_t t;
        t = 0x554e5f26;
        char username[MAX_USERNAME_LENGTH + 1] = {0};
        calcPIN(t, account, RAD, username);
        if (cmpPIN(username, "\x43\x56\x41\x49\x3e\x20\x64\x38")) {
            failed_count++;
        };
    }
    {
        time_t t;
        t = 0x554e5bc4;
        char username[MAX_USERNAME_LENGTH + 1] = {0};
        calcPIN(t, account, RAD, username);
        if (cmpPIN(username, "\x3a\x58\x44\x28\x3e\x20\x31\x61")) {
            failed_count++;
        };
    }
    {
        time_t t;
        t = 0x55633ed6;
        char username[MAX_USERNAME_LENGTH + 1] = {0};
        calcPIN(t, account, RAD, username);
        if (cmpPIN(username, "\x3f\x59\x44\x30\x4d\x20\x38\x36")) {
            failed_count++;
        };
    }
    if (failed_count) {
        fprintf(stderr, "%d tests failed.\n", failed_count);
    } else {
        printf("All tests passed.\n");
    }
    return 0;
}
