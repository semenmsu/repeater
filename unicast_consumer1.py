import socket

host = "127.0.0.1"
port = 15001
sock = socket.socket(socket.AF_INET, # Internet
                              socket.SOCK_DGRAM) # UDP
sock.bind((host, port))
while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print("received message: %s" % data)