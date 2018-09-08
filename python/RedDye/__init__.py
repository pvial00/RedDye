class RedDye:
    keylen = 32
    def keysetup(self, key, nonce):
        k = [0] * self.keylen
        j = 0
        for c, char in enumerate(key):
            k[c] = (k[c] + ord(char)) % 256
            j = (j + k[c]) % 256
        klen = len(k)
        for c in range(256):
            k[c % klen] = (k[c % klen] + j) % 256
            j = (j + k[c % klen]) % 256
        if nonce != "":
            for c, char in enumerate(nonce):
                k[c] = (k[c] + ord(char)) % 256
                j = (j + k[c]) % 256
            for c in range(256):
                k[c % klen] = (k[c % klen] + j) % 256
                j = (j + k[c % klen]) % 256
        return k, j

    def crypt(self, chars, key, nonce=""):
        ctxt = []
        k, j = self.keysetup(key, nonce)
        klen = len(k)
        c = 0
        i = 1
        for char in chars:
            k[c % klen] = (k[c % klen] + k[i] + j) % 256
            j = (j + k[c % klen] + c) % 256
            sub = ((ord(char)) ^ k[c % klen]) % 256
            ctxt.append(chr(sub))
            i = (i + 1) % klen
            c = (c + 1) % 256
        return "".join(ctxt)
    
    def kdf(self, key, iterations=10):
        if len(key) < self.keylen:
            for x in range(self.keylen - len(key)):
                key += chr(0)
        for i in range(iterations):
            key = self.crypt(key, key)
        return key
