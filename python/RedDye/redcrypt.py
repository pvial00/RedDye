from RedDye import RedDye
import sys, select, getpass, os, time, getopt

nonce_len = 16

try:
    mode = sys.argv[1]
except IndexError as ier:
    print "Error: Did you forget encrypt/decrypt?"
    sys.exit(1)

input_filename = sys.argv[2]
output_filename = sys.argv[3]

try:
    infile = open(input_filename, "r")
except IOError as ier:
    print "Input file not found."
    sys.exit(1)

try:
    outfile = open(output_filename, "w")
except IOError as ier:
    print "Output file not found."
    sys.exit(1)

try:
    key = sys.argv[4]
except IndexError as ier:
    key = getpass.getpass("Enter key: ")

reddye = RedDye()
key = reddye.kdf(key)

start = time.time()
data = infile.read()
infile.close()

if mode == "encrypt":
    nonce = os.urandom(nonce_len)
    c = reddye.crypt(data, key, nonce)
    outfile.write(nonce+c)
elif mode == "decrypt":
    nonce = data[:nonce_len]
    msg = data[nonce_len:]
    plain_text = reddye.crypt(msg, key, nonce)
    outfile.write(plain_text)
outfile.close()

end = time.time() - start
bps = len(data) / end
sys.stdout.write("Completed in "+str(end)+" seconds\n")
sys.stdout.write(str(bps)+" bytes per second.\n")
