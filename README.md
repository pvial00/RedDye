# RedDye Stream Cipher

Authored by Karl Zander

Passes Dieharder randomness tests

High speed stream cipher that uses a 256 bit key to achieve ultimate security at the highest speeds.  Originally designed to beat Solitaire as the fastest hand cipher, RedDye offers more flexibility because it can be applied to binary data as implemented here.  RedDye26 offers a reduced version operating on A-Z characters.

RedDye performance is slightly faster than that of AES.

RedDye operates under the assumption that one cannot easily revert the formula below and find k which is XOR'ed with the plaintext byte.

k[c] = (k[c] + k[c + 1] + j) % 256

j = (j + k[c] + c) % 256

Recommended nonce length is 64-128bits and up to 256 bits.

# RedDye Hand Cipher

*** Coded in Python as the RedDye H4 Cipher

First one assigns a key to be used of desired length.  For this example we will use the key TESTINGKEY.  We convert those letter to numbers using the key that A = 0 and Z = 25.

MESSAGE = REDDYECIPHERISFASTANDEASYTOUSE

KEY = MYTESTINGKEY

MYTESTINGKEY = 12, 24, 19, 4, 18, 19, 8, 13, 6, 10, 4, 24

Then we sum all letters in the key mod 26 and designate that number as j.  In this case j = 5.

We then convert the plaintext letters into numbers and apply the following formula. c is a counter that begins with the first letter in the key.  i is a counter which begins with the second letter in the key.  We always modify the letter on the left each round and add or substract that to the input letter and then increment our counters each round.

REDDYECIPHERISFASTANDEASYTOUSE = 17, 4, 3, 3, 24, 4, 2, 8, 15, 7, 4, 17, 8, 18, 5, 0, 18, 19, 0, 13, 3, 4, 0, 18, 24, 19, 14, 20, 18, 4

k[c] = (k[c] + k[c + 1 ] + j) % 26

j = (j + k[c] + c) % 26

sub = (k[c] + char) % 26  # sub becomes your output character.  to decrypt simply substract here instead of add

c = (c + 1) % 26

CIPHERTEXT = 6, 15, 6, 10, 4, 10, 13, 8, 19, 21, 3, 10, 18, 12, 2, 11, 2, 8, 15, 2, 14, 15, 22, 17, 20, 22, 9, 16, 10, 4, 25

or

CIPHERTEXT = GPGKEKNITVDKSMCLCIPCOPWRUWJQKEZ

# C crypt function usage

crypt(data, key, nonce);


# Python class usage
reddye = RedDye()

ctxt = reddye.crypt(data, key)
