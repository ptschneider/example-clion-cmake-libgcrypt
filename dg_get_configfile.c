
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "dg_get_configfile.h"

int dg_get_config(char* fname,rsa_samples_t* rsa_samples_ptr) {

    int capacity = 16;
    CfgEntry* entries = malloc(capacity * sizeof(CfgEntry));
    if (entries == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return 1;
    }

    CfgError err;
    Cfg cfg = {.entries = entries, .capacity = capacity};
    int res = cfg_parse_file(fname, &cfg, &err);

    if (res != 0) {
        cfg_fprint_error(stderr, &err);
        free(entries);
        return 1;
    }

    char* tmp = cfg_get_string(&cfg, "sample.rsa.one", "(genkey (rsa (nbits 3:128)))");
    if(tmp == NULL || strlen(tmp) < 1) { tmp = ""; }
    strcpy(rsa_samples_ptr->one, tmp);

    tmp = cfg_get_string(&cfg, "sample.rsa.two", "(genkey (rsa (nbits 3:128)))");
    if(tmp == NULL || strlen(tmp) < 1) { tmp = ""; }
    strcpy(rsa_samples_ptr->two, tmp);

    tmp = cfg_get_string(&cfg, "sample.rsa.three", "(genkey (rsa (nbits 3:128)))");
    if(tmp == NULL || strlen(tmp) < 1) { tmp = ""; }
    strcpy(rsa_samples_ptr->three, tmp);

    tmp = cfg_get_string(&cfg, "sample.rsa.four", "(genkey (rsa (nbits 3:128)))");
    if(tmp == NULL || strlen(tmp) < 1) { tmp = ""; }
    strcpy(rsa_samples_ptr->four, tmp);

    free(entries);
    return 0;
}