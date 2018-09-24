#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *crypt(unsigned char *data, unsigned char *key, unsigned char *nonce, long datalen) {
    int keylen = 32;
    int k[32] = {0};
    int j = 0;
    int i = 0;
    int c;
    for (c=0; c < keylen; c++) {
        k[c % keylen] = (k[c % keylen] + key[c % keylen]) & 0xff;
        j = (j + k[c % keylen]) & 0xff; }
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) & 0xff;
        j = (j + k[c % keylen]) & 0xff; }
    for (c = 0; c < sizeof(nonce); c++) {
        k[c % keylen] = (k[c % keylen] + nonce[c]) & 0xff;
        j = (j + k[c % keylen]) & 0xff; }
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) & 0xff;
        j = (j + k[c % keylen]) & 0xff; }

   c = 0;
   for (int x = 0; x < datalen; x++) {
       k[i] = (k[i] + k[(i + 1) % keylen] + j) & 0xff;
       j = (j + k[i] + c) & 0xff;
       data[x] = data[x] ^ k[i];
       c = (c + 1) & 0xff;
       i = (i + 1) % keylen;
   } 
}

unsigned char * kdf (unsigned char *password, unsigned char *key, unsigned char *salt, int iterations, int keylen) {
    for (int x = 0; x < keylen; x++) {
        key[x] = 0;
    }
    for (int x = 0; x < strlen(password); x++) {
        key[x] = (key[x] + password[x]) % 256;
    }
    for (int x = 0; x < iterations; x++) {
        crypt(key, key, salt, keylen);
    }
}

unsigned char * reddye_random (unsigned char *buf, int num_bytes) {
    int keylen = 32;
    int noncelen = 16;
    unsigned char *key[keylen];
    unsigned char nonce[noncelen];
    FILE *randfile;
    randfile = fopen("/dev/urandom", "rb");
    fread(&nonce, noncelen, 1, randfile);
    fread(key, keylen, 1, randfile);
    fclose(randfile);
    crypt(buf, key, nonce, sizeof(buf));
}
