
import socket
import struct
from math import sin, pi

opened_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send(data):
    data = bytes(data)
    opened_socket.sendto(data, ("192.168.1.20", 1234))

def set_r(buf):
    buf = [int(x) for x in buf]
    send([ord("r"), 0, 0] + buf)

def set_g(buf):
    buf = [int(x) for x in buf]
    send([ord("g"), 0, 0] + buf)

def set_b(buf):
    buf = [int(x) for x in buf]
    send([ord("b"), 0, 0] + buf)

def config(buf):
    buf = [int(x) for x in buf]
    send([ord("b"), 0, 0] + buf)

rs = [((sin(2 * pi * (t +   0) / 1024) + 1.0) / 2) * 255 for t in range(1024)]
gs = [((sin(2 * pi * (t + 300) / 1024) + 1.0) / 2) * 255 for t in range(1024)]
bs = [((sin(2 * pi * (t + 600) / 1024) + 1.0) / 2) * 255 for t in range(1024)]

#rs = [255 for t in range(1024)]
#gs = [0 for t in range(1024)]
#bs = [0 for t in range(1024)]

set_r(rs)
set_g(gs)
set_b(bs)
