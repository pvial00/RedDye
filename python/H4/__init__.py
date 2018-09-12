# RedDye Hand Cipher

class H4:
    def keysetup(self, key):
        k = [0] * len(key)
        j = 0
        for c, byte in enumerate(key):
            k[c] = (k[c] + (ord(byte) - 65)) % 26
            j = (j + (ord(byte) - 65)) % 26
        return k, j

    def encrypt(self, chars, key):
        ctxt = []
        c = 0
        k, j = self.keysetup(key)
        klen = len(k)
        for char in chars:
            k[c % klen] = (k[c % klen] + k[(c + 1) % klen] + j) % 26
            j = (j + k[c % klen] + c) % 26
            sub = (k[c % klen] + (ord(char) - 65)) % 26
            ctxt.append(chr(sub + 65))
            c = (c + 1) % 26
        return "".join(ctxt)
    
    def decrypt(self, chars, key):
        ctxt = []
        c = 0
        k, j = self.keysetup(key)
        klen = len(k)
        for char in chars:
            k[c % klen] = (k[c % klen] + k[(c + 1) % klen] + j) % 26
            j = (j + k[c % klen] + c) % 26
            sub = ((ord(char) - 65) - k[c % klen]) % 26
            ctxt.append(chr(sub + 65))
            c = (c + 1) % 26
        return "".join(ctxt)
