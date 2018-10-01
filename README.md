# RedDye Stream Cipher

*** Warning this cipher has not undergone heavy cryptanalysis and should not be used in production systems

Authored by Karl Zander

Passes Dieharder randomness tests

High speed stream cipher that uses a primarily 256 bit key (optionally 128, 512, 1024 and 2048) to achieve ultimate security at the highest speeds.  Originally designed to beat Solitaire as the fastest, most secure hand cipher, RedDye offers more flexibility because it can be applied to binary data as implemented here.  RedDye26 offers a reduced version operating on A-Z characters.

RedDye performance is faster than that of ASM implementations AES and ChaCha20.

RedDye's construction begins with a 256-bit (32 byte) key and nonce that are combined to form the array k[].  j is a value that is a rolling sum total of all calulated k[] values mod 256.  j is passed on to the encryption function.  k[] is processed two bytes at a time through the encryption algorithm.  The encryption algorithm outputs one byte at a time.  RedDye maintains two counters, c and i.  i is used only to iterate over k[].  c is used as an 8-bit counter which is combined with j to maintain uniformity.

RedDye operates under the assumption that one cannot easily revert the encryption formula below and find k which is XOR'ed with the plaintext byte.

k[i] = (k[i] + k[i + 1] + j) % 256

j = (j + k[i] + c) % 256

Recommended nonce length is 64-128bits.

# Cryptanalysis
Under a known plaintext attack knowing the first 32 bytes of the plaintext or plaintext equal to the length of the key.  k[] can be recalculated and the message decrypted.

# RedDye Hand Cipher

*** Coded in Python as the RedDye H4 Cipher

First one assigns a key to be used of desired length.  For this example we will use the key TESTINGKEY.  We convert those letter to numbers using the key that A = 0 and Z = 25.

MESSAGE = REDDYECIPHERISFASTANDEASYTOUSE

KEY = MYTESTINGKEY

MYTESTINGKEY = 12, 24, 19, 4, 18, 19, 8, 13, 6, 10, 4, 24

Then we sum all letters in the key mod 26 and designate that number as j.  In this case j = 5.

We then convert the plaintext letters into numbers and apply the following formula. c is a counter that begins with the first letter in the key.  We always modify the letter on the left each round and add or substract that to the input letter and then increment our counters each round.

REDDYECIPHERISFASTANDEASYTOUSE = 17, 4, 3, 3, 24, 4, 2, 8, 15, 7, 4, 17, 8, 18, 5, 0, 18, 19, 0, 13, 3, 4, 0, 18, 24, 19, 14, 20, 18, 4

k[c] = (k[c] + k[c + 1 ] + j) % 26

j = (j + k[c] + c) % 26

sub = (k[c] + char) % 26  # sub becomes your output character.  to decrypt simply substract here instead of add

c = (c + 1) % 26

CIPHERTEXT = 6, 15, 6, 10, 4, 10, 13, 8, 19, 21, 3, 10, 18, 12, 2, 11, 2, 8, 15, 2, 14, 15, 22, 17, 20, 22, 9, 16, 10, 4, 25

or

CIPHERTEXT = GPGKEKNITVDKSMCLCIPCOPWRUWJQKEZ

# C crypt function usage

crypt(data, key, nonce, datalen);


# Python class usage
reddye = RedDye()

ctxt = reddye.crypt(data, key)
