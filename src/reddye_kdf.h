#ifndef reddye_kdf_h__
#define reddye_kdf_h__

unsigned char * kdf(unsigned char *password, unsigned char *key, unsigned char *salt, int iterations, int keylen);

#endif 
