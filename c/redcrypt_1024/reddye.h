#ifndef reddye_h__
#define reddye_h__

unsigned char * crypt(unsigned char *data, unsigned char *key, unsigned char *nonce, long datalen);

unsigned char * kdf(unsigned char *password, unsigned char *key, unsigned char *salt, int iterations, int keylen);

#endif 
