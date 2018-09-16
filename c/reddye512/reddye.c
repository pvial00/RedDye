#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *crypt(unsigned char *data, unsigned char *key, unsigned char *nonce, long datalen) {
    int keylen = 64;
    int k[64] = {0};
    int j = 0;
    int c;
    for (c=0; c < strlen(key); c++) {
        k[c] = (k[c] + key[c]) & 0xff;
        j = (j + k[c]) & 0xff; }
    keylen = strlen(key);
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) & 0xff;
        j = (j + k[c % keylen]) & 0xff; }
    for (c = 0; c < strlen(nonce); c++) {
        k[c] = (k[c] + nonce[c]) & 0xff;
        j = (j + k[c]) % 256; }
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) & 0xff;
        j = (j + k[c % keylen]) & 0xff; }

   c = 0;
   for (int x = 0; x < datalen; x++) {
       k[c % keylen] = (k[c % keylen] + k[(c + 1) % keylen] + j) & 0xff;
       j = (j + k[c % keylen] + c) & 0xff;
       data[x] = data[x] ^ k[c % keylen];
       c = (c + 1) & 0xff;
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
