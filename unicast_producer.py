import socket
import time

host = "127.0.0.1"
port = 15001
message = "Hello, World!"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

count = 0
while True:
    sock.sendto(bytes(message+" "+str(count), 'utf-8'), (host, port))
    count +=1
    time.sleep(1)