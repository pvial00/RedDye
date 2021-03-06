#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reddye.c"
#include "reddye_kdf.c"
#include "h4a.c"

int keylen = 32;
int noncelen = 8;
int k[256] = {0};
int j = 0;

void keysetup(unsigned char *key, unsigned char *nonce, int keylen , int noncelen) {
    int c;
    int diff = 256 - keylen;
    int m = keylen / 2;
    for (c=0; c < keylen; c++) {
        k[c] = (k[c] + key[c]) & 0xff;
        j = (j + k[c]) & 0xff; }
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) & 0xff;
        j = (j + k[c % keylen] + c) & 0xff; }
    for (c = 0; c < noncelen; c++) {
        k[c] = (k[c] + nonce[c]) & 0xff;
        j = (j + k[c]) & 0xff; }
    for (c = 0; c < 256; c++) {
        k[c % keylen] = (k[c % keylen] + j) & 0xff;
        j = (j + k[c % keylen] + c) & 0xff; }
    for (c = 0; c < diff; c++) {
        k[c+keylen] = (k[c] + k[(c + 1) % diff] + j) & 0xff;
        j = (j + k[c % diff] + c) & 0xff; }
    for (c = 0; c < 256; c ++) {
        k[c] = (k[c] + k[(c + m) & 0xff] + j) & 0xff;
        j = (j + k[c] + c) & 0xff; }
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
    int output;
    unsigned char *key[keylen];
    unsigned char *password;
    int mac_length = 16;
    int iterations = 10000;
    unsigned char *salt = "RedDyeCipher";
    int saltlen = 12;
    unsigned char *nonce[noncelen];
    unsigned char *mac[mac_length];
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
	reddye_random(nonce, noncelen);
        fwrite(nonce, 1, noncelen, outfile);
	kdf(password, key, salt, iterations, keylen, saltlen);
        keysetup(key, nonce, keylen, noncelen);
        for (int d = 0; d < blocks; d++) {
            fread(block, buflen, 1, infile);
            bsize = sizeof(block);
            for (int b = 0; b < bsize; b++) {
                k[c] = (k[c] + k[(c + 1) & 0xff] + j) & 0xff;
                j = (j + k[c] + c) & 0xff;
		output = ((j + k[c]) & 0xff) ^ k[c];
                block[b] = block[b] ^ output;
                c = (c + 1) & 0xff;
            }
            if (d == (blocks - 1) && extra != 0) {
                bsize = extra;
            }
            fwrite(block, 1, bsize, outfile);
        }
	fclose(outfile);
        outfile = fopen(out, "rb");
        fseek(outfile, 0, SEEK_END);
        long fsize = ftell(outfile);
        fseek(outfile, 0, SEEK_SET);
	unsigned char * ctxt = NULL;
	ctxt = malloc(fsize);
	fread(ctxt, 1, fsize, outfile);
	fclose(outfile);
	h4a_mac(ctxt, fsize, mac, key, keylen);
        outfile = fopen(out, "wb");
	fwrite(mac, 1, mac_length, outfile);
	fwrite(ctxt, 1, fsize, outfile);
	free(ctxt);
	fclose(outfile);
    }
    else if (strcmp(mode, "decrypt") == 0) {
        long blocks = (fsize - mac_length - noncelen) / buflen;
        long extra = (fsize - mac_length  - noncelen) % buflen;
        if (extra != 0) {
            blocks += 1;
        }
        fread(mac, 1, mac_length, infile);
	unsigned char * ctxt = NULL;
	ctxt = malloc((fsize - mac_length));
        fread(ctxt, 1, (fsize - mac_length), infile);
        fseek(infile, 0, SEEK_SET);
	unsigned char * verification_mac[mac_length];
	kdf(password, key, salt, iterations, keylen, saltlen);
	h4a_mac(ctxt, (fsize - mac_length), verification_mac, key, keylen);
	free(ctxt);
	if (memcmp(mac, verification_mac, mac_length) == 0) {
            fread(mac, 1, mac_length, infile);
            fread(nonce, 1, noncelen, infile);
            keysetup(key, nonce, keylen, noncelen);
            for (int d = 0; d < blocks; d++) {
                fread(block, buflen, 1, infile);
                bsize = sizeof(block);
                for (int b = 0; b < bsize; b++) {
                    k[c] = (k[c] + k[(c + 1) & 0xff] + j) & 0xff;
                    j = (j + k[c] + c) & 0xff;
		    output = ((j + k[c]) & 0xff) ^ k[c];
                    block[b] = block[b] ^ output;
                    c = (c + 1) & 0xff;
                }
                if ((d == (blocks - 1)) && extra != 0) {
                    bsize = extra;
                }
                fwrite(block, 1, bsize, outfile);
            }
	}
	else {
	    printf("Error: Message has been tampered with.\n");
	}
    }
    fclose(infile);
    fclose(outfile);
    return 0;
}
