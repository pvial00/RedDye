#include <stdio.h>
#include <string.h>
#include "reddye.h"

int main(void)
{
    unsigned char msg[10] = "HELLOWORLD";
    unsigned char key[10] = "TESTINGKEY";
    unsigned char nonce[5] = "NONCE";
    printf("In main\n");
    printf("%s", msg);
    crypt(msg, key, nonce, strlen(msg));
    printf("%s", msg);
}
    
