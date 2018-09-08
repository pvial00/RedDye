# RedDye Stream Cipher

High speed stream cipher that uses a 256 bit key to achieve ultimate security at the highest speeds.

Passes Dieharder entropy tests


# Python class usage
reddye = RedDye()

ctxt = reddye.crypt(data, key)
