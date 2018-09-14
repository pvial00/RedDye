#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int kkdf[32];
int t = 0;

void keysetup(unsigned char *key, unsigned char *nonce) {
    int c;
    for (c=0; c < strlen(key); c++) {
        kkdf[c] = (kkdf[c] + key[c]) & 0xff;
        t = (t + kkdf[c]) & 0xff; }
    keylen = strlen(kkdf);
    for (c = 0; c < 256; c++) {
        kkdf[c % keylen] = (kkdf[c % keylen] + t) & 0xff;
        t = (t + kkdf[c % keylen]) & 0xff; }
    for (c = 0; c < strlen(nonce); c++) {
        kkdf[c] = (kkdf[c] + nonce[c]) & 0xff;
        t = (t + kkdf[c]) % 256; }
    for (c = 0; c < 256; c++) {
        kkdf[c % keylen] = (kkdf[c % keylen] + t) & 0xff;
        t = (t + kkdf[c % keylen]) & 0xff; }
}


unsigned char * crypt(unsigned char *data, unsigned char *key, unsigned char *nonce, long datalen) {
   keysetup(key, nonce);
   int c = 0;
   for (int x = 0; x < datalen; x++) {
       kkdf[c % keylen] = (kkdf[c % keylen] + kkdf[(c + 1) % keylen] + t) & 0xff;
       t = (t + kkdf[c % keylen] + c) & 0xff;
       data[x] = data[x] ^ kkdf[c % keylen];
       c = (c + 1) & 0xff;
   } 
   t = 0;
}
