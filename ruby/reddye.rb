class RedDye
    def keysetup(key, nonce)
	keylen = 32
	k = Array.new(keylen + 1, 0)
	j = 0
	for c in 0..(key.length - 2)
            k[c] = (k[c] + key[c].ord) % 256
	    j = (j + k[c]) % 256
	end
	for c in 0..255
	    k[c % keylen] = (k[c % keylen] + j) % 256
	    j = (j + k[c % keylen]) % 256
	end
	if nonce.length >= 8
	    for c in 0..(nonce.length - 1)
	        k[c] = (k[c] + nonce[c].ord) % 256
	        j = (j + k[c]) % 256
	    end
	    for c in 0..255
	        k[c % keylen] = (k[c % keylen] + j) % 256
	        j = (j + k[c % keylen]) % 256
	    end
	end
	return k, j
    end

    def crypt(data, key, nonce="")
        keylen = 32
	k, j = keysetup(key, nonce)
	c = 0
	ctxt = ""
	for i in 0..(data.length - 1)
	    k[c % keylen] = (k[c % keylen] + k[(c + 1) % keylen] + j) % 256
	    j = (j + k[c % keylen] + c) % 256
	    sub = data[i].ord ^ k[c % keylen]
	    ctxt += sub.chr
	    c = (c + 1) % 256
	end
	return ctxt
    end
    
    def kdf(key)
        keylen = 32
	iterations = 10
	if key.length < keylen
	    for i in 0..(keylen - (key.length - 1))
	        key += 0.chr
	    end
	end
	for i in 1..iterations
	    key = crypt(key, key)
	end
        return key
    end
end
