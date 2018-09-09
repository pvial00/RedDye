#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
int kdf_keylen = 32;
int kdf_k[32];
int t = 0;

void kdf_keysetup(unsigned char *key, unsigned char *nonce) {
    int c;
    for (c=0; c < sizeof(key); c++) {
        kdf_k[c] = (kdf_k[c] + key[c]) % 256;
        t = (t + kdf_k[c]) % 256; }
    kdf_keylen = sizeof(key);
    for (c = 0; c < 256; c++) {
        kdf_k[c % kdf_keylen] = (kdf_k[c % kdf_keylen] + t) % 256;
        t = (t + kdf_k[c % kdf_keylen]) % 256; }
}

unsigned char * kdf_crypt(unsigned char *f, unsigned char *key, unsigned char *nonce, long flen) {
   kdf_keysetup(key, nonce);
   int c = 0;
   int i = 1;
   for (int x = 0; x < flen; x++) {
       kdf_k[c % kdf_keylen] = (kdf_k[c % kdf_keylen] + kdf_k[i] + t) % 256;
       t = (t + kdf_k[c % kdf_keylen] + c) % 256;
       f[x] = f[x] ^ kdf_k[c % kdf_keylen];
       i = (i + 1) % 256;
       c = (c + 1) % 256; } }

unsigned char * kdf(unsigned char *key) {
    unsigned char n[16] = "01234567890ABCDEF";
    int iterations = 10;
    for (int x = 0; x < iterations; x++) {
        kdf_crypt(key, key, n, sizeof(key));
    }
}

