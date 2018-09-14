import java.io.*;
import java.lang.*;

public class RedDye {
    public int keylen = 32;
    public int[] k = new int[32];
    public int j = 0;
    void keysetup(byte[] key, byte[] nonce) {
        int c;
	for (c = 0; c < keylen; c++ ) {
	    k[c] = (k[c] + key[c]) & 0xff;
	    j = (j + k[c]) & 0xff;
	}
	for (c = 0; c < 256; c++) {
	    k[c % keylen] = (k[c % keylen] + j) & 0xff;
	    j = (j + k[c % keylen]) & 0xff;
	}
	for (c = 0; c < nonce.length; c++ ) {
	    k[c] = (k[c] + nonce[c]) & 0xff;
	    j = (j + k[c]) & 0xff;
	}
	for (c = 0; c < 256; c++) {
	    k[c % keylen] = (k[c % keylen] + j) & 0xff;
	    j = (j + k[c % keylen]) & 0xff;
	}
    }

    public byte[] crypt(byte[] data, byte[] key, byte[] nonce) {
        this.keysetup(key, nonce);
	int c = 0;
	for (int x = 0; x < data.length; x++) {
	    k[c % keylen] = (k[c % keylen] + k[(c + 1) % keylen] + j) & 0xff;
	    j = (j + k[c % keylen] + c) & 0xff;
	    data[x] = (byte)((int)data[x] ^ k[c % keylen]);
	    c = (c + 1) & 0xff;
	}
	return data;
    }
}
