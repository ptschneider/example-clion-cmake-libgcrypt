#ifndef DG_GET_CONFIGFILE_H
#define DG_GET_CONFIGFILE_H

#define DG_RSA_SAMPLE_LEN_MAX 32

typedef struct {
    char one[DG_RSA_SAMPLE_LEN_MAX];
    char two[DG_RSA_SAMPLE_LEN_MAX];
    char three[DG_RSA_SAMPLE_LEN_MAX];
    char four[DG_RSA_SAMPLE_LEN_MAX];
} rsa_samples_t;

int dg_get_config(char* fname,rsa_samples_t* rsa_samples_ptr);

#endif