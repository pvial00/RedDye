#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int keylen = 32;
int k[32];
int j = 0;

void keysetup(unsigned char *key, unsigned char *nonce) {
    int c;
    for (c=0; c < strlen(key); c++) {
        k[c] = (k[c] + key[c]) % 256;
        j = (j + k[c]) % 256; }
    keylen = strlen(key);
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) % 256;
        j = (j + k[c % keylen]) % 256; }
    for (c = 0; c < strlen(nonce); c++) {
        k[c] = (k[c] + nonce[c]) % 256;
        j = (j + k[c]) % 256; }
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) % 256;
        j = (j + k[c % keylen]) % 256; }
}


unsigned char * crypt(unsigned char *data, unsigned char *key, unsigned char *nonce, long datalen) {
   keysetup(key, nonce);
   int c = 0;
   int i = 1;
   for (int x = 0; x < datalen; x++) {
       k[c % keylen] = (k[c % keylen] + k[i] + j) % 256;
       j = (j + k[c % keylen] + c) % 256;
       data[x] = data[x] ^ k[c % keylen];
       i = (i + 1) % 256;
       c = (c + 1) % 256; } }
