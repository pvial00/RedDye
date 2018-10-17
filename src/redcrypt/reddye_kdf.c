#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char * kdf (unsigned char *password, unsigned char *key, unsigned char *salt, int iterations, int keylen) {
    for (int x = 0; x < keylen; x++) {
        key[x] = 0;
    }
    int n = 0;
    for (int x = 0; x < strlen(password); x++) {
        key[n] = (key[n] + password[x]) % 256;
        n = (n + 1) % keylen;
    }
    int kdf_k[256];
    for (int x = 0; x < 256; x++) {
        kdf_k[x] = 0;
    }
    int t = 0;
    int r = 0;
    int d = 256 - keylen;
    int y = 256 / 2;
    int out;
    for (n=0; n < keylen; n++) {
        kdf_k[n % keylen] = (kdf_k[n % keylen] + key[n % keylen]) & 0xff;
        t = (t + kdf_k[n % keylen]) & 0xff; }
    for (n = 0; n < 256; n++) {
        kdf_k[n % keylen] = (kdf_k[n % keylen] + t) & 0xff;
        t = (t + kdf_k[n % keylen]) & 0xff; }
    int saltlen = sizeof(salt);
    for (n = 0; n < keylen; n++) {
        kdf_k[n] = (kdf_k[n] + salt[n % saltlen]) & 0xff;
        t = (t + kdf_k[n]) & 0xff; }
    for (n = 0; n < 256; n++) {
        kdf_k[n % keylen] = (kdf_k[n % keylen] + t) & 0xff;
        t = (t + kdf_k[n % keylen]) & 0xff; }
    for (n = 0; n < d; n++) {
        kdf_k[n+keylen] = (kdf_k[n] + kdf_k[(n + 1) % d] + t) & 0xff;
	t = (t + kdf_k[n % d] + n) & 0xff; }
    for (n = 0; n < 256; n++) {
        kdf_k[n] = (kdf_k[n] + kdf_k[(n + y) & 0xff] + t) & 0xff;
	t = (t + kdf_k[n] + n) & 0xff; }

    n = 0;
    for (int x = 0; x < (256 * iterations); x++) {
       kdf_k[r] = (kdf_k[r] + kdf_k[(r + 1) % keylen] + t) & 0xff;
       t = (t + kdf_k[r] + n) & 0xff;
       out = ((t + kdf_k[r]) & 0xff) ^ kdf_k[r];
       key[r] = (unsigned char)key[r] ^ out;
       n = (n + 1) & 0xff;
       r = (r + 1) % keylen;
    }
}
