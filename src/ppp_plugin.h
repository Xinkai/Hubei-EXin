#ifndef HUBEIEXIN_PPP_PLUGIN_H
#define HUBEIEXIN_PPP_PLUGIN_H

#ifndef __USE_MISC
    #define __USE_MISC 1
#endif
#ifndef __USE_BSD
    #define __USE_BSD 1
#endif

#define PLUGIN_VERSION "0.1"

#include <sys/types.h>
#include <pppd/pppd.h>

void plugin_init(void);

#endif //HUBEIEXIN_PPP_PLUGIN_H
