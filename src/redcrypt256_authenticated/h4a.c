#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char * h4a_mac (unsigned char *data, int datalen, unsigned char *mac, unsigned char *key, int keylen) {
    int maclen = 16;
    for (int x = 0; x < maclen; x++) {
        mac[x] = 0;
    }
    int mac_k[256];
    for (int x = 0; x < 256; x++) {
        mac_k[x] = 0;
    }
    int t = 0;
    int r = 0;
    int n;
    int out;
    int in;
    int d = 256 - keylen;
    int y = (256 / 2) - 1;
    int multiplier = 2;
    for (n=0; n < keylen; n++) {
        mac_k[n] = (mac_k[n] + key[n]) & 0xff;
        t = (t + mac_k[n]) & 0xff; }
    for (n = 0; n < 256; n++) {
        mac_k[n] = (mac_k[n] + mac_k[(n + 1) & 0xff]+ t) & 0xff;
        t = (t + mac_k[n] + n) & 0xff; }
    for (n = 0; n < d; n++) {
        mac_k[n+keylen] = (mac_k[n] + mac_k[(n + 1) % d] + t) & 0xff;
	t = (t + mac_k[n % d] + n) & 0xff; }
    for (n = 0; n < 256; n++) {
        mac_k[n] = (mac_k[n] + mac_k[(n + y) & 0xff] + t) & 0xff;
	t = (t + mac_k[n] + n) & 0xff; }
    
    n = 0;
    int o = 0;
    for (long x = 0; x < datalen; x++) {
	mac_k[o] = (mac_k[o] + mac_k[(o + 1) & 0xff] + t) & 0xff;
	t = (t + mac_k[o] + n) & 0xff;
	out = ((t + mac_k[o]) & 0xff) ^ mac_k[o];
	in = out ^ data[x];
        mac[n] = ((mac[n] + data[x]) & 0xff) ^ in;
        n = (n + 1) % maclen;
	o = (o + 1) & 0xff;

    }

    n = 0;
    o = 0;
    for (int x = 0; x < (maclen * multiplier); x++) {
       mac_k[o] = (mac_k[o] + mac_k[(o + 1) & 0xff] + t) & 0xff;
       t = (t + mac_k[o] + o) & 0xff;
       out = ((t + mac_k[o]) & 0xff) ^ mac_k[o];
       mac[n] ^= out; 
       n = (n + 1) % maclen;
       o = (o + 1) & 0xff;
    }
}
