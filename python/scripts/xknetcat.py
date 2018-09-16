# Requires libreddye.so to be built and available
# and requires the python RedDye module
# xknetcat.py allows encrypted data transfer between systems using the RedDye cipher

import socket
import sys, getopt, os
import select, signal
from reddye import RedDye
from ctypes import *
cdll.LoadLibrary("libreddye.so")
libreddye = CDLL("libreddye.so")

server = False
port = 6969
host = "0.0.0.0"
recv_size = 65536
noncelen = 16
kdf_iters = 10
#stoprequest = threading.Event()
rt = ""

def encrypt(data, key):
    nonce = os.urandom(noncelen)
    cdll.LoadLibrary("libreddye.so")
    libreddye = CDLL("libreddye.so")
    libreddye.crypt(data, key, nonce, len(data))
    return nonce+data

def decrypt(data, key):
    nonce = data[:noncelen]
    msg = data[noncelen:]
    libreddye.crypt(msg, key, nonce, len(msg))
    return msg

def usage():
    sys.stdout.write("Server mode usage: knetcat -l <ip address> <port>\n")
    sys.stdout.write("Client mode usage: knetcat <ip address> <port>\n")

def signal_handler(signal, frame):
    #stoprequest.set()
    #rt.join(stoprequest)
    sys.exit(0)

def serverrun(host, port, key):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((host, port))
    s.listen(5)
    if select.select([sys.stdin,],[],[],0.0)[0]:
        c, addr = s.accept()
        data = sys.stdin.read()
        data = encrypt(data, key)
        c.send(data)
    else:
        c, addr = s.accept()
        buf = []
        while True:
            data = c.recv(recv_size)
            if not data: break
            buf.append(data)
        data = decrypt("".join(buf), key)
        sys.stdout.write(data)
    try:
        c.shutdown(1)
    except socket.error as ser:
        sys.stdout.write("Socket error, continuing\n")
    c.close()

def recv_thread(s):
    while not stoprequest.isSet():
        data = s.recv(recv_size)
        sys.stdout.write(data)
        sleep(0.4)

def clientrun(host, port, key):
    if select.select([sys.stdin,],[],[],0.0)[0]:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s = socket.socket()
        s.connect((host, port))
        data = sys.stdin.read()
        data = encrypt(data, key)
        s.send(data)
        s.shutdown(1)
        s.close()
    elif not sys.stdout.isatty():
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s = socket.socket()
        s.connect((host, port))
        buf = []
        while True:
            data = s.recv(recv_size)
            if not data: break
            buf.append(data)
        data = decrypt("".join(buf), key)
        sys.stdout.write(data)
        s.shutdown(1)
        s.close()
    elif sys.stdout.isatty():
        buf = ""
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s = socket.socket()
        s.connect((host, port))
        #rt = threading.Thread(target=recv_thread, args=(s,)).start()
        while True:
            d = select.select([s], [], [], 2)
            if d[0]:
                buf = []
                while True:
                    data = s.recv(recv_size)
                    if not data: break
                    buf.append(data)
                data = decrypt("".join(buf), key)
                sys.stdout.write(data)
                break
            buf = sys.stdin.readline()
            if buf != "":
                buf = encrypt(buf, key)
                s.send(buf)
        s.close()

signal.signal(signal.SIGINT, signal_handler)
argv = sys.argv[1:]
if len(argv) == 4:
    host = sys.argv[2]
    port = int(sys.argv[3])
    key = sys.argv[4]
    key = RedDye().kdf(key, kdf_iters)
    serverrun(host, port, key)
elif len(argv) == 3:
    host = sys.argv[1]
    port = int(sys.argv[2])
    key = sys.argv[3]
    key = RedDye().kdf(key, kdf_iters)
    clientrun(host, port, key)
else:
    usage()
    exit(0)
    
if '-h' in sys.argv[1:]:
    usage()
    exit(0)
