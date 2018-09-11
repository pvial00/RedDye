#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
int kdf_keylen = 32;
int kdf_k[] = { 83, 41, 201, 107, 175, 150, 205, 119, 188, 140, 154, 210, 72, 30, 194, 189, 130, 9, 134, 201, 11, 145, 29, 179, 192, 0, 125, 209, 184, 132, 207, 219 };
int t = 0;
int iterations = 10;

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

unsigned char * kdf_crypt(unsigned char *key, unsigned char *nonce, long flen) {
   kdf_keysetup(key, nonce);
   int c = 0;
   for (int x = 0; x < flen; x++) {
       kdf_k[c % kdf_keylen] = (kdf_k[c % kdf_keylen] + kdf_k[(c + 1) % kdf_keylen] + t) % 256;
       t = (t + kdf_k[c % kdf_keylen] + c) % 256;
       key[x] = key[x] ^ kdf_k[c % kdf_keylen];
       c = (c + 1) % 256; } return key; }

unsigned char * kdf(unsigned char *key) {
    unsigned char n[16] = "01234567890ABCDEF";
    for (int x = 0; x < iterations; x++) {
        kdf_crypt(key, n, kdf_keylen);
    }
}

