from socket import *
from math import *
from time import *
import struct

BIND_ADDRESS = 'localhost'
PORT = 1234 # confirmed with actual driver
BASE = 50
FREQUENCY = 265.0 # it is 250, but additional 15 hertz should compensate calculations time

def to_bytes(a,v):
    return a + struct.pack('!H', v)

s = socket(AF_INET, SOCK_STREAM)
s.bind((BIND_ADDRESS, PORT))
print 'listening on port ', PORT, ' of ', BIND_ADDRESS
s.listen(1)

try:
    while True:
        sc, addr = s.accept()
        try:
            print 'connection from ', addr
            arg = 0.00
            while 1:
                values = [
                    BASE,
                    int(BASE * abs(sin(arg))),
                    int(BASE * abs(cos(arg))),
                    int(BASE * abs(sin(pow(arg,2))))
                ]
                sc.send(reduce(to_bytes, values, ''))
                arg = arg + 0.001
                sleep(1.0 / FREQUENCY)
        except error:
            print 'connection to ', addr, ' closed'
        finally:
            sc.close()	
finally:
    s.close
