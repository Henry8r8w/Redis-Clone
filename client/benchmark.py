import socket
import time
import sys

HOST = '127.0.0.1'
PORT = 6379

def get_client():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(2) # 2 second timeout
    try:
        s.connect((HOST, PORT))
        return s
    except:
        return None
    
def test_ping(client):
      print("Testing: PING")
      client.sendall(b"*1\r\n$4\r\nPING\r\n") # default: aggregate
      print(f"Response:{client.recv(1024).decode().strip()}")

if __name__ == "__main__": 
    s = get_client()
    if s:
        test_ping(s)
    else:
        print("Could not connect")

