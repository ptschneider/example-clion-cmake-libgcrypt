#include <gcrypt.h>
#include "dg_gcry.h"
#include "dg_logger.h"


int dg_keygen_demo(char* fname){

    gcrypt_init();

    char logbuf[128];
    FILE* lockf = fopen(fname, "wb");

    if (!lockf) {
        sprintf(logbuf, "error reading data file %s", fname);
        log_msg(DG_ERR, logbuf );

        xerr("fopen() failed");
    }

    /* Generate a new RSA key pair. */
    printf("RSA key generation can take a few minutes. Your computer \n"
           "needs to gather random entropy. Please wait... \n\n");

    gcry_error_t err = 0;
    gcry_sexp_t rsa_parms;
    gcry_sexp_t rsa_keypair;

    log_msg_nano(DG_INFO, "building s-expr");

    err = gcry_sexp_build(&rsa_parms, NULL, "(genkey (rsa (nbits 4:2048)))");
    if (err) {
        xerr("gcrypt: failed to create rsa params");
    }

    log_msg_nano(DG_INFO, "generating rsa key");

    err = gcry_pk_genkey(&rsa_keypair, rsa_parms);
    if (err) {
        xerr("gcrypt: failed to create rsa key pair");
    }

    log_msg_nano(DG_INFO, "creating aes context");

    printf("RSA key generation complete! Please enter a password to lock \n"
           "your key pair. This password must be committed to memory. \n\n");

    /* Grab a key pair password and create an encryption context with it. */
    gcry_cipher_hd_t aes_hd;
    get_aes_ctx(&aes_hd);

    log_msg_nano(DG_INFO, "writing rsa keypair to plaintext buffer");

    /* Encrypt the RSA key pair. */
    size_t rsa_len = get_keypair_size(2048);
    void* rsa_buf = calloc(1, rsa_len);
    if (!rsa_buf) {
        xerr("malloc: could not allocate rsa buffer");
    }
    gcry_sexp_sprint(rsa_keypair, GCRYSEXP_FMT_CANON, rsa_buf, rsa_len);

    log_msg_nano(DG_INFO, "plaintext:");
    log_msg_nano(DG_INFO, rsa_buf);

    log_msg_nano(DG_INFO, "aes-encrypting rsa keypair");

    err = gcry_cipher_encrypt(aes_hd, (unsigned char*) rsa_buf,
                              rsa_len, NULL, 0);
    if (err) {
        xerr("gcrypt: could not encrypt with AES");
    }

    log_msg_nano(DG_INFO, "writing aes ciphertext");

    /* Write the encrypted key pair to disk. */
    if (fwrite(rsa_buf, rsa_len, 1, lockf) != 1) {
        perror("fwrite");
        xerr("fwrite() failed");
    }
    else{
        printf("done: wrote %zuk bytes to %s", rsa_len/1024, fname);
    }

    /* Release contexts. */
    gcry_sexp_release(rsa_keypair);
    gcry_sexp_release(rsa_parms);
    gcry_cipher_close(aes_hd);
    free(rsa_buf);
    fclose(lockf);

    return 0;
}

int dg_keygen_rsa(char* fname){

    gcrypt_init();

    FILE* lockf = fopen(fname, "wb");
    if (!lockf) {
        xerr("fopen() failed");
    }

    /* Generate a new RSA key pair. */
    printf("RSA key generation can take a few minutes. Your computer \n"
           "needs to gather random entropy. Please wait... \n\n");

    gcry_error_t err = 0;
    gcry_sexp_t rsa_parms;
    gcry_sexp_t rsa_keypair;
    const char* rsa_sexpr_fmt_default = "(genkey (rsa (nbits 4:2048)))";

    log_msg_nano(DG_INFO, "building s-expr");

    err = gcry_sexp_build(&rsa_parms, NULL, rsa_sexpr_fmt_default);
    if (err) {
        xerr("gcrypt: failed to create rsa params");
    }

    log_msg_nano(DG_INFO, "generating rsa keypair");

    err = gcry_pk_genkey(&rsa_keypair, rsa_parms);
    if (err) {
        xerr("gcrypt: failed to create rsa key pair");
    }



    log_msg_nano(DG_INFO, "creating aes context");

    printf("RSA key generation complete! Please enter a password to lock \n"
           "your key pair. This password must be committed to memory. \n\n");

    /* Grab a key pair password and create an encryption context with it. */
    gcry_cipher_hd_t aes_hd;
    get_aes_ctx(&aes_hd);

    /* Encrypt the RSA key pair. */
    size_t rsa_len = get_keypair_size(2048);
    void* rsa_buf = calloc(1, rsa_len);
    if (!rsa_buf) {
        xerr("malloc: could not allocate rsa buffer");
    }

    log_msg_nano(DG_INFO, "writing rsa keypair to plaintext buffer");

    /* GCRYSEXP_FMT_BASE64 GCRYSEXP_FMT_CANON GCRYSEXP_FMT_DEFAULT GCRYSEXP_FMT_ADVANCED */
    gcry_sexp_sprint(rsa_keypair, GCRYSEXP_FMT_CANON, rsa_buf, rsa_len);

    log_msg_nano(DG_INFO, "plaintext:");
    log_msg_nano(DG_INFO, rsa_buf);

    log_msg_nano(DG_INFO, "encrypting rsa keypair with aes-256");

    err = gcry_cipher_encrypt(aes_hd, (unsigned char*) rsa_buf,
                              rsa_len, NULL, 0);
    if (err) {
        xerr("gcrypt: could not encrypt with AES");
    }

    log_msg_nano(DG_INFO, "writing encrypteGCRYSEXP_FMT_ADVANCEDd payload to file");

    /* Write the encrypted key pair to disk. */
    if (fwrite(rsa_buf, rsa_len, 1, lockf) != 1) {
        perror("fwrite");
        xerr("fwrite() failed");
    }
    else{
      printf("done: wrote %zuk bytes to %s\n", rsa_len/1024, fname);
    }

    /* Release contexts. */
    gcry_sexp_release(rsa_keypair);
    gcry_sexp_release(rsa_parms);
    gcry_cipher_close(aes_hd);
    free(rsa_buf);
    fclose(lockf);

    return 0;
}

