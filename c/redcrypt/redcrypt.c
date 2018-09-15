#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reddye.c"

int keylen = 32;
//int k[] = { 83, 41, 201, 107, 175, 150, 205, 119, 188, 140, 154, 210, 72, 30, 194, 189, 130, 9, 134, 201, 11, 145, 29, 179, 192, 0, 125, 209, 184, 132, 207, 219 };
int k[32] = {0};
int j = 0;

void keysetup(unsigned char *key, unsigned char *nonce) {
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
        j = (j + k[c]) & 0xff; }
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) & 0xff;
        j = (j + k[c % keylen]) & 0xff; }
}

void usage() {
    printf("reddye <encrypt/decrypt> <input file> <output file> <password>\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    FILE *infile, *outfile, *randfile;
    char *in, *out, *mode;
    unsigned char *data = NULL;
    unsigned char *buf = NULL;
    int x = 0;
    int ch;
    int buflen = 131072;
    int bsize;
    unsigned char *key[keylen];
    unsigned char *password;
    int nonce_length = 16;
    int iterations = 10;
    unsigned char *salt = "RedDyeCipher";
    unsigned char nonce[nonce_length];
    unsigned char block[buflen];
    if (argc != 5) {
        usage();
    }
    mode = argv[1];
    in = argv[2];
    out = argv[3];
    password = argv[4];
    infile = fopen(in, "rb");
    fseek(infile, 0, SEEK_END);
    long fsize = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    outfile = fopen(out, "wb");
    int c = 0;
    if (strcmp(mode, "encrypt") == 0) {
        long blocks = fsize / buflen;
        long extra = fsize % buflen;
        if (extra != 0) {
            blocks += 1;
        }
        randfile = fopen("/dev/urandom", "rb");
        fread(&nonce, nonce_length, 1, randfile);
        fclose(randfile);
        fwrite(nonce, 1, nonce_length, outfile);
	kdf(password, key, salt, iterations, keylen);
        keysetup(key, nonce);
        for (int d = 0; d < blocks; d++) {
            fread(block, buflen, 1, infile);
            bsize = sizeof(block);
            for (int b = 0; b < bsize; b++) {
                k[c % keylen] = (k[c % keylen] + k[(c + 1) % keylen] + j) & 0xff;
                j = (j + k[c % keylen] + c) & 0xff;
                block[b] = block[b] ^ k[c % keylen];
                c = (c + 1) & 0xff;
            }
            if (d == (blocks - 1) && extra != 0) {
                bsize = extra;
            }
            fwrite(block, 1, bsize, outfile);
        }
    }
    else if (strcmp(mode, "decrypt") == 0) {
        long blocks = (fsize - nonce_length) / buflen;
        long extra = (fsize - nonce_length) % buflen;
        if (extra != 0) {
            blocks += 1;
        }
        fread(nonce, 1, nonce_length, infile);
	kdf(password, key, salt, iterations, keylen);
        keysetup(key, nonce);
        for (int d = 0; d < blocks; d++) {
            fread(block, buflen, 1, infile);
            bsize = sizeof(block);
            for (int b = 0; b < bsize; b++) {
                k[c % keylen] = (k[c % keylen] + k[(c + 1) % keylen] + j) & 0xff;
                j = (j + k[c % keylen] + c) & 0xff;
                block[b] = block[b] ^ k[c % keylen];
                c = (c + 1) & 0xff;
            }
            if ((d == (blocks - 1)) && extra != 0) {
                bsize = extra;
            }
            fwrite(block, 1, bsize, outfile);
        }
    }
    fclose(infile);
    fclose(outfile);
    return 0;
}
