#include "reddye_kdf2.c"

unsigned char * kdf (unsigned char *password) {
    unsigned char key[32] = {0};
    int iterations = 10;
    for (int x = 0; x < 32; x++) {
        key[x] = (key[x] + password[x]) % 256;
    }
    for (int x = 0; x < iterations; x++) {
        crypt(key, key, key, 32);
    }
}
