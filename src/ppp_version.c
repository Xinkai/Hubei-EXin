#include <stdio.h>
#ifndef __USE_MISC
    #define __USE_MISC
#endif

#ifndef __USE_BSD
    #define __USE_BSD
#endif
#include <sys/types.h>
#include <pppd/pppd.h>


int main() {
    printf("%s\n", VERSION);
    return 0;
}