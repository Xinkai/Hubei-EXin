#include <stdio.h>
#include <time.h>

#include "hubeiexin.h"
#include "asm.h"

#define TEST_PROGRESS_PER 0xFFFFF

int main() {
    time_t tStart;
    time(&tStart);
    tStart /= 5;

    time_t tEnd;
    tEnd = tStart + 60 * 60 * 24 * 365 * 3 / 5;

    unsigned char PIN27_1[PROC1_OUT_LENGTH] = {0};
    unsigned char PIN27_2[PROC1_OUT_LENGTH] = {0};
    for (time_t timeOverFive = tStart; timeOverFive < tEnd; timeOverFive++) {
        calcPinProc1(timeOverFive, PIN27_1);
        calcPinProc1ASM(timeOverFive, PIN27_2);
        if (memcmp(PIN27_1, PIN27_2, PROC1_OUT_LENGTH) != 0) {
            fprintf(stderr, "Unmatched %u\n", (unsigned int)timeOverFive);
            fprintf(stderr, " C  Version: \n");
            fprintHexByteArray(stderr, PIN27_1, PROC1_OUT_LENGTH);
            fprintf(stderr, "ASM Version: \n");
            fprintHexByteArray(stderr, PIN27_2, PROC1_OUT_LENGTH);
            return 1;
        } else {
            if (timeOverFive % TEST_PROGRESS_PER == 0) {
                printf("All Good so far %.2f%%\n", 100.0 * (timeOverFive - tStart) / (tEnd - tStart));
            }
        }
    }
    return 0;
};
