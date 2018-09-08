class RedDye:
    keylen = 26
    def keysetup(self, key):
        k = [0] * self.keylen
        j = 0
        for c, byte in enumerate(key):
            k[c] = (k[c] + (ord(byte) - 65)) % 26
            j = (j + (ord(byte) - 65)) % 26
        klen = len(k)
        for c in range(256):
            k[c % klen] = (k[c % klen] + j) % 26
            j = (j + k[c % klen]) % 26
        return k, j

    def encrypt(self, chars, key):
        ctxt = []
        c = 0
        i = 1
        k, j = self.keysetup(key)
        for char in chars:
            k[c] = (k[c] + k[i] + j) % 26
            j = (j + k[c] + c) % 26
            sub = (k[c] + (ord(char) - 65)) % 26
            ctxt.append(chr(sub + 65))
            i = (i + 1) % 26
            c = (c + 1) % 26
        return "".join(ctxt)
    
    def decrypt(self, chars, key):
        ctxt = []
        c = 0
        i = 1
        k, j = self.keysetup(key)
        for char in chars:
            k[c] = (k[c] + k[i] + j) % 26
            j = (j + k[c] + c) % 26
            sub = ((ord(char) - 65) - k[c]) % 26
            ctxt.append(chr(sub + 65))
            i = (i + 1) % 26
            c = (c + 1) % 26
        return "".join(ctxt)