int dg_decrypt_rsa(char* fname) {

    gcrypt_init();
    gcry_error_t err;

    FILE* lockf = fopen(fname, "rb");
    if (!lockf) {
        xerr("fopen() failed");
    }

    /* Grab a key pair password and create an AES context with it. */
    gcry_cipher_hd_t aes_hd;
    get_aes_ctx(&aes_hd);

    /* Read and decrypt the key pair from disk. */
    size_t rsa_len = get_keypair_size(2048);
    void* rsa_buf = calloc(1, rsa_len);
    if (!rsa_buf) {
        xerr("malloc: could not allocate rsa buffer");
    }

    if (fread(rsa_buf, rsa_len, 1, lockf) != 1) {
        xerr("fread() failed");
    }

    err = gcry_cipher_decrypt(aes_hd, (unsigned char*) rsa_buf,
                              rsa_len, NULL, 0);
    if (err) {
        xerr("gcrypt: failed to decrypt key pair");
    }

    /* Load the key pair components into sexps. */
    gcry_sexp_t rsa_keypair;
    err = gcry_sexp_new(&rsa_keypair, rsa_buf, rsa_len, 0);
    gcry_sexp_t pubk = gcry_sexp_find_token(rsa_keypair, "public-key", 0);
    gcry_sexp_t privk = gcry_sexp_find_token(rsa_keypair, "private-key", 0);

    /* Create a message. */
    gcry_mpi_t msg;
    gcry_sexp_t data;
    const unsigned char* s = (const unsigned char*) "Hello world.";
    err = gcry_mpi_scan(&msg, GCRYMPI_FMT_USG, s,
                        strlen((const char*) s), NULL);
    if (err) {
        xerr("failed to create a mpi from the message");
    }

    err = gcry_sexp_build(&data, NULL,
                           "(data (flags raw) (value %m))", msg);
    if (err) {
        xerr("failed to create a sexp from the message");
    }

    /* Encrypt the message. */
    gcry_sexp_t ciph;
    err = gcry_pk_encrypt(&ciph, data, pubk);
    if (err) {
        xerr("gcrypt: encryption failed");
    }

    /* Decrypt the message. */
    gcry_sexp_t plain;
    err = gcry_pk_decrypt(&plain, ciph, privk);
    if (err) {
        xerr("gcrypt: decryption failed");
    }

    /* Pretty-print the results. */
    gcry_mpi_t out_msg = gcry_sexp_nth_mpi(plain, 0, GCRYMPI_FMT_USG);
    printf("Original:\n");
    gcry_mpi_dump(msg);
    printf("\n" "Decrypted:\n");
    gcry_mpi_dump(out_msg);
    printf("\n");

    if (gcry_mpi_cmp(msg, out_msg)) {
        xerr("data corruption!");
    }

    printf("Messages match.\n");

    unsigned char obuf[64] = { 0 };
    err = gcry_mpi_print(GCRYMPI_FMT_USG, (unsigned char*) &obuf,
                         sizeof(obuf), NULL, out_msg);
    if (err) {
        xerr("failed to stringify mpi");
    }
    printf("-> %s\n", (char*) obuf);


    /* Release contexts. */
    gcry_mpi_release(msg);
    gcry_mpi_release(out_msg);
    gcry_sexp_release(rsa_keypair);
    gcry_sexp_release(pubk);
    gcry_sexp_release(privk);
    gcry_sexp_release(data);
    gcry_sexp_release(ciph);
    gcry_sexp_release(plain);
    gcry_cipher_close(aes_hd);
    free(rsa_buf);
    fclose(lockf);

    return 0;
}