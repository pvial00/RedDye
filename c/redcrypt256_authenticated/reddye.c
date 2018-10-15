#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *crypt(unsigned char *data, unsigned char *key, unsigned char *nonce, long datalen) {
    int keylen = 32;
    int diff = 256 - keylen;
    int k[256] = {0};
    int j = 0;
    int i = 0;
    int c;
    int m = (256 / 2) - 1;
    int output;
    for (c=0; c < keylen; c++) {
        k[c % keylen] = (k[c % keylen] + key[c % keylen]) & 0xff;
        j = (j + k[c % keylen]) & 0xff; }
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) & 0xff;
        j = (j + k[c % keylen] + c) & 0xff; }
    for (c = 0; c < strlen(nonce); c++) {
        k[c % keylen] = (k[c % keylen] + nonce[c]) & 0xff;
        j = (j + k[c % keylen]) & 0xff; }
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) & 0xff;
        j = (j + k[c % keylen] + c) & 0xff; }
    for (c = 0; c < diff; c++) {
        k[c+keylen] = (k[c] + k[(c + 1) % diff] + j) & 0xff;
	j = (j + k[c % diff] + c) & 0xff; }
    for (c = 0; c < 256; c++) {
        k[c] = (k[c] + k[(c + m) & 0xff] + j) & 0xff;
        j = (j + k[c] + c) & 0xff; }


   c = 0;
   for (int x = 0; x < datalen; x++) {
       k[c] = (k[c] + k[(c + 1) & 0xff] + j) & 0xff;
       j = (j + k[c] + c) & 0xff;
       output = ((j + k[c]) & 0xff) ^ k[c];
       data[x] = data[x] ^ output;
       c = (c + 1) & 0xff;
   } 
}

unsigned char * reddye_random (unsigned char *buf, int num_bytes) {
    int keylen = 256;
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
