#include <string.h>
#include <time.h>

#include "ppp_plugin.h"
#include "hubeiexin.h"
#include "utils.h"

char pppd_version[] = VERSION;
static char pwd[MAXSECRETLEN] = {0};
static bool username_set = 0;

static option_t options[] = {
    {
        "pwd", // password
        o_string, // opt_type
        pwd, // addr
        "pwd", // description
        OPT_STATIC, // flags
        NULL,  // addr2
        MAXSECRETLEN - 1, // upper_limit
        0, // lower_mit
        NULL, // source
        0, // priority
        0, // winner
    },
    {NULL, o_special_noarg, NULL, NULL, 0, NULL, 0, 0, NULL, 0, 0},
};

static int pap_modifyusername(char* user, char* passwd) {
    if (!username_set) {
        char exinDynaAccount[MAX_USERNAME_LENGTH + 1] = {0};
        time_t t;
        time(&t);
        calcPIN(t, user, RAD, exinDynaAccount);
        memcpy(user, exinDynaAccount, MAX_USERNAME_LENGTH);
        username_set = 1;
        fprintHexByteArray(stdout, (const unsigned char*)user, MAXNAMELEN);
    }

    if (passwd != NULL) {
        strcpy(passwd, pwd);
    }
    return 1;
}

void plugin_init(void) {
    printf("Hubei Exin\n");
    printf("  PPP Plugin Version: %s, against PPP Version: %s\n", PLUGIN_VERSION, pppd_version);
    printf("  Exin Algorithm Version: %s\n", EXIN_VERSION);
    add_options(options);
    pap_passwd_hook = pap_modifyusername;
}
