#include <byteswap.h>
#include "platform.h"

signed int bswap(const int intIn) {
    return __bswap_32(intIn);
}
