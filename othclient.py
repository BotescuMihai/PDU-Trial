import socket
import sys
import random
from ctypes import *

class msgIntType(Structure):
    _fields_ = [
        ("msg",c_uint32)
    ]

def main():
    server_addr = ('127.0.0.1', 18081)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        s.connect(server_addr)
        print("Connected to {:s}".format(repr(server_addr)))

        for i in range(1):
            print("")
            payload_out = msgIntType(1)
            print("Sending id={:d}".format(payload_out.msg))
            nsent = s.send(payload_out)
            # Alternative: s.sendall(...): coontinues to send data until either
            # all data has been sent or an error occurs. No return value.
            print("Sent {:d} bytes".format(nsent))
            buff = s.recv(sizeof(msgIntType))
            print(buff)
            payload_in = msgIntType.from_buffer_copy(buff)
       #     print(payload_in)
            print("Received id={:d}".format(payload_in.msg))
    except AttributeError as ae:
        print("Error creating the socket: {}".format(ae))
    except socket.error as se:
        print("Exception on socket: {}".format(se))
    finally:
        print("Closing socket")
        s.close()


if __name__ == "__main__":
    main()