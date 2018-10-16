#ifndef reddye_h__
#define reddye_h__

unsigned char * crypt(unsigned char *data, unsigned char *key, unsigned char *nonce, long datalen);

unsigned char * reddye_random (unsigned char *buf, int num_bytes);

#endif 
