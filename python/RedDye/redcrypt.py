from reddye import RedDye
import sys, select, getpass, os, time, getopt

nonce_len = 16
buf_len = 65536

try:
    mode = sys.argv[1]
except IndexError as ier:
    print "Error: Did you forget encrypt/decrypt?"
    sys.exit(1)

input_file = sys.argv[2]
output_file = sys.argv[3]

fsize = os.path.getsize(input_file)
if fsize < buf_len:
    blocks = 0
else:
    blocks = fsize / buf_len
extra = fsize % buf_len
if extra != 0:
    blocks += 1

try:
    key = sys.argv[4]
except IndexError as ier:
    key = getpass.getpass("Enter key: ")

def keysetup(key, nonce):
    k = [0] * len(key)
    j = 0
    for c, char in enumerate(key):
        k[c] = (k[c] + ord(char)) & 0xff
        j = (j + k[c]) & 0xff
    klen = len(k)
    for c in range(256):
        k[c % klen] = (k[c % klen] + j) & 0xff
        j = (j + k[c % klen]) & 0xff
    if nonce != "":
        for c, char in enumerate(nonce):
            k[c] = (k[c] + ord(char)) & 0xff
            j = (j + k[c]) & 0xff
        for c in range(256):
            k[c % klen] = (k[c % klen] + j) & 0xff
            j = (j + k[c % klen]) & 0xff
    return k, j

def encrypt(in_file, out_file, fsize, key):
    infile = open(in_file, "r")
    outfile = open(out_file, "w")
    nonce = os.urandom(nonce_len)
    outfile.write(nonce)
    start = time.time()
    k, j = keysetup(key, nonce)
    klen = len(k)
    c = 0
    for x in range(blocks):
        ctxt = []
        block = infile.read(buf_len)
        for b in block:
            k[c % klen] = (k[c % klen] + k[(c + 1) % klen] + j) & 0xff
            j = (j + k[c % klen] + c) & 0xff
            sub = ((ord(b)) ^ k[c % klen])
            ctxt.append(chr(sub))
            c = (c + 1) & 0xff
        outfile.write("".join(ctxt))
    infile.close()
    outfile.close()
    end = time.time() - start
    bps = fsize / end
    sys.stdout.write("Completed in "+str(end)+" seconds\n")
    sys.stdout.write(str(bps)+" bytes per second.\n")

def decrypt(in_file, out_file, fsize, key):
    infile = open(in_file, "r")
    outfile = open(out_file, "w")
    nonce = infile.read(nonce_len)
    start = time.time()
    k, j = keysetup(key, nonce)
    klen = len(k)
    c = 0
    for x in range(blocks):
        ctxt = []
        block = infile.read(buf_len)
        for b in block:
            k[c % klen] = (k[c % klen] + k[(c + 1) % klen] + j) & 0xff
            j = (j + k[c % klen] + c) & 0xff
            sub = ((ord(b)) ^ k[c % klen])
            ctxt.append(chr(sub))
            c = (c + 1) & 0xff
        outfile.write("".join(ctxt))
    infile.close()
    outfile.close()
    end = time.time() - start
    bps = fsize / end
    sys.stdout.write("Completed in "+str(end)+" seconds\n")
    sys.stdout.write(str(bps)+" bytes per second.\n")
            
            
key = RedDye().kdf(key)

if mode == "encrypt":
    encrypt(input_file, output_file, fsize, key)
elif mode == "decrypt":
    decrypt(input_file, output_file, (fsize - nonce_len), key)
