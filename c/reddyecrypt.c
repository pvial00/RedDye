#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int keylen = 32;
int k[32];
int j = 0;

void keysetup(unsigned char *key, unsigned char *nonce) {
    int c;
    for (c=0; c < strlen(key); c++) {
        k[c] = (k[c] + key[c]) & 0xff 
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


unsigned char * crypt(unsigned char *data, unsigned char *key, unsigned char *nonce, long datalen) {
   keysetup(key, nonce);
   int c = 0;
   for (int x = 0; x < datalen; x++) {
       k[c % keylen] = (k[c % keylen] + k[(c + 1) % keylen] + j) & 0xff;
       j = (j + k[c % keylen] + c) & 0xff;
       data[x] = data[x] ^ k[c % keylen];
       c = (c + 1) & 0xff; } }

int main(int argc, char *argv[]) {
    FILE *infile, *outfile, *randfile;
    char *in, *out, *mode;
    unsigned char *data = NULL;
    unsigned char *buf = NULL;
    int x = 0;
    int ch;
    unsigned char *key;
    //int key_length = 16;
    int nonce_length = 16;
    unsigned char nonce[nonce_length];
    int i;
    mode = argv[1];
    in = argv[2];
    out = argv[3];
    key = argv[4];
    infile = fopen(in, "rb");
    fseek(infile, 0, SEEK_END);
    long fsize = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    data = malloc(fsize);
    fread(data, 1, fsize, infile);
    fclose(infile);
    outfile = fopen(out, "wb");
    if (strcmp(mode, "encrypt") == 0) {
        randfile = fopen("/dev/urandom", "rb");
        fread(&nonce, nonce_length, 1, randfile);
        fclose(randfile);
        crypt(data, key, nonce, fsize);
        fwrite(nonce, 1, nonce_length, outfile);
        fwrite(data, 1, fsize, outfile);
        free(data);
    }
    else if (strcmp(mode, "decrypt") == 0) {
        for (i = 0; i < nonce_length; i++) {
            nonce[i] = data[i];
        }
        unsigned char *msg = NULL;
        msg = malloc((fsize - nonce_length));
        x = 0;
        for (i = nonce_length; i < fsize; i++) {
            msg[x] = data[i];
            x++;
        }
        free(data);
        crypt(msg, key, nonce, (fsize - nonce_length));
        fwrite(msg, 1, x, outfile);
        free(msg);
    }
    fclose(outfile);
    return 0;
}
