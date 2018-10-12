#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char * h4a_mac (unsigned char *data, unsigned char *mac, unsigned char *key) {
    int maclen = 16;
    for (int x = 0; x < maclen; x++) {
        mac[x] = 0;
    }
    int mac_k[maclen];
    int reg[maclen];
    for (int x = 0; x < maclen; x++) {
        mac_k[x] = 0;
	reg[maclen] = 0;
    }
    int t = 0;
    int r = 0;
    int n;
    int out;
    int in;
    for (n=0; n < maclen; n++) {
        mac_k[n % maclen] = (mac_k[n % maclen] + key[n % maclen]) & 0xff;
        t = (t + mac_k[n % maclen]) & 0xff; }
    for (n = 0; n < 256; n++) {
        mac_k[n % maclen] = (mac_k[n % maclen] + mac_k[(n + 1) % maclen]+ t) & 0xff;
        t = (t + mac_k[n % maclen] + n) & 0xff; }
    
    n = 0;
    for (long x = 0; x < strlen(data); x++) {
	mac_k[n] = (mac_k[n] + mac_k[(n + 1) % maclen] + t) & 0xff;
	t = (t + mac_k[n % maclen]) & 0xff;
	out = ((t + mac_k[n]) & 0xff) ^ mac_k[n];
	in = out ^ data[x];
	reg[n] = (reg[n] + data[x]) & 0xff;
        mac[n] = ((mac[n] + in) & 0xff) ^ reg[n];
        n = (n + 1) % maclen;
    }

    n = 0;
    for (int x = 0; x < (maclen * 2); x++) {
       mac_k[n] = (mac_k[n] + mac_k[(n + 1) % maclen] + t) & 0xff;
       t = (t + mac_k[n % maclen]) & 0xff;
       out = ((t + mac_k[n]) & 0xff) ^ mac_k[n];
       mac[n] ^= out; 
       n = (n + 1) % maclen;
    }
}
