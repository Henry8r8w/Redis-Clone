import socket
import asyncio

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
      client.sendall(b"*1\r\n$4\r\nPING\r\n") 
      data = client.recv(1024)
      print(f"Received {data!r}")


def send_cmd(client, *args):
    buf = f"*{len(args)}\r\n"
    for arg in args:
        buf += f"${len(arg)}\r\n{arg}\r\n"
    client.sendall(buf.encode('utf-8'))
    

def set_get(client):
    print("Testing SET name Henry")
    send_cmd(client, "SET", "name", "Henry")
    data = client.recv(1024)
    print(f"Received {data!r}")
   
    print("Testing GET name")
    send_cmd(client, "GET", "name")
    data = client.recv(1024)
    print(f"Received {data!r}")


if __name__ == "__main__": 
    s = get_client()
    if s:
        test_ping(s)
        set_get(s)
    else:
        print("Could not connect")

