# RedDye Stream Cipher

*** Warning this cipher has not undergone heavy cryptanalysis and should be used in production systems

*** RedDye has been undergone a slight refit to deal with known plaintext attack situations

Authored by Karl Zander

Passes Dieharder randomness tests

High speed stream cipher that uses a primarily 256 bit key (optionally 128, 512, 1024 and 2048) to achieve ultimate security at the highest speeds.  Originally designed to beat Solitaire as the fastest, most secure hand cipher, RedDye offers more flexibility because it can be applied to binary data as implemented here.  RedDye26 offers a reduced version operating on A-Z characters.

RedDye performance is faster than that of ASM implementations AES and ChaCha20 and RC4.

RedDye's construction begins with a 256-bit (32 byte) key and nonce that are combined to form the array k[].  j is a value that is a rolling sum total of all calulated k[] values mod 256.  j is passed on to the encryption function.  k[] is processed two bytes at a time through the encryption algorithm.  The encryption algorithm outputs one byte at a time.  RedDye maintains two counters, c and i.  i is used only to iterate over k[].  c is used as an 8-bit counter which is combined with j to maintain uniformity.

RedDye operates under the assumption that one cannot easily revert the encryption formula below and find k which is XOR'ed with the plaintext byte.

k[i] = (k[i] + k[i + 1] + j) % 256

j = (j + k[i] + c) % 256

output = ((j + k[i]) % 256) ^ k[i]

Recommended nonce length is 64-128bits.

# Authentication

RedDye is paired with a RedDye based authenticator called H4A.  It takes the message data and a key and generates a 128 bit authentication code that can be attached to the message to ensure the message has not been tampered with.  H4A relies on the strength of the RedDye cipher.

# Cryptanalysis
In a known plaintext attack situation the attack has no specific advantage knowing n bytes of the plaintext.

There is no known bias in a ciphertext only attack and the output isn't distinguishable from a random output.

# RedDye Hand Cipher

*** Coded in Python as the RedDye H4 Cipher

First one assigns a key to be used of desired length.  For this example we will use the key MYTESTINGKEY.  We convert those letter to numbers using the key that A = 0 and Z = 25.

MESSAGE = REDDYECIPHERISFASTANDEASYTOUSE

KEY = MYTESTINGKEY

MYTESTINGKEY = 12, 24, 19, 4, 18, 19, 8, 13, 6, 10, 4, 24

Then we sum all letters in the key mod 26 and designate that number as j.  In this case j = 5.

We then convert the plaintext letters into numbers and apply the following formula. c is a counter that begins with the first letter in the key.  We always modify the letter on the left each round and add or substract that to the input letter and then increment our counters each round.

REDDYECIPHERISFASTANDEASYTOUSE = 17, 4, 3, 3, 24, 4, 2, 8, 15, 7, 4, 17, 8, 18, 5, 0, 18, 19, 0, 13, 3, 4, 0, 18, 24, 19, 14, 20, 18, 4

k[c] = (k[c] + k[c + 1 ] + j) % 26

j = (j + k[c] + c) % 26

output = (((j + k[c]) % 26) + k[c]) % 26

sub = (output + char) % 26  # sub becomes your output character.  to decrypt simply substract here instead of add

c = (c + 1) % 26

CIPHERTEXT = 15, 6, 20, 12, 15, 6, 5, 22, 23, 6, 8, 13, 8, 19, 23, 20, 10, 1, 15, 10, 23, 4, 14, 8, 23, 13, 15, 25, 4, 4, 14

or

CIPHERTEXT = PGUMPGFWXGINITXUKBPKXEOIXNPZEEO

# C crypt function usage

crypt(data, key, nonce, datalen);


# Python class usage
reddye = RedDye()

ctxt = reddye.crypt(data, key)
